; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Miniature command processor.

(define (command-processor ignore args)
  (let ((in (current-input-port))
	(out (current-output-port))
	(err (current-error-port))
	(batch? (member "batch" (map os-string->string args))))
    (let loop ()
      ((call-with-current-continuation
	 (lambda (go)
	   (with-handler
	       (lambda (c punt)
		 (cond ((or (serious-condition? c) (interrupt-condition? c))
			(display-condition c err)
			(go (if batch?
				(lambda () 1)
				loop)))
		       ((warning? c)
			(display-condition c err))
		       (else (punt))))
	     (lambda ()
	       (if (not batch?)
		   (display "- " out))
	       (let ((form (read in)))
		 (cond ((eof-object? form)
			(newline out)
			(go (lambda () 0)))
		       ((and (pair? form) (eq? (car form) 'unquote))
			(case (cadr form)
			  ((load)
			   (mini-load in)
			   (go loop))
			  ((go)
			   (let ((form (read in)))
			     (go (lambda ()
				   (eval form (interaction-environment))))))
			  (else (error 'command-processor
				       "unknown command" (cadr form) 'go 'load (eq? (cadr form) 'load)))))
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
	  ((char=? c #\")
	   (let ((filename (read in)))
	     (load filename)
	     (mini-load in)))
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

(define (byte-vector->string b)
  (let ((size (- (byte-vector-length b) 1)))
    (do ((s (make-string size))
	 (i 0 (+ 1 i)))
	((= i size)
	 s)
      (string-set! s i (ascii->char (vector-ref b i))))))

    
