; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Code-point classification

(define-enumerated-type primary-category :primary-category
  primary-category?
  primary-categories
  primary-category-name
  primary-category-index
  (letter
   number
   punctuation
   symbol
   mark
   separator
   miscellaneous))

(define-finite-type general-category :general-category
  (primary-category id)
  general-category?
  general-categories
  general-category-name
  general-category-index
  (primary-category general-category-primary-category)
  (id general-category-id)
  ((uppercase-letter (primary-category letter) "Lu")
   (lowercase-letter (primary-category letter) "Ll")
   (titlecase-letter (primary-category letter) "Lt")
   (modified-letter (primary-category letter) "Lm")
   (other-letter (primary-category letter) "Lo")

   (non-spacing-mark (primary-category mark) "Mn")
   (combining-spacing-mark (primary-category mark) "Mc")
   (enclosing-mark (primary-category mark) "Me")
   
   (decimal-digit-number (primary-category number) "Nd")
   (letter-number (primary-category number) "Nl")
   (other-number (primary-category number) "No")

   (opening-punctuation (primary-category punctuation) "Ps")
   (closing-punctuation (primary-category punctuation) "Pe")
   (initial-quote-punctuation (primary-category punctuation) "Pi")
   (final-quote-punctuation (primary-category punctuation) "Pf")
   (dash-punctuation (primary-category punctuation) "Pd")
   (connector-punctuation (primary-category punctuation) "Pc")
   (other-punctuation (primary-category punctuation) "Po")
   
   (currency-symbol (primary-category symbol) "Sc")
   (mathematical-symbol (primary-category symbol) "Sm")
   (modifier-symbol (primary-category symbol) "Sk")
   (other-symbol (primary-category symbol) "So")

   (space-separator (primary-category separator) "Zs")
   (paragraph-separator (primary-category separator) "Zp")
   (line-separator (primary-category separator) "Zl")

   (control-character (primary-category miscellaneous) "Cc")
   (formatting-character (primary-category miscellaneous) "Cf")
   (surrogate (primary-category miscellaneous) "Cs")
   (private-use-character (primary-category miscellaneous) "Co")
   (unassigned (primary-category miscellaneous) "Cn")))

(define (bits-necessary count)
  (let loop ((e 0)
	     (reached 1))
    (if (>= reached count)
	e
	(loop (+ e 1) (* 2 reached)))))

(define *general-category-bits*
  (bits-necessary (vector-length general-categories)))
   
(define (id->general-category id)
  (let ((count (vector-length general-categories)))
    (let loop ((i 0))
      (cond
       ((>= i count) #f)
       ((string=? (general-category-id (vector-ref general-categories i))
		  id)
	(vector-ref general-categories i))
       (else
	(loop (+ 1 i)))))))
