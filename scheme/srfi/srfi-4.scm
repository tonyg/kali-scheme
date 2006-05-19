; SRFI 4: Homogeneous numeric vector datatypes
; Does not include hacks to the reader (intentionally).

(define (sub1 i) (- i 1))
(define (id x) x)

(define-syntax define-vector-types
  (syntax-rules ()
    ((define-vector-types spec ...)
     (begin (define-vector-type spec) ...))))
       
(define-syntax define-vector-type  
  (syntax-rules ()
    ((define-vector-type
       (size :tagvector
             tagvector? make-tagvector tagvector tagvector-length
             tagvector-ref tagvector-set! tagvector->list list->tagvector
             blob-ref blob-set!
             f->i i->f))
     (begin       
       (define-record-type tagvector :tagvector
         (really-make-tagvector blob)
         tagvector?
         (blob tagvector-blob))
       
       (define make-tagvector
         (case-lambda
           ((n)
            (really-make-tagvector (make-blob (* size n))))
           ((n x)
            (let* ((v (really-make-tagvector (make-blob (* size n))))
                   (b (tagvector-blob v)))     
              (do ((i n (sub1 i)))
                  ((zero? i) v)
                (blob-set! b (* size (sub1 i)) (f->i x)))))))
              
       (define (tagvector . elems)
         (let* ((n (length elems))
                (v (really-make-tagvector (make-blob (* size n))))
                (b (tagvector-blob v)))
           (do ((i n (sub1 i))
                (e (reverse elems) (cdr e)))
               ((zero? i) v)
             (blob-set! b (* size (sub1 i)) (f->i (car e))))))
       
       (define (tagvector-length v)
         (/ (blob-length (tagvector-blob v)) size))

       (define (tagvector-ref v i)
         (i->f (blob-ref (tagvector-blob v) (* size i))))
       
       (define (tagvector-set! v i n)
         (blob-set! (tagvector-blob v) (* size i) (f->i n)))
       
       (define (tagvector->list v)
         (map i->f
           (blob->uint-list size (endianness native) (tagvector-blob v))))
       
       (define (list->tagvector ls)
         (really-make-tagvector
          (uint-list->blob size (endianness native) (map f->i ls))))
       
       ))))

(define-vector-types
  (1 :s8vector 
     s8vector? make-s8vector s8vector s8vector-length
     s8vector-ref s8vector-set! s8vector->list list->s8vector
     blob-s8-ref blob-s8-set! id id)
  
  (1 :u8vector
     u8vector? make-u8vector u8vector u8vector-length
     u8vector-ref u8vector-set! u8vector->list list->u8vector
     blob-u8-ref blob-u8-set! id id)
 
  (2 :s16vector
     s16vector? make-s16vector s16vector s16vector-length
     s16vector-ref s16vector-set! s16vector->list list->s16vector
     blob-s16-native-ref blob-s16-native-set! id id)
  
  (2 :u16vector
     u16vector? make-u16vector u16vector u16vector-length
     u16vector-ref u16vector-set! u16vector->list list->u16vector
     blob-u16-native-ref blob-u16-native-set! id id)

  (4 :s32vector
     s32vector? make-s32vector s32vector s32vector-length
     s32vector-ref s32vector-set! s32vector->list list->s32vector
     blob-s32-native-ref blob-s32-native-set! id id)
  
  (4 :u32vector
     u32vector? make-u32vector u32vector u32vector-length
     u32vector-ref u32vector-set! u32vector->list list->u32vector
     blob-u32-native-ref blob-u32-native-set! id id)  
  
  (8 :s64vector
     s64vector? make-s64vector s64vector s64vector-length
     s64vector-ref s64vector-set! s64vector->list list->s64vector
     blob-s64-native-ref blob-s64-native-set! id id)  
  
  (8 :u64vector
     u64vector? make-u64vector u64vector u64vector-length u64vector-ref
     u64vector-set! u64vector->list list->u64vector
     blob-u64-native-ref blob-u64-native-set! id id)

  (4 :f32vector
     f32vector? make-u32vector f32vector f32vector-length f32vector-ref
     f32vector-set! f32vector->list list->f32vector
     blob-u32-native-ref blob-u32-native-set! fl->u32 u32->fl)

  (8 :f64vector
     f64vector? make-f64vector f64vector f64vector-length f64vector-ref
     f64vector-set! f64vector->list list->f64vector
     blob-u64-native-ref blob-u64-native-set! fl->u64 u64->fl))


