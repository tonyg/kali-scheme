
(define-primitive allocate-memory ((positive-integer? type/int32)) type/int32)
(define-primitive deallocate-memory ((positive-integer? type/int32)) type/unit)

(define-primitive copy-memory! 
  ((positive-integer? type/int32)
   (positive-integer? type/int32)
   (positive-integer? type/int32))
  type/unit)

(define-primitive memory-equal? 
  ((positive-integer? type/int32)
   (positive-integer? type/int32)
   (positive-integer? type/int32))
  type/boolean)

(define-primitive unsigned-byte-ref
  ((positive-integer? type/int32))
  type/int8u
  byte-ref)

(define-primitive unsigned-byte-set!
  ((positive-integer? type/int32) (unsigned-byte? type/int8u))
  type/unit
  byte-set!)

(define-primitive word-ref ((positive-integer? type/int32)) type/int32)
(define-primitive word-set!
  ((positive-integer? type/int32) (positive-integer? type/int32))
  type/unit)

(define-primitive char-pointer->string
  ((positive-integer? type/int32)
   (positive-integer? type/int32))
  type/string)

(define-primitive char-pointer->nul-terminated-string
  ((positive-integer? type/int32))
  type/string)

(let ((read-block-return-type
       (make-tuple-type (list type/int32 type/boolean type/status))))
  (define-primitive read-block
    ((input-port? type/input-port)
     (positive-integer? type/int32)
     (positive-integer? type/int32))
    read-block-return-type))
  
(define-primitive write-block
  ((output-port? type/output-port)
   (positive-integer? type/int32)
   (positive-integer? type/int32))
  type/status)

