
; Semispaces
;
; What about continuations?  Do we want to keep track to help in detecting
; unbounded recursions?

(define-record-type semispace :semispace
  (real-make-semispace mixed small large weaks)
  (mixed area semispace-mixed set-semispace-mixed!)
  (small area semispace-small set-semispace-small!)
  (large area semispace-large set-semispace-large!)
  (weaks area semispace-weaks set-semispace-weaks!))

(define (make-semispace)
  (real-make-semispace (null-pointer)
		       (null-pointer)
		       (null-pointer)
		       (null-pointer)))

(define *current-semispace*)
(define *other-semispace*)

(define (initialize)
  (set! *current-semispace* (make-semispace))
  (set! *other-semispace* (make-semispace)))

(define (s48-collect)
  (let ((temp *other-semispace*))
    (set! *other-semispace* *current-semispace*)
    (set! *current-semispace* temp)
    (collect *other-semispace* *current-semispace*)))

(define (collect from-semispace to-semispace)
  (set-gc-actions! from-semispace
		   (enum gc-action copy-mixed)
		   (enum gc-action copy-small)
		   (enum gc-action mark-large)
		   (enum gc-action copy-weak))
  (set-gc-actions! to-semispace
		   (enum gc-action error)
		   (enum gc-action error)
		   (enum gc-action error)
		   (enum gc-action error))
  (set! *to-space* to-semispace)
  (set! *from-space* from-semispace)
  (trace-static-areas)		; this should just be another root
  (s48-gc-root)			; trace the interpreter's roots
  (do-gc to-semispace)
  (clean-weak-pointers (semispace-weaks to-semispace))
  (s48-post-gc-cleanup)		; interpreter's post-gc clean up
  (set! *gc-count* (+ *gc-count* 1)))

(define *gc-count* 0)

(define (s48-gc-count)
  *gc-count*)

(define *from-space*)
(define *to-space*)

(define (set-gc-actions! semispace mixed small large weaks)
  (set-gc-action! (semispace-mixed semispace) mixed)
  (set-gc-action! (semispace-small semispace) small)
  (set-gc-action! (semispace-large semispace) large)
  (set-gc-action! (semispace-weaks semispace) weaks))

(define (set-gc-action! area action)
  (for-each-area (lambda (area)
		   (set-area-gc-action! area action))
		 area))

(define (for-each-area proc area)
  (let loop ((area area))
    (cond ((not (null-pointer? area))
	   (proc area)
	   (loop (area-next area))))))

; The LET* is to avoid introducing differences between one build and the next.

(define (do-gc semispace)
  (let loop ()
    (let* ((hit0 (trace-areas (semispace-mixed semispace)))
	   (hit1 (trace-areas (semispace-small semispace)))
	   (hit2 (trace-areas (semispace-large semispace))))
      (if (or hit0 hit1 hit1)
	  (loop)))))
  
