; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; exit

(define-command-syntax 'exit "[<status>]" "leave Scheme" '(&opt expression))

(define (exit . exp-option)
  (let ((status (if (null? exp-option)
                    0
                    (evaluate (car exp-option) (environment-for-commands)))))
    (exit-command-processor (lambda () status))))

; go

(define-command-syntax 'go "<exp>" "leave Scheme via tail recursion" '(expression))

(define (go exp)
  (let* ((env (environment-for-commands)))
    (exit-command-processor (lambda () (evaluate exp env)))))

; run

(define-command-syntax 'run "<exp>" "evaluate an expression" '(expression))

(define (run exp)
  (evaluate-and-select exp (environment-for-commands)))

; load

(define-command-syntax 'load "<filename> ..."
  "load Scheme source file(s)"
  '(&rest filename))

(define (load . filenames)
  (let ((env (environment-for-commands)))
    (maybe-noting-undefined-variables env
      (lambda ()
        (for-each (lambda (filename)
                    ((structure-ref scheme load) filename env))
                  filenames)))))

(define (maybe-noting-undefined-variables env thunk)
  (if (package? env)
      (noting-undefined-variables env thunk)
      (thunk)))

(define ? help)

(define-command-syntax 'help
  "[<command-name>]"
  "list all commands, or give help on a specific command"
  '(&opt name))

(define-command-syntax '? "[<command-name>]" "same as ,help" '(&opt name))

; Utilities

(define (pad-right string width padchar)
  (let ((n (- width (string-length string))))
    (if (<= n 0)
        string
        (string-append string (make-string n padchar)))))

