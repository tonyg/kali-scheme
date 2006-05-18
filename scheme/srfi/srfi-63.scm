;; SRFI 63: Homogeneous and Heterogeneous Arrays

;; Until the SRFI 4 implementation includes float vectors,
;; these definitions are needed:

(define make-f64vector make-vector)
(define f64vector-ref vector-ref)
(define f64vector-set! vector-set!)

(define make-f32vector make-vector)
(define f32vector-ref vector-ref)
(define f32vector-set! vector-set!)

(define-syntax enumerate
  (syntax-rules ()
    ((enumerate name (const val) ...)
     (define-syntax name
       (syntax-rules (const ...)
         ((name const) val) ...)))))
  
(enumerate a:
 (vector   0)
 (floc128b 1) (floc64b  2) (floc32b  3) (floc16b  4)
 (flor128b 5) (flor64b  6) (flor32b  7) (flor16b  8)
 (floq128d 9) (floq64d 10) (floq32d 11)
 (fixz64b 12) (fixz32b 13) (fixz16b 14) (fixz8b  15)
 (fixn64b 16) (fixn32b 17) (fixn16b 18) (fixn8b  19)
 (bool    20)
 (string  21))

;; This implementation uses SRFI-4 vectors as the store for
;; several of the homogeneous array types, but several types
;; are implemented using plain vectors.  To improve the 
;; implementation, simply update the appropriate entry in
;; this table.
(define implementation-list
  (let ((ls list))
    (ls (ls (a: vector)   make-vector    vector-ref    vector-set!)
        (ls (a: floc128b) make-vector    vector-ref    vector-set!)
        (ls (a: floc64b)  make-f64vector f64vector-ref f64vector-set!)
        (ls (a: floc32b)  make-f32vector f32vector-ref f32vector-set!)
        (ls (a: floc16b)  make-vector    vector-ref    vector-set!)
        (ls (a: flor128b) make-vector    vector-ref    vector-set!)
        (ls (a: flor64b)  make-vector    vector-ref    vector-set!)
        (ls (a: flor32b)  make-vector    vector-ref    vector-set!)
        (ls (a: flor16b)  make-vector    vector-ref    vector-set!)
        (ls (a: floq128d) make-vector    vector-ref    vector-set!)
        (ls (a: floq64d)  make-vector    vector-ref    vector-set!)
        (ls (a: floq32d)  make-vector    vector-ref    vector-set!)
        (ls (a: fixz64b)  make-s64vector s64vector-ref s64vector-set!)
        (ls (a: fixz32b)  make-s32vector s32vector-ref s32vector-set!)
        (ls (a: fixz16b)  make-s16vector s16vector-ref s16vector-set!)
        (ls (a: fixz8b)   make-s8vector  s8vector-ref  s8vector-set!)
        (ls (a: fixn64b)  make-u64vector u64vector-ref u64vector-set!)
        (ls (a: fixn32b)  make-u32vector u32vector-ref u32vector-set!)
        (ls (a: fixn16b)  make-u16vector u16vector-ref u16vector-set!)
        (ls (a: fixn8b)   make-u8vector  u8vector-ref  u8vector-set!)
        (ls (a: bool)     make-vector    vector-ref    vector-set!)    
        (ls (a: string)   make-string    string-ref    string-set!))))
  
(define (add1 i) (+ i 1))
(define (sub1 i) (- i 1)) 

(define-record-type strict-array :strict-array
  (make-strict-array dimensions scales offset store store-type)
  strict-array?
  (dimensions strict-array-dimensions)
  (scales strict-array-scales)
  (offset strict-array-offset)
  (store strict-array-store)
  (store-type strict-array-store-type))
    
(define (array-dimensions array)
  (cond ((vector? array) (list (vector-length array)))
        ((string? array) (list (string-length array)))
        (else (strict-array-dimensions array))))

