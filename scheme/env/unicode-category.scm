; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

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
  (primary-category id symbol)
  general-category?
  general-categories
  general-category-name
  general-category-index
  (primary-category general-category-primary-category)
  (id general-category-id)
  (symbol general-category-symbol)
  ((uppercase-letter (primary-category letter) "Lu" 'Lu)
   (lowercase-letter (primary-category letter) "Ll" 'Ll)
   (titlecase-letter (primary-category letter) "Lt" 'Lt)
   (modified-letter (primary-category letter) "Lm" 'Lm)
   (other-letter (primary-category letter) "Lo" 'Lo)

   (non-spacing-mark (primary-category mark) "Mn" 'Mn)
   (combining-spacing-mark (primary-category mark) "Mc" 'Mc)
   (enclosing-mark (primary-category mark) "Me" 'Me)
   
   (decimal-digit-number (primary-category number) "Nd" 'Nd)
   (letter-number (primary-category number) "Nl" 'Nl)
   (other-number (primary-category number) "No" 'No)

   (opening-punctuation (primary-category punctuation) "Ps" 'Ps)
   (closing-punctuation (primary-category punctuation) "Pe" 'Pe)
   (initial-quote-punctuation (primary-category punctuation) "Pi" 'Pi)
   (final-quote-punctuation (primary-category punctuation) "Pf" 'Pf)
   (dash-punctuation (primary-category punctuation) "Pd" 'Pd)
   (connector-punctuation (primary-category punctuation) "Pc" 'Pc)
   (other-punctuation (primary-category punctuation) "Po" 'Po)
   
   (currency-symbol (primary-category symbol) "Sc" 'Sc)
   (mathematical-symbol (primary-category symbol) "Sm" 'Sm)
   (modifier-symbol (primary-category symbol) "Sk" 'Sk)
   (other-symbol (primary-category symbol) "So" 'So)

   (space-separator (primary-category separator) "Zs" 'Zs)
   (paragraph-separator (primary-category separator) "Zp" 'Zp)
   (line-separator (primary-category separator) "Zl" 'Zl)

   (control-character (primary-category miscellaneous) "Cc" 'Cc)
   (formatting-character (primary-category miscellaneous) "Cf" 'Cf)
   (surrogate (primary-category miscellaneous) "Cs" 'Cs)
   (private-use-character (primary-category miscellaneous) "Co" 'Co)
   (unassigned (primary-category miscellaneous) "Cn" 'Cn)))

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
