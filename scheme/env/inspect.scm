; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; A dirty little inspector.
; Look and feel shamelessly plagiarized from the Lucid Lisp inspector.

; There are three commands for invoking the inspector with different
; initial objects:
;   ,inspect           -> focus object
;   ,inspect <exp>     -> value of <exp>
;   ,debug             -> continuation of stopped thread(s), preferentially
;                         chooses the thread with the most recent error
;   ,threads           -> list of current command level's threads

(define-command-syntax 'inspect "[<exp>]" "invoke the inspector"
  '(&opt form))

(define-command-syntax 'debug "" "inspect the current continuation" '())

(define-command-syntax 'threads "" "inspect stopped threads" '())

(define (debug)
  (new-selection (or (command-continuation)
		     (command-threads)))
  (set-command-results! (list (focus-object)) #f)	; force menu printing
  (if (not (value-stack))
      (set-value-stack! '())))

(define (threads)
  (set-focus-object! (command-threads))
  (set-command-results! (focus-object) #f)
  (if (not (value-stack))
      (set-value-stack! '())))

(define (inspect . maybe-exp)
  (if (null? maybe-exp)
      (set-command-results! (list (focus-object)) #f)	; force menu printing
      (evaluate-and-select (car maybe-exp)
			   (environment-for-commands)))
  (if (not (value-stack))
      (set-value-stack! '())))

;----------------
; Menu commands.

(define-command-syntax 'menu "" "print a selection menu for the focus object"
  '())

(define menu present-menu)

(define-command-syntax 'm #f #f '())

(define m present-more-menu)

; Leaving.

(define-command-syntax 'q "" "leave inspection mode" '())

(define (q)
  (set-command-results! (list (focus-object)) #f)
  (set-value-stack! #f))

; Menu selection

(define (select-menu-item . selection-commands)
  (execute-selection-commands selection-commands))

(define (execute-selection-commands commands)
  (for-each execute-selection-command commands))

(define (new-selection value)
  (set-focus-object! value)
  (set-command-results! (list value) #f))

(define (execute-selection-command name)
  (if (integer? name)
      (begin
	(if (and (>= name 0)
		 (< name (current-menu-length)))
	    (new-selection (current-menu-ref name))
	    (write-line "Invalid choice." (command-output))))
      (case name
	((u)
	 (let ((stack (value-stack)))
	   (cond ((null? stack)
		  (write-line "Can't go up from here." (command-output)))
		 ((not stack)
		  (write-line "No value stack: not in inspection mode."
			      (command-output)))
		 (else
		  (pop-value-stack!)
		  (set-command-results! (list (focus-object)))))))
	((d)
	 (if (continuation? (focus-object))
	     (new-selection (continuation-cont (focus-object)))
	     (write-line "Can't go down from a non-continuation."
			 (command-output))))
	((template)
	 (let ((template (coerce-to-template (focus-object))))
	   (if template
	       (new-selection template)
	       (write-line
		 (if (continuation? (focus-object))
		     "Unable to locate a template in this continuation."
		     "Not a procedure or a continuation.")
		 (command-output)))))
	(else
	 (error "bad selection command" name)))))

(define (coerce-to-template obj)
  (cond ((template? obj)
	 obj)
	((closure? obj)
	 (closure-template obj))
	((continuation? obj)
	 (continuation-template obj))
	(else
	 #f)))

(define (selection-command name)
  (lambda more-commands
    (execute-selection-commands (cons name more-commands))))
    
(define template (selection-command 'template))
(define u (selection-command 'u))
(define d (selection-command 'd))

(define-command-syntax 'template "" "inspect template"    '(&rest selection-command))
(define-command-syntax 'u        "" "pop inspector stack" '(&rest selection-command))
(define-command-syntax 'd        "" "down stack"          '(&rest selection-command))

;----------------
; A command to print out the file in which a procedure is defined.
; Why is this here and not in debug.scm?

(define-command-syntax 'where "[<procedure>]"
  "show procedure's source file name"
  '(&opt expression))

(define (where . maybe-exp)
  (let ((proc (if (null? maybe-exp)
		  (focus-object)
		  (eval (car maybe-exp) (environment-for-commands))))
	(port (command-output)))
    (if (procedure? proc)
	(let ((probe (where-defined proc)))
	  (if probe
	      (display probe port)
	      (display "Source file not recorded" port)))
	(display "Not a procedure" port))
    (newline port)))

(define (where-defined thing)
  (let loop ((dd (template-debug-data (closure-template thing))))
    (if (debug-data? dd)
	(if (string? (debug-data-name dd))
	    (debug-data-name dd)
	    (loop (debug-data-parent dd)))
	#f)))




