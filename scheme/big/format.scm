; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Quicky FORMAT
;
; (FORMAT port string . args)
;
; PORT is one of:
; an output port, in which case FORMAT prints to the port;
; #T, FORMAT prints to the current output port;
; #F, FORMAT returns a string.
;
; The following format directives have been implemented:
; ~~  -prints a single ~
; ~A  -prints the next argument using DISPLAY
; ~D  -prints the next argument using NUMBER->STRING (`D'ecimal)
; ~S  -prints the next argument using WRITE
; ~%  -prints a NEWLINE character
; ~&  -prints a NEWLINE character if the previous printed character was not one
;     (this is implemented using FRESH-LINE)
; ~?  -performs a recursive call to FORMAT using the next two arguments as the
;      string and the list of arguments
;
; FORMAT is case-insensitive with respect to letter directives (~a and ~A have
; the same effect).

; The entry point.  Gets the port and writes the output.
; Get the appropriate writer for the port specification.

(define (format port string . args)
  (cond ((not port)
 	 (call-with-string-output-port
  	   (lambda (port)
  	     (real-format port string args))))
	((eq? port #t)
	 (real-format (current-output-port) string args))
	((output-port? port)
	 (real-format port string args))
	(else
	 (error "invalid port argument to FORMAT" port))))

; Loop down the format string printing characters and dispatching on directives
; as required.  Procedures for the directives are in a vector indexed by
; character codes.  Each procedure takes four arguments: the format string,
; the index of the next unused character in the format string, the list of
; remaining arguments, and the writer.  Each should return a list of the unused
; arguments.

(define (real-format out string all-args)
  (let loop ((i 0) (args all-args))
    (cond ((>= i (string-length string))
	   (if (null? args)
	       #f
	       (error "too many arguments to FORMAT" string all-args)))
	  ((char=? #\~ (string-ref string i))
	   (if (= (+ i 1) (string-length string))
	       (error "invalid format string" string i)
	       (loop (+ i 2)
		     ((vector-ref format-dispatch-vector
				  (char->ascii (string-ref string (+ i 1))))
		      string
		      (+ i 2)
		      args
		      out))))
	  (else
	   (write-char (string-ref string i) out)
	   (loop (+ i 1) args)))))

; One more than the highest integer that CHAR->ASCII may return.
(define number-of-char-codes ascii-limit)

; The vector of procedures implementing format directives.

(define format-dispatch-vector
  (make-vector number-of-char-codes
	       (lambda (string i args out)
		 (error "illegal format command"
			string
			(string-ref string (- i 1))))))

; This implements FORMAT's case-insensitivity.

(define (define-format-command char proc)
  (vector-set! format-dispatch-vector (char->ascii char) proc)
  (if (char-alphabetic? char)
      (vector-set! format-dispatch-vector
		   (char->ascii (if (char-lower-case? char)
				    (char-upcase char)
				    (char-downcase char)))
		   proc)))

; Write a single ~ character.

(define-format-command #\~
  (lambda (string i args out)
    (write-char #\~ out)
    args))

; Newline

(define-format-command #\%
  (lambda (string i args out)
    (newline out)
    args))

; Fresh-Line

(define-format-command #\&
  (lambda (string i args out)
    (fresh-line out)
    args))

; Display (`A' is for ASCII)

(define-format-command #\a
  (lambda (string i args out)
    (check-for-format-arg args)
    (display (car args) out)
    (cdr args)))

; Decimals

(define-format-command #\d
  (lambda (string i args out)
    (check-for-format-arg args)
    (if (not (number? (car args)))
	(error "invalid number argument to ~D in FORMAT" string (car args)))
    (display (number->string (car args) 10) out)
    (cdr args)))

; Write (`S' is for S-expression)

(define-format-command #\s
  (lambda (string i args out)
    (check-for-format-arg args)
    (write (car args) out)
    (cdr args)))

; Recursion

(define-format-command #\?
  (lambda (string i args out)
    (check-for-format-arg args)
    (check-for-format-arg (cdr args))
    (real-format out (car args) (cadr args))
    (cddr args)))

; Signal an error if ARGS is empty.

(define (check-for-format-arg args)
  (if (null? args)
      (error "insufficient number of arguments to FORMAT")))


