; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; A "family" is a pair  family-id * precedence
; A "method" is a procedure  args -> (value + fail)
; A "dispatch table" is a pair  (list-of method) * (list-of family)

; Families have precedences in order to give some control over the order
; in which methods are tried.  Each method decides for itself whether it
; is applicable; if not, then the next method is tried.


; Methods aren't called tail-recursively... what a drag...

(define (make-family id prec) (cons id prec))
(define family-identity car)
(define family-precedence cdr)

(define family-of-last-resort (make-family 'last-resort -1000001)) ;infinity
(define default-family	     (make-family 'default     -1000000))


(define (same-family? c1 c2)
  (eqv? (family-identity c1) (family-identity c2)))

(define (make-method-table id)
  (cons (list (lambda args
                (apply call-error "invalid or unimplemented operation"
		       id args)))
        (list family-of-last-resort)))

(define method-table-methods car)
(define method-table-families cdr)

(define (define-method disp family method)
  (let ((displace! (lambda (z x y) (set-car! z x) (set-cdr! z y)))
        (prec (family-precedence family)))
    (let loop ((ms (method-table-methods disp))
	       (cs (method-table-families disp)))
      (cond ((> prec (family-precedence (car cs)))
             (displace! ms method (cons (car ms) (cdr ms)))
             (displace! cs family  (cons (car cs) (cdr cs))))
            ((same-family? family (car cs))
             (set-car! ms method)
	     (set-car! cs family))
            (else
             (loop (cdr ms) (cdr cs)))))))

(define (define-default-method disp proc)
  (define-method disp default-family proc))

(define (define-last-method disp proc)
  (define-method disp family-of-last-resort proc))

; Generic functions

(define (make-generic disp)
  (let ((ms (method-table-methods disp)))
    (lambda args
      (perform-dispatch ms args))))

(define (perform-dispatch ms args)
  (if (null? (cdr ms))
      (apply (car ms) args)
      (let ((result (apply (car ms) args)))
	(if (eq? result *fail*)
	    (perform-dispatch (cdr ms) args)
	    result))))

(define *fail* (cons '*fail* '()))
(define (fail) *fail*)

; A generic procedure for producing printed representations.
; Should return one of
;   - A list (symbol info ...), to be printed as #{Symbol info ...}
;   - #f, meaning no information available on how to print.
; This is intended to be used not only by write and display, but also by
; the pretty printer.

(define disclose-methods (make-method-table 'disclose))

(define disclose (make-generic disclose-methods))

(define-default-method disclose-methods (lambda (obj) #f))


; Special hack for exception handlers

(define (make-generic-exception-handler table opcode signal-exception)
  (define-last-method table		;Make failures tail-recur
    (lambda args (signal-exception opcode args)))
  (let ((ms (method-table-methods table)))
    (lambda (opcode args)
      (perform-dispatch ms args))))
