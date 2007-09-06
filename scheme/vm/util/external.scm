; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define (fake-it name)
  (lambda args
    (display "Call to ")
    (display (cons name args))
    (newline)
    0))

(define extended-vm        (fake-it 'extended-vm))
(define external-call      (fake-it 'call-external-value))
(define schedule-interrupt (fake-it 'schedule-interrupt))

(define dequeue-external-event! (fake-it 'dequeue-external-event!))

;; The following code "implements" bignums using the integers of the
;; host system. It stores these integers in the byte-vector of the
;; bignum stob just as the C code does but I'm not sure whether the
;; representation is the same.  Note that the VM passes the address
;; after the header to the external functions. We re-generate the
;; bignum stob from this address and use the b-vector operations to
;; access the data.

(define (bytes->b-vector bytes)
  (let ((bv (s48-allocate-bignum (length bytes))))
    (let lp ((bytes bytes)
             (pos 0))
      (if (null? bytes)
          bv
          (begin
            (b-vector-set! bv pos (car bytes))
            (lp (cdr bytes)
                (+ pos 1)))))))

(define (integer->b-vector int)
  (let lp ((i (abs int))
           (bytes '()))
    (if (< i 256)
        (bytes->b-vector
         (cons (if (< int 0) 1 0) ; sign
               (cons i bytes)))
        (lp (arithmetic-shift i -8)
            (cons (modulo i 256) bytes)))))

(define (b-vector->integer bv)
  (let lp ((index 1)
           (sig (- (b-vector-length bv) 2))
           (int 0))
    (if (= index (b-vector-length bv))
        (if (zero? (b-vector-ref bv 0))
            int
            (- int))
        (lp (+ index 1)
            (- sig 1)
            (+ (arithmetic-shift (b-vector-ref bv index) (* sig 8)) int)))))

;; no need for cached constants...
(define external-bignum-make-cached-constants
  (fake-it 'external-bignum-make-cached-constants))

(define-syntax define-binary-bignum-op/to-bignum
  (syntax-rules ()
    ((define-binary-bignum-op/to-bignum bignum-op integer-op)
     (define (bignum-op x y)
       (address-after-header
        (integer->b-vector
         (integer-op
          (b-vector->integer (address->stob-descriptor x))
          (b-vector->integer (address->stob-descriptor y)))))))))

(define-syntax define-binary-bignum-op
  (syntax-rules ()
    ((define-binary-bignum-op bignum-op integer-op)
     (define (bignum-op x y)
       (integer-op
        (b-vector->integer (address->stob-descriptor x))
        (b-vector->integer (address->stob-descriptor y)))))))

(define-syntax define-unary-bignum-op
  (syntax-rules ()
    ((define-unary-bignum-op bignum-op integer-op)
     (define (bignum-op x)
       (integer-op
        (b-vector->integer (address->stob-descriptor x)))))))

(define-syntax define-unary-bignum-op/to-bignum
  (syntax-rules ()
    ((define-unary-bignum-op/to-bignum bignum-op integer-op)
     (define (bignum-op x)
       (address-after-header
        (integer->b-vector
         (integer-op
          (b-vector->integer (address->stob-descriptor x)))))))))

(define-binary-bignum-op/to-bignum external-bignum-add +)
(define-binary-bignum-op/to-bignum external-bignum-subtract -)
(define-binary-bignum-op/to-bignum external-bignum-multiply *)
(define-binary-bignum-op/to-bignum external-bignum-quotient quotient) 
(define-binary-bignum-op/to-bignum external-bignum-remainder remainder) 
(define-binary-bignum-op external-bignum-divide
  (lambda (x y)
    (if (zero? y)
        (values #t 0 0)
        (values #f
                (address-after-header
                 (integer->b-vector (quotient x y)))
                (address-after-header
                 (integer->b-vector (remainder x y)))))))

(define-binary-bignum-op external-bignum-equal? =)

(define-binary-bignum-op external-bignum-compare
  (lambda (x y)
    (if (< x y)
        -1
        (if (= x y)
            0
            1))))

(define-unary-bignum-op external-bignum-test
  (lambda (x)
    (if (< x 0) -1
        (if (= x 0) 0
            1))))

(define-unary-bignum-op/to-bignum external-bignum-negate
  (lambda (x) (- x)))

(define external-bignum-from-long
  (lambda (long)
    (address-after-header (integer->b-vector long))))

(define  (external-bignum-fits-in-word? bignum-addr word-length two-compl?)
  (let ((bignum (b-vector->integer (address->stob-descriptor bignum-addr))))
    (and (>= bignum least-fixnum-value)
         (<= bignum greatest-fixnum-value))))

(define-unary-bignum-op external-bignum->long (lambda (x)
                                                x))

(define-binary-bignum-op/to-bignum external-bignum-bitwise-and bitwise-and)
(define-binary-bignum-op/to-bignum external-bignum-bitwise-xor bitwise-xor)
(define-binary-bignum-op/to-bignum external-bignum-bitwise-ior bitwise-ior)
(define-unary-bignum-op/to-bignum external-bignum-bitwise-not bitwise-not)
(define-unary-bignum-op external-bignum-bit-count bit-count)
(define  (external-bignum-arithmetic-shift bignum-addr y)
  (let ((bignum (address->stob-descriptor bignum-addr)))
    (address-after-header
     (integer->b-vector (arithmetic-shift bignum y)))))

(define (real-time) 0)
(define (run-time) 0)
(define (cheap-time) 0)

(define s48-call-native-procedure (fake-it 's48-call-native-code))
(define s48-invoke-native-continuation (fake-it 's48-call-native-code))
(define s48-native-return 0)

(define get-proposal-lock! (fake-it 'get-proposal-lock!))
(define release-proposal-lock! (fake-it 'release-proposal-lock!))

(define (shared-ref x) x)
(define-syntax shared-set!
  (syntax-rules ()
    ((shared-set! x v)
     (set! x v))))

(define (get-os-string-encoding)
  "UTF-8")

(define (raise-argument-type-error val)
  (fake-it 'raise-argument-type-error))

(define (raise-range-error val min max)
  (fake-it 'raise-range-error))
