; Copyright (c) 1994 Richard Kelsey.  See file COPYING.


; Code generation for primops.

(define-record-type c-primop
  (
   simple?
   generate
   )
  ())

(define (simple-c-primop? primop)
  (c-primop-simple? (primop-code-data primop)))

(define (primop-generate-c primop call port indent)
  ((c-primop-generate (primop-code-data primop))
   call port indent))

(define-syntax define-c-generator
  (lambda (exp r$ c$)
    (destructure (((ignore id simple? generate) exp))
      `(set-primop-code-data!
	(,(r$ 'get-prescheme-primop) ',id)
	(,(r$ 'c-primop-maker)
	 ,simple?
	 ,generate
	 )))))
	 

