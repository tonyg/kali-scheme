; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Tiny image and simple I/O test system

; This prints `Hello' and the first command argument, if any, then reads
; a line from standard input and prints it to standard output.

(define (start arg in out error-out resumer-records)
  (write-string "Hello " out)
  (if (vector? arg)
      (if (< 0 (vector-length arg))
	  (write-string (vector-ref arg 0) out)))
  (newline out)
  (collect)
  (collect)
  (symbol-check in out)
  (collect)
  (symbol-check in out)
;  (write-string (read-string in) out)
;  (newline out)
  (write-image "test.image" start "Hello?")
  0)

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
  (channel-maybe-write (car channel-option)
		       string
		       0
		       (string-length string)))
		       

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
		       (channel-maybe-read in buffer have (- 80 have) #f)))))
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

;----------------

(define (write-image filename start-procedure message)
  (let ((undumpable (make-vector 1000 #f)))
    (write-image-low filename start-procedure message undumpable)
    (if (vector-ref undumpable 0)
	(signal 'error
		"undumpable records written in image"
		(vector-prefix->list undumpable)))))

; Return a list containing the non-#F values at the beginning of VECTOR.

(define (vector-prefix->list vector)
  (do ((i 0 (+ i 1))
       (losers '() (cons (vector-ref vector i) losers)))
      ((or (= i (vector-length vector))
	   (if (vector-ref vector i) #f #t))
       losers)))

