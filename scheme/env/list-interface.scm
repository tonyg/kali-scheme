; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


;  ,open interfaces packages meta-types sort syntactic
;  ,config scheme

(define (list-interface thing)
  (cond ((structure? thing)
	 (list-interface-1 (structure-interface thing)
			   (lambda (name)
			     (let ((x (structure-lookup thing name #t)))
			       (if (binding? x)
				   (binding-type x)
				   #f)))))
	((interface? thing)
	 (list-interface-1 thing (lambda (name)
				   (interface-ref thing name))))
	(else '?)))

(define (list-interface-1 int lookup)
  (let ((l '()))
    (for-each-declaration (lambda (name type)
			    (if (not (memq name l))  ;compound signatures...
				(set! l (cons name l))))
			  int)
    (for-each (lambda (name)
		(write name)
		(display (make-string
			  (max 0 (- 25 (string-length
					(symbol->string name))))
			  #\space))
		(write-char #\space)
		(write (careful-type->sexp (lookup name)))    ;( ...)
		(newline))
	      (sort-list l (lambda (name1 name2)
			     (string<? (symbol->string name1)
				       (symbol->string name2)))))))

(define (careful-type->sexp thing)
  (cond ((not thing) 'undefined)
	((or (symbol? thing) (null? thing) (number? thing))
	 thing)     ;?
	((pair? thing)    ;e.g. (variable #{Type :value})
	 (cons (careful-type->sexp (car thing))
	       (careful-type->sexp (cdr thing))))
	(else (type->sexp thing #t))))
