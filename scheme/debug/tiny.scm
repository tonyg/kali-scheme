; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Tiny image and simple I/O test system

; This prints `Hello' and the first command argument, if any, then reads
; a line from standard input and prints it to standard output.

(define (start arg in out error-out resumer-records)
  (write-string "Hello " out)
  (if (vector? arg)
      (if (< 0 (vector-length arg))
	  (write-string (vector-ref arg 0) out)))
  (collect)
  (collect)
  (newline out)
  (write-string (read-string in) out)
  (newline out)
  0)

(define first? #t)

(define (write-string string . channel-option)  ; test n-ary procedures
  (channel-maybe-write string
		       0
		       (string-length string)
		       (car channel-option)))

(define (newline channel)
  (write-string "
" channel))

(define (read-string in)
  ((lambda (buffer)
     (letrec ((loop (lambda (have)
		      ((lambda (got)
			 (if (eq? got (eof-object))
			     "eof"
			     ((lambda (len)
				(if len
				    ((lambda (string)
				       (copy-string! buffer string len)
				       string)
				     (make-string len #\space))
				    (loop (+ have got))))
			      (has-newline buffer have got))))
		       (channel-maybe-read buffer have (- 80 have) #f in)))))
       (loop 0)))
   (make-string 80 #\space)))

(define (has-newline string start count)
  (letrec ((loop (lambda (i)
		   (if (= i count)
		       #f
		       (if (char=? #\newline
				   (string-ref string (+ start i)))
			   (+ start i)
			   (loop (+ i 1)))))))
    (loop 0)))

(define (copy-string! from to count)
  (letrec ((loop (lambda (i)
		   (if (< i count)
		       (begin
			 (string-set! to i (string-ref from i))
			 (loop (+ i 1)))))))
    (loop 0)))
