; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.

; Redefine CASE so that it doesn't call MEMV

(define-syntax case
  (lambda (e r c)
    (let ((x (r 'x))
	  (xlet (r 'let))
	  (xcond (r 'cond))
	  (xif (r 'if))
	  (xeq? (r 'eq?))
	  (xquote (r 'quote)))
      (let ((test (lambda (y)
		    `(,xeq? ,x (,xquote ,y)))))
	`(,xlet ((,x ,(cadr e)))
		(,xcond . ,(map (lambda (clause)
				  (if (c (car clause) 'else)
				      clause
				      `(,(let label ((xs (car clause)))
					   (cond ((null? xs) #f)
						 ((null? (cdr xs))
						  (test (car xs)))
						 (else
						  `(,xif ,(test (car xs))
							 #t
							 ,(label (cdr xs))))))
					. ,(cdr clause))))
				(cddr e))))))))

; RECEIVE (from big-scheme)

(define-syntax receive
  (syntax-rules ()
    ((receive ?vars ?producer . ?body)
     (call-with-values (lambda () ?producer)
		       (lambda ?vars . ?body)))))


(define-syntax external
  (lambda (e r c)
    (let ((l (length e)))
      (if (and (or (= l 3) (= l 4))
	       (string? (cadr e)))
	  `(,(r 'real-external) ,(cadr e) ',(caddr e))
	  e))))

; DEFINE-EXTERNAL-ENUMERATION (from prescheme)

(define-syntax define-external-enumeration
  (lambda (form rename compare)
    (let* ((name (cadr form))
	   (symbol->upcase-string
	    (lambda (s)
	      (list->string (map (lambda (c)
				   (if (char=? c #\-)
				       #\_
				       (char-upcase c)))
				 (string->list (symbol->string s))))))
	   (constant
	    (lambda (sym string)
	      `(,(rename 'make-external-constant) ',name ',sym ,string)))
	   (conc (lambda things
		   (string->symbol (apply string-append
					  (map (lambda (thing)
						 (if (symbol? thing)
						     (symbol->string thing)
						     thing))
					       things)))))
	   (var-name
	    (lambda (sym)
	      (conc name "/" sym)))
	   (components
	    (list->vector
	     (map (lambda (stuff)
		    (if (pair? stuff)
			(cons (car stuff)
			      (var-name (car stuff)))
			(cons stuff
			      (var-name stuff))))
		  (caddr form))))
	   (%define (rename 'define))
	   (%define-syntax (rename 'define-syntax))
	   (%begin (rename 'begin))
	   (%quote (rename 'quote))
	   (%make-external-constant (rename 'make-external-constant))
	   (e-name (conc name '- 'enumeration))
	   (count (vector-length components)))
      `(,%begin
	(,%define-syntax ,name
	    (let ((components ',components))
	      (lambda (e r c)
		(let ((key (cadr e)))
		  (cond ((c key 'enum)
			 (let ((which (caddr e)))
			   (let loop ((i 0)) ;vector-posq
			     (if (< i ,count)
				 (if (c which (car (vector-ref components i)))
				     (r (cdr (vector-ref components i)))
				     (loop (+ i 1)))
				 ;; (syntax-error "unknown enumerand name"
				 ;;               `(,(cadr e) ,(car e) ,(caddr e)))
				 e))))
			(else e))))))
	(,%define ,(conc name '- 'count) ,count)
        . ,(map (lambda (stuff)
		  (if (pair? stuff)
		      `(,%define ,(var-name (car stuff))
			   (,%make-external-constant ',name
						     ',(car stuff)
						     ,(cadr stuff)))
		      `(,%define ,(var-name stuff)
			   (,%make-external-constant ',name
						     ',stuff
						     ,(symbol->upcase-string stuff)))))
		(caddr form)))))
    (begin define define-syntax quote external make-external-constant))
