; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; This is file prim.scm.
; Requires DEFINE-PRIMITIVE macro.

;;;; VM data manipulation primitives

; Input checking and coercion

(define (input-type pred coercer)  ;Alonzo wins
  (lambda (f) (f pred coercer)))

(define (input-type-predicate type) (type (lambda (x y) y x)))
(define (input-type-coercion type)  (type (lambda (x y) x y)))

(define (no-coercion x) x)

(define any->         (input-type (lambda (x) x #t) no-coercion))
(define fixnum->      (input-type fixnum?      extract-fixnum))
(define char->        (input-type vm-char?     extract-char))
(define vm-char->     (input-type vm-char?     no-coercion))
(define boolean->     (input-type vm-boolean?  extract-boolean))
(define location->    (input-type location?    no-coercion))
(define string->      (input-type vm-string?   no-coercion))
(define vector->      (input-type vm-vector?   no-coercion))
(define code-vector-> (input-type code-vector? no-coercion))

; Output coercion

(define (return val)
  (set! *val* val)
  (goto continue 0))

(define return-any return)

(define (return-boolean x)
  (goto return (enter-boolean x)))

(define (return-fixnum x)
  (goto return (enter-fixnum x)))

(define (return-char x)
  (goto return (enter-char x)))

(define (return-unspecific x)
  x ;ignored
  (goto return unspecific-value))

(define (no-result)
  (goto return unspecific-value))


; Scalar primitives

(define-primitive eq? (any-> any->) vm-eq? return-boolean)

; Rudimentary generic arithmetic.  Incomplete and confusing.
;  How to modularize for VM's like Maclisp that have generic arithmetic
;  built-in?

; These predicates are used to characterize the numeric representations that
; are implemented in the VM.

(define primitive-number?  fixnum?)
(define primitive-real?    fixnum?)
(define primitive-integer? fixnum?)

(define number->  (input-type primitive-number?  no-coercion))
(define real->    (input-type primitive-real?    no-coercion))
(define integer-> (input-type primitive-integer? no-coercion))

(define-primitive number? (any->)
  (lambda (x)
    (or (fixnum? x)
	(bignum? x)
	(ratnum? x)
	(double? x)
	(extended-number? x)))
  return-boolean)

(define-primitive integer?
  (any->)
  (lambda (n)
    (cond ((or (fixnum? n)
	       (bignum? n))
	   (goto return (enter-boolean #t)))
	  ((extended-number? n)
	   (raise-exception wrong-type-argument 0 n))
	  (else
	   (goto return (enter-boolean #f))))))

(define-syntax define-rational-predicate
  (syntax-rules ()
    ((define-numeric-predicate op)
     (define-primitive op
       (any->)
       (lambda (n)
	 (cond ((or (fixnum? n)
		    (bignum? n)
		    (ratnum? n)
		    (double? n))
		(goto return (enter-boolean #t)))
	       ((extended-number? n)
		(raise-exception wrong-type-argument 0 n))
	       (else
		(goto return (enter-boolean #f)))))))))

(define-rational-predicate rational?)
(define-rational-predicate real?)
(define-rational-predicate complex?)

; These primitives have a simple answer in the case of fixnums; for all other
; they punt to the run-time system.

(define-primitive exact?      (number->) (lambda (n) #t) return-boolean)
(define-primitive real-part   (number->) (lambda (n) (goto return n)))
(define-primitive imag-part   (number->) (lambda (n) 0) return-fixnum)
(define-primitive floor       (number->) (lambda (n) (goto return n)))
(define-primitive numerator   (number->) (lambda (n) (goto return n)))
(define-primitive denominator (number->) (lambda (n) 1) return-fixnum)

(define-primitive angle (number->)
  (lambda (n)
    (if (>= n 0)
	(goto return (enter-fixnum 0))
	(raise-exception arithmetic-overflow 0 n))))

; beware of (abs least-fixnum)
(define-primitive magnitude (number->)
  (lambda (n)
    (abs-carefully n
		   return
		   (lambda (n)
		     (raise-exception arithmetic-overflow 0 n)))))

; These primitives all just raise an exception and let the run-time system do
; the work.

(define-syntax define-punt-primitive
  (syntax-rules ()
    ((define-punt-primitive op)
     (define-primitive op (number->)
       (lambda (n)
	 (raise-exception arithmetic-overflow 0 n))))))

(define-punt-primitive exact->inexact)
(define-punt-primitive inexact->exact)
(define-punt-primitive exp)
(define-punt-primitive log)
(define-punt-primitive sin)
(define-punt-primitive cos)
(define-punt-primitive tan)
(define-punt-primitive asin)
(define-punt-primitive acos)
(define-punt-primitive sqrt)

(define-syntax define-punt2-primitive
  (syntax-rules ()
    ((define-punt2-primitive op)
     (define-primitive op (number-> number->)
       (lambda (n m) (raise-exception arithmetic-overflow 0 n m))))))

(define-punt2-primitive atan)
(define-punt2-primitive make-polar)
(define-punt2-primitive make-rectangular)

(define (arithmetic-overflow x y)
  (raise-exception arithmetic-overflow 0 x y))

(define (arith op)
  (lambda (x y)
    (goto op x y return arithmetic-overflow)))

(define-primitive +         (number->  number->) (arith add-carefully))
(define-primitive -         (number->  number->) (arith subtract-carefully))
(define-primitive *         (number->  number->) (arith multiply-carefully))
(define-primitive /         (number->  number->) (arith divide-carefully))
(define-primitive quotient  (integer-> integer->) (arith quotient-carefully))
(define-primitive remainder (integer-> integer->) (arith remainder-carefully))
(define-primitive =         (number->  number->) vm-=  return-boolean)
(define-primitive <         (real->    real->)   vm-<  return-boolean)
(define-primitive >         (real->    real->)   vm->  return-boolean)
(define-primitive <=        (real->    real->)   vm-<= return-boolean)
(define-primitive >=        (real->    real->)   vm->= return-boolean)
(define-primitive arithmetic-shift (number-> number->)
  (arith shift-carefully))

(define-primitive char?       (any->) vm-char? return-boolean)
(define-primitive char=?      (vm-char-> vm-char->) vm-char=? return-boolean)
(define-primitive char<?      (vm-char-> vm-char->) vm-char<? return-boolean)
(define-primitive char->ascii (char->) char->ascii return-fixnum)

(define-primitive ascii->char
  (fixnum->)
  (lambda (x)
    (if (or (> x 255) (< x 0))
        (raise-exception wrong-type-argument 0 (enter-fixnum x))
	(goto return (enter-char (ascii->char x))))))

(define-primitive eof-object?
  (any->)
  (lambda (x) (vm-eq? x eof-object))
  return-boolean)

(define-primitive bitwise-not (fixnum->)          bitwise-not return-fixnum)
(define-primitive bitwise-and (fixnum-> fixnum->) bitwise-and return-fixnum)
(define-primitive bitwise-ior (fixnum-> fixnum->) bitwise-ior return-fixnum)
(define-primitive bitwise-xor (fixnum-> fixnum->) bitwise-xor return-fixnum)


(define-primitive stored-object-has-type?
  (any->)
  (lambda (x)
    (goto continue-with-value
	  (enter-boolean (stob-of-type? x (code-byte 0)))
	  1)))

(define-primitive stored-object-length
  (any->)
  (lambda (stob)
    (let ((type (code-byte 0)))
      (if (stob-of-type? stob type)
	  (goto continue-with-value
		(enter-fixnum (d-vector-length stob))
		1)
	  (raise-exception wrong-type-argument 1 stob (enter-fixnum type))))))
  
; Fixed sized objects

(define-primitive stored-object-ref
  (any->)
  (lambda (stob)
    (let ((type (code-byte 0))
	  (offset (code-byte 1)))
      (if (stob-of-type? stob type)
	  (goto continue-with-value
		(d-vector-ref stob offset)
		2)
	  (raise-exception wrong-type-argument 2
			   stob
			   (enter-fixnum type)
			   (enter-fixnum offset))))))

(define-primitive stored-object-set!
  (any-> any->)
  (lambda (stob value)
    (let ((type (code-byte 0))
	  (offset (code-byte 1)))
      (cond ((and (stob-of-type? stob type)
		  (not (immutable? stob)))
	     (d-vector-set! stob offset value)
	     (goto continue-with-value
		   unspecific-value
		   2))
	    (else
	     (raise-exception wrong-type-argument 2
			      stob
			      (enter-fixnum type)
			      (enter-fixnum offset)
			      value))))))

; Indexed objects

(define-primitive stored-object-indexed-ref (any-> any->)
  (lambda (stob index)
    (let ((type (code-byte 0)))
      (cond ((or (not (fixnum? index))
	         (not (stob-of-type? stob type)))
	     (raise-exception wrong-type-argument 1
			      stob (enter-fixnum type) index))
	    ((valid-index? (extract-fixnum index) (d-vector-length stob))
	     (goto continue-with-value
		   (d-vector-ref stob (extract-fixnum index))
		   1))
	    (else
	     (raise-exception index-out-of-range 1
			      stob (enter-fixnum type) index))))))

(define-primitive stored-object-indexed-set! (any-> any-> any->)
  (lambda (stob index value)
    (let ((type (code-byte 0)))
      (cond ((or (not (fixnum? index))
		 (not (stob-of-type? stob type))
		 (immutable? stob))
	     (raise-exception wrong-type-argument 1
			      stob (enter-fixnum type) index value))
	    ((valid-index? (extract-fixnum index) (d-vector-length stob))
	     (d-vector-set! stob (extract-fixnum index) value)
	     (goto continue-with-value unspecific-value 1))
	    (else
	     (raise-exception index-out-of-range 1
			      stob (enter-fixnum type) index value))))))

; Hacko record handlers done for speed.

(define-primitive checked-record-ref
  (any-> any-> fixnum->)
  (lambda (record type index)
    (cond ((not (and (stob-of-type? record (enum stob record))
		     (vm-eq? type (record-ref record 0))))
	   (raise-exception wrong-type-argument 0
			    record type (enter-fixnum index)))
	  ((valid-index? index (record-length record))
	   (goto return (record-ref record index)))
	  (else
	   (raise-exception index-out-of-range 0
			    record type (enter-fixnum index))))))

(define-primitive checked-record-set! (any-> any-> fixnum-> any->)
  (lambda (record type index value)
    (cond ((not (and (stob-of-type? record (enum stob record))
		     (vm-eq? type (record-ref record 0))
		     (not (immutable? record))))
	   (raise-exception wrong-type-argument 0
			    record type (enter-fixnum index) value))
	  ((valid-index? index (record-length record))
	   (record-set! record index value)
	   (goto no-result))
	  (else
	   (raise-exception index-out-of-range 0
			    record type (enter-fixnum index) value)))))

; Constructors

(define-primitive make-stored-object ()
  (lambda ()
    (let* ((len (code-byte 0))
	   (key (ensure-space (cells->bytes (+ stob-overhead len))))
	   (new (make-d-vector (code-byte 1) len key)))
      (cond ((>= len 1)
	     (d-vector-init! new (- len 1) *val*)
	     (do ((i (- len 2) (- i 1)))
		 ((> 0 i)
		  (unspecific))  ; for the type checker!
	       (d-vector-init! new i (pop)))))
      (goto continue-with-value new 2))))

; This is for the closed compiled versions of VECTOR and RECORD.
; *stack* = arg0 arg1 ... argN rest-list N+1 total-nargs

(define-primitive closed-make-stored-object ()
  (lambda ()
    (let* ((len (extract-fixnum (pop)))
	   (key (ensure-space (cells->bytes (+ stob-overhead len))))
	   (new (make-d-vector (code-byte 0) len key))
	   (stack-nargs (extract-fixnum (pop)))
	   (rest-list (pop)))
      (do ((i (- stack-nargs 1) (- i 1)))
	  ((> 0 i)
	   (unspecific))  ; for the type checker!
	(d-vector-init! new i (pop)))
      (do ((i stack-nargs (+ i 1))
	   (rest-list rest-list (vm-cdr rest-list)))
	  ((vm-eq? rest-list null)
	   (unspecific))  ; for the type checker!
	(d-vector-init! new i (vm-car rest-list)))
      (goto continue-with-value new 1))))

(define-primitive make-vector-object (any-> any->)
  (lambda (len init)
    (let ((type (code-byte 0)))
      (if (fixnum? len)
	  (let* ((len (extract-fixnum len))
		 (size (vm-vector-size len)))
	    (if (or (< len 0)
		    (> size max-stob-size-in-cells))
		(raise-exception wrong-type-argument 1
				 (enter-fixnum type) (enter-fixnum len) init)
		(receive (okay? key init)
		    (maybe-ensure-space-saving-temp size init)
		  (if (not okay?)
		      (raise-exception heap-overflow 1
				       (enter-fixnum type) (enter-fixnum len)
				       init)
		      (let ((v (make-d-vector type len key)))
			(do ((i (- len 1) (- i 1)))
			    ((< i 0))
			  (d-vector-set! v i init))
			(goto continue-with-value v 1))))))
	  (raise-exception wrong-type-argument 1
			   (enter-fixnum type) len init)))))

; Strings and code-vectors

(define-primitive string-length
  (string->)
  (lambda (string)
    (goto return-fixnum (vm-string-length string))))

(define-primitive code-vector-length
  (code-vector->)
  (lambda (code-vector)
    (goto return-fixnum (code-vector-length code-vector))))

(define (make-byte-ref ref length returner)
  (lambda (vector index)
    (if (valid-index? index (length vector))
	(goto returner (ref vector index))
	(raise-exception index-out-of-range 0 vector (enter-fixnum index)))))

(let ((proc (make-byte-ref vm-string-ref vm-string-length return-char)))
  (define-primitive string-ref (string-> fixnum->) proc))

(let ((proc (make-byte-ref code-vector-ref code-vector-length return-fixnum)))
  (define-primitive code-vector-ref (code-vector-> fixnum->) proc))

(define (make-byte-setter setter length enter-elt)
  (lambda (vector index char)
    (cond ((immutable? vector)
	   (raise-exception wrong-type-argument 0
			    vector (enter-fixnum index) (enter-elt char)))
	  ((valid-index? index (length vector))
	   (setter vector index char)
	   (goto no-result))
	  (else
	   (raise-exception index-out-of-range 0
			    vector (enter-fixnum index) (enter-elt char))))))

(let ((proc (make-byte-setter vm-string-set! vm-string-length enter-char)))
  (define-primitive string-set! (string-> fixnum-> char->) proc))

(let ((proc (make-byte-setter code-vector-set! code-vector-length enter-fixnum)))
  (define-primitive code-vector-set! (code-vector-> fixnum-> fixnum->) proc))

(define (byte-vector-maker size maker setter enter-elt)
  (lambda (len init)
    (let ((size (size len)))
      (if (or (< len 0)
	      (> size max-stob-size-in-cells))
	  (raise-exception wrong-type-argument 0
			   (enter-fixnum len) (enter-elt init))
	  (receive (okay? key ignore)
	      (maybe-ensure-space-saving-temp size (enter-fixnum 0))
	    (if (not okay?)
		(raise-exception heap-overflow 0
				 (enter-fixnum len) (enter-elt init))
		(let ((vector (maker len key)))
		  (do ((i (- len 1) (- i 1)))
		      ((< i 0))
		    (setter vector i init))
		  (goto return vector))))))))

(let ((proc (byte-vector-maker vm-string-size
			       vm-make-string
			       vm-string-set!
			       enter-char)))
  (define-primitive make-string (fixnum-> char->) proc))
  
(let ((proc (byte-vector-maker code-vector-size
			       make-code-vector
			       code-vector-set!
			       enter-fixnum)))
  (define-primitive make-code-vector (fixnum-> fixnum->) proc))

; Locations & mutability

(define-primitive location-defined? (location->)
  (lambda (loc)
    (return-boolean (or (not (undefined? (contents loc)))
			(= (contents loc) unassigned-marker)))))

(define-primitive set-location-defined?! (location-> boolean->)
  (lambda (loc value)
    (cond ((not value)
	   (set-contents! loc unbound-marker))
	  ((undefined? (contents loc))
	   (set-contents! loc unassigned-marker))))
  return-unspecific)

(define-primitive immutable? (any->) immutable? return-boolean)

(define-primitive make-immutable! (any->)
  (lambda (thing)
    (make-immutable! thing)
    (goto return thing)))


; Misc

(define-primitive false ()
  (lambda ()
    (goto return false)))

(define-primitive eof-object ()
  (lambda ()
    (goto return eof-object)))

(define-primitive trap (any->)
  (lambda (arg)
    (raise-exception trap 0 arg)))

(define-primitive find-all-symbols (vector->)
  (lambda (table)
    (let loop ((table table) (first? #t))
      (let ((symbols (find-all (enum stob symbol))))
	(cond ((not (false? symbols))
	       (really-find-all-symbols table symbols))
	      (first?
	       (loop (collect-saving-temp table) #f))
	      (else
	       (raise-exception heap-overflow 0 table)))))))

(define (really-find-all-symbols table symbols)
  (let loop ((table table) (symbols symbols) (first? #t))
    (let index-loop ((i 0))
      (cond ((= i (vm-vector-length symbols))
	     (goto no-result))
	    ((available? vm-pair-size)
	     (let ((key (preallocate-space vm-pair-size)))
	       (add-to-symbol-table (vm-vector-ref symbols i) table key)
	       (index-loop (+ i 1))))
	    (first?
	     (do ((i 1 (+ i 1)))                     ; clear table
		 ((= i (vm-vector-length table)))
	       (vm-vector-set! table i null))
	     (vm-vector-set! table 0 symbols)        ; save symbols in table
	     (let ((table (collect-saving-temp table)))
	       (let ((symbols (vm-vector-ref table 0)))
		 (vm-vector-set! table 0 null)
		 (loop table symbols #f))))
	    (else
	     (raise-exception heap-overflow 0 table))))))

(define-primitive find-all (fixnum->)
  (lambda (type)
    (let loop ((first? #t))
      (let ((vector (find-all type)))
	(cond ((not (false? vector))
	       (goto return vector))
	      (first?
	       (collect)
	       (loop #f))
	      (else
	       (raise-exception heap-overflow 0 (enter-fixnum type))))))))

(define-primitive find-all-records (any->)
  (lambda (type)
    (let loop ((first? #t) (type type))
      (let ((vector (find-all-records type)))   ; only one call site
	(cond ((not (false? vector))
	       (goto return vector))
	      (first?
	       (loop #f (collect-saving-temp type)))
	      (else
	       (raise-exception heap-overflow 0 type)))))))

(define-primitive collect ()
  (lambda ()
    (set! *val* unspecific-value)
    (collect)
    (goto continue 0)))

(define-consing-primitive add-finalizer! (any-> any->)
  (lambda (n) (* 2 vm-pair-size))
  (lambda (stob proc key)
    (cond ((not (and (stob? stob)
		     (closure? proc)))
	   (raise-exception wrong-type-argument 0 stob proc))
; This would be useful but could get quite expensive
;	  ((vm-assq stob *finalizer-alist*)
;	   (raise-exception has-finalizer 0 stob proc))
	  (else
	   (set! *finalizer-alist*
		 (vm-cons (vm-cons stob proc key)
			  *finalizer-alist*
			  key))
	   (goto no-result)))))

(define-primitive memory-status (fixnum-> any->)
  (lambda (key other)
    (enum-case memory-status-option key
      ((pointer-hash)
       (goto return (descriptor->fixnum other)))
      ((available)
       (goto return-fixnum (available)))
      ((heap-size)
       (goto return-fixnum (bytes->cells (heap-size))))
      ((stack-size)
       (goto return-fixnum (stack-size)))
      ((gc-count)
       (goto return-fixnum (gc-count)))
      ((expand-heap!)
       (raise-exception unimplemented-instruction 0 (enter-fixnum key) other))
      (else
       (raise-exception bad-option 0 (enter-fixnum key) other)))))

(define-primitive time (fixnum-> any->)
  (lambda (option other)
    (enum-case time-option option
      ((cheap-time)
       (goto return-fixnum (cheap-time)))
      ((run-time)
       (receive (seconds mseconds)
	   (run-time)
	 (goto return-time-value option seconds mseconds)))
      ((real-time)
       (receive (seconds mseconds)
	   (real-time)
	 (goto return-time-value option seconds mseconds)))
      (else
       (raise-exception bad-option 0 (enter-fixnum option) other)))))
      
; The largest number of seconds that can be converted into a fixnum number
; of milliseconds.

(define maximum-seconds (quotient (- greatest-fixnum-value 1000) 1000))

(define (return-time-value option seconds mseconds)
  (if (> seconds maximum-seconds)
      (raise-exception arithmetic-overflow 0
		       (enter-fixnum option)
		       (enter-fixnum seconds)
		       (enter-fixnum mseconds))
      (goto return-fixnum (+ (* seconds 1000) mseconds))))

(define-primitive schedule-interrupt (fixnum->)
  (lambda (delta)
    (clear-interrupt! (enum interrupt alarm))
    (goto return-fixnum (schedule-interrupt delta))))

; Convert from the user's exponent to the system's.

;(define (adjust-time mantissa exponent)
;  (let ((system (clock-exponent)))
;    (cond ((= exponent system)
;           mantissa)
;          ((> system exponent)
;           (quotient mantissa (expt 10 (- system exponent))))
;          (else
;           (* mantissa (expt 10 (- exponent system)))))))

(define external-> (input-type external? no-coercion))

(define-primitive external-lookup (external->)
  (lambda (external)
    (let ((name (external-name external))
	  (value (external-value external)))
      (if (and (vm-string? name)
	       (code-vector? value)
	       (lookup-external-name (address-after-header name)
				     (address-after-header value)))
	  (goto no-result)
	  (raise-exception unbound-external-name 0 external)))))

; The arguments have been pushed on the stack after the procedure.
; *stack* = procedure arg1 ... argN rest-list N+1 total-nargs
;
; The top-level driver loop removes the procedure and the arguments from
; the stack.

(define-primitive external-call ()
  (lambda ()
    (let* ((nargs (extract-fixnum (pop)))
	   (stack-nargs (extract-fixnum (pop)))
	   (rest-list (pop)))
      (do ((rest-list rest-list (vm-cdr rest-list)))
	  ((vm-eq? rest-list null))
	(push (vm-car rest-list)))
      (let ((proc (stack-ref (- nargs 1))))
	(cond ((not (external? proc))    ; lots of junk on the stack...
	       (raise-exception wrong-type-argument 0 proc))
	      (else
	       (set! *external-call-nargs* (- nargs 1)) ; don't count proc
	       (set! *val* proc)
	       (set! *code-pointer* (address+ *code-pointer* 1))
	       (enum return-option external-call))))))) ; return to driver loop

(define-enumeration vm-extension-status
  (okay
   exception
   ))

(define *extension-value* (unspecific))

(define-primitive vm-extension (fixnum-> any->)
  (lambda (key value)
    (let ((status (extended-vm key value)))
      (cond ((vm-eq? status (enum vm-extension-status okay))
	     (goto return *extension-value*))
	    ((vm-eq? status (enum vm-extension-status exception))
	     (raise-exception extension-exception 0 (enter-fixnum key) value))
	    (else
	     (raise-exception extension-return-error 0 (enter-fixnum key) value))))))

; This is exported to keep *EXTENSION-VALUE* from being eliminated by the
; compiler.

(define (set-extension-value! value)
  (set! *extension-value* value))

(define-primitive vm-return (fixnum-> any->)
  (lambda (key value)
    (set! *val* value)
    (enum return-option exit)))            ; the VM returns this value

(define-primitive current-thread ()
  (lambda () *current-thread*)
  return-any)

(define-primitive set-current-thread! (any->)
  (lambda (state)
    (set! *current-thread* state))
  return-unspecific)

(define-primitive session-data ()
  (lambda () *session-data*)
  return-any)

(define-primitive set-session-data! (any->)
  (lambda (state)
    (set! *session-data* state))
  return-unspecific)

; Unnecessary primitives

(define-primitive string=? (string-> string->) vm-string=? return-boolean)

; Special primitive called by the reader.
; Primitive for the sake of speed.  Probably should be flushed.

(define-consing-primitive reverse-list->string (any-> fixnum->) 
  (lambda (n) (vm-string-size (extract-fixnum n)))
  (lambda (l n k)
    (if (not (or (vm-pair? l) (vm-eq? l null)))
        (raise-exception wrong-type-argument 0 l (enter-fixnum n))
        (let ((obj (vm-make-string n k)))
          (do ((l l (vm-cdr l))
               (i (- n 1) (- i 1)))
              ((< i 0)
	       (goto return obj))
            (vm-string-set! obj i (extract-char (vm-car l))))))))

(define-primitive string-hash (string->) vm-string-hash return-fixnum)

(define-consing-primitive intern (string-> vector->)
  (lambda (ignore) (+ vm-symbol-size vm-pair-size))
  intern
  return)

; Messy because we have to detect circular lists (alternatively we
; could check for interrupts and then pclsr).  ***

(define-primitive assq (any-> any->)
  (lambda (thing list)
    (let ((lose (lambda ()
		  (raise-exception wrong-type-argument 0 thing list))))
      (let loop ((list list) (slow list) (move-slow? #t))
	(cond ((vm-eq? list null)
	       (goto return-boolean #f))
	      ((not (vm-pair? list))
	       (lose))
	      (else
	       (let ((head (vm-car list)))
		 (cond ((not (vm-pair? head))
			(lose))
		       ((vm-eq? (vm-car head) thing)
			(goto return head))
		       (else
			(let ((list (vm-cdr list)))
			  (cond ((eq? list slow)
				 (lose))
				(move-slow?
				 (loop list (vm-cdr slow) #f))
				(else
				 (loop list slow #t)))))))))))))

; Eventually add make-table, table-ref, table-set! as primitives?
; No -- write a compiler instead.

; *** Our entry for the obscure comment of the year contest.
;
; Pclsring is the term in ITS for the mechanism that makes the operating system
; appear to be a virtual machine.  The paradigm is that of the BLT instruction
; on the PDP-10: its arguments are in a set of registers, and if the instruction
; gets interrupted in the middle, the registers reflect the intermediate state;
; the PC is set to the BLT instruction itself, and the process can be resumed
; in the usual way.
; For more on pclsring see `Pclsring: Keeping Process State Modular' by Alan
; Bawden (ftp.ai.mit.edu:pub/alan/pclsr.memo).
