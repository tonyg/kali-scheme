; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

; (make-vector size init)

(define-c-generator make-vector #t
  (lambda (call port indent)
    (let ((type (node-type call)))
      (write-c-coercion type port)
      (format port "malloc(sizeof(")
      (display-c-type (pointer-type-to type) #f port)
      (format port ") * ")
      (c-value (call-arg call 0) port)
      (format port ")"))))

(define-c-generator vector-ref #t
  (lambda (call port indent)
    (generate-c-vector-ref (call-arg call 0) (call-arg call 1) port)))

(define (generate-c-vector-ref vector index port)
  (display "*(" port)
  (c-value vector port)
  (display " + " port)
  (c-value index port)
  (writec port #\)))

(define-c-generator vector-set! #t
  (lambda (call port indent)
    (generate-c-vector-set (call-arg call 1)
			   (call-arg call 2)
			   (call-arg call 3)
			   port indent)))

(define (generate-c-vector-set vector index value port indent)
  (indent-to port indent)
  (generate-c-vector-ref vector index port)
  (display " = " port)
  (c-value value port)
  (writec port #\;))

(define-c-generator make-string #t
  (lambda (call port indent)
    ; calloc is used as a hack to get a zero at the end
    (format port "(char *)calloc( 1, 1 + ")
    (c-value (call-arg call 0) port)
    (format port ")")))

(define-c-generator string-length #t
  (lambda (call port indent)
    (format port "strlen((char *) ")
    (c-value (call-arg call 0) port)
    (format port ")")))

(define-c-generator string-ref #t
  (lambda (call port indent)
    (generate-c-vector-ref (call-arg call 0) (call-arg call 1) port)))

(define-c-generator string-set! #f
  (lambda (call port indent)
    (generate-c-vector-set (call-arg call 1)
			   (call-arg call 2)
			   (call-arg call 3)
			   port indent)))

(define-c-generator make-record #f
  (lambda (call port indent)
    (let ((type (get-record-type (literal-value (call-arg call 0)))))
      (write-c-coercion type port)
      (format port "malloc(sizeof(struct ")
      (write-c-identifier (record-type-name type) port)
      (format port "))"))))

(define-c-generator record-ref #t
  (lambda (call port indent)
    (generate-c-record-ref (call-arg call 0)
			   (call-arg call 1)
			   (call-arg call 2)
			   port)))

(define (generate-c-record-ref record type field port)
  (let ((field (get-record-type-field (literal-value type)
				      (literal-value field))))
    (c-value record port)
    (display "->" port)
    (write-c-identifier (record-field-name field) port)))

(define-c-generator record-set! #t
  (lambda (call port indent)
    (generate-c-record-set (call-arg call 1)
			   (call-arg call 2)
			   (call-arg call 3)
			   (call-arg call 4)
			   port indent)))

(define (generate-c-record-set record value type field port indent)
  (indent-to port indent)
  (generate-c-record-ref record type field port)
  (display " = " port)
  (c-value value port)
  (writec port #\;))

(define-c-generator allocate-memory #t
  (lambda (call port indent)
    (write-c-coercion type/address port)
    (format port "malloc(") 
    (c-value (call-arg call 0) port)
    (format port ")")))

(define-c-generator deallocate #t
  (lambda (call port indent)
    (format port "free(") 
    (c-value (call-arg call 0) port)
    (format port ")")))

(define-c-generator deallocate-memory #t
  (lambda (call port indent)
    (format port "free(") 
    (c-value (call-arg call 0) port)
    (format port ")")))

(define-c-generator address+ #t
  (lambda (call port indent)
    (simple-c-primop "+" call port)))
  
(define-c-generator address-difference #t
  (lambda (call port indent)
    (simple-c-primop "-" call port)))

(define-c-generator address= #t
  (lambda (call port indent)
    (simple-c-primop "==" call port)))

(define-c-generator address< #t
  (lambda (call port indent)
    (simple-c-primop "<" call port)))

(define-c-generator address->integer #t
  (lambda (call port indent)
    (format port "((long) ") 
    (c-value (call-arg call 0) port)
    (format port ")")))

(define-c-generator integer->address #t
  (lambda (call port indent)
    (format port "((char *) ") 
    (c-value (call-arg call 0) port)
    (format port ")")))

(define-c-generator copy-memory! #t
  (lambda (call port indent)
    (format port "memcpy((void *)") 
    (c-value (call-arg call 1) port)
    (format port ", (void *)") 
    (c-value (call-arg call 0) port)
    (format port ",") 
    (c-value (call-arg call 2) port)
    (format port ")")))

(define-c-generator memory-equal? #t
  (lambda (call port indent)
    (format port "(!memcmp((void *)") 
    (c-value (call-arg call 1) port)
    (format port ", (void *)") 
    (c-value (call-arg call 0) port)
    (format port ",") 
    (c-value (call-arg call 2) port)
    (format port "))")))

(define-c-generator byte-ref #t
  (lambda (call port indent)
    (generate-c-memory-ref "unsigned char" (call-arg call 0) port)))

(define-c-generator word-ref #t
  (lambda (call port indent)
    (generate-c-memory-ref "long" (call-arg call 0) port)))

(define (generate-c-memory-ref type pointer port)
  (format port "*((~A *) " type)
  (c-value pointer port)
  (writec port #\)))

(define-c-generator byte-set! #t
  (lambda (call port indent)
    (generate-c-memory-set! "unsigned char"
			    (call-arg call 1)
			    (call-arg call 2)
			    port
			    indent)))

(define-c-generator word-set! #t
  (lambda (call port indent)
    (generate-c-memory-set! "long"
			    (call-arg call 1)
			    (call-arg call 2)
			    port
			    indent)))

(define (generate-c-memory-set! type pointer value port indent)
  (indent-to port indent)
  (generate-c-memory-ref type pointer port)
  (display " = " port)
  (c-value value port)
  (writec port #\;))

(define-c-generator char-pointer->string #t
  (lambda (call port indent)
    (format port "((char *)") 
    (c-value (call-arg call 0) port)
    (format port ")")))

(define-c-generator char-pointer->nul-terminated-string #t
  (lambda (call port indent)
    (format port "((char *)") 
    (c-value (call-arg call 0) port)
    (format port ")")))

(define-c-generator computed-goto #f
  (lambda (call port indent)
    (generate-c-switch call port indent)))

(define (generate-c-switch call port indent)
  (let ((size (call-exits call)))
    (indent-to port indent)
    (display "switch (" port)
    (c-value (call-arg call (+ size 1)) port)
    (display ") {" port)
    (let ((indent (+ indent 2)))
      (do ((i 0 (+ i 1))
	   (labels (literal-value (call-arg call size)) (cdr labels)))
	  ((>= i size))
	(for-each (lambda (l)
		    (indent-to port indent)
		    (format port "case ~D : " l))
		  (car labels))
	(write-c-switch-case (call-arg call i) port indent)))
    (indent-to port indent)
    (display "}" port)))

(define (write-c-switch-case node port indent)
  (writec port #\{)
  (write-c-block (lambda-body node) port (+ indent 2))
  (indent-to port (+ indent 2))
  (display "break;" port))
