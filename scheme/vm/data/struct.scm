; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file struct.scm.

; This file defines a level of abstraction for storage somewhat higher
; than that of d-vectors and b-vectors: pairs, symbols, and other datatypes.

(define (stob-maker type maker)
  (lambda (length key)
    (maker type length key)))

(define (stob-predicate type)
  (lambda (obj) (stob-of-type? obj type)))

; data for these comes from STOB-DATA in arch.scm

(define-shared-primitive-data-type pair #t)
(define-shared-primitive-data-type symbol #t #f
  make-symbol                           		; hidden from RTS
  ()
  (symbol-next set-symbol-next!))       		; hidden from RTS
(define-shared-primitive-data-type closure #f #f)
(define-shared-primitive-data-type location)
(define-shared-primitive-data-type cell)

(define-shared-primitive-data-type weak-pointer)

(define-shared-primitive-data-type shared-binding #f #f
  #f
  ()
  (shared-binding-next set-shared-binding-next!))	; hidden from RTS

(define-shared-primitive-data-type port)
(define-shared-primitive-data-type channel #f #f
  make-channel                           		; hidden from RTS
  (;; these setters are hidden from the RTS
   (channel-status          set-channel-status!)
   (channel-id              set-channel-id!)
   (channel-os-index        set-channel-os-index!)
   (channel-close-silently? set-channel-close-silently?!))
  ;; none of these are visible to the RTS
  (channel-next      set-channel-next!)
  ;; this is
  ;; false - if there's nothing going on
  ;; true - if there's an operation pending
  ;; the number of bytes transferred - if error? (below) is false
  ;; the error code - if error? (below) is true
  (channel-os-status set-channel-os-status!)
  (channel-error?    set-channel-error?!))

; Vectors and so on

(define-vector-data-type vector #t)
(define-vector-data-type record)
(define-vector-data-type extended-number)

(define make-bignum  (stob-maker (enum stob byte-vector) make-b-vector))
(define bignum?       (stob-predicate (enum stob bignum)))
(define bignum-length b-vector-length)
(define bignum-ref    b-vector-ref)
(define bignum-set!   b-vector-set!)
(define (bignum-size len)
  (+ stob-overhead (bytes->cells len)))


(define-vector-data-type continuation)
(define-vector-data-type template)

(define (vm-make-vector+gc len)
  (let ((vector (maybe-make-d-vector+gc (enum stob vector)
					len)))
    (if (false? vector)
	(error "Out of space, unable to allocate"))
    vector))

(define (vm-vector-fill! v val)
  (do ((i 0 (+ i 1)))
      ((= i (vm-vector-length v)) v)
    (vm-vector-set! v i val)))

; We use D-VECTOR-INIT! because continuations in the heap are only initialized,
; never modified.

(define-syntax define-cont-field
  (syntax-rules ()
    ((define-cont-field ref set offset)
     (begin
       (define (ref c)     (continuation-ref c offset))
       (define (set c val) (d-vector-init!   c offset val))))))

(define-cont-field continuation-cont set-continuation-cont!
  continuation-cont-index)
(define-cont-field continuation-pc set-continuation-pc!
  continuation-pc-index)
(define-cont-field continuation-code set-continuation-code!
  continuation-code-index)

(define (template-code tem) (template-ref tem 0))
(define (template-byte-code tem) (template-ref tem 1))
(define (template-name tem) (template-ref tem 2))

; Code vectors

(define make-code-vector   (stob-maker (enum stob byte-vector) make-b-vector))
(define code-vector?       (stob-predicate (enum stob byte-vector)))
(define code-vector-length b-vector-length)
(define code-vector-ref    b-vector-ref)
(define code-vector-set!   b-vector-set!)

(define (code-vector-size len)
  (+ stob-overhead (bytes->cells len)))

; for small strings only
(define (vm-make-string length key)
  (make-b-vector (enum stob string)
		 (scalar-value-units->bytes length)
		 key))

(define (vm-make-string+gc length)
  (let ((string (maybe-make-b-vector+gc (enum stob string)
					(scalar-value-units->bytes length))))
    (if (false? string)
	(error "Out of space, unable to allocate"))
    string))

(define vm-string? (stob-predicate (enum stob string)))

(define (vm-string-length x)
  (bytes->scalar-value-units (b-vector-length x)))

; deals in code points, not PreScheme characters
; #### This should be rewritten as a loop the PreScheme compiler can unroll
(define (vm-string-ref s i)
  (let ((base (scalar-value-units->bytes i)))
    (do ((bits 0 (+ bits bits-per-byte))
	 (j 0 (+ 1 j))
	 (scalar-value 0
		     (adjoin-bits (b-vector-ref s (+ base j))
				  scalar-value
				  bits)))
	((>= j bytes-per-scalar-value-unit)
	 scalar-value))))

;; #### ditto
(define (vm-string-set! s i c)
  (let ((base (scalar-value-units->bytes i)))
    (do ((bits 0 (+ bits bits-per-byte))
	 (j 0 (+ 1 j))
	 (shifted c (unsigned-high-bits shifted bits-per-byte)))
	((>= j bytes-per-scalar-value-unit))
      (b-vector-set! s (+ base j)
		     (low-bits shifted bits-per-byte)))
    (unspecific)))			; avoid type problem

(define (vm-string-size length)
  (+ stob-overhead (bytes->cells (scalar-value-units->bytes length))))

; Converting external (C, Latin-1) strings to S48 strings.

; for small strings only
(define (enter-string string key)
  (let* ((len (string-length string))
	 (v (vm-make-string len key)))
    (copy-string-to-vm-string/latin-1! string len v)
    v))

(define (enter-string+gc-n string len)
  (let ((v (vm-make-string+gc len)))
    (copy-string-to-vm-string/latin-1! string len v)
    v))

(define (enter-string+gc string)
  (enter-string+gc-n string (string-length string)))

(define (copy-string-to-vm-string/latin-1! string len v)
  (do ((i 0 (+ i 1)))
      ((>= i len))
    (vm-string-set! v i (char->ascii (string-ref string i))))
  (unspecific))

(define (copy-vm-string-to-string/latin-1! vm-string start count string)
  (do ((i 0 (+ 1 i)))
      ((>= i count))
    (let ((c (vm-string-ref vm-string i)))
      (string-set! string (+ i start) 
		   (if (<= c 255)
		       (ascii->char c)
		       #\?))))
  (unspecific))

(define (copy-vm-string-chars! from from-index to to-index count)
  (copy-memory! (address+ (address-after-header from)
			  (* from-index bytes-per-scalar-value-unit))
		(address+ (address-after-header to)
			  (* to-index bytes-per-scalar-value-unit))
		(* count bytes-per-scalar-value-unit)))

; This depends on our having 0 bytes at the end of strings.

; We should really be doing the NUL termination here, but
; DEFINE-CONSING-PRIMITIVE doesn't let us do it easily.

(define (extract-low-string code-vector)         ; used by OPEN
  (assert (code-vector? code-vector))
  (fetch-nul-terminated-string (address-after-header code-vector)))
  
(define (vm-string=? s1 s2)
  (assert (and (vm-string? s1) (vm-string? s2)))
  (let ((len (b-vector-length s1)))
    (and (= len (b-vector-length s2))
	 (memory-equal? (address-after-header s1)
			(address-after-header s2)
			len))))

;; This is only a very crude approximation for debugging purposes.
(define (write-vm-string vm-string out)
  (do ((size (vm-string-length vm-string))
       (i 0 (+ 1 i)))
      ((>= i size) 0)			; make type checker happy
    (write-char (ascii->char (vm-string-ref vm-string i)) out)))  

; Number predicates

;(define bignum?       (stob-predicate (enum stob bignum)))
(define ratnum?       (stob-predicate (enum stob ratnum)))
(define double?       (stob-predicate (enum stob double)))

; Doubles

(define (extract-double double)
  (fetch-flonum (address-after-header double)))

(define double-bytes 8)

(define double-size
  (+ stob-overhead (bytes->cells double-bytes)))

(define (enter-double value key)
  (let ((double (make-b-vector (enum stob double) double-bytes key)))
    (store-flonum! (address-after-header double) value)
    double))

; Hashing

; The hash function used here is to take the sum of the ascii values
; of the characters in the string, modulo the symbol table size.
;
; This hash function was also compared against some others, e.g.
; adding in the length as well, and taking only the odd or only the
; even characters.  It fared about the same as adding the length, and
; much better than examining only every other character.
;
; Perhaps a hash function that is sensitive to the positions of the
; characters should be tried?  (Consider CADDR, CDADR, CDDAR.)
;
; Of course, if we switched to rehashing, a prime modulus would be
; important.

(define (vm-string-hash s)
  (let ((n (vm-string-length s)))
    (do ((i 0 (+ i 1))
         (h 0 (+ h (vm-string-ref s i))))
        ((>= i n) h))))

