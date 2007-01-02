; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-interface prescheme-interface
  (export ((if begin lambda letrec quote set!
	       define define-syntax let-syntax letrec-syntax
	       and cond case do let let* or
	       quasiquote
	       syntax-rules
	       ) :syntax)  ; no delay
	  
	  (goto :syntax)
	  (external :syntax)

	  ((define-enumeration define-external-enumeration enum) :syntax)
	  ((name->enumerand enumerand->name) :syntax)  ; loadtime only

	  not

	  eq?
	  + - * = <  ; /
	  <= > >=
	  abs
	  expt
	  quotient remainder
	  ; floor numerator denominator
	  ; real-part imag-part
	  ; exp log sin cos tan asin acos atan sqrt
	  ; angle magnitude make-polar make-rectangular
	  min max
	  char=? char<?

	  ;; Data manipulation
	  make-vector vector-length
	  vector-ref vector-set!

	  make-string string-length
	  string-ref string-set!

	  deallocate
	  null-pointer
	  null-pointer?

	  values call-with-values

	  current-input-port current-output-port current-error-port
	  open-input-file open-output-file
	  close-output-port close-input-port

	  read-char peek-char read-integer
	  write-char newline write-string write-integer
	  force-output

	  (errors :syntax)
	  error-string

	  ascii->char char->ascii
	  
	  shift-left arithmetic-shift-right logical-shift-right
	  bitwise-and bitwise-ior bitwise-xor bitwise-not
	  unspecific
	  error
	  ))

; memory (malloc and free)

(define-interface ps-memory-interface
  (export allocate-memory
	  deallocate-memory

	  unsigned-byte-ref unsigned-byte-set!
	  word-ref word-set!
	  flonum-ref flonum-set!

	  address?
	  null-address null-address?

	  address+ address- address-difference
	  address= address< address<= address> address>=
	  address->integer integer->address

	  copy-memory! memory-equal?

	  char-pointer->string char-pointer->nul-terminated-string

	  read-block write-block))

(define-interface ps-flonums-interface
  (export fl+ fl- fl* fl/ fl= fl< fl> fl<= fl>=))

(define-interface ps-receive-interface
  (export receive))

