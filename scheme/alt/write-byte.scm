; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; It's the best shot, given R5RS.

(define (set-port-crlf?! port val)
  (values))

(define (write-byte byte port)
  (write-char (ascii->char byte) port))
  
