; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Read a command.  No command name completion, yet.

(define (read-command prompt form-preferred? i-port)
  (really-read-command prompt form-preferred? i-port no-more-commands))

(define (really-read-command prompt form-preferred? i-port more-commands)
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
                 (read-named-command i-port more-commands form-preferred?))
		((or form-preferred?
		     (and (not (char-alphabetic? c))
			  (not (char-numeric? c))
			  (not (char=? c #\?))))
		 (read-evaluation-command i-port))
		(else
		 (read-named-command i-port more-commands form-preferred?))))))))

(define (read-command-carefully prompt form-preferred? i-port . more-commands)
  (call-with-current-continuation
    (lambda (k)
      (with-handler
        (lambda (c punt)
	  (cond ((batch-mode?)
		 (punt))
		((or (read-error? c)
		     (read-command-error? c))
		 (let ((port (last (condition-stuff c))))
		   (if (eq? port i-port)
		       (eat-until-newline i-port))
		   (display-condition c (command-output))
		   (k #f)))
		((reset-command-input? c)
		 (k #f))
		(else
		 (punt))))
	(lambda ()
	  (really-read-command prompt form-preferred? i-port
			       (if (null? more-commands)
				   no-more-commands
				   (car more-commands))))))))

(define (eat-until-newline port)
  (do ()
      ((or (not (char-ready? port))
	   (let ((c (read-char port)))
	     (or (eof-object? c)
		 (char=? c #\newline)))))))

(define (read-evaluation-command i-port)
  (let ((form (read-form i-port)))
    (if (eq? (skip-over horizontal-space? i-port) #\newline)
	(read-char i-port))
    (make-command 'run (list form))))

(define (no-more-commands name)
  #f)

; Read a single form, allowing ## as a way to refer to last command
; output.

(define (read-form port)
  (with-sharp-sharp (make-node (get-operator 'quote)
			       (list 'quote (focus-object)))
    (lambda () (read port))))

; Read a command line:  <name> <arg> ... <newline>

(define (read-named-command port more-commands form-preferred?)
  (let ((c-name (read port)))
    (let ((syntax (or (more-commands c-name)
		      (get-command-syntax c-name))))
      (cond (syntax
	     (make-command c-name
			   (read-command-arguments syntax #f port
						   more-commands
						   form-preferred?)))
	    (else
	     (read-command-arguments '(&rest form) #f port #f #f) ; flush junk
	     (write-line "Unrecognized command name." (command-output))
	     #f)))))

(define (read-command-arguments ds opt? port more-commands form-preferred?)
  (let recur ((ds ds) (opt? opt?))
    (let ((c (skip-over horizontal-space? port)))
      (cond ((and (not (null? ds))
		  (eq? (car ds) '&opt))
	     (recur (cdr ds) #t))
	    ((or (eof-object? c)
		 (char=? c #\newline)
		 (if (char=? c #\;)    ;Comment
			     (begin (gobble-line port)
				    #t)
			     #f))
		 (cond ((or (null? ds)
			    (eq? (car ds) '&rest)
			    opt?)
			(read-char port)
			'())
		       (else
			(read-command-error port
					    "too few command arguments"))))
	    ((null? ds)
	     (read-command-error port "too many command arguments"))
	    ((eq? (car ds) '&rest)
	     (let ((arg (read-command-argument (cadr ds) port)))
	       (cons arg (recur ds #f))))
	    ((eq? (car ds) 'command)	; must be the last argument
	     (if (not (null? (cdr ds)))
		 (error "invalid argument descriptions" ds))
	     (list (really-read-command #f form-preferred? port more-commands)))
	    (else
	     (let ((arg (read-command-argument (car ds) port)))
	       (cons arg (recur (cdr ds) opt?))))))))

(define (read-command-argument d port)
  (if (procedure? d)
      (d port)
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
	(else (error "invalid argument description" d)))))

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

(define make-command cons)      ;(name . args)

; (put 'with-sharp-sharp 'scheme-indent-hook 1)
