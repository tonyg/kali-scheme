; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Rewrite-rule compiler (a.k.a. "extend-syntax")

; Example:
;
; (define-syntax or
;   (syntax-rules ()
;     ((or) #f)
;     ((or e) e)
;     ((or e1 e ...) (let ((temp e1))
;		       (if temp temp (or e ...))))))

(define-syntax syntax-rules
  (let ()

    (define name? symbol?)

    (define (segment-pattern? pattern)
      (and (segment-template? pattern)
	   (or (null? (cddr pattern))
	       (syntax-error "segment matching not implemented" pattern))))

    (define (segment-template? pattern)
      (and (pair? pattern)
	   (pair? (cdr pattern))
	   (memq (cadr pattern) indicators-for-zero-or-more)))

    (define indicators-for-zero-or-more (list (string->symbol "...") '---))

    (lambda (exp r c)

      (define %input (r '%input))     ;Gensym these, if you like.
      (define %compare (r '%compare))
      (define %rename (r '%rename))
      (define %tail (r '%tail))
      (define %temp (r '%temp))

      (define rules (cddr exp))
      (define subkeywords (cadr exp))

  (define (make-transformer rules)
    `(lambda (,%input ,%rename ,%compare)
       (let ((,%tail (cdr ,%input)))
	 (cond ,@(map process-rule rules)
		 (else
		  (syntax-error
		   "use of macro doesn't match definition"
		   ,%input))))))

  (define (process-rule rule)
    (if (and (pair? rule)
	     (pair? (cdr rule))
	     (null? (cddr rule)))
	(let ((pattern (cdar rule))
	      (template (cadr rule)))
	  `((and ,@(process-match %tail pattern))
	    (let* ,(process-pattern pattern
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
	       `((,%compare ,input (,%rename ',pattern)))
	       `()))
	  ((segment-pattern? pattern)
	   (process-segment-match input (car pattern)))
	  ((pair? pattern)
	   `((let ((,%temp ,input))
	       (and (pair? ,%temp)
		    ,@(process-match `(car ,%temp) (car pattern))
		    ,@(process-match `(cdr ,%temp) (cdr pattern))))))
	  ((or (null? pattern) (boolean? pattern) (char? pattern))
	   `((eq? ,input ',pattern)))
	  (else
	   `((equal? ,input ',pattern)))))

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
					 `(map (lambda (,%temp) ,x)
						 ,path))))))
	  ((pair? pattern)
	   (append (process-pattern (car pattern) `(car ,path) mapit)
		   (process-pattern (cdr pattern) `(cdr ,path) mapit)))
	  (else '())))

  ; Generate code to compose the output expression according to template

  (define (process-template template rank env)
    (cond ((name? template)
	   (let ((probe (assq template env)))
	     (if probe
		 (if (<= (cdr probe) rank)
		     template
		     (syntax-error "template rank error (too few ...'s?)"
				   template))
		 `(,%rename ',template))))
	  ((segment-template? template)
	   (let ((vars
		  (free-meta-variables (car template) (+ rank 1) env '())))
	     (if (null? vars)
		 (syntax-error "too many ...'s" template)
		 (let* ((x (process-template (car template)
					     (+ rank 1)
					     env))
			(gen (if (equal? (list x) vars)
				 x	;+++
				 `(map (lambda ,vars ,x)
					 ,@vars))))
		   (if (null? (cddr template))
		       gen		;+++
		       `(append ,gen ,(process-template (cddr template)
							  rank env)))))))
	  ((pair? template)
	   `(cons ,(process-template (car template) rank env)
		    ,(process-template (cdr template) rank env)))
	  (else `(quote ,template))))

  ; Return an association list of (var . rank)

  (define (meta-variables pattern rank vars)
    (cond ((name? pattern)
	   (if (memq pattern subkeywords)
	       vars
	       (cons (cons pattern rank) vars)))
	  ((segment-pattern? pattern)
	   (meta-variables (car pattern) (+ rank 1) vars))
	  ((pair? pattern)
	   (meta-variables (car pattern) rank
			   (meta-variables (cdr pattern) rank vars)))
	  (else vars)))

  ; Return a list of meta-variables of given higher rank

  (define (free-meta-variables template rank env free)
    (cond ((name? template)
	   (if (and (not (memq template free))
		    (let ((probe (assq template env)))
		      (and probe (>= (cdr probe) rank))))
	       (cons template free)
	       free))
	  ((segment-template? template)
	   (free-meta-variables (car template)
				rank env
				(free-meta-variables (cddr template)
						     rank env free)))
	  ((pair? template)
	   (free-meta-variables (car template)
				rank env
				(free-meta-variables (cdr template)
						     rank env free)))
	  (else free)))

  c ;ignored

  ;; Kludge for Scheme 48 static linker.
  ;; `(cons ,(make-transformer rules)
  ;;          ',(find-free-names-in-syntax-rules subkeywords rules))

  (make-transformer rules))))