;; --
;; Flonum <-> Integer conversions.  
;; Based on SRFI 56 Reference Implementation by Alex Shinn.
;; Both use big endian.

(define (combine . bytes)
  (combine-ls bytes))

(define (combine-ls bytes)
  (let loop ((b bytes) (acc 0))
    (if (null? b) acc
        (loop (cdr b) (+ (arithmetic-shift acc 8) (car b))))))

;; Takes an unsigned 32 bit integer to the flonum it represents.
(define (u32->fl n)
  
  (define (mantissa expn b2 b3 b4)
    (case expn   ; recognize special literal exponents
      ((255) #f)
      ((0)       ; denormalized
       (exact->inexact (* (expt 2 (- 1 (+ 127 23))) (combine b2 b3 b4))))
      (else
       (exact->inexact
        (* (expt 2 (- expn (+ 127 23)))
           (combine (+ b2 128) b3 b4)))))) ; hidden bit
  
  (define (exponent b1 b2 b3 b4)
    (if (> b2 127)  ; 1st bit of b2 is low bit of expn
        (mantissa (+ (* 2 b1) 1) (- b2 128) b3 b4)
        (mantissa (* 2 b1) b2 b3 b4)))
  
  (define (sign b1 b2 b3 b4)
    (if (> b1 127)  ; 1st bit of b1 is sign
        (cond ((exponent (- b1 128) b2 b3 b4) => -) (else #f))
        (exponent b1 b2 b3 b4)))
  
  (let* ((b  (uint-list->blob 4 (endianness big) (list n)))
         (b1 (blob-u8-ref b 0))  
         (b2 (blob-u8-ref b 1))
         (b3 (blob-u8-ref b 2))  
         (b4 (blob-u8-ref b 3)))
    
    (sign b1 b2 b3 b4)))

;; Takes an unsigned 64 bit integer to the flonum it represents.
(define (u64->fl n)
  
  (define (mantissa expn b2 b3 b4 b5 b6 b7 b8)
    (case expn   ; recognize special literal exponents
      ((255) #f) ; won't handle NaN and +/- Inf
      ((0)       ; denormalized
       (exact->inexact (* (expt 2.0 (- 1 (+ 1023 52)))
                          (combine b2 b3 b4 b5 b6 b7 b8))))
      (else
       (exact->inexact
        (* (expt 2.0 (- expn (+ 1023 52)))
           (combine (+ b2 16) b3 b4 b5 b6 b7 b8)))))) ; hidden bit
  (define (exponent b1 b2 b3 b4 b5 b6 b7 b8)
    (mantissa (bitwise-ior (arithmetic-shift b1 4)          ; 7 bits
                           (extract-bit-field 4 4 b2))      ; + 4 bits
              (extract-bit-field 4 0 b2) b3 b4 b5 b6 b7 b8))
  (define (sign b1 b2 b3 b4 b5 b6 b7 b8)
    (if (> b1 127)  ; 1st bit of b1 is sign
        (cond ((exponent (- b1 128) b2 b3 b4 b5 b6 b7 b8) => -)
              (else #f))
        (exponent b1 b2 b3 b4 b5 b6 b7 b8)))
  (let* ((b  (uint-list->blob 8 (endianness big) (list n)))
         (b1 (blob-u8-ref b 0))  (b2 (blob-u8-ref b 1))
         (b3 (blob-u8-ref b 2))  (b4 (blob-u8-ref b 3))
         (b5 (blob-u8-ref b 4))  (b6 (blob-u8-ref b 5))
         (b7 (blob-u8-ref b 6))  (b8 (blob-u8-ref b 7)))
    (sign b1 b2 b3 b4 b5 b6 b7 b8)))

(define (call-with-mantissa&exponent num f)
  (cond
    ((negative? num) (call-with-mantissa&exponent (- num) f))
    ((zero? num) (f 0 0))
    (else
     (let ((base 2) (mant-size 23) (exp-size 8))
       (let* ((bot (expt base mant-size))
              (top (* base bot)))
         (let loop ((n (exact->inexact num)) (e 0))
           (cond
             ((>= n top)
              (loop (/ n base) (+ e 1)))
             ((< n bot)
              (loop (* n base) (- e 1)))
             (else
              (f (inexact->exact (round n)) e)))))))))

(define (extract-bit-field size position n)
  (bitwise-and (bitwise-not (arithmetic-shift -1 size))
               (arithmetic-shift n (- position))))

;; Takes a flonum to its representation as an unsigned 32 bit integer.
(define (fl->u32 num)
  (cond
    ((zero? num) 0)
    (else
     (combine-ls
      (call-with-mantissa&exponent num
        (lambda (f e)
          (let ((e0 (+ e 127 23)))
            (cond
              ((negative? e0)
               (let* ((f1 (inexact->exact (round (* f (expt 2 (- e0 1))))))
                      (b2 (extract-bit-field 7 16 f1))        ; mant:16-23
                      (b3 (extract-bit-field 8 8 f1))         ; mant:8-15
                      (b4 (extract-bit-field 8 0 f1)))        ; mant:0-7
                 (list (if (negative? num) 128 0) b2 b3 b4)))
              ((> e0 255) ; XXXX here we just write infinity
               (list (if (negative? num) 255 127) 128 0 0))
              (else
               (let* ((b0 (arithmetic-shift e0 -1))
                      (b1 (if (negative? num) (+ b0 128) b0)) ; sign + exp:1-7
                      (b2 (bitwise-ior
                           (if (odd? e0) 128 0)               ; exp:0
                           (extract-bit-field 7 16 f)))       ;   + mant:16-23
                      (b3 (extract-bit-field 8 8 f))          ; mant:8-15
                      (b4 (extract-bit-field 8 0 f)))         ; mant:0-7
                 (list b1 b2 b3 b4)))))))))))

;; Takes a flonum to its representation as an unsigned 64 bit integer.
(define (fl->u64 num)
  (cond
    ((zero? num) 0)
    (else
     (combine-ls
      (call-with-mantissa&exponent num 2 52 11
        (lambda (f e)
          (let ((e0 (+ e 1023 52)))
            (cond
              ((negative? e0)
               (let* ((f1 (inexact->exact (round (* f (expt 2 (- e0 1))))))
                      (b2 (extract-bit-field 4 48 f1))
                      (b3 (extract-bit-field 8 40 f1))
                      (b4 (extract-bit-field 8 32 f1))
                      (b5 (extract-bit-field 8 24 f1))
                      (b6 (extract-bit-field 8 16 f1))
                      (b7 (extract-bit-field 8 8 f1))
                      (b8 (extract-bit-field 8 0 f1)))
                 (list (if (negative? num) 128 0) b2 b3 b4 b5 b6 b7 b8)))
              ((> e0 4095) ; infinity
               (list (if (negative? num) 255 127) 224 0 0 0 0 0 0))
              (else
               (let* ((b0 (extract-bit-field 7 4 e0))
                      (b1 (if (negative? num) (+ b0 128) b0))
                      (b2 (bitwise-ior (arithmetic-shift
                                        (extract-bit-field 4 0 e0)
                                        4)
                                       (extract-bit-field 4 48 f)))
                      (b3 (extract-bit-field 8 40 f))
                      (b4 (extract-bit-field 8 32 f))
                      (b5 (extract-bit-field 8 24 f))
                      (b6 (extract-bit-field 8 16 f))
                      (b7 (extract-bit-field 8 8 f))
                      (b8 (extract-bit-field 8 0 f)))
                 (list b1 b2 b3 b4 b5 b6 b7 b8)))))))))))

;; --
;; Reader Hacks

; Commented out since incompatible with R5RS, and float vector hacks
; are ommited entirely.

; (define (vector-reader char port)
;   (define (err)
;     (reading-error port "expected 8, 16, 32, or 64"))
  
;   (define (s fs fu)
;     (lambda (args)
;       (apply (if (char=? #\s char) fs fu) args)))
  
;   (read-char port)
;   (let ((f (case (read-char port)
;              ((#\8) (s s8vector u8vector))
;              ((#\1) (case (read-char port)
;                       ((#\6) (s s16vector u16vector))
;                       (else (err))))
;              ((#\3) (case (read-char port)
;                       ((#\2) (s s32vector u32vector))
;                       (else (err))))
;              ((#\6) (case (read-char port)
;                       ((#\4) (s s64vector u64vector))
;                       (else (err))))
;              (else (err)))))
;     (f (sub-read-carefully port))))

; (define-sharp-macro #\s vector-reader)
; (define-sharp-macro #\u vector-reader)

