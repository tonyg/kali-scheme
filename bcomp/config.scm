; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; For define-package macro

(define (make-a-package opens-thunk accesses-thunk for-syntax-thunk
			dir clauses names)
  (make-package opens-thunk accesses-thunk
		;; Pretty much of a kludge
		*evaluator*
		(if for-syntax-thunk
		    (delay (for-syntax-thunk))
		    (*make-package-for-syntax-promise*))
		dir
		clauses
		(if (pair? names)
		    (car names)
		    names)))

(define (init-defpackage! evaluator foo)
  (set! *evaluator* evaluator)
  (set! *make-package-for-syntax-promise* foo))

(define (loser . rest)
  (error "init-defpackage! neglected"))

(define *evaluator* loser)
(define *make-package-for-syntax-promise* loser)

(define signature-of structure-signature)

(define *verify-later!* (lambda (thunk) #f))

(define (verify-later! thunk)
  (*verify-later!* thunk))

(define (set-verify-later! proc)
  (set! *verify-later!* proc))
