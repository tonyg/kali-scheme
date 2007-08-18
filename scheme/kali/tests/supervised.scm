
(define (start-crash-server)
  (let loop ((n 1))
    (if (zero? (modulo n 5))
	(begin
	  (display "now crashing!")
	  (newline)
	  (/ 1 0)
	  (loop (+ n 1)))
	(begin
	  (display n)
	  (display " sleep...")
	  (newline)
	  (sleep 1000)
	  (loop (+ n 1))))))
