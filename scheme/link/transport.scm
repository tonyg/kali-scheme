; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file transport.scm.

; System builder for bootstrapping and debugging.

; Things that have to be written out:
;  Made by the compiler
;   closures
;   templates
;   code-vectors
;   locations
;  Quoted data
;   symbols
;   pairs
;   vectors
;   strings
;   booleans
;   characters

; Convert THING to the Scheme 48 virtual machine's representation and
; return it.  Locations and symbols may have multiple references in
; the image.  Their transported addresses are kept in a table.

(define (transport thing)
  (let transport ((thing thing))
    (cond ((immediate? thing)
           (transport-immediate thing))
          ((closure? thing)
           (transport-closure thing))
          ((code-vector? thing)
           (allocate-b-vector thing code-vector-length))
          ((location? thing)
           (let ((address (table-ref *locations* thing)))
             (cond (address address)
                   (else
                    (let ((desc (transport-location thing)))
                      (table-set! *locations* thing desc)
                      desc)))))
          ((symbol? thing)
           (let ((address (table-ref *symbols* thing)))
             (cond (address address)
                   (else
                    (let ((desc (transport-symbol thing)))
                      (table-set! *symbols* thing desc)
                      desc)))))
          ((pair? thing)
           (transport-pair thing))
          ((template? thing)
           (transport-template thing))
          ((vector? thing)
           (transport-vector thing))
          ((string? thing)
           (allocate-b-vector thing (lambda (x) (+ 1 (string-length x)))))
          (else
           (error "cannot transport object" thing)))))

; Transport the things that are not allocated from the heap.

