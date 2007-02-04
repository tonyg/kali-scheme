; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; The barest skeleton of a test suite for some stuff in the compiler

(user '(open test-suites))

(config '(run 
	  (define-structure bar (export bench-tests)
	    (open scheme test-suites))))

(in 'bar '(run (define-test-suite bench-tests)))
(in 'bar '(bench off))
(in 'bar '(run (define (foo) (cadr '(a b)))))
(in 'bar '(run (define cadr list)))
(in 'bar '(run (define-test-case non-bench bench-tests (check (foo) => '((a b))))))

(in 'bar '(bench on))
(in 'bar '(run (define (baz) (car '(a b)))))
(in 'bar '(run (define car list)))
(in 'bar '(run (define-test-case bench bench-tests (check (baz) => 'a))))

(user '(open bar))

(config '(run
	  (define-structure test1 (export test1-tests x)
	    (open scheme test-suites)
	    (begin (define-test-suite test1-tests)
		   (define x 10)
		   (define (z) x)))))
(config '(run 
	  (define-structure test2 (export test2-tests)
	    (open scheme test1 test-suites)
	    (begin (define-test-suite test2-tests)
		   (define (z) x)))))
(config '(run
	  (define-structure test3 (export test3-tests)
	    (open scheme test1 test-suites)
	    (begin (define-test-suite test3-tests)
		   (define (z) x)))))
(load-package 'test2)
(load-package 'test3)
(in 'test3 '(run (define x 20)))
(in 'test3 '(open test2))
(in 'test2 '(run (define-test-case shadowing2 test2-tests (check (z) => 10))))
(in 'test3 '(run (define-test-case shadowing3 test3-tests (check (z) => 20))))
(in 'test1 '(run (define-test-case shadowing1 test1-tests (check (z) => 10))))

(user '(open test1 test2 test3))

(user '(run (define-test-suite compiler-tests (bench-tests
					       test1-tests test2-tests test3-tests))))

