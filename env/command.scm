; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file command.scm.

; Command processor.

; The command processor's state is of three kinds:
; 1. User context - preserved across dump commands.
;    This includes the designated user and configuration environments.
; 2. Session state - one per "login"; not preserved across dump commands.
;    This includes ## and the command loop's interactive ports.
; 3. Command levels - one for each different command level.
;    This includes any condition being handled, and continuations.

(define command-prefix #\,)


; User context.

(define (make-user-context thunk)
  (let ((t (make-table)))
    (let-fluid $user-context t
      (lambda ()
        (for-each (lambda (name+thunk)
                    (table-set! t (car name+thunk) ((cdr name+thunk))))
                  *user-context-initializers*)
        (thunk)
        t))))

(define *user-context-initializers* '())

(define $user-context (make-fluid #f))
; (set-fluid! $user-context (make-user-context unspecific)) ;Bad for GC
(define (user-context) (fluid $user-context))

(define (user-context-accessor name initializer)
  (set! *user-context-initializers*
        (append *user-context-initializers*
                (list (cons name initializer))))
  (let ((probe (fluid $user-context)))
    (if probe (table-set! probe name (initializer))))
  (lambda ()
    (table-ref (user-context) name)))

(define (user-context-modifier name)
  (lambda (new)
    (table-set! (user-context) name new)))


; Session state.

(define session-type
  (make-record-type 'session '(input output values batch-mode? bow?)))
(define make-session
  (record-constructor session-type
                      '(input output values batch-mode? bow?)))
(define $session
  (make-fluid (make-session (current-input-port)
                            (current-output-port)
                            '() #f #f)))
(define (session-accessor name)
  (let ((a (record-accessor session-type name)))
    (lambda () (a (fluid $session)))))
(define (session-modifier name)
  (let ((m (record-modifier session-type name)))
    (lambda (new) (m (fluid $session) new))))
(define command-input (session-accessor 'input))
(define command-output (session-accessor 'output))
(define focus-values (session-accessor 'values))
(define set-focus-values! (session-modifier 'values))
(define batch-mode? (session-accessor 'batch-mode?))
(define set-batch-mode?! (session-modifier 'batch-mode?))
(define break-on-warnings? (session-accessor 'bow?))
(define set-break-on-warnings?! (session-modifier 'bow?))



; Command levels.

(define $command-levels (make-fluid '()))
(define (command-level) (car (fluid $command-levels)))

(define :command-level
  (make-record-type 'command-level
                    '(throw vm-cont condition interrupts
			    ;; env
			    )))
(define make-command-level
  (record-constructor :command-level
                      '(throw vm-cont condition interrupts
			      ;; env
			      )))
(define command-level? (record-predicate :command-level))
(define command-level-throw
  (record-accessor :command-level 'throw))
(define command-level-vm-cont
  (record-accessor :command-level 'vm-cont))
(define command-level-condition
  (record-accessor :command-level 'condition))
(define command-level-interrupts
  (record-accessor :command-level 'interrupts))
;(define command-level-env
;  (record-accessor :command-level 'env))
;(define set-command-level-env!
;  (record-modifier :command-level 'env))

(define environment-for-commands interaction-environment)

; --------------------
; Main entry point.

(define (start-command-processor resume-args context
				 ;; initial-env
				 start-thunk)
  (interrupt-before-heap-overflow!)
  ((let-fluids $command-levels '()
               $user-context context  ;Log in
               $session (make-session (current-input-port)
                                      (current-output-port)
                                      resume-args
				      (and (pair? resume-args)
					   (equal? (car resume-args) "batch"))
                                      #f)
     (lambda ()
       (command-loop start-thunk #f
		     ;; initial-env
		     )))))

; Entry for initialization & testing.

(define (command-processor command-env args) 
  (start-command-processor args
                           (make-user-context
			    (lambda ()
			      (set-user-command-environment! command-env)))
                           ;; (interaction-environment)
                           unspecific))

; Command loop
; Uses:
;  1. startup, 2. condition handler, 3. abort-to-level, 4. breakpoint

(define (command-loop start-thunk condition
		      ;; env
		      )
  (call-with-command-level condition ;; env
    (lambda (level)
      (start-command-level start-thunk level))))

(define (call-with-command-level condition
				 ;; env
				 proc)
  (primitive-catch
    (lambda (vm-cont)
      ((call-with-current-continuation
         (lambda (throw)
           (proc (make-command-level throw vm-cont condition
                                     (enabled-interrupts)
                                     ;; env
				     ))))))))

(define (start-command-level start-thunk level)
  (with-handler command-loop-condition-handler
    (lambda ()
      (let-fluids $command-levels (cons level (fluid $command-levels))
                  $note-undefined #f    ;useful
        (lambda ()
          ;;(with-interaction-environment (command-level-env level)
            ;;(lambda ()
              (start-thunk)
              (let ((condition (command-level-condition level)))
                (if condition
                    (display-condition condition (command-output)))
                (if (not (= (enabled-interrupts) all-interrupts))
                    (begin (if (not (and (interrupt? condition)
                                         (= (caddr condition) all-interrupts)))
                               (write-line "(Enabling interrupts)"
                                           (command-output)))
                           (set-enabled-interrupts! all-interrupts))))
              (let loop ()
                (let ((command (read-command-carefully (command-prompt)
                                                       (form-preferred?)
                                                       (command-input))))
                  (showing-focus-object
                   (lambda ()
                     (execute-command command)))
                  (loop))))))));;))

(define form-preferred?
  (user-context-accessor 'form-preferred? (lambda () #t)))

; Command level control

(define (pop-command-level)
  (let ((levels (fluid $command-levels)))
    (if (null? (cdr levels))
        (if (or (batch-mode?)
                (y-or-n? "Exit Scheme 48" #t))
            (exit-command-processor (lambda () 0))
            (abort-to-command-level (car levels)))
        (abort-to-command-level (cadr levels)))))

(define (abort-to-command-level level)
  (throw-to-command-level
       level
       (lambda ()
         (start-command-level
          (lambda ()
            (cond ((command-level-condition level)
                   (display "Back to" (command-output)))
                  ((null? (fluid $command-levels))
                   (newline (command-output))
                   (write-line "Top level" (command-output)))))
          ;; Condition will be displayed.
          level))))

(define (throw-to-command-level level thunk)
  ((command-level-throw level) thunk))

(define (exit-command-processor thunk)
  (throw-to-command-level (top-command-level)
                          (lambda () thunk)))

; Condition handler

(define (command-loop-condition-handler c next-handler)
  (cond ((or (warning? c) (note? c))
         (if (break-on-warnings?)
             (deal-with-condition c)
             (begin (display-condition c (command-output))
                    (unspecific))))     ;proceed
        ((or (error? c) (interrupt? c))
         (if (batch-mode?)
             (begin (display-condition c (command-output))
                    (let ((status (if (error? c) 1 2)))
                      (exit-command-processor (lambda () status))))
             (deal-with-condition c)))
        (else                           
         (next-handler))))

(define push-command-levels?
  (user-context-accessor 'push-command-levels (lambda () #t)))
;(define set-push-command-levels?!
;  (user-context-modifier 'push-command-levels))


(define (deal-with-condition c)
  (if (push-command-levels?)
      (command-loop list c
		    ;; (interaction-environment)
		    )
      (call-with-command-level c ;; (interaction-environment)
        (lambda (level)
          (set-focus-object! level)
          (display-condition c (command-output))
          (abort-to-command-level (car (fluid $command-levels)))))))


(define-condition-type 'note '())
(define note? (condition-predicate 'note))

(define (command-prompt)
  (let ((level (- (length (fluid $command-levels)) 1))
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


; Evaluate a form

(define (evaluate-and-select form env)
  (call-with-values (lambda ()
                      (evaluate form env))
    (lambda results
      (if (or (null? results)
              (not (null? (cdr results)))
              (not (eq? (car results) (unspecific))))
          (set-focus-values! results))
      (apply values results))))

(define-generic evaluate &evaluate (form env))

(define-method &evaluate (form env) (eval form env))


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


; Sentinels - run after every command.

(define *sentinels* '())
(define (run-sentinels)
  (for-each (lambda (sentinel) (sentinel)) *sentinels*))
(define (add-sentinel! sentinel)
  (if (not (memq sentinel *sentinels*))
      (set! *sentinels* (cons sentinel *sentinels*))))



; Commands.

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
  (cond ((eof-object? command)
         (newline (command-output))
         (pop-command-level))
        ((not command))       ; error while reading
        (else
         (let* ((name (car command))
		(proc (evaluate name (user-command-environment))))
	   (dynamic-wind (lambda () #f)
			 (lambda ()
			   (apply proc (cdr command)))
			 run-sentinels)))))

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
"This is an alpha-test version of Scheme 48.  You are interacting with"
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
                   

; Utilities

(define (top-command-level)
  (last (fluid $command-levels)))

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
    (write-line "Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees."
		port)
    (write-line "Please report bugs to scheme-48-bugs@martigny.ai.mit.edu."
                port)
    (if (not (batch-mode?))
	(write-line "Type ,? (comma question-mark) for help." port))))


(define (command-continuation)          ;utility for debugger
  (let ((obj (focus-object)))
    (command-level-vm-cont
     (if (command-level? obj)
         obj
         (command-level)))))
