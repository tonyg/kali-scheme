; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This builds the macro call to make the folder and constructs the arguments
; for the resulting fold procedure.  The first three clauses add a loop variable
; and final expression if they are missing.
;
; I caved in and passed the body to the FOLDER macro instead of passing it in
; as a lambda to the procedure produced by the macro.

(define-syntax reduce
  (syntax-rules ()

    ; Single state, no loop variable
    ((reduce (fold-vars ...) (state-var) body maybe-final ...)
     (iterate loop (fold-vars ...) (state-var) (loop body) maybe-final ...))

    ; No state, no loop variable
    ((reduce (fold-vars ...) () body maybe-final ...)
     (iterate loop (fold-vars ...) ()
	      (begin body (loop))
	      maybe-final ...))

    ; Multiple state, no loop variable
    ((reduce (fold-vars ...) (state-vars ...) body maybe-final ...)
     (iterate loop (fold-vars ...) (state-vars ...)
	      (call-with-values (lambda () body) loop)
	      maybe-final ...))))

(define-syntax iterate
  (syntax-rules ()

    ; No final expression
    ((iterate loop (fold-vars ...) ((state-var init) ...) body)
     (iterate loop (fold-vars ...) ((state-var init) ...) body
	     (values state-var ...)))

    ; Weird degenerate case with no iteration variables.
    ((iterate loop () ((state-var init) ...) body final)
     (let ((state-var init) ...) final))

    ; All there
    ((iterate loop
	      ((type value-var args ...) ...)
	      ((state-var init) ...)
	      body
	      final)
     ((folder ((type value-var args ...) ...) (state-var ...) loop body)
      (lambda (state-var ...)
	final)
      args ... ...
      init ...))))

; The entrance to all the rest of this.  The first step is to make the lists
; of types, value variables (which will be bound to the elements of the
; sequences), and fold variables (which will be bound to the successive
; state values of the sequence producer).

(define-syntax folder
  (syntax-rules ()
    ((folder ((type value-var args ...) ...) (state-var ...) loop body)
     (var-loop ((type value-var args ...) ...)
	       #f () () () ()
	       (state-var ...) loop body))))

; If we have reached the end of the sequences we go on to FOLDER-LOOP to build
; the body of the loop.  Otherwise we make new variables to match the arguments
; to the next sequence and continue.

(define-syntax var-loop
  (syntax-rules ()
    ((var-loop () seen-synched? (type ...)
	       (value-var ...) ((fold-var init) ...) end-tests
	       (state-var ...)
	       body-loop
	       body)
     (folder-loop (type ...)
		  (let ((body-loop (lambda (state-var ...)
				     (loop fold-var ... state-var ...))))
		    body)
		  ()
		  ((fold-var init) ...)
		  end-tests
		  (state-var ...)
		  loop
		  final))
    ((var-loop ((type value-var args ...) more ...) stuff ...)
     (copy-vars (args ...) () (var-loop2 type value-var (more ...) stuff ...)))))

; This is the continuation to the COPY-VARS use above.  We add the variables
; to the end of the variables list and then get the SYNC value.

(define-syntax var-loop2
  (syntax-rules ()
    ((var-loop2 vars type value-var stuff ...)
     (type sync (var-loop3 (type value-var vars stuff ...))))))

; This is the continuation to the SYNC `call' in VAR-LOOP2.  We get the state
; variable names.

(define-syntax var-loop3
  (syntax-rules ()
    ((var-loop3 sync? (type value-var vars stuff ...))
     (type state-vars vars (var-loop4 (type sync? value-var vars stuff ...))))))

; This is the end of the VAR-LOOP body.  We dispatch on whether TYPE, which
; has just been queried for its info, is synchronized and whether it is the
; first or a subsequent synchronized type.  In all cases we add the various
; code fragments obtained from TYPE to the appropriate lists and then go back
; to the top of the loop.

(define-syntax var-loop4
  (syntax-rules ()
    ; TYPE is the first synchronized type we have seen.
    ((var-loop4 ((fold-var init) ...)
		(type
		 #t
		 value-var
		 vars
		 more-types
		 #f
		 (types ...) (value-vars ...) (fold-init ...) end-tests
		 stuff ...))
     (var-loop more-types
	       #t
	       ((type #t #f (fold-var ...) value-var vars)
		types ...)
	       (value-vars ... value-var)
	       (fold-init ... (fold-var init) ...)
	       end-tests
	       stuff ...))
    ; TYPE is a synchronized type but not the first we have seen.
    ((var-loop4 ((fold-var init) ...)
		(type
		 #t
		 value-var
		 vars
		 more-types
		 #t
		 (types ...) (value-vars ...) (fold-init ...)
		 end-tests
		 stuff ...))
     (var-loop more-types
	       #t
	       ((type #t #t (fold-var ...) value-var vars)
		types ...)
	       (value-vars ... value-var)
	       (fold-init ... (fold-var init) ...)
	       ((type done vars (fold-var ...)) . end-tests)
	       stuff ...))
    ; TYPE is not synchronized.
    ((var-loop4 ((fold-var init) ...)
		(type
		 #f
		 value-var
		 vars
		 more-types
		 seen-synched?
		 (types ...) (value-vars ...) (fold-init ...) end-tests
		 stuff ...))
     (var-loop more-types
	       seen-synched?
	       ((type #f seen-synched? (fold-var ...) value-var vars)
		types ...)
	       (value-vars ... value-var)
	       (fold-init ... (fold-var init) ...)
	       end-tests
	       stuff ...))))

; A loop to produce a list of fresh variables.  The new variables are tacked
; onto the end of the body.

(define-syntax copy-vars
  (syntax-rules ()
    ((copy-vars () vars (cont stuff ...))
     (cont vars stuff ...))
    ((copy-vars (x y ...) (vars ...) cont)
     (copy-vars (y ...) (vars ... a) cont))))

; Here we build up the body of the loop.  When all the sequences are done we
; are done.
;
; For each sequence we do (type step stuff ...) to build the body and
; (type init vars ...) to get the initial value of the fold variable for
; that sequence.  Here `step' and `init' are keywords.
;
; The actual arguments to (type step stuff ...) are:
;  vars ...		; variables bound to the sequence's arguments
;  fold-var		; variable bound to the last state and to be bound to the
;			;  next state
;  value-var		; variable to be bound to the next element
;  loop-body-exp	; this expression continues the loop
;  final-exp		; this expression ends the loop

