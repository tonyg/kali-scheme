; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; A dirty little inspector.
; This breaks abstractions left and right.
; Look and feel shamelessly plagiarized from the Lucid Lisp inspector.

; Inspector state:
;    thing    ; object currently being inspected, obtained as (focus-object)
;    menu     ; cached result of (prepare-menu thing).  This is a list of
;               lists (<name-or-#f> <value>).
;    position ; position within menu; modified by M (more) command
;    stack    ; list of other things

(define-record-type inspector-state inspector-state?
  (make-inspector-state menu position stack)
  (menu inspector-state-menu)
  (position inspector-state-position set-inspector-state-position!)
  (stack inspector-state-stack))

; The inspector is a distinct REPL with its own state.  This allows the
; user to continue with the same inspection stack after an error.

(define inspector-state repl-data)
(define set-inspector-state! set-repl-data!)

(define *menu-limit* 15)	; maximum menu entries
(define *write-depth* 3)	; limit for recursive writes
(define *write-length* 5)       ; ditto

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
  (showing-focus-object
   (lambda ()
     (set-focus-object! (or (command-continuation)
			    (command-threads)))))
  (inspect))

(define (threads)
  (showing-focus-object
   (lambda ()
     (set-focus-object! (command-threads))))
  (inspect))

(define (inspect . maybe-exp)
  (if (not (null? maybe-exp))
      (with-limited-output
       (lambda ()
	 (showing-focus-object
	  (lambda ()
	    (evaluate-and-select (car maybe-exp)
				 (environment-for-commands)))))))
  (push-command-level inspector
		      (make-inspector-state (prepare-menu (focus-object))
					    0
					    '())))

;----------------
; Actual entry point for the inspector.  We print the menu and then loop
; reading commands.

(define (inspector)
  (present-menu)
  (let loop ()
    (let ((command (read-command-carefully "inspect: "
					   #f  ; command preferred
					   (command-input)
					   inspector-commands)))
      (cond ((eof-object? command)
	     (newline (command-output))
	     (proceed-with-command-level (cadr (command-levels))))
	    ((not command)   ; read command error
	     (loop))
	    (else
	     (with-limited-output
	      (lambda ()
		(execute-inspector-command command)))
	     (loop))))))

(define (present-menu)
  (let ((state (inspector-state)))
    (display-menu (inspector-state-menu state)
		  (inspector-state-position state)
		  (command-output))))

; Go to a new thing by making a new inspector state.

(define (new-selection thing stack)
  (set-inspector-state!
     (make-inspector-state (prepare-menu thing) 0 stack)))

; Selection commands are either an integer, which selects a menu item,
; or `u' to move up the stack, `d' to move to the next continuation
; (only valid when the current object is a continuation), or `t' which
; moves to a procedure's template (only valid when the current object
; is a template).

(define (read-selection-command port)
  (let ((x (read port)))
    (if (or (integer? x)
	    (memq x '(u d t)))
	x
	(read-command-error port "invalid selection command" x))))

(define selection-command-syntax (list '&rest read-selection-command))

(define (inspector-commands name)
  (if (integer? name)
      selection-command-syntax
      (case name
	((? m q) '())               ; no arguments
	((u d t) selection-command-syntax)
	(else #f))))

; Execute a command.
;
; We save the current object and state to compare to the new ones to see
; if we need to display a new menu.  The old object is pushed on the stack
; only if nothing has been popped off.

(define (execute-inspector-command command)
  (let ((result-before (focus-object))
	(state-before (inspector-state)))
    (showing-focus-object
     (lambda ()
       (let ((name (car command)))
	 (if (integer? name)
	     (execute-selection-command command)
	     (case name
	       ((u d t)
		(execute-selection-command command))
	       ((m) (inspect-more))
	       ((q) (proceed-with-command-level (cadr (command-levels))))
	       ((?) (inspect-help))
	       (else (execute-command command)))))))
    (let ((result-after (focus-object))
	  (state-after (inspector-state)))
      (if (not (eq? result-after result-before))
	  (begin (if (eq? state-after state-before)
		     (new-selection result-after
				    (cons result-before
					  (inspector-state-stack state-before))))
		 (present-menu))))))

; Choose a new object.

(define (execute-selection-command command)
  (if (not (null? command))
      (let ((name (car command)))
	(if (integer? name)
	    (let ((menu (inspector-state-menu (inspector-state))))
	      (if (and (>= name 0)
		       (< name (length menu)))
		  (move-to-object! (menu-ref menu name))
		  (write-line "Invalid choice." (command-output))))
	    (case name
	      ((u) (pop-inspector-stack))
	      ((d) (inspect-next-continuation))
	      ((t) (select-template))
	      (else (error "bad selection command" name))))
	(execute-selection-command (cdr command)))))

; Procedures for the various commands.

(define (move-to-object! object)
  (new-selection object
		 (cons (focus-object)
		       (inspector-state-stack (inspector-state))))
  (set-focus-object! object))

(define (pop-inspector-stack)
  (let ((stack (inspector-state-stack (inspector-state))))
    (if (pair? stack)
	(begin (new-selection (car stack) (cdr stack))
	       (set-focus-object! (car stack)))
	(write-line "Can't go up from here." (command-output)))))

(define (inspect-next-continuation)
  (if (continuation? (focus-object))
      (move-to-object! (continuation-parent (focus-object)))
      (write-line "Can't go down from a non-continuation." (command-output))))

(define (inspect-more)
  (let* ((state (inspector-state))
	 (menu (inspector-state-menu state))
	 (position (inspector-state-position state)))
    (if (> (length menu) (+ *menu-limit* position))
	(let ((position (- (+ position *menu-limit*) 1)))
	  (set-inspector-state-position! state position)
	  (present-menu))
	(write-line "There is no more." (command-output)))))

(define (select-template)
  (move-to-object! (coerce-to-template (focus-object))))

(define (inspect-help)
  (let ((o-port (command-output)))
    (for-each (lambda (s) (display s o-port) (newline o-port))
	      '("q          quit"
		"u          up stack (= go to previous object)"
		"d          down stack"
		"t          template"
		"<integer>  menu item"
		"or any command processor command"
		"multiple u d t <integer> commands can be put on one line"))))

;----------------
; Menus.
;
; A menu is a list of lists (<name-or-#f> <thing>).

(define (menu-ref menu n)
  (cadr (list-ref menu n)))

; Get a menu for THING.  We know about a fixed set of types.

(define (prepare-menu thing)
  (cond ((list? thing)
         (map (lambda (x)
		(list #f x))
              thing))

        ((pair? thing)
         `((car ,(car thing)) (cdr ,(cdr thing))))

        ((vector? thing)
         (prepare-menu (vector->list thing)))

        ((closure? thing)
         (prepare-environment-menu
              (closure-env thing)
              (get-shape (template-debug-data (closure-template thing))
                         0)))

	((template? thing)
	 (prepare-menu (template->list thing)))

        ((continuation? thing)
         (prepare-continuation-menu thing))

        ((record? thing)
         (prepare-record-menu thing))

        ((location? thing)
         `((id ,(location-id thing))
           (contents ,(contents thing))))

	((weak-pointer? thing)
	 `((ref ,(weak-pointer-ref thing))))

        (else '())))

(define (template->list template)
  (do ((i (- (template-length template) 1) (- i 1))
       (r '() (cons (template-ref template i) r)))
      ((< i 0) r)))

; Continuation menus have the both the saved operand stack and the
; save environment, for which names may be available.

(define (prepare-continuation-menu thing)
  (let ((next (continuation-parent thing)))
    `(,@(let recur ((c thing))
	  (if (eq? c next)
	      '()
	      (let ((z (continuation-arg-count c)))
		(do ((i (- z 1) (- i 1))
		     (l (recur (continuation-cont c))
			(cons (list #f (continuation-arg c i))
			      l)))
		    ((< i 0) l)))))
      ,@(prepare-environment-menu (continuation-env thing)
				  (get-shape (continuation-debug-data thing)
					     (continuation-pc thing))))))

(define (continuation-debug-data thing)
  (template-debug-data (continuation-template thing)))

; Records that have record types get printed with the names of the fields.

(define (prepare-record-menu thing)
  (let ((rt (record-type thing))
        (z (record-length thing)))
    (if (record-type? rt)
        (do ((i (- z 1) (- i 1))
             (f (reverse (record-type-field-names rt)) (cdr f))
             (l '() (cons (list (car f) (record-ref thing i)) l)))
            ((< i 1) l))
        (do ((i (- z 1) (- i 1))
             (l '() (cons (list #f (record-ref thing i)) l)))
            ((< i 0) l)))))

; We may have the names (`shape') for environments, in which case they
; are used in the menus.

(define (prepare-environment-menu env shape)
  (if (vector? env)
      (let ((values (rib-values env)))
        (if (pair? shape)
            (append (map list (car shape) values)
                    (prepare-environment-menu (vector-ref env 0)
                                              (cdr shape)))
            (append (map (lambda (x)
			   (list #f x))
			 values)
                    (prepare-environment-menu (vector-ref env 0)
					      shape))))
      '()))

(define (rib-values env)
  (let ((z (vector-length env)))
    (do ((i 1 (+ i 1))
	 (l '() (cons (if (vector-unassigned? env i)
			  'unassigned
			  (vector-ref env i))
		      l)))
	((>= i z) l))))

; Returns a list of proper lists describing the environment in effect
; at the given pc with the given template's code vector.
;
; Entries in the environment-maps table (one per template) have the form
;   #(parent-uid pc-in-parent (env-map ...))
;
; Each environment map (one per let or lambda-expression) has the form
;   #(pc-before pc-after (var ...) (env-map ...))
;
; Cf. procedure (note-environment vars segment) in comp.scm.

(define (get-shape dd pc)
  (if (debug-data? dd)
      (let loop ((emaps (debug-data-env-maps dd))
                 (shape (get-shape (get-debug-data
                                    (debug-data-parent dd))
                                   (debug-data-pc-in-parent dd))))
        (if (null? emaps)
            shape
            (let ((pc-before (vector-ref (car emaps) 0))
                  (pc-after  (vector-ref (car emaps) 1))
                  (vars      (vector-ref (car emaps) 2))
                  (more-maps (vector-ref (car emaps) 3)))
              (if (and (>= pc pc-before)
                       (< pc pc-after))
                  (loop more-maps
                        (cons (vector->list vars) shape))
                  (loop (cdr emaps) shape)))))
      '()))

;----------------
; Printing menus.
;
; If the current thing is a continuation we print its source code first.
; Then we step down the menu until we run out or we reach the menu limit.


(define (display-menu menu start port)
  (newline port)
  (maybe-display-source (focus-object) #f)
  (let ((menu (list-tail menu start))
	(limit (+ start *menu-limit*)))
    (let loop ((i start) (menu menu))
      (with-limited-output
       (lambda ()
	 (cond ((null? menu))
	       ((and (>= i limit)
		     (not (null? (cdr menu))))
		(display " [m] more..." port) (newline port))
	       (else
		(let ((item (car menu)))
		  (display " [" port)
		  (write i port)
		  (if (car item)
		      (begin (display ": " port)
			     (write-carefully (car item) port)))
		  (display "] " port)
		  (write-carefully
		   (value->expression (cadr item))
		   port)
		  (newline port)
		  (loop (+ i 1) (cdr menu))))))))))

; Exception continuations don't have source, so we get the source from
; the next continuation if it is from the same procedure invocation.

(define (maybe-display-source thing exception?)
  (cond ((not (continuation? thing))
	 (values))
	((exception-continuation? thing)
	 (let ((next (continuation-cont thing)))
	   (if (not (eq? next (continuation-parent thing)))
	       (maybe-display-source next #t))))
	(else
	 (let ((dd (continuation-debug-data thing)))
	   (if dd
	       (let ((source (assoc (continuation-pc thing)
				    (debug-data-source dd))))
		 (if source
		     (display-source-info (cdr source) exception?))))))))
  
; Show the source code for a continuation, if we have it.

(define (display-source-info info exception?)
  (if (pair? info)
      (let ((o-port (command-output))
	    (i (car info))
	    (exp (cdr info)))
	(if (and (integer? i) (list? exp))
	    (begin
	      (display (if exception?
			   "Next call is "
			   "Waiting for ")
		       o-port)
	      (limited-write (list-ref exp i) o-port
			     *write-depth* *write-length*)
	      (newline o-port)
	      (display "  in " o-port)
	      (limited-write (append (sublist exp 0 i)
				     (list '^^^)
				     (list-tail exp (+ i 1)))
			     o-port
			     *write-depth* *write-length*)
	      (newline o-port))))))

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

;----------------
; Utilities

(define (coerce-to-template obj)
  (cond ((template? obj) obj)
	((closure? obj) (closure-template obj))
	((continuation? obj) (continuation-template obj))
	(else (error "expected a procedure or continuation" obj))))

(define (with-limited-output thunk)
  (let-fluids $write-depth *write-depth*
              $write-length *write-length*
    thunk))

