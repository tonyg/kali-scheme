; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.



; For DEFINE-STRUCTURE macro

(define (make-a-package opens-thunk accesses-thunk tower
			dir clauses names)
  (make-package opens-thunk accesses-thunk
		#t    ;unstable
		tower
		dir
		clauses
		#f
		(if (pair? names)
		    (car names)
		    names)))

(define (loser . rest)
  (error "init-defpackage! neglected"))

(define interface-of structure-interface)

(define *verify-later!* (lambda (thunk) #f))

(define (verify-later! thunk)
  (*verify-later!* thunk))

(define (set-verify-later! proc)
  (set! *verify-later!* proc))
