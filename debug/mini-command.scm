; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Miniature command processor.

(define (command-processor args)
  (let ((in (current-input-port))
	(out (current-output-port))
	(batch? (member "batch" args)))
    (let loop ()
      ((call-with-current-continuation
	 (lambda (go)
	   (with-handler
	       (lambda (c punt)
		 (cond ((or (error? c) (interrupt? c))
			(display-condition c out)
			(go (if batch?
				(lambda () 1)
				loop)))
		       ((warning? c)
			(display-condition c out))
		       (else (punt))))
	     (lambda ()
	       (if (not batch?) (display "- " out))
	       (let ((form (read in)))
		 (cond ((eof-object? form)
			(newline out)
			(go (lambda () 0)))
		       ((equal? form '(unquote load))
			(mini-load in)
			(go loop))
		       ((equal? form '(unquote go))
			(let ((form (read in)))
			  (go (lambda ()
				(eval form (interaction-environment))))))
		       (else
			(call-with-values
			    (lambda () (eval form (interaction-environment)))
			  (lambda results
			    (for-each (lambda (result)
					(write result out)
					(newline out))
				      results)
			    (go loop))))))))))))))

(define (mini-load in)
  (let ((c (peek-char in)))
    (cond ((char=? c #\newline) (read-char in) #t)
	  ((char-whitespace? c) (read-char in) (mini-load in))
	  (else
	   (let ((filename (read-string in char-whitespace?)))
	     (load filename)
	     (mini-load in))))))

(define (read-string port delimiter?)
  (let loop ((l '()) (n 0))
    (let ((c (peek-char port)))
      (cond ((or (eof-object? c)
                 (delimiter? c))
             (list->string (reverse l)))
            (else
             (loop (cons (read-char port) l) (+ n 1)))))))
