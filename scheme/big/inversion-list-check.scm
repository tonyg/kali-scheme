; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-test-suite inversion-lists-tests)

(define-test-case creation/membership inversion-lists-tests
  (check (not (inversion-list-member? 5 (make-empty-inversion-list 0 1000))))
  (check (inversion-list-member? 5 (number->inversion-list 0 1000 5)))
  (check (not (inversion-list-member? 4 (number->inversion-list 0 1000 5))))
  (check (not (inversion-list-member? 6 (number->inversion-list 0 1000 5))))
  (check (not (inversion-list-member? 6 (range->inversion-list 0 1000 500 1000))))
  (check (not (inversion-list-member? 499 (range->inversion-list 0 1000 500 1000))))
  (check (inversion-list-member? 500 (range->inversion-list 0 1000 500 1000)))
  (check (inversion-list-member? 1000 (range->inversion-list 0 1000 500 1000))))

(define-test-case complement/1 inversion-lists-tests
  (check
   (inversion-list-complement
    (inversion-list-complement
     (range->inversion-list 0 1000 5 10)))
   (=> inversion-list=?)
   (range->inversion-list 0 1000 5 10)))

(define-test-case complement/2 inversion-lists-tests
  (check
   (inversion-list-complement
    (inversion-list-complement
     (range->inversion-list 0 1000 0 1000)))
   (=> inversion-list=?)
   (range->inversion-list 0 1000 0 1000)))

(define-test-case union/1 inversion-lists-tests
  (check
   (inversion-list-union (range->inversion-list 0 1000 5 10)
			 (range->inversion-list 0 1000 20 30))
   (=> inversion-list=?)
   
   (ranges->inversion-list 0 1000 '(5 . 10) '(20 . 30))))

(define-test-case union/2 inversion-lists-tests
  (check
   (inversion-list-union (range->inversion-list 0 1000 5 10)
			 (range->inversion-list 0 1000 7 8))
   (=> inversion-list=?)
   (range->inversion-list 0 1000 5 10)))
  
(define-test-case union/3 inversion-lists-tests
  (check
   (inversion-list-union (range->inversion-list 0 1000 5 10)
			 (range->inversion-list 0 1000 7 15))
   (=> inversion-list=?)
   (range->inversion-list 0 1000 5 15)))

(define-test-case union/4 inversion-lists-tests
  (check
   (inversion-list-union (range->inversion-list 0 1000 500 1000)
			 (range->inversion-list 0 1000 0 500))
   (=> inversion-list=?)
   (range->inversion-list 0 1000 0 1000)))

(define-test-case intersection/1 inversion-lists-tests
  (check
   (inversion-list-intersection (range->inversion-list 0 1000 5 10)
				(range->inversion-list 0 1000 20 30))
   (=> inversion-list=?)
   (make-empty-inversion-list 0 1000)))
  
(define-test-case intersection/2 inversion-lists-tests
  (check
   (inversion-list-intersection (range->inversion-list 0 1000 5 10)
				(range->inversion-list 0 1000 7 8))
   (=> inversion-list=?)
   (range->inversion-list 0 1000 7 8)))

(define-test-case intersection/3 inversion-lists-tests
  (check
   (inversion-list-intersection (range->inversion-list 0 1000 5 10)
				(range->inversion-list 0 1000 7 15))
   (=> inversion-list=?)
   (range->inversion-list 0 1000 7 10)))

(define-test-case intersection/4 inversion-lists-tests
  (check
   (inversion-list-intersection (range->inversion-list 0 1000 500 1000)
				(range->inversion-list 0 1000 0 501))
   (=> inversion-list=?)
   (range->inversion-list 0 1000 500 501)))

(define-test-case intersection/5 inversion-lists-tests
  (check
   (inversion-list-intersection (range->inversion-list 0 1000 500 1000)
				(range->inversion-list 0 1000 501 505))
   (=> inversion-list=?)
   (range->inversion-list 0 1000 501 505)))

(define-test-case adjoin inversion-lists-tests
  (check
   (inversion-list-adjoin (range->inversion-list 0 1000 0 999) 999)
   (=> inversion-list=?)
   (range->inversion-list 0 1000 0 1000)))

(define-test-case remove inversion-lists-tests
  (check
   (inversion-list-remove (range->inversion-list 0 1000 0 1000) 999)
   (=> inversion-list=?)
   (range->inversion-list 0 1000 0 999)))

(define-test-case size inversion-lists-tests
  (check
   (inversion-list-size
    (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000)))
   => 510))

(define-test-case copy inversion-lists-tests
  (check
   (inversion-list-copy
    (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000)))
   (=> inversion-list=?)
   (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000))))

(define-test-case fold/done? inversion-lists-tests
  (check
   (inversion-list-fold/done?
    (lambda (n sum)
      (+ n sum))
    0
    (lambda (sum)
      (> sum 250000))
    (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000)))
   =>
   250781))

(define (i-list-sum i-list)
  (let loop ((cursor (inversion-list-cursor i-list))
	     (sum 0))
    (if (inversion-list-cursor-at-end? cursor)
	sum
	(loop (inversion-list-cursor-next i-list cursor)
	      (+ (inversion-list-cursor-ref cursor)
		 sum)))))
    
(define-test-case cursor inversion-lists-tests
  (check
   (i-list-sum (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000)))
   => 374870))

(define-test-case hash inversion-lists-tests 
  (check
   (not
    (= (inversion-list-hash (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000)) 1031)
       (inversion-list-hash (ranges->inversion-list 0 1000 '(5 . 10) '(500 . 1000)) 1031)))))


