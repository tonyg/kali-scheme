; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

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

(define (start-command-processor resume-args start-thunk)
  (restart-command-processor resume-args #f start-thunk))

(define (restart-command-processor resume-args context start-thunk)
  ((with-handler command-loop-condition-handler
     (lambda ()
       (start-command-levels resume-args
			     context
			     start-thunk
			     real-command-loop
			     #f)))))	; no condition

; Entry for initialization & testing.

(define (command-processor command-env resume-args)
  (start-command-processor resume-args
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
  (push-command-level real-command-loop condition))

(define command-level-condition command-level-repl-data)

; Install the handler, bind $NOTE-UNDEFINED to keep from annoying the user,
; display the condition and start reading commands.
;
; This has SHOWING-FOCUS-OBJECT inlined by hand to reduce the amount of noise
; the debugger sees on the stack.

(define (real-command-loop)
  (let-fluids $note-undefined #f    ;useful
      (lambda ()
	(display-command-level-condition (command-level-condition (command-level)))
	(let loop ()
	  (let ((command (read-command-carefully (command-prompt)
						 (form-preferred?)
						 (command-input)))
		(focus-before (focus-values)))
	    (execute-command command)
	    (let ((focus-after (focus-values)))
	      (if (not (eq? focus-after focus-before))
		  (show-command-results focus-after)))
	    (loop))))))

(define (display-command-level-condition condition)
  (if condition
      (display-condition condition (command-output))))

; If #T anything that doesn't start with the command prefix (a comma) is
; treated as an argument to RUN.  If #F no commas are needed and RUN
; commands must be explicit.

(define form-preferred?
  (user-context-accessor 'form-preferred? (lambda () #t)))

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
; errors and interrupts we stop the level or exit.

(define (command-loop-condition-handler c next-handler)
  (cond ((or (warning? c)
	     (note? c))
         (if (break-on-warnings?)
             (deal-with-condition c)
             (begin (force-output (current-output-port)) ; keep synchronous
	            (display-condition c (current-error-port))
                    (unspecific))))     ;proceed
        ((or (error? c) (interrupt? c))
         (if (batch-mode?)
             (begin (force-output (current-output-port)) ; keep synchronous
	            (display-condition c (current-error-port))
                    (let ((status (if (error? c) 1 2)))
                      (scheme-exit-now status)))
             (deal-with-condition c)))
	((reset-command-input? c)
	 (unspecific))			;proceed
        (else                           
         (next-handler))))

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

(define-condition-type 'note '())
(define note? (condition-predicate 'note))

; The prompt is "level-number environment-id-string> " or just
; "environment-id-string> " at top level.  The id-string is empty for the
; current user package and the name of the package otherwise.

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
		   "> ")))

(define-generic environment-id-string &environment-id-string (env))

(define-method &environment-id-string (env) "")

;----------------
; Evaluate a form and save its result as the current focus values.

(define (evaluate-and-select form env)
  (call-with-values (lambda ()
                      (eval form env))
    (lambda results
      (if (or (null? results)
              (not (null? (cdr results)))
              (not (eq? (car results) (unspecific))))
          (set-focus-values! results))
      (apply values results))))

;----------------
; Display the focus object if it changes (sort of like emacs's redisplay)

(define (showing-focus-object thunk)
  (let ((focus-before (focus-values)))
    (thunk)
    (let ((focus-after (focus-values)))
      (if (not (eq? focus-after focus-before))
          (show-command-results focus-after)))))

(define (focus-object)
  (let ((v (focus-values)))
    (if (and (pair? v) (null? (cdr v))) (car v) v)))

(define (set-focus-object! obj)
  (set-focus-values! (list obj)))

(define (show-command-results results)
  (cond ((null? results))
        ((not (null? (cdr results)))
         (let ((out (command-output)))
           (display "; " out)
           (write (length results) out)
           (display " values" out)
           (newline out))
         (for-each show-command-result results))
        (else ;(not (eq? (car results) (unspecific)))
         (show-command-result (car results)))))

(define (show-command-result result)
  (write-carefully (value->expression result)
                   (command-output))
  (newline (command-output)))

(define $write-depth (make-fluid -1))
(define $write-length (make-fluid -1))

(define (write-carefully x port)
  (if (error? (ignore-errors (lambda ()
                               (limited-write x port
                                              (fluid $write-depth)
                                              (fluid $write-length))
                               #f)))
      (display "<Error while printing.>" port)))

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
"This is a beta-test version of Scheme 48.  You are interacting with"
"the command processor.  A command is either a Scheme form to evaluate"
"or one of the following:"
""))

    (list-command-help (user-command-help) f? o-port)
    (for-each (lambda (s)
                (write-line s o-port))
              '(
""
"Square brackets [...] indicate optional arguments."
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

(define (value->expression obj)         ;mumble
  (if (or (number? obj) (char? obj) (string? obj) (boolean? obj))
      obj
      `',obj))

(define (write-spaces count o-port)
  (do ((count count (- count 1)))
      ((<= count 0))
    (display #\space o-port)))

(define (command-prefix-string prefix)
  (cond ((string? prefix) prefix)
        ((char? prefix) (string prefix))
        ((symbol? prefix) (symbol->string prefix))))

(define (write-line string port)
  (display string port)
  (newline port))

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
    (display "." port)
    (newline port)
    (write-line "Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees."
		port)
    (write-line "Please report bugs to scheme-48-bugs@martigny.ai.mit.edu."
                port)
    (if (not (batch-mode?))
	(write-line "Type ,? (comma question-mark) for help." port))))

; The following is used by the debugger to get an appropriate continuation
; or list of threads to show the user.

(define thread? (structure-ref threads thread?))
(define thread-continuation (structure-ref threads-internal thread-continuation))

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
	  (((structure-ref continuations continuation?) obj)
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
