; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; ,config ,load debug/test.scm

(define-structure testing (export (test :syntax) lost?)
  (open scheme signals handle conditions)
  (begin

(define *lost?* #f)
(define (lost?) *lost?*)

(define (run-test string compare want thunk)
  (let ((result
	 (call-with-current-continuation
	   (lambda (k)
	     (with-handler (lambda (condition punt)
			     (if (error? condition)
				 (k condition)
				 (punt)))
	       thunk)))))
    (if (not (compare want result))
	(begin (display "Test ") (write string) (display " failed.") (newline)
	       (display "Wanted ") (write want)
	       (display ", but got ") (write result) (display ".")
	       (newline)
	       (set! *lost?* #t)))))

(define-syntax test
  (syntax-rules ()
    ((test ?string ?compare ?want ?exp)
     (run-test ?string ?compare ?want (lambda () ?exp)))))

))
