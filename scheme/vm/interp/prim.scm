; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Scalar primitives

(define-primitive eq? (any-> any->) vm-eq? return-boolean)

(define-primitive char?       (any->) vm-char? return-boolean)
(define-primitive char=?      (vm-char-> vm-char->) vm-char=? return-boolean)
(define-primitive char<?      (vm-char-> vm-char->) vm-char<? return-boolean)

(define-primitive char->scalar-value (char-scalar-value->) (lambda (c) c) return-fixnum)

; Unicode surrogates are not scalar values

(define (scalar-value? x)
  (and (>= x 0)
       (or (<= x #xd7ff)
	   (and (>= x #xe000) (<= x #x10ffff)))))

(define-primitive scalar-value->char
  (fixnum->)
  (lambda (x)
    (if (scalar-value? x)
	(goto return (scalar-value->char x))
	(raise-exception wrong-type-argument 0 (enter-fixnum x)))))

(define-primitive scalar-value?
  (fixnum->)
  scalar-value?
  return-boolean)

(define-syntax define-encode-char
  (syntax-rules ()
    ((define-encode-char ?name ?cont)
     (define-primitive ?name (fixnum-> char-scalar-value-> code-vector-> fixnum-> fixnum->)
       (lambda (encoding value buffer start count)
	 (if (or (immutable? buffer)
		 (> 0 start)
		 (> 0 count)
		 (> (+ start count) (code-vector-length buffer)))
	     (raise-exception wrong-type-argument 0
			      (enter-fixnum encoding)
			      (scalar-value->char value)
			      buffer (enter-fixnum start) (enter-fixnum count))
	     (call-with-values
		 (lambda () 
		   (encode-scalar-value encoding value buffer start count))
	       (lambda (encoding-ok? ok? out-of-space? count)
		 (if encoding-ok?
		     (call-with-values
			 (lambda () (values ok? out-of-space? count))
		       ?cont)
		     (raise-exception bad-option 0
				      (enter-fixnum encoding)))))))))))

(define-encode-char encode-char
  (lambda (ok? out-of-space? count)
    (push (enter-boolean (and ok? (not out-of-space?))))
    (push (if ok? (enter-fixnum count) false))
    (goto return-values 2 null 0)))

(define-encode-char encode-char!
  (lambda (ok? out-of-space? count)
    (goto return unspecific-value)))

(define-syntax define-decode-char
  (syntax-rules ()
    ((define-decode-char ?name ?cont)
     (define-primitive ?name (fixnum-> code-vector-> fixnum-> fixnum->)
       (lambda (encoding buffer start count)
	 (if (or (> 0 start)
		 (> 0 count)
		 (> (+ start count) (code-vector-length buffer)))
	     (raise-exception wrong-type-argument 0
			      (enter-fixnum encoding) buffer (enter-fixnum start) (enter-fixnum count))
	     (call-with-values
		 (lambda () (decode-scalar-value encoding buffer start count))
	       (lambda (encoding-ok? ok? incomplete? value count)
		 (if (not encoding-ok?)
		     (raise-exception bad-option 0
				      (enter-fixnum encoding))
		     (call-with-values
			 (lambda () (values ok? incomplete? value count))
		       ?cont))))))))))

(define-decode-char decode-char
  (lambda (ok? incomplete? value count)
    (push (if (and ok? (not incomplete?))
	      (scalar-value->char value)
	      false))
    (push (if ok? (enter-fixnum count) false))
    (goto return-values 2 null 0)))

; this makes limited sense: we only get the exception side effect
(define-decode-char decode-char!
  (lambda (ok? incomplete? value count)
    (goto return unspecific-value)))

(define-primitive eof-object?
  (any->)
  (lambda (x) (vm-eq? x eof-object))
  return-boolean)

;----------------

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
  
; Constructors

(define-primitive make-stored-object ()
  (lambda ()
    (let* ((len (code-byte 0))
	   (key (ensure-space (+ stob-overhead len)))
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
	   (key (ensure-space (+ stob-overhead len)))
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
		(begin
		  (save-temp0! init)
		  (let* ((v (maybe-make-d-vector+gc type len))
			 (init (recover-temp0!)))
		    (if (false? v)
			(raise-exception heap-overflow 1
					 (enter-fixnum type) (enter-fixnum len)
					 init)
			(begin
			  (do ((i (- len 1) (- i 1)))
			      ((< i 0))
			    (d-vector-set! v i init))
			  (goto continue-with-value v 1)))))))
	  (raise-exception wrong-type-argument 1
			   (enter-fixnum type) len init)))))

; Doubles

(define-primitive make-double ()
  (lambda ()
    (let* ((len 8)			; IEEE 754 double precision
	   (new (maybe-make-b-vector+gc (enum stob double) len)))
      (if (false? new)
	  (raise-exception heap-overflow 0
			   (enter-fixnum (enum stob double)))
	  (begin
;	    (do ((i (- len 1) (- i 1)))
;		((< i 0))
;	      (b-vector-set! new i 0))
	    (goto return new))))))

; Strings and byte vectors

(define-primitive string-length
  (string->)
  (lambda (string)
    (goto return-fixnum (vm-string-length string))))

(define-primitive byte-vector-length
  (code-vector->)
  (lambda (byte-vector)
    (goto return-fixnum (code-vector-length byte-vector))))

(define (make-byte-ref ref length returner)
  (lambda (vector index)
    (if (valid-index? index (length vector))
	(goto returner (ref vector index))
	(raise-exception index-out-of-range 0 vector (enter-fixnum index)))))

(let ((proc (make-byte-ref vm-string-ref vm-string-length return-scalar-value-char)))
  (define-primitive string-ref (string-> fixnum->) proc))

(let ((proc (make-byte-ref code-vector-ref code-vector-length return-fixnum)))
  (define-primitive byte-vector-ref (code-vector-> fixnum->) proc))

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

(let ((proc (make-byte-setter vm-string-set! vm-string-length scalar-value->char)))
  (define-primitive string-set! (string-> fixnum-> char-scalar-value->) proc))

(let ((proc (make-byte-setter code-vector-set! code-vector-length enter-fixnum)))
  (define-primitive byte-vector-set! (code-vector-> fixnum-> fixnum->) proc))

(define (byte-vector-maker size bytes type initialize setter enter-elt)
  (lambda (len init)
    (let ((size (size len)))
      (if (or (< len 0)
	      (> size max-stob-size-in-cells))
	  (raise-exception wrong-type-argument
			   0
			   (enter-fixnum len)
			   (enter-elt init))
	  (let ((vector (maybe-make-b-vector+gc type (bytes len))))
	    (if (false? vector)
		(raise-exception heap-overflow
				 0
				 (enter-fixnum len)
				 (enter-elt init))
		(begin
		  (initialize vector len)
		  (do ((i (- len 1) (- i 1)))
		      ((< i 0))
		    (setter vector i init))
		  (goto return vector))))))))

(let ((proc (byte-vector-maker vm-string-size
			       scalar-value-units->bytes
			       (enum stob string)
			       (lambda (string length)
				 0)
			       vm-string-set!
			       scalar-value->char)))
  (define-primitive make-string (fixnum-> char-scalar-value->) proc))
  
(let ((proc (byte-vector-maker code-vector-size
			       (lambda (len) len)
			       (enum stob byte-vector)
			       (lambda (byte-vector length) 0)
			       code-vector-set!
			       enter-fixnum)))
  (define-primitive make-byte-vector (fixnum-> fixnum->) proc))

(define-primitive copy-string-chars! (string-> fixnum-> string-> fixnum-> fixnum->)
  (lambda (from from-index to to-index count)
    (cond ((not (and (okay-copy-string? from from-index count)
		     (okay-copy-string? to   to-index   count)
		     (not (immutable? to))
		     (<= 0 count)))
	   (raise-exception wrong-type-argument 0
			    from (enter-fixnum from-index)
			    to (enter-fixnum to-index)
			    (enter-fixnum count)))
	  (else
	   (copy-vm-string-chars! from from-index to to-index count)
	   (goto continue-with-value unspecific-value 0)))))

(define (okay-copy-string? s index count)
  (and (<= 0 index)
       (<= (+ index count)
	   (vm-string-length s))))

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

(define-primitive make-mutable! (any->)
  (lambda (thing)
    (make-mutable! thing)
    (goto return thing)))

(define-primitive make-weak-pointer (any->)
  (lambda (init)
    (let ((weak-pointer (make-weak-pointer init weak-pointer-size)))
      (goto continue-with-value
	    weak-pointer
	    0))))
;----------------
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

(define-primitive find-all (fixnum->)
  (lambda (type)
    (let loop ((first? #t))
      (let ((vector (s48-find-all type)))
	(cond ((not (false? vector))
	       (goto return vector))
	      (first?
	       ;; if the result vector couldn't be created force a
	       ;; major collection and try again once.
	       (s48-collect #t)
	       (loop #f))
	      (else
	       (raise-exception heap-overflow 0 (enter-fixnum type))))))))

(define-primitive find-all-records (any->)
  (lambda (type)
    (let loop ((first? #t) (type type))
      (let ((vector (s48-find-all-records type)))
	(cond ((not (false? vector))
	       (goto return vector))
	      (first?
	       (save-temp0! type)
	       (s48-collect #t)
	       (loop #f (recover-temp0!)))
	      (else
	       (raise-exception heap-overflow 0 type)))))))

(define-primitive collect ()
  (lambda ()
    ;; does a major collection in any case
    (set! *val* unspecific-value)
    (s48-collect #t)
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
	   (get-proposal-lock!)
	   (shared-set! *finalizer-alist*
			(vm-cons (vm-cons stob proc key)
				 (shared-ref *finalizer-alist*)
				 key))
	   (release-proposal-lock!)
	   (goto no-result)))))

(define-primitive memory-status (fixnum-> any->)
  (lambda (key other)
    (enum-case memory-status-option key
      ((pointer-hash)
       (goto return (descriptor->fixnum other)))
      ((available)
       (goto return-fixnum (s48-available)))
      ((heap-size)
       (goto return-fixnum (bytes->cells (s48-heap-size))))
      ((max-heap-size)
       (goto return-fixnum (s48-max-heap-size)))
      ((stack-size)
       (goto return-fixnum (stack-size)))
      ((gc-count)
       (goto return-fixnum (s48-gc-count)))
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

(define-enumeration vm-extension-status
  (okay
   exception
   ))

(define s48-*extension-value*)

(define-primitive vm-extension (fixnum-> any->)
  (lambda (key value)
    (let ((status (extended-vm key value)))
      (cond ((vm-eq? status (enum vm-extension-status okay))
	     (goto return s48-*extension-value*))
	    ((vm-eq? status (enum vm-extension-status exception))
	     (raise-exception extension-exception 0 (enter-fixnum key) value))
	    (else
	     (raise-exception extension-return-error 0 (enter-fixnum key) value))))))

; This is exported to keep s48-*EXTENSION-VALUE* from being eliminated by the
; compiler.

(define (s48-set-extension-value! value)
  (set! s48-*extension-value* value))

; Used to indicate which stack block we are returning to.  Set to FALSE if we are
; returning from the VM as a whole.
(define s48-*callback-return-stack-block* false)

(define-primitive return-from-callback (any-> any->)
  (lambda (stack-block value)
    (enable-interrupts!)	; Disabled to ensure that we return to the right
				; stack block.
    (set! s48-*callback-return-stack-block* stack-block)
    value))                     ; the interpreter returns this value

(define-primitive current-thread ()
  (lambda () *current-thread*)
  return-any)

(define-primitive set-current-thread! (any->)
  (lambda (state)
    (set! *current-thread* state))
  return-unspecific)

(define-primitive session-data ()
  (lambda () (shared-ref *session-data*))
  return-any)

(define-primitive set-session-data! (any->)
  (lambda (state)
    (shared-set! *session-data* state))
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
            (vm-string-set! obj i (char->scalar-value (vm-car l))))))))

(define-primitive string-hash (string->) vm-string-hash return-fixnum)

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
