; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; --------------------

; Redefinitions of some usual scheme things so as to make the new
; exception system kick in when it needs to.

(define (number? n)
  (or ((structure-ref scheme number?) n)
      (extended-number? n)))

;(define (integer? n)
;  (if ((structure-ref scheme number?) n)
;      ((structure-ref scheme integer?) n)
;      (and (extended-number? n)
;           ... raise exception ...)))

(define (+ x y) ((structure-ref scheme +) x y))
(define (* x y) ((structure-ref scheme *) x y))
(define (- x y) ((structure-ref scheme -) x y))
(define (/ x y) ((structure-ref scheme /) x y))
(define (= x y) ((structure-ref scheme =) x y))
(define (< x y) ((structure-ref scheme <) x y))
(define (make-vector x y) ((structure-ref scheme make-vector) x y))
(define (make-string x y) ((structure-ref scheme make-string) x y))
(define (apply x y) ((structure-ref scheme apply) x y))

(define (read-char x) ((structure-ref scheme read-char) x))
(define (peek-char x) ((structure-ref scheme peek-char) x))
(define (char-ready? x) ((structure-ref scheme char-ready?) x))
(define (write-char x y) ((structure-ref scheme write-char) x y))


; --------------------

; Fake interrupt and exception system.
; This needs to be reconciled with alt/primitives.scm.

(define (with-exceptions thunk)
  (with-handler
       (lambda (c punt)
	 (cond ((and (exception? c)
		     (procedure? (get-exception-handler)))
		(handle-exception-carefully c))
	       ((interrupt? c)
		(if (not (deal-with-interrupt c))
		    (punt)))
	       ;; ((vm-return? c)
	       ;;  (vm-return (cadr c)))
	       (else
		(punt))))
     thunk))

(define (handle-exception-carefully c)
  (display "(Exception: ") (write c) (display ")") (newline)
  (noting-exceptional-context c
    (lambda ()
      (raise-exception (exception-opcode c)
		       (exception-arguments c)))))

(define (noting-exceptional-context c thunk)
  (call-with-current-continuation
    (lambda (k)
      ;; Save for future inspection, just in case.
      (set! *exceptional-context* (cons c k))
      (thunk))))

(define *exceptional-context* #f)

(define (deal-with-interrupt c)
  (noting-exceptional-context c
    (lambda ()
      (maybe-handle-interrupt
       (if (and (pair? (cdr c)) (integer? (cadr c)))
	   (cadr c)
	   (enum interrupt keyboard))))))

; (define (poll-for-interrupts) ...)


; Get the whole thing started

(define (?start-with-exceptions entry-point arg)
  (with-exceptions
   (lambda ()
     (?start entry-point arg))))

(define (in struct form)
  (eval form (structure-package struct)))


; Fake linker to make scripts.scm possible

(define (link-simple-system filename resumer-exp . structs)
  (link-system structs (lambda () resumer-exp) filename))

(define (link-reified-system some filename make-resumer-exp . structs)
  (link-system (append structs (map (lambda (name+struct)
				      (if (pair? name+struct)
					  (cdr name+struct)
					  name+struct))
				    some))
	       (lambda ()
		 (display "Reifying") (newline)
		 `(,make-resumer-exp
		   (lambda ()
		     ,(reify-structures some
					(lambda (loc) loc)))))
	       filename))


(define (link-system structs make-resumer filename)
  (for-each ensure-loaded structs)
  (let* ((p (make-simple-package structs eval #f))
	 (r (eval (make-resumer) p)))
    (check-package p)
    r))

(define (check-package p)
  (let ((names (undefined-variables p)))
    (if (not (null? names))
	(begin (display "Undefined: ") 
	       (write names) (newline)))))

(define-syntax struct-list
  (syntax-rules ()
    ((struct-list name ...) (list (cons 'name name) ...))))
