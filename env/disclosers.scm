; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; --------------------
; DISCLOSE methods

(define-method disclose-methods (make-family 'closure 0)
  (lambda (obj)
    (if (closure? obj)
	(let ((id (template-id (closure-template obj)))
	      (name (template-print-name (closure-template obj))))
	  (if name
	      (list 'procedure
		    id
		    ;; A heuristic that sometimes loses.
		    (if (and (pair? name)
			     (eq? (car name) '#t) ;Curried
			     (vector? (closure-env obj)))
			(error-form
			  (if (null? (cdddr name))
			      (caddr name)
			      (cdddr name))
			  (reverse (cdr (vector->list (closure-env obj)))))
			name))
	      (list 'procedure id)))
	(fail))))

(define-method disclose-methods (make-family 'template 0)
  (lambda (obj)
    (if (template? obj)
	(let ((id (template-id obj))
	      (name (template-print-name obj)))
	  (if name
	      (list 'template id name)
	      (list 'template id)))
	(fail))))

(define-method disclose-methods (make-family 'location 0)
  (lambda (obj)
    (if (location? obj)
	(cons 'location
	      (cons (location-id obj)
		    (let ((name (location-name obj)))
		      (if (and name (not (eq? name (location-id obj))))
			  (list name (location-package-name obj))
			  '()))))
        (fail))))

(define-method disclose-methods (make-family 'continuation 0)
  (lambda (obj)
    (if (continuation? obj)
        (list 'continuation
	      (list 'pc (continuation-pc obj))
	      (let ((tem (continuation-template obj)))
		(or (template-print-name tem) (template-id tem))))
        (fail))))

(define-method disclose-methods (make-family 'code-vector 0)
  (lambda (obj)
    (if (code-vector? obj)
	(list 'code-vector (code-vector-length obj))
;        (cons 'code-vector
;              (let ((z (code-vector-length obj)))
;                (do ((i (- z 1) (- i 1))
;                     (l '() (cons (code-vector-ref obj i) l)))
;                    ((< i 0) l))))
	(fail))))


(define (template-print-name tem)
  (make-print-name (template-names tem)))

(define (make-print-name names)
  (if (null? names)
      #f
      (let ((name (car names))
	    (parent-name (make-print-name (cdr names))))
	(cond (parent-name
	       `(,(if name name 'unnamed)
		 in
		 ,@(if (pair? parent-name) parent-name (list parent-name))))
	      ((string? name) #f)  ;File name
	      (else name)))))

(define (template-file-name tem)
  (let loop ((names (template-names tem)))
    (if (null? names)
	#f
	(if (string? (car names))
	    (car names)
	    (loop (cdr names))))))

; --------------------
; Location names

(define (location-info loc)
  (let ((id (location-id loc)))
    (if (integer? id)
	(table-ref location-info-table id)
	#f)))

(define (location-name loc)
  (let ((probe (location-info loc)))
    (if probe
	(car probe)
	#f)))

(define (location-package-name loc)
  (let ((probe (location-info loc)))
    (if probe
	(table-ref package-name-table (cdr probe))
	#f)))


; --------------------
; Condition disclosers

(define-default-method disclose-condition-methods
  (lambda (c)
    (cons (cond ((error? c) 'error)
		((warning? c) 'warning)
		(else (car c)))
	  (condition-stuff c))))

(define-method disclose-condition-methods (make-family 'interrupt 0)
  (lambda (c)
    (if (interrupt? c)
	(list 'interrupt (enumerand->name (cadr c) interrupt))
	(fail))))
	

; Make prettier error messages for exceptions

(define-method disclose-condition-methods (make-family 'exception 0)
  (lambda (c)
    (if (exception? c)
	(let ((opcode (exception-opcode c))
	      (args   (exception-arguments c)))
	  ((vector-ref exception-disclosers opcode)
	   opcode
	   args))
	(fail))))

(define exception-disclosers
  (make-vector (vector-length op)
	       (lambda (opcode args)
		 (list 'error
		       "exception"
		       (let ((name (enumerand->name opcode op)))
			 (if (>= opcode op/eq?)
			     (error-form name args)
			     (cons name args)))))))

(define op/eq? (enum op eq?))

(define (define-exception-discloser opcode discloser)
  (vector-set! exception-disclosers opcode discloser))

(let ((disc (lambda (opcode args)
	      (let ((loc (car args)))
		(cons 'error
		      (cons (if (location-defined? loc)
				"unassigned variable"
				"undefined variable")
			    (cons (or (location-name loc) loc)
				  (let ((pack
					 (location-package-name loc)))
				    (if pack
					(list (list 'package pack))
					'())))))))))
  (define-exception-discloser (enum op global) disc)
  (define-exception-discloser (enum op set-global!) disc))

(let ((disc (lambda (opcode args)
	      (let ((proc (car args))
		    (as (cadr args)))
		(list 'error
		      "wrong number of arguments"
		      (error-form (or (if (closure? proc)
					  (or (template-print-name
					       (closure-template proc))
					      proc)
					  proc)
				      proc)
				  as))))))
  (define-exception-discloser (enum op check-nargs=) disc)
  (define-exception-discloser (enum op check-nargs>=) disc))

(define-exception-discloser (enum op call)
  (lambda (opcode args)
    (list 'error
	  "attempt to call a non-procedure"
	  (map value->expression (cons (car args) (cadr args))))))

(define-exception-discloser (enum op values)
  (lambda (opcode args)
    (if (null? (car args))
	(list 'error
	      "returning zero values when one is expected"
	      '(values))
	(list 'error
	      "returning several values when only one is expected"
	      (error-form 'values (car args))))))

(let ((disc (lambda (opcode args)
	      (let ((thing     (car args))
		    (type-byte (cadr args))
		    (offset    (caddr args))
		    (rest      (cdddr args)))
		(let ((data (assq (enumerand->name type-byte stob)
				  stob-data)))
		  (list 'error
			"exception"
			(error-form ((if (= opcode op/stored-object-ref)
					 car
					 cadr)
				     (list-ref data (+ offset 3)))
				    (cons thing rest))))))))
  (define-exception-discloser (enum op stored-object-ref) disc)
  (define-exception-discloser (enum op stored-object-set!) disc))

(define op/stored-object-ref (enum op stored-object-ref))

(let ((disc (lambda (opcode args)
	      (let ((type (enumerand->name (car args) stob)))
		(list 'error
		      "exception"
		      (error-form (string->symbol
				   ;; Don't simplify this to "make-"  --JAR
				   (string-append (symbol->string 'make-)
						  (symbol->string type)))
				  (cdr args)))))))
  (define-exception-discloser (enum op make-vector-object) disc)
  (define-exception-discloser (enum op make-byte-vector-object) disc)
  (define-exception-discloser (enum op make-char-vector-object) disc))

(define (vector-exception-discloser suffix)
  (lambda (opcode args)
    (let ((type (enumerand->name (cadr args) stob)))
      (list 'error
	    "exception"
	    (error-form (string->symbol
			 (string-append (symbol->string type)
					"-"
					(symbol->string suffix)))
			(cons (car args) (cddr args)))))))

(let ((disc (vector-exception-discloser 'length)))
  (define-exception-discloser (enum op stored-object-length) disc)
  (define-exception-discloser (enum op stored-object-byte-length) disc))

(let ((disc (vector-exception-discloser 'ref)))
  (define-exception-discloser (enum op stored-object-indexed-ref) disc)
  (define-exception-discloser (enum op stored-object-indexed-byte-ref) disc)
  (define-exception-discloser (enum op stored-object-indexed-char-ref) disc))

(let ((disc (vector-exception-discloser 'set!)))
  (define-exception-discloser (enum op stored-object-indexed-set!) disc)
  (define-exception-discloser (enum op stored-object-indexed-byte-set!) disc)
  (define-exception-discloser (enum op stored-object-indexed-char-set!) disc))



; Call-errors should print as (proc 'arg1 'arg2 ...)

(define-method disclose-condition-methods (make-family 'call-error 0)
  (lambda (c)
    (if (call-error? c)
	(list 'error (cadr c) (error-form (caddr c) (cdddr c)))
	(fail))))

; --------------------
; Utilities

(define (error-form proc args)
  (cons proc (map value->expression args)))

(define (value->expression obj)
  (if (or (number? obj) (char? obj) (string? obj) (boolean? obj))
      obj
      `',obj))
