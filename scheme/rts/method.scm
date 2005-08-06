; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Generic procedure package

; This is written in fairly portable Scheme.  It needs:
;   Scheme 48 low-level macros (explicit renaming), in one small place.
;   (CALL-ERROR message proc arg ...)  - signal an error.
;   Record package and DEFINE-RECORD-TYPES macro.
;   An object :RECORD-TYPE which is the record type descriptor for
;     record type descriptors (record types are assumed to be records).
;     This wouldn't be difficult to change.
;   A RECORD? predicate (not essential - only for defining a DISCLOSE
;     method for records).

; --------------------
; Simple types.
; More specific types have higher priorities.  The priorities are used
; to establish the ordinary in which type predicates are called.

(define-record-type simple-type :simple-type
  (really-make-simple-type supers predicate priority id)
  simple-type?
  (supers simple-type-superiors)
  (predicate simple-type-predicate)
  (priority simple-type-priority)
  (id simple-type-id)
  (more))    ;if needed later

(define-record-discloser :simple-type
  (lambda (c) `(simple-type ,(simple-type-id c))))

(define (make-simple-type supers predicate id)
  (make-immutable!
   (really-make-simple-type supers
			    predicate
			    (compute-priority supers)
			    id)))

(define (compute-priority supers)
  (if (null? supers)
      0
      (+ (apply max (map %type-priority supers))
	 *increment*)))

(define *increment* 10)


; These two procedures will become generic later, but must exist early
; in order to be able to bootstrap the method definition mechanism.

(define (%type-priority type)
  (cond ((simple-type? type)
	 (simple-type-priority type))
	((record-type? type)
	 (record-type-priority type))
	(else (type-priority type))))	;generic

(define (%type-predicate type)
  (cond ((simple-type? type)
	 (simple-type-predicate type))
	((record-type? type)
	 (record-predicate type))
	(else (type-predicate type))))  ;generic

(define (%same-type? t1 t2)
  (or (eq? t1 t2)
      (if (simple-type? t1)
	  #f
	  (if (record-type? t1)
	      #f
	      (same-type? t1 t2)))))
	  

(define-syntax define-simple-type
  (syntax-rules ()
    ((define-simple-type ?name (?super ...) ?pred)
     (define ?name (make-simple-type (list ?super ...) ?pred '?name)))))

; --------------------
; Built-in Scheme types

(define-simple-type :syntax    () #f)
(define-simple-type :values    () #f)    ;any number of values

(define (value? x) #t)
(define-simple-type :value     (:values) value?)
(define-simple-type :zero      (:values) (lambda (x) #f))

(define-simple-type :number    (:value) number?)
(define-simple-type :complex   (:number) complex?)
(define-simple-type :real      (:complex) real?)
(define-simple-type :rational  (:real) rational?)
(define-simple-type :integer   (:rational) integer?)
(define-simple-type :exact-integer (:integer)
  (lambda (n) (and (integer? n) (exact? n))))

(define-simple-type :boolean   (:value) boolean?)
(define-simple-type :symbol    (:value) symbol?)
(define-simple-type :char      (:value) char?)
(define-simple-type :null      (:value) null?)
(define-simple-type :pair      (:value) pair?)
(define-simple-type :vector    (:value) vector?)
(define-simple-type :string    (:value) string?)
(define-simple-type :procedure (:value) procedure?)

(define-simple-type :input-port  (:value) input-port?)
(define-simple-type :output-port (:value) output-port?)
(define-simple-type :eof-object	 (:value) eof-object?)

; If there is no RECORD? predicate, do
;   (define-simple-type :record	 (:value) value?)
; and change the DISCLOSE method for records to
;   (or (disclose-record obj) (next-method)).

(define-simple-type :record	 (:value) record?)

; If record types are not records, un-comment the following line.
; (define-simple-type :record-type (:value) record-type?)

; Given a record type, RECORD-TYPE-PRIORITY returns its priority.
; Here we establish that every record type is a direct subtype of the
; :RECORD type.

(define record-type-priority
  (let ((r-priority
	 (simple-type-priority (make-simple-type (list :record) #f #f))))
    (lambda (rt) r-priority)))

; --------------------
; Method-info records are triples <type-list, n-ary?, proc>.

(define-record-type method-info :method-info
  (really-make-method-info types n-ary? proc)
  method-info?
  (types method-info-types)
  (n-ary? method-info-n-ary?)
  (proc method-info-proc))

(define (make-method-info types n-ary? proc)
  (make-immutable! (really-make-method-info types n-ary? proc)))

(define-record-discloser :method-info
  (lambda (info)
    `(method-info ,(method-info-types info) ,(method-info-n-ary? info))))

