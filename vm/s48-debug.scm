; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Debugging stuff for Scheme48

(define-syntax import
  (lambda (form compare rename)
    (let ((env (cadr form))
	  (names (cddr form)))
      `(begin . ,(map (lambda (n)
			(if (pair? n)
			    `(define ,(car n)
			       ((access-scheme48 'environment-ref)
				,env
				',(cadr n)))
			    `(define ,n
			       ((access-scheme48 'environment-ref)
				,env
				',n))))
		      names)))))

(define vm-environment ((access-scheme48 'name->environment) 'vm))

