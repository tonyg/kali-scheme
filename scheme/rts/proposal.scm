; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Higher-level proposal stuff.

; Execute THUNK atomically with its own proposal, saving and restoring
; the current proposal.

(define (call-atomically thunk)
  (let ((old (current-proposal)))
    (let loop ()
      (set-current-proposal! (make-proposal))
      (call-with-values thunk
			(lambda results
			  (if (maybe-commit)
			      (begin
				(set-current-proposal! old)
				(apply values results))
			      (loop)))))))

; Ditto, but no values are returned.

(define (call-atomically! thunk)
  (with-new-proposal (lose)
    (thunk)
    (or (maybe-commit)
	(lose)))
  (values))

; Same again, except that we use the current proposal, if there is one
; (and don't commit on the existing proposal).

(define (call-ensuring-atomicity thunk)
  (if (current-proposal)
      (thunk)
      (call-atomically thunk)))

(define (call-ensuring-atomicity! thunk)
  (if (current-proposal)
      (thunk)
      (call-atomically! thunk)))

; Macro versions of the above that avoid the need to write (lambda () ...)
; around the critical section.

(define-syntax atomically
  (syntax-rules ()
    ((atomically)
     (unspecific))
    ((atomically body ...)
     (call-atomically
       (lambda () body ...)))))

(define-syntax atomically!
  (syntax-rules ()
    ((atomically)
     (values))
    ((atomically body ...)
     (call-atomically!
       (lambda () body ...)))))

(define-syntax ensure-atomicity
  (syntax-rules ()
    ((ensure-atomicity)
     (unspecific))
    ((ensure-atomicity body ...)
     (call-ensuring-atomicity
       (lambda () body ...)))))

(define-syntax ensure-atomicity!
  (syntax-rules ()
    ((ensure-atomicity)
     (values))
    ((ensure-atomicity body ...)
     (call-ensuring-atomicity!
       (lambda () body ...)))))

; Save the existing proposal, install a new one, execute the body, and then
; replace the original proposal.

(define-syntax with-new-proposal
  (syntax-rules ()
    ((with-new-proposal (?lose) ?body ?more ...)
     (let ((old (current-proposal)))
       (call-with-values
	(lambda ()
	  (let ?lose ()
	    (set-current-proposal! (make-proposal))
	    (begin ?body ?more ...)))
	(lambda results
	  (set-current-proposal! old)
	  (apply values results)))))))

; Useful for getting rid of a proposal before raising an error.

(define (remove-current-proposal!)
  (set-current-proposal! #f))

; For use when an inconsistency has been detected.  The SET-CAR! ensures that
; the earlier PROVISIONAL-CAR will fail.

(define (invalidate-current-proposal!)
  (let ((value (provisional-car j-random-pair)))
    (set-car! j-random-pair (cons #f #f))
    value))

(define j-random-pair (list #f))