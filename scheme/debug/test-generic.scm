; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define g-methods (make-method-table 'g))

(define g (make-generic g-methods))

(define foo-family (make-family 'foo 1))

(define-method g-methods foo-family
  (lambda (x)
    (if (even? x)
	'win
	(fail))))

(define bar-family (make-family 'bar 2))  ;More specific

(define-method g-methods bar-family
  (lambda (x)
    (case x
      ((1 3 5) 'ok)
      (else (fail)))))

(define-method g-methods (make-family 'baz 2)  ;Same priority as bar-family
  (lambda (x)
    (case x
      ((3) 'great)
      (else (fail)))))

; (g 0) => 'win
; (g 1) => 'ok
; (g 3) => 'great
; (g 9) => error

