; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file debug.scm

;;;; Debugging

(import vm-environment
	initialize-heap
	initialize-machine
	initialize-i/o-system
	clear-registers
	reset-stack-pointer
	save-symbol-table
	save-environment-table
	(get-symbol-table get-saved-symbol-table)
	(get-system-environment get-saved-environment-table)
	(vm-make-closure make-closure)
	start-vm
	(vm-set-contents! set-contents!)
	(collect interpreter-collect)
	available
	)

(define (init . size-option)
  (let ((size (if (null? size-option) 200000 (car size-option))))
    (initialize-heap size)
    (initialize-machine)
    (initialize-i/o-system)
    (clear-registers)                ;purge garbage from registers
    (save-symbol-table (make-vm-table 512))
    (save-environment-table (make-vm-table 512))
    'done))

(define (make-vm-table size)
  (let ((t (vm-make-vector size universal-key)))
    (do ((i 0 (+ i 1)))
	((>= i size))
      (vm-vector-set! t i null))
    t))

(define (reset-interpreter)
  (clear-registers)
  (reset-stack-pointer))

(define (cold-load)
  (for-each (lambda (def)
              (run-form def #f))
            (definitions-for-all-compiler-primitives))
  (let ((load (lambda (f)
                (let ((f (string-downcase (symbol->string f))))
                  (boot-load (string-append s48/rts
                                            "/"
                                            f
                                            ".scm"))))))
    (for-each load '(enum arch basic rtsistruct sys io read))
    (collect)
    (for-each load '(comp cprim derive ssig))
    (run-form '(initialize-user-environment) #f)
    'done))

(define (string-downcase string)
  (list->string (map char-downcase (string->list string))))

; Misc. bootstrap and debugging stuff

(define (boot-load filename)
  (if (< (available) 5000)    ; heuristic check
      (collect))
  (call-with-input-file
    filename
    (lambda (port)
      (newline)
      (display "Loading ")
      (write filename)
      (force-output (current-output-port))
      (let loop ()
        (let ((form (read port)))
          (cond ((eof-object? form) 'done)
                (else
                 (run-form form filename)
                 (write-char #\.)
                 (force-output (current-output-port))
                 (loop))))))))

(define (run exp)
  (reset-interpreter)
  (extract (run-form exp #f)))

(define (run-form form file-id)
  (let loop ((forms (process-forms (list form) system-environment)) (value #f))
    (cond ((null? forms)
	   value)
	  ((caar forms)
	   (let ((form (cdar forms)))
	     (boot-define (cadr form)
			  (run-expression (caddr form)
					  (cadr form)))
	     (loop (cdr forms) unspecific)))
	  (else
	   (loop (cdr forms) (run-expression (cdar forms) file-id))))))

(define (warn message . rest)
  (display message)
  (for-each display rest))

(define system-environment compiler-base-environment)

(define (run-expression exp where)
  (if (and (pair? exp) (eq? (car exp) 'lambda))
      (vm-make-closure
       (enter (compile-lambda exp system-environment where))
       (enter where)
       universal-key)
      (start-vm
       (vm-make-closure
	(enter (compile-lambda `(lambda () (halt ,exp))
			       system-environment
			       where))
	(enter where)
	universal-key))))

(define (boot-define name val)  ;RUN relies on this returning a descriptor
  (let ((name (enter name)))
    (vm-set-contents! (lookup (get-system-environment)
			      name
			      universal-key)
		      val)
    name))