(define (trace-areas area)
  (let area-loop ((area area) (hit? #f))
    (if (null-pointer? area)
	hit?
	(let loop ((hit? hit?))
	  (let ((start (area-trace area))
		(end   (area-frontier  area)))
	    (cond ((address= start end)
		   (area-loop (area-next area) hit?))
		  (else
		   (s48-trace-locations! start end)
		   (loop #t))))))))

(define trace-value-location (allocate-memory (cells->bytes 1)))

(define (s48-trace-value stob)
  (store! trace-value-location stob)
  (s48-trace-locations! trace-value-location
			(address1+ trace-value-location))
  (fetch trace-value-location))

(define (s48-trace-stob-contents! stob)
  (let ((start (address-after-header stob))
	(size (bytes->a-units (header-length-in-bytes (stob-header stob)))))
    (s48-trace-locations! start (address+ start size))))

;----------------------------------------------------------------
;
; Copy everything pointed to from somewhere between START (inclusive)
; and END (exclusive).
;
; This is written using internal defines because the Pre-Scheme compiler
; doesn't check for sets of mutually-recursive top-level procedures.  It
; should.

(define (s48-trace-locations! start end)
  (let loop ((addr start))
    (if (address< addr end)
	(let ((thing (fetch addr))
	      (next (address1+ addr)))

	  (define (trace)
	    (cond ((header? thing)
		   (loop (if (b-vector-header? thing)
			     (address+ next (header-length-in-a-units thing))
			     next)))
		  ((not (stob? thing))
		   (loop next))
		  (else
		   (trace-stob thing))))

	  (define (trace-stob stob)
	    (let-syntax ((ekg (syntax-rules ()
				((ekg (header) action)
				 (let ((header (stob-header stob)))
				   (cond ((stob? header) ;***Broken heart
					  (store! addr header)
					  (loop next))
					 (else
					  action)))))))
	      (let ((area (memory-map-ref (address-at-header stob))))
		(if (not (null-pointer? area))
		    (enum-case gc-action (area-gc-action area)
		      ((copy-small)
		       (ekg (header) (copy-small header stob)))
		      ((copy-mixed)
		       (ekg (header) (copy-mixed header stob)))
		      ((copy-weak)
		       (ekg (header) (copy-weak header stob)))
		      ((ignore)
		       (loop next))
		      ((mark-large)
		       (mark-large area)
		       (loop next))
		      ((error)
		       (error "got error gc-action"))
		      (else
		       (error "got unexpected gc-action")))))))

	  (define (copy-small header thing)
	    (copy-object header
			 thing 
			 (let ((area (semispace-small *to-space*)))
			   (if (< (header-length-in-a-units header)
				  (area-remaining area))
			       area
			       (allocate-small-area)))))

	  (define (copy-mixed thing)
	    (let ((header (stob-header thing)))
	      (cond ((stob? header)            ;***Broken heart
		     header)
		    ((vm-eq? weak-pointer-header h)
		     (copy-weak-pointer thing))
		    ((<= (header-length-in-bytes header)
			 small-object-limit)
		     (copy-small header thing))
		    (else
		     (copy-large header thing)))))

	  (define (copy-large header thing)
	    (copy-object header
			 thing
			 (allocate-large-area (+ (header-length-in-bytes header)
						 (cells->bytes stob-overhead)))))

	  (define (copy-weak-pointer thing)
	    (copy-object weak-pointer-header
			 thing 
			 (let ((area (semispace-weaks *to-space*)))
			   (if (< (header-length-in-a-units weak-pointer-header)
				  (area-remaining area))
			       area
			       (allocate-weak-area)))))

	  (define (copy-object header thing area)
	    (let ((frontier (area-frontier area)))
	      (store! frontier header)
	      (let* ((data-addr (address+ frontier
					  (cells->a-units stob-overhead)))
		     (new (address->stob-descriptor data-addr)))
		(stob-header-set! thing new) ;***Break heart
		(copy-memory! (address-after-header thing)
			      data-addr
			      (header-length-in-bytes header))
		(set-area-frontier! area
				    (address+ data-addr
					      (header-length-in-a-units h)))
		(store! addr new)
		(loop next))))
	  (trace))))
  0)  ; for the type-checker

; Remove AREA from from-space's list and put it on to-space's.  Ignore
; AREA from now on.

(define (mark-large area)
  (set-semispace-large! *from-space*
			(delete-area! (semispace-large *from-space*)
				      area))
  (set-area-next! area (semispace-large *to-space*))
  (set-semispace-large! *to-space* area)
  (set-area-gc-action! area (enum gc-action ignore))
  (set-area-trace! area (area-start area)))

; Either not a stored object, or it has a forwarding pointer, or it is
; in an area that is not currently being collected.

(define (s48-extant? thing)
  (or (not (stob? thing))
      (stob? (stob-header thing))
      (let ((area (memory-map-ref (address-at-header thing))))
	(or (null-pointer? area)	; what would this mean?
	    (= (area-gc-action area)
	       (enum gc-action ignore))))))

(define (clean-weak-pointers areas)
  (for-each-area (lambda (area)
		   (let ((end (area-frontier area)))
		     (let loop ((addr (address1+ (area-start area))))
		       (if (address< addr end)
			   (let ((contents (fetch addr)))
			     (if (not (s48-extant? contents))
				 (store addr false))
			     (loop (address+ addr (cells->a-units 2))))))))
		 areas))

(define weak-pointer-header
  (make-header (enum stob weak-pointer) (cells->bytes (- weak-pointer-size 1))))

;----------------------------------------------------------------

; These numbers are just guesses.

(define small-object-limit (pages->bytes 4))
(define minimum-small-area-size 16)			; pages
(define maximum-small-area-size 32)			; pages

(define (allocate-small-area)
  (let ((area (allocate-area minimum-small-area-size maximum-small-area-size)))
    (set-area-gc-action! area (enum gc-action error))
    (set-area-next! area (semispace-small *current-semispace*))
    (set-semispace-small! *current-semispace* area)
    area))

(define (allocate-large-area size-in-bytes)
  (let* ((pages (bytes->pages size-in-bytes))
	 (area (allocate-area pages pages)))
    (set-area-gc-action! area (enum gc-action error))
    (set-area-next! area (semispace-large *current-semispace*))
    (set-semispace-large! *current-semispace* area)
    area))

(define (allocate-weak-area)
  (let ((area (allocate-area 2 4)))
    (set-area-gc-action! area (enum gc-action error))
    (set-area-next! area (semispace-weaks *current-semispace*))
    (set-semispace-weaks! *current-semispace* area)
    area))

