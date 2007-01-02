; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Dilapidated Object-Oriented Dynamic Language
; Dynamic Object-Oriented Dynamic Language
; Drug-crazed Object-Oriented Dynamic Language

; Written for clarity, not for speed.

; Tests are in test-doodl.scm.

(define <object>     :value)
(define <number>     :number)
(define <complex>    :complex)
(define <real>	     :real)
(define <rational>   :rational)
(define <integer>    :integer)
(define <pair>	     :pair)
(define <symbol>     :symbol)
(define <char>	     :char)
(define <null>	     :null)
(define <vector>     :vector)
(define <string>     :string)
(define <eof-object> :eof-object)
(define <function>   :procedure)
(define <input-port> :input-port)
(define <output-port> :output-port)


; --------------------
; Generic functions

(define method-table? (type-predicate :method-table))

(define-syntax define-generic-function
  (syntax-rules (setter)
    ((define-generic-function (setter ?name) ?parameter-list) ;for define-slot
     (define-setter ?name
       (make-generic-function
	    '(the-setter ?name)
	    (method-info ?name ("next" next-method . ?parameter-list)
	      (next-method)))))
    ((define-generic-function ?name ?parameter-list)
     (define ?name
       (make-generic-function
	    '?name
	    (method-info ?name ("next" next-method . ?parameter-list)
	      (next-method)))))))
	
(define (make-generic-function id prototype)
  (let ((mtable (make-method-table id prototype)))
    (annotate-procedure (make-generic mtable) mtable)))

(define (generic-function? f)
  (and (procedure? f)
       (method-table? (procedure-annotation f))))

(define-simple-type <generic-function> (<function>) generic-function?)

(really-define-method &add-method! ((g <generic-function>) foo)
  (add-method! (procedure-annotation g) foo))

(really-define-method &disclose ((g <generic-function>))
  `(generic-function ,(method-table-id (procedure-annotation g))))

(define method-table-id (record-accessor :method-table 'id))

; --------------------
; Method info (applicability / action pairs)

; D***n-style METHOD syntax

(define-syntax method
  (syntax-rules ()
    ((method ?specs ?body ...)
     (make-method (method-info anonymous ?specs ?body ...)))))

(define method-table-methods (record-accessor :method-table 'methods))

(define (make-method info)
  (letrec ((perform (methods->perform
		      (list info
			    (method-info method args
			      (apply call-error "invalid arguments" m args)))
		      ;; This oughta be a prototype
		      #f))
	   (m (annotate-procedure (lambda args (perform args))
				  info)))
    m))

(define method-info? (record-predicate :method-info))

(define (method? f)
  (and (procedure? f)
       (method-info? (procedure-annotation f))))

(define-simple-type <method> (<function>) method?)

(really-define-method &disclose ((m <method>))
  `(method ,(procedure-annotation m)))

