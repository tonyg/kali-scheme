; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

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
(define-shared-primitive-data-type closure #f #t)
(define-shared-primitive-data-type location)
(define-shared-primitive-data-type weak-pointer)
(define-shared-primitive-data-type shared-binding #f #f
  #f
  ()
  (shared-binding-next set-shared-binding-next!))	; hidden from RTS
(define-shared-primitive-data-type port)
(define-shared-primitive-data-type channel #f #f
  make-channel                           		; hidden from RTS
  (;; these setters are hidden from the RTS
   (channel-status    set-channel-status!)
   (channel-id        set-channel-id!)
   (channel-os-index  set-channel-os-index!))
  ;; none of these are visible to the RTS
  (channel-next      set-channel-next!)
  (channel-os-status set-channel-os-status!))

; Vectors and so on

(define-vector-data-type vector #t)
(define-vector-data-type record)
(define-vector-data-type extended-number)
(define-vector-data-type continuation)
(define-vector-data-type template)

(define (vm-vector-fill! v val)
  (do ((i 0 (+ i 1)))
      ((= i (vm-vector-length v)) v)
    (vm-vector-set! v i val)))

(define (continuation-cont     c) (continuation-ref c 0))
(define (continuation-pc       c) (continuation-ref c 1))
(define (continuation-template c) (continuation-ref c 2))
(define (continuation-env      c) (continuation-ref c 3))

; Continuations are only initialized

(define (set-continuation-cont!     c val) (d-vector-init! c 0 val))
(define (set-continuation-pc!       c val) (d-vector-init! c 1 val))
(define (set-continuation-template! c val) (d-vector-init! c 2 val))
(define (set-continuation-env!      c val) (d-vector-init! c 3 val))

(define continuation-cells 4)

(define (template-code tem) (template-ref tem 0))
(define (template-name tem) (template-ref tem 1))

; The VM needs a few templates for various obscure purposes.

(define (make-template-containing-ops op1 op2 key)
  (let ((temp (make-template 2 key))
        (code (make-code-vector 2 key)))
    (template-set! temp 0 code)
    (code-vector-set! code 0 op1)
    (code-vector-set! code 1 op2)
    temp))

(define (op-template-size op-count)
  (+ (template-size 2) (code-vector-size op-count)))

; Code vectors

(define make-code-vector   (stob-maker (enum stob byte-vector) make-b-vector))
(define code-vector?       (stob-predicate (enum stob byte-vector)))
(define code-vector-length b-vector-length)
(define code-vector-ref    b-vector-ref)
(define code-vector-set!   b-vector-set!)

(define (code-vector-size len)
  (+ stob-overhead (bytes->cells len)))

; Strings are presented as being one character shorter than they really
; are to hide the null character at the end.

(define (vm-make-string length key)
  (let ((string (make-b-vector (enum stob string) (+ length 1) key)))
    (b-vector-set! string length 0)
    string))
(define vm-string?       (stob-predicate (enum stob string)))
(define vm-string-length (lambda (x) (- (b-vector-length x) 1)))
(define vm-string-ref    (lambda (s i) (ascii->char (b-vector-ref s i))))
(define vm-string-set!   (lambda (s i c) (b-vector-set! s i (char->ascii c))))

(define (vm-string-size length)
  (+ stob-overhead (bytes->cells (+ 1 length))))

(define (enter-string string key)           ; used by VMIO on startup
  (let ((z (string-length string)))
    (let ((v (vm-make-string z key)))
      (do ((i 0 (+ i 1)))
          ((>= i z) v)
        (vm-string-set! v i (string-ref string i))))))

; This depends on our having 0 bytes at the end of strings.

(define (extract-string string)         ; used by OPEN
  (fetch-nul-terminated-string (address-after-header string)))
  
(define (vm-string=? s1 s2)
  (assert (and (vm-string? s1) (vm-string? s2)))
  (let ((len (b-vector-length s1)))
    (and (= len (b-vector-length s2))
	 (memory-equal? (address-after-header s1)
			(address-after-header s2)
			len))))

; Number predicates

(define bignum?       (stob-predicate (enum stob bignum)))
(define ratnum?       (stob-predicate (enum stob ratnum)))
(define double?       (stob-predicate (enum stob double)))

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
         (h 0 (+ h (char->ascii (vm-string-ref s i)))))
        ((>= i n) h))))