(define (array-scales array)
  (cond ((string? array) '(1))
        ((vector? array) '(1))
        (else (strict-array-scales array))))

(define (array-store array)
  (cond ((string? array) array)
        ((vector? array) array)
        (else (strict-array-store array))))

(define store-makers
  (apply vector
         (map (lambda (item) (list-ref item 1)) implementation-list)))

(define store-reffers
  (apply vector
         (map (lambda (item) (list-ref item 2)) implementation-list)))

(define store-setters
  (apply vector
         (map (lambda (item) (list-ref item 3)) implementation-list)))

(define (array-store-type array)
  (cond ((string? array) (a: string))
        ((vector? array) (a: vector))
        (else (strict-array-store-type array))))

(define (array-store-ref array)
  (vector-ref store-reffers (array-store-type array)))

(define (array-store-set array)
  (vector-ref store-setters (array-store-type array)))

(define (array-store-maker array-type)
  (vector-ref store-makers array-type))

(define (array-offset array)
  (cond ((string? array) 0)
        ((vector? array) 0)
        (else (strict-array-offset array))))

(define (array? obj)
  (or (string? obj)
      (vector? obj)
      (strict-array? obj)))

(define (equal? obj1 obj2) 
  (cond ((eqv? obj1 obj2) #t)
        ((pair? obj1)
         (and (pair? obj2)
              (equal? (car obj1) (car obj2))
              (equal? (cdr obj1) (cdr obj2))))

        ;; The logic in these two cases were rearranged from s48's
        ;; original equal? to handle things like (equal? "" '#()).
        
        ((and (string? obj1) (string? obj2))
         (string=? obj1 obj2))
        ((and (vector? obj1) (vector? obj2))
         (let ((z (vector-length obj1)))
           (and (= z (vector-length obj2))
                (let loop ((i 0))
                  (cond ((= i z) #t)
                        ((equal? (vector-ref obj1 i) (vector-ref obj2 i))
                         (loop (add1 i)))
                        (else #f))))))
        ((array? obj1)
         (and (array? obj2)
              (equal? (array-dimensions obj1)
                      (array-dimensions obj2))
              (equal? (array->vector obj1)
                      (array->vector obj2))))
        
        (else #f)))

(define (array-rank x)
  (if (array? x)
      (length (array-dimensions x))
      0))

(define (make-array prototype . dimensions)
  (let ((prot    (array-store prototype))
        (pdims   (array-dimensions prototype))
        (onedim? (eqv? 1 (length dimensions)))
        (tcnt    (apply * dimensions)))    
    (let ((initializer
           (if (zero? (apply * pdims)) '()
               (list      ;; a list with single element at origin
                (apply array-ref prototype
                       (map (lambda (x) 0) pdims))))))    
      
      (cond ((and onedim? (string? prot))
             (apply make-string (car dimensions) initializer))
            ((and onedim? (vector? prot))
             (apply make-vector (car dimensions) initializer))            
            (else
             (let* ((store-type (array-store-type prototype))
                    (store (apply (array-store-maker store-type)
                                  tcnt initializer)))
               (let loop ((dims (reverse dimensions)) (scales '(1)))
                 (if (null? dims)
                     (make-strict-array dimensions (cdr scales) 0 
                                        store
                                        store-type)
                     (loop (cdr dims)
                           (cons (* (car dims) (car scales)) scales))))))))))

(define (make-shared-array array mapper . dimensions)
  (define odl (array-scales array))
  (define rank (length dimensions))
  (define shape
    (map (lambda (dim) (if (list? dim) dim (list 0 (sub1 dim)))) dimensions))
  
  (do ((idx (sub1 rank) (sub1 idx))
       (uvt (if (zero? rank)
                '()
                (append (cdr (vector->list (make-vector rank 0))) '(1)))
            (append (cdr uvt) '(0)))
       (uvts '() (cons uvt uvts)))
      ((negative? idx)
       (let ((ker0 (apply + (map * odl (apply mapper uvt)))))
	 (make-strict-array
	  (map (lambda (dim) (add1 (- (cadr dim) (car dim)))) shape)
	  (map (lambda (uvt) (- (apply + (map * odl (apply mapper uvt))) ker0))
	       uvts)
	  (apply +
		 (array-offset array)
		 (map * odl (apply mapper (map car shape))))
	  (array-store array)
          (array-store-type array))))))

(define (list->array rank proto lst)
  (define dimensions
    (do ((shp '() (cons (length row) shp))
         (row lst (car lst))
         (rnk (sub1 rank) (sub1 rnk)))
	((negative? rnk) (reverse shp))))
  (let ((nra (apply make-array proto dimensions)))
    (define (l2ra dims idxs row)
      (cond ((null? dims)
             (apply array-set! nra row (reverse idxs)))
            (;; ERROR CHECKING
             (if (not (eqv? (car dims) (length row)))
                 (error "non-rectangular array" dims dimensions))
             
             (do ((idx 0 (add1 idx))
                  (row row (cdr row)))
		 ((>= idx (car dims)))
               (l2ra (cdr dims) (cons idx idxs) (car row))))))
    (l2ra dimensions '() lst)
    nra))

(define (array->list ra)
  (define (ra2l dims idxs)
    (if (null? dims)
        (apply array-ref ra (reverse idxs))
        (do ((lst '() (cons (ra2l (cdr dims) (cons idx idxs)) lst))
             (idx (sub1 (car dims)) (sub1 idx)))
	    ((negative? idx) lst))))
  (ra2l (array-dimensions ra) '()))

(define (vector->array vect prototype . dimensions)
  (let ((vdx (vector-length vect))
        (ra (apply make-array prototype dimensions)))
    (define (v2ra dims idxs)
      (cond ((null? dims)
             (set! vdx (sub1 vdx))
             (apply array-set! ra (vector-ref vect vdx) (reverse idxs)))
            (else
             (do ((idx (sub1 (car dims)) (sub1 idx)))
		 ((negative? idx) vect)
               (v2ra (cdr dims) (cons idx idxs))))))
    (v2ra dimensions '())
    ra))

(define (array->vector ra)
  (define dims (array-dimensions ra))
  (let* ((vdx (apply * dims))
         (vect (make-vector vdx)))
    (define (ra2v dims idxs)
      (if (null? dims)
          (let ((val (apply array-ref ra (reverse idxs))))
            (set! vdx (sub1 vdx))
            (vector-set! vect vdx val))
          (do ((idx (sub1 (car dims)) (sub1 idx)))
	      ((negative? idx) vect)
            (ra2v (cdr dims) (cons idx idxs)))))
    (ra2v dims '())
    vect))

(define (array-in-bounds? array . indices)
  (do ((bnds (array-dimensions array) (cdr bnds))
       (idxs indices (cdr idxs)))
      ((or (null? bnds)
	   (null? idxs)
	   (not (integer? (car idxs)))
	   (not (< -1 (car idxs) (car bnds))))
       (and (null? bnds) (null? idxs)))))

(define (array-ref array . indices)
  ((array-store-ref array)
   (array-store array)
   (apply + (array-offset array) (map * (array-scales array) indices))))

(define (array-set! array obj . indices)    
  ((array-store-set array)
   (array-store array)
   (apply + (array-offset array) (map * (array-scales array) indices))
   obj))

(define (tag-maker array-type)
  (case-lambda
    (()  (make-strict-array 
          '(0) '(1) 0 
          ((array-store-maker array-type) 0)
          array-type))
    
    ((x) (make-strict-array 
          '(1) '(1) 0
          ((array-store-maker array-type) 1 x)
          array-type))))

(define a:floc128b (tag-maker (a: floc128b)))
(define a:floc64b  (tag-maker (a: floc64b)))
(define a:floc32b  (tag-maker (a: floc32b)))
(define a:floc16b  (tag-maker (a: floc16b)))

(define a:flor128b (tag-maker (a: flor128b)))
(define a:flor64b  (tag-maker (a: flor64b)))
(define a:flor32b  (tag-maker (a: flor32b)))
(define a:flor16b  (tag-maker (a: flor16b)))

(define a:floq128d (tag-maker (a: floq128d)))
(define a:floq64d  (tag-maker (a: floq64d)))
(define a:floq32d  (tag-maker (a: floq32d)))

(define a:fixz64b  (tag-maker (a: fixz64b)))
(define a:fixz16b  (tag-maker (a: fixz16b)))
(define a:fixz32b  (tag-maker (a: fixz32b)))
(define a:fixz8b   (tag-maker (a: fixz8b)))

(define a:fixn64b  (tag-maker (a: fixn64b)))
(define a:fixn32b  (tag-maker (a: fixn32b)))
(define a:fixn16b  (tag-maker (a: fixn16b)))
(define a:fixn8b   (tag-maker (a: fixn8b)))

(define a:bool     (tag-maker (a: bool)))