; --------------------
; Method lists

; A method list is a list of method-info records, sorted in order from
; most specific to least specific.

(define (empty-method-list) '())

; insert-method inserts an entry into a method list so that the most
; specific methods come earliest in the list.  The last method should
; be a default method or error signal(l)er.

(define (insert-method info ms)
  (let recur ((ms ms))
    (if (null? ms)
	(cons info ms)
	(if (more-specific? (car ms) info)
	    (cons (car ms) (recur (cdr ms)))
	    (cons info
		  (if (same-applicability? (car ms) info)
		      (cdr ms)
		      ms))))))

; Replace an existing method with identical domain.

(define (same-applicability? info1 info2)
  (and (every2 %same-type?
	       (method-info-types info1)
	       (method-info-types info2))
       (eq? (method-info-n-ary? info1) (method-info-n-ary? info2))))

(define (every2 pred l1 l2)
  (if (null? l1)
      (null? l2)
      (if (null? l2)
	  #f
	  (and (pred (car l1) (car l2)) (every2 pred (cdr l1) (cdr l2))))))

; This interacts with methods->perform, below.
; In this version, it's supposed to be a total order.

(define (more-specific? info1 info2)
  (let ((t1 (method-info-types info1))
	(t2 (method-info-types info2)))
    (let ((l1 (length t1))
	  (l2 (length t2))
	  (foo? (and (not (method-info-n-ary? info1))
		     (method-info-n-ary? info2))))
      (if (= l1 l2)
	  (or foo?
	      (let loop ((l1 t1)
			 (l2 t2))
		(if (null? l2)
		    #f
		    (or (more-specific-type? (car l1) (car l2))
			(and (%same-type? (car l1) (car l2))
			     (loop (cdr l1) (cdr l2)))))))
	  (and (> l1 l2)
	       foo?)))))
  

(define (more-specific-type? t1 t2)
  (> (%type-priority t1) (%type-priority t2)))

; --------------------
; A method table is a cell that contains a method list.
; Note that the method table is not reachable from the generic
; procedure.  This means good things for the GC.

(define-record-type method-table :method-table
  (really-make-method-table methods prototype
			    generic get-perform set-perform! id)
  method-table?
  (methods method-table-methods set-method-table-methods!)
  (prototype method-table-prototype)
  (generic make-generic)
  (get-perform method-table-get-perform)
  (set-perform! method-table-set-perform!)
  (id method-table-id))

(define-record-discloser :method-table
  (lambda (t) `(method-table ,(method-table-id t))))

(define (make-method-table id . option)
  (let* ((prototype (if (null? option)
			(make-method-info '() #t #f)
			(car option)))
	 (mtable (call-with-values make-cell-for-generic
		   (lambda (generic get-perform set-perform!)
		     (really-make-method-table '()
					       prototype
					       generic
					       get-perform
					       set-perform!
					       id)))))
    (set-final-method!
         mtable
	 (lambda (next-method . args)
	   (apply call-error "invalid or unimplemented operation"
		  id args)))
    mtable))

(define (make-cell-for-generic)
  (let ((perform #f))
    ;; PERFORM always caches (METHODS->PERFORM method-list prototype).
    (values (lambda args (perform args)) ;Generic proc
	    (lambda () perform)
	    (lambda (new) (set! perform new)))))

(define (add-to-method-table! mtable info)
  (let ((l (insert-method info (method-table-methods mtable))))
    (set-method-table-methods! mtable l)
    ((method-table-set-perform! mtable)
     (methods->perform l (method-table-prototype mtable)))))

(define (set-final-method! mtable proc)
  (add-to-method-table! mtable
			(make-method-info '()
					  #t
					  proc)))

(define (apply-generic mtable args)
  ;; (apply (make-generic mtable) args)
  (((method-table-get-perform mtable)) args)) ;+++

; DEFINE-GENERIC

(define-syntax define-generic
  (syntax-rules ()
    ((define-generic ?name ?mtable-name)
     (begin (define ?mtable-name (make-method-table '?name))
	    (define ?name (make-generic ?mtable-name))))
    ((define-generic ?name ?mtable-name (?spec . ?specs))
     (begin (define ?mtable-name
	      (make-method-table '?name
				 (method-info ?name ("next" next-method
							    ?spec . ?specs)
				   (next-method))))
	    (define ?name (make-generic ?mtable-name))))))

; --------------------
; Method combination.

; Here is the specification:

;(define (apply-generic mtable args)
;  (let loop ((ms (method-table-methods mtable)))
;    (let ((next-method (lambda () (loop (cdr ms)))))
;      (if (let test ((ts (method-info-types (car ms)))
;                     (args args))
;            (if (null? ts)
;                (or (null? args)
;                    (method-info-n-ary? (car ms)))
;                (and ((%type-predicate (car ts)) (car args))
;                     (test (cdr ts) (cdr args)))))
;          (apply (method-info-proc (car ms))
;                 next-method
;                 args)
;          (next-method)))))

;   (perform arg-list)
;   (apply proc next-method-thunk arg-list)

; This version of METHODS->PERFORM simply marches through all the
; methods, looking for one that handles the operation.

; The prototype is currently ignored, but it could be put to good use.

(define (methods->perform l prototype)
  (let recur ((l l))
    (let* ((info (car l))
	   (proc (method-info-proc info)))
      (if (null? (cdr l))
	  (last-action proc)
	  (one-action (argument-sequence-predicate info)
		      proc
		      (recur (cdr l)))))))

(define (last-action proc)
  (lambda (args)
    (apply proc #f args)))

(define (one-action pred proc perform-next)
  (lambda (args)
    (if (pred args)
	(apply proc
	       (lambda () (perform-next args))    ; next-method
	       args)
	(perform-next args))))

(define (argument-sequence-predicate info)
  (let recur ((types (method-info-types info)))
    (if (null? types)
	(if (method-info-n-ary? info) value? null?)
	(let ((pred (%type-predicate (car types)))
	      (check-rest (recur (cdr types))))
	  (if (eq? pred value?)
	      (check-for-next check-rest) ;+++
	      (check-next pred check-rest))))))

(define (check-for-next check-rest)
  (lambda (args)
    (if (null? args)
	#f
	(check-rest (cdr args)))))

(define (check-next pred check-rest)
  (lambda (args)
    (if (null? args)
	#f
	(if (pred (car args))
	    (check-rest (cdr args))
	    #f))))

; --------------------
; METHOD-INFO macro.
; Returns a method-info record.

; You can specify the name of the next-method parameter by saying
;   (method-info my-name (x y "next" n) body ...)
; Otherwise, the next-method parameter will be named next-method.
; Just pretend it's Dylan and that #next reads as "next".

(define-syntax method-info
  (syntax-rules ()
    ((method-info ?id ?formals ?body ...)
     (method-internal ?formals () () #f ?id ?body ...))))

(define-syntax method-internal
  (syntax-rules ()
    ((method-internal ((?formal1 ?type1) . ?specs)
		      (?formal ...) (?type ...) ?next
		      . ?rest)
     (method-internal ?specs
		      (?formal ... ?formal1) (?type ... ?type1) ?next
		      . ?rest))

    ((method-internal ("next" ?next . ?specs)
		      (?formal ...) (?type ...) ?ignore
		      . ?rest)
     (method-internal ?specs
		      (?formal ...) (?type ...) ?next
		      . ?rest))

    ((method-internal (?spec . ?specs)
		      (?formal ...) (?type ...) ?next
		      . ?rest)
     (method-internal ?specs
		      (?formal ... ?spec) (?type ... :value) ?next
		      . ?rest))

    ((method-internal ?rest
		      (?formal ...) (?type ...) ?next
		      ?id ?body ...)
     (make-method-info (list ?type ...)
		       (not (null? '?rest))
		       (let ((?id (with-next-method ?next (?formal ... . ?rest)
				    ?body ...)))
			 ;; The (let ...) is a hack for the Scheme 48
			 ;; byte code compiler, which will remember
			 ;; ?id as the procedure's name.  This should
			 ;; aid debugging a little bit since the name
			 ;; shows up in backtraces and the inspector.
			 ?id)))))

; Non-hygienic, a la Dylan

(define-syntax with-next-method
  (cons (lambda (e r c)
	  (let ((next (or (cadr e) 'next-method)))
	    `(,(r 'lambda) (,next ,@(caddr e))
			   ,@(cdddr e))))
	'(lambda)))

; DEFINE-METHOD macro.

(define-syntax define-method
  (syntax-rules ()
    ((define-method ?mtable ?formals ?body ...)
     (add-method! ?mtable
		  (method-info ?mtable ?formals ?body ...)))))

(define-generic add-method! &add-method! (mtable info))

(let ((info
       (method-info add-method! ((mtable :method-table) (info :method-info))
	 (add-to-method-table! mtable info))))
  (add-to-method-table! &add-method! info))

; --------------------
; Generic functions on types: sort of a meta-object protocol, huh?

(define-generic type-predicate &type-predicate (t))

(define-method &type-predicate ((t :record-type)) (record-predicate t))
(define-method &type-predicate ((t :simple-type)) (simple-type-predicate t))

(define-generic type-priority &type-priority (t))

(define-method &type-priority ((t :record-type)) (record-type-priority t))
(define-method &type-priority ((t :simple-type)) (simple-type-priority t))

(define-generic type-superiors &type-superiors (t))

(define-method &type-superiors ((t :record-type)) (list :record))
(define-method &type-superiors ((t :simple-type)) (simple-type-superiors t))


; Type equivalence

(define-generic same-type? &same-type? (t1 t2))

(define-method &same-type? (t1 t2) (eq? t1 t2))

(define-method &same-type? ((t1 :simple-type) (t2 :simple-type))
  (and (eq? (simple-type-predicate t1) (simple-type-predicate t2))
       (eq? (simple-type-id t1) (simple-type-id t2))))    ;?

; --------------------
; Singleton types.

(define-record-type singleton :singleton
  (singleton value)
  (value singleton-value))
  
(define-record-discloser :singleton
  (lambda (s) `(singleton ,(singleton-value s))))

(define (compare-to val)
  (lambda (x) (eqv? x val)))

(define-method &type-predicate ((s :singleton))
  (compare-to (singleton-value s)))

(define-method &type-priority ((s :singleton)) 1000000)

(define-method &same-type? ((s1 :singleton) (s2 :singleton))
  (eqv? (singleton-value s1) (singleton-value s2)))

; --------------------
; DISCLOSE

; A generic procedure for producing printed representations.
; Should return one of
;   - A list (symbol info ...), to be printed as #{Symbol info ...}
;   - #f, meaning no information available on how to print.
; This is intended to be used not only by write and display, but also by
; the pretty printer.

(define-generic disclose &disclose (x))

(define-method &disclose (obj) #f)

(define-method &disclose ((obj :record))
  (or (disclose-record obj)
      '(record)))

(define-method &add-method! ((d (singleton &disclose)) info)
  (let ((t (car (method-info-types info))))
    (if (record-type? t)
	(define-record-discloser t (proc->discloser (method-info-proc info)))
	(next-method))))

(define (proc->discloser proc)
  (lambda (arg)
    (proc (lambda () #f) arg)))

;(define-method &disclose ((s :singleton))
;  `(singleton ,(singleton-value s)))
