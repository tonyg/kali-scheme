; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


(define (load-configuration filename)
  (let-fluid $source-file-name filename
    (lambda () (load filename))))

(define-syntax structure-ref
  (syntax-rules ()
    ((structure-ref ?struct ?name)
     (*structure-ref ?struct '?name))))
