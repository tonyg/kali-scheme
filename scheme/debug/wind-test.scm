; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


"
The correct output looks something like this:

wind-1  f: 1
wind-2  f: 2
before-throw-out  f: 3
unwind-2  f: 2
unwind-1  f: 1
after-throw-out  f: top
wind-1  f: 1
wind-2  f: 2
after-throw-in  f: 3
unwind-2  f: 2
unwind-1  f: 1
done  f: top
"

(define (wind-test)
  (let* ((f (make-fluid 'top))
	 (report (lambda (foo)
		   (write foo)
		   (display "  f: ")
		   (write (fluid f))
		   (newline))))
    ((call-with-current-continuation
       (lambda (k1)
	 (let-fluid f 1
	   (lambda ()
	     (dynamic-wind
	      (lambda () (report 'wind-1))
	      (lambda ()
		(let-fluid f 2
		  (lambda ()
		    (dynamic-wind
		     (lambda () (report 'wind-2))
		     (lambda ()
		       (let-fluid f 3
			 (lambda ()
			   (report 'before-throw-out)
			   (call-with-current-continuation
			     (lambda (k2)
			       (k1 (lambda ()
				     (report 'after-throw-out)
				     (k2 #f)))))
			   (report 'after-throw-in)
			   (lambda () (report 'done)))))
		     (lambda () (report 'unwind-2))))))
	      (lambda () (report 'unwind-1))))))))))

