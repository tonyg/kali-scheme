; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


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
				   (template-id
				    (continuation-template k))))))))
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
       (if (error? condition)
	   (and (exception? condition)
		(let ((opcode (exception-opcode condition)))
		  (or (= opcode (enum op global))
		      (= opcode (enum op local))
		      (= opcode (enum op local0))
		      (= opcode (enum op local1))
		      (= opcode (enum op local2))
		      (= opcode (enum op set-global!))
		      (>= opcode (enum op eq?)))))
	   #t)))

(define (breakpoint . rest)
  (command-loop (make-condition 'breakpoint rest)))

(define-condition-type 'breakpoint '())
(define breakpoint? (condition-predicate 'breakpoint))

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

(define (reset)
  (abort-to-command-level (top-command-level)))

(define-command-syntax 'reset "" "top level" '())

(define (go-to-level n)
  (let ((levels (reverse (command-levels))))
    (if (and (integer? n)
	     (>= n 0)
	     (< n (length levels)))
	(abort-to-command-level (list-ref levels n))
	(write-line "invalid command level" (command-output)))))

(define-command-syntax 'level "<number>" "go to specific command level"
  '(expression))

(define level go-to-level)

(define-command-syntax 'condition ""
  "select an object that describes the current error condition"
  '())

(define (condition)
  (let ((c (command-level-condition (command-level))))
    (if c
	(set-focus-object! c)
	(write-line "no condition" (command-output)))))

; Commands that toggle various flags.

(define (toggle-command get set description)
  (lambda maybe-option
    (let ((b (if (null? maybe-option)
		 (not (get))
		 (case (car maybe-option)
		   ((off) #f)
		   ((on) #t)
		   ((?) (get))
		   (else (error "invalid setting (should be on or off or ?)"
				(car maybe-option))))))
	  (out (command-output)))
      (set b)
      (display (if b "Will " "Won't ") out)
      (display description out)
      (newline out))))

(define syntax-for-toggle '(&opt name))

; Batch mode

(define-command-syntax 'batch "[on | off]"
  "enable/disable batch mode (no prompt, errors exit)"
  syntax-for-toggle)

(define batch
  (toggle-command batch-mode?
		  set-batch-mode?!
		  "exit on errors"))

; Benchmark mode (i.e., inline primitives)

(define-command-syntax 'bench "[on | off]"
  "enable/disable benchmark mode (integrate primitives)"
  syntax-for-toggle)

(define bench
  (toggle-command (lambda ()
		    (package-integrate? (environment-for-commands)))
		  (lambda (b)
		    (set-package-integrate?! (environment-for-commands) b))
		  "compile some calls in line"))

; Break on warnings

(define-command-syntax 'break-on-warnings "[on | off]"
  "treat warnings as errors"
  syntax-for-toggle)

(define break-on-warnings
  (toggle-command break-on-warnings?
		  set-break-on-warnings?!
		  "enter breakpoint on warnings"))


(define-command-syntax 'form-preferred "[on | off]"
  "enable/disable form-preferred command processor mode"
  syntax-for-toggle)

(define form-preferred
  (toggle-command (user-context-accessor 'form-preferred? (lambda () #t))
		  (user-context-modifier 'form-preferred?)
		  "prefer forms to commands"))

(define-command-syntax 'levels "[on | off]"
  "disable/enable command levels"
  syntax-for-toggle)

(define levels
  (toggle-command (user-context-accessor 'push-command-levels (lambda () #t))
		  (user-context-modifier 'push-command-levels)
		  "push command level on errors"))


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
	(before (memory-status memory-status-option/available #f)))
    ((structure-ref primitives collect))
    (let ((after (memory-status memory-status-option/available #f)))
      (display "Before: " port)
      (write before port)
      (display " words free in semispace")
      (newline)
      (display "After:  " port)
      (write after port)
      (display " words free in semispace")
      (newline))))

(define memory-status-option/available (enum memory-status-option available))

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

(define *trace-depth* 8)
(define (apply-traced proc name args)
  (let ((port (command-output)))
    (dynamic-wind
     (lambda ()
       (display "[" port))
     (lambda ()
       (let-fluids $write-length *trace-depth*
	 $write-depth *trace-depth*
	 (lambda ()
	   (display "Enter " port)
	   (write-carefully (error-form name args) port)
	   (newline port)))
       (call-with-values (lambda ()
			   (apply proc args))
	 (lambda results
	   (let-fluids $write-length *trace-depth*
	     $write-depth (- *trace-depth* 1)
	     (lambda ()
	       (display " Leave " port)
	       (write-carefully name port)
	       (for-each (lambda (result)
			   (display " " port)
			   (write-carefully (value->expression result) port))
			 results)))
	   (apply values results))))
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
	    (set-focus-values! results)))))))

; N is in milliseconds

(define (write-hundredths n port)
  (let ((n (round (quotient n 10))))
    (write (quotient n 100) port)
    (write-char #\. port)
    (let ((r (remainder n 100)))
      (if (< r 10)
	  (write-char #\0 port))
      (write r port))))

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
			   (error "unusual command in ,from-file ... ,end"
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
  (if (user-context)
      (let* ((translated ((structure-ref filenames translate) filename))
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

(define (get-file-environment filename)
  (let ((probe (assoc filename (file-environments)))) ;translate ?
    (if probe
	(weak-pointer-ref (cdr probe))
	#f)))

(set-fluid! $note-file-package note-file-environment!)

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
	       (write probe)
	       (newline port))
	(write-line "Not bound" port))))

; ,expand <form>

(define-command-syntax 'expand "[<form>]"
  "macro-expand a form"
  '(&opt expression))

(define (expand . maybe-exp)
  (let ((exp (if (null? maybe-exp)
		 (focus-object)
		 (car maybe-exp)))
	(env (package->environment (environment-for-commands))))
    (set-focus-object!
     (schemify ((structure-ref syntactic expand) exp env) env))))
