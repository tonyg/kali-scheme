; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.




(define (compose-continuation proc cont)
  (primitive-cwcc
    (lambda (k)
      (with-continuation cont ;(if cont cont null-continuation)
        (lambda ()
          (proc (primitive-cwcc
                  (lambda (k2) (with-continuation k (lambda () k2))))))))))


; Old definition that relies on details of VM architecture:

;(define null-continuation #f)

;(define null-continuation (make-continuation 4 #f)) ;temp kludge
;(continuation-set! null-continuation 1 0)
;(continuation-set! null-continuation 2
;                   ;; op/trap = 140
;                   (segment-data->template (make-code-vector 1 140) #f '()))

;(put 'primitive-cwcc 'scheme-indent-hook 0)
;(put 'with-continuation 'scheme-indent-hook 1)

;(define compose-continuation
;  (let ((tem
;         (let ((cv (make-code-vector 6 0)))
;           (code-vector-set! cv 0 op/push) ;push return value
;           (code-vector-set! cv 1 op/local) ;fetch procedure
;           (code-vector-set! cv 3 1)    ;over = 1
;           (code-vector-set! cv 4 op/call)
;           (code-vector-set! cv 5 1)    ;one argument
;           (segment-data->template cv 0 '()))))
;    (lambda (proc parent-cont)
;      (let ((cont (make-continuation 4 #f)))
;        (continuation-set! cont 0 parent-cont)
;        (continuation-set! cont 1 0)    ;pc
;        (continuation-set! cont 2 tem)  ;template
;        (continuation-set! cont 3 (vector #f proc)) ;environment
;        cont))))

