; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file data.scm.
; Requires DEFINE-ENUMERATION macro.

;;;; Data representations

; This implementation of the data representations is particularly
; tuned for byte-addressable machines with 4 bytes per word.
; Good representations for other kinds of machines would necessarily
; look quite different; e.g. on a word-addressed machine you might
; want to put tag bits in the high end of a word, or even go to some
; kind of BIBOP system.

; Utility bit-hacking functions

(define (adjoin-bits high low k)
  (+ (ashl high k) low))

(define (low-bits n k)
  (bitwise-and n (- (ashl 1 k) 1)))

(define high-bits ashr)

; Descriptors
;  A descriptor describes a Scheme object.
;  A descriptor is represented as an integer whose low two bits are
;  tag bits.  The high bits contain information whose format and
;  meaning are dependent on the tag.

(define tag-field-width 2)
(define data-field-width (- bits-per-cell tag-field-width))

(define (make-descriptor tag data)
  (adjoin-bits data tag tag-field-width))

(define (descriptor-tag descriptor)
  (low-bits descriptor tag-field-width))

(define (descriptor-data descriptor)
  (high-bits descriptor tag-field-width))

(define (set-descriptor-tag! proto-descriptor tag)
  (assert (= 0 (descriptor-tag proto-descriptor)))
  (+ proto-descriptor tag))

(define vm-eq? =)

; The four tags are: fixnum, immediate (character, boolean, etc.),
; header (gives the type and size of a stored object), and stored
; (pointer into memory).
; The header and immediate tags could be multiplexed, thus freeing up
; one of the 4 type codes for some other purpose, but the
; implementation is simpler if they're not.

(define-enumeration tag
  (fixnum
   immediate
   header
   stob))

;; (assert (>= (ashl 1 tag-field-width)
;;             (vector-length tag)))

(define (fixnum? descriptor)
  (= (descriptor-tag descriptor) (enum tag fixnum)))

(define (immediate? descriptor)
  (= (descriptor-tag descriptor) (enum tag immediate)))

(define (header? descriptor)
  (= (descriptor-tag descriptor) (enum tag header)))

(define (stob? descriptor)
  (= (descriptor-tag descriptor) (enum tag stob)))

; Fixnums

(define bits-per-fixnum
  (- (if (< bits-per-cell useful-bits-per-word)
          bits-per-cell
          useful-bits-per-word)
      tag-field-width))

(define    least-fixnum-value (- 0 (ashl 1 (- bits-per-fixnum 1))))
(define greatest-fixnum-value (-   (ashl 1 (- bits-per-fixnum 1)) 1))

(define (too-big-for-fixnum? n)
  (> n greatest-fixnum-value))

(define (too-small-for-fixnum? n)
  (< n least-fixnum-value))

(define (overflows? n)
  (or (too-big-for-fixnum? n)
      (too-small-for-fixnum? n)))

(define (enter-fixnum n)
  (assert (not (overflows? n)))
  (make-descriptor (enum tag fixnum) n))

(define (extract-fixnum p)
  (assert (fixnum? p))
  (descriptor-data p))

; Generic number stuff

(define (vm-number? x)
  (or (fixnum? x)
      (extended-number? x)))

(define (carefully op)
  (lambda (x y succ fail)
    (let ((z (op (extract-fixnum x) (extract-fixnum y))))
      (if (overflows? z)
          (goto fail x y)
          (goto succ (enter-fixnum z))))))

(define add-carefully (carefully +))
(define subtract-carefully (carefully -))

(define half-word-size (quotient bits-per-fixnum 2))
(define half-word-mask (- (ashl 1 half-word-size) 1))
(define max-middle (- (ashl 1 (+ 1 (- bits-per-fixnum half-word-size)))
		       1))

(define (multiply-carefully x y succ fail)
  (let* ((a (extract-fixnum x))
         (b (extract-fixnum y))
         (positive-result? (if (>= a 0)
                               (>= b 0)
                               (< b 0)))
         (a (abs a))
         (b (abs b))
	 (lo-a (bitwise-and half-word-mask a))
	 (lo-b (bitwise-and half-word-mask b))
         (hi-a (bitwise-and half-word-mask (high-bits a half-word-size)))
         (hi-b (bitwise-and half-word-mask (high-bits b half-word-size)))
	 (lo-c (* lo-a lo-b))
	 (mid-c (+ (* lo-a hi-b) (* lo-b hi-a)))
	 (c (+ lo-c (ashl mid-c half-word-size))))
    (cond ((or (and (> hi-a 0) (> hi-b 0))
	       (too-big-for-fixnum? lo-c)
	       (> mid-c max-middle))
	   (goto fail x y))
	  (positive-result?
	   (if (too-big-for-fixnum? c)
	       (goto fail x y)
	       (goto succ (enter-fixnum c))))
	  (else
	   (if (too-small-for-fixnum? (- 0 c))
	       (goto fail x y)
	       (goto succ (enter-fixnum (- 0 c))))))))

; Test cases for bits-per-cell = 28, bits-per-fixnum = 26

