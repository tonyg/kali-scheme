; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.

(define-data-type list
  (pair? (cons car cdr)
	 (car integer car set-car!)
	 (cdr list    cdr set-cdr!))
  (null? null))

; Also want pair->list (but probably not null->list).
; That means that PAIR is a separate type, which is not what ML does.
; Does the constructor make a pair or a list?  Two constructors?
; The minimal version needs the pair-maker and pair->list.

(define-data-type list
  (pair pair->list
	(make-pair car cdr)
	(car integer car set-car!)
	(cdr list    cdr set-cdr!))
  (null))

(define (cons x y)
  (pair->list (make-pair x y)))

; Could write it this way from scratch.

(define-record-type :pair
  (make-pair car cdr)
  (car integer car set-car!)
  (cdr list    cdr set-cdr!))

(define-data-type :list
  (pair :pair)
  (null? null))

; pair->list needs to cons, especially if there are multiple options.
; This does show that the basic idea is sound - only the implementation
; changes from ML.  Polymorphic lists would be tough this way.

(define (member? list x)
  (let loop ((list list))
    (cond ((null? list)
	   #f)
	  ((= x (car list))
	   #t)
	  (else
	   (loop (cdr list))))))

(define (member? list x)
  (let loop ((list list))
    (delistify list
      ((null)
       #f)
      ((pair head tail)
       (if (= x head)
	   #t
	   (loop tail))))))

(define (reverse! list)
  (if (or (null? list)
	  (null? (cdr list)))
      list
      (let loop ((list list) (prev null))
	(let ((next (cdr list)))
	  (set-cdr! list prev)
	  (if (null? next)
	      list
	      (loop next list))))))

; Not terrible.

(define (reverse! list)
  (delistify list
    ((null)
     list)
    ((pair . first-pair)
     (delistify (cdr first-pair)
       ((null)
	list)
       ((pair)
	(let loop ((pair first-pair) (prev null))
	  (let ((next (cdr pair)))
	    (set-cdr! pair prev)
	    (delistify next
	      ((null)
	       pair)
	      ((pair . next-pair)
	       (loop next-pair next))))))))))