(define-syntax define-method
  (syntax-rules (setter)
    ((define-method (setter ?id) ?formals ?body ...)
     (really-define-setter-method ?id ?formals 'bar ?body ...))
    ((define-method ?id ?formals ?body ...)
     (really-define-method ?id ?formals 'foo ?body ...))))

(define-syntax really-define-setter-method
  (lambda (e r c)
    `(,(r 'really-define-method)
      ,(string->symbol (string-append (symbol->string (cadr e))
				      "-"
				      (symbol->string 'setter)))
      ,@(cddr e))))

; --------------------
; (SETTER foo)

(define-syntax the-setter
  (lambda (e r c)
    (string->symbol (string-append (symbol->string (cadr e))
				   "-"
				   (symbol->string 'setter)))))

(define-syntax define-setter
  (lambda (e r c)
    `(,(r 'define)
      ,(string->symbol (string-append (symbol->string (cadr e))
				      "-"
				      (symbol->string 'setter)))
      ,(caddr e))))

(define-syntax set
  (syntax-rules ()
    ((set (?fun ?arg ...) ?val)
     ((the-setter ?fun) ?arg ... ?val))
    ((set ?var ?val)
     (set! ?var ?val))))

(define car-setter set-car!)
(define cdr-setter set-cdr!)
(define vector-ref-setter vector-set!)

; --------------------
; DEFINE-CLASS

(define-syntax define-class
  (syntax-rules ()
    ((define-class ?class-name (?super ...) ?slot ...)
     (begin (define-slot ?slot)
	    ...
	    (define ?class-name
              (make-class (list ?super ...)
			  (list ?slot ...)
                          '?class-name))))))

(define-syntax define-slot
  (syntax-rules ()
    ((define-slot ?slot)
     (begin (define-generic-function ?slot (x))
	    (define-generic-function (setter ?slot) (x new-val))
	    (define-method ?slot ((x <instance>))
	      (instance-slot-ref x ?slot))
	    (define-setter-method ?slot ((x <instance>) new-val)
	      (instance-slot-set! x ?slot new-val))))))

(define-syntax define-setter-method
  (lambda (e r c)
    `(,(r 'define-method)
      ,(string->symbol (string-append (symbol->string (cadr e))
				      "-"
				      (symbol->string 'setter)))
      ,@(cddr e))))

; Instances

(define-record-type instance <instance>
  (make-instance classes slots)
  instance?
  (classes instance-classes)
  (slots instance-slot-values))

(define (instance-slot-ref instance slot)
  (cond ((assq slot (instance-slot-values instance)) => cdr)
	(else (call-error "no such slot"
			  instance-slot-ref instance slot))))

(define (instance-slot-set! instance slot new-value)
  (cond ((assq slot (instance-slot-values instance))
	 => (lambda (z) (set-cdr! z new-value)))
	(else (call-error "no such slot"
			  instance-slot-set! instance slot new-value))))

; Classes

(define-record-type class <class>
  (really-make-class classes predicate priority slots id)
  class?
  (classes class-classes)
  (predicate class-predicate)
  (priority class-priority)
  (slots class-slots)
  (id class-id))

(define-record-discloser <class>
  (lambda (c) `(class ,(class-id c))))

(really-define-method &type-predicate ((c <class>)) (class-predicate c))
(really-define-method &type-priority ((c <class>)) (class-priority c))

(define (make-class supers slots id)
  (letrec ((class
	    (really-make-class
	         (reduce unionq '() (map get-classes supers))
		 (lambda (x)		;Predicate
		   (and (instance? x)
			(memq class (instance-classes x))))
		 (if (null? supers)	;Priority
		     (type-priority <instance>)
		     (+ (apply max (map type-priority supers))
			*increment*))
		 (unionq slots
			 (reduce unionq '() (map get-slots supers)))
		 id)))
    class))

(define *increment* 10)

(define (get-classes type)
  (if (class? type)
      (cons type
	    (class-classes type))
      '()))

(define (get-slots type)
  (if (class? type)
      (class-slots type)
      '()))

(define-generic-function make (class . key/value-pairs))

(define-method make ((c <class>) . key/value-pairs)
  (let ((i (make-instance (cons c (class-classes c))
			  (map (lambda (slot)
				 (cons slot '*uninitialized*))
			       (class-slots c)))))
    (apply initialize i key/value-pairs)
    i))

(define-generic-function initialize (i . key/value-pairs))

(define-method initialize ((i <instance>)) (unspecific))


(define (unionq l1 l2)
  (cond ((null? l1) l2)
	((null? l2) l1)
	((memq (car l1) l2) (unionq (cdr l1) l2))
	(else (cons (car l1) (unionq (cdr l1) l2)))))

; --------------------
; Random

(define id? eq?)

(define-syntax bind
  (lambda (e r c)
    (if (and (pair? (cdr e))
	     (list? (cadr e)))
	(let ((%call-with-values (r 'call-with-values))
	      (%lambda (r 'lambda))
	      (%method (r 'method))
	      (%begin (r 'begin)))
	  (let recur ((specs (cadr e)))
	    (if (null? specs)
		`(,%begin ,@(cddr e))
		(let ((rspec (reverse (car specs))))
		  `(,%call-with-values
		    (,%lambda () ,(car rspec))
		    (,%method ,(reverse (cdr rspec))
			      ,(recur (cdr specs))))))))
	e)))

(define-simple-type <list> (<object>) list?)

; --------------------
; More?

; (instance? obj class)
; (as class object) => instance

; <type>
; (union type1 type2)
; (union* type ...)
; (subtype? type1 type2 )
;   per design note 05

; (define-method foo (x y #values (foo <integer>)) ...)
;   per design note 21

; (define-method f ((x (limited <integer> min: -1000 max: 1000)) ...) 
;   ...)
;   design note 06

; <collection>, etc.


; <exact> and <inexact> ?

;(define <sequence>
;  (make-generalization (list <list> <vector> <string>) '<sequence>))

;(define <port>
;  (make-generalization (list <input-port> <output-port>) '<port>))


; Need reader syntax:
;   #next #rest #key   etc.
;     - implement with (define-sharp-macro #\n ...) ?
;   keywords - foo:  
;     - implement by customizing parse-token
