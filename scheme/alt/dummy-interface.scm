; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Interfaces are ignored.  Only dependencies are significant.

(define (make-indirect-interface name thunk)
  (thunk))

(define (make-simple-interface name items)
  (cons 'export items))

(define (make-compound-interface name . sigs)
  (cons 'compound-interface sigs))

; Types
(define :value ':value)
(define :syntax ':syntax)
(define :structure ':structure)
(define :procedure ':procedure)
(define :number ':number)
(define :type ':type)

(define-syntax proc
  (lambda (e r c) ''proc-lossage))

(define-syntax interface-of
  (lambda (e r c) ''interface-of-lossage))

(define (note-reference-to-interface! int thing)
  'int-lossage)

(define (interface-name int)
  'int-lossage)

(define (interface? int)
  (and (pair? int)
       (or (eq? 'export (car int))
	   (eq? 'compound-interface (car int)))))

(define (note-interface-name! int name)
  'int-lossage)

