; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-structure vm-data vm-data-interface
  (open scheme-level-1 enumerated bitwise ascii
        architecture platform prescheme 
	vm-utilities ps-unsigned-integers)
  (files ((".." vm data) data)))

(define-structures ((vm-utilities vm-utilities-interface))
  (open scheme prescheme)
  (files ((".." vm util) vm-utilities))
  (begin
;    (define-syntax assert
;      (lambda (exp rename compare)
;    	0))
    (define (assert x)
      (if (not x)
    	  (error "assertion failed")))
   ))
