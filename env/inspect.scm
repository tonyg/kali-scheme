; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; A dirty little inspector.
; This breaks abstractions left and right.
; Look and feel shamelessly plagiarized from the Lucid Lisp inspector.

; Eventually, integrate this better with the command processor.

; Inspector state:
;    thing  = (focus-object)
;    menu   = (prepare-menu thing)
;    start  = position within menu; modified by M (more) command
;    stack  = list of other things

(define *menu-limit* 15)
(define *write-depth* 3)
(define *write-length* 5)

(define (make-inspector-state menu position stack)
  (cons (cons position menu) stack))
(define $inspector-state (make-fluid (make-inspector-state '() 0 '())))

(define (inspect)
  (let-fluid $inspector-state
      (make-inspector-state (prepare-menu (focus-object)) 0 '())
    (lambda ()
      (present-menu)
      (let loop ()
	(let ((command (read-command-carefully "inspect: "
					       #f  ;command preferred
					       (command-input))))
	  (cond ((eof-object? command)
		 (newline (command-output))
		 (unspecific))
		(else
		 (let-fluids $write-depth *write-depth*
			     $write-length *write-length*
		   (lambda ()
		     (execute-inspector-command command)))
		 (loop))))))))

(define (present-menu)
  (let ((pos+menu (car (fluid $inspector-state))))
    (display-menu (cdr pos+menu)
		  (car pos+menu)
		  (command-output))))

(define (new-selection thing stack)
  (set-fluid! $inspector-state
	      (make-inspector-state (prepare-menu thing) 0 stack)))

(define (execute-inspector-command command)
  (let ((result-before (focus-object))
	(state-before (fluid $inspector-state)))

    (showing-focus-object
     (lambda ()
       (let ((name (car command)))
	 (if (integer? name)
	     (let ((menu (cdar (fluid $inspector-state))))
	       (if (and (>= name 0)
			(< name (length menu)))
		   (set-focus-object! (menu-ref menu name))
		   (write-line "Invalid choice." (command-output))))
	     (case name
	       ((u) (pop-inspector-stack))
	       ((m) (inspect-more))
	       ((d) (inspect-next-continuation))
	       ((t) (select-template))
	       ((q) (abort-to-command-level (car (fluid $command-levels))))
	       ((?) (inspect-help))
	       (else (execute-command command)))))))

    (let ((result-after (focus-object))
	  (state-after (fluid $inspector-state)))
      ;; Prepare & display a new menu if we're looking at
      ;; a new thing.  Push old thing on stack only if
      ;; no one's been futzing with the stack.
      (if (not (eq? result-after result-before))
	  (begin (if (eq? state-after state-before)
		     (new-selection result-after
				    (cons result-before
					  (cdr state-before))))
		 (present-menu))))))


(define (pop-inspector-stack)
  (let ((stack (cdr (fluid $inspector-state))))
    (if (pair? stack)
	(begin (new-selection (car stack) (cdr stack))
	       (set-focus-object! (car stack)))
	(write-line "Can't go up from here." (command-output)))))

(define (inspect-next-continuation)
  (if (continuation? (focus-object))
      (set-focus-object! (continuation-parent (focus-object)))
      (write-line "Can't go down from a non-continuation." (command-output))))

(define (inspect-more)
  (let* ((state (fluid $inspector-state))
	 (pos+menu (car state))
	 (menu (cdr pos+menu))
	 (position (car pos+menu)))
    (if (> (length menu) (+ *menu-limit* position))
	(let ((position (- (+ position *menu-limit*) 1)))
	  (set-car! pos+menu position)
	  (present-menu))
	(write-line "There is no more." (command-output)))))

(define (select-template)
  (set-focus-object! (coerce-to-template (focus-object))))


(define (inspect-help)
  (let ((o-port (command-output)))
    (for-each (lambda (s) (display s o-port) (newline o-port))
	      '("q        quit"
		"u        up stack (= go to previous object)"
		"d        down stack"
		"t        template"
		"<form>   evaluate a form (## is current object)"
		"or any command processor command"))))
              

(define (menu-ref menu n)
  (cadr (list-ref menu n)))


; Menus.

(define (prepare-menu thing)
  (cond ((list? thing)
         (map (lambda (x) (list #f x))
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
	 `(ref ,(weak-pointer-ref thing)))

        (else '())))

(define (template->list template)
  (do ((i (- (template-length template) 1) (- i 1))
       (r '() (cons (template-ref template i) r)))
      ((< i 0) r)))

(define (prepare-continuation-menu thing)
  (let ((dd (continuation-debug-data thing))
        (next (continuation-parent thing)))
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
				  (get-shape dd (continuation-pc thing))))))

(define (continuation-debug-data thing)
  (template-debug-data (continuation-template thing)))

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

(define (prepare-environment-menu env shape)
  (if (vector? env)
      (let ((values (rib-values env)))
        (if (pair? shape)
            (append (map list (car shape) values)
                    (prepare-environment-menu (vector-ref env 0)
                                              (cdr shape)))
            (append (map (lambda (x) (list #f x)) values)
                    (prepare-environment-menu (vector-ref env 0) shape))))
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
  (if dd
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



; Information display

(define (display-menu menu start port)
  (newline port)
  (let ((thing (focus-object)))
    (if (continuation? thing)
	(let ((dd (continuation-debug-data thing)))
	  (if dd
	      (let ((source (assoc (continuation-pc thing)
				   (debug-data-source dd))))
		(if source
		    (display-source-info (cdr source))))))))
  (let ((menu (list-tail menu start))
	(limit (+ start *menu-limit*)))
    (let loop ((i start) (menu menu))
      (let-fluids $write-depth *write-depth*
		  $write-length *write-length*
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

(define (display-source-info info)
  (if (pair? info)
      (let ((o-port (command-output))
	    (i (car info))
	    (exp (cdr info)))
	(if (and (integer? i) (list? exp))
	    (begin
		(display "Waiting for " o-port)
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


(define-command 'inspect "<exp>" "inspector (? for help)"
  '(&opt form)
  (lambda (form)
    (if form
	(showing-focus-object
	 (lambda ()
	   (evaluate-and-select form (environment-for-commands)))))
    (inspect)))

(define (debug)
  (showing-focus-object
   (lambda ()
     (set-focus-object! (command-continuation))))
  (inspect))

(define-command 'debug "" "inspect the stack"
  '() debug)




(define (where-defined thing)
  (let loop ((dd (template-debug-data (closure-template thing))))
    (if (debug-data? dd)
	(if (string? (debug-data-name dd))
	    (debug-data-name dd)
	    (loop (debug-data-parent dd)))
	#f)))

(define-command 'where "<procedure>" "show source file name"
  '(&opt expression)
  (lambda (exp)
    (let ((proc (if exp
		    (evaluate exp (environment-for-commands))
		    (focus-object)))
	  (port (command-output)))
      (if (procedure? proc)
	  (let ((probe (where-defined proc)))
	    (if probe
		(display probe port)
		(display "Source file not recorded" port)))
	  (display "Not a procedure" port))
      (newline port))))




(define (coerce-to-template obj)	;utillity for various commands
  (cond ((template? obj) obj)
	((closure? obj) (closure-template obj))
	((continuation? obj) (continuation-template obj))
	(else (error "expected a procedure or continuation" obj))))
