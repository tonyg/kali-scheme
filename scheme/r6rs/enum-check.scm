; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-test-suite r6rs-enums-tests)

;; These are all from the R6RS document

(define-test-case enum-set-indexer r6rs-enums-tests
  (check
   (let* ((e (make-enumeration '(red green blue)))
	  (i (enum-set-indexer e)))
     (list (i 'red) (i 'green) (i 'blue) (i 'yellow)))
   => '(0 1 2 #f)))

(define-test-case enum-set->list r6rs-enums-tests
  (check
   (let* ((e (make-enumeration '(red green blue)))
	  (c (enum-set-constructor e)))
     (enum-set->list (c '(blue red)))) 
   => '(red blue)))

(define-test-case enum-set-tests r6rs-enums-tests
  (let* ((e (make-enumeration '(red green blue)))
	 (c (enum-set-constructor e)))
    (check (enum-set-member? 'blue (c '(red blue))))
    (check (not (enum-set-member? 'green (c '(red blue)))))
    (check (enum-set-subset? (c '(red blue)) e))
    (check (enum-set-subset? (c '(red blue)) (c '(blue red))))
    (check (not (enum-set-subset? (c '(red blue)) (c '(red)))))
    (check (enum-set=? (c '(red blue)) (c '(blue red))))))

(define-test-case enum-set-logical r6rs-enums-tests
  (let* ((e (make-enumeration '(red green blue)))
	 (c (enum-set-constructor e)))
    (check (enum-set->list
	    (enum-set-union (c '(blue)) (c '(red))))
	   => '(red blue))
    (check (enum-set->list
	    (enum-set-intersection (c '(red green))
				   (c '(red blue))))
	   => '(red))
    (check (enum-set->list
	    (enum-set-difference (c '(red green))
				 (c '(red blue))))
	   => '(green))))

(define-test-case enum-set-projection r6rs-enums-tests
  (check
   (let ((e1 (make-enumeration
	      '(red green blue black)))
	 (e2 (make-enumeration
	      '(red black white))))
     (enum-set->list
      (enum-set-projection e1 e2)))
   => '(red black)))

(define-enumeration color
  (black white purple maroon)
  color-set)

(define-test-case define-enumeration r6rs-enums-tests
  (check (color black) => 'black)
  (check (enum-set->list (color-set))  => '())
  (check (enum-set->list
	  (color-set maroon white))
	  => '(white maroon)))


