; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define max-stob-size-in-cells
  (+ max-stob-contents-size-in-cells
     stob-overhead))

(define (make-stob type len key)
  (let ((addr (s48-allocate-space type (+ len (cells->bytes stob-overhead)) key)))
    (store! addr (make-header type len))
    (address->stob-descriptor (address+ addr (cells->bytes stob-overhead)))))

(define (make-d-vector type len key)
  (make-stob type (cells->bytes len) key))

(define make-b-vector make-stob)

; Used to copy stuff from the stack to the heap.

(define (header+contents->stob header contents key)
  (let* ((addr (s48-allocate-space (header-type header)
				   (+ (header-length-in-bytes header)
				      (cells->bytes stob-overhead))
				   key))
	 (data-addr (address+ addr (cells->bytes stob-overhead))))
    (store! addr header)
    (copy-memory! contents data-addr (header-length-in-bytes header))
    (address->stob-descriptor data-addr)))

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
  (fetch (address+ (address-after-header x) (cells->a-units index))))

(define (d-vector-set! x index value)
  (assert (valid-index? index (d-vector-length x)))
  (let ((addr (address+ (address-after-header x) (cells->a-units index))))
    (s48-write-barrier x addr value)
    (store! addr value)))

(define (d-vector-init! x index value)
  (assert (valid-index? index (d-vector-length x)))
  (store! (address+ (address-after-header x) (cells->a-units index))
          value))

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
  (fetch-byte (address+ (address-after-header b-vector) index)))

(define (b-vector-set! b-vector index value)
  (assert (valid-index? index (b-vector-length b-vector)))
  (store-byte! (address+ (address-after-header b-vector) index) value))

; Various utilities

(define (valid-index? index len)
  (and (>= index 0) (< index len)))


