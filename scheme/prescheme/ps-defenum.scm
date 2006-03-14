; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; DEFINE-ENUMERATION macro hacked to use external (C enumeration) names
; instead of integers.

;(define-external-enumeration bing
;  ((foo "BAR")
;   baz))
;  =>
;(define-syntax bing ...)
;(define bing/foo (make-external-constant 'bing 'foo "BAR"))
;(define bing/baz (make-external-constant 'bing 'baz "BAZ"))
;
;(enum bing foo) => bing/foo

(define-record-type external-constant :external-constant
  (make-external-constant enum-name name c-string)
  external-constant?
  (enum-name external-constant-enum-name)
  (name external-constant-name)
  (c-string external-constant-c-string))

(define-record-discloser :external-constant
  (lambda (external-const)
    (list 'enum
	  (external-constant-enum-name external-const)
	  (external-constant-name external-const))))

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


