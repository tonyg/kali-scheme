; SRFI 4: Homogeneous numeric vector datatypes
; Does not include hacks to the reader.
; Does not include float vectors.

; TODO: implement float vectors.

(define (sub1 i) (- i 1))

(define-syntax define-vector-types
  (syntax-rules ()
    ((define-vector-types spec ...)
     (begin (define-vector-type spec) ...))))
       
(define-syntax define-vector-type  
  (syntax-rules ()
    ((define-vector-types
       (size :tagvector
             tagvector? make-tagvector tagvector tagvector-length
             tagvector-ref tagvector-set! tagvector->list list->tagvector
             blob-ref blob-set!))
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
                (blob-set! b (* size (sub1 i)) x))))))
              
       (define (tagvector . elems)
         (let* ((n (length elems))
                (v (really-make-tagvector (make-blob (* size n))))
                (b (tagvector-blob v)))
           (do ((i n (sub1 i))
                (e (reverse elems) (cdr e)))
               ((zero? i) v)
             (blob-set! b (* size (sub1 i)) (car e)))))
       
       (define (tagvector-length v)
         (/ (blob-length (tagvector-blob v)) size))

       (define (tagvector-ref v i)
         (blob-ref (tagvector-blob v) (* size i)))
       
       (define (tagvector-set! v i n)
         (blob-set! (tagvector-blob v) (* size i) n))
       
       (define (tagvector->list v)
         (blob->uint-list size (endianness native) (tagvector-blob v)))
       
       (define (list->tagvector ls)
         (really-make-tagvector
          (uint-list->blob size (endianness native) ls)))
       
       ))))

(define-vector-types
  (1 :s8vector 
     s8vector? make-s8vector s8vector s8vector-length
     s8vector-ref s8vector-set! s8vector->list list->s8vector
     blob-s8-ref blob-s8-set!)
  
  (1 :u8vector
     u8vector? make-u8vector u8vector u8vector-length
     u8vector-ref u8vector-set! u8vector->list list->u8vector
     blob-u8-ref blob-u8-set!)
 
  (2 :s16vector
     s16vector? make-s16vector s16vector s16vector-length
     s16vector-ref s16vector-set! s16vector->list list->s16vector
     blob-s16-native-ref blob-s16-native-set!)  
  
  (2 :u16vector
     u16vector? make-u16vector u16vector u16vector-length
     u16vector-ref u16vector-set! u16vector->list list->u16vector
     blob-u16-native-ref blob-u16-native-set!)

  (4 :s32vector
     s32vector? make-s32vector s32vector s32vector-length
     s32vector-ref s32vector-set! s32vector->list list->s32vector
     blob-s32-native-ref blob-s32-native-set!)  
  
  (4 :u32vector
     u32vector? make-u32vector u32vector u32vector-length
     u32vector-ref u32vector-set! u32vector->list list->u32vector
     blob-u32-native-ref blob-u32-native-set!)  
  
  (8 :s64vector
     s64vector? make-s64vector s64vector s64vector-length
     s64vector-ref s64vector-set! s64vector->list list->s64vector
     blob-s64-native-ref blob-s64-native-set!)  
  
  (8 :u64vector
     u64vector? make-u64vector u64vector u64vector-length u64vector-ref
     u64vector-set! u64vector->list list->u64vector
     blob-u64-native-ref blob-u64-native-set!))

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

