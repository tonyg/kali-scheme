; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.




(define (schemify node)
  (if (node? node)
      ((operator-table-ref schemifiers (node-operator-id node)) node)
      node))

(define schemifiers
  (make-operator-table (lambda (node)
			 (let ((form (node-form node)))
			   (if (list? form)
			       (map schemify form)
			       form)))))

(define (define-schemifier name proc)
  (operator-define! schemifiers name proc))

(define-schemifier 'quote
  (lambda (node) (list 'quote (cadr node))))
