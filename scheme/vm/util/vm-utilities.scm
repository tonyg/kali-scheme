; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define (adjoin-bits high low k)
  (+ (shift-left high k) low))

(define (low-bits n k)
  (bitwise-and n (- (shift-left 1 k) 1)))

(define high-bits arithmetic-shift-right)

(define unsigned-high-bits logical-shift-right)

(define (digit? ch)
  (let ((ch (char->ascii ch)))
    (and (>= ch (char->ascii #\0))
	 (<= ch (char->ascii #\9)))))

(define (vector+length-fill! v length x)
  (do ((i 0 (+ i 1)))
      ((>= i length))
    (vector-set! v i x)))

; Apply PROC to 0 ... N-1.

(define (natural-for-each proc n)
  (do ((i 0 (+ i 1)))
      ((= i n))
    (proc i)))

;----------------

(define (error? status)
  (not (eq? status (enum errors no-errors))))

(define (write-error-string string)
  (write-string string (current-error-port)))

(define (write-error-integer integer)
  (write-integer integer (current-error-port)))

(define (write-error-newline)
  (write-char #\newline (current-error-port)))

(define (error-message string)
  (write-error-string string)
  (write-error-newline))
