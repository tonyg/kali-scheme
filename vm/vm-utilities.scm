
(define (adjoin-bits high low k)
  (+ (ashl high k) low))

(define (low-bits n k)
  (bitwise-and n (- (ashl 1 k) 1)))

(define high-bits ashr)

(define unsigned-high-bits lshr)

(define (digit? ch)
  (let ((ch (char->ascii ch)))
    (and (>= ch (char->ascii #\0))
	 (<= ch (char->ascii #\9)))))

(define (read-number port)
  (let ((ch (let loop ()
	      (ps-read-char port
			    (lambda (ch)
			      (if (digit? ch) ch (loop)))
			    (lambda () -1)))))
    (let loop ((r 0) (ch ch))
      (let ((r (+ (- (char->ascii ch) (char->ascii #\0))
		  (* r 10))))
	(ps-read-char port
		      (lambda (ch)
			(if (digit? ch)
			    (loop r ch)
			    r))
		      (lambda () r))))))

(define (write-number x port)
  (really-write-number (cond ((< x 0)
			      (write-char #\- port)
			      (- 0 x))
			     (else
			      x))
		       port)
  (newline port))

(define (really-write-number x port)
  (if (>= x 10)
      (really-write-number (quotient x 10) port))
  (write-char (ascii->char (+ (remainder x 10)
			      (char->ascii #\0)))
	      port))

(define (vector+length-fill! v length x)
  (do ((i 0 (+ i 1)))
      ((>= i length))
    (vector-set! v i x)))
