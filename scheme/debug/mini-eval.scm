; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; Miniature evaluator.

(define (eval form package)
  (evil form package))

(define (evil exp env)
  (cond ((symbol? exp) (env exp))
	((not (pair? exp)) exp)
	((eq? (car exp) 'quote) (cadr exp))
	((eq? (car exp) 'lambda)
	 (lambda args
	   (evil-begin (cddr exp) (bind (cadr exp) args env))))
	((eq? (car exp) 'if)
	 (evil (if (evil (cadr exp) env)
		   (caddr exp)
		   (cadddr exp))
	       env))
	((eq? (car exp) 'define)
	 (let* ((pat (cadr exp))
		(lhs (if (pair? pat) (car pat) pat))
		(rhs (if (pair? pat)
			 `(lambda ,(cdr pat) ,@(cddr exp))
			 (caddr exp))))
	   ((env '%%define%%) lhs (evil rhs env))))
	(else
	 (apply (evil (car exp) env)
		(map (lambda (arg) (evil arg env)) (cdr exp))))))

(define (evil-begin exp-list env)
  (if (null? (cdr exp-list))
      (evil (car exp-list) env)
      (begin (evil (car exp-list) env)
	     (evil-begin (cdr exp-list) env))))

(define (bind names vals env)
  (let ((alist (map cons names vals)))
    (lambda (name)
      (let ((probe (assq name alist)))
	(if probe (cdr probe) (env name))))))

; Initial package

(define (initial-package name)
  (let ((probe (assq name *initial-bindings*)))
    (if probe (cdr probe) (error "unbound" name))))

(define (define-initial name val)
  (let ((probe (assq name *initial-bindings*)))
    (if probe
	(set-cdr! probe val)
	(set! *initial-bindings*
	      (cons (cons name val) *initial-bindings*)))))

(define *initial-bindings*
  (list (cons '%%define%% define-initial)))

(define-syntax define-initial-stuff
  (syntax-rules ()
    ((define-initial-stuff ?name ...)
     (for-each define-initial
	       '(?name ...)
	       (list ?name ...)))))

(define-initial-stuff
  cons car cdr + - * / < = > list map append reverse
  make-vector vector-ref vector-set! vector-length
  apply)


; LOAD

(define (load filename)
  (load-into filename (interaction-environment)))

(define (load-into filename env)
  (call-with-input-file filename
    (lambda (port)
      (let loop ()
	(let ((form (read port)))
	  (cond ((eof-object? form))
		(else
		 (eval form env)
		 (loop))))))))

(define (eval-from-file forms env filename)
  (for-each (lambda (form) (eval form env)) forms))
(define (eval-scanned-forms forms env filename)
  (for-each (lambda (form) (eval form env)) forms))


; Interaction environment

(define (set-interaction-environment! package)
  (set! *interaction-environment* package))

(define (interaction-environment)
  *interaction-environment*)

(define *interaction-environment* initial-package)

(define (set-scheme-report-environment! n package)
  (set! *scheme-report-environment* package))

(define (scheme-report-environment n)
  *scheme-report-environment*)

(define (null-environment n)
  *scheme-report-environment*)

(define *scheme-report-environment* #f)
