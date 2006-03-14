; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-enumeration area-type-size
  (small large weaks))

; at least one of these needs to be synchronized with what the linker writes
(define-enumeration image-format
  (two-space bibop)) 

(define (valid-image-format? f)
  (enum-case image-format f
    ((two-space bibop) #t)
    (else #f)))
