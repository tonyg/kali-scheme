; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.




(define (make-population)
  (list '<population>))

(define (add-to-population! x pop)
  (if (not x) (error "can't put #f in a population"))
  (set-cdr! pop (cons (make-weak-pointer x) (cdr pop))))

(define (population-reduce cons nil pop)
  (do ((l (cdr pop) (cdr l))
       (prev pop l)
       (m nil (let ((w (weak-pointer-ref (car l))))
		(if w
		    (cons w m)
		    (begin (set-cdr! prev (cdr l))
			   m)))))
      ((null? l) m)))

(define (population->list pop)
  (population-reduce cons '() pop))

(define (walk-population proc pop)
  (population-reduce (lambda (thing junk) (proc thing))
		     #f
		     pop))
