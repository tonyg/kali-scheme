; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

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
	 (error "invalid name" name))))

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
	  (let ((x (desyntaxify (car thing)))
		(y (desyntaxify (cdr thing))))
	    (if (and (eq? x (car thing))
		     (eq? y (cdr thing)))
		thing
		(cons x y)))))
	((vector? thing)
	 (make-immutable!
	  (let ((new (make-vector (vector-length thing) #f)))
	    (let loop ((i 0) (same? #t))
	      (if (>= i (vector-length thing))
		  (if same? thing new)
		  (let ((x (desyntaxify (vector-ref thing i))))
		    (vector-set! new i x)
		    (loop (+ i 1)
			  (and same? (eq? x (vector-ref thing i))))))))))
	(else
	 (warn "invalid datum in quotation" thing)
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

; Convert an alias (generated name) to S-expression form ("qualified name").

(define (name->qualified name env)
  (cond ((not (generated? name))
	 name)
	((let ((d0 (lookup env name))
	       (d1 (lookup env (generated-name name))))
	   (and d0 d1 (same-denotation? d0 d1)))
	 (generated-name name))   ;+++
	(else
	 (make-qualified (qualify-parent (generated-parent-name name)
					 env)
			 (generated-name name)
			 (generated-uid name)))))
	 
; As an optimization, we elide intermediate steps in the lookup path
; when possible.  E.g.
;     #(>> #(>> #(>> define-record-type define-accessors)
;		define-accessor)
;	   record-ref)
; is replaced with
;     #(>> define-record-type record-ref)
;
; I think that this is buggy.  The RECUR calls are using the wrong environment.
; ENV is not the environment in which the names will be looked up.

(define (qualify-parent name env)
  (let recur ((name name))
    (if (generated? name)
	(let ((parent (generated-parent-name name)))
	  (if (let ((b1 (lookup env name))
		    (b2 (lookup env parent)))
		(and b1
		     b2
		     (or (same-denotation? b1 b2)
			 (and (binding? b1)
			      (binding? b2)
			      (let ((s1 (binding-static b1))
				    (s2 (binding-static b2)))
				(and (transform? s1)
				     (transform? s2)
				     (eq? (transform-env s1)
					  (transform-env s2))))))))
	      (recur parent) ;+++
	      (make-qualified (recur parent)
			      (generated-name name)
			      (generated-uid name))))
	name)))
  


