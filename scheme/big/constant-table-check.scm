; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-test-suite constant-tables-tests)

(define (check-table entries hash-function)
  (let ((table (make-constant-table entries hash-function)))
    (for-each (lambda (p)
		(check (constant-table-lookup table (car p))
		       => (cdr p)))
	      entries)))
	    
(define-test-case simple constant-tables-tests
  (check-table '((foo . 1) (bar . 2) (baz . 3) (bala . 4))
	       symbol-hash))

(define-test-case not-present constant-tables-tests
  (let ((table (make-constant-table '((foo . 1) (bar . 2) (baz . 3) (bala . 4)) 
				    symbol-hash)))
    (check (not (constant-table-lookup table 'yellow)))
    (check (not (constant-table-lookup table 'balab)))
    (check (not (constant-table-lookup table 'foobar)))
    (check (not (constant-table-lookup table 'foobarbaz)))))

(define-test-case bigger constant-tables-tests
  (let loop ((i 0) (entries '()))
    (if (= i 1000)
	(check-table entries symbol-hash)
	(loop (+ 1 i)
	      (cons (cons (string->symbol (number->string i))
			  i)
		    entries)))))
