; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; define-enumeration macro

(define-syntax define-enumeration
  (lambda (form rename compare)
    (let ((name (cadr form))
	  (parts (caddr form))
	  (conc (lambda things
		  (string->symbol (apply string-append
					 (map (lambda (thing)
						(if (symbol? thing)
						    (symbol->string thing)
						    thing))
					      things)))))
	  (%define (rename 'define))
	  (%begin (rename 'begin))
	  (%quote (rename 'quote)))
      (do ((i 0 (+ i 1))
	   (p parts (cdr p))
	   (r '() (cons `(,%define ,(conc name "/" (car p)) ,i)
			r)))
	  ((null? p)
	   `(,%begin ,@r
		     (,%define ,(conc name "-" 'count) ,i)
		     ;; ?
		     (,%define ,name (,%quote #(,@parts))))))))
  (begin define quote))

; enum macro

(define-syntax enum
  (lambda (form rename compare)
    (let ((type-name (cadr form))
	  (component-name (caddr form))
	  (%name->enumerand (rename 'name->enumerand))
	  (%quote (rename 'quote)))
      `(,%name->enumerand (,%quote ,component-name)
			  ,type-name)))
  (name->enumerand quote))


(define (name->enumerand e e-type)
  (let loop ((i 0))			;vector-posq
       (if (eq? e (vector-ref e-type i))
	   i
	   (loop (+ i 1)))))