; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-macro define-syntax
  (lambda (form expander)
    (expander `(define-macro ,(cadr form)
		 (let ((transformer ,(caddr form)))
		   (lambda (form expander)
		     (expander (transformer form
					    (lambda (x) x)
					    eq?)
			       expander))))
	      expander)))

