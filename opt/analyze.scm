; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; Simple code analysis to determine whether it's a good idea to
; in-line calls to a given procedure.


; Hook into the byte code compiler.

(set-optimizer! 'auto-integrate
  (lambda (stuff p)
    (set-package-integrate?! p #t)
    (newline)
    (display "Analyzing... ") (force-output (current-output-port))
    (let* ((names '())
	   (stuff
	    (map (lambda (filename+forms)
		   (let ((filename (car filename+forms))
			 (forms (cdr filename+forms)))
		     (let-fluid $source-file-name filename
		       (lambda ()
			 (set! names (append (analyze-forms forms p) names))
			 (cons filename forms)))))
		 (alpha-stuff stuff p))))
      (cond ((not (null? names))
	     (newline)
	     (display "Calls will be compiled in line: ")
	     (write (reverse names)))
	    (else
	     (display "no in-line procedures")))
      (newline)
      stuff)))

(define (analyze-forms scanned-forms p)
  (let ((inlines '()))
    (for-each (lambda (form)
		(let ((lhs (analyze-form form p)))
		  (if lhs
		      (set! inlines (cons lhs inlines)))))
	      scanned-forms)
    inlines))

(define (analyze-form form p)
  (if (define? form)
      (let ((lhs (define-lhs form))
	    (rhs (define-rhs form)))
	(if (and (when-lose "assigned" lhs
			    (not (variable-ever-assigned? p lhs)))
		 (inlinable-rhs? rhs p lhs))
	    (begin (package-define! p lhs
				    (make-inline-transform rhs p lhs))
		   lhs)
	    #f))
      #f))

(define (inlinable-rhs? exp p id)
  (cond ((pair? exp)
	 (and (when-lose "not lambda" (list id (car exp))
		(eq? (car exp) operator/lambda))
	      (simple-lambda? exp id p)))
	((when-lose "non-symbol rhs" (list exp id)
	   (symbol? exp))
	 ;; TO DO: extend to generated names
	 (and (when-lose "rhs assigned" (list id exp)
		(not (variable-ever-assigned? p exp)))
	      (when-lose "not definitely procedure" (list id exp)
		(let ((probe (package-lookup p exp)))
		  (or (operator? probe)
		      (transform? probe)
		      (eq? probe 'procedure))))))
	(else #f)))

(define (simple-lambda? exp id p)
  (let ((locals (lambda-locals exp)))
    (and (when-lose "n-ary" id
	   (not (n-ary? (lambda-formals exp))))
	 (when-lose "count" id
	   (every (lambda (local)
		    (and (= (local-assignment-count local) 0)
			 (= (local-reference-count local) 1)))
		  locals))
	 (when-lose "analysis" id
	   (analyze (lambda-body exp) ret p)))))

(define (when-lose reason id x)
  (if (and *debug?* (not x))
      (begin (write id)
	     (display " lost because of ")
	     (display reason)
	     (newline)))
  x)

(define *debug?* #f)


; --------------------
; Main dispatch for analyzer

(define (analyze exp ret? p)
  (if (pair? exp)
      (let ((op (car exp)))
	(cond ((operator? op)
	       ((get-analyzer op) exp ret? p))
	      ((name? op)
	       (let ((probe (package-lookup p (car exp))))
		 (if (transform? probe)
		     (analyze (transform probe exp (lambda (name)
						     (probe-package p name)))
			      ret? p)
		     (analyze-call exp ret? p))))
	      (else
	       (analyze-call exp ret? p))))
      nil-analysis))

(define (analyze-random exp ret? p)
  ;; Primop.
  (reduce (lambda (arg analysis)
	    (cons-analysis (analyze arg no-ret p)
			   analysis))
	  nil-analysis
	  (cdr exp)))

(define nil-analysis '())

(define cons-analysis
  (lambda (a1 a2)
    (if (and a1 a2) (append a1 a2) #f)))

(define foul-analysis #f)


; Particular operators

(define analyzers
  (make-compilator-table (lambda (exp ret? p) (analyze-random exp ret? p))))

(define define-analyzer (car analyzers))

(define get-analyzer (cdr analyzers))

(define-analyzer 'local 'syntax		;local
  (lambda (exp ret? p) (list exp)))

(define-analyzer 'quote 'syntax
  (lambda (exp ret? p)
    (if (when-lose "hairy quotation" #f
	 (acceptable-literal? (quotation-value exp)))
	nil-analysis
	foul-analysis)))

(define-analyzer 'lambda 'syntax
  (lambda (exp ret? p) foul-analysis))

(define-analyzer 'set! 'syntax
  (lambda (exp ret? p) foul-analysis))

(define-analyzer 'if 'syntax
  (lambda (exp ret? p)
    (cons-analysis (analyze (if-test exp) no-ret p)
		   (let ((a1 (analyze (if-con exp) ret? p))
			 (a2 (analyze (if-alt exp) ret? p)))
		     (if (and a1 a2
			      (every (lambda (a)
				       (memq a a2))
				     a1))
			 a1
			 foul-analysis)))))

(define-analyzer 'begin 'syntax
  (lambda (exp ret? p)
    (cons-analysis (analyze (begin-first exp) no-ret p)
		   (analyze (begin-second exp) ret? p))))

(define-analyzer 'call 'syntax
  (lambda (exp ret? p)
    (analyze-call (cdr exp) ret? p)))

(define (analyze-call proc+args ret? p)
  (let ((proc (car proc+args)))
    (if (when-lose "non-local non-tail call" proc
	  (or ret?			;tail calls are ok
	      (local? proc)))		;so are calls to arguments
	(reduce (lambda (arg analysis)
		  (cons-analysis (analyze arg no-ret p)
				 analysis))
		nil-analysis
		proc+args)
	foul-analysis)))

(define no-ret #f)

(define ret #t)

(define (acceptable-literal? x) ;Things that TRANSPORT won't copy.
  (or (integer? x)
      (boolean? x)
      (null? x)
      (char? x)
      (name? x)))



; Once we know that we want something to be inlined, this stuff
; actually makes use of the fact.  For procedures for which all
; arguments can be substituted unconditionally, we make a transform
; (a macro, practically) that performs the subtitution.

(define (make-inline-transform exp p name)
  (let ((free (free-top-level-variables exp)))
    (make-transform (really-make-procedure-for-inline-transform exp)
		    p			;env ?
		    'procedure
		    free
		    `(make-procedure-for-inline-transform
			',(make-substitution-template exp p free)
			the-package)
		    name)))

(define (make-substitution-template exp p free)
  (schemify exp
	 (map (lambda (free) (cons free (name->extrinsic free p)))
	      free)))

; This must return an S-expression containing no operators, generated
; names, or procedures.  Assumes that the expression is the output of
; alpha-conversion.

(define (schemify exp env)
  (cond ((pair? exp)
	 (let ((op (car exp)))
	   (cond ((eq? op operator/quote)
		  (let ((val (desyntaxify (quotation-value exp))))
		    (if (literal? val)
			val		;+++
			`(,oplet/quote ,val))))
		 ((eq? op operator/local)
		  (schemify-lookup env (local-name exp)))
		 ((eq? op operator/lambda)
		  (schemify-lambda exp env))
		 (else
		  (map (lambda (exp)
			 (schemify exp env))
		       exp)))))
	((operator? exp)		;ugh
	 (operator->oplet exp))
	((name? exp)
	 (schemify-lookup env exp))
	(else exp)))

; Lambda must be special-cased because (1) the formals may be an
; improper list containing generated names, and (2) the body contains
; a bogus quoted contant that mustn't be handed to desyntaxify.

(define (schemify-lambda exp env)
  (let* ((formals (lambda-formals exp))
	 (names (normalize-formals formals))
	 (new-env (append (map (lambda (name)
				 (cons name (unused-name env name)))
			       names)
			  env)))
    `(,oplet/lambda ,(let recur ((foo formals))
		       (cond ((name? foo) (schemify-lookup new-env foo))
			     ((pair? foo)
			      (cons (recur (car foo))
				    (recur (cdr foo))))
			     (else foo)))
       ,(schemify (lambda-body exp) new-env))))

(define (schemify-lookup env name)
  (let ((probe (assq name env)))
    (if probe
	(cdr probe)
	(begin (warn "name not found in environment" name)
	       name))))
  
; I know that this is extremely pedantic.

(define (unused-name env name)
  (let ((sym (if (generated? name)
		 (generated-symbol name)
		 name)))
    (do ((i 0 (+ i 1))
	 (name sym
	       (string->symbol (string-append (symbol->string sym)
					      (number->string i)))))
	((not (assq name env)) name))))
      

(define oplet/quote (operator->oplet operator/quote))
(define oplet/lambda (operator->oplet operator/lambda))

(define (quotation? obj)
  (and (pair? obj)
       (eq? (car obj) 'quote)))




; This actually also throws in the locals that are SET!, but that's
; harmless (I think).

(define (free-top-level-variables exp)
  (let recur ((exp exp) (vars '()))
    (cond ((pair? exp)
	   (let ((op (car exp)))
	     (cond ((eq? op operator/quote) vars)
		   ((eq? op operator/local) vars)
		   ((eq? op operator/lambda)
		    (recur (lambda-body exp) vars))
		   (else
		    (reduce recur vars exp)))))
	  ((name? exp) (if (memq exp vars) vars (cons exp vars)))
	  (else vars))))


; --------------------
    

;(define (foo f p)
;  (analyze-forms (alpha-forms (scan-file f p) p)))
;
;
;(define (tst e p)
;  (inlinable-rhs? (alpha e p) #f))
;
;(define b (make-compiler-base))
;
;(define p (make-simple-package (list b) eval #f))
;
;; (define b-stuff (alpha-structure b))
;
