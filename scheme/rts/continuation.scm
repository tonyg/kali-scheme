; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Continuations

(define (make-ref index)
  (lambda (c)
    (continuation-ref c index)))

(define continuation-cont          (make-ref continuation-cont-index))
(define real-continuation-code     (make-ref continuation-code-index))
(define real-continuation-pc       (make-ref continuation-pc-index))
(define vm-exception-cont-pc       (make-ref exception-cont-pc-index))
(define vm-exception-cont-code     (make-ref exception-cont-code-index))

; This one is exported
(define vm-exception-continuation-exception
  (make-ref exception-cont-exception-index))

; Exception continuations contain the state of the VM when an exception occured.

(define (vm-exception-continuation? thing)
  (and (continuation? thing)
       (= 13 (real-continuation-pc thing))
       (let ((code (real-continuation-code thing)))
	 (and (= 1				; one return value
		 (code-vector-ref code 14))
	      (= (enum op return-from-exception)
		 (code-vector-ref code 15))))))

(define (call-with-values-continuation? thing)
  (and (continuation? thing)
       (= 13 (real-continuation-pc thing))
       (= call-with-values-protocol
	  (code-vector-ref (real-continuation-code thing)
			   14))))

(define (continuation-pc c)
  (if (vm-exception-continuation? c)
      (vm-exception-cont-pc c)
      (real-continuation-pc c)))

(define (continuation-code c)
  (if (vm-exception-continuation? c)
      (vm-exception-cont-code c)
      (real-continuation-code c)))

; This finds the template if it is in the continuation.  Not all continuations
; have templates.

(define (continuation-template c)
  (cond
   ((and (call-with-values-continuation? c)
	 (closure? (continuation-arg c 0)))
    (closure-template (continuation-arg c 0)))
   ((let loop ((i 0))
      (if (= i (continuation-length c))
	  #f
	  (let ((value (continuation-ref c i)))
	    (if (and (template? value)
		     (eq? (template-code value)
			  (continuation-code c)))
		value
		(loop (+ i 1)))))))
   ;; look among the primops for the template this continuation
   ;; belongs to
   (else
    (let ((code (continuation-code c)))
      (let loop ((i (vector-length all-operators)))
	(if (zero? i)
	    #f
	    (let* ((primitive-proc (vector-ref all-operators (- i 1)))
		   (primitive-template (closure-template primitive-proc)))
	      (if (eq? code (template-code primitive-template))
		  primitive-template
		  (loop (- i 1))))))))))

; Accessing the saved operand stack.

(define (continuation-arg c i)
  (continuation-ref c (+ continuation-cells
			 (if (vm-exception-continuation? c)
			     exception-continuation-cells
			     0)
			 i)))

(define (continuation-arg-count c)
  (- (continuation-length c)
     (+ continuation-cells
	(if (vm-exception-continuation? c)
	    exception-continuation-cells
	    0))))

(define-simple-type :continuation (:value) continuation?)

(define-method &disclose ((obj :continuation))
  (list (if (vm-exception-continuation? obj)
	    'vm-exception-continuation
	    'continuation)
	`(pc ,(continuation-pc obj))
	(let ((template (continuation-template obj)))
	  (if template
	      (template-info template)
	      '?))))

(define (continuation-preview c)
  (if (continuation? c)
      (cons (cons (let ((template (continuation-template c)))
		    (if template
			(template-info template)
			'?))
		  (continuation-pc c))
	    (continuation-preview (continuation-cont c)))
      '()))
