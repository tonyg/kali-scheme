; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Miniature package system.  This links mini-eval up to the output of
; the package reifier.

(define (package names locs get-location uid) ;Reified package
  (lambda (name)
    (let loop ((i (- (vector-length names) 1)))
      (if (< i 0)
	  (error "unbound" name)
	  (if (eq? name (vector-ref names i))
	      (contents (get-location (vector-ref locs i)))
	      (loop (- i 1)))))))

(define (make-simple-package opens foo1 foo2 name)
  
  (define bindings
    (list (cons '%%define%%
		(lambda (name val)
		  (set! bindings (cons (cons name val) bindings))))))

  (lambda (name)
    (let ((probe (assq name bindings)))
      (if probe
	  (cdr probe)
	  (let loop ((opens opens))
	    (if (null? opens)
		(error "unbound" name)
		(if (memq name (structure-interface (car opens)))
		    ((structure-package (car opens)) name)
		    (loop (cdr opens)))))))))

; Structures

(define (make-structure package interface . name-option)
  (cons package (vector->list interface)))

(define structure-interface cdr)
(define structure-package car)


; Things used by reification forms

(define (operator name type-exp)
  `(operator ,name ,type-exp))

(define (simple-interface names type) names)

; Etc.

(define (transform . rest) (cons 'transform rest))
(define (usual-transform . rest)
  (cons 'usual-transform rest))

(define (transform-for-structure-ref . rest)
  (cons 'transform-for-structure-ref rest))
(define (inline-transform . rest)
  (cons 'inline-transform rest))
(define (primop . rest)
  (cons 'primop rest))

(define (package-define-static! package name op) 'lose)

; --------------------
; ???

; (define (integrate-all-primitives! . rest) 'lose)

;(define (package-lookup p name)
;  ((p '%%lookup-operator%%) name))

;(define (package-ensure-defined! p name)
;  (package-define! p name (make-location 'defined name)))

