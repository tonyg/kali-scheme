; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file struct.scm.

;;;; Structure definitions

; This file defines a level of abstraction for storage somewhat higher
; than that of d-vectors and b-vectors: pairs, symbols, and other datatypes.

;; (assert (>= (ashl 1 header-type-field-width)
;;              (vector-length stob)))


(define (stob-predicate type)
  (lambda (obj) (stob-of-type? obj type)))

; data for these comes from STOB-DATA in arch.scm

(define-shared-primitive-data-type pair #t)
(define-shared-primitive-data-type symbol #t)
(define-shared-primitive-data-type closure)
(define-shared-primitive-data-type location)
(define-shared-primitive-data-type weak-pointer)
(define-shared-primitive-data-type external)

(define-primitive-data-type stob/port port make-port
  (port-mode set-port-mode!)
  (port-index set-port-index!)
  (peeked-char set-peeked-char!)
  (port-id set-port-id!))  ; setter needed by the post-GC code

; Vectors

(define (vm-make-vector len key)
  (make-d-vector stob/vector len key))
(define vm-vector?       (stob-predicate stob/vector))
(define vm-vector-length d-vector-length)
(define vm-vector-ref    d-vector-ref)
(define vm-vector-set!   d-vector-set!)

(define (vm-vector-size len)
  (+ len 1))

(define (vm-vector-fill! v val)
  (do ((i 0 (+ i 1)))
      ((= i (vm-vector-length v)) v)
    (vm-vector-set! v i val)))

; Records

(define (make-record len key)
  (make-d-vector stob/record len key))
(define record?       (stob-predicate stob/record))
(define record-length d-vector-length)
(define record-ref    d-vector-ref)
(define record-set!   d-vector-set!)

(define (record-size len)
  (+ len 1))

; Extended-Numbers

(define (make-extended-number len key)
  (make-d-vector stob/extended-number len key))
(define extended-number?       (stob-predicate stob/extended-number))
(define extended-number-length d-vector-length)
(define extended-number-ref    d-vector-ref)
(define extended-number-set!   d-vector-set!)

(define (extended-number-size len)
  (+ len 1))

; Continuations

(define (make-continuation len key)
  (make-d-vector stob/continuation len key))
(define continuation?       (stob-predicate stob/continuation))
(define continuation-length d-vector-length)
(define continuation-ref    d-vector-ref)
(define continuation-set!   d-vector-set!)

(define (continuation-size len)
  (+ len 1))

; Code vectors

(define (make-code-vector len key)
  (make-b-vector stob/code-vector len key))
(define code-vector?        (stob-predicate  stob/code-vector))
(define code-vector-length b-vector-length)
(define code-vector-ref    b-vector-ref)
(define code-vector-set!   b-vector-set!)

(define (code-vector-size len)
  (+ 1 (bytes->cells len)))

; Templates

(define (make-template len key)
  (make-d-vector stob/template len key))
(define template?       (stob-predicate stob/template))
(define template-length d-vector-length)
(define template-ref    d-vector-ref)
(define template-set!   d-vector-set!)

(define (template-size len)
  (+ len 1))

(define (template-code tem) (template-ref tem 0))
(define (template-name tem) (template-ref tem 1))

(define (make-special-op-template op)
  (let ((temp (make-template 2 universal-key))
        (code (make-code-vector 2 universal-key)))
    (template-set! temp 0 code)
    (code-vector-set! code 0 op)
    (code-vector-set! code 1 op/return)
    temp))

(define (make-special-two-op-template op1 op2)
  (let ((temp (make-template 2 universal-key))
        (code (make-code-vector 3 universal-key)))
    (template-set! temp 0 code)
    (code-vector-set! code 0 op1)
    (code-vector-set! code 0 op2)
    (code-vector-set! code 1 op/return)
    temp))

(define special-op-template-size
  (+ (template-size 2) (code-vector-size 2)))

(define special-two-op-template-size
  (+ (template-size 2) (code-vector-size 3)))

; Strings

(define (vm-make-string len key)
  (make-b-vector stob/string len key))
(define vm-string?       (stob-predicate  stob/string))
(define vm-string-length b-vector-length)
(define vm-string-ref    (lambda (s i) (ascii->char (b-vector-ref s i))))
(define vm-string-set!   (lambda (s i c) (b-vector-set! s i (char->ascii c))))

(define vm-string-size code-vector-size)

(define (enter-string string)           ; used by VMIO on startup
  (let ((z (string-length string)))
    (let ((v (vm-make-string z universal-key)))
      (do ((i 0 (+ i 1)))
          ((>= i z) v)
        (vm-string-set! v i (string-ref string i))))))

(define (extract-string string)         ; used by OPEN, WRITE-STRING, SUSPEND
  (let ((z (vm-string-length string)))
    (let ((v (make-string z)))
      (do ((i 0 (+ i 1)))
          ((>= i z) v)
        (string-set! v i (vm-string-ref string i))))))

(define (write-vm-string string port)
  (write-bytes (address-after-header string)
	       (vm-string-length string)
	       port))

(define vm-string=? stob-equal?)

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
                  vm-make-symbol))

(define add-to-symbol-table
  (table-searcher (lambda (sym) (vm-string-hash (vm-symbol->string sym)))
		  vm-eq?
		  (lambda (sym key) sym)))

(define xlookup
  (table-searcher (lambda (sym) (vm-string-hash (vm-symbol->string sym)))
                  (lambda (sym location)
                    (vm-eq? sym (location-id location)))
                  (lambda (sym key) (make-location unbound-marker sym key))))

(define (lookup env sym key)
  (xlookup sym env key))

; Cf. struct.scm:
;
; The hash function was tested on 607 symbols from the
; scheme48 sources.  Whether or not the symbol table size (modulus)
; was prime or not was found not to make much difference; in fact,
; moduli of 256 and 512 worked out pretty well.  The standard
; deviation for the length of the buckets was as follows:
;      199           1.744
;      256           1.695
;      509           1.175
;      512           1.202
;      1021          0.828
; Since taking a remainder mod 512 is much faster than taking one mod
; 509, 512 is the choice here for the table size.

(define log-table-size 9)
(define table-size (ashl 1 log-table-size))

(define (make-hash-table key)
  (let ((table (vm-make-vector table-size key)))
    (vm-vector-fill! table null)
    table))

(define make-location-table make-hash-table)

(define make-symbol-table   make-hash-table)


; Eventually, perhaps: make-table, table-ref, table-set!