(define (transport-immediate thing)
  (cond ((integer? thing)
         (make-descriptor (enum tag fixnum) thing))
        ((char? thing)
         (make-immediate (enum imm char) (char->ascii thing)))
        ((eq? thing '())
         vm-null)
        ((eq? thing #f)
         vm-false)
        ((eq? thing #t)
         vm-true)
        ((eq? thing (unspecific))
         vm-unspecific)
        (else
         (error "cannot transport literal" thing))))

;==============================================================================
; The heap is a list of transported stored objects, each of which is either a
; string, a code-vector, or a vector of length N+1 representing a stored object
; with N cells.  The last slot of the vector is the object's header.

(define *heap* '())

(define *hp* 0)       ; Current heap-pointer (in a-units)

(define *symbols* #f)    ; Table of already-transported symbols

(define *locations* #f)  ; Table of already-transported locations

(define (initialize-memory)
  (set! *hp*      0)
  (set! *heap*    '())
  (set! *symbols* (make-table))
  (set! *locations* (make-table location-id)))

; Allocate a new stored object in the heap.  DATA is whatever data is
; associated with the object, LEN is the length of the object (not
; including the header) in bytes.  A pointer to the new object is
; returned.

(define (allocate-stob data len)
  (let ((addr (+ *hp* (cells->a-units 1))))   ; move past header
    (set! *hp* (+ addr (bytes->a-units len)))
    (set! *heap* (cons data *heap*))
    (make-stob-descriptor addr)))

; Allocate a new stored object that contains descriptors.  This
; creates a vector to hold the header and the object's tranported
; contents and allocates a stob in the heap.  Returns a pair
; containing the stob-pointer and the vector.

(define (allocate-d-vector type cells immutable?)
  (let* ((vec (make-vector (+ cells 1) 0))
         (ptr (allocate-stob vec (cells->bytes cells)))
         (hdr (make-header type (cells->bytes cells))))
    (vector-set! vec cells (if immutable? (make-header-immutable hdr) hdr))
    (cons ptr vec)))

; Allocate a new stored object that contains data.  VEC is either a
; code-vector or a string.

(define (allocate-b-vector vec length)
  (let ((len (cells->bytes (bytes->cells (length vec)))))
    (allocate-stob vec len)))

;==============================================================================
; Transport an object with two slots.  ALLOCATE-D-VECTOR allocates the
; storage and then the two values are transported.

(define (transport-two-slot type accessor1 offset1 accessor2 offset2
                            immutable?)
  (lambda (thing)
    (let* ((data (allocate-d-vector type 2 immutable?))
           (descriptor (car data))
           (vector (cdr data)))
      (vector-set! vector offset1 (transport (accessor1 thing)))
      (vector-set! vector offset2 (transport (accessor2 thing)))
      descriptor)))

; Closures and pairs are transported using TRANSPORT-TWO-SLOT.

(define transport-closure
  (transport-two-slot (enum stob closure)
                      closure-template closure-template-offset
                      closure-env      closure-env-offset
                      #t))  ; ***

(define transport-pair
  (transport-two-slot (enum stob pair)
                      car car-offset
                      cdr cdr-offset
                      #t))  ; *** ?

; Transporting a location requires some care so as to avoid calling CONTENTS
; when the location is unbound.

(define (transport-location loc)
  (let* ((data (allocate-d-vector (enum stob location) 2 #f))
         (descriptor (car data))
         (vector (cdr data)))
    (vector-set! vector
                 location-contents-offset
                 (if (location-defined? loc)
                     (transport (contents loc))
                     vm-unbound))
    (vector-set! vector
                 location-id-offset
                 (transport (location-id loc)))
    descriptor))

; Symbols have two slots, the string containing the symbol's name and a slot
; used in building the symbol table.
; Characters in the symbol name are made to be lower case.

(define (transport-symbol symbol)
  (let* ((data (allocate-d-vector (enum stob symbol) 2 #t))
         (descriptor (car data))
         (vector (cdr data)))
    (vector-set! vector
                 0
                 (transport (symbol-case-converter (symbol->string symbol))))
    (vector-set! vector
                 1
                 (transport #f))
    descriptor))

(define (string-case-converter string)
  (let ((new (make-string (string-length string) #\x)))
    (do ((i 0 (+ i 1)))
        ((>= i (string-length new))
         new)
      (string-set! new i (preferred-case (string-ref string i))))))

;(define preferred-case                  ;Copied from rts/read.scm
;  (if (char=? (string-ref (symbol->string 't) 0) #\T)
;      char-upcase
;      char-downcase))
(define preferred-case char-downcase)

(define symbol-case-converter
  (if (char=? (string-ref (symbol->string 't) 0)
              (preferred-case #\t))
      (lambda (string) string)
      string-case-converter))

; Templates and vectors have an arbitrary number of slots but are otherwise
; the same as pairs and closures.

(define (transport-template template)
  (transport-vector-like template
                         (enum stob template)
                         (template-length template)
                         template-ref
                         #f))

(define (transport-vector vector)
  (transport-vector-like vector
                         (enum stob vector)
                         (vector-length vector)
                         vector-ref
                         #t))  ;***

(define (transport-vector-like vector type length ref immutable?)
  (let* ((data (allocate-d-vector type length immutable?))
         (descriptor (car data))
         (new (cdr data)))
    (do ((i 0 (+ i 1)))
        ((>= i length))
      (vector-set! new i (transport (ref vector i))))
    descriptor))

;==============================================================================
; Writing the heap out to a port.

(define (write-heap port)
  (do ((heap (reverse *heap*) (cdr heap)))
      ((null? heap))
    (write-heap-stob (car heap) port)))

; Dispatch on the type of THING and call WRITE-STOB.

(define (write-heap-stob thing port)
  (cond ((string? thing)
         (let ((len (+ 1 (string-length thing))))
           (write-stob (make-header-immutable ; ***
                        (make-header (enum stob string) len))
                       thing len nulled-string-ref write-char port)
           (align-port len port)))
        ((code-vector? thing)
         (let ((len (code-vector-length thing)))
           (write-stob (make-header-immutable  ; ***
                        (make-header (enum stob byte-vector) len))
                       thing len code-vector-ref write-byte port)
           (align-port len port)))
        ((vector? thing)
         (let ((len (vector-length thing)))
           (write-stob (vector-ref thing (- len 1))
                       thing (- len 1) vector-ref write-descriptor port)))
        (else
         (error "do not know how to write stob" thing))))

(define (nulled-string-ref string i)
  (if (= i (string-length string))
      (ascii->char 0)
      (string-ref string i)))

; Write out a transported STOB to PORT.  HEADER is the header, LENGTH is the
; number of objects the STOB contains, ACCESSOR and WRITER access the contents
; and write them to the heap.

(define (write-stob header contents length accessor writer port)
  (write-descriptor header port)
  (do ((i 0 (+ i 1)))
      ((>= i length))
    (writer (accessor contents i) port)))

; Write out zeros to align the port on a four-byte boundary.

(define (align-port len port)
  (let ((count (- (cells->bytes (bytes->cells len)) len)))
    (do ((count count (- count 1)))
        ((<= count 0))
      (write-byte 0 port))))

