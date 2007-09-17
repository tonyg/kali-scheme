; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Commands for debugging.

; translate

(define-command-syntax 'translate "<from> <to>"
  "establish file name translation"
  '(filename filename))

(define translate set-translation!)

; preview  -- show continuations

(define (preview)
  (let ((cont (command-continuation)))
    (if cont
	(display-preview (continuation-preview cont)
			 (command-output)))))

(define (display-preview preview port)
  (for-each (lambda (info+pc)
	      (if (not (fluid-let-continuation-info? (car info+pc)))
		  (display-template-names (car info+pc) port)))
	    preview))

(define (display-template-names info port)
  (let ((names (debug-data-names info)))
    (display "  " port)
    (if (null? names)
	(begin (display "unnamed " port)
	       (write `(id ,(if (debug-data? info)
				(debug-data-uid info)
				info))
		      port))
	(let loop ((names names))
	  (if (car names)
	      (write (car names) port)
	      (display "unnamed" port))
	  (if (and (not (null? (cdr names)))
		   (cadr names))
	      (begin (display " in " port)
		     (loop (cdr names))))))
    (newline port)))

(define fluid-let-continuation-info?	;Incestuous!
  (let ((id (let-fluid (make-fluid #f) #f
	      (lambda ()
		(primitive-catch (lambda (k)
				   (let ((template (continuation-template k)))
				     (if template
					 (template-id template)
					 #f))))))))
    (lambda (info) 
      (eqv? (if (debug-data? info)
		(debug-data-uid info)
		info)
	    id))))

(define-command-syntax 'preview ""
  "show pending continuations (stack trace)"
  '())

; Proceed

(define (really-proceed vals)
  (let* ((level (command-level))
	 (condition (command-level-condition level)))
    (if (ok-to-proceed? condition)
	(apply proceed-with-command-level (cadr (command-levels)) vals)
	(begin
	  (write-line "No way to proceed from here." (command-output))
	  (write condition (command-output))
	  (newline (command-output))))))

(define-command-syntax 'proceed "<exp>" "proceed after an interrupt or error"
  '(&rest expression))

(define (proceed . exps)
  (really-proceed (map (lambda (exp)
			 (eval exp (environment-for-commands)))
		       exps)))

; Scrutinize the condition to ensure that it's safe to return from the
; call to RAISE.

(define (ok-to-proceed? condition)
  (and condition
       (if (serious-condition? condition)
	   (and (vm-exception? condition)
		(let ((opcode (vm-exception-opcode condition)))
		  (or (= opcode (enum op global))
		      (>= opcode (enum op eq?)))))
	   #t)))

(define (breakpoint . rest)
  (command-loop (conditions:condition (make-breakpoint-condition)
				      (make-who-condition 'breakpoint)
				      (make-irritants-condition rest))))

(define-condition-type &breakpoint &condition
  make-breakpoint-condition breakpoint-condition?)

; push

(define-command-syntax 'push "" "push command level" '())

(define (push)
  (command-loop (if (command-level? (focus-object))
		    (command-level-condition (focus-object))
		    #f)))

; pop (same as ^D (= end-of-file))

(define-command-syntax 'pop "" "pop command level" '())

(define (pop)
  (pop-command-level))

; reset

(define (reset . maybe-level)
  (if (null? maybe-level)
      (abort-to-command-level (top-command-level))
      (go-to-level (car maybe-level))))

(define-command-syntax 'reset "[<number>]"
  "restart command level (default level is 0)"
  '(&opt expression))

(define (go-to-level n)
  (let ((level (find-level n)))
    (if level
	(abort-to-command-level level)
	(write-line "invalid command level" (command-output)))))

; Old ,level command retained for compatibility.
; Has no help strings s it won't show up in the ,? list.

(define-command-syntax 'level #f #f '(expression))

(define level go-to-level)

; Find the Nth command level.

(define (find-level n)
  (let ((levels (reverse (command-levels))))
    (if (and (integer? n)
	     (>= n 0)
	     (< n (length levels)))
	(list-ref levels n)
	#f)))

; resume
; Same as ,reset except that we don't restart the level.

(define (resume . maybe-level)
  (let ((level (if (null? maybe-level)
		   (top-command-level)
		   (find-level (car maybe-level)))))
    (if level
	(begin
	  (if (command-level-paused-thread level)
	      (kill-paused-thread! level))
	  (proceed-with-command-level level))
	(write-line "invalid command level" (command-output)))))

(define-command-syntax 'resume "[<number>]"
  "resume specific command level (default is 0)"
  '(&opt expression))

(define-command-syntax 'condition ""
  "select an object that describes the current error condition"
  '())

(define (condition)
  (let ((c (command-level-condition (command-level))))
    (if c
	(set-command-results! (list c) #t)
	(write-line "no condition" (command-output)))))

; Toggling various boolean flags.

(define-command-syntax 'set "<setting> [<on-or-off-or-literal-or-?>]"
  "set the value of a setting (? lists settings)"
  '(name &opt literal))

(define-command-syntax 'unset "<setting>"
  "set boolean setting to off"
  '(name))

(define (set name . maybe-value)
  (if (eq? name '?)
      (list-settings)
      (let* ((setting (lookup-setting name))
	     (value (cond ((not setting)
			   (assertion-violation 'set "setting not found" name))
			  ((null? maybe-value)
			   (if (setting-boolean? setting)
			       #t
			       (assertion-violation 'set "no value specified")))
			  ((eq? (car maybe-value) '?)
			   (if (setting-boolean? setting)
			       (display (if (setting-value setting)
					    "on, "
					    "off, ")
					(command-output)))
			   (setting-value setting))
			  ((setting-boolean? setting)
			   (case (car maybe-value)
			     ((off) #f)
			     ((on) #t)
			     (else
			      (assertion-violation
			       'set
			       "invalid value for boolean setting; should be on or off"))))
			  (else
			   (car maybe-value))))
	     (out (command-output)))
	(setting-set! setting value)
	(display (setting-doc setting) out)
	(if (not (setting-boolean? setting))
	    (begin
	      (display " is " (command-output))
	      (write value (command-output))))
	(newline out))))
	 
(define (unset name)
  (let ((setting (lookup-setting name))
	(out (command-output)))
    (if (not setting)
	(assertion-violation 'unset "setting not found" name)
	(setting-set! setting #f))
    (display (setting-doc setting) out)
    (newline out)))

; The actual settings.

(define (positive-integer? n)
  (and (integer? n)
       (exact? n)
       (positive? n)))

(add-setting 'batch #t
	     batch-mode?
	     set-batch-mode?!
	     "will not prompt and will exit on errors"
	     "will prompt and will not exit on errors")

(add-setting 'inline-values #t
	     (lambda ()
	       (package-integrate? (environment-for-commands)))
	     (lambda (b)
	       (set-package-integrate?! (environment-for-commands) b))
	     "will compile some calls in line"
	     "will not compile calls in line")

(add-setting 'break-on-warnings #t
	     break-on-warnings?
	     set-break-on-warnings?!
	     "will enter breakpoint on warnings"
	     "will not enter breakpoint on warnings")

(add-setting 'load-noisily #t
	     load-noisily?
	     set-load-noisily?!
	     "will notify when loading modules and files"
	     "will not notify when loading modules and files")

;(add-setting 'form-preferred #t
;             form-preferred?
;             set-form-preferred?!
;             "commas are required before commands"
;             "commas are not required before commands")

(add-setting 'levels #t
	     push-command-levels?
	     set-push-command-levels?!
	     "will push command level on errors"
	     "will not push command level on errors")

(add-setting 'inspector-menu-limit positive-integer?
	     inspector-menu-limit
	     set-inspector-menu-limit!
	     "maximum number of menu entries in inspector")

(add-setting 'inspector-writing-depth positive-integer?
	     inspector-writing-depth
	     set-inspector-writing-depth!
	     "maximum writing depth in inspector")

(add-setting 'inspector-writing-length positive-integer?
	     inspector-writing-length
	     set-inspector-writing-length!
	     "maximum writing length in inspector")

(add-setting 'condition-writing-length positive-integer?
	     condition-writing-length
	     set-condition-writing-length!
	     "maximum writing length for conditions")

(add-setting 'condition-writing-depth positive-integer?
	     condition-writing-depth
	     set-condition-writing-depth!
	     "maximum writing depth for conditions")

; Old toggling commands retained for compatibility
; These have no help strings.

(define (define-toggle-syntax name help)
  (define-command-syntax name #f #f '(&opt name)))
  
(define (toggle-command name)
  (lambda maybe-value
    (set name (if (null? maybe-value)
		  (if (setting-value (or (lookup-setting name)
					 (assertion-violation 'toggle "setting not found" name)))
		      'off
		      'on)
		  (car maybe-value)))))

(define-toggle-syntax 'batch 
  "enable/disable batch mode (no prompt, errors exit)")

(define batch (toggle-command 'batch))

(define-toggle-syntax 'bench
  "enable/disable inlining of primitives")

(define bench (toggle-command 'inline-values))

(define-toggle-syntax 'break-on-warnings
  "treat warnings as errors")

(define break-on-warnings (toggle-command 'break-on-warnings))

;(define-toggle-syntax 'form-preferred
;  "enable/disable form-preferred command processor mode")
;
;(define form-preferred (toggle-command 'form-preferred))

(define-toggle-syntax 'levels
  "disable/enable command levels")

(define levels (toggle-command 'levels))

; Flush debug data base

(define-command-syntax 'flush "[<kind> ...]"
  "start forgetting debug information
Kind should be one of: names maps files source tabulate
 location-names file-packages"
  '(&rest name))

(define (flush . kinds)
  (cond ((null? kinds)
	 (write-line "Flushing location names and tabulated debug info"
		     (command-output))
	 (flush-location-names)
	 ((debug-flag-modifier 'table) (make-table)))
	(else
	 (for-each (lambda (kind)
		     (cond ((memq kind debug-flag-names)
			    ((debug-flag-modifier kind)
			     (if (eq? kind 'table) (make-table) #f)))
			   ((eq? kind 'location-names)
			    (flush-location-names))
			   ((eq? kind 'file-packages)
			    (forget-file-environments))
			   (else
			    (write-line "Unrecognized debug flag"
					(command-output)))))
		   kinds))))

; Control retention of debugging information

(define-command-syntax 'keep "[<kind> ...]"
  "start remembering debug information
Kind should be one of: names maps files source tabulate"
  '(&rest name))

(define (keep . kinds)
  (let ((port (command-output)))
    (if (null? kinds)
	(for-each (lambda (kind)
		    (if (not (eq? kind 'table))
			(begin
			  (display (if ((debug-flag-accessor kind))
				       "+ " "- ")
				   port)
			  (display kind port)
			  (newline port))))
		  debug-flag-names)
	(for-each (lambda (kind)
		    (if (and (memq kind debug-flag-names)
			     (not (eq? kind 'table)))
			((debug-flag-modifier kind) #t)
			(write-line "Unrecognized debug flag"
				    port)))
		  kinds))))


; Collect some garbage

(define (collect)
  (let ((port (command-output))
	(available-before (available-memory))
	(heap-size-before (heap-size)))
    (primitives:collect)
    (let ((available-after (available-memory))
	  (heap-size-after (heap-size)))
      (display "Before: " port)
      (write available-before port)
      (display " out of " port)
      (display heap-size-before port)
      (display" words available" port)
      (newline port)
      (display "After:  " port)
      (write available-after port)
      (display " out of " port)
      (display heap-size-after port)
      (display " words available" port)
      (newline port))))

(define (available-memory)
  (primitives:memory-status (enum memory-status-option available)
			    #f))

(define (heap-size)
  (primitives:memory-status (enum memory-status-option heap-size)
			    #f))

(define-command-syntax 'collect "" "invoke the garbage collector" '())

; Undefined (this is sort of pointless now that NOTING-UNDEFINED-VARIABLES
; exists)
;
;(define (show-undefined-variables)
;  (let ((out (command-output))
;        (undef (undefined-variables (environment-for-commands))))
;    (if (not (null? undef))
;        (begin (display "Undefined: " out)
;               (write undef out)
;               (newline out)))))
;
;(define-command-syntax 'undefined "" "list undefined variables"
;  '() show-undefined-variables)



; Trace and untrace

(define traced-procedures
  (user-context-accessor 'traced (lambda () '())))
(define set-traced-procedures!
  (user-context-modifier 'traced))

(define (trace . names)
  (if (null? names)
      (let ((port (command-output)))
	(write (map car (traced-procedures)) port)
	(newline port))
      (for-each trace-1 names)))

(define-command-syntax 'trace "<name> ..."
  "trace calls to given procedure(s)"
  '(&rest name))

(define (untrace . names)
  (if (null? names)
      (for-each untrace-1 (map car (traced-procedures)))
      (for-each untrace-1 names)))

(define-command-syntax 'untrace "<name> ..." "stop tracing calls"
  '(&rest name))

(add-setting 'trace-writing-depth positive-integer?
	     trace-writing-depth
	     set-trace-writing-depth!
	     "writing depth for traces")

; Trace internals

(define (trace-1 name)
  (let* ((env (environment-for-commands))
	 (proc (environment-ref env name))
	 (traced (make-traced proc name)))
    (set-traced-procedures!
		(cons (list name traced proc env)
		      (traced-procedures)))
    (environment-define! env name traced))) ;was environment-set!
	   
; Should be doing clookup's here -- avoid creating new locations

(define (untrace-1 name)
  (let ((probe (assq name (traced-procedures))))
    (if probe
	(let* ((traced (cadr probe))
	       (proc (caddr probe))
	       (env (cadddr probe)))
	  (if (eq? (environment-ref env name) traced)
	      (environment-set! env name proc)
	      (let ((out (command-output)))
		(display "Value of " out)
		(write name out)
		(display " changed since ,trace; not restoring it." out)
		(newline out)))
	  (set-traced-procedures!
		      (filter (lambda (x)
				(not (eq? (car x) name)))
			      (traced-procedures))))
	(write-line "?" (command-output)))))

(define (make-traced proc name)
  (lambda args
    (apply-traced proc name args)))

(define (apply-traced proc name args)
  (let ((port (command-output)))
    (dynamic-wind
     (lambda ()
       (display "[" port))
     (lambda ()
       (let ((depth (trace-writing-depth)))
	 (with-limited-output
	  (lambda ()
	    (display "Enter " port)
	    (write-carefully (error-form name args) port)
	    (newline port))
	  depth depth)
	 (call-with-values (lambda ()
			     (apply proc args))
	   (lambda results
	     (with-limited-output
	      (lambda ()
		(display " Leave " port)
		(write-carefully name port)
		(for-each (lambda (result)
			    (display " " port)
			    (write-carefully (value->expression result) port))
			  results))
	      depth
	      (- depth 1))
	     (apply values results)))))
     (lambda ()
       (display "]" port)
       (newline port)))))

; Timer stuff.

(define (time command)
  (let ((thunk (if (eq? (car command) 'run)
		   (eval `(lambda () ,(cadr command))
			 (environment-for-commands))
		   (lambda () (execute-command command))))
	(port (command-output)))
    (let ((start-run-time (run-time))
	  (start-real-time (real-time)))
      (call-with-values thunk
	(lambda results
	  (let ((stop-run-time (run-time))
		(stop-real-time (real-time)))
	    (display "Run time: " port)
	    (write-hundredths (- stop-run-time start-run-time) port)
	    (display " seconds; Elapsed time: " port)
	    (write-hundredths (- stop-real-time start-real-time) port)
	    (display " seconds" port)
	    (newline port)
	    (set-command-results! results)))))))

