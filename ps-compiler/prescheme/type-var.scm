; Copyright (c) 1994 Richard Kelsey.  See file COPYING.


; Type variables - what a mess

(define-record-type uvar
  (prefix        ; a name for debugging
   (depth)       ; lexical depth of the uvar
   id            ; a number
   (arith-op?)   ; true if this corresponds to an arithmetic operation (set
                 ;  when uvars are merged)
   (tuple-okay?) ; true if this can be unified with a tuple, set when merged
   )
  ((place        #f)  ; used in producing type schemes
   (source       #f)  ; to let the user know where this came from

   (binding      #f)  ; known value of this uvar

   (constraint-count 0)  ; number of constraints
   (sinks       '())  ; where our value goes
   (sources     '())  ; where we get our values

   (temp         #f)  ; useful field
   ))

(define-record-discloser type/uvar
  (lambda (uvar)
    (list 'uvar
	  (uvar-prefix uvar)
	  (uvar-depth uvar)
	  (uvar-id uvar)
	  (uvar-arith-op? uvar)
	  (uvar-constraint-count uvar)
	  (uvar-binding uvar))))

(define (make-uvar prefix depth . maybe-id)
  (uvar-maker prefix
	      depth
	      (if (null? maybe-id)
		  (unique-id)
		  (car maybe-id))
	      #f
	      #f))

(define (make-arith-op-uvar prefix depth . maybe-id)
  (uvar-maker prefix
	      depth
	      (if (null? maybe-id)
		  (unique-id)
		  (car maybe-id))
	      #t
	      #f))

(define (make-tuple-uvar prefix depth . maybe-id)
  (uvar-maker prefix
	      depth
	      (if (null? maybe-id)
		  (unique-id)
		  (car maybe-id))
	      #f
	      #t))

(define *constrained-uvars* '())  ; list of all constrained uvars

(define (constrained-uvars)
  (let ((cus (filter uvar-constrained? *constrained-uvars*)))
    (set! *constrained-uvars* cus)
    cus))

(define (add-to-constraint-count! uvar constraints)
  (let ((count (uvar-constraint-count uvar)))
    (if (= count 0)
	(set! *constrained-uvars* (cons uvar *constrained-uvars*)))
    (set-uvar-constraint-count! uvar (+ count constraints))))

(define (uvar-constrained? uvar)
  (> (uvar-constraint-count uvar) 0))

(define-record-type relation
  ((source)    ; sources     (uvar or type)
   (sink)      ; destination (uvar or type)
   site        ; where to put the coercion
   )
  (temp))      ; useful slot

(define make-relation relation-maker)

(define-record-discloser type/relation
  (lambda (relation)
    (list (relation-source relation)
	  '=>
	  (relation-sink relation))))

(define (add-relation source sink site)
  (let ((new (make-relation source sink site)))
    (cond ((uvar? source)
	   (set-uvar-sinks! source (cons new (uvar-sinks source)))
	   (add-to-constraint-count! source 1)))
    (cond ((uvar? sink)
	   (set-uvar-sources! sink (cons new (uvar-sources sink)))
	   (add-to-constraint-count! sink 1)))
    new))
    
; Return a type greater or equal to TYPE, which is the type of SITE.

(define (make-constrained-type prefix type depth site)
  (let ((type (maybe-follow-uvar type)))
    (if (and site
	     (or (uvar? type)
		 (lattice-type? type)))
	(let ((new (if (and (uvar? type)
			    (uvar-tuple-okay? type))
		       (make-tuple-uvar prefix depth)
		       (make-uvar prefix depth))))
	  (add-relation type new site)
	  new)
	type)))

; Could this safely short-circuit the chains?

(define (maybe-follow-uvar type)
  (cond ((and (uvar? type)
	      (uvar-binding type))
	 => maybe-follow-uvar)
	(else type)))

; Substitute VALUE for UVAR, if this will not introduce a circularity.
; or cause other problems.  Returns an error-printing thunk if there is
; a problem.

(define (bind-uvar! uvar value)
  (cond ((uvar? value)
	 (if (> (uvar-constraint-count uvar)
		(uvar-constraint-count value))
	     (bind-uvar-to-uvar! value uvar)
	     (bind-uvar-to-uvar! uvar value))
	 #f)
	((lattice-type? value)
	 (set-uvar-binding! uvar value)
	 (for-each (lambda (relation)
		     (set-relation-source! relation value))
		   (uvar-sinks uvar))
	 (for-each (lambda (relation)
		     (set-relation-sink! relation value))
		   (uvar-sources uvar))
	 #f)
	(else
	 (bind-uvar-to-type! uvar value))))

(define (bind-uvar-to-uvar! uvar0 uvar1)
  (minimize-type-depth! uvar1 (uvar-depth uvar0))
  (set-uvar-binding! uvar0 uvar1)
  (if (uvar-constrained? uvar1)
      (merge-constraints! uvar0 uvar1))
  (if (and (uvar-arith-op? uvar0)
	   (not (uvar-arith-op? uvar1)))
      (set-uvar-arith-op?! uvar1 #t))
  (if (and (uvar-tuple-okay? uvar1)
	   (not (uvar-tuple-okay? uvar0)))
      (set-uvar-tuple-okay?! uvar1 #f)))

(define (bind-uvar-to-type! uvar type)
  (receive (uvars types depth)
      (find-constraints uvar)
    (let ((errors '())
	  (mark (cons 1 2)))
      (for-each (lambda (uvar)
		  (set-uvar-temp! uvar mark)
		  (if (uvar-arith-op? uvar)
		      (set! errors (cons (arith-op-error uvar) errors))))
		uvars)
      (if (marked-uvar? type mark)
	  (set! errors (cons circularity-error errors)))
      (if (and (tuple-type? type)
	       (not (every? uvar-tuple-okay? uvars)))
	  (set! errors (cons (tuple-error type) errors)))
      (if (not (null? types))
	  (set! errors (cons (arithmetic-mismatch-error (car types)) errors)))
      (cond ((null? errors)  ; whew!
	     (minimize-type-depth! type depth)
	     (for-each (lambda (uvar)
			 (set-uvar-binding! uvar type))
		       uvars)
	     #f)
	    (else
	     (lambda ()
	       (format #t "unifying ")
	       (display-type uvar (current-output-port))
	       (format #t " == ")
	       (display-type type (current-output-port))
	       (format #t "~% would cause the following problem~A:"
		       (if (null? (cdr errors)) "" "s"))
	       (for-each (lambda (x) (x)) errors)))))))
		       
(define (circularity-error)
  (format #t "~% creation of a circular type"))

(define (tuple-error type)
  (lambda ()
    (if (null? (tuple-type-types type))
	(format #t "~% returning no values where one is expected")
	(format #t "~% returning ~D values where one is expected"
		(length (tuple-type-types type))))))

(define (arith-op-error uvar)
  (lambda ()
    (format #t "~% unifying a non-arithmetic with an argument or result of ~S"
	    (uvar-prefix uvar))))
     
(define (arithmetic-mismatch-error arith-type)
  (lambda ()
    (format #t "~% type mismatch with ")
    (display-type arith-type (current-output-port))))

; Check that UVAR does not occur in EXP.

(define (marked-uvar? exp marker)
  (let label ((exp exp))
    (cond ((or (base-type? exp)
	       (record-type? exp))
	   #f)
	  ((uvar? exp)
	   (if (uvar-binding exp)
	       (label (uvar-binding exp))
	       (eq? marker (uvar-temp exp))))
	  ((other-type? exp)
	   (every? label (other-type-subtypes exp)))
	  (else
	   (identity (bug "funny type ~S" exp))))))
  
; Make the depths of any uvars in TYPE be no greater than DEPTH.

(define (minimize-type-depth! type depth)
  (let label ((type type))
    (cond ((other-type? type)
	   (for-each label (other-type-subtypes type)))
	  ((uvar? type)
	   (cond ((uvar-binding type)
		  => label)
		 ((< depth (uvar-depth type))
		  (set-uvar-depth! type depth)))))))

; Merge the constraints of UVAR into those of uvar VALUE.

(define (merge-constraints! uvar value)
  (add-to-constraint-count! value (uvar-constraint-count uvar))
  (for-each (lambda (relation)
	      (set-relation-source! relation value))
	    (uvar-sinks uvar))
  (set-uvar-sinks! value
		    (append (uvar-sinks uvar)
			    (uvar-sinks value)))
  (for-each (lambda (relation)
	      (set-relation-sink! relation value))
	    (uvar-sources uvar))
  (set-uvar-sources! value
		  (append (uvar-sources uvar)
			  (uvar-sources value))))

(define (find-constraints initial-uvar)
  (let ((uvars '())
	(types '())
	(depth (uvar-depth initial-uvar))
	(key (cons 1 2)))
    
    (define (start)  ; how it all begins
      (do-uvar initial-uvar)
      (values uvars types depth))
    
    (define (do-uvar uvar)
      (cond ((uvar-binding uvar)
	     (do-type (uvar-binding uvar)))
	    ((eq? key (uvar-temp uvar))
	     (values))
	    (else
	     (set-uvar-temp! uvar key)
	     (if (< (uvar-depth uvar) depth)
		 (set! depth (uvar-depth uvar)))
	     (set! uvars (cons uvar uvars))
	     (for-each do-relation-source (uvar-sources uvar))
	     (for-each do-relation-sink (uvar-sinks uvar)))))
    
    (define (do-relation-source relation)
      (do-type (relation-source relation)))
    (define (do-relation-sink relation)
      (do-type (relation-sink relation)))
    
    (define (do-type type)
      (if (uvar? type)
	  (do-uvar type)
	  (set! types (cons type types))))
    
    (start)))

; Set the depth of all uvars in TYPE to be -1 so that it will not be made
; polymorphic at any level.

(define (make-nonpolymorphic! type)
  (cond ((uvar? type)
	 (set-uvar-depth! type -1))
	((other-type? type)
	 (for-each make-nonpolymorphic! (other-type-subtypes type)))
	;((type-scheme? type)
	; (make-nonpolymorphic! (type-scheme-type type)))
	))

;------------------------------------------------------------
; Micro utilities

(define *unique-id-counter* 0)

(define (unique-id)
  (set! *unique-id-counter* (+ *unique-id-counter* 1))
  *unique-id-counter*)

(define (reset-type-vars!)
  (set! *unique-id-counter* 0)
  (set! *constrained-uvars* '()))
