; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


;(define (simplify-subtract call)
;  (simplify-args call 0)
;  ((pattern-simplifier
;    ((- 'a 'b) '(- a b))                    ; constant folding
;    ((- x 'a) (+ '(- 0 a) x))               ; convert to a normal form
;    ((- 'a (+ 'b x)) (- '(- a b) x))        ; merging constants
;    ((- 'a (- 'b x)) (+ x '(- a b)))        ; ditto
;    ((- x (+ 'a y)) (+ '(- 0 a) (- x y)))   ; convert to a normal form
;    ((- (+ 'a x) (+ 'b y)) (- (+ '(- a b) x) y)))
;   call))

; (pattern-simplifier pattern-spec ...)
;  =>
; (lambda (call-node) ...)
; The resulting procedure replaces instances of IN-PATTERNs with the
; corresponding OUT-PATTERNs.
;
; <pattern-spec> ::= (in-pattern out-pattern) |
;                    (in-pattern boolean-expression out-pattern) 
;
; All of the IN-PATTERNs for a particular simplifier must be calls to the
; same primop.  If the boolean-expression is present it is evaluated after
; the in-pattern is matched and in an environment where the symbols of the
; the in-pattern are bound to the corresponding values from the call.
;
; x       matches anything
; 'x      matches any literal
; (x ...) matches a call to primop X
; 5       matches the literal 5

; The patterns are matched in order.

;----------------
; Call MATCH-CALLS with a continuation that makes code to construct the
; right-hand side of the specification.  This assumes that the left-hand side
; of all of the specifications will be calls to the same primitive.  The
; initial CASE is removed from the code returned by MATCH-CALLS.

(define (make-pattern-simplifier specs rename compare)
  (let-fluids $rename rename
	      $compare compare
    (lambda ()
      (set! *generate-symbol-index* 0)
      (let* ((initial (generate-symbol 'initial))
	     (exp (match-calls (map (lambda (spec)
				      (make-pattern (car spec) (cdr spec)))
				    specs)
			       initial
			       #f
			       (lambda (patterns)
				 (if (null? patterns)
				     (error "no patterns matched" specs)
				     (check-predicates patterns initial))))))
	`(,(rename 'lambda) (,initial)
	   ,(cadar (cddr exp))))))) ; strip off initial CASE

; We user fluids to keep from having to pass these everywhere.

(define $rename (make-fluid #f))
(define $compare (make-fluid #f))

(define (name=? a b)
  ((fluid $compare) a b))

(define (rename a)		; this is not used
  ((fluid $rename) a))
    
(define-record-type pattern
  (spec        ; the specification this pattern is to match
   (env)       ; an a-list mapping atoms in the pattern to the identifiers
               ; that will be bound to the value matched by the atom
   parent      ; if this pattern is an argument in another pattern, this
               ; field contains the other pattern
   predicate   ; predicate call or #F
   build-spec  ; specification for the transformed pattern
   )
  ())

(define-record-discloser type/pattern
  (lambda (p)
    (list 'pattern (pattern-spec p))))

; Returns the pattern for the I'th argument in PATTERN.

(define (pattern-arg pattern i)
  (list-ref (pattern-spec pattern) (+ i 1)))

(define (make-pattern spec specs)
  (receive (build-spec predicate)
      (if (null? (cdr specs))
	  (values (car specs) #f)
	  (values (cadr specs) (car specs)))
    (pattern-maker spec '() #f predicate build-spec)))

; For each pattern in PATTERN, extend the environment with the I'th argument
; of the pattern bound to ID.

(define (extend-pattern-envs patterns i id)
  (map (lambda (pattern)
	 (let ((arg (pattern-arg pattern i)))
	   (set-pattern-env! pattern
			     (cons (if (pair? arg)
				       (list (cadr arg) id #t)
				       (list arg id #f))
				   (pattern-env pattern)))))
       patterns))

; Return the parent of PATTERN, setting the environment of the parent to be
; the environment of PATTERN.  This is only used once we are done with PATTERN
; and want to continue with the next argument in the parent.

(define (get-pattern-parent pattern)
  (let ((p (pattern-parent pattern)))
    (set-pattern-env! p (pattern-env pattern))
    p))

; Sort PATTERNS by the primop being called, and for each set of patterns
; matching the same primop, call MATCH-CALL-ARGS to generate code for
; those patterns.  FINISH-CALL-MATCH builds the clauses that this generates
; into a CASE expression.
; CALL-VAR is the identifier that will be bound to the call being matched.
; FAIL-VAR is either #f or a variable that should be called if no pattern
; matches.
; MORE is a procedure that finishes with the patterns after this call has
; been matched.

(define (match-calls patterns call-var fail-var more)
  (let ((primop-var (generate-symbol 'primop)))
    (let loop ((patterns patterns) (res '()))
      (if (null? patterns)
	  (finish-call-match res call-var primop-var fail-var)
	  (let ((primop (car (pattern-spec (car patterns)))))
	    (receive (same other)
		(partition-list (lambda (p)
				  (name=? primop (car (pattern-spec p))))
				(cdr patterns))
	      (loop other
		    (cons `(,(if (number? primop) 'else `(,primop))
			    ,(match-call-args (cons (car patterns) same)
					      0
					      call-var
					      fail-var
					      more))
			  res))))))))

(define (finish-call-match clauses call-var primop-var fail-var)
  (receive (elses other)
      (partition-list (lambda (c)
			(name=? (car c) 'else))
		      clauses)
    `(case (primop-id (call-primop ,call-var))
       ,@(reverse other)
       (else ,(cond ((null? elses)
		     (if fail-var `(,fail-var) #f))
		    ((null? (cdr elses))
		     `(let ((,primop-var (call-primop ,call-var)))
			,(cadar elses)))  ; strip of uneeded ELSE
		    (else
		     (error "more than one ELSE clause" elses)))))))

; Similar to MATCH-CALLS, except that this is matching the I'th argument of a
; call.  All patterns with similar I'th arguments are grouped together and
; passed to MATCH-CALL-ARG.  The clauses that are returned are made into a
; COND expression by FINISH-MATCH-CALL-ARGS.
; If there are fewer than I arguments, MORE is called to continue matching
; other parts of the patterns.
; Patterns that always match the I'th argument are handled separately.
; They are used to generate the ELSE clause of the conditional returned.
; If there are no such patterns, then the passed-in FAIL-VAR is called
; if no patterns match.

(define (match-call-args patterns i call-var fail-var more)
  (if (>= i (length (cdr (pattern-spec (car patterns)))))
      (more patterns)
      (receive (atom-patterns other-patterns)
	  (partition-list (lambda (p)
			    (atom? (pattern-arg p i)))
			  patterns)
	(let* ((arg-var (generate-symbol 'arg))
	       (else-code (cond ((null? atom-patterns)
				 #f)
				(else
				 (extend-pattern-envs atom-patterns i arg-var)
				 (match-call-args atom-patterns (+ i 1)
						  call-var fail-var more))))
	       (fail-var (if else-code (generate-symbol 'fail) fail-var))
	       (more (lambda (patterns)
		       (match-call-args patterns (+ i 1)
					call-var fail-var more))))
	  (let loop ((patterns other-patterns) (clauses '()))
	    (if (null? patterns)
		(finish-match-call-args i call-var arg-var fail-var
					else-code clauses)
		(let ((first (car patterns)))
		  (receive (same other)
		      (partition-list (lambda (p)
					(same-arg-pattern? first p i))
				      (cdr patterns))
		    (loop other
			  (cons (match-call-arg (cons first same)
						i
						arg-var
						fail-var
						more)
				clauses))))))))))

; If ELSE-CODE exists this binds FAIL-VAR to a failure procedure containing it.
; The CLAUSES are put in a COND.

(define (finish-match-call-args i call-var arg-var fail-var else-code clauses)
  `(let ((,arg-var (call-arg ,call-var ,i)))
     ,(if else-code
	  `(let ((,fail-var (lambda () ,else-code)))
	     (cond ,@clauses (else (,fail-var))))
	  `(cond ,@clauses (else ,(if fail-var `(,fail-var) #f))))))

; Are the I'th arguments of patterns P1 and P2 the same as far as matching
; arguments is concerned?

(define (same-arg-pattern? p1 p2 i)
  (let ((a1 (pattern-arg p1 i))
	(a2 (pattern-arg p2 i)))
    (cond ((atom? a1)
	   (atom? a2))
	  ((atom? a2)
	   #f)
	  ((name=? (car a1) 'quote)
	   (name=? (car a2) 'quote))
	  ((name=? (car a2) 'quote)
	   #f)
	  (else #t))))

; Dispatch on the type of the I'th argument of PATTERNS (all of which have
; similar I'th arguments) and generate the appropriate code.
; ARG-VAR is the identifier that will be bound to the actual argument.
; MORE is a procedure that generates code for the rest of the patterns.
; Atoms always match and require that the environments of the patterns
; be extended.
; Code for literals and calls are generated by other procedures.

(define (match-call-arg patterns i arg-var fail-var more)
  (let ((arg (pattern-arg (car patterns) i)))
    (cond ((name=? (car arg) 'quote)
	   `((literal-node? ,arg-var)
	     ,(match-literal patterns i arg-var fail-var more)))
	  (else
	   `((call-node? ,arg-var)
	     ,(match-calls (map (lambda (p)
				  (pattern-maker (pattern-arg p i)
						 (pattern-env p)
						 p
						 (pattern-predicate p)
						 (pattern-build-spec p)))
				patterns)
			   arg-var
			   fail-var
			   (lambda (patterns)
			     (more (map get-pattern-parent patterns)))))))))

; Again we sort the patterns into similar groups and build a clause for
; each group.  Patterns with symbols have their environments extended.
; FINISH-MATCH-LITERAL puts the clauses into a CASE expression.

(define (match-literal patterns i arg-var fail-var more)
  (receive (symbols numbers)
      (partition-list (lambda (p)
			(name? (cadr (pattern-arg p i))))
		      patterns)
    (extend-pattern-envs symbols i arg-var)
    (if (null? numbers)
	(more symbols)
	(let loop ((patterns numbers) (clauses '()))
	  (if (null? patterns)
	      (finish-match-literal clauses
				    (if (null? symbols)
					(if fail-var `(,fail-var) #f)
					(more symbols))
				    arg-var)
	      (receive (same other)
		  (partition-list (lambda (p)
				    (= (cadr (pattern-arg (car patterns) i))
				       (cadr (pattern-arg p i))))
				  (cdr patterns))
		(loop other
		      (cons `((,(cadr (pattern-arg (car patterns) i)))
			      ,(more (cons (car patterns) same)))
			    clauses))))))))

; Not great, but what to do?  I don't think the real NAME? is available.

(define (name? x)
  (not (or (pair? x)
	   (number? x))))

(define (finish-match-literal clauses else arg-var)
  (if (null? clauses)
      else
      `(case (literal-value ,arg-var)
	 ,@(reverse clauses)
	 (else ,else))))
				   
;------------------------------------------------------------------------------
; GENSYM utility

(define *generate-symbol-index* 0)

(define (generate-symbol sym)
  (let ((i *generate-symbol-index*))
    (set! *generate-symbol-index* (+ i 1))
    (concatenate-symbol sym "." i)))

;------------------------------------------------------------------------------
; Add code to check the predicate if any.

(define (check-predicates patterns initial)
  (let label ((patterns patterns))
    (cond ((null? (cdr patterns))
	   (let ((pattern (car patterns)))
	     (if (pattern-predicate pattern)
		 (make-predicate-check pattern initial #f)
		 (make-builder pattern initial))))
	  ((pattern-predicate (car patterns))
	   (make-predicate-check (car patterns)
				 initial
				 (label (cdr patterns))))
	  (else
	   (error "multiple patterns matched ~S"
		  patterns)))))

(define (make-predicate-check pattern initial rest)
  `(if (let ,(map (lambda (p)
		    `(,(car p) ,(if (caddr p)
				    `(literal-value ,(cadr p))
				    (cadr p))))
		  (pattern-env pattern))
	 ,(pattern-predicate pattern))
       ,(make-builder pattern initial)
       ,rest))

;------------------------------------------------------------------------------
; Building the result of a pattern match
; A new environment is made as the builder must keep track of how many times
; each node in the matched pattern is used.
; CLAUSES is a list of LET-NODES clauses for making the call nodes in the
; produced pattern.  VALUE is what will replace the original pattern in the
; node tree.  Any nodes that are used in the result are DETACHed.

(define (make-builder pattern initial)
  (let ((env (map (lambda (p)
		    (list (car p) (cadr p) #f))
		  (pattern-env pattern)))
	(pattern (pattern-build-spec pattern))
	(sym (generate-symbol 'result)))
    (let ((clauses (if (and (pair? pattern)
			    (not (name=? (car pattern) 'quote)))
		       (reverse (build-call sym pattern env))
		       '()))
	  (value (cond ((not (pair? pattern))
			(lookup-pattern pattern env))
		       ((name=? (car pattern) 'quote)
			`(make-literal-node ,(build-literal (cadr pattern) env)
					    (node-type ,initial)))
		       (else
			sym))))
      `(begin
	 ,@(filter-map (lambda (data)
			 (if (caddr data)
			     `(detach ,(cadr data))
			     #f))
		       env)
	 (let-nodes ,clauses
           (replace ,initial ,value))))))

; Go down the arguments in PATTERN making the appropriate LET-NODES spec
; for each.

(define (build-call id pattern env)
  (let loop ((arg-patterns (cdr pattern)) (args '()) (clauses '()))
    (if (null? arg-patterns)
	`((,id (,(car pattern) 0 . ,(reverse args)))
	  . ,clauses)
	(let ((arg (car arg-patterns)))
	  (cond ((atom? arg)
		 (loop (cdr arg-patterns)
		       (cons (lookup-pattern arg env) args)
		       clauses))
		((name=? (car arg) 'quote)
		 (loop (cdr arg-patterns)
		       (cons `'(,(build-literal (cadr arg) env)
				type/unknown)
			     args)
		       clauses))
		(else
		 (let ((sym (generate-symbol 'new)))
		   (loop (cdr arg-patterns)
			 (cons sym args)
			 (append (build-call sym arg env) clauses)))))))))

; A literal specification is either a number, a symbol which will bound to a
; number, or an expression to be evaluated.

(define (build-literal spec env)
  (cond ((number? spec)
	 spec)
	((name? spec)
	 `(literal-value ,(lookup-literal spec env)))
	(else
	 `(,(car spec)
	   . ,(map (lambda (a)
		     (build-literal a env))
		   (cdr spec))))))

; Get the identifier that will be bound to the value of PATTERN.

(define (lookup-literal pattern env)
  (cond ((assoc pattern env)
	 => cadr)
	(else
	 (error "pattern ~S not found in env" pattern))))

; Get the identifier that will be bound to the node value of PATTERN.
; Annotate the environment to mark that the node has been used.

(define (lookup-pattern pattern env)
  (cond ((assoc pattern env)
	 => (lambda (data)
	      (if (caddr data)
		  (error "node ~S is used more than once" (car data)))
	      (set-car! (cddr data) 1)
	      (cadr data)))
	(else
	 (error "pattern ~S not found in env" pattern))))



