; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Read a command.  No command name completion, yet.

(define (read-command prompt form-preferred? i-port)
  (let ((o-port (command-output)))
    (let prompt-loop ()
      (if (and prompt (not (batch-mode?)))
	  (display prompt o-port))
      (force-output o-port)		;foo
      (let loop ()
        (let ((c (peek-char i-port)))
          (cond ((eof-object? c)
                 (read-char i-port))
                ((char-whitespace? c)
                 (read-char i-port)
                 (if (char=? c #\newline)
                     (prompt-loop)
                     (loop)))
                ((char=? c #\;)      ;Comment
		 (gobble-line i-port)
		 (prompt-loop))
                ((char=? c command-prefix)
                 (read-char i-port)
                 (read-named-command i-port))
		((or form-preferred?
		     (and (not (char-alphabetic? c))
			  (not (char-numeric? c))
			  (not (char=? c #\?))))
		 (read-evaluation-command i-port))
		(else
		 (read-named-command i-port))))))))

(define (read-command-carefully prompt form-preferred? i-port)
  (call-with-current-continuation
    (lambda (k)
      (with-handler
	  (lambda (c punt)
	    (if (and (not (batch-mode?))
		     (or (read-error? c)
			 (read-command-error? c)))
		(let ((port (last (condition-stuff c))))
		  (if (eq? port i-port)
                      (do ()
                          ((or (not (char-ready? port))
                               (let ((c (read-char port)))
                                 (or (eof-object? c) (char=? c #\newline)))))))
		  (display-condition c (command-output))
		  (k (make-command 'read-command-error '())))
		(punt)))
	(lambda ()
	  (read-command prompt form-preferred? i-port))))))

(define (read-evaluation-command i-port)
  (let ((form (read-form i-port)))
    (if (eq? (skip-over horizontal-space? i-port) #\newline)
	(read-char i-port))
    (make-command 'evaluate (list form))))

; Read a single form, allowing ## as a way to refer to last command
; output.

(define (read-form port)
  (with-sharp-sharp (make-node (get-operator 'literal 'leaf)
			       (focus-object))
    (lambda () (read port))))

; Read a command line:  <name> <arg> ... <newline>

(define (read-named-command port)
  (let ((c-name (read port)))
    (make-command c-name
		  (read-command-arguments (get-command-syntax c-name)
					  #f
					  port))))

(define (read-command-arguments ds opt? port)
  (let ((c (skip-over horizontal-space? port)))
    (cond ((and (not (null? ds))
		(eq? (car ds) '&opt))
	   (read-command-arguments (cdr ds) #t port))
	  ((or (eof-object? c)
	       (char=? c #\newline)
	       (if (char=? c #\;)    ;Comment
		   (begin (gobble-line port)
			  #t)
		   #f))
	   (cond ((or (null? ds) (eq? (car ds) '&rest))
		  (read-char port)
		  '())
		 (opt?
		  (read-char port)
		  (map (lambda (d) #f) ds))
		 (else
		  (read-command-error port "too few command arguments"))))
	  ((null? ds)
	   (read-command-error port "too many command arguments"))
	  ((eq? (car ds) '&rest)
	   (cons (read-command-argument (cadr ds) port)
		 (read-command-arguments ds #f port)))
	  (else
	   (cons (read-command-argument (car ds) port)
		 (read-command-arguments (cdr ds) opt? port))))))

(define (read-command-argument d port)
  (case d
    ((filename)
     (read-string port char-whitespace?))
    ((expression form)
     (read-form port))
    ((name)
     (let ((thing (read port)))
       (if (symbol? thing)
	   thing
	   (read-command-error port "invalid name" thing))))
    (else (error "invalid argument description" d))))

(define-condition-type 'read-command-error '(error))
(define read-command-error? (condition-predicate 'read-command-error))

(define (read-command-error port message . rest)
  (apply signal 'read-command-error message (append rest (list port))))


; Utilities.

(define (horizontal-space? c)
  (and (char-whitespace? c)
       (not (char=? c #\newline))))

(define (read-string port delimiter?)
  (let loop ((l '()))
    (let ((c (peek-char port)))
      (cond ((or (eof-object? c)
                 (delimiter? c))
             (list->string (reverse l)))
            (else
             (loop (cons (read-char port) l)))))))

(define (skip-over pred port)
  (let ((c (peek-char port)))
    (cond ((eof-object? c) c)
	  ((pred c) (read-char port) (skip-over pred port))
	  (else c))))



; ## should evaluate to the last REP-loop result.
(define-sharp-macro #\#
  (lambda (c port)
    (read-char port)
    ((fluid $sharp-sharp) port)))

(define $sharp-sharp
  (make-fluid (lambda (port) (reading-error port "## in invalid context"))))

(define (with-sharp-sharp form body)
  (let-fluid $sharp-sharp (lambda (port) form) body))


; (put 'with-sharp-sharp 'scheme-indent-hook 1)
