
; (d-s-p (id (arg-pred arg-type) ...) result-type)

(define-syntax define-simple
  (lambda (exp r c)
    (let ((id (cadr exp))
	  (args (caddr exp))
	  (result (cadddr exp))
	  (primop (if (null? (cddddr exp)) (cadr exp) (car (cddddr exp)))))
      `(define-primitive (,id . ,(map car args)) ,id
	 (lambda (args node depth return?)
	   ,@(do ((i 0 (+ i 1))
		  (args args (cdr args))
		  (res '() (cons `(check-arg-type args ,i ,(cadar args) depth node)
				 res)))
		 ((null? args)
		  (reverse res)))
	   ,result)
	 ,id
	 (lambda (args type)
	   (make-primop-call-node (get-prescheme-primop ',primop) args))
	 #t))))

(define-simple allocate-memory ((positive-integer? type/int32)) type/int32)
(define-simple deallocate-memory ((positive-integer? type/int32)) type/unit)

; deallocate
; null-pointer?

; copy-memory!

(define-simple unsigned-byte-ref
  ((positive-integer? type/int32))
  type/int8u
  byte-ref)

(define-simple unsigned-byte-set!
  ((positive-integer? type/int32) (unsigned-byte? type/int8u))
  type/unit
  byte-set!)

; (unsigned-byte-set! <integer-pointer> <byte>)

(define-simple word-ref ((positive-integer? type/int32)) type/int32)
(define-simple word-set!
  ((positive-integer? type/int32) (positive-integer? type/int32))
  type/unit)

; char-pointer->string
; read-block write-block