; N is in milliseconds

(define (write-hundredths n port)
  (let ((n (round (quotient n 10))))
    (write (quotient n 100) port)
    (write-char #\. port)
    (let ((r (remainder n 100)))
      (if (< r 10)
	  (write-char #\0 port))
      (write r port))))

; Copied from rts/time.scm to avoid a dependency.

(define (real-time)
  (primitives:time (enum time-option real-time) #f))

(define (run-time)
  (primitives:time (enum time-option run-time) #f))

(define-command-syntax 'time "<command>" "measure execution time"
  '(command))

; Support for stuffing things from Emacs.

(define-command-syntax 'from-file #f #f	;"<filename>" "editor support"
  '(&opt filename))

(define-command-syntax 'end #f #f
  '())

(define (from-file . maybe-filename)
  (let* ((filename (if (null? maybe-filename) #f (car maybe-filename)))
	 (env (let ((probe (if filename
			       (get-file-environment filename)
			       #f))
		    (c (environment-for-commands)))
		(if (and probe (not (eq? probe c)))
		    (let ((port (command-output)))
		      (newline port)
		      (display filename port)
		      (display " => " port)
		      (write probe port)
		      (display " " port) ;dots follow
		      probe)
		    c)))
	 (in (command-input))
	 (forms (let recur ()
		  (let ((command (read-command #f #t in)))
		    (if (eof-object? command)
			'()
			(case (car command)
			  ((end) '())
			  ((#f run) (cons (cadr command) (recur)))
			  (else
			   (assertion-violation 'from-file
						"unusual command in ,from-file ... ,end"
						command))))))))
    (if (package? env)
	(with-interaction-environment env
	  (lambda ()
	    (noting-undefined-variables env
	      (lambda ()
		(eval-from-file forms env (if (null? maybe-filename)
					      #f
					      (car maybe-filename)))))
	    (newline (command-output))))
	(for-each (lambda (form) (eval form env)) ;Foo
		  env))))

; Filename -> environment map.

(define file-environments
  (user-context-accessor 'file-environments (lambda () '())))

(define set-file-environments!
  (user-context-modifier 'file-environments))

(define (forget-file-environments)
  (set-file-environments! '()))

(define (note-file-environment! filename env)
  (if (maybe-user-context)
      (let* ((translated (filenames:translate filename))
	     (envs (file-environments))
	     (probe (or (assoc filename envs) ;What to do?
			(assoc translated envs))))
	(if probe
	    (if (not (eq? env (weak-pointer-ref (cdr probe))))
		(let ((port (command-output)))
		  (newline port)
		  (display "Changing default package for file " port)
		  (display filename port)
		  (display " from" port)
		  (newline port)
		  (write (weak-pointer-ref (cdr probe)) port)
		  (display " to " port)
		  (write env port)
		  (newline port)
		  (set-cdr! probe (make-weak-pointer env))))
	    (set-file-environments!
	     (cons (cons filename (make-weak-pointer env))
		   envs))))))

; Temporary hack until we get default values for unhandled upcalls.

; This gets called during the building of, say scheme48.image, while
; there's still the REALLY-SIGNAL-CONDITION from EXCEPTIONS
; installed---so we make sure we get the right ones.

(define (maybe-user-context)
  (call-with-current-continuation
    (lambda (exit)
      (with-handler (lambda (condition punt)
		      (if (serious-condition? condition)
			  (exit #f)
			  (punt)))
		    user-context))))

(define (get-file-environment filename)
  (let ((probe (assoc filename (file-environments)))) ;translate ?
    (if probe
	(weak-pointer-ref (cdr probe))
	#f)))

(fluid-cell-set! $note-file-package note-file-environment!)

(define-command-syntax 'forget "<filename>"
  "forget file/package association"
  '(filename))

(define (forget filename)
  (note-file-environment! filename #f))

; ,bound? <name>

(define-command-syntax 'bound? "<name>"
  "display binding of name, if any"
  '(name))

(define (bound? name)
  (let ((port (command-output))
	(probe (package-lookup (environment-for-commands) name)))
    (if probe
	(begin (display "Bound to " port)
	       (cond ((binding? probe)
                      (describe-binding probe port))
                     (else
                      (write probe port)
                      (newline port)))
               (set-focus-object! probe))
	(write-line "Not bound" port))))

(define (describe-binding binding port)
  (let ((type (binding-type binding))
	(location (binding-place binding))
	(static (binding-static binding)))
    (display (binding-type-description binding) port)
    (write-char #\space port)
    (write location port)
    (newline port)
    (display "  Type " port)
    (write (type->sexp type #t) port)
    (newline port)
    (cond (static (display "  Static " port)
                  (write static port)
                  (newline port)))))

(define (binding-type-description binding)
  (let ((type (binding-type binding))
        (static (binding-static binding)))
    (cond ((variable-type? type) "mutable variable")
          ((eq? type undeclared-type) "unknown denotation")
          ((subtype? type syntax-type)
           (if (transform? static) "macro" "special operator"))
          ((primop? static) "primitive procedure")
          ((transform? static) "integrated procedure")
          (else "variable"))))

; ,expand <form>

(define-command-syntax 'expand "[<form>]"
  "macro-expand a form"
  '(&opt expression))

; Doesn't work - the current syntax interface doesn't have anything that only
; expands once.
;(define-command-syntax 'expand-once "[<form>]"
;  "macro-expand a form once"
;  '(&opt expression))

(define (expand . maybe-exp)
  (do-expand maybe-exp syntactic:expand-form))

(define (expand-once . maybe-exp)
  (do-expand maybe-exp syntactic:expand))

(define (do-expand maybe-exp expander)
  (let ((exp (if (null? maybe-exp)
		 (focus-object)
		 (car maybe-exp)))
	(env (package->environment (environment-for-commands))))
    (set-command-results!
     (list (schemify (expander exp env)
		     env)))))




