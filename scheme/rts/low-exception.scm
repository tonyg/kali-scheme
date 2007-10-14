; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is to avoid a circular dependency: Doing this right requires
; the EXCEPTION structure, which comes (much) later.

(define (error-proc . args)
  (debug-message "error called before it's ready"))
(define (assertion-violation-proc . args)
  (debug-message "assertion-violation called before it's ready"))
(define (implementation-restriction-violation-proc . args)
  (debug-message "implementation-restriction-violation called before it's ready"))
(define (warning-proc . args)
  (debug-message "warning called before it's ready"))
(define (syntax-violation-proc . args)
  (debug-message "syntax-violation called before it's ready"))
(define (note-proc . args)
  (debug-message "note called before it's ready"))

(define (error who message . irritants)
  (apply error-proc who message irritants))

(define (assertion-violation who message . irritants)
  (apply assertion-violation-proc who message irritants))

(define (implementation-restriction-violation who message . irritants)
  (apply implementation-restriction-violation who message irritants))

(define (warning who message . irritants)
  (apply warning-proc who message irritants))

(define (note who message . irritants)
  (apply note-proc who message irritants))

(define (syntax-violation who message . irritants)
  (apply syntax-violation-proc who message irritants))

(define (initialize-low-exception-procedures!
	 error assertion-violation implementation-restriction-violation
	 warning note
	 syntax-violation)
  (set! error-proc error)
  (set! assertion-violation-proc assertion-violation)
  (set! implementation-restriction-violation-proc implementation-restriction-violation)
  (set! warning-proc warning)
  (set! note-proc note)
  (set! syntax-violation-proc syntax-violation))

