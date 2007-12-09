; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define (s48-disable-interrupts!)
  (disable-interrupts!))

(define (s48-enable-interrupts!)
  (enable-interrupts!))

					; used for raising exceptions in external code
(define (s48-push x)
  (push x))

(define (s48-stack-ref i)
  (stack-ref i))

(define (s48-stack-set! x v)
  (stack-set! x v))

(define (s48-enter-integer x)
  (enter-integer x (ensure-space long-as-integer-size)))

(define (s48-enter-unsigned-integer x)
  (enter-unsigned-integer x (ensure-space long-as-integer-size)))

					; arguments must either both be intergers or both floanums
(define (s48-integer-or-floanum-add x y)
  (if (double? x)
      (flonum-add x y)
      (integer-add x y)))
(define (s48-integer-or-floanum-sub x y)
  (if (double? x)
      (flonum-subtract x y)
      (integer-subtract x y)))

(define (s48-integer-or-floanum-mul x y)
  (if (double? x)
      (flonum-multiply x y)
      (integer-multiply x y)))

(define (s48-integer-remainder x y)
  (integer-remainder x y))

(define (s48-integer-quotient x y)
  (integer-quotient x y))

(define (s48-integer-divide x y)
  (integer-divide x y))

(define-syntax define-integer-or-floanum-comparison
  (syntax-rules ()
    ((define-integer-or-floanum-comparison 
       s48-integer-or-floanum-proc integer-op flonum-op)
     (define (s48-integer-or-floanum-proc x y)
       (enter-boolean
	(if (double? x)
	    (flonum-op x y)
	    (integer-op x y)))))))

(define-integer-or-floanum-comparison s48-integer-or-floanum-= integer= flonum=)
(define-integer-or-floanum-comparison s48-integer-or-floanum-< integer< flonum<)
(define-integer-or-floanum-comparison s48-integer-or-floanum-> integer> flonum>)
(define-integer-or-floanum-comparison s48-integer-or-floanum-<= integer<= flonum<=)
(define-integer-or-floanum-comparison s48-integer-or-floanum->= integer>= flonum>=)

(define (s48-integer-bitwise-not x)
  (integer-bitwise-not x))
(define (s48-integer-bit-count x)
  (integer-bit-count x))
(define (s48-integer-bitwise-and x y)
  (integer-bitwise-and x y))
(define (s48-integer-bitwise-ior x y)
  (integer-bitwise-ior x y))
(define (s48-integer-bitwise-xor x y)
  (integer-bitwise-xor x y))

;; Strings

(define (ensure-string s)
  (if (not (vm-string? s))
      (argument-type-violation s)))

(define (ensure-index-range i min max)
  (if (or (< i min)
	  (> i max))
      (range-violation (enter-fixnum i)
		       (enter-fixnum min) (enter-fixnum max))))     
  
(define (ensure-string-index s i)
  (ensure-index-range i 0 (- (vm-string-length s) 1)))

(define (s48-string-set s i c)
  (ensure-string  s)
  (ensure-string-index s i)
  (vm-string-set! s i c))

(define (s48-string-ref s i)
  (ensure-string s)
  (ensure-string-index s i)
  (vm-string-ref s i))

(define (s48-string-length s)
  (ensure-string s)
  (vm-string-length s))

(define (s48-allocate-string len)
  (vm-make-string+gc len))

(define (s48-enter-string-latin-1 s)
  (enter-string+gc s))

(define (s48-enter-string-latin-1-n s count)
  (enter-string+gc-n s count))

(define (s48-copy-latin-1-to-string-n string len vm-string)
  (ensure-string vm-string)
  (ensure-index-range len 0 (vm-string-length vm-string))
  (copy-string-to-vm-string/latin-1! string len vm-string))

(define (s48-copy-latin-1-to-string string vm-string)
  (ensure-string vm-string)
  (let ((len (string-length string)))
    (ensure-index-range len 0 (vm-string-length vm-string))
    (copy-string-to-vm-string/latin-1! string (string-length string) vm-string)))

(define (s48-copy-string-to-latin-1 vm-string string)
  (ensure-string vm-string)
  (copy-vm-string-to-string/latin-1! vm-string 0 (vm-string-length vm-string) string))

(define (s48-copy-string-to-latin-1-n vm-string start count string)
  (ensure-string vm-string)
  (ensure-string-index vm-string start)
  (ensure-index-range count 0 (- (vm-string-length vm-string) start))
  (copy-vm-string-to-string/latin-1! vm-string start count string))

;; UTF encodings

;; General utilities

