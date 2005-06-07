; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; The user's state is in two parts:
; User context - preserved across dump commands (but not by us).
;   This includes the designated user and configuration environments
;   and the values of a bunch of user-preference switches.
;
;   Static
;    command-environment
;    command-syntax-table
;    user-command-environment
;    user-command-help
;    user-environment
;    config-package
;    traced (?)
;    file-environments
;    
;   Modified
;    break-on-warnings?
;    load-noisily?
;    ask-before-loading?
;
; User session state - one per "login"; not preserved across dump commands.
;   Kept in a fluid variable in the command-levels scheduler thread.
;   More pedestrian threads access it via an upcall.
;
;   Static
;    user-context
;    command input, output, and error ports
;    command thread (for spawning threads)
;   Modified
;    focus value (##)
;    batch?
;    exit-status

;----------------
; User context.
;
; This is a symbol table stored in a slot in the session state (see below).
; *USER-CONTEXT-INITIALIZERS* is a list of (<name> . <initial-value-thunk>)
; pairs.  The <thunk>s are called to get the initial value of the <name>d
; slots.

(define (make-user-context)
  (let ((context (make-symbol-table)))
    (for-each (lambda (name+thunk)
		(table-set! context (car name+thunk) ((cdr name+thunk))))
	      *user-context-initializers*)
    context))

(define *user-context-initializers* '())

; Add a new slot to the user context.

(define (user-context-accessor name initializer)
  (set! *user-context-initializers*
        (append *user-context-initializers*
                (list (cons name initializer))))
  (lambda ()
    (table-ref (user-context) name)))

(define (user-context-modifier name)
  (lambda (new)
    (table-set! (user-context) name new)))

; Various bits of context.

(define break-on-warnings?      (user-context-accessor 'break-on-warnings?
						       (lambda () #f)))
(define set-break-on-warnings?! (user-context-modifier 'break-on-warnings?))

(define load-noisily?           (user-context-accessor 'load-noisily?
						       (lambda () #f)))
(define set-load-noisily?!      (user-context-modifier 'load-noisily?))

;----------------
; User session state.
;
; User information relevant to a particular session (`login').
;
; There isn't so much of this, so we just use a record.

(define-record-type user-session :user-session
  (make-user-session command-thread
		     user-context
		     script-thunk repl-thunk
		     command-input command-output command-error-output
		     focus-object
		     exit-status
		     batch-mode?
		     script-mode?)
  user-session?
  (command-thread	user-session-command-thread)
  (repl-thunk		user-session-repl-thunk)
  (script-thunk		user-session-script-thunk)
  (user-context		user-session-user-context)
  (command-input 	user-session-command-input)
  (command-output	user-session-command-output)
  (command-error-output user-session-command-error-output)
  (exit-status  user-session-exit-status  set-user-session-exit-status!)
  (batch-mode?  user-session-batch-mode?  set-user-session-batch-mode?!)
  (script-mode? user-session-script-mode? set-user-session-script-mode?!)
  (focus-object user-session-focus-object set-user-session-focus-object!))

; Two local macros that do a bit of name mangling.
;
; (define-session-slot <name>)
;  ->
; (define (<name>)
;   (user-session-<name> (user-session)))
;
; (define-settable-session-slot <name>)
;  ->
; (begin
;   (define (<name>)
;     (user-session-<name> (user-session)))
;   (define (set-<name>! value)
;     (set-user-session-<name>! (user-session) value)))

(define-syntax define-session-slot
  (lambda (e r c)
    (let* ((name (cadr e))
	   (sconc (lambda args
		    (string->symbol (apply string-append args))))
	   (read (sconc "user-session-" (symbol->string name))))
      `(define (,name)
	 ;(debug-message "[u-s " ',(cadr e) "]" )
	 (,read (user-session))))))

(define-syntax define-settable-session-slot
  (lambda (e r c)
    (let* ((name (cadr e))
	   (string-name (symbol->string name))
	   (sconc (lambda args
		    (string->symbol (apply string-append args))))
	   (read (sconc "user-session-" string-name))
	   (write (sconc "set-user-session-" string-name "!"))
	   (write-name (caddr e)))
      `(begin
	 (define (,name)
	   ;(debug-message "[u-s " ',name "]" )
	   (,read (user-session)))
	 (define (,write-name value)
	   ;(debug-message "[u-s! " ',name "]" )
	   (,write (user-session) value))))))

(define-session-slot command-thread)
(define-session-slot user-context)
(define-session-slot command-input)
(define-session-slot command-output)
(define-session-slot command-error-output)

(define-settable-session-slot focus-object really-set-focus-object!)
(define-settable-session-slot batch-mode?  set-batch-mode?!)
(define-settable-session-slot exit-status  set-exit-status!)

; If we get new focus values we clear the menu, add the old focus values to
; the stack, if there is one, and actually set the focus values.

(define (set-focus-object! value)
  (set-menu! #f)
  (let ((old (focus-object)))
    (really-set-focus-object! value)
    (if (and (value-stack)
	     (not (eq? old (focus-object))))
	(set-value-stack! (cons old (value-stack))))))

(define (pop-value-stack!)
  (set-menu! #f)
  (let ((stack (value-stack)))
    (set-focus-object! (car stack))
    (set-value-stack! (cdr stack))))
