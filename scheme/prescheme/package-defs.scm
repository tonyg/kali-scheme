; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-structures ((prescheme prescheme-interface)
		    (ps-memory ps-memory-interface)
		    (memory-debug (export reinitialize-memory))
		    (ps-receive ps-receive-interface)
		    (external-constants (export make-external-constant
						external-constant?
						external-constant-enum-name
						external-constant-name
						external-constant-c-string)))
  (open (modify scheme (rename (open-input-file   scheme:open-input-file)
			       (open-output-file  scheme:open-output-file)
			       (close-input-port  scheme:close-input-port)
			       (close-output-port scheme:close-output-port)
			       (read-char         scheme:read-char)
			       (peek-char         scheme:peek-char)))
	code-vectors bitwise ascii primitives signals enumerated
	define-record-types
	bigbit)   ; make sure that bignum bitwise operations are loaded
  (optimize auto-integrate)
  (begin
    ; What we will get in C on many machines
    (define pre-scheme-integer-size 32))
  (files ps-defenum prescheme memory))

(define-structure ps-record-types (export define-record-type)
  (open scheme (modify define-record-types (prefix s48:)))
  (begin
    (define-syntax define-record-type
      (syntax-rules ()
	((define-record-type name type-name
	   constructor
	   (field type more ...) ...)
	 (s48:define-record-type name type-name
	   constructor
	   (field more ...) ...))))))

(define-structure ps-flonums ps-flonums-interface
  (open scheme)
  (optimize auto-integrate)
  (begin
    (define fl+ +) (define fl- -) (define fl* *) (define fl/ /)
    (define fl= =)
    (define fl< <) (define fl> >)
    (define fl<= <=) (define fl>= >=)))

;(define-structure byte-vectors byte-vector-interface
;  (open scheme code-vectors)
;  (optimize auto-integrate)
;  (files byte-vector))
;
;(define-interface byte-vector-interface
;  (export make-byte-vector
;          byte-vector?
;          byte-vector-ref byte-vector-set!
;          signed-byte-vector-ref signed-byte-vector-set!
;          byte-vector-ref32 byte-vector-set32!
;          signed-byte-vector-ref32 signed-byte-vector-set32!))
;
;(define-structure xmemory ps-memory-interface
;  (open scheme byte-vectors signals enumerated bitwise)
;  (files simple-memory))