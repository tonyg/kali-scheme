
; no copyright please, silly shell script

(define *annotations* '()

(define (annotate-procedure proc ann)
  (let ((new (lambda args (apply proc args))))
    (set! *annotations* (cons (cons new ann) *annotations*))
    new))

(define (procedure-annotation proc)
  (cond ((assq proc *annotations*) => cdr)
	(else #f)))
