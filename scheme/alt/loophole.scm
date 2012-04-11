; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


(define-syntax loophole
  (syntax-rules ()
    ((loophole ?type ?form)
     (begin (lambda () ?type)    ;Elicit unbound-variable warnings, etc.
	    ?form))))
    
