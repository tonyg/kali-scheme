; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.



(define (xwrite-string string out)
  (let ((len (string-length string)))
    (do ((i 0 (+ i 1)))
	((>= i len))
      (write-char (string-ref string (- len (+ i 1))) out))
    (newline out)))

(define (write-string string out)
  (let ((len (string-length string)))
    (do ((i 0 (+ i 1)))
	((>= i len))
      (write-char (string-ref string i) out))
    (newline out)))

(define a-string "Hello sailor...")

(define (test)
  (let* ((in (current-input-port))
	 (out (current-output-port))
	 (len (ashr (read-number in) 2))
	 (string (make-string len)))
    (let loop ((i 0))
      (if (< i len)
	  (ps-read-char in
	    (lambda (ch)
	      (string-set! string i ch)
	      (loop (+ i 1)))
	    (lambda ()
	      (unassigned)))))
    (write-string string out)
    (xwrite-string string out)
    (deallocate string)
    (write-string a-string out)
    (xwrite-string a-string out)))

(define (read-number port)
  (let loop ((r 0))
    (ps-read-char port
      (lambda (ch)
	(cond ((digit? ch)
	       (loop (+ (- (char->ascii ch) (char->ascii #\0))
			(* r 10))))
	      (else r)))
      (lambda () 0))))

(define (digit? ch)
  (let ((ch (char->ascii ch)))
    (and (>= ch (char->ascii #\0))
	 (<= ch (char->ascii #\9)))))