;; SIZE is in bytes
(define (make-decode! enc unit-inc)
  (lambda (p s size)
    (let loop ((index 0)
	       (target-index 0))
      (if (>= index size)
	  (unspecific)
	  (call-with-values
	      (lambda ()
		(decode-scalar-value
		 enc
		 (address+ p index)
		 (- size index)))
	    (lambda (encoding-ok? ok? incomplete? value count)
	      (cond
	       ((not encoding-ok?)
		(vm-string-set! s target-index (char->ascii #\?))
		(loop (+ unit-inc index) (+ target-index 1)))
	       (incomplete?
		(vm-string-set! s target-index (char->ascii #\?))
		(unspecific))
	       (else
		(vm-string-set! s target-index value)
		(loop (+ index count) (+ target-index 1))))))))))

;; returns # bytes consumed, # characters decoded
;; SIZE is in bytes
(define (make-encoding-length enc unit-inc) 
  (lambda (p size)
    (let loop ((index 0)
	       (target-index 0))
      (if (>= index size)
	  (values index target-index)
	  (call-with-values
	      (lambda ()
		(decode-scalar-value
		 enc
		 (address+ p index)
		 (- size index)))
	    (lambda (encoding-ok? ok? incomplete? value count)
	      (cond
	       ((not encoding-ok?)
		(loop (+ unit-inc index) (+ target-index 1)))
	       (incomplete?
		(values index target-index))
	       (else
		(loop (+ index count) (+ target-index 1))))))))))

(define (make-string-encoding-length enc unit-inc)

  (define (scalar-value-encoding-length sv)
    (call-with-values
	(lambda ()
	  (encode-scalar-value enc sv (integer->address 0) 0))
      (lambda (encoding-ok? ok? out-of-space? count)
	;; we know the encoding is OK
	count)))

  (lambda (s start-index count)
    (let loop ((encoding-length 0)
	       (char-index 0))
      (if (>= char-index count)
	  (quotient encoding-length unit-inc)
	  (loop (+ encoding-length 
		   (scalar-value-encoding-length
		    (vm-string-ref s (+ start-index char-index))))
		(+ 1 char-index))))))

(define (make-copy-vm-string-to-string enc unit-inc)
  (lambda (vm-string start count string)
    (let loop ((source-index 0)
	       (target-index 0))
      (if (>= source-index count)
	  (quotient target-index unit-inc)
	  (let ((sv (vm-string-ref vm-string (+ start source-index))))
	    (call-with-values
		(lambda ()
		  (encode-scalar-value enc
				       sv (address+ string target-index) 4))
	      (lambda (encoding-ok? ok? out-of-space? count)
		(loop (+ source-index 1) (+ target-index count)))))))))

;; UTF-8

(define-syntax define-external-encoding-procedures
  (syntax-rules ()
    ((define-external-encoding-procedures
       ?enc
       ?unit-inc ?encoding-size ; all in bytes
       ?enter-string ?enter-string-n
       ?string-length ?string-length-n
       ?copy-string-to ?copy-string-to-n
       ;; need to pass those in to work around limitation in PreScheme compiler
       ?decode! ?encoding-length ?string-encoding-length ?copy-vm-string-to-string!
       )
     (begin

(define (?enter-string p)
  (call-with-values
      (lambda ()
	(?encoding-length p (?encoding-size p)))
    (lambda (consumed decoded)
      (let ((vm (vm-make-string+gc decoded)))
	(?decode! p vm consumed)
	vm))))

(define (?enter-string-n p size)
  (call-with-values
      (lambda ()
	(?encoding-length p size))
    (lambda (consumed decoded)
      (let ((vm (vm-make-string+gc decoded)))
	(?decode! p vm consumed)
	vm))))

(define (?string-length vm-string)
  (ensure-string vm-string)
  (?string-encoding-length vm-string 0 (vm-string-length vm-string)))

(define (?string-length-n vm-string start-index count)
  (ensure-string vm-string)
  (ensure-string-index vm-string start-index)
  (ensure-index-range count 0 (- (vm-string-length vm-string) start-index))
  (?string-encoding-length vm-string start-index count))

(define (?copy-string-to vm-string string)
  (ensure-string vm-string)
  (?copy-vm-string-to-string! vm-string 0 (vm-string-length vm-string) string))

(define (?copy-string-to-n vm-string start count string)
  (ensure-string vm-string)
  (ensure-string-index vm-string start)
  (ensure-index-range count 0 (- (vm-string-length vm-string) start))
  (?copy-vm-string-to-string! vm-string start count string))

(define ?decode!
  (make-decode! (enum text-encoding-option ?enc) ?unit-inc))

(define ?encoding-length
  (make-encoding-length (enum text-encoding-option ?enc) ?unit-inc))

(define ?string-encoding-length
  (make-string-encoding-length (enum text-encoding-option ?enc) ?unit-inc))

(define ?copy-vm-string-to-string!
  (make-copy-vm-string-to-string (enum text-encoding-option ?enc) ?unit-inc))

))))

(define (utf-8-encoding-size p)
  (string-length (fetch-nul-terminated-string p)))

(define-external-encoding-procedures
  utf-8 1 utf-8-encoding-size
  s48-enter-string-utf-8 s48-enter-string-utf-8-n
  s48-string-utf-8-length s48-string-utf-8-length-n
  s48-copy-string-to-utf-8 s48-copy-string-to-utf-8-n

  decode/utf-8!
  encoding-length/utf-8 string-encoding-length/utf-8
  copy-vm-string-to-string/utf-8!
  )

; silly but works
(define (utf-16-encoding-size p)
  (let ((s (fetch-nul-terminated-string p)))
    (let loop ((i 0))
      (if (and (= 0 (char->ascii (string-ref s i)))
	       (= 0 (char->ascii (string-ref s (+ 1 i)))))
	  i ; in bytes
	  (loop (+ i 2))))))

(define-external-encoding-procedures
  utf-16be 2 utf-16-encoding-size
  ;; trailing slash because we need to fix the types C-side
  s48-enter-string-utf-16be/ s48-enter-string-utf-16be-n/
  s48-string-utf-16be-length s48-string-utf-16be-length-n
  s48-copy-string-to-utf-16be/ s48-copy-string-to-utf-16be-n/

  decode/utf-16be!
  encoding-length/utf-16be string-encoding-length/utf-16be
  copy-vm-string-to-string/utf-16be!
  )

(define-external-encoding-procedures
  utf-16le 2 utf-16-encoding-size
  ;; trailing slash because we need to fix the types C-side
  s48-enter-string-utf-16le/ s48-enter-string-utf-16le-n/
  s48-string-utf-16le-length s48-string-utf-16le-length-n
  s48-copy-string-to-utf-16le/ s48-copy-string-to-utf-16le-n/

  decode/utf-16le!
  encoding-length/utf-16le string-encoding-length/utf-16le
  copy-vm-string-to-string/utf-16le!
  )

