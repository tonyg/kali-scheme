; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; ,config ,load =scheme48/debug/test.scm
; ,exec ,load =scheme48/big/inversion-list-check.scm
; ,exec (done)

(load-package 'testing)

(config '(run 
(define-structure inversion-lists-test (export)

  (open scheme testing
	inversion-lists)

  (begin

(test "creation/membership" equal?
      '(#f #t #f #f #f #f #t #t)
      (list
       (inversion-list-member? 5 (make-empty-inversion-list 0 1000))
       (inversion-list-member? 5 (number->inversion-list 0 1000 5))
       (inversion-list-member? 4 (number->inversion-list 0 1000 5))
       (inversion-list-member? 6 (number->inversion-list 0 1000 5))
       (inversion-list-member? 6 (range->inversion-list 0 1000 500 1000))
       (inversion-list-member? 499 (range->inversion-list 0 1000 500 1000))
       (inversion-list-member? 500 (range->inversion-list 0 1000 500 1000))
       (inversion-list-member? 1000 (range->inversion-list 0 1000 500 1000))))

(test "complement/1" inversion-list=?
      (range->inversion-list 0 1000 5 10)
      (inversion-list-complement
       (inversion-list-complement
	(range->inversion-list 0 1000 5 10))))

(test "complement/2" inversion-list=?
      (range->inversion-list 0 1000 0 1000)
      (inversion-list-complement
       (inversion-list-complement
	(range->inversion-list 0 1000 0 1000))))

(test "union/1" inversion-list=?
      (ranges->inversion-list 0 1000 '(5 . 10) '(20 . 30))
      (inversion-list-union (range->inversion-list 0 1000 5 10)
			    (range->inversion-list 0 1000 20 30)))

(test "union/2" inversion-list=?
      (range->inversion-list 0 1000 5 10)
      (inversion-list-union (range->inversion-list 0 1000 5 10)
			    (range->inversion-list 0 1000 7 8)))

(test "union/3" inversion-list=?
      (range->inversion-list 0 1000 5 15)
      (inversion-list-union (range->inversion-list 0 1000 5 10)
			    (range->inversion-list 0 1000 7 15)))

(test "union/4" inversion-list=?
      (range->inversion-list 0 1000 0 1000)
      (inversion-list-union (range->inversion-list 0 1000 500 1000)
			    (range->inversion-list 0 1000 0 500)))

(test "intersection/1" inversion-list=?
      (make-empty-inversion-list 0 1000)
      (inversion-list-intersection (range->inversion-list 0 1000 5 10)
				   (range->inversion-list 0 1000 20 30)))

(test "intersection/2" inversion-list=?
      (range->inversion-list 0 1000 7 8)
      (inversion-list-intersection (range->inversion-list 0 1000 5 10)
				   (range->inversion-list 0 1000 7 8)))

(test "intersection/3" inversion-list=?
      (range->inversion-list 0 1000 7 10)
      (inversion-list-intersection (range->inversion-list 0 1000 5 10)
				   (range->inversion-list 0 1000 7 15)))

(test "intersection/4" inversion-list=?
      (range->inversion-list 0 1000 500 501)
      (inversion-list-intersection (range->inversion-list 0 1000 500 1000)
				   (range->inversion-list 0 1000 0 501)))

(test "adjoin" inversion-list=?
      (range->inversion-list 0 1000 0 1000)
      (inversion-list-adjoin (range->inversion-list 0 1000 0 999) 999))

(test "remove" inversion-list=?
      (range->inversion-list 0 1000 0 999)
      (inversion-list-remove (range->inversion-list 0 1000 0 1000) 999))

(test "size" =
      510
      (inversion-list-size
       (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000))))

(test "copy" inversion-list=?
      (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000))
      (inversion-list-copy
       (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000))))

(test "fold/done?" =
      250781
      (inversion-list-fold/done?
       (lambda (n sum)
	 (+ n sum))
       0
       (lambda (sum)
	 (> sum 250000))
       (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000))))

(define (i-list-sum i-list)
  (let loop ((cursor (inversion-list-cursor i-list))
	     (sum 0))
    (if (inversion-list-cursor-at-end? cursor)
	sum
	(loop (inversion-list-cursor-next i-list cursor)
	      (+ (inversion-list-cursor-ref cursor)
		 sum)))))
    
(test "cursor" =
      374870
      (i-list-sum (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000))))

(test "hash" (lambda (n1 n2) (not (= n1 n2)))
      (inversion-list-hash (ranges->inversion-list 0 1000 '(5 . 10) '(15 . 20) '(500 . 1000)) 1031)
      (inversion-list-hash (ranges->inversion-list 0 1000 '(5 . 10) '(500 . 1000)) 1031))

))))

(if (in 'testing '(run (lost?)))
    (display "Some tests failed.")
    (display "All tests succeeded."))
(newline)

(define (done)
  (exit (if (in 'testing '(run (lost?))) 1 0)))
