

(define *one* 1)

(define (fact n)
  (let loop ((i n) (r *one*))
    (if (<= *one* i)
	(loop (- i *one*) (* i r))
	r)))

(define (all)
  (set! *one* (fact (if (or (> (fact 10) 100)
			    (char-ready? (current-input-port)))
			10 20))))
