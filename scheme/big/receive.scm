; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define-syntax receive
  (syntax-rules ()
    ((receive ?vars ?producer . ?body)
     (call-with-values (lambda () ?producer)
       (lambda ?vars . ?body)))))
