; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; The following several packages have Scheme-implementation-specific 
; variants that are much better for one reason or another than
; the generic versions defined here.

(define-package ((signals signals-interface)
		 (handle (export ignore-errors))
		 (features features-interface))
  (open scheme-level-2)
  (files (alt features)))

(define-package ((records records-interface))
  (open scheme-level-2 signals)
  (files (alt record)))

(define-package ((ascii (export ascii->char char->ascii)))
  (open scheme-level-2 signals)
  (files (alt ascii)))

(define-package ((bitwise bitwise-interface))
  (open scheme-level-2 signals)
  (files (alt bitwise)))

(define-package ((code-vectors code-vectors-interface))
  (open scheme-level-1)
  (files (alt code-vectors)))

