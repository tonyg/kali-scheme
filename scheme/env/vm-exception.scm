; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; More precise conditions for VM exceptions.

; GLOBAL and SET-GLOBAL! are in shadow.scm.

(let ((handler (lambda (opcode reason proc . rest)
		 (signal-vm-exception
		  opcode reason
		  (map value->expression (cons proc rest))))))
  (define-vm-exception-handler (enum op call) handler)
  (define-vm-exception-handler (enum op tail-call) handler)
  (define-vm-exception-handler (enum op big-call) handler))

(define-vm-exception-handler (enum op with-continuation)
  (lambda (opcode reason val)
    (signal-vm-exception opcode reason (value->expression val))))

(let ((handler (lambda (opcode reason . args)
		 (signal-vm-exception
		  opcode reason
		  (cons 'apply (map value->expression args))))))
  (define-vm-exception-handler (enum op apply) handler)
  (define-vm-exception-handler (enum op closed-apply) handler))

(let ((handler (lambda (opcode reason proc args)
		 (signal-condition
		  (condition
		   (construct-vm-exception opcode reason)
		   (make-assertion-violation)
		   (cond (proc
			  (condition
			   (make-message-condition "returning wrong number of values")
			   (make-irritants-condition (list (cons proc args))))) 
			 ((null? args)
			  (condition
			   (make-message-condition
			    "returning zero values when one is expected")
			   (make-who-condition 'values)
			   (make-irritants-condition (list '(values)))))
			 (else
			  (condition
			   (make-message-condition "returning wrong number of values")
			   (make-who-condition 'values)
			   (make-irritants-condition
			    (list (error-form 'values args)))))))))))
  (define-vm-exception-handler (enum op return) handler)
  (define-vm-exception-handler (enum op values) handler)
  (define-vm-exception-handler (enum op closed-values) handler))

 (let ((handler 
	(lambda (opcode reason thing type-byte offset . rest)
	  (let* ((data (assq (enumerand->name type-byte stob)
			     stob-data))
		 (who
		  ((if (= opcode
			  (enum op stored-object-ref))
		       car
		       cadr)
		   (list-ref data (+ offset 3)))))
	    (signal-condition
	     (condition
	      (construct-vm-exception opcode reason)
	      (make-assertion-violation)
	      (make-message-condition (vm-exception-reason->message reason))
	      (make-who-condition who)
	      (make-irritants-condition
	       (list
		(error-form who (cons thing rest))))))))))
  (define-vm-exception-handler (enum op stored-object-ref) handler)
  (define-vm-exception-handler (enum op stored-object-set!) handler))

(define-vm-exception-handler (enum op make-vector-object)
  (lambda (opcode reason type . rest)
    (let* ((type-name (enumerand->name type stob))
	   (maker
	    (string->symbol
	     ;; Don't simplify this to "make-"  --JAR
	     (string-append (symbol->string 'make-)
			    (symbol->string type-name)))))
      (signal-condition
       (condition
	(construct-vm-exception opcode reason)
	(make-assertion-violation)
	(make-who-condition maker)
	(make-message-condition (vm-exception-reason->message reason))
	(make-irritants-condition
	 (list
	  (error-form maker rest))))))))

(define (vector-vm-exception-handler suffix)
  (lambda (opcode reason thing type . rest)
    (let* ((type-name (enumerand->name type stob))
	   (maker
	    (string->symbol
	     (string-append (symbol->string type-name)
			    "-"
			    (symbol->string suffix)))))
      (signal-condition
       (condition
	(construct-vm-exception opcode reason)
	(make-assertion-violation)
	(make-who-condition maker)
	(make-message-condition (vm-exception-reason->message reason))
	(make-irritants-condition
	 (list (error-form maker (cons thing rest)))))))))

(define-vm-exception-handler (enum op stored-object-length)
  (vector-vm-exception-handler 'length))

(define-vm-exception-handler (enum op stored-object-indexed-ref)
  (vector-vm-exception-handler 'ref))

(define-vm-exception-handler (enum op stored-object-indexed-set!)
  (vector-vm-exception-handler 'set!))

(define-vm-exception-handler (enum op scalar-value->char)
  (lambda (opcode reason value)
    (signal-vm-exception opcode reason
			 `(scalar-value->char ,(value->expression value)))))

(define-vm-exception-handler (enum op close-channel)
  (lambda (opcode reason channel status . rest)
    (apply signal-i/o-error opcode reason channel status rest)))

(define-vm-exception-handler (enum op channel-ready?)
  (lambda (opcode reason channel status . rest)
    (apply signal-i/o-error opcode reason channel status rest)))

(define (signal-i/o-error opcode reason channel status . rest)
  (enum-case exception reason
	     ((os-error)
	      (signal-condition
	       (condition
		(construct-vm-exception opcode reason)
		(make-i/o-error)
		(make-who-condition (if reason
				     (enumerand->name reason exception)
				     #f))
		(make-message-condition
		 (os-string->string (byte-vector->os-string (os-error-message status))))
		(make-irritants-condition (cons channel rest)))))
	     (else
	      (apply signal-vm-exception opcode reason channel status rest))))

(define-vm-exception-handler (enum op write-image-low)
  (lambda (opcode reason status filename . rest)
    (enum-case exception reason
	       ((os-error)
		(signal-condition
		 (condition
		  (construct-vm-exception opcode reason)
		  (make-i/o-error)
		  (make-who-condition 'write-image)
		  (make-message-condition
		   (os-string->string (byte-vector->os-string (os-error-message status))))
		  (make-irritants-condition
		   (cons (byte-vector->os-string filename)
			 rest)))))
	       (else
		(apply signal-vm-exception opcode reason status rest)))))

; Utilities

(define (construct-vm-exception opcode reason)
  (make-vm-exception opcode
		     (if reason
			 (enumerand->name reason exception)
			 #f)))
