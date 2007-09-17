; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; For DEFINE-STRUCTURE macro

(define (make-a-package opens-thunk accesses-thunk tower
			dir clauses name)
  (make-package opens-thunk accesses-thunk
		#t    ;unstable
		tower
		dir
		clauses
		#f
		name))

(define (loser . rest)
  (assertion-violation 'init-defpackage! "init-defpackage! neglected"))

(define interface-of structure-interface)

(define *verify-later!* (lambda (thunk) #f))

(define (verify-later! thunk)
  (*verify-later!* thunk))

(define (set-verify-later! proc)
  (set! *verify-later!* proc))

(define (note-name! thing name)
  (cond ((interface? thing)
	 (note-interface-name! thing name))
	((structure? thing)
	 (note-structure-name! thing name)))
  thing)
