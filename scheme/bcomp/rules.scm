; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; The syntax-rules macro (new in R5RS)

; Example:
;
; (define-syntax or
;   (syntax-rules ()
;     ((or)          #f)
;     ((or e)        e)
;     ((or e1 e ...) (let ((temp e1))
;		       (if temp temp (or e ...))))))

(define-usual-macro 'syntax-rules
  (lambda (exp r c)
    (if (pair? (cdr exp))
        (let ((subkeywords (cadr exp))
              (rules (cddr exp)))
       (if (and (list? subkeywords)
		(every name? subkeywords))
       ;; Pair of the procedure and list of auxiliary names
	   `(,(r 'cons)              ;should be 'transformer
	       ,(process-rules rules subkeywords r c)
	       (,(r 'quote)
	        ,(find-free-names-in-syntax-rules subkeywords rules)))
	   exp))
   exp))
  '(append and car cdr cond cons else eq? equal? lambda let let* map
	   pair? quote code-quote values))


(define (process-rules rules subkeywords r c)

  (define %append (r 'append))
  (define %apply (r 'apply))
  (define %and (r 'and))
  (define %car (r 'car))
  (define %cdr (r 'cdr))
  (define %compare (r 'compare))
  (define %cond (r 'cond))
  (define %cons (r 'cons))
  (define %else (r 'else))
  (define %eq? (r 'eq?))
  (define %equal? (r 'equal?))
  (define %input (r 'input))
  (define %lambda (r 'lambda))
  (define %let (r 'let))
  (define %let* (r 'let*))
  (define %map (r 'map))
  (define %pair? (r 'pair?))
  (define %quote (r 'quote))
  (define %code-quote (r 'code-quote))
  (define %rename (r 'rename))
  (define %tail (r 'tail))
  (define %temp (r 'temp))

  (define (make-transformer rules)
    `(,%lambda (,%input ,%rename ,%compare)
       (,%let ((,%tail (,%cdr ,%input)))
	 (,%cond ,@(map process-rule rules)
		 (,%else ,%input)))))       ;Error when left unchanged.

  (define (process-rule rule)
    (if (and (pair? rule)
	     (pair? (cdr rule))
	     (null? (cddr rule)))
	(let ((pattern (cdar rule))
	      (template (cadr rule)))
	  `((,%and ,@(process-match %tail pattern))
	    (,%let* ,(process-pattern pattern
				      %tail
				      (lambda (x) x))
		    ,(process-template template
				       0
				       (meta-variables pattern 0 '())))))
	(syntax-error "ill-formed syntax rule" rule)))

  ; Generate code to test whether input expression matches pattern

  (define (process-match input pattern)
    (cond ((name? pattern)
	   (if (member pattern subkeywords)
	       `((,%compare ,input (,%rename (,%code-quote ,pattern))))
	       `()))
	  ((segment-pattern? pattern)
	   (process-segment-match input (car pattern)))
	  ((pair? pattern)
	   `((,%let ((,%temp ,input))
	       (,%and (,%pair? ,%temp)
		    ,@(process-match `(,%car ,%temp) (car pattern))
		    ,@(process-match `(,%cdr ,%temp) (cdr pattern))))))
	  ((or (null? pattern) (boolean? pattern) (char? pattern))
	   `((,%eq? ,input ',pattern)))
	  (else
	   `((,%equal? ,input ',pattern)))))

  (define (process-segment-match input pattern)
    (let ((conjuncts (process-match '(car l) pattern)))
      (if (null? conjuncts)
	  `((list? ,input))			;+++
	  `((let loop ((l ,input))
	      (or (null? l)
		  (and (pair? l)
		       ,@conjuncts
		       (loop (cdr l)))))))))

  ; Generate code to take apart the input expression
  ; This is pretty bad, but it seems to work (can't say why).

  (define (process-pattern pattern path mapit)
    (cond ((name? pattern)
	   (if (memq pattern subkeywords)
	       '()
	       (list (list pattern (mapit path)))))
	  ((segment-pattern? pattern)
	   (process-pattern (car pattern)
			    %temp
			    (lambda (x)	;temp is free in x
			      (mapit (if (eq? %temp x)
					 path ;+++
					 `(,%map (,%lambda (,%temp) ,x)
						 ,path))))))
	  ((pair? pattern)
	   (append (process-pattern (car pattern) `(,%car ,path) mapit)
		   (process-pattern (cdr pattern) `(,%cdr ,path) mapit)))
	  (else '())))

  ; Generate code to compose the output expression according to template

  (define (process-template template dim env)
    (cond ((name? template)
	   (let ((probe (assq template env)))
	     (if probe
		 (if (<= (cdr probe) dim)
		     template
		     (syntax-error "template dimension error (too few ...'s?)"
				   template))
		 `(,%rename (,%code-quote ,template)))))
	  ((segment-template? template)
	   (let* ((depth (segment-depth template))
		  (seg-dim (+ dim depth))
		  (vars
		   (free-meta-variables (car template) seg-dim env '())))
	     (if (null? vars)
		 (syntax-error "too many ...'s" template)
		 (let* ((x (process-template (car template)
					     seg-dim
					     env))
			(gen (if (equal? (list x) vars)
				 x	;+++
				 `(,%map (,%lambda ,vars ,x)
					 ,@vars)))
			(gen (do ((d depth (- d 1))
				  (gen gen `(,%apply ,%append ,gen)))
				 ((= d 1)
				  gen))))
		   (if (null? (segment-tail template))
		       gen		;+++
		       `(,%append ,gen ,(process-template (segment-tail template)
							  dim env)))))))
	  ((pair? template)
	   `(,%cons ,(process-template (car template) dim env)
		    ,(process-template (cdr template) dim env)))
	  (else
	   `(,%quote ,template))))

  ; Return an association list of (var . dim)

  (define (meta-variables pattern dim vars)
    (cond ((name? pattern)
	   (if (memq pattern subkeywords)
	       vars
	       (cons (cons pattern dim) vars)))
	  ((segment-pattern? pattern)
	   (meta-variables (car pattern) (+ dim 1) vars))
	  ((pair? pattern)
	   (meta-variables (car pattern) dim
			   (meta-variables (cdr pattern) dim vars)))
	  (else vars)))

  ; Return a list of meta-variables of given higher dim

  (define (free-meta-variables template dim env free)
    (cond ((name? template)
	   (if (and (not (memq template free))
		    (let ((probe (assq template env)))
		      (and probe (>= (cdr probe) dim))))
	       (cons template free)
	       free))
	  ((segment-template? template)
	   (free-meta-variables (car template)
				dim env
				(free-meta-variables (cddr template)
						     dim env free)))
	  ((pair? template)
	   (free-meta-variables (car template)
				dim env
				(free-meta-variables (cdr template)
						     dim env free)))
	  (else free)))

  (make-transformer rules))

(define (segment-pattern? pattern)
  (and (segment-template? pattern)
       (or (null? (cddr pattern))
	   (syntax-error "segment matching not implemented" pattern))))

(define (segment-template? pattern)
  (and (pair? pattern)
       (pair? (cdr pattern))
       (memq (cadr pattern) indicators-for-zero-or-more)))

; Count the number of `...'s in PATTERN.

(define (segment-depth pattern)
  (if (segment-template? pattern)
      (+ 1 (segment-depth (cdr pattern)))
      0))

; Get whatever is after the `...'s in PATTERN.

(define (segment-tail pattern)
  (let loop ((pattern (cdr pattern)))
    (if (and (pair? pattern)
             (memq (car pattern) indicators-for-zero-or-more))
        (loop (cdr pattern))
        pattern)))

(define indicators-for-zero-or-more (list (string->symbol "...")))

;(define (name? thing)
;  (or (symbol? thing)
;      (not (or (pair? thing)            ;Kludge!
;               (null? thing)
;               (number? thing)
;               (boolean? thing)
;               (char? thing)
;               (string? thing)))))

; The following is used by Scheme 48's static linker.

(define (find-free-names-in-syntax-rules subkeywords rules)

  (define (meta-variables pattern vars)
    (cond ((name? pattern)
	   (if (memq pattern subkeywords)
	       vars
	       (cons pattern vars)))
	  ((segment-pattern? pattern)
	   (meta-variables (car pattern) ;vars
			   (meta-variables (cddr pattern) vars)))
	  ((pair? pattern)
	   (meta-variables (car pattern)
			   (meta-variables (cdr pattern) vars)))
	  (else vars)))

  (define (free-names template vars names)
    (cond ((name? template)
	   (if (or (memq template vars)
		   (memq template names))
	       names
	       (cons template names)))
	  ((segment-template? template)
	   (free-names (car template)
		       vars
		       (free-names (cddr template) vars names)))
	  ((pair? template)
	   (free-names (car template)
		       vars
		       (free-names (cdr template) vars names)))
	  (else names)))

  (do ((rules rules (cdr rules))
       (names subkeywords
	      (let ((rule (car rules)))
		(free-names (cadr rule)
			    (meta-variables (cdar rule) '())
			    names))))
      ((null? rules) names)))
