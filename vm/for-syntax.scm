; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


(define (concatenate-symbol . syms)
  (string->symbol (apply string-append (map symbol->string syms))))


