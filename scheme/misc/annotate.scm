; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.



; Derived from
;  (lambda (x y) (vector (car x) (cdr x) y))

(define annotate-procedure
  (lap annotate-procedure ()
       (protocol 2 (push template))
       (stack-ref 2)
       (stored-object-ref closure 0)
       (push)
       (stack-ref 3)
       (stored-object-ref closure 1)
       (push+stack-ref 3)
       (make-stored-object 3 closure)
       (return)))

; Derived from
;  (lambda (x) (if (< 2 (vector-length x)) (vector-ref x 2) #f))

(define procedure-annotation
  (lap procedure-anotation ()
       (protocol 1 (push template))
       (literal 2)
       (push)
       (stack-ref 2)
       (stored-object-length closure)
       (<)
       (jump-if-false (=> no-annotation))
       (stack-ref+push 1)
       (literal 2)
       (stored-object-indexed-ref closure 0)
       (return)
     no-annotation
       (false)
       (return)))
