; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Link script.

(define (link-initial-system)
  (let ((structures-to-open		;Structures to open for the initial
	 (struct-list scheme		;system's read-eval-print loop.
		      environments
		      module-system
		      ensures-loaded
		      packages
		      packages-internal)))  ; package-for-syntax
    (link-reified-system (append (desirable-structures)
				 structures-to-open)
			 '(build initial)
			 ;; The expression that evaluates to the
			 ;; procedure that maps the reified-structure alist
			 ;; to the startup procedure:
			 `(start ',(map car structures-to-open))
			 ;; Structures to open for evaluating that
			 ;; expression and the expression that
			 ;; evaluates to the reified-structure alist:
			 initial-system
			 for-reification
			 ;; scheme-level-1
			 )))

(define (desirable-structures)
  (let ((env (interaction-environment))
	(l '()))
    (for-each (lambda (int)
		(for-each-declaration
		     (lambda (name package-name type)
		       (if (not (assq name l))
			   (let ((s (eval name env)))
			     (if (structure? s)
				 (set! l (cons (cons name s) l))))))
		     int))
	      (list low-structures-interface
		    run-time-structures-interface
		    features-structures-interface
		    run-time-internals-structures-interface
		    compiler-structures-interface
		    initial-structures-interface))
    (reverse l)))


; Your choice of evaluators:

(define scheme (make-scheme environments evaluation))
; (define scheme (make-scheme mini-environments mini-eval))
; (define scheme (make-scheme environments run))
; etc.

; Your choice of command processors.

(define initial-system
  (make-initial-system scheme (make-mini-command scheme)))
