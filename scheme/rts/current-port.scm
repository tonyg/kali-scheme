; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; Current input, output, error, and noise ports.

(define $current-input-port  (make-fluid #f))
(define $current-output-port (make-fluid #f))
(define $current-error-port  (make-fluid #f))
(define $current-noise-port  (make-fluid #f))  ; defaults to the error port

(define (current-input-port)
  (fluid $current-input-port))

(define (current-output-port)
  (fluid $current-output-port))

(define (current-error-port)
  (fluid $current-error-port))

(define (current-noise-port)
  (fluid $current-noise-port))

(define (initialize-i/o input output error thunk)
  (with-current-ports input output error thunk))

(define (with-current-ports in out error thunk)
  (let-fluids $current-input-port in
	      $current-output-port out
	      $current-error-port error
	      $current-noise-port error
    thunk))

(define (call-with-current-input-port port thunk)
  (let-fluid $current-input-port port thunk))

(define (call-with-current-output-port port thunk)
  (let-fluid $current-output-port port thunk))

(define (call-with-current-noise-port port thunk)
  (let-fluid $current-noise-port port thunk))

(define (silently thunk)
  (call-with-current-noise-port (make-null-output-port) thunk))

;----------------
; Procedures with default port arguments.

(define (newline . port-option)
  (write-char #\newline (output-port-option port-option)))

(define (char-ready? . port-option)
  (real-char-ready? (input-port-option port-option)))

(define (output-port-option port-option)
  (cond ((null? port-option) (current-output-port))
	((null? (cdr port-option)) (car port-option))
	(else (error "write-mumble: too many arguments" port-option))))

(define (input-port-option port-option)
  (cond ((null? port-option) (current-input-port))
	((null? (cdr port-option)) (car port-option))
	(else (error "read-mumble: too many arguments" port-option))))

; Wrong-number-of-arguments handlers

(define-wna-handler read-char
  (lambda (args)
    (if (null? args)
	(read-char (input-port-option args))
	(wna-lose read-char args))))

(define-wna-handler peek-char
  (lambda (args)
    (if (null? args)
	(peek-char (input-port-option args))
	(wna-lose peek-char args))))

(define-wna-handler write-char
  (lambda (args)
    (if (and (not (null? args))
	     (null? (cdr args)))
	(write-char (car args) (output-port-option (cdr args)))
	(wna-lose write-char args))))

;----------------
; A poor man's WRITE for use in debugging

(define (message . stuff)
  (for-each (lambda (thing)
	      (write-string (cond ((number? thing)
				   (number->string thing))
				  ((string? thing)
				   thing)
				  ((symbol? thing)
				   (symbol->string thing))
				  ((eq? thing #f)
				   "#f")
				  ((eq? thing #t)
				   "#t")
				  ((null? thing)
				   "()")
				  ((pair? thing)
				   "(...)")
				  ((vector? thing)
				   "#(...)")
				  ((char? thing)
				   (string-append "#\\"
						  (list->string (list thing))))
				  ((procedure? thing)
				   "#{procedure}")
				  (else
				   "???"))
			    (current-error-port)))
	    stuff)
  (newline (current-error-port)))
