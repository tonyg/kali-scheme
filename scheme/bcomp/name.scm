; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Names (symbols) and generated names.

(define (name? thing)
  (or (symbol? thing)
      (generated? thing)))

; Generated names

; Generated names make lexically-scoped macros work.  They're the same
; as what Alan Bawden and Chris Hanson call "aliases".  The parent
; field is always another name (perhaps generated).  The parent chain
; provides an access path to the name's binding, should one ever be
; needed.  That is: If name M is bound to a transform T that generates
; name G as an alias for name N, then M is (generated-parent-name G),
; so we can get the binding of G by accessing the binding of N in T's
; environment of closure, and we get T by looking up M in the
; environment in which M is *used*.

(define-record-type generated :generated
  (make-generated name token env parent-name)
  generated?
  (name        generated-name)
  (token       generated-token)
  (env	       generated-env)
  (parent-name generated-parent-name))

(define-record-discloser :generated
  (lambda (name)
    (list 'generated (generated-name name) (generated-uid name))))

(define (generate-name name env parent-name)    ;for opt/inline.scm
  (make-generated name (cons #f #f) env parent-name))

(define (generated-uid generated-name)
  (let ((token (generated-token generated-name)))
    (or (car token)
	(let ((uid *generated-uid*))
	  (set! *generated-uid* (+ *generated-uid* 1))
	  (set-car! token uid)
	  uid))))

(define *generated-uid* 0)

(define (name->symbol name)
  (if (symbol? name)
      name
      (string->symbol (string-append (symbol->string
				       (name->symbol (generated-name name)))
				     "##"
				     (number->string (generated-uid name))))))

(define (name-hash name)
  (cond ((symbol? name)
	 (string-hash (symbol->string name)))
	((generated? name)
	 (name-hash (generated-name name)))
	(else
	 (assertion-violation 'name-hash "invalid name" name))))

(define make-name-table
  (make-table-maker eq? name-hash))

; Used by QUOTE to turn generated names back into symbols

(define (desyntaxify thing)
  (cond ((or (boolean? thing) (null? thing) (number? thing)
	     (symbol? thing) (char? thing))
	 thing)
	((string? thing)
	 (make-immutable! thing))
	((generated? thing)
	 (desyntaxify (generated-name thing)))
	((pair? thing)
	 (make-immutable!
	  (cons (desyntaxify (car thing))
		(desyntaxify (cdr thing)))))
	((vector? thing)
	 (make-immutable!
	  (let ((new (make-vector (vector-length thing) #f)))
	    (let loop ((i 0))
	      (if (>= i (vector-length thing))
		  new
		  (begin
		    (vector-set! new i (desyntaxify (vector-ref thing i)))
		    (loop (+ i 1))))))))
	(else
	 (warning 'desyntaxify "invalid datum in quotation" thing)
	 thing)))

;----------------
; Qualified names
; 
; A qualified name is a generated name that has been translated into a path.
; For example, if syntax A introduces a reference to procedure B, then the
; reference to B, as a qualified name, will be #(>> A B).  If B refers to
; C and is substituted in-line, then the reference to C is #(>> #(>> A B) C).
; The binding for C can be located by going to the structure which supplies A,
; finding where it gets B from, and then looking up C there.

; These can't be records because they are included in linked images.

(define (make-qualified transform-name sym uid)
  (vector '>> transform-name sym uid))

(define (qualified? thing)
  (and (vector? thing)
       (= (vector-length thing) 4)
       (eq? (vector-ref thing 0) '>>)))

(define (qualified-parent-name q) (vector-ref q 1))
(define (qualified-symbol q) (vector-ref q 2))
(define (qualified-uid q) (vector-ref q 3))

  


