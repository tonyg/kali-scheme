; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Deal with shadowed variables.

; When a variable is shadowed by a variable, split the existing shared
; location into two replacement locations.

; name (structure-ref p name) (define name ...) within a single template
; will lose big.

(define *replaced-locations* '()) ;alist of (old rep ((uid ...) . new))

(define (shadow-location! old p-uids new replacement)
  (if (and (location-defined? old)
	   (location-assigned? old))
      (set-contents! replacement (contents old)))
  (let ((bar (cdr (or (assq old *replaced-locations*)
		      (let ((foo (list old replacement)))
			(set! *replaced-locations*
			      (cons foo *replaced-locations*))
			foo)))))
    (set-cdr! bar (cons (cons p-uids new) (cdr bar)))
    (set-location-defined?! old #f)))  ;so that exceptions will be raised

(define maybe-replace-location
  (let ((assq assq)	;paranoia
	(memv memv))
    (lambda (loc p-uid)			;a package's unique id
      (let ((probe (assq loc *replaced-locations*)))
	(if probe
	    (let ((bar (cdr probe)))
	      (let loop2 ((frobs (cdr bar)))
		(if (eq? frobs '())	;(null? frobs)
		    (maybe-replace-location (car bar) p-uid)
		    (if (memv p-uid (car (car frobs)))
			(maybe-replace-location (cdr (car frobs)) p-uid)
			(loop2 (cdr frobs))))))
	    loc)))))

; Exception handler:

(define (deal-with-replaced-variables succeed)
  (lambda (opcode args)
    (primitive-catch
     (lambda (cont)
       (let* ((loc (car args))
	      (tem (continuation-template cont))
	      (index (code-vector-ref (template-code tem)
				      (- (continuation-pc cont) 1))))
	 (if (eq? (template-ref tem index) loc)
	     (let* ((p-uid (do ((env (continuation-env cont)
				     (vector-ref env 0)))
			       ((not (vector? env)) env)))
		    (new (maybe-replace-location loc p-uid)))
	       (if (eq? new loc)
		   (signal-exception opcode args)
		   (begin (template-set! tem index new)
			  (signal 'note "Replaced location" loc new p-uid)
			  (if (location-defined? new)
			      (succeed new (cdr args))
			      (signal-exception opcode
						(cons new (cdr args)))))))
	     (error "lossage in deal-with-replaced-variables"
		    loc index)))))))

(let ((op/global (enum op global))
      (op/set-global! (enum op set-global!)))

  (define-exception-handler op/global
    (deal-with-replaced-variables
       (lambda (loc more-args)
	 (if (location-assigned? loc)
	     (contents loc)
	     (signal-exception op/global (cons loc more-args))))))

  (define-exception-handler op/set-global!
    (deal-with-replaced-variables
       (lambda (loc more-args)
	 (set-contents! loc (car more-args))))))
