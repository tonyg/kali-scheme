; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file prim.scm.
; Requires DEFINE-PRIMITIVE macro.

;;;; VM data manipulation primitives

; Input checking and coercion

(define (input-type pred coercer)  ;Alonzo wins
  (lambda (f) (f pred coercer)))

(define (input-type-predicate type) (type (lambda (x y) y x)))
(define (input-type-coercion type)  (type (lambda (x y) x y)))

(define (no-coercion x) x)

(define any->      (input-type (lambda (x) x #t) no-coercion))
(define fixnum->   (input-type fixnum?  extract-fixnum))
(define char->     (input-type vm-char? extract-char))
(define boolean->  (input-type vm-boolean? extract-boolean))
(define location-> (input-type location? no-coercion))
(define string->   (input-type vm-string? no-coercion))
(define vector->   (input-type vm-vector? no-coercion))

; Output coercion

(define (return val)
  (set! *val* val)
  (goto interpret))

(define return-any return)

(define (return-boolean x)
  (goto return (enter-boolean x)))

(define (return-fixnum x)
  (goto return (enter-fixnum x)))

(define (return-char x)
  (goto return (enter-char x)))

(define (return-unspecific x)
  x ;ignored
  (goto return unspecific))

; Scalar primitives

(define-primitive op/eq? (any-> any->) vm-eq? return-boolean)

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

(define-primitive op/number?   (any->) vm-number?   return-boolean)

(define (define-numeric-predicate op)
  (define-primitive op
    (any->)
    (lambda (n)
      (cond ((fixnum? n)
	     (goto return (enter-boolean #t)))
	    ((extended-number? n)
	     (goto raise-exception1 0 n))
	    (else
	     (goto return (enter-boolean #f)))))))

(define-numeric-predicate op/integer?)
(define-numeric-predicate op/rational?)
(define-numeric-predicate op/real?)
(define-numeric-predicate op/complex?)

; These primitives have a simple answer in the case of fixnums; for all other
; they punt to the run-time system.

(define-primitive op/exact?      (number->) (lambda (n) #t) return-boolean)
(define-primitive op/real-part   (number->) (lambda (n) (goto return n)))
(define-primitive op/imag-part   (number->) (lambda (n)
					      (goto return (enter-fixnum 0))))
(define-primitive op/floor       (number->) (lambda (n) (goto return n)))
(define-primitive op/numerator   (number->) (lambda (n) (goto return n)))
(define-primitive op/denominator (number->) (lambda (n)
					      (goto return (enter-fixnum 1))))
(define-primitive op/angle       (number->) (lambda (n)
					      (if (>= n 0)
						  (goto return (enter-fixnum 0))
						  (goto raise-exception1 0 n))))

; beware of (abs least-fixnum)
(define-primitive op/magnitude   (number->)
  (lambda (n)
    (let ((r (abs n)))
      (if (too-big-for-fixnum? r)
	  (goto raise-exception1 0 n)
	  (goto return (enter-fixnum r))))))

; These primitives all just raise an exception and let the run-time system do
; the work.

(define (define-punt-primitive op)
  (define-primitive op (number->) (lambda (n) (goto raise-exception1 0 n))))

(define-punt-primitive op/exact->inexact)
(define-punt-primitive op/inexact->exact)
(define-punt-primitive op/exp)
(define-punt-primitive op/log)
(define-punt-primitive op/sin)
(define-punt-primitive op/cos)
(define-punt-primitive op/tan)
(define-punt-primitive op/asin)
(define-punt-primitive op/acos)
(define-punt-primitive op/atan)
(define-punt-primitive op/sqrt)

(define (define-punt2-primitive op)
  (define-primitive op (number-> number->)
    (lambda (n m) (goto raise-exception2 0 n m))))

(define-punt2-primitive op/make-polar)
(define-punt2-primitive op/make-rectangular)

(define (arithmetic-overflow x y)
  (goto raise-exception2 0 x y))

(define (arith op)
  (lambda (x y)
    (goto op x y return arithmetic-overflow)))

(define-primitive op/+         (number->  number->) (arith add-carefully))
(define-primitive op/-         (number->  number->) (arith subtract-carefully))
(define-primitive op/*         (number->  number->) (arith multiply-carefully))
(define-primitive op//         (number->  number->) (arith divide-carefully))
(define-primitive op/quotient  (integer-> integer->) (arith quotient-carefully))
(define-primitive op/remainder (integer-> integer->) (arith remainder-carefully))
(define-primitive op/=         (number->  number->) vm-= return-boolean)
(define-primitive op/<         (real->    real->)   vm-< return-boolean)
(define-primitive op/arithmetic-shift (number-> number->)
  (arith shift-carefully))

(define-primitive op/char?       (any->) vm-char? return-boolean)
(define-primitive op/char=?      (char-> char->) vm-char=? return-boolean)
(define-primitive op/char<?      (char-> char->) vm-char<? return-boolean)
(define-primitive op/char->ascii (char->) char->ascii return-fixnum)

(define-primitive op/ascii->char
  (fixnum->)
  (lambda (x)
    (if (or (> x 255) (< x 0))
        (goto raise-exception1 0 (enter-fixnum x))
	(goto return (enter-char (ascii->char x))))))

(define-primitive op/eof-object?
  (any->)
  (lambda (x) (vm-eq? x eof-object))
  return-boolean)

(define-primitive op/bitwise-not (fixnum->)          bitwise-not return-fixnum)
(define-primitive op/bitwise-and (fixnum-> fixnum->) bitwise-and return-fixnum)
(define-primitive op/bitwise-ior (fixnum-> fixnum->) bitwise-ior return-fixnum)
(define-primitive op/bitwise-xor (fixnum-> fixnum->) bitwise-xor return-fixnum)


(define-primitive op/stored-object-has-type?
  (any->)
  (lambda (x)
    (stob-of-type? x (next-byte)))
  return-boolean)

(define-primitive op/stored-object-length
  (any->)
  (lambda (stob)
    (let ((type (next-byte)))
      (if (stob-of-type? stob type)
	  (goto return-fixnum (d-vector-length stob))
	  (goto raise-exception2 1 stob (enter-fixnum type))))))
  
(define-primitive op/stored-object-byte-length
  (any->)
  (lambda (stob)
    (let ((type (next-byte)))
      (if (stob-of-type? stob type)
	  (goto return-fixnum (b-vector-length stob))
	  (goto raise-exception2 1 stob (enter-fixnum type))))))

; Fixed sized objects

(define-primitive op/stored-object-ref
  (any->)
  (lambda (stob)
    (let* ((type (next-byte))
	   (offset (next-byte)))
      (if (stob-of-type? stob type)
	  (goto return (d-vector-ref stob offset))
	  (goto raise-exception3 2
		stob
		(enter-fixnum type)
		(enter-fixnum offset))))))

(define-primitive op/stored-object-set!
  (any-> any->)
  (lambda (stob value)
    (let* ((type (next-byte))
	   (offset (next-byte)))
      (cond ((and (stob-of-type? stob type)
		  (not (immutable? stob)))
	     (d-vector-set! stob offset value)
	     (goto return unspecific))
	    (else
	     (goto raise-exception4 2
		   stob
		   (enter-fixnum type)
		   (enter-fixnum offset)
		   value))))))

; Indexed objects

(define-primitive op/stored-object-indexed-ref
  (any-> any->)
  (stob-indexed-ref d-vector-length d-vector-ref return))

(define-primitive op/stored-object-indexed-byte-ref
  (any-> any->)
  (stob-indexed-ref b-vector-length b-vector-ref return-fixnum))

(define-primitive op/stored-object-indexed-char-ref
  (any-> any->)
  (stob-indexed-ref b-vector-length vm-string-ref return-char))

(define (stob-indexed-ref length ref returner)
  (lambda (stob index)
    (let* ((type (next-byte))
	   (lose (lambda ()
		   (goto raise-exception3 1 stob (enter-fixnum type) index))))
      (if (fixnum? index)
	  (let ((index (extract-fixnum index)))
	    (if (and (stob-of-type? stob type)
		     (valid-index? index (length stob)))
		(goto returner (ref stob index))
		(lose)))
	  (lose)))))

(define-primitive op/stored-object-indexed-set!
  (any-> any-> any->)
  (stob-indexed-setter d-vector-length any-> d-vector-set!))

(define-primitive op/stored-object-indexed-byte-set!
  (any-> any-> any->)
  (stob-indexed-setter b-vector-length fixnum-> b-vector-set!))

(define-primitive op/stored-object-indexed-char-set!
  (any-> any-> any->)
  (stob-indexed-setter b-vector-length char-> vm-string-set!))

(define (stob-indexed-setter length value-type setter!)
  (let ((value? (input-type-predicate value-type))
	(extract-value (input-type-coercion value-type)))
    (lambda (stob index value)
      (let ((type (next-byte)))
	(cond ((and (stob-of-type? stob type)
		    (fixnum? index)
		    (valid-index? (extract-fixnum index) (length stob))
		    (value? value)
		    (not (immutable? stob)))
	       (setter! stob (extract-fixnum index) (extract-value value))
	       (goto return unspecific))
	      (else
	       (goto raise-exception4 1 stob (enter-fixnum type) index value)))))))
  
; Constructors

(define-primitive op/make-stored-object
  ()
  (lambda ()
    (let* ((key (ensure-space (cells->bytes (this-byte))))
	   (len (next-byte))  ; can't consume this byte until after ENSURE-SPACE
	   (new (make-d-vector (next-byte) len key)))
      (cond ((>= len 1)
	     (d-vector-set! new (- len 1) *val*)
	     (do ((i (- len 2) (- i 1)))
		 ((> 0 i))
	       (d-vector-set! new i (pop)))))
      new))
  return)

(define-primitive op/make-vector-object
  (any-> any->)
  (vector-maker cells->bytes make-d-vector d-vector-set! any->))

(define-primitive op/make-byte-vector-object
  (any-> any->)
  (vector-maker (lambda (x) x) make-b-vector b-vector-set! fixnum->))

(define-primitive op/make-char-vector-object
  (any-> any->)
  (vector-maker (lambda (x) x) make-b-vector vm-string-set! char->))

(define (vector-maker size make set init-type)
  (let ((init? (input-type-predicate init-type))
	(extract-init (input-type-coercion init-type)))
    (lambda (len init)
      (let* ((type (next-byte))
	     (finish (lambda (len init)
		       (let* ((raw-init (extract-init init))
			      (key (preallocate-space (size len)))
			      (v (make type len key)))
			 ;; Clear out storage
			 (do ((i (- len 1) (- i 1)))
			     ((< i 0))
			   (set v i raw-init))
			 (goto return v))))
	     (lose (lambda ()
		     (goto raise-exception3 1 (enter-fixnum type) len init))))
	(if (or (not (fixnum? len))
		(not (init? init)))
	    (lose)
	    (let ((len (extract-fixnum len)))
	      (cond ((not (>= len 0))
		     (lose))
		    ((available? (size len))
		     (finish len init))
		    (else
		     (let ((init (collect-saving-temp init)))
		       (if (available? (size len))
			   (finish len init)
			   (lose)))))))))))

(define-primitive op/location-defined? (location->)
  (lambda (loc)
    (return-boolean (or (not (undefined? (contents loc)))
			(= (contents loc) unassigned-marker)))))

(define-primitive op/set-location-defined?! (location-> boolean->)
  (lambda (loc value)
    (cond ((not value)
	   (set-contents! loc unbound-marker))
	  ((undefined? (contents loc))
	   (set-contents! loc unassigned-marker)))
    (goto return unspecific)))

(define-primitive op/immutable?      (any->) immutable?      return-boolean)
(define-primitive op/make-immutable! (any->) make-immutable! return-unspecific)


; I/O primitives

(define (vm-input-port? obj)
  (and (port? obj)
       (= (port-mode obj) (enter-fixnum for-input))))

(define (vm-output-port? obj)
  (and (port? obj)
       (= (port-mode obj) (enter-fixnum for-output))))

(define port->        (input-type port?           no-coercion))
(define input-port->  (input-type vm-input-port?  no-coercion))
(define output-port-> (input-type vm-output-port? no-coercion))
(define open-input-port->
  (input-type (lambda (p)
		(and (vm-input-port? p)
		     (open? p)))
	      no-coercion))
(define open-output-port->
  (input-type (lambda (p)
		(and (vm-output-port? p)
		     (open? p)))
	      extract-port))

(define-primitive op/input-port? (any->) vm-input-port? return-boolean)
(define-primitive op/output-port? (any->) vm-output-port? return-boolean)

(define-consing-primitive op/open-port (string-> fixnum->)
  (lambda (ignore) port-size)
  (lambda (filename mode key)
    (let loop ((index (find-port-index)) (filename filename))
      (cond ((>= index 0)
             (let* ((port
                     (cond ((= mode for-output)
                            (open-output-file (extract-string filename)))
                           (else        ;(= mode for-input)
                            (open-input-file (extract-string filename))))))
               (if (not (null-pointer? port))
                   (let ((vm-port (make-port (enter-fixnum mode)
					     (enter-fixnum index)
					     false
					     filename
					     key)))
                     (use-port-index! index vm-port port)
                     (goto return vm-port))
                   (goto return false))))
            (else
             (let ((filename (collect-saving-temp filename)))
	       (let ((index (find-port-index)))
		 (if (>= index 0)
		     (loop index filename)
		     (error "ran out of ports")))))))))

(define-primitive op/close-port (port->) close-port return-unspecific)

(define-primitive op/read-char (open-input-port->)
  (lambda (port)
    (let ((c (peeked-char port)))
      (goto return (cond ((false? c)
			  (let ((c (read-char (extract-port port))))
			    (if (eof-object? c)
				eof-object
				(enter-char c))))
			 (else
			  (set-peeked-char! port false)
			  c))))))

(define-primitive op/peek-char (open-input-port->)
  (lambda (port)
    (let ((c (peeked-char port)))
      (goto return (cond ((false? c)
			  (let* ((c (read-char (extract-port port)))
				 (c (if (eof-object? c)
					eof-object
					(enter-char c))))
			    (set-peeked-char! port c)
			    c))
			 (else c))))))

(define-primitive op/char-ready? (open-input-port->)
  (lambda (port)
    (goto return (enter-boolean (char-ready? (extract-port port))))))
    
(define-primitive op/write-char (char-> open-output-port->)
  (lambda (c port)
    (write-char c port)
    (goto return unspecific)))

(define-primitive op/write-string (string-> open-output-port->)
  (lambda (s port)
    (write-vm-string s port)
    (goto return unspecific)))

(define-primitive op/force-output (open-output-port->)
  (lambda (port)
    (force-output port)
    (goto return unspecific)))

; Misc

(define-primitive op/false ()
  (lambda ()
    (goto return false)))

(define-primitive op/trap (any->)
  (lambda (arg)
    (goto raise-exception1 0 arg)))

(define-primitive op/find-all-symbols (vector->)
  (lambda (table)
    (if (add-symbols-to-table table)
	(goto return unspecific)
	(goto raise-exception 0))))

(define-primitive op/find-all-xs (fixnum->)
  (lambda (type)
    (let ((vector (find-all-xs type)))
      (if (not (false? vector))
	  (goto return vector)
	  (goto raise-exception1 0 (enter-fixnum type))))))

; RESUME-PROC is called when the image is resumed.
; This does a garbage collection rooting from RESUME-PROC, writes the heap
; into a file, and then aborts the garbage collection (which didn't modify
; any VM registers or the stack).

(define-primitive op/write-image (string-> any-> string->)
  (lambda (filename resume-proc comment-string)
    (let ((port (open-output-file (extract-string filename))))
      (cond ((null-pointer? port)
	     (goto raise-exception2 0 filename resume-proc))
	    (else
	     (write-vm-string comment-string port)
	     (begin-collection)
	     (let ((resume-proc (trace-value resume-proc)))
	       (clean-weak-pointers)
	       (let ((size (write-image port resume-proc)))
		 (close-output-port port)
		 (abort-collection)
		 (goto return (enter-fixnum size)))))))))

(define-primitive op/collect ()
  (lambda ()
    (set! *val* unspecific)
    (interpreter-collect)
    (goto return unspecific)))

(define-primitive op/memory-status (fixnum-> any->)
  (lambda (key other)
    (cond ((= key memory-status-option/available)
	   (goto return (enter-fixnum (available))))
	  ((= key memory-status-option/heap-size)
	   (goto return
		 (enter-fixnum
		  (bytes->cells (- *newspace-end* *newspace-begin*)))))
	  ((= key memory-status-option/stack-size)
	   (goto return (enter-fixnum *stack-size*)))
	  ((= key memory-status-option/set-minimum-recovered-space!)
	   (cond ((fixnum? other)
		  (let ((old *minimum-recovered-space*))
		    (set! *minimum-recovered-space*
			  (extract-fixnum other))
		    (goto return (enter-fixnum old))))
		 (else
		  (goto raise-exception2 0 (enter-fixnum key) other))))
	  ((= key memory-status-option/gc-count)
	   (goto return (enter-fixnum *gc-count*)))
	  (else
	   (goto raise-exception2 0 (enter-fixnum key) other)))))

(define-primitive op/time (fixnum-> any->)
  (lambda (key other)
    (cond ((= key time-option/ticks-per-second)
	   (goto return (enter-fixnum (ps-ticks-per-second))))
	  ((= key time-option/run-time)
	   (goto return (enter-fixnum (ps-run-time))))
	  ((= key time-option/real-time)
	   (goto return (enter-fixnum (ps-real-time))))
	  (else
	   (goto raise-exception2 0 (enter-fixnum key) other)))))
                            
(define-primitive op/schedule-interrupt (fixnum->)
  (lambda (time)
    (clear-interrupt! interrupt/alarm)
    (goto return (enter-fixnum (ps-schedule-interrupt time)))))

(define external-> (input-type external? no-coercion))

(define-primitive op/external-lookup (external->)
  (lambda (external)
    (let ((name (external-name external))
	  (value (external-value external)))
      (if (and (vm-string? name)
	       (code-vector? value)
	       (lookup-external-name (address-after-header name)
				     (address-after-header value)))
	  (goto return unspecific)
	  (goto raise-exception1 0 external)))))

; This is only for the closed-compiled version of EXTERNAL-CALL.
;
; This is a mess because the arguments have been pushed on the stack after
; the procedure.
; The top-level driver loop removes the procedure and the arguments from
; the stack.

(define-primitive op/external-call ()
  (lambda ()
    (let ((proc (stack-ref (- *nargs* 1))))
      (cond ((not (external? proc))
	     (goto raise-exception1 0 proc))   ; lots of junk on the stack...
	    (else
	     (set! *nargs* (- *nargs* 1))      ; don't count the procedure
	     (set! *val* proc)
	     return-option/external-call)))))  ; return to driver loop

(define-primitive op/vm-extension (fixnum-> any->)
  (lambda (key value)
    (let ((return-value (extended-vm key value)))
      (if (undefined? return-value)
	  (goto raise-exception2 0 (enter-fixnum key) value)
	  (goto return return-value)))))

(define-primitive op/vm-return (fixnum-> any->)
  (lambda (key value)
    (set! *val* value)
    return-option/exit))            ; the VM returns this value

(define-primitive op/get-dynamic-state ()
  (lambda () *dynamic-state*)
  return-any)

(define-primitive op/set-dynamic-state! (any->)
  (lambda (state)
    (set! *dynamic-state* state)
    unspecific)
  return-any)

; Unnecessary primitives

(define-primitive op/string=? (string-> string->) vm-string=? return-boolean)

; Special primitive called by the reader.
; Primitive for the sake of speed.  Probably should be flushed.

(define-consing-primitive op/reverse-list->string (any-> fixnum->) 
  (lambda (n) (vm-string-size n))
  (lambda (l n k)
    (if (not (or (vm-pair? l) (vm-eq? l null)))
        (goto raise-exception2 0 l (enter-fixnum n))
        (let ((obj (vm-make-string n k)))
          (do ((l l (vm-cdr l))
               (i (- n 1) (- i 1)))
              ((< i 0) (goto return obj))
            (vm-string-set! obj i (extract-char (vm-car l))))))))

(define-primitive op/string-hash (string->) vm-string-hash return-fixnum)

(define-consing-primitive op/intern (string-> vector->)
  (lambda (ignore) (+ vm-symbol-size vm-pair-size))
  intern
  return)

;#|
;(define-primitive op/vector (fixnum->)
;  (let* ((min-args (next-byte))
;         (len (- *nargs* min-args))
;         (key (ensure-space (vector-size len)))
;         (vec (make-vector len)))
;    (do ((i (- len 1) (- i 1)))
;        ((= i -1)
;         (set! *val* l)
;         (set! *nargs* (+ min-args 1))
;         (goto interpret))
;      (vector-set vec i (pop)))))
;|#
; Eventually add make-table, table-ref, table-set! as primitives?
; No -- write a compiler instead.
