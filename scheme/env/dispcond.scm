; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.



; Displaying conditions

(define display-condition
  (let ((display display) (newline newline))
    (lambda (c port)
      (if (ignore-errors (lambda ()
			   (newline port)
			   (really-display-condition c port)
			   #f))
	  (begin (display "<Error while displaying condition.>" port)
		 (newline port))))))

(define (really-display-condition c port)
  (let* ((stuff (disclose-condition c))
	 (stuff (if (and (list? stuff)
			 (not (null? stuff))
			 (symbol? (car stuff)))
		    stuff
		    (list 'condition stuff))))
    (display-type-name (car stuff) port)
    (if (not (null? (cdr stuff)))
	(begin (display ": " port)
	       (let ((message (cadr stuff)))
		 (if (string? message)
		     (display message port)
		     (limited-write message port *depth* *length*)))
	       (let ((spaces
		      (make-string (+ (string-length
				       (symbol->string (car stuff)))
				      2)
				   #\space)))
		 (for-each (lambda (irritant)
			     (newline port)
			     (display spaces port)
			     (limited-write irritant port *depth* *length*))
			   (cddr stuff)))))
    (newline port)))

(define *depth* 5)
(define *length* 6)

(define-generic disclose-condition &disclose-condition)

(define-method &disclose-condition (c) c)



(define (limited-write obj port max-depth max-length)
  (let recur ((obj obj) (depth 0))
    (if (and (= depth max-depth)
	     (not (or (boolean? obj)
		      (null? obj)
		      (number? obj)
		      (symbol? obj)
		      (char? obj)
		      (string? obj))))
	(display "#" port)
	(call-with-current-continuation
	  (lambda (escape)
	    (recurring-write obj port
	      (let ((count 0))
		(lambda (sub)
		  (if (= count max-length)
		      (begin (display "---" port)
			     (write-char
			      (if (or (pair? obj) (vector? obj))
				  #\)
				  #\})
			      port)
			     (escape #t))
		      (begin (set! count (+ count 1))
			     (recur sub (+ depth 1))))))))))))

