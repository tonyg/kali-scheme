; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


; Type variables - what a mess

(define-record-type uvar :uvar
  (really-make-uvar prefix depth id tuple-okay?
	     place source binding temp)		; all initialized to #F
  uvar?
  (prefix  uvar-prefix)			  ; a name for debugging
  (depth   uvar-depth set-uvar-depth!)	  ; lexical depth of the uvar
  (id      uvar-id)			  ; a number
  ; true if this can be unified with a tuple, set when merged
  (tuple-okay? uvar-tuple-okay? set-uvar-tuple-okay?!)
  (place   uvar-place   set-uvar-place!)  ; used in producing type schemes
  (source  uvar-source  set-uvar-source!)
  ; to let the user know where this came from
  (binding uvar-binding set-uvar-binding!); known value of this uvar
  (temp    uvar-temp    set-uvar-temp!))  ; useful field

(define-record-discloser :uvar
  (lambda (uvar)
    (list 'uvar
	  (uvar-prefix uvar)
	  (uvar-depth uvar)
	  (uvar-id uvar)
	  (uvar-binding uvar))))

(define (make-uvar prefix depth . maybe-id)
  (really-make-uvar prefix
		    depth
		    (if (null? maybe-id)
			(unique-id)
			(car maybe-id))
		    #f			; tuple-okay?
		    #f #f #f #f))	; place source binding temp

(define (make-tuple-uvar prefix depth . maybe-id)
  (really-make-uvar prefix
		    depth
		    (if (null? maybe-id)
			(unique-id)
			(car maybe-id))
		    #t			; tuple-okay?
		    #f #f #f #f))	; place source binding temp

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
	 (bind-uvar-to-uvar! uvar value)
	 #f)
	(else
	 (bind-uvar-to-type! uvar value))))

(define (bind-uvar-to-uvar! uvar0 uvar1)
  (minimize-type-depth! uvar1 (uvar-depth uvar0))
  (set-uvar-binding! uvar0 uvar1)
  (if (and (uvar-tuple-okay? uvar1)
	   (not (uvar-tuple-okay? uvar0)))
      (set-uvar-tuple-okay?! uvar1 #f)))

(define (bind-uvar-to-type! uvar type)
  (let ((errors '()))
    (if (uvar-in-type? uvar type)
	(set! errors (cons circularity-error errors)))
    (if (and (tuple-type? type)
	     (not (uvar-tuple-okay? uvar)))
	(set! errors (cons (tuple-error type) errors)))
    (cond ((null? errors)  ; whew!
	   (minimize-type-depth! type (uvar-depth uvar))
	   (set-uvar-binding! uvar type)
	   #f)
	  (else
	   (lambda ()
	     (format #t "unifying ")
	     (display-type uvar (current-output-port))
	     (format #t " == ")
	     (display-type type (current-output-port))
	     (format #t "~% would cause the following problem~A:"
		     (if (null? (cdr errors)) "" "s"))
	     (for-each (lambda (x) (x)) errors))))))
		       
(define (circularity-error)
  (format #t "~% creation of a circular type"))

(define (tuple-error type)
  (lambda ()
    (if (null? (tuple-type-types type))
	(format #t "~% returning no values where one is expected")
	(format #t "~% returning ~D values where one is expected"
		(length (tuple-type-types type))))))

; Check that UVAR does not occur in EXP.

(define (uvar-in-type? uvar exp)
  (let label ((exp exp))
    (cond ((or (base-type? exp)
	       (record-type? exp))
	   #f)
	  ((uvar? exp)
	   (if (uvar-binding exp)
	       (label (uvar-binding exp))
	       (eq? exp uvar)))
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
  (set! *unique-id-counter* 0))
