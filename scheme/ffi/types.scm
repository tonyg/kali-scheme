;;; Types

(define-record-type ctype :ctype
  (really-make-ctype primitive? base scheme->c c->scheme)
  ctype?
  (primitive? ctype-primitive?)
  (base       ctype-base)
  (scheme->c  ctype-scheme->c)
  (c->scheme  ctype-c->scheme))

(define-record-discloser :ctype
  (lambda (obj)
    (list 'ctype (if (ctype-primitive? obj)
                     'primitive
                     (ctype-base obj)))))

(define (make-ctype base scheme->c c->scheme)
  (really-make-ctype #f base scheme->c c->scheme))

(define (ctype-get-primitive type)
  (let loop ((type type))
    (if (ctype-primitive? type)
        (ctype-base type)
        (loop (ctype-base type)))))

(define (c->scheme obj type)
  (for-each display `("c->scheme: " ,obj ", " ,type #\newline))
  (if (byte-vector? obj)
      (debug-pointer "obj" obj))
  (if (ctype-primitive? type)
      obj
      ;; FIXME: make tail-recursive?
      ((ctype-c->scheme type) (c->scheme obj (ctype-base type)))))

(define (scheme->c obj type)
  (if (ctype-primitive? type)
      obj
      (scheme->c ((ctype-scheme->c type) obj) (ctype-base type))))

(define (pointer-value bvec)
  (+ (byte-vector-ref bvec 0)
     (* (byte-vector-ref bvec 1) 256)
     (* (byte-vector-ref bvec 2) 65536)
     (* (byte-vector-ref bvec 3) 16777216)))

(define (debug-pointer msg bvec)
  (for-each display (list msg ": " (pointer-value bvec)  #\newline))
  (force-output (current-output-port)))

(define (make-primitive-type binding)
  ;;(debug-pointer `("primitive type " ,binding) (shared-binding-ref binding))
  (really-make-ctype #t (shared-binding-ref binding) #f #f))

(define-syntax import-types
  (lambda (form r compare)
    `(,(r 'begin)
      ,@(append-map (lambda (name)
                      (let ((ffi-name (string->symbol
                                       (string-append "ffi-type-"
                                                      (symbol->string name))))
                            (uscore-name (string->symbol
                                          (string-append "_" (symbol->string name)))))
                        `((,(r 'import-definition) ,ffi-name)
                          (,(r 'define) ,uscore-name
                           (,(r 'make-primitive-type) ,ffi-name)))))
                    (cdr form)))))

(import-types void bool int8 uint8 int16 uint16 int32 uint32 int64 uint64
              float double pointer fpointer)

(define (compiler-sizeof thing)
  (call-imported-binding ffi-compiler-sizeof thing))
(import-definition ffi-compiler-sizeof)

;; Integer type aliases

(define _sint8 _int8)
(define _sint16 _int16)
(define _sint32 _int32)
(define _sint64 _int64)

;; _byte etc is a convenient name for _uint8 & _sint8
;; (_byte is unsigned)
(define _byte  _uint8)
(define _ubyte _uint8)
(define _sbyte _int8)

;; _word etc is a convenient name for _uint16 & _sint16
;; (_word is unsigned)
(define _word  _uint16)
(define _uword _uint16)
(define _sword _int16)

;; _short etc is a convenient name for whatever is the compiler's `short'
;; (_short is signed)
(define-values (_short _ushort _sshort)
  (case (compiler-sizeof 'short)
    ((2) (values _int16 _uint16 _int16))
    ((4) (values _int32 _uint32 _int32))
    (else (error "ffi internal error: bad compiler size for `short'"))))

;; _int etc is a convenient name for whatever is the compiler's `int'
;; (_int is signed)
(define-values (_int _uint _sint)
  (case (compiler-sizeof 'int)
    ((2) (values _int16 _uint16 _int16))
    ((4) (values _int32 _uint32 _int32))
    ((8) (values _int64 _uint64 _int64))
    (else (error "ffi internal error: bad compiler size for `int'"))))

;; _long etc is a convenient name for whatever is the compiler's `long'
;; (_long is signed)
(define-values (_long _ulong _slong)
  (case (compiler-sizeof 'long)
    ((4) (values _int32 _uint32 _int32))
    ((8) (values _int64 _uint64 _int64))
    (else (error "ffi internal error: bad compiler size for `long'"))))

;; _llong etc is a convenient name for whatever is the compiler's `long long'
;; (_llong is signed)
(define-values (_llong _ullong _sllong)
  (case (compiler-sizeof '(long long))
    ((4) (values _int32 _uint32 _int32))
    ((8) (values _int64 _uint64 _int64))
    (else (error "ffi internal error: bad compiler size for `llong'"))))

