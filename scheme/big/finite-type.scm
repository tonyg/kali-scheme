; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Finite types (i.e. record types with a fixed set of elements).
;
; An enumeration is really a special case of a finite type.
;
; (define-finite-type <id and dispatch-macro>
;   <type name>
;   (<constructor field name> ...)
;   <predicate>
;   <vector of elements>
;   <name accessor>
;   <index accessor>
;   (<field name> <field accessor> [<field setter>])
;   ...
;   ((<element name> <constructor field value> ...)
;    ...))
;
; This is equivalent to
;
; (define-record-type <id and dispatch-macro>
;   <type name>
;   (maker name index <constructor field name> ...)
;   <predicate>
;   (name <name accessor>)
;   (index <index accessor>)
;   (<field name> <field accessor> [<field setter>])
;   ...)
;
; (define <vector of elements>
;   (vector (maker <element name> 0 <constructor field value>)
;           (maker <element name> 1 <constructor field value>)
;           ...))
;
; (define-dispatch-macro <id and dispatch-macro>
;   (<element name> ...)
;   <vector-of-elements>)
;
; where DEFINE-DISPATCH-MACRO defines an ENUM-like dispatcher.
;
;(define-finite-type foo :foo       ; id and type
;  foo?                             ; predicate
;  foo-elements                     ; vector containing all elements
;  foo-name                         ; name accessor
;  foo-index                        ; index accessor
;  (color foo-color set-foo-color!) ; any additional fields
;  ((name color)                    ; element pattern
;   (a 'red)                        ; the elements themselves
;   (b 'green)
;   (c 'puce)
;   (d 'taupe)))
;
; (foo a) -> #{foo a}

(define-syntax define-finite-type
  (lambda (form rename compare)
    (let ((destruct (lambda (proc)
		      (apply proc (cdr form))))
	  (%define-record-type (rename 'define-record-type))
	  (%define-record-discloser (rename 'define-record-discloser))
	  (%define (rename 'define))
	  (%begin (rename 'begin))
	  (%lambda (rename 'lambda))
	  (%vector (rename 'vector))
	  (%list (rename 'list))
	  (%define-dispatch (rename 'define-dispatch))
	  (%make-immutable! (rename 'make-immutable!))
	  (%maker (rename 'maker))
	  (%name (rename 'name))
	  (%index (rename 'index))
	  (%blah (rename 'blah)))
      (destruct (lambda (foo :foo pattern foo? foo-elements foo-name foo-index
			 . more)
		  (let* ((fields (do ((more more (cdr more))
				      (fields '() (cons (car more) fields)))
				     ((or (null? more)
					  (pair? (caar more)))
				      (reverse fields))))
			 (elts (car (reverse more)))
			 (names (map car elts)))
		    `(,%begin
		      (,%define-record-type ,foo ,:foo
			  (,%maker ,%name ,%index . ,pattern)
			  ,foo?
			  (,%name ,foo-name)
			  (,%index ,foo-index)
			  . ,fields)
		      (,%define-record-discloser ,:foo
			  (,%lambda (,%blah)
			      (,%list ',foo (,foo-name ,%blah))))
		      (,%define ,foo-elements
			  (,%make-immutable!
			    (,%vector . ,(do ((elts elts (cdr elts))
				  	      (i 0 (+ i 1))
					      (res '() `((,%maker ',(caar elts)
								  ,i
								  . ,(cdar elts))
							 . ,res)))
					     ((null? elts)
					      (reverse res))))))
		      (,%define-dispatch ,foo ,names ,foo-elements)))))))
  (define-record-type define-record-discloser define-dispatch
   define begin lambda vector list))

; (define-dispatch <name> (<member name> ...) <vector of members>)
;
; This defines <name> to be a macro (<name> X) that looks X up in
; the list of member names and returns the corresponding element of
; <vector of members>.
;
; (define-dispatch foo (a b c) members)
; (foo b) -expands-into-> (vector-ref members 1)

(define-syntax define-dispatch
  (lambda (form0 rename0 compare0)
    (let ((name (cadr form0))
	  (names (caddr form0))
	  (elts (cadddr form0))
	  (%vector-ref (rename0 'vector-ref))
	  (%code-quote (rename0 'code-quote)))
      `(define-syntax ,name
	 (lambda (form1 rename1 compare1)
	   (let ((elt (cadr form1)))
	     (let loop ((names ',names) (i 0))
	       (cond ((null? names)
		      form1)
		     ((compare1 elt (car names))
		      (list (,%code-quote ,%vector-ref) (rename1 ',elts) i))
		     (else
		      (loop (cdr names) (+ i 1))))))))))
  (vector-ref code-quote))

; (define-enumerated-type <id and dispatch-macro>
;   <type name>
;   <predicate>
;   <vector of elements>
;   <name accessor>
;   <index accessor>
;   (<element name> ...))
;
; This is a simplified version that has no additional fields.  It avoids
; a lot of unnecessary parens around the element names.
;
; The above expands into:
;
; (define-finite-type <id and dispatch-macro>
;   <type name>
;   <predicate>
;   <vector of elements>
;   <name accessor>
;   <index accessor>
;   ((name)
;    (<element name>)
;    ...))

(define-syntax define-enumerated-type
  (syntax-rules ()
    ((define-enumerated-type id type-name predicate elements
       name-accessor index-accessor
       (element ...))
     (define-finite-type     id type-name () predicate elements
       name-accessor index-accessor
       ((element) ...)))))

