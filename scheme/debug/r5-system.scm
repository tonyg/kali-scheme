; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Link script.

(define (link-revised^5-system)
  (let ((structures-to-open (struct-list scheme)))
    (link-reified-system structures-to-open
			 'revised^5
			 `(start ',(map car structures-to-open))
			 initial-system
			 for-reification
			 ;; Extra stuff (from more-packages.scm)
			 disclosers
			 package-mutation shadowing
			 bignums ratnums floatnums
			 )))

(define scheme (make-scheme environments evaluation))

(define initial-system
  (make-initial-system scheme (make-mini-command scheme)))
