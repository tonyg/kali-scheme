
; don't copyright this, silly shell script


(define (fact n)
  (if (= n 0)
      (+ n 'a)
      (* n (fact (- n 1)))))
