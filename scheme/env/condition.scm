; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; This is an implementation of SRFI 35, together with some conversion
;; machinery to dock to the primitive condition system in SIMPLE-CONDITIONS.

(define-record-type condition-type :condition-type
  (really-make-condition-type name supertype fields all-fields)
  condition-type?
  (name condition-type-name)
  (supertype condition-type-supertype)
  (fields condition-type-fields)
  (all-fields condition-type-all-fields))

(define-record-discloser :condition-type
  (lambda (r)
    (list 'condition-type
	  (condition-type-name r))))

(define (make-condition-type name supertype fields)
  (if (not (symbol? name))
      (call-error "name is not a symbol"
		  make-condition-type
		  name))
  (if (not (condition-type? supertype))
      (call-error "supertype is not a condition type"
		  make-condition-type
		  supertype))
  (if (elements-in-common? (condition-type-all-fields supertype)
			   fields)
      (call-error "duplicate field name"
		  make-condition-type
		  fields (condition-type-all-fields supertype)))
  (really-make-condition-type name
                              supertype
                              fields
                              (append (condition-type-all-fields supertype)
                                      fields)))

(define-syntax define-condition-type
  (syntax-rules ()
    ((define-condition-type ?name ?supertype ?predicate
       (?field1 ?accessor1) ...)
     (begin
       (define ?name
         (make-condition-type '?name
                              ?supertype
                              '(?field1 ...)))
       (define (?predicate thing)
         (and (condition? thing)
              (condition-has-type? thing ?name)))
       (define (?accessor1 condition)
         (condition-ref (extract-condition condition ?name)
                        '?field1))
       ...))))

(define (condition-subtype? subtype supertype)
  (let recur ((subtype subtype))
    (cond ((not subtype) #f)
          ((eq? subtype supertype) #t)
          (else
           (recur (condition-type-supertype subtype))))))

(define (condition-type-field-supertype condition-type field)
  (let loop ((condition-type condition-type))
    (cond ((not condition-type) #f)
          ((memq field (condition-type-fields condition-type))
           condition-type)
          (else
           (loop (condition-type-supertype condition-type))))))

; The type-field-alist is of the form
; ((<type> (<field-name> . <value>) ...) ...)
(define-record-type condition :condition
  (really-really-make-condition type-field-alist)
  condition?
  (type-field-alist condition-type-field-alist))

(define-record-discloser :condition
  (lambda (r)
    (cons 'condition
	  (condition-type-field-alist r))))

;; pairs of (type . discloser)
;; Each discloser consumes the entire condition object
;; and returns a list of irritants

(define *primitive-condition-disclose-alist* '())

(define (define-primitive-condition-discloser type discloser)
  (set! *primitive-condition-disclose-alist*
	(cons (cons type discloser) *primitive-condition-disclose-alist*)))

(define (disclose-primitive-condition type alist condition)
  (let loop ((discloser-alist *primitive-condition-disclose-alist*))
    (cond
     ((null? discloser-alist)
      (list (cons (condition-type-name type) alist)))
     ((eq? type (caar discloser-alist))
      ((cdar discloser-alist) condition))
     (else
      (loop (cdr discloser-alist))))))

;; This is messy because it generates output for humns

(define-method &disclose-condition ((c :condition))
  (let* ((type-field-alist (condition-type-field-alist c))
	 (simple (assq &simple-condition type-field-alist)))
    (if (and simple
	     (pair? type-field-alist)
	     (null? (cdr type-field-alist)))
	;; rare case: all we have is the converted simple condition
	(cons (simple-condition-type c)
	      (simple-condition-stuff c))
	(let ((type-symbol
	       (cond
		((error? c) 'error)
		((warning? c) 'warning)
		((bug? c) 'bug)
		(else 'condition)))
	      (relevant
	       (apply append
		      (filter-map (lambda (pair)
				    (let ((type (car pair)))
				      (cond
				       ((or (eq? &message type)
					    (eq? &simple-condition type)
					    (eq? &irritants type))
					#f)
				       (else
					(disclose-primitive-condition (car pair) (cdr pair) c)))))
				  (condition-type-field-alist c)))))
	  `(,type-symbol
	    ,@(if (message-condition? c)
		  (list (condition-message c))
		  '())
	    ,@(if (irritants? c)
		  (condition-irritants c)
		  '())
	    ,@relevant)))))

(define (really-make-condition type-field-alist)
  (for-each (lambda (pair)
	      (let ((type (car pair))
		    (alist (cdr pair)))
		(if (not (list-set-eq? (condition-type-all-fields type)
				       (map car alist)))
		    (call-error "condition fields don't match condition type"
				really-make-condition
				(map car alist)
				(condition-type-all-fields type)
				type-field-alist))))
	    type-field-alist)
  (really-really-make-condition type-field-alist))

(define (make-condition type . field-plist)
  (let ((alist (let label ((plist field-plist))
                 (if (null? plist)
                            '()
                     (cons (cons (car plist)
                                 (cadr plist))
                           (label (cddr plist)))))))
    (if (not (list-set-eq? (condition-type-all-fields type)
			   (map car alist)))
        (apply call-error "condition fields don't match condition type"
	       make-condition
	       type field-plist))
    (really-make-condition (list (cons type alist)))))

(define (condition-has-type? condition type)
  (any? (lambda (has-type)
	  (condition-subtype? has-type type))
	(condition-types condition)))

(define (condition-ref condition field)
  (type-field-alist-ref (condition-type-field-alist condition)
                        field))

(define (type-field-alist-ref the-type-field-alist field)
  (let loop ((type-field-alist the-type-field-alist))
    (cond ((null? type-field-alist)
           (call-error "field not found"
		       type-field-alist-ref
		       field the-type-field-alist))
          ((assq field (cdr (car type-field-alist)))
           => cdr)
          (else
           (loop (cdr type-field-alist))))))

(define (make-compound-condition condition-1 . conditions)
  (really-make-condition
   (apply append (map condition-type-field-alist
                      (cons condition-1 conditions)))))

(define (extract-condition condition type)
  (let ((entry (first (lambda (entry)
			(condition-subtype? (car entry) type))
		      (condition-type-field-alist condition))))
    (if (not entry)
        (call-error "invalid condition type"
		    extract-condition
		    condition type))
    (really-make-condition
      (list (cons type
                  (map (lambda (field)
                         (assq field (cdr entry)))
                       (condition-type-all-fields type)))))))

(define-syntax condition
  (syntax-rules ()
    ((condition (?type1 (?field1 ?value1) ...) ...)
     (type-field-alist->condition
      (list
       (cons ?type1
             (list (cons '?field1 ?value1) ...))
       ...)))))

(define (type-field-alist->condition type-field-alist)
  (really-make-condition
   (map (lambda (entry)
	  (let* ((type (car entry))
		 (all-fields (condition-type-all-fields type)))
	    (if (not (list-set<=? (map car (cdr entry)) all-fields))
		(call-error "invalid field or fields"
			    type-field-alist->condition
			    (map car (cdr entry))
			    type
			    all-fields))
	    (cons type
		  (map (lambda (field)
			 (or (assq field (cdr entry))
			     (cons field
				   (type-field-alist-ref type-field-alist field))))
		       all-fields))))
	type-field-alist)))

(define (condition-types condition)
  (map car (condition-type-field-alist condition)))

(define (check-condition-type-field-alist the-type-field-alist)
  (let loop ((type-field-alist the-type-field-alist))
    (if (not (null? type-field-alist))
        (let* ((entry (car type-field-alist))
               (type (car entry))
               (field-alist (cdr entry))
               (fields (map car field-alist))
               (all-fields (condition-type-all-fields type)))
          (for-each (lambda (missing-field)
                      (let ((supertype
                             (condition-type-field-supertype type missing-field)))
                        (if (not
                             (any? (lambda (entry)
				     (let ((type (car entry)))
				       (condition-subtype? type supertype)))
				   the-type-field-alist))
                            (call-error "missing field in condition construction"
					check-condition-type-field-alist
					type
					missing-field
					the-type-field-alist))))
                    (list-set-difference all-fields fields))
          (loop (cdr type-field-alist))))))

;; Utilities, defined locally to avoid having to load SRFI 1 or
;; BIG-UTIL into the image.

;; (These need to come before the standard condition types below.)

(define (elements-in-common? list-1 list-2)
  (any? (lambda (element-1)
	  (memq element-1 list-2))
	list-1))

(define (list-set<=? list-1 list-2)
  (every? (lambda (element-1)
	    (memq element-1 list-2))
	  list-1))

(define (list-set-eq? list-1 list-2)
  (and (list-set<=? list-1 list-2)
       (list-set<=? list-2 list-1)))

(define (list-set-difference list-1 list-2)
  (filter (lambda (element-1)
	    (not (memq element-1 list-2)))
	  list-1))

(define (filter-map f l)
  (let loop ((l l) (r '()))
    (cond ((null? l)
	   (reverse r))
          ((f (car l))
           => (lambda (x)
                (loop (cdr l) (cons x r))))
          (else
	   (loop (cdr l) r)))))

(define (first pred list)
  (let loop ((list list))
    (cond ((null? list)
	   #f)
          ((pred (car list))
	   (car list))
          (else
	   (loop (cdr list))))))

(define (any? proc list)
  (let loop ((list list))
    (cond ((null? list)
	   #f)
          ((proc (car list))
	   #t)
          (else
	   (loop (cdr list))))))

(define (every? pred list)
  (let loop ((list list))
    (cond ((null? list)
	   #t)
          ((pred (car list))
	   (loop (cdr list)))
          (else
	   #f))))

(define (filter pred l)
  (let loop ((l l) (r '()))
    (cond ((null? l)
	   (reverse r))
          ((pred (car l))
	   (loop (cdr l) (cons (car l) r)))
          (else
	   (loop (cdr l) r)))))

;; Standard condition types

(define &condition (really-make-condition-type '&condition
                                               #f
                                               '()
                                               '()))

(define-condition-type &message &condition
  message-condition?
  (message condition-message))

(define-condition-type &serious &condition
  serious-condition?)

(define-condition-type &error &serious
  error?)

;; Beyond SRFI 35

(define-condition-type &bug &serious
  bug?)

;; High-level versions of what's in SIMPLE-CONDITIONS

(define-condition-type &irritants &condition
  irritants?
  (values condition-irritants))

(define-condition-type &call-error &bug
  call-error?
  (proc call-error-proc)
  (args call-error-args))

(define-primitive-condition-discloser &call-error
  (lambda (c)
    (list 'call-error:
	  (cons (call-error-proc c)
		(call-error-args c)))))

(define-condition-type &vm-exception &error
  vm-exception?
  (opcode vm-exception-opcode)
  (reason vm-exception-reason)
  (arguments vm-exception-arguments))

(define-primitive-condition-discloser &vm-exception
  (lambda (c)
    (cddr ; car is always ERROR, cadr is always the message
     (disclose-vm-condition (vm-exception-opcode c)
			    (vm-exception-reason c)
			    (vm-exception-arguments c)))))

;; could be smoother wrt SRFI 36
(define-condition-type &primitive-i/o-error &condition
  primitive-i/o-error?
  (status i/o-error-status)
  (operation i/o-error-operation)
  (arguments i/o-error-arguments))

(define-condition-type &warning &condition
  warning?)

(define-condition-type &note &condition
  note?)

(define-condition-type &syntax-error &warning
  syntax-error?)

;; This doesn't really belong here
(define-condition-type &interrupt &condition
  interrupt?
  (type interrupt-type))

(define-primitive-condition-discloser &interrupt
  (lambda (c)
    (list
     (list '&interrupt
	   (enumerand->name (interrupt-type c) interrupt)))))

;; This is for backwards compatibility and shouldn't be used by application code
(define-condition-type &simple-condition &condition
  simple-condition?
  (type simple-condition-type)
  (stuff simple-condition-stuff))

