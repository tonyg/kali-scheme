; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define-generic g &g)

(define-method &g ((x :number)) 'win)

(define-method &g ((n :integer))
  (if (= n 13)
      (next-method)
      'ok))

(define-method &g ((s :symbol))
  (if (= s 13)
      (next-method)
      'ok))

; (g 1/2) => 'win
; (g 10) => 'ok
; (g 13) => 'win


(define-generic elt &elt)

(define-method &elt ((x :vector) y)
  (vector-ref x y))

(define-method &elt ((x :string) y)
  (string-ref x y))

(define-method &elt ((x :list) y)
  (list-ref x y))

; Generic length

; (define-generic-function glength ((s :sequence)))

