(define-test-suite sicp-tests)

(define-test-case get/put sicp-tests
  (check (begin (put 'foo 'prop 'a)
		(get 'foo 'prop)) 
	 => 'a))
