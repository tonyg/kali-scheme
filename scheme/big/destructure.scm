; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is a destructuring version of LET.
; (DESTRUCTURE ((<pattern> <expression>) ...) body ...)
; The patterns can be:
;   identifiers, which are bound to the corresponding part of the value
;   lists of patterns (including dotted pairs)
;   vectors of patterns
;
; Bug (?): (destructure (((a) '(1 2))) ...) works.  The code does not check
; to see if there are more elements than the minimum number required.

(define-syntax destructure
  (lambda (form rename compare)
    (let ((specs (cadr form))
	  (body (cddr form))
	  (%car (rename 'car))
	  (%cdr (rename 'cdr))
	  (%vref (rename 'vector-ref))
	  (%let* (rename 'let*))
	  (gensym (lambda (i)
		    (rename (string->symbol
			     (string-append "x" (number->string i))))))
	  (atom? (lambda (x) (not (pair? x)))))
      (letrec ((expand-pattern
		(lambda (pattern value i)
		  (cond ((or (not pattern) (null? pattern))
			 '())
			((vector? pattern)
			 (let ((xvalue (if (atom? value)
					   value
					   (gensym i))))
			   `(,@(if (eq? value xvalue) '() `((,xvalue ,value)))
			     ,@(expand-vector pattern xvalue i))))
			((atom? pattern)
			 `((,pattern ,value)))
			(else
			 (let ((xvalue (if (atom? value)
					   value
					   (gensym i))))
			   `(,@(if (eq? value xvalue) '() `((,xvalue ,value)))
			     ,@(expand-pattern (car pattern)
					       `(,%car ,xvalue)
					       (+ i 1))
			     ,@(if (null? (cdr pattern))
 				   '()
				   (expand-pattern (cdr pattern)
						   `(,%cdr ,xvalue)
						   (+ i 1)))))))))
	       (expand-vector
		(lambda (vec xvalue i)
		  (do ((j (- (vector-length vec) 1) (- j 1))
		       (ps '() (append (expand-pattern (vector-ref vec j)
						       `(,%vref ,xvalue ,j)
						       (+ i 1))
				       ps)))
		      ((< j 0) ps)))))
	(do ((specs specs (cdr specs))
	     (res '() (append (expand-pattern (caar specs) (cadar specs) 0)
			      res)))
	    ((null? specs)
	     `(,%let* ,res . ,body)))))))

