; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.

; This is file xnum.scm.

;;;; Extended numbers

; This is pretty much a replica of the record facility.

(define (extended-number-type r)
  (extended-number-ref r 0))

(define extended-number-type-type
  (make-record-type "Extended-number-type" '(id field-names)))

(define make-extended-number-type
  (record-constructor extended-number-type-type '(id field-names)))

(define extended-number-type?
  (record-predicate extended-number-type-type))

(define extended-number-type-identification
  (record-accessor extended-number-type-type 'id))

(define extended-number-type-field-names
  (record-accessor extended-number-type-type 'field-names))

(define (extended-number-type-field-index nt name)
  (let loop ((names (extended-number-type-field-names nt))
	     (i 1))
    (cond ((null? names) (error "unknown field index"
				(extended-number-type-identification nt)
				name))
	  ((eq? name (car names))
	   i)
	  (else (loop (cdr names) (+ i 1))))))

(define (extended-number-constructor nt names)
  (define indexes (map (lambda (name)
			 (extended-number-type-field-index nt name))
		       names))
  (define size (+ 1 (length (extended-number-type-field-names nt))))
  (define (constructor . args)
    (let ((n (make-extended-number size (unspecific))))
      (extended-number-set! n 0 nt)
      (let loop ((is indexes) (args args))
	(if (and (null? is) (null? args))
	    n
	    (begin (extended-number-set! n (car is) (car args))
		   (loop (cdr is) (cdr args)))))))
  constructor)

(define (extended-number-accessor nt name)
  (define index (extended-number-type-field-index nt name))
  (define (accessor n)	;Use internal define to get proc to print better
    (extended-number-ref n index))
  accessor)

(define (extended-number-predicate nt)
  (define (predicate x)
    (and (extended-number? x)
	 (eq? (extended-number-type x) nt)))
  predicate)

; Make all the numeric instructions be extensible.

(define plus-table           (make-method-table '+))
(define minus-table          (make-method-table '-))
(define *-table              (make-method-table '*))
(define /-table              (make-method-table '/))
(define =-table              (make-method-table '=))
(define <-table              (make-method-table '<))
(define quotient-table       (make-method-table 'quotient))
(define remainder-table      (make-method-table 'remainder))
  
(define integer?-table       (make-method-table 'integer?))
(define rational?-table      (make-method-table 'rational?))
(define real?-table          (make-method-table 'real?))
(define complex?-table       (make-method-table 'complex?))
(define number?-table        (make-method-table 'number?))
(define exact?-table         (make-method-table 'exact?))

(define exact->inexact-table (make-method-table 'exact->inexact))
(define inexact->exact-table (make-method-table 'inexact->exact))
(define real-part-table      (make-method-table 'real-part))
(define imag-part-table      (make-method-table 'imag-part))

(define floor-table          (make-method-table 'floor))
(define numerator-table      (make-method-table 'numerator))
(define denominator-table    (make-method-table 'denominator))

(define sqrt-table           (make-method-table 'sqrt))
(define make-rectangular-table (make-method-table 'make-rectangular))

; Default methods.

(define-default-method integer?-table  (lambda (x) #f))
(define-default-method rational?-table integer?)
(define-default-method real?-table     rational?)
(define-default-method complex?-table  real?)
(define-default-method number?-table   complex?)

(define-default-method exact?-table    (lambda (x) #t))

; there's no default method for exact->inexact or inexact->exact

(define-default-method real-part-table
  (lambda (x)
    (if (real? x) x (fail))))

(define-default-method imag-part-table
  (lambda (x) 
    (if (real? x)
	(if (exact? x) 0 (exact->inexact 0))
	(fail))))

(define-default-method floor-table
  (lambda (n) (if (integer? n) n (fail))))

(define-default-method numerator-table
  (lambda (n) (if (integer? n) n (fail))))

(define-default-method denominator-table
  (lambda (n)
    (if (integer? n)
	(if (exact? n) 1 (exact->inexact 1))
	(fail))))

(define-default-method sqrt-table
  (lambda (n)
    (if (and (integer? n)
	     (>= n 0))
	(non-negative-integer-sqrt n)
	(fail))))

(define (non-negative-integer-sqrt n)
  (cond ((<= n 1)    ; for both 0 and 1
	 n)
	;; ((< n 0)
	;;  (make-rectangular 0 (integer-sqrt (- 0 n))))
	(else
	 (let loop ((m (quotient n 2)))
	   (let ((m1 (quotient n m)))
	     (cond ((< m1 m)
		    (loop (quotient (+ m m1) 2)))
		   ((= n (* m m))
		    m)
		   (else
		    (exact->inexact m))))))))

; Allow access to extended number methods from primitive byte codes.
; Think of the VM's implementation of the opcode as just one method among
; many.

(make-opcode-generic! (enum op +) plus-table)
(make-opcode-generic! (enum op -) minus-table)
(make-opcode-generic! (enum op *) *-table)
(make-opcode-generic! (enum op /) /-table)
(make-opcode-generic! (enum op =) =-table)
(make-opcode-generic! (enum op <) <-table)
(make-opcode-generic! (enum op quotient) quotient-table)
(make-opcode-generic! (enum op remainder) remainder-table)

(make-opcode-generic! (enum op integer?) integer?-table)
(make-opcode-generic! (enum op rational?) rational?-table)
(make-opcode-generic! (enum op real?) real?-table)
(make-opcode-generic! (enum op complex?) complex?-table)
(make-opcode-generic! (enum op number?) number?-table)

(make-opcode-generic! (enum op exact?) exact?-table)
(make-opcode-generic! (enum op exact->inexact) exact->inexact-table)
(make-opcode-generic! (enum op inexact->exact) inexact->exact-table)
(make-opcode-generic! (enum op real-part) real-part-table)
(make-opcode-generic! (enum op imag-part) imag-part-table)
(make-opcode-generic! (enum op floor) floor-table)
(make-opcode-generic! (enum op numerator) numerator-table)
(make-opcode-generic! (enum op denominator) denominator-table)

(make-opcode-generic! (enum op sqrt) sqrt-table)
(make-opcode-generic! (enum op make-rectangular) make-rectangular-table) ;weird

; Unimplemented: cos sin tan acos asin atan exp ...
