; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; Cf. alt/config.scm

(define (load-configuration filename . rest)
  (let ((save filename))
    (dynamic-wind (lambda () (set! *source-file-name* filename))
		  (lambda ()
		    (apply load filename rest))
		  (lambda () (set! *source-file-name* save)))))
(define (%file-name%) *source-file-name*)
(define *source-file-name* "")


; ?

(define-syntax structure-ref
  (syntax-rules ()
    ((structure-ref ?struct ?name)
     (*structure-ref ?struct '?name))))
