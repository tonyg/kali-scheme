; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Structure reification.

(define *least* #f)

(define (reify-structures some)
  (let* ((count 0)
	 (least 1000000)
	 (greatest -1000000)
	 (locs (make-table))
	 (loser (reify-structures-1 some
				    (lambda (loc)
				      (let ((id (location-id loc)))
					(if (not (table-ref locs id))
					    (begin 
					      (if (< id least)
						  (set! *least* loc))
					      (set! least (min least id))
					      (set! greatest (max greatest id))
					      (set! count (+ count 1))
					      (table-set! locs id loc)))
					id))))
	 (size (+ (- greatest least) 1)))
    (write `(least ,least size ,size count ,count)) (newline)
    (values loser
	    (let ((v (make-vector size #f)))
	      (table-walk (lambda (id loc)
			    (vector-set! v (- id least) loc))
			  locs)
	      v)
	    least)))


; This is pretty gross.  We really want some kind of object dumper
; instead.

(define *objects* '())          ;List of (object . creation-form)
(define *object-count* 0)
(define *initializations* '())
(define *deal-with-location* (lambda (loc) loc))
(define *package-table* #f)     ;Entries are package-info structures


; REIFY-STRUCTURES returns a form that evaluates to a procedure that
; returns an alist of (name . structure).  deal-with-location is a
; procedure that maps locations to labels for them (e.g. integers).
; The procedure takes one argument, a procedure that will be applied
; to the labels at startup time to re-obtain the corresponding
; locations.

(define (reify-structures-1 alist deal-with-location)
  (flush-state)
  (set! *deal-with-location* deal-with-location)

  (display "Reifying") (force-output (current-output-port))

  (let* ((result-form (reify-object alist))
	 (shebang
	  `(lambda (get-location)
	     (let ((the-objects (make-vector ,*object-count* #f)))
	       (begin ,@(map (lambda (init) (init))
			     (reverse *initializations*)))
	       (let ((structs ,result-form))
		 (set! the-objects #f)	;SO IT CAN BE GC'D
		 (set! get-location #f)
		 structs)))))
    (newline)
    (if *reify-debug* (*reify-debug* shebang))

    (flush-state)
    (set! *deal-with-location* (lambda (loc) loc))

    shebang))

(define (flush-state)
  (set! *objects* '())
  (set! *object-count* 0)
  (set! *initializations* '())
  (set! *package-table* (make-table package-uid)))


; Return an expression that will evaluate to thing.

(define (reify-object thing)
  (cond ((structure? thing)
	 (let ((p-form (reify-package (structure-package thing))))
	   (process-one-object
	        thing
		(lambda ()
		  `(make-structure
		    ,p-form
		    ,(interface-expression thing)
		    ',(structure-name thing)))
		(lambda ()
		  (process-exports thing p-form)
		  (write-char #\. (current-output-port))
		  (force-output (current-output-port))))))
	((null? thing) ''())
	((pair? thing)
	 (if (list? thing)
	     `(list ,@(map reify-object thing))
	     `(cons ,(reify-object (car thing))
		    ,(reify-object (cdr thing)))))
	((symbol? thing)
	 `',thing)
	((transform? thing)
	 (process-transform thing))
	((operator? thing)
	 `(operator ',(operator-name thing)
		    ',(type->sexp (operator-type thing) #t)))
	((primop? thing)
	 `(primop ',(primop-name thing)))
	;; ((interface? thing) ...)
	(else (error "don't know how to reify this" thing))))

(define (reify-package thing)
  (process-one-object thing
		      (lambda ()
			(let ((bindings (package-info-bindings (package-info thing))))
			  `(package
			    ;; Each binding is a pair (name . loc)
			    ',(list->vector (map car bindings)) ;names
			    ',(list->vector (map cdr bindings)) ;location ids
			    get-location
			    ,(package-uid thing))))
		      (lambda ()
			(table-set! *package-table*
				    thing
				    (make-package-info)))))


; General utility for uniquifying objects.

(define (process-one-object obj make-creation-form when-new)
  (let ((probe (assq obj *objects*)))
    (if probe
	(cdr probe)
	(let* ((index *object-count*)
	       (form `(vector-ref the-objects ,index)))
	  (set! *object-count* (+ *object-count* 1))
	  (set! *objects*
		(cons (cons obj form) *objects*))
	  (add-initialization!
	    (lambda ()
	      `(vector-set! the-objects ,index ,(make-creation-form))))
	  (when-new)
	  form))))

(define (add-initialization! thunk)
  (set! *initializations*
	(cons thunk *initializations*)))


; Add initializers that will create a structure's exported bindings.

(define (process-exports struct p-form)
  (let* ((package (structure-package struct))
	 (info (package-info package)))
    (for-each-export (lambda (name want-type binding)
		       (if (not (process-one-binding name package info p-form))
			   (warn "undefined export" name package)))
		     struct)))


; Packages...

(define package-info-type
  (make-record-type 'reify-info
		    '(bindings  ;List of (name static-info location)
		      table)))  ;Caches (assq? name bindings)

(define (package-info package)
  (table-ref *package-table* package))

(define make-package-info
  (let ((make (record-constructor package-info-type
				  '(bindings table))))
    (lambda ()
      (make '()
	    (make-name-table)))))

(define package-info-bindings (record-accessor package-info-type 'bindings))
(define package-info-table    (record-accessor package-info-type 'table))

(define set-package-info-bindings!
  (record-modifier package-info-type 'bindings))


(define (process-one-binding name package info p-form)	; => #t iff bound
  (let ((table (package-info-table info)))
    (if (table-ref table name)
	#t
	(let ((binding (package-lookup package name)))
	  (table-set! (package-info-table info) name #t)
	  (if (binding? binding)
	      (begin (really-process-one-binding name info binding p-form)
		     #t)
	      #f)))))

(define (really-process-one-binding name info binding p-form)
  (let ((static (binding-static binding))
	(loc (*deal-with-location* (binding-place binding))))
    (set-package-info-bindings!
         info
	 (cons (cons name loc)
	       (package-info-bindings info)))
    (if static
	(add-package-define! p-form name (reify-object static)))))

(define (add-package-define! p-form name s-form)
  (add-initialization!
   (lambda ()
     `(package-define-static! ,p-form
			      ',name
			      ,s-form))))

(define (process-transform transform)
  (let ((name (transform-id transform))
	(env (transform-env transform)))
    (let ((env-form
	   (if (package? env)
	       (reify-package env)
	       (reify-object env))))
      (process-one-object
       transform
       (let ((source (transform-source transform)))
	 (lambda ()
	   `(transform ,source		;transformer
		       ,env-form
		       ',(type->sexp (transform-type transform) #t) ;type
		       #f		;',source  -- omitted to save space...
		       ',name)))
       (if (package? env)
	   (lambda ()
	     (let ((info (package-info env)))
	       (for-each (lambda (name)
			   (process-one-binding name env info env-form))
			 (or (transform-aux-names transform) ; () must be true
			     (begin
			       (warn "reified macro's auxiliary bindings are unknown"
				     name)
			       '())))))
	   (lambda () #f))))))


(define (interface-expression struct)
  (let ((names '())
	(types '()))
    (for-each-export (lambda (name type binding)
		       (set! names (cons name names))
		       (set! types (cons (if (eq? type undeclared-type)
					     ':undeclared
					     (type->sexp type #t))
					 types)))
		     struct)
    `(simple-interface ',(list->vector names) ',(list->vector types))))


; The compiler doesn't like to see unusual objects quoted, but this will
; fake it out.

(define strange-quotation
  (let ((operator/literal (get-operator 'literal)))
    (define (normal? thing)
      (or (number? thing)
	  (and (vector? thing)
	       (every normal? (vector->list thing)))))
    (lambda (thing)
      (if (normal? thing)
	  `',thing
	  (make-node operator/literal thing)))))


(define *reify-debug*  ;#f
  (let ((fn "build/reify-debug.tmp"))
    (lambda (x) (call-with-output-file fn
		  (lambda (port)
		    (display "Writing linker debug file ")
		    (display fn) (force-output (current-output-port))
		    (write x port)
		    (newline))))))
