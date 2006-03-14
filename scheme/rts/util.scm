; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file util.scm.

;;;; Utilities

(define (unspecific) (if #f #f))

(define (reduce cons nil list)		;used by length, append, etc.
  (if (null? list)
      nil
      (cons (car list) (reduce cons nil (cdr list)))))

(define (filter pred lst)
  (reduce (lambda (x rest)
	    (if (pred x) (cons x rest) rest))
	  '()
	  lst))

; Position of an object within a list

(define (pos pred)
  (lambda (thing l)
    (let loop ((i 0) (l l))
      (cond ((null? l) #f)
	    ((pred thing (car l)) i)
	    (else (loop (+ i 1) (cdr l)))))))

(define posq (pos eq?))
(define posv (pos eqv?))
(define position (pos equal?))

; Is pred true of any element of l?

(define (any pred l)
  ;; (reduce or #f l), sort of
  (if (null? l)
      #f
      (or (pred (car l))
	  (any pred (cdr l)))))

; Is pred true of every element of l?

(define (every pred l)
  ;; (reduce and #t l), sort of
  (if (null? l)
      #t
      (and (pred (car l)) (every pred (cdr l)))))

(define (sublist l start end)
  (if (> start 0)
      (sublist (cdr l) (- start 1) (- end 1))
      (let recur ((l l) (end end))
	(if (= end 0)
	    '()
	    (cons (car l) (recur (cdr l) (- end 1)))))))

(define (last x)
  (if (null? (cdr x))
      (car x)
      (last (cdr x))))

(define (insert x l <)
  (cond ((null? l) (list x))
        ((< x (car l)) (cons x l))
        (else (cons (car l) (insert x (cdr l) <)))))

;----------------
; Variations on a theme.
;
; FOLD is a tail-recursive version of REDUCE.

(define (fold folder list accumulator)
  (do ((list list (cdr list))
       (accum accumulator (folder (car list) accum)))
      ((null? list)
       accum)))

(define (fold->2 folder list acc0 acc1)
  (let loop ((list list) (acc0 acc0) (acc1 acc1))
    (if (null? list)
	(values acc0 acc1)
	(call-with-values
	 (lambda ()
	   (folder (car list) acc0 acc1))
	 (lambda (acc0 acc1)
	   (loop (cdr list) acc0 acc1))))))

(define (fold->3 folder list acc0 acc1 acc2)
  (let loop ((list list) (acc0 acc0) (acc1 acc1) (acc2 acc2))
    (if (null? list)
	(values acc0 acc1 acc2)
	(call-with-values
	 (lambda ()
	   (folder (car list) acc0 acc1 acc2))
	 (lambda (acc0 acc1 acc2)
	   (loop (cdr list) acc0 acc1 acc2))))))

;----------------
; A version of LET and LET* which allows clauses that return multiple values.
;
; There is another copy of this in big/mvlet.scm.
;
; MV = multiple-value
;
; (mvlet (<clause> ...) <body>)
; (mvlet* (<clause> ...) <body>)
;
; <clause> ::= (<ids> <expression>)
; <ids> ::= <id> | (<id> ...) | (<id> ... . <id>)
;
; A clause of the form (<id> <exp>) is like a normal LET clause.  There is no
; clause equivalent to
;   (call-with-values (lambda () <expression>)
;                     (lambda <id> <body>))

(define-syntax mvlet
  (syntax-rules ()
    ((mvlet () body ...)
     (let () body ...))
    ((mvlet (clause ...) body ...)
     (mvlet-helper (clause ...) () (body ...)))))

(define-syntax mvlet-helper
  (syntax-rules ()
    ((mvlet-helper () clauses (body ...))
     (let clauses body ...))
    ((mvlet-helper (((var . more-vars) val) more ...) clauses body)
     (copy-vars (var . more-vars) () val (more ...) clauses body))
    ((mvlet-helper ((var val) more ...) clauses body)
     (mvlet-helper (more ...) ((var val) . clauses) body))))

(define-syntax copy-vars
  (syntax-rules ()
    ((copy-vars (var . more-vars) (copies ...)
		val more clauses body)
     (copy-vars more-vars (copies ... x)
		val more ((var x) . clauses) body))
    ((copy-vars () copies val more clauses body)
     (call-with-values
       (lambda () val)
       (lambda copies
	 (mvlet-helper more clauses body))))
    ((copy-vars last (copies ...) val more clauses body)
     (call-with-values
       (lambda () val)
       (lambda (copies ... . lastx)
	 (mvlet-helper more ((last lastx) . clauses) body))))))

(define-syntax mvlet*
  (syntax-rules ()
    ((mvlet* () body ...)
     (let () body ...))
    ((mvlet* (((vars ...) val) clause ...) body ...)
     (call-with-values
       (lambda () val)
       (lambda (vars ...)
	 (mvlet (clause ...) body ...))))
    ((mvlet* ((var val) clause ...) body ...)
     (let ((var val)) (mvlet (clause ...) body ...)))))

