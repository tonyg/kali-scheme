; Copyright (c) 1994 Richard Kelsey.  See file COPYING.


(define-record-type base-type
  (name
   uid       ; an integer
   kind      ; INTEGER, FLOAT, or #f
   size      ; number of bytes
   )
  ())

(define-record-discloser type/base-type
  (lambda (base-type)
    (list (base-type-name base-type)
	  (base-type-uid  base-type))))

(define *next-base-type-uid* 0)

(define (next-base-type-uid)
  (let ((x *next-base-type-uid*))
    (set! *next-base-type-uid* (+ x 1))
    x))

(define base-type-table (make-table))

(define (make-base-type name kind size)
  (let ((type (base-type-maker name (next-base-type-uid) kind size)))
    (table-set! base-type-table name type)
    type))

(define (lookup-type id)
  (cond ((table-ref base-type-table id)
	 => identity)
	(else #f)))

;  0 <= x <= 127  (intersection of int8 and int8u, for constants)
(define type/int7u        (make-base-type 'int7u   'int   1))

(define type/int8         (make-base-type 'int8    'int   1))
(define type/int8u        (make-base-type 'int8u   'int   1))
(define type/int32        (make-base-type 'int32   'int   4))
(define type/float64      (make-base-type 'float64 'float 8))

(define type/null         (make-base-type 'null     #f #f))  ; no value
(define type/unit         (make-base-type 'unit     #f #f))  ; single value
(define type/boolean      (make-base-type 'boolean  #f #f))
(define type/undetermined (make-base-type '?        #f #f))
(define type/input-port   (make-base-type 'input-port  #f #f))
(define type/output-port  (make-base-type 'output-port #f #f))

(define type/char type/int8u)

(define (make-atomic-type name)
  (base-type-maker name (next-base-type-uid) #f #f))

(define type/unknown type/undetermined) ; an alias

(define pointer-size 4) ; bytes in a pointer

(define (coercable-type? x)
  (and (base-type? x)
       (or (eq? (base-type-kind x) 'int)
	   (eq? (base-type-kind x) 'float))))

(define lattice-type? coercable-type?)

(define (type>= type1 type2)
  (let ((type1 (maybe-follow-uvar type1))
	(type2 (maybe-follow-uvar type2)))
    (or (eq? type1 type2)
	(if (base-type? type1)
	    (and (base-type? type2)
		 (base-type> type1 type2))
	    (type-eq? type1 type2)))))

(define (type> type1 type2)
  (not (type>= type2 type1)))

; This should probably be encoded in some more reasonable way.

(define (base-type> type1 type2)
  (if (or (not (base-type? type1))
	  (not (base-type? type2)))
      #f
      (case (base-type-name type1)
	((float64)
	 (not (eq? (base-type-name type2) 'float64)))
	((int32)
	 (and (eq? (base-type-kind type2) 'int)
	      (> 4 (base-type-size type2))))
        ((int8 int8u)
	 (eq? (base-type-name type2) 'int7u))
	(else
	 #f))))

(define (base-type>= type1 type2)
  (not (base-type> type2 type1)))

(define (type-name type)
  (if (base-type? type)
      (base-type-name type)
      (error "type has no name ~S" type)))

; Return the join of two types (which must be EQ? or coercable types).

(define (join-types type1 type2)
  (cond ((or (not type2)
	     (eq? type1 type2))
	 type1)
	((or (not type1)
	     (base-type> type1 type2))
	 type2)
	(else
	 type1)))

(define (make-base-type-table)
  (let ((elts (make-vector *next-base-type-uid* #f)))
    (values (lambda (type)
	      (vector-ref elts (base-type-uid type)))
	    (lambda (type value)
	      (vector-set! elts (base-type-uid type) value)))))

;--------------------------------------------------
; This won't terminate on recursive types.

(define (type-eq? type1 type2)
  (let ((type1 (maybe-follow-uvar type1))
	(type2 (maybe-follow-uvar type2)))
    (or (eq? type1 type2)
	(and (other-type? type1)
	     (other-type? type2)
	     (eq? (other-type-kind type1)
		  (other-type-kind type2))
	     (let loop ((l1 (other-type-subtypes type1))
			(l2 (other-type-subtypes type2)))
	       (cond ((null? l1) (null? l2))
		     ((null? l2) #f)
		     ((type-eq? (car l1) (car l2))
		      (loop (cdr l1) (cdr l2)))
		     (else #f)))))))

;--------------------------------------------------
; Arrow and pointer types (and perhaps others later)
; All done together to simplify the type walking

(define-record-type other-type
  (
   kind
   (subtypes)       ; set when finalized
   )
  (
   (finalized? #f)
   ))

(define make-other-type other-type-maker)

(define-record-discloser type/other-type
  (lambda (type)
    (case (other-type-kind type)
      ((arrow)
       (list 'arrow-type
	     (arrow-type-args type)
	     (arrow-type-result type)))
      (else
       (cons (other-type-kind type)
	     (other-type-subtypes type))))))

(define (make-other-type-predicate kind)
  (lambda (x)
    (and (other-type? x)
	 (eq? kind (other-type-kind x)))))

; Arrow

(define (make-arrow-type args result)
  (other-type-maker 'arrow (cons result args)))

(define arrow-type? (make-other-type-predicate 'arrow))

(define (arrow-type-args type)
  (cdr (other-type-subtypes type)))

(define (arrow-type-result type)
  (car (other-type-subtypes type)))

; Pointer

(define (make-pointer-type type)
  (other-type-maker 'pointer (list type)))

(define pointer-type? (make-other-type-predicate 'pointer))
(define (pointer-type-to pointer-type)
  (car (other-type-subtypes pointer-type)))

(define type/string (make-pointer-type type/int8u))
(define type/byte-pointer (make-pointer-type type/int8u))

; Tuple (used for arguments and returning multiple values)

(define (make-tuple-type types)
  (if (and (not (null? types))
	   (null? (cdr types)))
      (car types)
      (other-type-maker 'tuple types)))

(define tuple-type? (make-other-type-predicate 'tuple))

(define (tuple-type-types type)
  (other-type-subtypes type))

;--------------------------------------------------

(define (finalize-type type)
  (let ((type (maybe-follow-uvar type)))
    (cond ((and (other-type? type)
		(not (other-type-finalized? type)))
	   (let ((subs (other-type-subtypes type)))
	     (set-other-type-finalized?! type #t)
	     (set-other-type-subtypes! type (map finalize-type subs))))
	  ((and (uvar? type)               
		(uvar-tuple-okay? type)) ; unused return value
	   (bind-uvar! type type/unit)))
    type))

;--------------------------------------------------

(define (expand-type-spec spec)
  (cond ((pair? spec)
	 (case (car spec)
	   ((=>)
	    (make-arrow-type (map expand-type-spec (cadr spec))
			     (make-tuple-type (map expand-type-spec
						   (cddr spec)))))
	   ((^)
	    (make-pointer-type (expand-type-spec (cadr spec))))
	   ((tuple)
	    (make-tuple-type (map expand-type-spec (cdr spec))))
	   (else
	    (error "unknown type syntax ~S" spec))))
	((not (symbol? spec))
	 (error "unknown type syntax ~S" spec))
	((lookup-type spec)
	 => identity)
	((lookup-record-type spec)
	 => make-pointer-type)
	(else
	 (error "unknown type name ~S" spec))))

;--------------------------------------------------

(define (print-relation-list list select)
  (for-each (lambda (rel)
	      (let ((dest (maybe-follow-uvar (select rel))))
		(if (uvar? dest)
		    (format #t " ~S~S" (uvar-prefix dest) (uvar-id dest))
		    (format #t " ~S" dest))))
	    list)
  (newline))

(define (show-constraints)
  (for-each (lambda (uvar)
	      (cond ((not (uvar-binding uvar))
		     (format #t "~S~S~%" (uvar-prefix uvar) (uvar-id uvar))
		     (print-relation-list (uvar-sources uvar) relation-source)
		     (print-relation-list (uvar-sinks uvar) relation-sink))))
	    *constrained-uvars*))
		     
(define (display-type type port)

  (define (do-list list)
    (write-char #\( port)
    (cond ((not (null? list))
	   (do-type (car list))
	   (for-each (lambda (type)
		       (write-char #\space port)
		       (do-type type))
		     (cdr list))))
    (write-char #\) port))

  (define (do-type type)
    (let ((type (maybe-follow-uvar type)))
      (cond ((base-type? type)
	     (display (base-type-name type) port))
	    ((record-type? type)
	     (display (record-type-name type) port))
	    ((arrow-type? type)
	     (write-char #\( port)
	     (do-list (arrow-type-args type))
	     (display " -> " port)
	     (do-type (arrow-type-result type))
	     (write-char #\) port))
	    ((pointer-type? type)
	     (write-char #\* port)
	     (do-type (pointer-type-to type)))
	    ((uvar? type)
	     (write-char #\T port)
	     (display (uvar-id type) port))
	    ((type-scheme? type)
	     (display "(for-all " port)
	     (do-list (type-scheme-free-uvars type))
	     (display " " port)
	     (do-type (type-scheme-type type))
	     (display ")" port))
	    ((tuple-type? type)
	     (display "(tuple " port)
	     (do-list (tuple-type-types type))
	     (display ")" port))
	    (else
	     (bug "don't know how to display type ~S" type)))))
    
  (do-type type))