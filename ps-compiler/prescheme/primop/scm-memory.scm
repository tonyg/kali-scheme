; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.


(define-primitive allocate-memory ((positive-integer? type/integer)) type/address)
(define-primitive deallocate-memory ((address? type/address)) type/unit)

(define-load-time-primitive (address? #f) address?)

(define-primitive address+ 
  ((address? type/address)
   (integer? type/integer))
  type/address)

(define-semi-primitive (address- address? integer?) address-
  (lambda (args node depth return?)
    (check-arg-type args 0 type/address depth node)
    (check-arg-type args 1 type/integer depth node)
    type/address)
  (lambda (x y) (address+ x (- 0 y))))

(define-primitive address-difference
  ((address? type/address)
   (address? type/address))
  type/integer)

(define-primitive address=
  ((address? type/address)
   (address? type/address))
  type/boolean)

(define-primitive address<
  ((address? type/address)
   (address? type/address))
  type/boolean)

(define-prescheme! 'null-address
  (let ((location (make-undefined-location 'null-address)))
    (set-contents! location (make-external-value "NULL" type/address))
    location)
  #f)

(define-semi-primitive (null-address? address?) null-address?
  (lambda (args node depth return)
    (check-arg-type args 0 type/address depth node)
    type/boolean)
  (lambda (x) (address= x null-address)))

(define (address-comparison-rule args node depth return?)
  (check-arg-type args 0 type/address depth node)
  (check-arg-type args 1 type/address depth node)
  type/boolean)

(define-semi-primitive (address> address? address?) address>
  address-comparison-rule
  (lambda (x y) (address< y x)))

(define-semi-primitive (address<= address? address?) address<=
  address-comparison-rule
  (lambda (x y) (not (address< y x))))

(define-semi-primitive (address>= address? address?) address>=
  address-comparison-rule
  (lambda (x y) (not (address< x y))))

(define-primitive address->integer
  ((address? type/address))
  type/integer)
   
(define-primitive integer->address
  ((integer? type/integer))
  type/address)

(define-primitive copy-memory! 
  ((address? type/address)
   (address? type/address)
   (positive-integer? type/integer))
  type/unit)

(define-primitive memory-equal? 
  ((address? type/address)
   (address? type/address)
   (positive-integer? type/integer))
  type/boolean)

(define-primitive unsigned-byte-ref
  ((address? type/address))
  type/integer
  byte-ref)

(define-primitive unsigned-byte-set!
  ((address? type/address) (unsigned-byte? type/integer))
  type/unit
  byte-set!)

(define-primitive word-ref ((address? type/address)) type/integer)
(define-primitive word-set!
  ((address? type/address) (positive-integer? type/integer))
  type/unit)

(define-primitive flonum-ref ((address? type/address)) type/float)
(define-primitive flonum-set!
  ((address? type/address) (floatnum? type/float))
  type/unit)

(define-primitive char-pointer->string
  ((address? type/address)
   (positive-integer? type/integer))
  type/string)

(define-primitive char-pointer->nul-terminated-string
  ((address? type/address))
  type/string)

(let ((read-block-return-type
       (make-tuple-type (list type/integer type/boolean type/status))))
  (define-primitive read-block
    ((input-port? type/input-port)
     (address? type/address)
     (positive-integer? type/integer))
    read-block-return-type))
  
(define-primitive write-block
  ((output-port? type/output-port)
   (address? type/address)
   (positive-integer? type/integer))
  type/status)

