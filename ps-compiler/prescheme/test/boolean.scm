; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.




(define (test in out)
  (write-string '"Type in two numbers: " out)
  (let* ((i (read-integer in))
         (j (read-integer in)))
    (write-string '"A = " out)
    (write-integer i out)
    (newline out)
    (write-string '"B = " out)
    (write-integer j out)
    (newline out)
    (write-string (if (and (< i j)
			   (or (= (remainder i '2) '0)
			       (= (remainder j '2) '0)))
		      '"A < B and A or B is even"
		      '"A >= B or A and B are both odd")
		  out)
    (newline out)
    '0))

(test (current-input-port) (current-output-port))
