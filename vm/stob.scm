
(define (make-d-vector type len key)
  (make-stob type (cells->bytes len) key))

(define make-b-vector make-stob)

(define (address-after-header stob)
  (assert (stob? stob))
  (stob-descriptor->address stob))

(define (address-at-header stob)
  (addr- (address-after-header stob) (cells->a-units 1)))

(define (stob-length-in-bytes stob)
  (header-length-in-bytes (stob-header stob)))

(define (address-after-stob stob)
  (addr+ (address-after-header stob)
         (bytes->a-units (stob-length-in-bytes stob))))

; Accessing memory via stob descriptors

(define (stob-ref stob index)
  (fetch (addr+ (address-after-header stob) (cells->a-units index))))

(define (stob-set! stob index value)
  (store! (addr+ (address-after-header stob) (cells->a-units index))
          value))

(define (stob-header stob)
  (stob-ref stob -1))

(define (stob-header-set! stob header)
  (stob-set! stob -1 header))

(define (stob-type obj)
  (header-type (stob-header obj)))

(define (stob-of-type? obj type)
  (and (stob? obj)
       (= (stob-type obj) type)))

; Immutability

(define (immutable? thing)
  (or (not (stob? thing))
      (immutable-header? (stob-header thing))))

(define (make-immutable! thing)
  (if (not (immutable? thing))
      (stob-header-set! thing (make-header-immutable (stob-header thing)))))

; D-vectors (vectors of descriptors)

(define (d-vector? obj)
  (and (stob? obj)
       (< (header-type (stob-header obj)) least-b-vector-type)))

; The type in these routines is used only for internal error checking.

(define (d-vector-length x)
  (assert (d-vector? x))
  (header-length-in-cells (stob-header x)))

(define (d-vector-ref x index)
  (assert (valid-index? index (d-vector-length x)))
  (stob-ref x index))

(define (d-vector-set! x index val)
  (assert (valid-index? index (d-vector-length x)))
  (stob-set! x index val))

; B-vector = vector of bytes.

(define (b-vector? obj)
  (and (stob? obj)
       (>= (header-type (stob-header obj))
	   least-b-vector-type)))

(define (b-vector-length x)
  (assert (b-vector? x))
  (header-length-in-bytes (stob-header x)))

(define (b-vector-ref b-vector index)
  (assert (valid-index? index (b-vector-length b-vector)))
  (fetch-byte (addr+ (address-after-header b-vector) index)))

(define (b-vector-set! b-vector index value)
  (assert (valid-index? index (b-vector-length b-vector)))
  (store-byte! (addr+ (address-after-header b-vector) index) value))

; Various utilities

(define (copy-stob stob key)
  (assert (stob? stob))
  (let ((new (make-stob (header-type (stob-header stob))
			(header-length-in-bytes (stob-header stob))
			key)))
    (copy-cells! (address-after-header stob)
		 (address-after-header new)
		 (bytes->cells (stob-length-in-bytes stob)))
    new))

(define (copy-cells! from to count)
  (let ((end (addr+ from (cells->a-units count))))
    (do ((from from (addr1+ from))
         (to to (addr1+ to)))
        ((>= from end))
      (store! to (fetch from)))))

(define (stob-equal? stob1 stob2)    ;CMPC3 loop or "strncmp"
  (let ((z1 (stob-header stob1))
        (z2 (stob-header stob2)))
    (and (= (make-header-immutable z1)
	    (make-header-immutable z2))
         (let ((z (header-length-in-cells z1)))
           (let loop ((i 0))
             (cond ((>= i z) #t)
                   ((= (stob-ref stob1 i)
                        (stob-ref stob2 i))
                    (loop (+ i 1)))
                   (else #f)))))))

(define (valid-index? index len)
  (and (>= index 0) (< index len)))


