
; The barest skeleton of a test suite.
; Mostly it makes sure that many of the external packages load without
; error.

; ,exec ,load debug/check.scm
; (done)

(load-package 'testing)

(config '(run 
	  (define-structure bar (export)
	    (open scheme testing))))

(in 'bar '(bench off))
(in 'bar '(run (define (foo) (cadr '(a b)))))
(in 'bar '(run (define cadr list)))
(in 'bar '(run (test "non-bench" equal? '((a b)) (foo))))

(in 'bar '(bench on))
(in 'bar '(run (define (foo) (car '(a b)))))
(in 'bar '(run (define car list)))
(in 'bar '(run (test "bench" equal? 'a (foo))))

(config '(run 
(define-structure foo (export)
  (open scheme testing
	assembler
	queues
	random
	sort
	big-scheme
	arrays
	dump/restore
	search-trees
	threads
	sicp)
  (begin

(test "* 1" = 6 (* 1 2 3))
(test "* 2" = (* 214760876 10) 2147608760)
(test "* 3" = (* 47123 46039) 2169495797)
(test "apply" equal? '(1 2 3 4) (apply list 1 2 '(3 4)))
(test "char<->integer" eq? #\a (integer->char (char->integer #\a)))
(test "lap" equal? #f ((lap #f (false) (return))))
(let ((q (make-queue)))
  (enqueue q 'a)
  (test "q" eq? 'a (dequeue q)))
(test "random" <= 0 ((make-random 7)))
(test "sort" equal? '(1 2 3 3) (sort-list '(2 3 1 3) <))
(test "bigbit" = (expt 2 100) (arithmetic-shift 1 100))
(test "format" string=? "x(1 2)" (format #f "x~s" '(1 2)))
(test "destructure" eq? 'a (destructure (((x (y) z) '(b (a) c))) y))
(test "array" eq? 'a
      (let ((a (make-array 'b 3 4)))
	(array-set! a 'a 1 2)
	(array-ref a 1 2)))
(test "receive" eq? 'a (receive (x y) (values 'b 'a) y))
(let ((z '(a "b" 3 #t)))
  (test "dump" equal? z
	(let ((q (make-queue)))
	  (dump z (lambda (c) (enqueue q c)) -1)
	  (restore (lambda () (dequeue q))))))
(with-multitasking
 (lambda ()
   (let* ((cv (make-condvar))
	  (th (spawn (lambda ()
		       (relinquish-timeslice)
		       (condvar-set! cv 'foo))
		     'test)))
     (test "threads" eq? 'foo (condvar-ref cv)))))
(test "explode" equal? 'ab3 (implode (explode 'ab3)))
(test "get/put" equal? 'a (begin (put 'foo 'prop 'a)
				 (get 'foo 'prop)))
(test "search-trees" eq? 'a
      (let ((t (make-search-tree = <)))
	(search-tree-set! t 3 'b)
	(search-tree-set! t 4 'a)
	(search-tree-set! t 5 'c)
	(search-tree-ref t 4)))

))))

(load-package 'foo)

(load-package 'floatnums)

(in 'foo '(run (let* ((one (exact->inexact 1))
		      (three (exact->inexact 3))
		      (third (/ one three))
		      (xthird (inexact->exact third)))
		 (test "float" eq? #f (= 1/3 xthird))
		 (test "exact<->inexact" = third (exact->inexact xthird)))))


; All done.

(if (in 'testing '(run (lost?)))
    (display "Some tests failed.")
    (display "All tests succeeded."))
(newline)

(define (done)
  (exit (if (in 'testing '(run (lost?))) 1 0)))
