; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


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
      (or (pred (car l)) (any pred (cdr l)))))

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
