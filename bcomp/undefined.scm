; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; Maintain and display a list of undefined names.

(define $note-undefined (make-fluid #f))

(define (note-undefined! name)
  (let ((note (fluid $note-undefined)))
    (if note (note name))))

(define (noting-undefined-variables p thunk)
  (let* ((losers '())
	 (foo (lambda (q name)
		(let ((probe (assq q losers)))
		  (if probe
		      (if (not (member name (cdr probe)))
			  (set-cdr! probe (cons name (cdr probe))))
		      (set! losers (cons (list q name) losers)))))))
    (let-fluid $note-undefined
	(or (fluid $note-undefined)	;Recursive LOAD
	    (lambda (name)
	      (if (generated? name)
		  (foo (generated-env name)
		       (generated-symbol name))
		  (foo p name))))
      (lambda ()
	(let ((result (thunk)))
	  (for-each
	    (lambda (p+names)
	      (let* ((q (car p+names))
		     (names
		      (filter (lambda (name)
				;; Keep the ones that are still unbound.
				(unbound? (package-lookup q name)))
			      (cdr p+names))))
		(if (not (null? names))
		    (begin (display "Undefined")
			   (if (not (eq? q p))
			       (begin (display " ")
				      (write `(package ,(package-name
							 (car p+names))))))
			   (display ": ")
			   (write (map (lambda (name)
					 (if (generated? name)
					     (generated-symbol name)
					     name))
				       (reverse names)))
			   (newline)))))
	    losers)
	  result)))))
