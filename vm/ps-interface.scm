
(define-interface pre-scheme-primitives-interface
  (export ((if begin lambda letrec quote set!
	       define define-syntax let-syntax
	       and cond do let let* or
	       quasiquote
	       ;syntax-rules
	       ) :syntax)  ; no delay, case is below
	  ;; letrec-syntax  -- not yet implemented
	  
	  (goto :syntax)
	  (external :syntax)

	  eq?
	  + - * = <  ; /
	  quotient remainder
	  ; floor numerator denominator
	  ; real-part imag-part
	  ; exp log sin cos tan asin acos atan sqrt
	  ; angle magnitude make-polar make-rectangular
	  char=? char<?

	  ;; Data manipulation
	  make-vector vector-length
	  vector-ref vector-set!

	  make-string string-length
	  string-ref string-set!

	  ps-read-char
	  char-ready?
	  write-char 

	  ; values call-with-values

	  ascii->char char->ascii

	  ; extras
	  small*  ; hack for robots
	  allocate-memory deallocate-memory read-block write-block
	  deallocate null-pointer? null-port?
	  ashl ashr lshr
	  bitwise-and bitwise-ior bitwise-xor bitwise-not
	  force-output
	  ;abort
	  ;runtime
	  unassigned
	  error
	  write-string
	  call-external-value
	  ))

(define-interface pre-scheme-internal-primitives-interface
  (export stdin stdout
	  open-file
	  close-port
	  coerce   ; this is really syntax
	  contents-int8u set-contents-int8u!
	  contents-int32 set-contents-int32!
	  ))

(define-interface pre-scheme-interface
  (compound-interface
   pre-scheme-primitives-interface
   (export (case :syntax)
	   ;; Data manipulation
	   ;make-string string-length string-ref string-set!
	   unsigned-byte-ref unsigned-byte-set!
	   word-ref word-set!

	   <= > >=
	   abs
	   ;char-alphabetic?
	   ;ceiling
	   ;char-ci<=? char-ci<? char-ci=? char-ci>=? char-ci>?
	   ;char-downcase char-lower-case? char-numeric?
	   ;char-upcase
	   ;char-upper-case? char-whitespace? char<=?
	   ;char>=? char>? close-input-port close-output-port
	   not
	   ;vector-fill!

	   close-output-port close-input-port
	   open-input-file open-output-file
	   current-output-port
	   current-input-port
	   newline
	   call-with-input-file

	   )))
