; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.


; write s-expressions

; Memory

(define *memory*)
(define *hp*)

(define (initialize-memory! size)
  (set! *memory* (allocate-memory size))
  ;(if (null-pointer? *memory*)
  ;    (error "out of memory, unable to continue"))
  (set! *hp* *memory*))

(define (allocate size)
  (let ((p *hp*))
    (set! *hp* (address+ *hp* size))
    p))

(define (words->a-units x)
  (* x 4))

; Data

(define tag-bits 1)
(define tag-mask (- (shift-left 1 tag-bits) 1))

(define tag/fixnum 0)
(define tag/pair   1)

(define (enter-fixnum x)
  (+ (shift-left x tag-bits) tag/fixnum))

(define (extract-fixnum x)
  (arithmetic-shift-right x tag-bits))

(define (make-predicate tag)
  (lambda (x)
    (= tag (bitwise-and x tag-mask))))

(define fixnum?  (make-predicate tag/fixnum))
(define my-pair? (make-predicate tag/pair))

(define (make-accessor tag offset)
  (lambda (x)
    (word-ref (address+ (integer->address (+ x (- 0 tag)))
			(words->a-units offset)))))

(define (make-setter tag offset)
  (lambda (x v)
    (word-set! (address+ (integer->address (+ x (- 0 tag)))
			 (words->a-units offset))
	       v)))

(define pair-size 16)   ; bytes

(define head (make-accessor tag/pair 0))
(define tail (make-accessor tag/pair 1))

(define set-head! (make-setter tag/pair 0))
(define set-tail! (make-setter tag/pair 1))

(define (make-pair x y)
  (let ((p (+ tag/pair (address->integer (allocate pair-size)))))
    (set-head! p x)
    (set-tail! p y)
    p))

(define null tag/pair)

(define (my-null? x)
  (= x null))

(define (print-s-exp x out)
  (cond ((fixnum? x)
         (write-number-no-newline (extract-fixnum x) out))
        ((my-null? x)
         (write-char #\( out)
         (write-char #\) out))
        ((my-pair? x)
         (write-char #\( out)
         (print-s-exp (head x) out)
         (let loop ((x (tail x)))
           (cond ((my-null? x)
                  (write-char #\) out))
                 ((my-pair? x)
                  (write-char #\space out)
                  (print-s-exp (head x) out)
                  (loop (tail x)))
                 (else
                  (write-char #\space out)
                  (write-char #\. out)
                  (write-char #\space out)
                  (print-s-exp x out)
                  (write-char #\) out)))))))

(define *input-port*)
(define *peeked-char?* #f)
(define *peeked-char*)

(define (readc)
  (cond (*peeked-char?*
	 (set! *peeked-char?* #f)
	 *peeked-char*)
	(else
	 (call-with-values
	  (lambda ()
	    (read-char *input-port*))
	  (lambda (ch eof? status)
	    (if eof? (ascii->char 0) ch))))))

(define (peekc)
  (if *peeked-char?*
      *peeked-char*
      (call-with-values
       (lambda ()
	 (read-char *input-port*))
       (lambda (ch eof? status)
	 (if eof?
	     (ascii->char 0)
	     (begin
	       (set! *peeked-char?* #t)
	       (set! *peeked-char* ch)
	       ch))))))

(define (digit? ch)
  (let ((ch (char->ascii ch)))
    (and (>= ch (char->ascii #\0))
	 (<= ch (char->ascii #\9)))))

(define (read-number)
  (let loop ()
    (case (peekc)
      ((#\-)
       (readc)
       (- 0 (really-read-number)))
      ((#\+)
       (readc)
       (really-read-number))
      (else
       (really-read-number)))))

(define (really-read-number)
  (let loop ((r 0))
    (let ((ch (peekc)))
      (cond ((digit? ch)
	     (readc)
	     (loop (+ (- (char->ascii ch) (char->ascii #\0))
		      (* r 10))))
	    (else r)))))

(define (read-s-exp)
  (case (peekc)
    ((#\space #\newline)
     (readc)
     (read-s-exp))
    ((#\- #\+)
     (enter-fixnum (read-number)))
    ((#\()
     (readc)
     (read-list))
    (else
     (if (digit? (peekc))
	 (enter-fixnum (read-number))
	 -1))))

(define (read-list)
  (case (peekc)
    ((#\space #\newline)
     (readc)
     (read-list))
    ((#\))
     (readc)
     null)
    ((#\.)
     (readc)       ; eat the dot
     (let ((res (read-s-exp)))
       (if (read-r-paren)
	   res
	   -1)))
    (else
     (let ((head (read-s-exp)))
       (make-pair head (read-list))))))

(define (read-r-paren)
  (case (peekc)
    ((#\space #\newline)
     (readc)
     (read-r-paren))
    ((#\))
     #t)
    (else #f)))

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


(define (test size)
  (initialize-memory! size)
  (let ((s-exp (make-pair (enter-fixnum 1)
                (make-pair (enter-fixnum 2)
                 (make-pair (make-pair (enter-fixnum 3) (enter-fixnum 4))
                            null))))
	(out (current-output-port)))
    (print-s-exp s-exp out)
    (newline out)
    (set! *input-port* (current-input-port))
    (print-s-exp (read-s-exp) out)
    (newline out)))

