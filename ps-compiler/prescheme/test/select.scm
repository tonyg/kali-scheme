; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


(define (test)
  (let ((in (current-input-port))
	(out (current-output-port))
	(s1 (make-port-set))
	(s2 (make-port-set)))
    (let loop ((i 0))
      (cond ((char-ready? in)
	     (got-char in out i)
	     (loop 0))
	    (else
	     (clear-port-set! s1)
	     (clear-port-set! s2)
	     (add-to-port-set! s1 in)
	     (case (find-ready-ports s1 s2 #f)
	       ((0)
		(loop (+ i 1)))
	       ((1)
		(cond ((port-set-member? s1 in)
		       (got-char in out i)
		       (loop 0))
		      (else
		       (write-string "not in port set" out)
		       (newline out))))
	       (else
		(write-string "funny port count " out))))))))

(define (got-char in out i)
  (write-string "Got " out)
  (ps-read-char in
		(lambda (char)
		  (write-number-no-newline (ascii->char char) out))
		(lambda ()
		  (write-string "EOF!" out)))
  (write-string " after " out)
  (write-number i out))

; Printing integers

; Return 10**n such that 10**n <= x < 10**(n+1)

(define (integer-mask x)
  (do ((x x (quotient x 10))
       (mask 1 (* mask 10)))
      ((< x 10) mask)))

; Write positive integer X out to PORT

(define (write-number x port)
  (write-number-no-newline x port)
  (write-char '#\newline port))

(define (write-number-no-newline x port)
  (let ((x (cond ((< x 0)
                  (write-char '#\- port)
                  (- 0 x))
                 (else
                  x))))
    (let loop ((x x) (mask (integer-mask x)))
      (let ((digit (quotient x mask)))
        (write-char (ascii->char (+ digit (char->ascii '#\0))) port)
        (if (> mask 1)
	    (loop (remainder x mask) (quotient mask 10)))))))



		
