; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Commands for debugging.

; translate

(define-command 'translate "<from> <to>" "establish file name translation"
  '(filename filename)
  set-translation!)


; preview  -- show continuations

(define (preview)
  (display-preview ;; (continuation-preview (command-continuation))
		   (command-continuation)
		   (command-output)))

(define (display-preview c port)
  ;; (for-each (lambda (id+pc) ...) preview)
  (if (continuation? c)
      (begin
	(if (not (fluid-let-continuation? c))
	    (display-template-names (template-names (continuation-template c))
				    port))
	(display-preview (continuation-parent c) port))))

(define (display-template-names names port)
  (display "  " port)
  (if (null? names)
      (display "unnamed" port)
      (let loop ((names names))
	(if (car names)
	    (write (car names) port)
	    (display "unnamed" port))
	(if (and (not (null? (cdr names)))
		 (cadr names))
	    (begin (display " in " port)
		   (loop (cdr names))))))
  (newline port))

(define fluid-let-continuation?
  (let ((id (let-fluid (make-fluid #f) #f
	      (lambda ()
		(primitive-catch (lambda (k)
				   (template-id
				    (continuation-template k))))))))
    (lambda (c)
      (and (continuation? c)
	   (eq? (template-id (continuation-template c)) id)))))


(define-command 'preview "" "show continuations (stack trace)"
  '() preview)

; Proceed

(define (proceed vals)
  (let* ((level (command-level))
	 (condition (command-level-condition level)))
    (if (ok-to-proceed? condition)
	(throw-to-command-level
	     level
	     (lambda ()
	       (let ((interrupts (enabled-interrupts))
		     (new-interrupts (command-level-interrupts level)))
		 (if (not (= new-interrupts interrupts))
		     (begin (if (not (and (interrupt? condition)
					  (= (caddr condition) interrupts)))
				(write-line "(Disabling interrupts)"
					    (command-output)))
			    (set-enabled-interrupts! new-interrupts))))
	       (apply values vals)))
	(write-line "No way to proceed from here." (command-output)))))

(define-command 'proceed "<exp>" "proceed after interrupt or error"
  '(&rest expression)
  (lambda exps
    (proceed (map (lambda (exp)
		    (evaluate exp (environment-for-commands)))
		  exps))))

; Scrutinize the condition to ensure that it's safe to return from the
; call to RAISE.

(define (ok-to-proceed? condition)
  (and condition
       (if (error? condition)
	   (and (exception? condition)
		(let ((opcode (exception-opcode condition)))
		  (or (= opcode op/global)
		      (= opcode op/local0)
		      (= opcode op/set-global!)
		      (>= opcode op/eq?))))
	   #t)))

(define op/global (enum op global))
(define op/local0 (enum op local0))
(define op/set-global! (enum op set-global!))
(define op/eq? (enum op eq?))


(define (breakpoint . rest)
  (command-loop unspecific
		(make-condition 'breakpoint rest)
		(interaction-environment)))

(define-condition-type 'breakpoint '())
(define breakpoint? (condition-predicate 'breakpoint))


; push

(define-command 'push "" "" '()
  (lambda ()
    (command-loop list
		  (if (command-level? (focus-object))
		      (command-level-condition (focus-object))
		      #f)
		  (interaction-environment))))


; reset

(define (reset)
  (abort-to-command-level (top-command-level)))

(define-command 'reset "" "top level"
  '() reset)


(define (go-to-level n)
  (let ((levels (reverse (fluid $command-levels))))
    (if (and (integer? n)
	     (>= n 0)
	     (< n (length levels)))
	(abort-to-command-level (list-ref levels n))
	(write-line "invalid command level" (command-output)))))

(define-command 'level "<number>" "go to command level"
  '(expression) go-to-level)



(define-command 'condition "" "select condition object"
  '() (lambda ()
	(let ((c (command-level-condition (command-level))))
	  (if c
	      (set-focus-object! c)
	      (write-line "no condition" (command-output))))))


; Commands that toggle various flags.

(define (toggle-command get set description)
  (lambda (option)
    (let ((b (case option
	       ((#f) (not (get)))
	       ((off) #f)
	       ((on) #t)
	       ((?) (get))
	       (else (error "invalid setting (should be on or off)"
			    option))))
	  (out (command-output)))
      (set b)
      (display (if b "Will " "Won't ") out)
      (display description out)
      (newline out))))
(define syntax-for-toggle '(&opt name))


; Batch mode

(define-command 'batch "" "toggle batch mode (no prompt, errors exit)"
  syntax-for-toggle
  (toggle-command batch-mode?
		  set-batch-mode?!
		  "exit on errors"))


; Benchmark mode (i.e., inline primitives)

(define-command 'bench "" "benchmark mode (integrate primitives)"
  syntax-for-toggle
  (toggle-command (lambda ()
		    (package-integrate? (environment-for-commands)))
		  (lambda (b)
		    (set-package-integrate?! (environment-for-commands) b))
		  "compile some calls in line"))

; Break on warnings

(define-command 'break-on-warnings "" "treat warnings as errors"
  syntax-for-toggle
  (toggle-command break-on-warnings?
		  set-break-on-warnings?!
		  "enter breakpoint on warnings"))


(define-command 'form-preferred "" "enable/disable form-preferred mode"
  syntax-for-toggle
  (toggle-command (user-context-accessor 'form-preferred? (lambda () #t))
		  (user-context-modifier 'form-preferred?)
		  "prefer forms to commands"))

(define-command 'levels "" "disable/enable command levels"
  syntax-for-toggle
  (toggle-command (user-context-accessor 'push-command-levels (lambda () #t))
		  (user-context-modifier 'push-command-levels)
		  "push command level on errors"))


; Flush debug data base

(define-command 'flush "<kind>" "start forgetting debug information"
  '(&rest name)
  (lambda kinds
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
			     (else
			      (write-line "Unrecognized debug flag"
					  (command-output)))))
		     kinds)))))

; Control retention of debugging information

(define-command 'keep "<kind>" "start remembering debug information"
  '(&rest name)
  (lambda kinds
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
		    kinds)))))


; Collect some garbage

(define (collect-command)
  (let ((port (command-output))
	(before (memory-status memory-status-option/available #f)))
    (collect)
    (let ((after (memory-status memory-status-option/available #f)))
      (display "Before: " port)
      (write before port)
      (newline)
      (display "After:  " port)
      (write after port)
      (newline))))

(define memory-status-option/available (enum memory-status-option available))

(define-command 'collect "" "GC" '() collect-command)


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
;(define-command 'undefined "" "list undefined variables"
;  '() show-undefined-variables)



; Trace and untrace

(define (trace . names)
  (if (null? names)
      (let ((port (command-output)))
	(write (map car (fluid $traced)) port)
	(newline port))
      (for-each trace-1 names)))

(define-command 'trace "<name> ..." "trace calls to given procedure(s)"
  '(&rest name) trace)

(define (untrace . names)
  (if (null? names)
      (for-each untrace-1 (map car (fluid $traced)))
      (for-each untrace-1 names)))

(define-command 'untrace "<name> ..." "stop tracing calls"
  '(&rest name) untrace)

; Trace internals

(define $traced       (make-fluid '()))

(define (trace-1 name)
  (let* ((env (environment-for-commands))
	 (proc (environment-ref env name))
	 (traced (make-traced proc name)))
    (set-fluid! $traced
		(cons (list name traced proc env)
		      (fluid $traced)))
    (environment-define! env name traced))) ;was environment-set!
	   
; Should be doing clookup's here -- avoid creating new locations

(define (untrace-1 name)
  (let ((probe (assq name (fluid $traced))))
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
	  (set-fluid! $traced
		      (filter (lambda (x)
				(not (eq? (car x) name)))
			      (fluid $traced))))
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

(define (time-command form)
  (let* ((thunk (evaluate `(lambda () ,form) (environment-for-commands)))
	 (start-time (time time-option/run-time #f)))
    (call-with-values thunk
      (lambda results
	(let* ((stop-time (time time-option/run-time #f))
	       (dt (- stop-time start-time))
	       (units-per-second (time time-option/ticks-per-second #f))
	       (delta (quotient (* dt 100) units-per-second))
	       (port (command-output)))
	  (display "Run time: " port)
	  (write-hundredths delta port)
	  (display " seconds" port)
	  (newline port)
	  (set-focus-values! results))))))

(define time-option/run-time (enum time-option run-time))
(define time-option/ticks-per-second (enum time-option ticks-per-second))

(define (write-hundredths n port)
  (write (quotient n 100) port)
  (write-char #\. port)
  (let ((r (remainder n 100)))
    (if (< r 10)
	(write-char #\0 port))
    (write r port)))

(define-command 'time "<exp>" "measure execution time"
  '(expression) time-command)

; Support for stuffing things from Emacs.

(define-command 'from-file #f #f	;"<filename>" "editor support"
  '(&opt filename)
  (lambda (filename)
    (let ((env (let ((probe (get-file-environment filename))
		     (c (environment-for-commands)))
		 (if (and probe (not (eq? probe c)))
		     (let ((port (command-output)))
		       (newline port)
		       (display "In " port)
		       (write probe port)
		       (display " " port) ;dots follow
		       probe)
		     c)))
	  (in (command-input)))
      (let ((forms (let recur ()
		     (let ((command (read-command #f #t in)))
		       (if (eof-object? command)
			   '()
			   (case (car command)
			     ((end) '())
			     ((evaluate) (cons (cadr command) (recur)))
			     (else
			      (error "unusual command in ,from-file ... ,end"
				     command))))))))
	(if (package? env)
	    (noting-undefined-variables env
	      (lambda ()
		(eval-from-file forms env filename)))
	    (for-each (lambda (form) (eval form env)) ;Foo
		      env))))))


; Filename -> environment map.
; These should probably be weak pointers.  Fix later.

(define *file-environments* '())

(define (note-file-environment! filename env)
  (let* ((translated (translate filename))
	 (probe (or (assoc filename *file-environments*)    ;What to do?
		    (assoc translated *file-environments*))))
    (if probe
	(if (not (eq? env (cdr probe)))
	    (let ((port (command-output)))
	      (newline port)
	      (display "Changing default package for file " port)
	      (display filename port)
	      (display " from" port)
	      (newline port)
	      (write (cdr probe) port)
	      (display " to " port)
	      (write env port)
	      (newline port)
	      (set-cdr! probe env)))
	(set! *file-environments*
	      (cons (cons filename env) *file-environments*)))))

(define (get-file-environment filename)
  (let ((probe (assoc filename *file-environments*))) ;translate ?
    (if probe
	(cdr probe)
	#f)))

(set-fluid! $note-file-package note-file-environment!)

(define-command 'forget "<filename>"
  "forget that file goes in package"
  '(filename)
  (lambda (filename)
    (note-file-environment! filename #f)))


; Losing ,take command.  (See file TODO.)

(define-command 'take "<filename>" "read commands from file"
  '(filename)
  (lambda (filename)
    (call-with-input-file (translate filename)
      (lambda (port)
	(let loop ()
	  (let ((command (read-command #f ;no prompt
				       #f ;command preferred
				       port)))
	    (cond ((eof-object? command))
		  (else
		   (execute-command command)
		   (loop)))))))))

; ,bound? <name>

(define-command 'bound? "<name>"
  "display binding of name, if any"
  '(name)
  (lambda (name)
    (let ((port (command-output))
          (probe (package-lookup (environment-for-commands) name)))
      (if probe
          (begin (display "Bound to " port)
		 (if (operator? probe)
		     (begin (write (operator-type probe))
			    (display #\space)))
                 (write probe)
                 (newline port))
          (write-line "Not bound" port)))))

; ,expand <form>

(define-command 'expand "[<form>]"
  "macro-expand a form"
  '(&opt expression)
  (lambda (exp)
    (let ((exp (or exp (focus-object)))
	  (env (environment-for-commands)))
      (set-focus-object!
           (schemify (classify exp (package->environment env)))))))
