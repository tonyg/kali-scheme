; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

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
(define-shared-primitive-data-type symbol #t #t
  make-symbol)                                      ; hidden from user
(define-shared-primitive-data-type closure #f #t)
(define-shared-primitive-data-type location)
(define-shared-primitive-data-type weak-pointer)
(define-shared-primitive-data-type external)
(define-shared-primitive-data-type port)
(define-shared-primitive-data-type channel #f #f
  make-channel                           ; hidden from user
  (;; these setters are hidden from user
   (channel-status    set-channel-status!)
   (channel-id        set-channel-id!)
   (channel-os-index  set-channel-os-index!))
  ;; none of these are visible to the user
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

(define (make-template-containing-ops op1 op2)
  (let ((temp (make-template 2 universal-key))
        (code (make-code-vector 2 universal-key)))
    (template-set! temp 0 code)
    (code-vector-set! code 0 op1)
    (code-vector-set! code 1 op2)
    temp))

(define (op-template-size op-count)
  (+ (template-size 2) (code-vector-size op-count)))

; Code vectors

(define make-code-vector   (stob-maker (enum stob code-vector) make-b-vector))
(define code-vector?       (stob-predicate (enum stob code-vector)))
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

(define (enter-string string)           ; used by VMIO on startup
  (let ((z (string-length string)))
    (let ((v (vm-make-string z universal-key)))
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

; Symbol table and environment lookup

(define (table-searcher hash match? make-new)
  ;; In FX terms, this procedure has type
  ;;          (poly (t1 t2 t3)
  ;;            (proc ((proc (t1) int)     ;hash
  ;;                   (proc (t1 t2) bool) ;match?
  ;;                   (proc (t1) t2))     ;make-new
  ;;              (proc (t1 (vector-of (list-of t2)))
  ;;                t2)))
  ;; For the symbol table, t1 = string, t2 = t3 = symbol.
  (lambda (obj table key)
    (let* ((index (bitwise-and (hash obj) (- (vm-vector-length table) 1)))
           (bucket (vm-vector-ref table index)))
      (let loop ((b bucket))
        (cond ((vm-eq? b null)
               (let ((new (make-new obj key)))
                 (vm-vector-set! table index (vm-cons new bucket key))
                 new))
              ((match? obj (vm-car b)) (vm-car b))
              (else (loop (vm-cdr b))))))))

(define intern
  (table-searcher vm-string-hash
                  (lambda (string sym)
                    (vm-string=? string (vm-symbol->string sym)))
		  (lambda (string key)
		    (vm-make-symbol string key))))

(define add-to-symbol-table
  (table-searcher (lambda (sym) (vm-string-hash (vm-symbol->string sym)))
		  vm-eq?
		  (lambda (sym key) sym)))

