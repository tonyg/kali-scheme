; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file derive.scm.

;;;; Macro expanders for the standard macros

(define the-usual-transforms (make-table))

(define (define-usual-macro name proc aux-names)
  (table-set! the-usual-transforms
	      name
	      (cons (lambda (exp rename compare)
		      (apply proc rename compare (cdr exp)))
		    aux-names)))

(define (usual-transform name)
  (table-ref the-usual-transforms name))

;

(define-usual-macro 'and
  (lambda (rename compare . conjuncts)
    (cond ((null? conjuncts) `#t)
          ((null? (cdr conjuncts)) (car conjuncts))
          (else `(,(rename 'if) ,(car conjuncts)
		     (,(rename 'and) ,@(cdr conjuncts))
		     ,#f))))   ; bootstrapping does not allow #F embedded in
                               ; quoted structure
  '(if and))

; Tortuously crafted so as to avoid dependency on any (unspecific)
; procedure.

(define-usual-macro 'cond
  (lambda (rename compare . clauses)
    (let ((result
	   (let recur ((clauses clauses))
	     (if (null? clauses)
		 '()
		 (list
		  (let ((clause (car clauses))
			(more-clauses (cdr clauses)))
		    (cond ((and (null? more-clauses)
				(compare (car clause) (rename 'else)))
			   `(,(rename 'begin) ,@(cdr clause)))
			  ((null? (cdr clause))
			   `(,(rename 'or) ,(car clause)
					   ,@(recur more-clauses)))
			  ((compare (cadr clause) (rename '=>))
			   (let ((temp (rename 'temp)))
			     `(,(rename 'let)
			       ((,temp ,(car clause)))
			       (,(rename 'if) ,temp
					      (,(caddr clause) ,temp)
					      ,@(recur more-clauses)))))
			  (else
			   `(,(rename 'if) ,(car clause)
					   (,(rename 'begin) ,@(cdr clause))
					   ,@(recur more-clauses))))))))))
      (if (null? result)
	  (syntax-error "empty COND")
	  (car result))))
  '(or cond begin let if begin))

(define-usual-macro 'do
  (lambda (rename compare . (specs end . body))
    (let ((%loop (rename 'loop))
	  (%letrec (rename 'letrec))
	  (%lambda (rename 'lambda))
	  (%cond (rename 'cond)))
      `(,%letrec ((,%loop
		   (,%lambda ,(map car specs)
			     (,%cond ,end
				     (else ,@body
					   (,%loop
					    ,@(map (lambda (y)
						     (if (null? (cddr y))
							 (car y)
							 (caddr y)))
						   specs)))))))
		 (,%loop ,@(map cadr specs)))))
  '(letrec lambda cond))

(define-usual-macro 'let
  (lambda (rename compare . (specs . body))
    (cond ((list? specs)
           `((,(rename 'lambda) ,(map car specs) ,@body)
             ,@(map cadr specs)))
          ((name? specs)
           (let ((tag specs)
                 (specs (car body))
                 (body (cdr body))
		 (%letrec (rename 'letrec))
		 (%lambda (rename 'lambda)))
             `(,%letrec ((,tag (,%lambda ,(map car specs) ,@body)))
			(,tag ,@(map cadr specs)))))
	  (else (syntax-error "invalid LET syntax"
			      `(let ,specs ,@body)))))
  '(lambda letrec))

(define-usual-macro 'let*
  (lambda (rename compare . (specs . body))
    (if (or (null? specs)
            (null? (cdr specs)))
        `(,(rename 'let) ,specs ,@body)
        `(,(rename 'let) (,(car specs))
			 (,(rename 'let*) ,(cdr specs) ,@body))))
  '(let let*))

(define-usual-macro 'or
  (lambda (rename compare . disjuncts)
    (cond ((null? disjuncts) #f)  ;not '#f
          ((null? (cdr disjuncts)) (car disjuncts))
          (else (let ((temp (rename 'temp)))
		  `(,(rename 'let) ((,temp ,(car disjuncts)))
		     (,(rename 'if) ,temp
			 ,temp
			 (,(rename 'or) ,@(cdr disjuncts))))))))
  '(let if or))


; CASE needs auxiliary MEMV.

(define-usual-macro 'case
  (lambda (rename compare . (key . clauses))
    (let ((temp (rename 'temp))
	  (%eqv? (rename 'eq?))
	  (%memv (rename 'memv))
	  (%quote (rename 'quote)))
      `(,(rename 'let) ((,temp ,key))
	 (,(rename 'cond) ,@(map (lambda (clause)
			`(,(cond ((compare (car clause) (rename 'else))
				  (car clause))
				 ((null? (car clause))
				  #f)
				 ((null? (cdar clause)) ;+++
				  `(,%eqv? ,temp (,%quote ,(caar clause))))
				 (else
				  `(,%memv ,temp (,%quote ,(car clause)))))
			  ,@(cdr clause)))
		      clauses)))))
  '(let cond eqv? memv quote))


; Quasiquote

(define-usual-macro 'quasiquote
  (lambda (rename compare . (x))

    (define %quote (rename 'quote))
    (define %quasiquote (rename 'quasiquote))
    (define %unquote (rename 'unquote))
    (define %unquote-splicing (rename 'unquote-splicing))
    (define %append (rename 'append))
    (define %cons (rename 'cons))
    (define %list->vector (rename 'list->vector))

    (define (expand-quasiquote x level)
      (descend-quasiquote x level finalize-quasiquote))

    (define (finalize-quasiquote mode arg)
      (cond ((eq? mode 'quote) `(,%quote ,arg))
	    ((eq? mode 'unquote) arg)
	    ((eq? mode 'unquote-splicing)
	     (syntax-error ",@ in invalid context" arg))
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
	      (cond ((and (eq? car-mode 'quote) (eq? cdr-mode 'quote))
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
      (and (pair? x) (compare (car x) marker)))

    (expand-quasiquote x 0))
  '(append cons list->vector quasiquote unquote unquote-splicing))
