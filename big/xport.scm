; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Extensible ports

; Input ports

(define-record-type extensible-input-port
  (local-data
   methods)
  ())

(define make-extensible-input-port extensible-input-port-maker)

(define-record-type input-port-methods
  (close-port
   read-char
   peek-char
   char-ready?
   current-column
   current-row
   )
  ())

(define make-input-port-methods input-port-methods-maker)

; Output ports

(define-record-type extensible-output-port
  (local-data
   methods)
  ())

(define make-extensible-output-port extensible-output-port-maker)

(define-record-type output-port-methods
  (close-port
   write-char
   write-string
   force-output
   fresh-line
   current-column
   current-row
   )
  ())

(define make-output-port-methods output-port-methods-maker)

; Operations

; CLOSE-PORT must work on both types of extensible ports.

(define-exception-handler (enum op close-port)
  (lambda (opcode args)
    (let ((port (car args)))
      (cond ((extensible-input-port? port)
	     ((input-port-methods-close-port
	       (extensible-input-port-methods port))
	      (extensible-input-port-local-data port)))
	    ((extensible-output-port? port)
	     ((output-port-methods-close-port
	       (extensible-output-port-methods port))
	      (extensible-output-port-local-data port)))
	    (else
	     (raise-port-exception opcode args))))))

(define (raise-port-exception opcode args)
  (signal-exception opcode args))

; Predicates
; These won't work as the VM does not raise an exception when predicates are
; applied to records.

;(define-exception-handler (enum op input-port?)
;  (lambda (opcode args)
;    (extensible-input-port? (car args))))

;(define-exception-handler (enum op output-port?)
;  (lambda (opcode args)
;    (extensible-output-port? (car args))))

; These will work for any code loaded subsequently...

(define (input-port? thing)
  (or ((structure-ref ports input-port?) thing)
      (extensible-input-port? thing)))

(define (output-port? thing)
  (or ((structure-ref ports output-port?) thing)
      (extensible-output-port? thing)))

; Other methods

(define (define-input-port-method op method)
  (define-exception-handler op
    (lambda (opcode args)
      (let ((port (car args)))
	(if (extensible-input-port? port)
	    ((method (extensible-input-port-methods port))
	     (extensible-input-port-local-data port))
	    (raise-port-exception opcode args))))))
  
(define-input-port-method (enum op read-char) input-port-methods-read-char)
(define-input-port-method (enum op peek-char) input-port-methods-peek-char)
(define-input-port-method (enum op char-ready?) input-port-methods-char-ready?)

(define (define-output-port-method op arg-count method)
  (define-exception-handler op
    (case arg-count
      ((0)
       (lambda (opcode args)
	 (let ((port (car args)))
	   (if (extensible-output-port? port)
	       ((method (extensible-output-port-methods port))
		(extensible-output-port-local-data port))
	       (raise-port-exception opcode args)))))
      ((1)
       (lambda (opcode args)
	 (let ((port (cadr args)))
	   (if (extensible-output-port? port)
	       ((method (extensible-output-port-methods port))
		(extensible-output-port-local-data port)
		(car args))
	       (raise-port-exception opcode args))))))))
  
(define-output-port-method (enum op write-char)
  1 output-port-methods-write-char)
(define-output-port-method (enum op write-string)
  1 output-port-methods-write-string)
(define-output-port-method (enum op force-output)
  0 output-port-methods-force-output)

(define (make-new-port-method id input-method output-method default)
  (lambda (port)
    (cond ((extensible-input-port? port)
	   ((input-method (extensible-input-port-methods port))
	    (extensible-input-port-local-data port)))
	  ((extensible-output-port? port)
	   ((output-method (extensible-output-port-methods port))
	    (extensible-output-port-local-data port)))
	  (else
	   (default port)))))

(define current-column
  (make-new-port-method 'current-column
			input-port-methods-current-column
			output-port-methods-current-column
			(lambda (port) #f)))

(define current-row
  (make-new-port-method 'current-row
			input-port-methods-current-row
			output-port-methods-current-row
			(lambda (port) #f)))

(define (make-new-output-port-method id method default)
  (lambda (port)
    (if (extensible-output-port? port)
	((method (extensible-output-port-methods port))
	 (extensible-output-port-local-data port))
	(default port))))

(define fresh-line
  (make-new-output-port-method 'fresh-line
			       output-port-methods-fresh-line
			       newline))
									 


(define force-output (structure-ref ports force-output))
