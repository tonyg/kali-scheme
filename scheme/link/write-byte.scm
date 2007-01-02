; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; It's the best shot, given R5RS.  It doesn't work in the Unicode
;; world, however, so it's only loaded when we're linking with an old
;; version of Scheme 48.

(define (write-byte byte port)
  (write-char (ascii->char byte) port))
  
