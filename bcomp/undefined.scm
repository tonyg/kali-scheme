; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.



; Maintain and display a list of undefined names.

(define $note-undefined (make-fluid #f))

(define (note-undefined! p name)
  (let ((note (fluid $note-undefined)))
    (if note (note p name))))

(define (noting-undefined-variables p thunk)
  (if (fluid $note-undefined)
      (thunk)
      (let* ((losers '())
	     (foo (lambda (env name)
		    (let ((probe (assq env losers)))
		      (if probe
			  (if (not (member name (cdr probe)))
			      (set-cdr! probe (cons name (cdr probe))))
			  (set! losers (cons (list env name) losers)))))))
	(let-fluid $note-undefined
	    (lambda (p name)
	      (if (generated? name)
		  (foo (generated-env name)
		       (generated-symbol name))
		  (foo p name)))
	  (lambda ()
	    (dynamic-wind
	     (lambda () #f)
	     thunk
	     (lambda ()
	       (for-each
		(lambda (p+names)
		  (let* ((env (car p+names))
			 (names
			  (filter (lambda (name)
				    ;; Keep the ones that are still unbound.
				    (unbound? (generic-lookup env name)))
				  (cdr p+names))))
		    (if (not (null? names))
			(begin (display "Undefined")
			       (if (not (eq? env p))
				   (begin (display " in ")
					  (write (car p+names))))
			       (display ": ")
			       (write (map (lambda (name)
					     (if (generated? name)
						 (generated-symbol name)
						 name))
					   (reverse names)))
			       (newline)))))
		losers))))))))
