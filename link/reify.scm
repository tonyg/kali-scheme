; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Structure reification.

; This is pretty gross.  We really want some kind of object dumper
; instead.

(define *objects* '())          ;List of (object . creation-form)
(define *object-count* 0)
(define *initializations* '())
(define *deal-with-location* (lambda (loc) loc))
(define *package-table* #f)     ;Entries are package-info structures


; REIFY-STRUCTURES returns a form that evaluates to an alist of
; (name . structure).
; deal-with-location is a procedure that maps locations to labels for
; them (e.g. integers); get-location is an expression that evaluates
; to a procedure that will be applied to the labelsat startup time to
; re-obtain the corresponding locations.

(define (reify-structures alist deal-with-location get-location)
  (set! *objects* '())
  (set! *object-count* 0)
  (set! *initializations* '())
  (set! *package-table* (make-table package-uid))
  (set! *deal-with-location* deal-with-location)

  (display "Reifying") (force-output (current-output-port))

  (let* ((result-form (reify-object alist))
	 (shebang
	  `(let ((the-objects (make-vector ,*object-count* #f))
		 (get-location ,get-location))
	     (begin ,@(reverse *initializations*))
	     (begin ,@(let ((l '()))
			(table-walk (lambda (p info)
				      (set! l
					    (cons (make-package-init-form info)
						  l)))
				    *package-table*)
			l))
	     (let ((structs ,result-form))
	       (set! the-objects #f)	;SO IT CAN BE GC'D
	       (set! get-location #f)
	       structs))))
    (newline)
    (if *reify-debug* (*reify-debug* shebang))

    (set! *objects* '())
    (set! *initializations* '())
    (set! *package-table* (make-table package-uid))
    (set! *deal-with-location* (lambda (loc) loc))

    shebang))

; Return an expression that will evaluate to thing.

(define (reify-object thing)
  (cond ((structure? thing)
	 (let* ((p-form (process-exports thing)))

	   (write-char #\.) (force-output (current-output-port))

	   `(make-structure
	     ,p-form
	     ,(interface-expression thing)
	     ',(structure-name thing))))
	((package? thing)
	 (process-one-object thing
			     (lambda ()
			       `(make-simple-package '() #f #f))
			     (lambda (form)
			       (table-set! *package-table* thing
					   (make-package-info form)))))
	((null? thing) ''())
	((pair? thing)
	 `(cons ,(reify-object (car thing))
		,(reify-object (cdr thing))))
	((symbol? thing)
	 `',thing)
	((transform? thing)
	 (process-transform thing))
	((operator? thing)
	 `(get-operator ',(operator-name thing)
			',(operator-type thing)))
	;; ((interface? thing) ...)
	(else (error "don't know how to reify this" thing))))

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
		`(vector-set! the-objects ,index ,(make-creation-form)))
	  (when-new form)
	  form))))

(define (add-initialization! form)
  (set! *initializations*
	(cons form *initializations*)))


; Add initializers that will create a structure's exported bindings.

(define (process-exports struct)
  (let* ((p (structure-package struct))
	 (info (process-one-package p)))
    (for-each-export (lambda (name want-type binding)
		       (if (not (process-one-binding name p info))
			   (warn "undefined export" name p)))
		     struct)
    (package-info-form info)))


; Packages...

(define (process-one-package p)
  (reify-object p)
  (table-ref *package-table* p))

(define package-info-type
  (make-record-type 'reify-info
		    '(bindings  ;List of (name static-info location)
		      form      ;'(vector-ref the-objects <index>)
		      table)))  ;Caches (assq? name bindings)

(define make-package-info
  (let ((make (record-constructor package-info-type
				  '(bindings form table))))
    (lambda (form)
      (make '()
	    form
	    (make-table name-hash)))))

(define package-info-bindings (record-accessor package-info-type 'bindings))
(define package-info-form     (record-accessor package-info-type 'form))
(define package-info-table    (record-accessor package-info-type 'table))

(define set-package-info-bindings!
  (record-modifier package-info-type 'bindings))


(define (make-package-init-form info)
  (let* ((bindings (package-info-bindings info)))
    `(initialize-reified-package!
	,(package-info-form info)
	;; Each binding is a list (name static loc)
	',(list->vector (map car bindings))    ;names
	,(strange-quotation (list->vector (map caddr bindings))) ;locations
	get-location)))


(define (process-one-binding name p info)	; => #t iff bound
  (let ((table (package-info-table info)))
    (if (table-ref table name)
	#t
	(let ((binding (package-lookup p name)))
	  (table-set! (package-info-table info) name #t)
	  (if (binding? binding)
	      (begin (really-process-one-binding name info binding)
		     #t)
	      #f)))))

(define (really-process-one-binding name info binding)
  (let ((static (binding-static binding))
	(loc (*deal-with-location* (binding-place binding))))
    (if static
	(add-initialization!
	    `(package-define! ,(package-info-form info)
			      ',name
			      ,(reify-object static)
			      (get-location ,(strange-quotation loc))))
	(let ((type (binding-type binding)))
	  (set-package-info-bindings!
	    info
	    (cons (list name
			(if (equal? static usual-variable-type)
			    #f
			    static)
			loc)
		  (package-info-bindings info)))))))

(define (process-transform t)
  (let* ((env (transform-env t))
	 (name (transform-id t))
	 (env-form (reify-object env)))
    (process-one-object
     t
     (lambda ()
       (let ((source (transform-source t)))
	 `(make-transform ,source	;transformer
			  ,env-form
			  ',(transform-type t) ;type
			  #f		;',source  -- omitted to save space...
			  ',name)))
     (lambda (form)
       (let ((info (process-one-package env)))
	 (for-each (lambda (name)
		     (process-one-binding name env info))
		   (or (transform-aux-names t)    ; () must be true
		       (begin
			 (warn "reified macro's auxiliary bindings are unknown"
			       name)
			 '()))))))))


(define (interface-expression struct)
  (let ((items '()))
    (for-each-export (lambda (name type binding)
		       (set! items (cons (list name type) items)))
		     struct)
    `(make-simple-interface #f ',(list->vector items))))


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
  (let ((fn "reify-debug.tmp"))
    (lambda (x) (call-with-output-file fn
		  (lambda (port)
		    (display "Writing linker debug file ")
		    (display fn) (force-output (current-output-port))
		    (write x port)
		    (newline))))))
