; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Tiny image and simple I/O test system

(define (start arg in in-encoding out out-encoding error-out error-out-encoding resumer-records)
  (letrec ((fib (lambda (n)
		  (if (< n 2)
		      1
		      (+ (fib (- n 1))
			 (fib (- n 2) ))))))
    (fib 32)
    0)

  (write-string "Hello " out)

  (write-string (read-string in) out)
  (newline out)

  (if (vector? arg)
      (if (< 0 (vector-length arg))
          (write-byte-vector (vector-ref arg 0) out)))
  (newline out)
  (let ((c #f))
    (foo (lambda ()
           (if (call-with-current-continuation (lambda (cc)
                                                 (set! c cc)
                                                 10))
               'a
               'b)))
    (collect)
    (collect)
    (symbol-check in out)
    (collect)
    (if c 1 2)
    (write-image "test.image" start "Hello?" error-out)
    0))

(define (foo thunk)
  (+ (begin (thunk) 1)
     2))

(define (wrapper thunk out)
  (call-with-current-continuation
    (lambda (c)
      (write-string "[in]" out)
      (newline out)
      (call-with-values
       (let ((x thunk)) (set! thunk #f) x)
       (lambda results
	 (write-string "[out with " out)
	 (display-list results out)
	 (write-string "]" out)
	 (newline out)
	 (apply values results))))))

(define (display-list c out)
  (if (eq? c '())
      (write-string "()" out)
      (begin
	(write-string "(" out)
	(write-string (symbol->string (car c)) out)
	(let loop ((c (cdr c)))
	  (if (eq? c '())
	      (write-string ")" out)
	      (begin
		(write-string " " out)
		(write-string (symbol->string (car c)) out)
		(loop (cdr c))))))))

(define (call-with-current-continuation proc)
  (primitive-cwcc
    (lambda (cont)
      (proc (continuation->procedure cont)))))

(define (primitive-cwcc p)
  (primitive-catch (lambda (cont)
		     (p (loophole :escape cont))))) ;?

(define (continuation->procedure cont)
  (lambda results
    (with-continuation cont
      (lambda ()
	(apply values results)))))

(define *the-symbols* '())

(define (lookup s)
  (let loop ((syms *the-symbols*))
    (cond ((eq? syms '())
	   (set! *the-symbols*
		 (cons (lookup-shared-binding s #f)
		       *the-symbols*))
	   #f)
	  ((string=? s (shared-binding-name (car syms)))
	   (car syms))
	  (else
	   (loop (cdr syms))))))

(define (symbol-check in out)
  (let loop ()
    (write-string "> " out)
    (let ((s (read-string in)))
      (if (< 0 (string-length s))
	  (let ((sym (lookup s)))
	    (write-string (if sym
			      (if (eq? sym (lookup-shared-binding s #f))
				  "[old, eq]"
				  "[old, not eq]")
			      "[new]")
			  out)
	    (newline out)
	    (loop))))))

(define (write-string string . channel-option)  ; test n-ary procedures
  (write-byte-vector (string->byte-vector string)
		     (car channel-option)))

(define (write-byte-vector bytes channel)
  (channel-maybe-write channel
		       bytes
		       0
		       (byte-vector-length bytes)))

(define (string->byte-vector string)
  (let* ((size (string-length string))
	 (bytes (make-byte-vector size 0)))
    (do ((i 0 (+ 1 i)))
	((>= i size))
      (byte-vector-set! bytes i (char->scalar-value (string-ref string i))))
    bytes))

(define (string->low-string string)
  (let* ((size (string-length string))
	 (bytes (make-byte-vector (+ 1 size) 0)))
    (do ((i 0 (+ 1 i)))
	((>= i size))
      (byte-vector-set! bytes i (char->scalar-value (string-ref string i))))
    bytes))

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
				       (copy-bytes-to-string! buffer string len)
				       string)
				     (make-string len #\space))
				    (loop (+ have got))))
			      (has-newline buffer have got))))
		       (channel-maybe-read in buffer have (- 80 have) #f)))))
       (loop 0)))
   (make-byte-vector 80 (char->scalar-value #\space))))

(define (has-newline bytes start count)
  (letrec ((loop (lambda (i)
		   (if (= i count)
		       #f
		       (if (char=? #\newline
				   (scalar-value->char
				    (byte-vector-ref bytes (+ start i))))
			   (+ start i)
			   (loop (+ i 1)))))))
    (loop 0)))

(define (copy-bytes-to-string! from to count)
  (letrec ((loop (lambda (i)
		   (if (< i count)
		       (begin
			 (string-set! to i
				      (scalar-value->char (byte-vector-ref from i)))
			 (loop (+ i 1)))))))
    (loop 0)))

;----------------

(define (write-image filename start-procedure message error-out)
  (let ((undumpable (make-vector 1000 #f)))
    (write-image-low (string->low-string filename)
		     start-procedure (string->low-string message)
		     undumpable)
    (if (vector-ref undumpable 0)
	(begin
	  (write-string "undumpable records written in image:" error-out)
	  (display-list (vector-prefix->list undumpable) error-out)
	  (newline error-out)))))

; Return a list containing the non-#F values at the beginning of VECTOR.

(define (vector-prefix->list vector)
  (do ((i 0 (+ i 1))
       (losers '() (cons (vector-ref vector i) losers)))
      ((or (= i (vector-length vector))
	   (if (vector-ref vector i) #f #t))
       losers)))

