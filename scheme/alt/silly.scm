; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.



(define (reverse-list->string l n)
  ;; Significantly faster than (list->string (reverse l))
  (let ((s (make-string n #\x)))
    (let loop ((i (- n 1)) (l l))
      (if (< i 0) s (begin (string-set! s i (car l))
			   (loop (- i 1) (cdr l)))))))