;   (do ((i 2 (* i 2))
;        (j (* -2 (expt 2 23)) (/ j 2)))
;       ((>= j 0) 'ok)
;     (write `((* ,i ,j) ?=? ,(* i j)))
;     (newline))

(define (divide-carefully x y succ fail)
  (quotient-carefully x y
                      (lambda (q)
                        (remainder-carefully x y
                                             (lambda (r)
                                               (if (= r (enter-fixnum 0))
                                                   (goto succ q)
                                                   (goto fail x y)))
                                             fail))
                      fail))

; Watch out for (quotient least-fixnum -1)
(define (quotient-carefully x y succ fail)
  (if (= y (enter-fixnum 0))
      (fail x y)
      (let* ((a (extract-fixnum x))
	     (b (extract-fixnum y))
	     (positive-result? (if (>= a 0)
				   (>= b 0)
				   (< b 0)))
	     (a (abs a))
	     (b (abs b))
	     (c (quotient a b)))
	(cond ((not positive-result?)
	       (goto succ (enter-fixnum (- 0 c))))
	      ((too-big-for-fixnum? c)  ; (quotient least-fixnum -1)
	       (goto fail x y))
	      (else
	       (goto succ (enter-fixnum c)))))))

; Overflow check not necessary
(define (remainder-carefully x y succ fail)
  (if (= y (enter-fixnum 0))
      (goto fail x y)
      (let* ((a (extract-fixnum x))
	     (b (extract-fixnum y))
	     (positive-result? (>= a 0))
	     (a (abs a))
	     (b (abs b))
	     (c (remainder a b)))
	(goto succ (enter-fixnum (if positive-result? c (- 0 c)))))))

(define (shift-carefully value+tag count+tag succ fail)
  (let ((value (extract-fixnum value+tag))
	(count (extract-fixnum count+tag)))
    (if (< count 0)
	(goto succ (enter-fixnum (ashr value (- 0 count))))
	(let ((result (extract-fixnum (enter-fixnum (ashl value count)))))
	  (if (and (= value (ashr result count))
		   (if (>= value 0)
		       (>= result 0)
		       (< result 0)))
	      (goto succ (enter-fixnum result))
	      (goto fail value+tag count+tag))))))

; These happen to work out, given our representation for fixnums.
(define vm-= =)
(define vm-< <)

; Immediates
;  The number 8 is chosen to streamline 8-bit-byte-oriented implementations.

(define immediate-type-field-width
  (- 8 tag-field-width))

(define (make-immediate type info)
  (make-descriptor (enum tag immediate)
                   (adjoin-bits info type immediate-type-field-width)))

(define (immediate-type imm)
  (assert (immediate? imm))
  (low-bits (descriptor-data imm)
             immediate-type-field-width))

(define (immediate-info imm)
  (assert (immediate? imm))
  (high-bits (descriptor-data imm)
              immediate-type-field-width))

(define (tag&immediate-type descriptor)
  (low-bits descriptor (+ tag-field-width immediate-type-field-width)))

(define (make-tag&immediate-type type)
  (adjoin-bits type (enum tag immediate) tag-field-width))

(define-enumeration imm
  (false      ; #f
   true       ; #t
   char
   unspecific
   undefined
   eof
   null))

;; (assert (>= (ashl 1 immediate-type-field-width)
;;             (vector-length imm)))

(define (immediate-predicate type)
  (lambda (descriptor)
    ;; Check low 8 bits...
    (= (tag&immediate-type descriptor)
	(make-tag&immediate-type type))))

(define false?     (immediate-predicate (enum imm false)))
(define vm-char?   (immediate-predicate (enum imm char)))
(define undefined? (immediate-predicate (enum imm undefined)))

(define true          (make-immediate (enum imm true) 0))
(define false         (make-immediate (enum imm false) 0))
(define eof-object    (make-immediate (enum imm eof)  0))
(define null          (make-immediate (enum imm null) 0))
(define unspecific    (make-immediate (enum imm unspecific) 0))
(define quiescent         (make-immediate (enum imm undefined) 0))
(define unbound-marker    (make-immediate (enum imm undefined) 1))
(define unassigned-marker (make-immediate (enum imm undefined) 2))

(define (enter-boolean b)
  (if b true false))

(define (extract-boolean b)
  (assert (vm-boolean? b))
  (if (vm-eq? b false) #f #t))

(define (vm-boolean? x)
  (or (vm-eq? x false)
      (vm-eq? x true)))

; Characters

(define (enter-char c)
  (make-immediate (enum imm char) (char->ascii c)))

(define (extract-char d)
  (assert (vm-char? d))
  (ascii->char (immediate-info d)))

(define vm-char=? char=?)
(define vm-char<? char<?)
(define vm-char->ascii char->ascii)
(define vm-ascii->char ascii->char)

; Headers

(define header-type-field-width (- immediate-type-field-width 1))

(define (make-header type length-in-bytes)
  (make-descriptor (enum tag header) (adjoin-bits length-in-bytes
                                            type
                                            (+ 1 header-type-field-width))))

(define header-immutable-bit-mask
  (adjoin-bits 1 0 (+ header-type-field-width tag-field-width)))

(define (make-header-immutable header)
  (bitwise-ior header header-immutable-bit-mask))

(define (header-type h)
  (assert (header? h))
  (low-bits (descriptor-data h)
             header-type-field-width))

(define (immutable-header? h)
  (assert (header? h))
  (not (= 0 (bitwise-and h header-immutable-bit-mask))))

(define (header-length-in-bytes h)
  (assert (header? h))
  (high-bits (descriptor-data h)
	     (+ 1 header-type-field-width)))

(define (header-length-in-cells header)
  (bytes->cells (header-length-in-bytes header)))

(define (header-a-units h)   ;Used by GC to find end of any object
  (bytes->a-units (header-length-in-bytes h)))

(define (immutable? thing)
  (or (not (stob? thing))
      (immutable-header? (stob-header thing))))

(define (make-immutable! thing)
  (if (not (immutable? thing))
      (stob-header-set! thing (make-header-immutable (stob-header thing)))))

; Stored objects
;  The data field of a descriptor for a stored object contains the
;  cell number of the first cell after the object's header cell.

;;; Moved from STRUCT to get LEAST-B-VECTOR-TYPE in this file.

(define least-b-vector-type (enum stob string))

(define (make-stob-descriptor addr)
  (set-descriptor-tag! addr (enum tag stob)))

(define (address-at-header stob)
  (assert (stob? stob))
  (- stob (+ (enum tag stob) addressing-units-per-cell))) 

(define (address-after-header stob)
  (assert (stob? stob))
  (- stob (enum tag stob)))

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


; Allocation
;  *hp* is the heap pointer and *limit* is the limit beyond which no
;  storage should be allocated.  Both of these are addresses (not
;  descriptors).

(define check-preallocation? #f)

(define *hp* 0)
(define *limit* 0)

(define (available? cells)
  (addr< (addr+ *hp* (cells->a-units cells)) *limit*))

(define (available)
  (a-units->cells (addr- *limit* *hp*)))

(define *heap-key* 0)
(define universal-key 0)
(define *okayed-space* 0)

(define (preallocate-space cells)
  (cond (check-preallocation?
	 (assert (available? cells))
	 (set! *heap-key* (+ *heap-key* 1))
	 (set! *okayed-space* cells)
	 *heap-key*)
	(else
	 universal-key)))

(define (make-stob type len key)	;len is in bytes
  (if check-preallocation?
      (let ((cells (+ (bytes->cells len) 1)))
	(assert (available? cells))
	(cond ((= key universal-key) 0)
	      ((not (and (= key *heap-key*)
			 (>= *okayed-space* cells)))
	       (error "invalid heap key" key cells))
	      (else
	       (set! *okayed-space* (- *okayed-space* cells))))))
  (store! *hp* (make-header type len))  ;(store-next!)
  (set! *hp* (addr1+ *hp*))
  (let ((new (make-stob-descriptor *hp*)))
    (set! *hp* (addr+ *hp* (bytes->a-units len)))
    (if (> len 0)			; for B-VECTORs that don't want to use all of the
        (store! (addr+ *hp* (cells->bytes -1)) 0)) ; last descriptor
    new))                               ; use all of the last descriptor

(define (copy-stob stob key)
  (assert (stob? stob))
  (let ((new (make-stob (header-type (stob-header stob))
			(header-length-in-bytes (stob-header stob))
			key)))
    (copy-stob! stob new)
    new))  

(define (copy-stob! from to)
  (copy-cells! (address-after-header from)
               (address-after-header to)
               (bytes->cells (stob-length-in-bytes from))))

(define (copy-cells! from to count)
  (let ((end (addr+ from (cells->a-units count))))
    (do ((from from (addr1+ from))
         (to to (addr1+ to)))
        ((>= from end))
      (store! to (fetch from)))))

; D-vectors (vectors of descriptors)

(define (d-vector-header? h)
  (< (header-type h) least-b-vector-type))

(define (d-vector? obj)
  (and (stob? obj)
       (< (header-type (stob-header obj)) least-b-vector-type)))

(define (make-d-vector type len key)
  (make-stob type (cells->bytes len) key))

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

(define (b-vector-header? h)
  (and (header? h)
       (>= (header-type h) least-b-vector-type)))

(define (b-vector? obj)
  (and (stob? obj)
       (>= (header-type (stob-header obj))
            least-b-vector-type)))

(define make-b-vector make-stob)

(define (b-vector-length x)
  (assert (b-vector? x))
  (header-length-in-bytes (stob-header x)))


(define (b-vector-ref b-vector index)
  (assert (valid-index? index (b-vector-length b-vector)))
  (fetch-byte (addr+ (address-after-header b-vector) index)))

(define (b-vector-set! b-vector index value)
  (assert (valid-index? index (b-vector-length b-vector)))
  (store-byte! (addr+ (address-after-header b-vector) index) value))




