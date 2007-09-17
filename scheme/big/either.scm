; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Nondeterminism, Prolog, or whatever you want to call it.  This is
; depth-first search implemented using call/cc.

; The fluid variable $FAIL is bound to a thunk to be called in case of failure.

(define $fail
  (make-fluid (make-cell
	       (lambda ()
		 (assertion-violation 'fail
				      "call to FAIL outside WITH-NONDETERMINISM")))))

(define (with-nondeterminism thunk)
  (let-fluid $fail
	     (make-cell (lambda ()
			  (assertion-violation 'with-nondeterminism
					       "nondeterminism ran out of choices")))
	     thunk))

; Call the current failure function.

(define (fail)
  ((fluid-cell-ref $fail)))

; For the alternation operator, Icon's a | b or McCarthy's (amb a b),
; we write (either a b).

(define-syntax either
  (syntax-rules ()
    ((either) (fail))
    ((either x) x)
    ((either x y ...)
     (%either (lambda () x) (lambda () (either y ...))))))

; 1. Save the current failure procedure and continuation.
; 2. Install a new failure procedure that restores the old failure procedure
;    and continuation and then calls THUNK2.
; 3. Call THUNK1.

(define (%either thunk1 thunk2)
  (let ((save (fluid-cell-ref $fail)))
    ((call-with-current-continuation
       (lambda (k)
	 (fluid-cell-set! $fail
			  (lambda ()
			    (fluid-cell-set! $fail save)
			    (k thunk2)))
	 thunk1)))))

; (one-value x) is Prolog's CUT operator.  X is allowed to return only once.

(define-syntax one-value
  (syntax-rules ()
    ((one-value x) (%one-value (lambda () x)))))

(define (%one-value thunk)
  (let ((save (fluid-cell-ref $fail)))
    (call-with-values thunk
		      (lambda args
			(fluid-cell-set! $fail save)
			(apply values args)))))

; (all-values a) returns a list of all the possible values of the
; expression a.  Prolog calls this "bagof"; I forget what Icon calls it.

(define-syntax all-values
  (syntax-rules ()
    ((all-values x) (%all-values (lambda () x)))))

(define (%all-values thunk)
  (let ((results '()))
    (either (let ((new-result (thunk)))
	      (set! results (cons new-result results))
	      (fail))
	    (reverse results))))
