; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; For define-package macro

(define (make-a-package opens-thunk accesses-thunk dir clauses)
  (make-package opens-thunk accesses-thunk
		;; Pretty much of a kludge
		*evaluator*
		(*make-package-for-syntax-promise*)
		dir clauses))

(define (init-defpackage! evaluator foo)
  (set! *evaluator* evaluator)
  (set! *make-package-for-syntax-promise* foo))

(define (loser . rest)
  (error "init-defpackage! neglected"))

(define *evaluator* loser)
(define *make-package-for-syntax-promise* loser)


(define (make-compiler-base)
  (let ((items '())
	(procs '())
	(specials '()))
    (table-walk (lambda (name op)
		  (let ((type (operator-type op)))
		    (set! items (cons `(,name ,type) items))
		    (case type
		      ((syntax) (set! specials (cons name specials)))
		      ((procedure) (set! procs (cons name procs)))
		      (else
		       ;; Various compilers will have internal operators
		       ;; (warn "unrecocognized operator type" name type)
		       0))))
		operators-table)
    (if (not (and (memq 'primitive-procedure specials)
		  (memq 'define specials)))
	(warn "impoverished compiler base" specials))
    (make-structure
       'primitives
       (make-simple-signature items)
       (make-package (lambda () '())
		     ;; accesses eval-for-syntax dir clauses
		     (lambda () '())
		     #f			;evaluator
		     (delay #f)		;env for syntax
		     ""			;file name
		     `((specials ,@specials)
		       (begin ,@(map (lambda (name)
				       `(define ,name
					  (primitive-procedure ,name)))
				     procs))
		       (integrate-primitives))))))


(define signature-of structure-signature)
