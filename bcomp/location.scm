; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; Locations

(define *location-uid* 2000)		;Set to 0 by reset-compiler-state!
(define location-name-table (make-table))
(define (flush-location-names)
  (set! location-name-table (make-table))
  (set! package-name-table (make-table))) ;hmm

(define (make-new-location p name)
  (let ((uid (new-location-uid)))
    (table-set! location-name-table uid
		(cons (name->symbol name) (package-uid p))) ;?
    (make-undefined-location uid)))

(define (new-location-uid)
  (let ((uid *location-uid*))
    (set! *location-uid* (+ *location-uid* 1))
    uid))

(define (location-info loc)
  (let ((id (location-id loc)))
    (if (integer? id)
	(table-ref location-name-table id)
	#f)))

(define (copy-location-info! from to)
  (let ((probe (location-info from)))
    (if probe
	(table-set! location-name-table (location-id to) probe))))

(define (location-name loc)
  (let ((probe (location-info loc)))
    (if probe
	(car probe)
	#f)))

(define (location-package-name loc)
  (let ((probe (location-info loc)))
    (if probe
	(table-ref package-name-table (cdr probe))
	#f)))


; --------------------
; Location-level operations

(define (copy-shadowed-contents! import loc)
  (if (and (location-defined? import)
	   (location-assigned? import)
	   (not (location-assigned? loc)))
      (set-contents! loc (contents import))))

; Get a location for a variable.  This is called by someone who plans
; to squirrel it away somewhere, so we'd better remember that.

(define (package-lookup-location p name assign?)
  (cond ((table-ref (package-definitions p) name)
	 (get-location p name assign?))
	((generated? name)
	 (package-lookup-location (generated-env name) (generated-symbol name) assign?))
	(else
	 (exporting-package p name
	   (lambda (q type)
	     (cond (assign?
		    (warn "SET! of an imported variable"
			  name q p)
		    (get-location p name #t)) ;wrong
		   ((eq? type 'syntax)
		    ;; (warn "syntax referenced as variable" ...)
		    (get-location p name #f))
		   (else
		    (lookup-imported-location q name p))))
	   (lambda () (get-location p name #f))))))

(define (lookup-imported-location p name cache?)
  (let ((loc
	 (if (table-ref (package-definitions p) name)
	     (get-location p name #f)
	     (exporting-package p name
	       (lambda (q type)
		 (lookup-imported-location q name (if cache? p #f)))
	       (lambda ()
		 (get-location p name #f))))))
    (if cache?
	(table-set! (package-cached cache?) name loc))
    loc))

; Find a location.  For immediate use by things like environment-ref.
; Don't record caching, and don't create a new location.

(define (package-find-location p name assign?)
  (cond ((table-ref (package-definitions p) name)
	 (table-ref (package-locations p) name))
	((generated? name)
	 (package-find-location (generated-env name) (generated-symbol name)
				assign?))
	((not assign?)
	 (exporting-package p name
			    (lambda (q type)
			      (package-find-location q name #f))
			    (lambda () #f)))
	(else #f)))


; Get a location for a name that's currently undefined, in hopes that
; it will become defined (as a variable) before it gets used.

(define (get-location p name assign?)
  (let* ((table (package-locations p))
	 (loc (or (table-ref table name)
		  (let ((new (make-new-location p name)))
		    (table-set! table name new)
		    new))))
    (if assign?
	(table-set! assigned-locations (location-id loc) #t))
    loc))

; Mumble.

(define assigned-locations (make-table))

(define (location-ever-assigned? loc)
  (table-ref assigned-locations (location-id loc)))

; --------------------
; If any looked-up location is now invalid, do something about it.  We
; might have a previously unbound variable that is now exported by
; some structure; or, we might have a variable that was bound in some
; structure before, but now either that structure doesn't export it at
; all, or we're getting now that binding from some new structure.

(define (verify-package p)
  (verify-package-undefineds p)
  (verify-package-cached p))

(define (verify-package-undefineds p)
  (let ((newly-defined '())
	(defs (package-definitions p))
	(locs (package-locations p)))
    (table-walk (lambda (name prev)
		  (if (and (not (table-ref defs name))
			   (not (location-ever-assigned? prev)))
		      (let ((new (lookup-imported-location p name #f)))
			(if (not (eq? new prev))
			    (set! newly-defined
				  (cons (cons name prev)
					newly-defined))))))
		locs)
    (if (not (null? newly-defined))
	(begin (display "Newly accessible in ")
	       (write (package-name p))
	       (display ": ")
	       (write (map car newly-defined))
	       (newline)))
    (for-each (lambda (loser)
		(let ((name (car loser))
		      (prev (cdr loser)))
		  (table-set! locs name #f)
		  (*shadow-action* prev '() #f
				   (package-lookup-location p name #f))))
	      newly-defined)))

(define (verify-package-cached p)
  (let ((newly-moved '())
	(cached (package-cached p)))
    (table-walk (lambda (name prev)
		  (let ((new (lookup-imported-location p name #f)))
		    (if (not (eq? new prev))
			(set! newly-moved
			      (cons (cons name prev) newly-moved)))))
		cached)
    (if (not (null? newly-moved))
	(begin (display "Definitions visible in ")
	       (write (package-name p))
	       (display " have moved: ")
	       (write (map car newly-moved))
	       (newline)))
    (for-each (lambda (loser)
		(let ((name (car loser))
		      (prev (cdr loser)))
		  (cope-with-mutation p name
				      (package-lookup-location p name #f)
				      prev)))
	      newly-moved)))


; A package system mutation has newly caused NAME to be bound in
; package P to NEW, but prior references assumed that its binding was
; PREV (perhaps inherited from another package).  If PREV is a
; location, then each occurrence of it stowed away in templates and
; packages must eventually be replaced by either location NEW, if P's
; binding is the one that's visible at that occurrence, or to a fresh
; location to replace PREV, if not.

; Calling routine arranges that new and prev are locations different
; from each other.

(define (cope-with-mutation p name new prev)
  (let ((replacement (make-undefined-location (new-location-uid))))
    (set-location-defined?! replacement #t)
    (copy-location-info! prev replacement)
    (*shadow-action* prev
		     (map package-uid
			  (packages-seeing-location p name prev))
		     new
		     replacement)))

(define *shadow-action*
  (lambda (prev uids new rep)
    (warn "shadowing" prev uids new rep)))
(define (set-shadow-action! proc) (set! *shadow-action* proc))


; Return a list of all packages that might cached a particular inherited
; binding.

(define (packages-seeing-location p name loc)
  (let ((losers '()))
    (let recur ((p p))
      (if (and (not (memq p losers))
	       (not (table-ref (package-definitions p) name)))
	  (begin (set! losers (cons p losers))
		 (walk-population
		   (lambda (struct)
		     (if (structure-exports? struct name)
			 (walk-population recur (structure-clients struct))))
		   (package-structures p)))))
    losers))





(define (reset-packages-state!)
  (flush-location-names)
  (set! *location-uid* 0)  ;was 200 - why?
  (set! *package-uid* 0))  ;was 1 - why?



(define (undefined-variables p)
  (let ((names '()))
    (table-walk (lambda (name loc)
		  (if (not (location-defined? loc))
		      (set! names (cons name names))))
		(package-locations p))
    names))


; For reification

(define (package-define-location! p name loc)
  (table-set! (package-definitions p) name 'value)
  (table-set! (package-locations p) name loc))

