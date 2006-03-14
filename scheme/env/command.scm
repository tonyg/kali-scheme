; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Interpreting commands.

; Commands begin with a comma because it's an unshifted key and because
; someone else (I can't remember who) was already using it as a command
; prefix.

(define command-prefix #\,)

; Fire up the processor.
;
; The double-paren around the WITH-HANDLER is because it returns a
; thunk which is the thing to do after the command-processor exits.
;
; There are two version, one for an initial start and the other for restarting
; with an existing user context.

(define (start-command-processor resume-args greeting-thunk start-thunk)
  (restart-command-processor resume-args #f greeting-thunk start-thunk))

(define (restart-command-processor resume-args context greeting-thunk start-thunk)
  ((with-handler command-loop-condition-handler
     (lambda ()
       (start-command-levels resume-args
			     context
			     greeting-thunk
			     start-thunk
			     script-runner
			     real-command-loop
			     #f			; no condition
			     #f			; not inspecting
			     (current-input-port)
			     (current-output-port)
			     (current-error-port))))))

; Entry for initialization & testing.

(define (command-processor command-env resume-args)
  (start-command-processor resume-args
			   values
			   (lambda ()
			     (set-user-command-environment! command-env)
			     unspecific)))

;----------------
; Command loop
; Called from:
;  1. condition handler, 2. abort-to-level, 3. breakpoint
;
; The condition is either #F or whatever caused a new command loop to be
; started.

(define (command-loop condition)
  (push-command-level condition #f))

; Install the handler, bind $NOTE-UNDEFINED to keep from annoying the user,
; bind $FOCUS-BEFORE to avoid keeping state on the stack where it can be
; captured by CALL/CC, display the condition and start reading commands.

(define (real-command-loop)
  (let ((results-cell (make-cell #f)))
    (let-fluids $note-undefined #f    ;useful
		$command-results results-cell
      (lambda ()
	(display-command-level-condition
	  (command-level-condition (command-level)))
	(let command-loop ()
	  (let ((command (read-command-carefully (command-prompt)
						 (form-preferred?)
						 (command-input))))
	    (cell-set! results-cell #f)
	    (execute-command command)
	    (let ((results (cell-ref results-cell)))
	      (if results
		  (show-command-results results)))
	    (command-loop)))))))

; For saving the results returned by a command.

(define $command-results (make-fluid #f))

(define (set-command-results! results . maybe-set-focus-object?)
  (fluid-cell-set! $command-results results)
  (if (or (null? maybe-set-focus-object?)
	  (car maybe-set-focus-object?))
      (case (length results)
	((0)
	 (values))
	((1)
	 (if (not (eq? (car results)
		       (unspecific)))
	     (set-focus-object! (car results))))
	(else
	 (set-focus-object! results)))))

(define (display-command-level-condition condition)
  (if condition
      (display-condition condition (command-output))))

; If #T anything that doesn't start with the command prefix (a comma) is
; treated as an argument to RUN.  If #F no commas are needed and RUN
; commands must be explicit.

(define (form-preferred?)
  (not (value-stack)))

; If true then print a menu when showing results.

(define (inspect-mode?)
  (value-stack))

; Go up to the previous level or exit if there are no more levels.

(define (pop-command-level)
  (let ((levels (command-levels)))
    (if (null? (cdr levels))
        (cond ((batch-mode?)
	       ; perhaps this should use scheme-exit-now, but I'm
	       ; worried that it is what handles normal EOF. (HCC)
	       (exit-command-processor (lambda () 0)))
	      ((y-or-n? "Exit Scheme 48" #t)
	       (exit-command-processor (lambda () 1)))
	      (else
	       (abort-to-command-level (car levels))))
	(let ((level (cadr (command-levels))))
	  (if (command-level-paused-thread level)
	      (kill-paused-thread! level))
	  (proceed-with-command-level level)))))

(define (exit-command-processor thunk)
  (throw-to-command-level (top-command-level)
			  (lambda () thunk)))

; Condition handler.
; For warnings and notes we go stop the current level or continue, for
; errors and interrupts we stop the level or exit.  We always continue for
; warnings on the command level thread to avoid circularity problems.

(define (command-loop-condition-handler c next-handler)
  (let ((c (coerce-to-condition c)))
    (cond ((or (warning? c)
	       (note? c))
	   (if (break-on-warnings?)
	       (deal-with-condition c)
	       (begin (force-output (current-output-port)) ; keep synchronous
		      (display-condition c (current-error-port))
		      (unspecific))))	;proceed
	  ((or (error? c) (bug? c) (interrupt? c))
	   (if (batch-mode?)
	       (begin (force-output (current-output-port)) ; keep synchronous
		      (display-condition c (current-error-port))
		      (let ((status
			     (cond
			      ((error? c) 1)
			      ((bug? c) 3) ; historical, probably nonsense
			      (else 2))))
			(scheme-exit-now status)))
	       (deal-with-condition c)))
	  ((reset-command-input? c)
	   (unspecific))		;proceed
	  (else                           
	   (next-handler)))))

; Stop the current level either by pushing a new one or restarting it.
; If we restart the current level we save it as the focus object to give
; the user a chance to figure out what happened.

(define (deal-with-condition c)
  (if (push-command-levels?)
      (command-loop c)
      (let ((level (car (command-levels))))
	(set-focus-object! level)
	(display-condition c (command-output))
	(restart-command-level level))))

(define (abort-to-command-level level)
  (cond ((eq? level (car (reverse (command-levels))))
	 (newline (command-output))
	 (write-line "Top level" (command-output)))
	(else
	 (display "Back to " (command-output))))
  (restart-command-level level))

; The prompt is "level-number environment-id-string> " or just
; "environment-id-string> " at top level.  The id-string is empty for the
; current user package and the name of the package otherwise.
; The ">" changes to ":" in command-preferred mode.

(define (command-prompt)
  (let ((level (- (length (command-levels)) 1))
	(id (environment-id-string (environment-for-commands))))
    (string-append (if (= level 0)
		       ""
		       (number->string level))
		   (if (or (= level 0) (= (string-length id) 0))
		       ""
		       " ")
		   id
		   (if (form-preferred?)
		       "> "
		       ": "))))

(define-generic environment-id-string &environment-id-string (env))

(define-method &environment-id-string (env) "")

;----------------
; Loading scripts

(define (script-runner)
  (run-script (cdr (focus-object))))

; This loads a script SRFI-22-style

; ARG is a list of command-line arguments after "run-script"

(define (run-script arg)
  (run-script-handler (car arg) (cdr arg)))

(define *script-handler-alist* '())

(define (define-script-handler tag proc)
  (set! *script-handler-alist*
	(cons (cons tag proc) *script-handler-alist*)))

(define (run-script-handler tag args)
  (cond
   ((assoc tag *script-handler-alist*)
    => (lambda (pair)
	 (silently
	  (lambda ()
	    ((cdr pair) args)))))
   (else
    (display "invalid argument to run-script-handler:" (current-error-port))
    (display tag (current-error-port))
    (newline (current-error-port))
    1)))

(define (EX_SOFTWARE) (shared-binding-ref (lookup-imported-binding "EX_SOFTWARE")))

(define (with-srfi-22-error-handling thunk)
  (call-with-current-continuation
   (lambda (k)
     (with-handler
      (lambda (c punt)
	(let ((c (coerce-to-condition c)))
	  (if (or (error? c) (bug? c))
	      (begin
		(display-condition c (current-error-port))
		(k (EX_SOFTWARE)))
	      (punt))))
      (lambda ()
	(thunk)
	0)))))

(define-script-handler "r5rs"
  (lambda (args)
    (with-srfi-22-error-handling
     (lambda ()
       (load-script-into (car args) (interaction-environment))
       ((environment-ref (interaction-environment) 'main) args)))))

(define-script-handler "srfi-7"
  (lambda (args)
    (with-srfi-22-error-handling
     (lambda ()
       (eval '(load-package 'srfi-7) (user-command-environment))
       (eval `(load-srfi-7-script 'srfi-7-script ,(car args))
	     (user-command-environment))
       (let ((cell (make-cell #f)))	; kludge
	 (let-fluid $command-results cell
	  (lambda ()
	    (eval '(in 'srfi-7-script '(run main))
		  (user-command-environment))))
	 ((car (cell-ref cell)) args))))))

;----------------
; Evaluate a form and save its result as the current focus values.
; The unspecific object is discarded.

(define (evaluate-and-select form env)
  (call-with-values (lambda ()
                      (eval form env))
    (lambda results
      (set-command-results! results)
      (apply values results))))

;----------------
; Printing command results.  The results are also saved as the current
; focus object.

(define (show-command-results results)
  (let ((out (command-output)))
    (case (length results)
      ((0)
       (display "; no values returned" out)
       (newline out))
      ((1)
       (show-command-result (car results))
       (if (inspect-mode?)
	   (present-menu)))
      (else
       (display "; " out)
       (write (length results) out)
       (display " values returned" out)
       (if (inspect-mode?)
	   (present-menu)
	   (begin
	     (newline out)
	     (for-each show-command-result results)))))))

(define (show-command-result result)
  (let ((out (command-output)))
    ((if (inspect-mode?)
	 with-limited-output
	 (lambda (p) (p)))
     (lambda ()
       (write-carefully (value->expression result)
			out)
       (newline out)))))

;----------------
; Sentinels - run after every command.

(define *sentinels* '())

(define (run-sentinels)
  (for-each (lambda (sentinel) (sentinel)) *sentinels*))

(define (add-sentinel! sentinel)
  (if (not (memq sentinel *sentinels*))
      (set! *sentinels* (cons sentinel *sentinels*))))

;----------------
; Commands.

(define environment-for-commands interaction-environment)

(define command-environment
  (user-context-accessor 'command-environment interaction-environment))

;(define *command-structure* (unspecific))
;
;(define (command-structure)
;  *command-structure*)
;
;(define (set-command-structure! structure)  ; called on initial startup
;  (set! *command-structure* structure))

(define command-syntax-table (make-table))
(define *command-help* '())

(define (get-command-syntax name)
  (or (table-ref (user-command-syntax-table) name)
      (table-ref command-syntax-table name)))

(define (define-command-syntax name help1 help2 arg-descriptions)
  (table-set! command-syntax-table name arg-descriptions)
  (if help1
      (set! *command-help* (add-help *command-help* name help1 help2))))

(define (add-help help name help1 help2)  
  (insert (list name
                (string-append (symbol->string name) " " help1)
                help2)
          help
          (lambda (z1 z2)
            (string<=? (cadr z1) (cadr z2)))))

(define user-command-syntax-table
  (user-context-accessor 'user-command-syntax-table (lambda () (make-table))))

(define user-command-environment
  (user-context-accessor 'user-command-environment (lambda () #f)))

(define set-user-command-environment!
  (user-context-modifier 'user-command-environment))

(define user-command-help
  (user-context-accessor 'user-command-help (lambda () *command-help*)))

(define set-user-command-help!
  (user-context-modifier 'user-command-help))

(define (define-user-command-syntax name help1 help2 arg-descriptions)
  (table-set! (user-command-syntax-table) name arg-descriptions)
  (if help1
      (set-user-command-help!
           (add-help (user-command-help) name help1 help2))))

(define (execute-command command)
  (run-sentinels)
  (cond ((eof-object? command)
         (newline (command-output))
         (pop-command-level))
        ((not command))       ; error while reading
        (else
         (let* ((name (car command))
		(proc (eval name (user-command-environment))))
	   (apply proc (cdr command))))))

;----------------
; Settings - these are cells for controlling the behavior
; of the command interpreter.
;
; This code is here so that the help listing can print out the settings
; and their current values.

(define-record-type setting :setting
  (make-setting name type get set on-doc off-doc)
  setting?
  (name setting-name)
  ;; is #t for boolean or a predicate
  (type setting-type)
  (get setting-get)
  (set setting-set)
  ;; We have two documentation strings, one for `on' and one for `off'.
  (on-doc setting-on-doc)
  (off-doc setting-off-doc))

(define (setting-boolean? setting)
  (eqv? #t (setting-type setting)))

; alist mapping names to :SETTING records
(define *settings-alist* '())

(define (lookup-setting name)
  (cond
   ((assq name *settings-alist*) => cdr)
   (else #f)))

(define (add-setting name boolean? get set on-doc . maybe-off-doc)
  (set! *settings-alist*
	(insert (cons name
		      (make-setting name boolean? get set on-doc
			      (if (null? maybe-off-doc)
				  #f
				  (car maybe-off-doc))))
		*settings-alist*
		(lambda (z1 z2)
		  (string<=? (symbol->string (car z1))
			     (symbol->string (car z2)))))))

(define (setting-value setting)
  ((setting-get setting)))

(define (setting-set! setting value)
  (if (if (setting-boolean? setting)
	  (and (not (eqv? value #t))
	       (not (eqv? value #f)))
	  (not ((setting-type setting) value)))
      (error "invalid value for setting" (setting-name setting) value))
  ((setting-set setting) value))

(define (setting-doc setting)
  (cond
   ((not (setting-boolean? setting))
    (setting-on-doc setting))
   ((setting-value setting)
    (setting-on-doc setting))
   (else
    (setting-off-doc setting))))

; Print out a list of the settings and their current values.

(define (list-settings)
  (let ((o-port (command-output))
	(size (apply max
		     (map (lambda (z)
			    (string-length (symbol->string (setting-name (cdr z)))))
			  *settings-alist*))))
    (for-each (lambda (z)
		(let* ((setting (cdr z))
		       (name (symbol->string (setting-name setting))))
		  (display #\space o-port)
		  (display name o-port)
		  (display #\space o-port)
		  (write-spaces (- size (string-length name)) o-port)
		  (display #\( o-port)
		  (cond
		   ((not (setting-boolean? setting))
		    (write (setting-value setting) o-port)
		    (display ", " o-port)
		    (display (setting-on-doc setting) o-port))
		   ((setting-value setting)
		    (display "on, " o-port)
		    (display (setting-on-doc setting) o-port))
		   (else
		    (display "off, " o-port)
		    (display (setting-off-doc setting) o-port)))
		  (display #\) o-port)
		  (newline o-port)))
	      *settings-alist*)))


;----------------
; help

(define (help . maybe-id)
  (if (null? maybe-id)
      (list-commands)
      (print-command-help (car maybe-id))))

(define (print-command-help id)
  (let ((o-port (command-output)))
    (display #\space o-port)
    (cond ((assq id (user-command-help))
           => (lambda (data)
                (if (form-preferred?) (display command-prefix o-port))
                (display (cadr data) o-port)
                (display "    " o-port)
                (display (caddr data) o-port)))
          (else
           (display #\" o-port)
           (display id o-port)
           (display #\" o-port)
           (display #\space o-port)
           (display "is not a command.")))
    (newline o-port)))

(define (list-commands)
  (let ((o-port (command-output))
        (widest 28)
        (f? (form-preferred?)))
    (for-each (lambda (s)
                (write-line s o-port))
              '(
"This is Scheme 48.  You are interacting with the command processor."
"A command is either a Scheme form to evaluate or one of the following:"
""))

    (list-command-help (user-command-help) f? o-port)
    (for-each (lambda (s)
                (write-line s o-port))
              '(
""
"Square brackets [...] indicate optional arguments."
""
"The following settings are set by the `set' and `unset' commands:"
""
                ))
    (list-settings)
    (for-each (lambda (s)
                (write-line s o-port))
              '(
""		
"The expression ## evaluates to the last value displayed by the command"
"processor."
                ))))

(define (list-command-help data prefix? o-port)
  (let* ((strings (map (if prefix?
                           (lambda (d)
                             (string-append (command-prefix-string
                                             command-prefix)
                                            (cadr d)))
                           cadr)
                       data))
         (count (length strings))
         (back-half (list-tail strings (quotient (+ 1 count) 2))))
    (let loop ((s1 strings) (s2 back-half))
      (cond ((not (eq? s1 back-half))
             (display #\space o-port)
             (display (car s1) o-port)
             (write-spaces (max 1 (- 32 (string-length (car s1)))) o-port)
             (if (not (null? s2))
                 (display (car s2) o-port))
             (newline o-port)
             (loop (cdr s1) (if (null? s2) s2 (cdr s2))))))))
                   
;----------------
; Utilities

(define (error-form proc args)
  (cons proc (map value->expression args)))

; Print non-self-evaluating value X as 'X.

(define (value->expression obj)         ;mumble
  (if (or (symbol? obj)
	  (pair? obj)
	  (null? obj)
	  (vector? obj))
      `',obj
      obj))

(define (write-spaces count o-port)
  (do ((count count (- count 1)))
      ((<= count 0))
    (display #\space o-port)))

(define (command-prefix-string prefix)
  (cond ((string? prefix) prefix)
        ((char? prefix) (string prefix))
        ((symbol? prefix) (symbol->string prefix))))

(define (y-or-n? question eof-value)
  (let ((i-port (command-input))
        (o-port (command-output)))
    (let loop ((count *y-or-n-eof-count*))
      (display question o-port)
      (display " (y/n)? " o-port)
      (let ((line (read-line i-port)))
        (cond ((eof-object? line)
               (newline o-port)
               (if (= count 0)
                   eof-value
                   (begin (display "I'll only ask another " o-port)
                          (write count o-port)
                          (display " times." o-port)
                          (newline o-port)
                          (loop (- count 1)))))
              ((< (string-length line) 1) (loop count))
              ((char=? (string-ref line 0) #\y) #t)
              ((char=? (string-ref line 0) #\n) #f)
              (else (loop count)))))))

(define *y-or-n-eof-count* 100)

(define (read-line port)
  (let loop ((l '()))
    (let ((c (read-char port)))
      (if (eof-object? c)
          c
          (if (char=? c #\newline)
              (list->string (reverse l))
              (loop (cons c l)))))))

(define (greet-user info)
  (let ((port (command-output)))
    (display "Welcome to Scheme 48 " port)
    (display version-info port)
    (if info
        (begin (write-char #\space port)
               (display info port)))
    (newline port)
    (write-line "Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees."
		port)
    (write-line "Please report bugs to scheme-48-bugs@s48.org."
                port)
    (write-line "Get more information at http://www.s48.org/."
		port)
    (if (not (batch-mode?))
	(write-line "Type ,? (comma question-mark) for help." port))))

(define (command-continuation)          ;utility for debugger
  (let ((obj (focus-object)))
    (cond ((debug-command-level)
	   => (lambda (level)
		(if (command-level-paused-thread level)
		    (thread-continuation (command-level-paused-thread level))
		    (let ((threads (command-level-threads level)))
		      (if (= 1 (length threads))
			  (thread-continuation (car threads))
			  #f)))))
	  ((continuation? obj)
	   obj)
	  ((thread? obj)
	   (thread-continuation obj))
	  (else #f))))

(define (command-threads)               ;utility for debugger
  (let ((level (debug-command-level)))
    (if level
	(command-level-threads level)
	#f)))

(define (debug-command-level)  
  (let* ((obj (focus-object)))
    (if (command-level? obj)
	obj
	(let ((levels (command-levels)))
	  (if (null? (cdr levels))
	      #f
	      (cadr levels))))))
