; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


;Date: Mon, 24 Jan 94 15:10:30 -0500
;To: jar@ai.mit.edu
;Subject: integer->string
;From: kelsey@research.nj.nec.com
;Sender: kelsey@research.nj.nec.com
;
;
;I have gotten tired of waiting for bignums to print out.
;Here is a somewhat more complex and faster version of integer->string.
;
;Converting 10**100 to a string:
; Current:                    0.44 seconds
; This:                       0.12 seconds
; This using integer-divide:  0.06 seconds
;
;There is no overwhelming reason to use this, but here it is.


(define integer->string
  (let ()

    (define (integer->string n radix)

      (define (small-integer->magnitude n l)
	(if (= n 0)
	    l
	    (small-integer->magnitude (quotient n radix)
				      (cons (integer->digit (remainder n radix))
					    l))))
      
      (define (integer->magnitude n)
	(let ((rrrr (* (* radix radix) (* radix radix))))
	  
	  (let recur ((n n) (l '()))
	       (if (< n rrrr)
		   (small-integer->magnitude n l)
		   (do ((i 4 (- i 1))
			(n0 (remainder n rrrr) (quotient n0 radix))
			(l l (cons (integer->digit (remainder n0 radix)) l)))
		       ((= 0 i)
			(recur (quotient n rrrr) l)))))))

      (let ((magnitude (cond ((= n 0) '(#\0))
			     ((< n 1000000)
			      (small-integer->magnitude (abs n) '()))
			     (else
			      (integer->magnitude (abs n))))))
	(list->string (if (>= n 0)
			  magnitude
			  (cons #\- magnitude)))))

    (define (integer->digit n)
      (ascii->char (+ n (if (< n 10)
			    zero
			    a-minus-10))))

    (define zero (char->ascii #\0))
    (define a-minus-10 (- (char->ascii #\a) 10))

    integer->string))



