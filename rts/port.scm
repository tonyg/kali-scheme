; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.

; Current input and output ports.

(define $current-input-port  (make-fluid #f))
(define $current-output-port (make-fluid #f))
(define $error-output-port   (make-fluid #f))

(define (current-input-port)
  (fluid $current-input-port))

(define (current-output-port)
  (fluid $current-output-port))

(define (error-output-port)
  (fluid $error-output-port))

(define (with-initial-ports in out thunk)
  (let-fluids $current-input-port in
	      $current-output-port out
	      $error-output-port out
    thunk))


; File openers with unwind protection

(define (call-with-mumble-file open close)
  (lambda (string proc)
    (let ((port #f))
      (dynamic-wind (lambda ()
		      (if port
			  (warn "throwing back into a call-with-...put-file"
				string)
			  (set! port (open string))))
		    (lambda () (proc port))
		    (lambda ()
		      (if port
			  (close port)))))))

(define call-with-input-file
  (call-with-mumble-file open-input-file close-input-port))

(define call-with-output-file
  (call-with-mumble-file open-output-file close-output-port))

;(define (call-with-input-file string proc)
;  (let* ((port (open-input-file string))
;         (result (proc port)))
;    (close-input-port port)
;    result))
;
;(define (call-with-output-file string proc)
;  (let* ((port (open-output-file string))
;         (result (proc port)))
;    (close-output-port port)
;    result))

(define (with-input-from-file string thunk)
  (call-with-input-file string
    (lambda (port)
      (let-fluid $current-input-port port thunk))))

(define (with-output-to-file string thunk)
  (call-with-output-file string
    (lambda (port)
      (let-fluid $current-output-port port thunk))))

(define (newline . port-option)
  (write-char #\newline (output-port-option port-option)))


(define (output-port-option port-option)
  (cond ((null? port-option) (current-output-port))
	((null? (cdr port-option)) (car port-option))
	(else (error "write-mumble: too many arguments" port-option))))

(define (input-port-option port-option)
  (cond ((null? port-option) (current-input-port))
	((null? (cdr port-option)) (car port-option))
	(else (error "read-mumble: too many arguments" port-option))))