(define-syntax folder-loop
  (syntax-rules ()
    ((folder-loop ()			; no more sequences
		  loop-body		; body so far
		  (args ...)		; complete list of sequence arguments
		  ((fold-var init) ...) ; fold variables and their initial values
		  end-tests		; end test for first synchronized seq.
		  (state-var ...)	; the user's state variables
		  loop			; loop variable
		  final)		; final argument variable
     (lambda (final args ... state-var ...)
       (let loop ((fold-var init) ... (state-var state-var) ...)
	 loop-body)))
    ; Not synchronized
    ((folder-loop ((type #f synched? fold-vars value-var (vars ...)) more ...)
		  loop-body
		  args
		  fold-var-inits end-tests state-vars
		  loop final)
     (folder-loop (more ...)
		  (type step (vars ...) fold-vars value-var
			loop-body (final . state-vars))
		  (vars ... . args)
		  fold-var-inits end-tests state-vars
		  loop final))
    ; Synchronized, not first such
    ((folder-loop ((type #t #t fold-vars value-var (vars ...)) more ...)
		  loop-body
		  args
		  fold-var-inits end-tests state-vars
		  loop final)
     (folder-loop (more ...)
		  (type step (vars ...) fold-vars value-var
			loop-body
			(begin (error "synchronized sequence ended early")
			       (values)))
		  (vars ... . args)
		  fold-var-inits end-tests state-vars
		  loop final))
    ; First synchronized sequence
    ((folder-loop ((type #t #f fold-vars value-var (vars ...)) more ...)
		  loop-body
		  args
		  fold-var-inits end-tests state-vars
		  loop final)
     (folder-loop (more ...)
		  (type step (vars ...) fold-vars value-var
			loop-body (if (and . end-tests)
				      (final . state-vars)
				      (begin (error "synchronized sequence ended early")
					     (values))))
		  (vars ... . args)
		  fold-var-inits end-stests state-vars
		  loop final))))
		 
; Iterators

; (list* var list)
; (vector* var vector)
; (string* var string)
; (count* var start [end [step]])
; (bits* var integer [step-size])
; (input* var input-port reader)        ; (reader port) -> value or eof-object
; (stream* var function initial-state)  ; (function state) -> [value new-state]
;   A new-state of #F means that the previous value was the final one.

(define-syntax list*
  (syntax-rules (sync state-vars step)
    ((list* sync (next more))
     (next #f more))
    ((list* state-vars (start-list) (next more))
     (next ((list-var start-list)) more))
    ((list* step (start-list) (list-var) value-var loop-body final-exp)
     (if (null? list-var)
         final-exp
         (let ((value-var (car list-var))
               (list-var (cdr list-var)))
           loop-body)))))

(define-syntax list%
  (syntax-rules (sync done)
    ((list% sync (next more))
     (next #t more))
    ((list% done (start-list) (list-var))
     (null? list-var))
    ((list% stuff ...)
     (list* stuff ...))))

(define-syntax vector*
  (syntax-rules (sync state-vars step)
    ((vector* sync (next more))
     (next #f more))
    ((vector* state-vars (vector) (next more))
     (next ((i 0)) more))
    ((vector* step (vector) (i) value-var loop-body final-exp)
     (if (= i (vector-length vector))
	 final-exp
	 (let ((value-var (vector-ref vector i))
	       (i (+ i 1)))
	   loop-body)))))

(define-syntax vector%
  (syntax-rules (sync done)
    ((vector% sync (next more))
     (next #t more))
    ((vector% done (vector) (i))
     (= i (vector-length vector)))
    ((vector% stuff ...)
     (vector* stuff ...))))

(define-syntax string*
  (syntax-rules (sync state-vars step)
    ((string* sync (next more))
     (next #f more))
    ((string* state-vars (string) (next more))
     (next ((i 0)) more))
    ((string* step (string) (i) value-var loop-body final-exp)
     (if (= i (string-length string))
	 final-exp
	 (let ((value-var (string-ref string i))
	       (i (+ i 1)))
	   loop-body)))))

(define-syntax string%
  (syntax-rules (sync done)
    ((string% sync (next more))
     (next #t more))
    ((string% done (string) (i))
     (= i (string-length string)))
    ((string% stuff ...)
     (string* stuff ...))))

(define-syntax count*
  (syntax-rules (sync state-vars step)
    ((count* sync (next more))
     (next #f more))
    ((count* state-vars (start args ...) (next more))
     (next ((i start)) more))
    ((count* step (start) (i) value-var loop-body final-exp)
     (let ((value-var i)
	   (i (+ i 1)))
       loop-body))
    ((count* step (start end) (i) value-var loop-body final-exp)
     (count* step (start end 1) (i) value-var loop-body final-exp))
; This doesn't work because we don't see the original arguments, just variables
; bound to them.
;    ((count* step (start #f increment) (i) value-var loop-body final-exp)
;     (let ((value-var i)
;	   (i (+ i increment)))
;       loop-body))
    ((count* step (start end increment) (i) value-var loop-body final-exp)
     (if (= i end)
	 final-exp
	 (let ((value-var i)
	       (i (+ i increment)))
	   loop-body)))))

; Synchronized, so we don't allow the unbounded version.

(define-syntax count%
  (syntax-rules (sync done state-vars step)
    ((count% sync (next more))
     (next #t more))
    ((count% done (start end increment ...) (i))
     (= end i))
    ((count% state-vars (start args ...) (next more))
     (next ((i start)) more))
    ((count% step (start end) (i) value-var loop-body final-exp)
     (count% step (start end 1) (i) value-var loop-body final-exp))
    ((count% step (start end increment) (i) value-var loop-body final-exp)
     (if (= i end)
	 final-exp
	 (let ((value-var i)
	       (i (+ i increment)))
	   loop-body)))))

; I would really like to be able to lift the mask calculation out of the loop.
; There could be yet another clause in iterators that returned (VAR VAL) clauses
; to be added to a LET around the loop.

(define-syntax bits*
  (syntax-rules (sync state-vars step)
    ((bits* sync (next more))
     (next #f more))
    ((bits* state-vars (bit-set args ...) (next more))
     (next ((i bit-set)) more))
    ((bits* step (bit-set) (i) value-var loop-body final-exp)
     (if (= i 0)
	 final-exp
	 (let ((value-var (odd? i))
	       (i (arithmetic-shift i -1)))
	   loop-body)))
    ((bits* step (bit-set size) (i) value-var loop-body final-exp)
     (if (= i 0)
	 final-exp
	 (let ((value-var (bitwise-and i (- (arithmetic-shift 1 size) 1)))
	       (i (arithmetic-shift i (- size))))
	   loop-body)))))

; This one is unlikely to be used much, because the termination test is
; so data dependent.

(define-syntax bits%
  (syntax-rules (sync done state-vars step)
    ((bits% sync (next more))
     (next #t more))
    ((bits% done values (i))
     (= i 0))
    ((bits% more ...)
     (bits* more ...))))

(define-syntax input*
  (syntax-rules (sync state-vars step)
    ((input* sync (next more))
     (next #f more))
    ((input* state-vars (port reader) (next more))
     (next () more))
    ((input* step (port reader) () value-var loop-body final-exp)
     (let ((value-var (reader port)))
       (if (eof-object? value-var)
	   final-exp
	   loop-body)))))

(define-syntax input%
  (syntax-rules (sync done)
    ((input% sync (next more))
     (next #t more))
    ((input% done (port reader) ())
     (eof-object? (peek-char port)))
    ((input% more ...)
     (input* more ...))))

(define-syntax stream*
  (syntax-rules (sync state-vars step)
    ((stream* sync (next more))
     (next #f more))
    ((stream* state-vars (function start-state) (next more))
     (next ((state start-state)) more))
    ((stream* step (function start-state) (state) value-var loop-body final-exp)
     (call-with-values
       (lambda ()
	 (function state))
       (lambda (value-var state)
	 (if state
	     loop-body
	     final-exp))))))

(define-syntax stream%
  (syntax-rules (sync done)
    ((stream% sync (next more))
     (next #t more))
    ((stream% done (function start-state) (state))
     (call-with-values
       (lambda ()
	 (function state))
       (lambda (value-var state)
	 (not state))))
    ((stream% more ...)
     (stream* more ...))))

