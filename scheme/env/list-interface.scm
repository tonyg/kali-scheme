; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


;  ,open interfaces packages meta-types sort syntactic
;  ,config scheme

; Print out the names and types exported by THING, which is either a structure
; or an interface.

(define (list-interface thing)
  (cond ((structure? thing)
	 (list-interface-1 (structure-interface thing)
			   (lambda (name type)
			     (let ((x (structure-lookup thing name #t)))
			       (if (binding? x)
				   (binding-type x)
				   #f)))))
	((interface? thing)
	 (list-interface-1 thing
			   (lambda (name type)
			     type)))
	(else '?)))

; LOOKUP is passed the package-name and the type from the interface and
; returns a (possibly different) type.

(define (list-interface-1 int lookup)
  (let ((names '()))
    (for-each-declaration (lambda (name package-name type)
			    (if (not (assq name names))  ;compound signatures...
				(set! names
				      (cons (cons name
						  (lookup package-name type))
					    names))))
			  int)
    (for-each (lambda (pair)
		(let ((name (car pair))
		      (type (cdr pair)))
		  (write name)
		  (display (make-string
			    (max 0 (- 25 (string-length
					  (symbol->string name))))
			    #\space))
		  (write-char #\space)
		  (write (careful-type->sexp type))    ;( ...)
		  (newline)))
	      (sort-list names
			 (lambda (pair1 pair2)
			   (string<? (symbol->string (car pair1))
				     (symbol->string (car pair2))))))))

(define (careful-type->sexp thing)
  (cond ((not thing) 'undefined)
	((or (symbol? thing)
	     (null? thing)
	     (number? thing))
	 thing)     ;?
	((pair? thing)    ;e.g. (variable #{Type :value})
	 (cons (careful-type->sexp (car thing))
	       (careful-type->sexp (cdr thing))))
	(else
	 (type->sexp thing #t))))

