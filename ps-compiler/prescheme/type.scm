; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

(define-record-type base-type :base-type
  (really-make-base-type name uid)
  base-type?
  (name base-type-name)
  (uid  base-type-uid))		; an integer

(define-record-discloser :base-type
  (lambda (base-type)
    (list (base-type-name base-type)
	  (base-type-uid  base-type))))

(define *next-base-type-uid* 0)

(define (next-base-type-uid)
  (let ((x *next-base-type-uid*))
    (set! *next-base-type-uid* (+ x 1))
    x))

(define base-type-table (make-table))

(define (make-base-type name)
  (let ((type (really-make-base-type name (next-base-type-uid))))
    (table-set! base-type-table name type)
    type))

(define (lookup-type id)
  (cond ((table-ref base-type-table id)
	 => identity)
	(else #f)))

(define type/integer      (make-base-type 'integer))
(define type/float        (make-base-type 'float))

(define type/null         (make-base-type 'null))  ; no value
(define type/unit         (make-base-type 'unit))  ; single value
(define type/boolean      (make-base-type 'boolean))
(define type/undetermined (make-base-type '?))
(define type/input-port   (make-base-type 'input-port))
(define type/output-port  (make-base-type 'output-port))
(define type/address      (make-base-type 'address))
(define type/char         (make-base-type 'char))

(define (make-atomic-type name)
  (really-make-base-type name (next-base-type-uid)))

(define type/unknown type/undetermined) ; an alias

(define (type-name type)
  (if (base-type? type)
      (base-type-name type)
      (error "type has no name ~S" type)))

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

(define-record-type other-type :other-type
  (really-make-other-type kind subtypes finalized?)
  other-type?
  (kind       other-type-kind)
  (subtypes   other-type-subtypes set-other-type-subtypes!) ; set when finalized
  (finalized? other-type-finalized? set-other-type-finalized?!))

(define (make-other-type kind subtypes)
  (really-make-other-type kind subtypes #f))

(define-record-discloser :other-type
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
  (make-other-type 'arrow (cons result args)))

(define arrow-type? (make-other-type-predicate 'arrow))

(define (arrow-type-args type)
  (cdr (other-type-subtypes type)))

(define (arrow-type-result type)
  (car (other-type-subtypes type)))

; Pointer

(define (make-pointer-type type)
  (make-other-type 'pointer (list type)))

(define pointer-type? (make-other-type-predicate 'pointer))
(define (pointer-type-to pointer-type)
  (car (other-type-subtypes pointer-type)))

(define type/string (make-pointer-type type/char))

; Tuple (used for arguments and returning multiple values)

(define (make-tuple-type types)
  (if (and (not (null? types))
	   (null? (cdr types)))
      (car types)
      (make-other-type 'tuple types)))

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