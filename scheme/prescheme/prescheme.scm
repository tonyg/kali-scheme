; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Stuff in Pre-Scheme that is not in Scheme.

(define shift-left arithmetic-shift)

(define (arithmetic-shift-right i n)
  (arithmetic-shift i (- 0 n)))

; Hack for the robots
(define small* *) ; could do a range check

(define int-mask (- (arithmetic-shift 1 pre-scheme-integer-size) 1))

(define (logical-shift-right i n)
  (if (>= i 0)
      (arithmetic-shift i (- 0 n))
      (arithmetic-shift (bitwise-and i int-mask) (- 0 n))))

(define (deallocate x) #f)
(define the-null-pointer (list 'null-pointer))
(define (null-pointer? x) (eq? x the-null-pointer))
(define (null-pointer)
  the-null-pointer)

(define-external-enumeration errors
  (no-errors
   (parse-error    "EDOM")
   (file-not-found "ENOENT")
   (out-of-memory  "ENOMEM")
   (invalid-port   "EBADF")
   ))

(define (error-string status)
  "an error")
  ; (symbol->string (enumerand->name status errors)))

(define (open-input-file name)
  (let ((port (scheme:open-input-file name)))
    (values port
	    (if port
		(enum errors no-errors)
		(enum errors file-not-found)))))

(define (open-output-file name)
  (let ((port (scheme:open-output-file name)))
    (values port
	    (if port
		(enum errors no-errors)
		(enum errors file-not-found)))))

(define (close-input-port port)
  (scheme:close-input-port port)
  (enum errors no-errors))

(define (close-output-port port)
  (scheme:close-output-port port)
  (enum errors no-errors))

(define (read-char port)
  (let ((ch (scheme:read-char port)))
    (if (eof-object? ch)
	(values (ascii->char 0) #t (enum errors no-errors))
	(values ch #f (enum errors no-errors)))))

(define (peek-char port)
  (let ((ch (scheme:peek-char port)))
    (if (eof-object? ch)
	(values (ascii->char 0) #t (enum errors no-errors))
	(values ch #f (enum errors no-errors)))))

(define (read-integer port)
  (eat-whitespace! port)
  (let ((neg? (let ((x (scheme:peek-char port)))
		(if (eof-object? x)
		    #f
		    (case x
		      ((#\+) (scheme:read-char port) #f)
		      ((#\-) (scheme:read-char port) #t)
		      (else #f))))))
    (let loop ((n 0) (any? #f))
      (let ((x (scheme:peek-char port)))
	(cond ((and (char? x)
		    (char-numeric? x))
	       (scheme:read-char port)
	       (loop (+ (* n 10)
			(- (char->integer x)
			   (char->integer #\0)))
		     #t))
	      (any?
	       (values (if neg? (- n) n) #f (enum errors no-errors)))
	      ((eof-object? x)
	       (values 0 #t (enum errors no-errors)))
	      (else
	       (values 0 #f (enum errors parse-error))))))))

(define (eat-whitespace! port)
  (cond ((char-whitespace? (scheme:peek-char port))
	 (scheme:read-char port)
	 (eat-whitespace! port))))

(define (write-x string port)
  (display string port)
  (enum errors no-errors))

(define write-char write-x)
(define write-string write-x)
(define write-integer write-x)

(define (force-output port)
  (enum errors no-errors))

(define (newline port)
  (write-char #\newline port)
  (enum errors no-errors))

(define-syntax goto
  (lambda (exp rename compare)
    (cdr exp)))

; (external <string> <type> . <maybe scheme value>)

(define-syntax external
  (lambda (exp rename compare)
    (if (null? (cdddr exp))
	exp
	(cadddr exp))))

(define current-error-port current-output-port)

; RECEIVE (from big-scheme)

(define-syntax receive
  (syntax-rules ()
    ((receive ?vars ?producer . ?body)
     (call-with-values (lambda () ?producer)
		       (lambda ?vars . ?body)))))
