; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Deal with shadowed variables.

; When a variable is shadowed by a variable, split the existing shared
; location into two replacement locations.

; name (structure-ref p name) (define name ...) within a single template
; will lose big.

;(define *replaced-locations* '()) ;alist of (old rep ((uid ...) . new))

(define (shadow-location! old p-uids new replacement)
  (if (location-defined? old)
      (set-contents! replacement (contents old)))
  (set-location-id! old
		    (vector replacement p-uids new))
  (set-location-defined?! old #f))  ;so that exceptions will be raised

(define maybe-replace-location
  (let ((memv memv))
    (lambda (loc p-uid)			;Package's unique id
      (let ((foo (location-id loc)))
	(if (vector? foo)
	    (maybe-replace-location
	     (if (memv p-uid (vector-ref foo 1))
		 (vector-ref foo 2)
		 (vector-ref foo 0))
	     p-uid)
	    loc)))))

; Exception handler:

(define (deal-with-replaced-variables succeed)
  (lambda (opcode reason loc . rest)
    (if (= reason (enum exception undefined-global))
	(deal-with-replaced-variable opcode reason loc rest succeed)
	(apply signal-exception opcode reason loc rest))))

(define (deal-with-replaced-variable opcode reason loc rest succeed)
  (primitive-catch
   (lambda (cont)
     (let* ((tem (continuation-template cont))
	    (index (+ (code-vector-ref (template-code tem)
				       (+ (continuation-pc cont) 2))
		      (* (code-vector-ref (template-code tem)
					  (+ (continuation-pc cont) 1))
			 byte-limit))))
       (if (eq? (template-ref tem index) loc)
	   (let* ((p-uid (do ((env (continuation-env cont)
				   (vector-ref env 0)))
			     ((not (vector? env)) env)))
		  (new (maybe-replace-location loc p-uid)))
	     (if (eq? new loc)
		 (apply signal-exception opcode reason loc rest)
		 (begin (template-set! tem index new)
			;(signal 'note "Replaced location" loc new p-uid)
			(if (location-defined? new)
			    (succeed new rest)
			    (apply signal-exception opcode reason new rest)))))
	   (error "lossage in deal-with-replaced-variables"
		  loc index))))))

(define-exception-handler (enum op global)
  (deal-with-replaced-variables
   (lambda (loc more-args)
     (contents loc))))

(define-exception-handler (enum op set-global!)
  (deal-with-replaced-variables
   (lambda (loc more-args)
     (set-contents! loc (car more-args)))))
