; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define (link-simple-system filename resumer-exp . structs)
  (link-system structs (lambda () resumer-exp) filename))

(define (link-reified-system some filename make-resumer-exp . structs)
  (link-system (append structs (map cdr some))
	       (lambda ()
		 (display "Reifying") (newline)
		 `(,make-resumer-exp
		   (lambda ()
		     ,(reify-structures some
					(lambda (loc) loc)
					`(lambda (loc) loc)))))
	       filename))


(define (link-system structs make-resumer filename)
  (for-each ensure-loaded structs)
  (let* ((p (make-simple-package structs eval #f))
	 (r (eval (make-resumer) p)))
    ;; (check-package p)
    r))

;(define (check-package p)
;  (let ((names (undefined-variables p)))
;    (if (not (null? names))
;        (begin (display "Undefined: ") 
;               (write names) (newline)))))

(define-syntax struct-list
  (syntax-rules ()
    ((struct-list name ...) (list (cons 'name name) ...))))
