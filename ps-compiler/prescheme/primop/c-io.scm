; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.

(define (write-c-io-call call port name . args)
  (format port name)
  (writec port #\()
  (for-each (lambda (arg)
	      (cond ((string? arg)
		     (format port arg))
		    ((variable? arg)
		     (c-variable arg port))
		    (else
		     (c-value (call-arg call arg) port))))
	    args)
  (writec port #\)))

; stdin, stdout, and stderr cannot be variables because they may be macros in C.

(define-c-generator stdin #t
  (lambda (call port indent)
    (format port "stdin")))

(define-c-generator stdout #t
  (lambda (call port indent)
    (format port "stdout")))

(define-c-generator stderr #t
  (lambda (call port indent)
    (format port "stderr")))

; char eof? status

(define-c-generator read-char #f
  (lambda (call port indent)
    (indent-to port indent)
    (let ((vars (lambda-variables (call-arg call 0))))
      (write-c-io-call call port "PS_READ_CHAR" 1 ", "
		       (car vars) ", " (cadr vars) ", " (caddr vars)))))

(define-c-generator peek-char #f
  (lambda (call port indent)
    (indent-to port indent)
    (let ((vars (lambda-variables (call-arg call 0))))
      (write-c-io-call call port "PS_PEEK_CHAR" 1 ", "
		       (car vars) ", " (cadr vars) ", " (caddr vars)))))

(define-c-generator read-integer #f
  (lambda (call port indent)
    (indent-to port indent)
    (let ((vars (lambda-variables (call-arg call 0))))
      (write-c-io-call call port "PS_READ_INTEGER" 1 ", "
		       (car vars) ", " (cadr vars) ", " (caddr vars)))))

(define-c-generator write-char #f
  (lambda (call port indent)
    (indent-to port indent)
    (let ((vars (lambda-variables (call-arg call 0))))
      (if (used? (car vars))
	  (write-c-io-call call port "PS_WRITE_CHAR" 1 ", " 2 ", " (car vars))
	  (begin
	    (display "{ long ignoreXX;" port)
	    (indent-to port indent)
	    (write-c-io-call call port "PS_WRITE_CHAR" 1 ", " 2 ", ignoreXX")
	    (display " }" port))))))

(define-c-generator write-string #t
  (lambda (call port indent)
    (write-c-io-call call port "ps_write_string" 0 ", " 1)))

(define-c-generator write-integer #t
  (lambda (call port indent)
    (write-c-io-call call port "ps_write_integer" 0 ", " 1)))

(define-c-generator force-output #t
  (lambda (call port indent)
    (write-c-io-call call port "ps_flush" 0)))

(define-c-generator read-block #f
  (lambda (call port indent)
    (let ((vars (lambda-variables (call-arg call 0))))
      (c-assign-to-variable (car vars) port indent)
      (write-c-io-call call port "ps_read_block" 1 ", ((char *) " 2 "), " 3
		       ", &" (cadr vars) ", &" (caddr vars))
      (write-char #\; port))))

(define-c-generator write-block #t
  (lambda (call port indent)
    (write-c-io-call call port "ps_write_block" 0 ", ((char *) " 1 ")"
		     ", " 2)))

; (read-block (lambda (okay? eof? got) ...) port buffer count)
;
;(define-c-generator read-block #f
;  (lambda (call port indent)
;    (let* ((cont (call-arg call 0))
;           (vars (lambda-variables cont)))
;      ;;  got = ps_read(port, buffer, count, &okay?, &eof?);
;      (c-assign-to-variable (caddr vars) port indent)
;      (write-c-io-call call port
;                       "ps_read" 1 ", (void *)" 2 ", " 3 ", &" (car vars)
;                       ", &" (cadr vars))
;      (write-char #\; port))))
;
;; (write-block (lambda (okay? sent) ...) port buffer count)
;
;(define-c-generator write-block #f
;  (lambda (call port indent)
;    (let* ((cont (call-arg call 0))
;           (vars (lambda-variables cont)))
;      ;;  sent = ps_write(port, buffer, count, &okay?);
;      (c-assign-to-variable (cadr vars) port indent)
;      (write-c-io-call call port
;                       "ps_write" 1 ", (void *)" 2 ", " 3 ", &" (car vars))
;      (write-char #\; port))))

(define-c-generator open-input-file #f
  (lambda (call port indent)
    (let ((vars (lambda-variables (call-arg call 0))))
      (c-assign-to-variable (car vars) port indent)
      (write-c-io-call call port "ps_open_input_file" 1 ", &" (cadr vars))
      (write-char #\; port))))

(define-c-generator open-output-file #f
  (lambda (call port indent)
    (let ((vars (lambda-variables (call-arg call 0))))
      (c-assign-to-variable (car vars) port indent)
      (write-c-io-call call port "ps_open_output_file" 1 ", &" (cadr vars))
      (write-char #\; port))))

(define-c-generator close-input-port #t
  (lambda (call port indent)
    (write-c-io-call call port "ps_close" 0)))

(define-c-generator close-output-port #t
  (lambda (call port indent)
    (write-c-io-call call port "ps_close" 0)))

(define-c-generator abort #t
  (lambda (call port indent)
    (format port "(exit -1)")))

(define-c-generator error #f
  (lambda (call port indent)
    (indent-to port indent)
    (format port "ps_error(")
    (c-value (call-arg call 1) port)
    (format port ", ~D" (- (call-arg-count call) 2))
    (do ((i 2 (+ i 1)))
	((= i (call-arg-count call)))
      (format port ", ")
      (c-value (call-arg call i) port))
    (format port ");")))

(define-c-generator error-string #t
  (lambda (call port indent)
    (write-c-io-call call port "ps_error_string" 0)))

; (c-e-v <proc> <nargs> <pointer-to-args>)

(define-c-generator call-external-value #t
  (lambda (call port indent)
    (format port "((long(*)())")
    (c-value (call-arg call 0) port)
    (format port ")(")
    (c-value (call-arg call 1) port)
    (format port ", ")
    (c-value (call-arg call 2) port)
    (writec port #\))))


