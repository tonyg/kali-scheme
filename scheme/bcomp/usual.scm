; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file usual.scm.

;;;; Macro expanders for the standard macros

(define the-usual-transforms (make-table))

(define (define-usual-macro name proc aux-names)
  (table-set! the-usual-transforms
	      name
	      (cons proc aux-names)))

(define (usual-transform name)
  (or (table-ref the-usual-transforms name)
      (assertion-violation 'usual-transform  "no such transform" name)))

; Ordinarily we would write #f instead of ,#f below.  However, it is
; useful (although decreasingly so) to be able compile Scheme 48
; images using a Scheme system that does not distinguish #f from ().
; In this case, the cross-compiler treats the expression #f (= ()) as
; boolean false, and any () (= #f) in a quoted constant as the empty
; list.  If we were to write `(if ... #f) then this would be seen in
; the *target* system as `(if ... ()), which would be a syntax error.

(define-usual-macro 'and
  (lambda (exp r c)
    (let ((conjuncts (cdr exp)))
      (cond ((null? conjuncts) `#t)
	    ((null? (cdr conjuncts)) (car conjuncts))
	    (else `(,(r 'if) ,(car conjuncts)
		       (,(r 'and) ,@(cdr conjuncts))
		       ,#f)))))
  '(if and))

; Tortuously crafted so as to avoid the need for an (unspecific)
; procedure.  Unspecific values come from IF forms.

(define-usual-macro 'cond
  (lambda (exp r c)
    (let ((clauses (cdr exp)))
      (if (or (null? clauses)
	      (not (every list? clauses)))
	  exp
	  (car (let recur ((clauses clauses))
		 (if (null? clauses)
		     '()
		     (list
		      (let ((clause (car clauses))
			    (more (recur (cdr clauses))))
			(cond ((c (car clause) (r 'else))
			       ;; (if (not (null? more)) ...error...)
			       `(,(r 'begin) ,@(cdr clause)))
			      ((null? (cdr clause))
			       `(,(r 'or) ,(car clause)
					  ,@more))
			      ((c (cadr clause) (r '=>))
			       (let ((temp (r 'temp)))
				 (if (null? (cddr clause))
				     exp
				     `(,(r 'let)
				       ((,temp ,(car clause)))
				       (,(r 'if) ,temp
						 (,(caddr clause) ,temp)
						 ,@more)))))
			      (else
			       `(,(r 'if) ,(car clause)
					  (,(r 'begin) ,@(cdr clause))
					  ,@more)))))))))))
  '(or cond begin let if begin))

(define-usual-macro 'do
  (lambda (exp r c)
    (if (and (pair? (cdr exp))
	     (pair? (cddr exp)))
	(let ((specs (cadr exp))
	      (end (caddr exp))
	      (body (cdddr exp))
	      (%loop (r 'loop))
	      (%letrec (r 'letrec))
	      (%lambda (r 'lambda))
	      (%cond (r 'cond)))
	  (if (and (list? specs)
		   (every do-spec? specs)
		   (list? end))
	      `(,%letrec ((,%loop
			   (,%lambda ,(map car specs)
				     (,%cond ,end
					     (else ,@body
						   (,%loop
						    ,@(map (lambda (spec)
							     (if (null? (cddr spec))
								 (car spec)
								 (caddr spec)))
							   specs)))))))
			 (,%loop ,@(map cadr specs)))
	      exp))
	exp))
  '(letrec lambda cond))

(define (do-spec? s)
  (and (pair? s)
       (name? (car s))
       (pair? (cdr s))
       (let ((rest (cddr s)))
	 (or (null? rest)
	     (and (pair? rest)
		  (null? (cdr rest)))))))

(define-usual-macro 'let
  (lambda (exp r c)
    (if (pair? (cdr exp))
	(let ((specs (cadr exp))
	      (body (cddr exp))
	      (%lambda (r 'lambda)))
	  (if (name? specs)
	      (let ((tag specs)
		    (specs (car body))
		    (body (cdr body))
		    (%letrec (r 'letrec)))
		(if (specs? specs)
		    `((,%letrec ((,tag (,%lambda ,(map car specs) ,@body)))
				,tag)
		      ,@(map cadr specs))
		    exp))
	      (if (specs? specs)
		  `((,%lambda ,(map car specs) ,@body)
		    ,@(map cadr specs))
		  exp)))
	exp))
  '(lambda letrec))

(define-usual-macro 'let*
  (lambda (exp r c)
    (if (pair? (cdr exp))
	(let ((specs (cadr exp))
	      (body (cddr exp)))
	  (if (specs? specs)
	      (if (or (null? specs)
		      (null? (cdr specs)))
		  `(,(r 'let) ,specs ,@body)
		  `(,(r 'let) (,(car specs))
			      (,(r 'let*) ,(cdr specs) ,@body)))
	      exp))
	exp))
  '(let let*))

(define (specs? x)
  (or (null? x)
      (and (pair? x)
	   (let ((s (car x)))
	     (and (pair? s)
		  (name? (car s))
		  (pair? (cdr s))
		  (null? (cddr s))))
	   (specs? (cdr x)))))

(define-usual-macro 'or
  (lambda (exp r c)
    (let ((disjuncts (cdr exp)))
      (cond ((null? disjuncts)
	     #f)  ;not '#f
	    ((not (pair? disjuncts))
	     exp)
	    ((null? (cdr disjuncts))
	     (car disjuncts))
	    (else
	     (let ((temp (r 'temp)))
	       `(,(r 'let) ((,temp ,(car disjuncts)))
		  (,(r 'if) ,temp
			    ,temp
			    (,(r 'or) ,@(cdr disjuncts)))))))))
  '(let if or))

; CASE needs auxiliary MEMV.

(define-usual-macro 'case
  (lambda (exp r c)
    (if (and (list? (cdr exp))
	     (every (lambda (clause)
		      (case-clause? clause c (r 'else)))
		    (cddr exp)))
	(let ((key (cadr exp))
	      (clauses (cddr exp))
	      (temp (r 'temp))
	      (%eqv? (r 'eqv?))
	      (%eq? (r 'eq?))     ;+++ hack for symbols
	      (%memv (r 'memv))
	      (%quote (r 'quote))
	      (%else (r 'else)))
	  `(,(r 'let)
	    ((,temp ,key))
	    (,(r 'cond)
	     ,@(map (lambda (clause)
		      `(,(cond ((c (car clause) %else)
				(car clause))
			       ((null? (car clause))
				#f)
			       ((null? (cdar clause)) ;+++
				`(,(if (symbol? (caar clauses)) %eq? %eqv?)
				    ,temp
				    (,%quote ,(caar clause))))
			       (else
				`(,%memv ,temp (,%quote ,(car clause)))))
			,@(cdr clause)))
		    clauses))))
	exp))
  '(let cond eqv? eq? memv quote))

(define (case-clause? c compare %else)
  (and (list? c)
       (let ((head (car c)))
	 (or (null? head)
	     (compare head %else)
	     (list? head)))))

; Quasiquote

(define-usual-macro 'quasiquote
  (lambda (exp r c)

    (define %quote (r 'quote))
    (define %quasiquote (r 'quasiquote))
    (define %unquote (r 'unquote))
    (define %unquote-splicing (r 'unquote-splicing))
    (define %append (r 'append))
    (define %cons (r 'cons))
    (define %list->vector (r 'list->vector))

    (define (expand-quasiquote x level)
      (descend-quasiquote x level finalize-quasiquote))

    (define (finalize-quasiquote mode arg)
      (cond ((eq? mode 'quote) `(,%quote ,arg))
	    ((eq? mode 'unquote) arg)
	    ((eq? mode 'unquote-splicing)
	     (syntax-violation 'quasiquote ",@ in invalid context" arg))
	    (else `(,mode ,@arg))))

    (define (descend-quasiquote x level return)
      (cond ((vector? x)
	     (descend-quasiquote-vector x level return))
	    ((not (pair? x))
	     (return 'quote x))
	    ((interesting-to-quasiquote? x %quasiquote)
	     (descend-quasiquote-pair x (+ level 1) return))
	    ((interesting-to-quasiquote? x %unquote)
	     (cond ((= level 0)
		    (return 'unquote (cadr x)))
		   (else
		    (descend-quasiquote-pair x (- level 1) return))))
	    ((interesting-to-quasiquote? x %unquote-splicing)
	     (cond ((= level 0)
		    (return 'unquote-splicing (cadr x)))
		   (else
		    (descend-quasiquote-pair x (- level 1) return))))
	    (else
	     (descend-quasiquote-pair x level return))))

    (define (descend-quasiquote-pair x level return)
      (descend-quasiquote (car x) level
	(lambda (car-mode car-arg)
	  (descend-quasiquote (cdr x) level
	    (lambda (cdr-mode cdr-arg)
	      (cond ((and (eq? car-mode 'quote)
			  (eq? cdr-mode 'quote))
		     (return 'quote x))
		    ((eq? car-mode 'unquote-splicing)
		     ;; (,@mumble ...)
		     (cond ((and (eq? cdr-mode 'quote) (null? cdr-arg))
			    (return 'unquote
				    car-arg))
			   (else
			    (return %append
				    (list car-arg (finalize-quasiquote
						     cdr-mode cdr-arg))))))
		    (else
		     (return %cons
			     (list (finalize-quasiquote car-mode car-arg)
				   (finalize-quasiquote cdr-mode cdr-arg))))))))))

    (define (descend-quasiquote-vector x level return)
      (descend-quasiquote (vector->list x) level
	(lambda (mode arg)
	  (case mode
	    ((quote) (return 'quote x))
	    (else (return %list->vector
			  (list (finalize-quasiquote mode arg))))))))

    (define (interesting-to-quasiquote? x marker)
      (and (pair? x)
	   (c (car x) marker)))

    (if (and (pair? (cdr exp))
	     (null? (cddr exp)))
	(expand-quasiquote (cadr exp) 0)
	exp))
  '(append cons list->vector quasiquote unquote unquote-splicing))


;(define (tst e)
;  (let ((probe (usual-transform (car e))))
;    ((car probe) e (lambda (x) x) eq?)))
