; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

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
	(extended-number? x)))
  return-boolean)

(define-syntax define-numeric-predicate
  (syntax-rules ()
    ((define-numeric-predicate op)
     (define-primitive op
       (any->)
       (lambda (n)
	 (cond ((fixnum? n)
		(goto return (enter-boolean #t)))
	       ((extended-number? n)
		(goto raise-exception1 0 n))
	       (else
		(goto return (enter-boolean #f)))))))))

(define-numeric-predicate integer?)
(define-numeric-predicate rational?)
(define-numeric-predicate real?)
(define-numeric-predicate complex?)

; These primitives have a simple answer in the case of fixnums; for all other
; they punt to the run-time system.

(define-primitive exact?      (number->) (lambda (n) #t) return-boolean)
(define-primitive real-part   (number->) (lambda (n) (goto return n)))
(define-primitive imag-part   (number->) (lambda (n)
					      (goto return (enter-fixnum 0))))
(define-primitive floor       (number->) (lambda (n) (goto return n)))
(define-primitive numerator   (number->) (lambda (n) (goto return n)))
(define-primitive denominator (number->) (lambda (n)
					      (goto return (enter-fixnum 1))))
(define-primitive angle       (number->) (lambda (n)
					      (if (>= n 0)
						  (goto return (enter-fixnum 0))
						  (goto raise-exception1 0 n))))

; beware of (abs least-fixnum)
(define-primitive magnitude   (number->)
  (lambda (n)
    (abs-carefully n
		   return
		   (lambda (n)
		     (goto raise-exception1 0 n)))))

; These primitives all just raise an exception and let the run-time system do
; the work.

(define-syntax define-punt-primitive
  (syntax-rules ()
    ((define-punt-primitive op)
     (define-primitive op (number->)
       (lambda (n) (goto raise-exception1 0 n))))))

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
       (lambda (n m) (goto raise-exception2 0 n m))))))

(define-punt2-primitive atan)
(define-punt2-primitive make-polar)
(define-punt2-primitive make-rectangular)

(define (arithmetic-overflow x y)
  (goto raise-exception2 0 x y))

(define (arith op)
  (lambda (x y)
    (goto op x y return arithmetic-overflow)))

(define-primitive +         (number->  number->) (arith add-carefully))
(define-primitive -         (number->  number->) (arith subtract-carefully))
(define-primitive *         (number->  number->) (arith multiply-carefully))
(define-primitive /         (number->  number->) (arith divide-carefully))
(define-primitive quotient  (integer-> integer->) (arith quotient-carefully))
(define-primitive remainder (integer-> integer->) (arith remainder-carefully))
(define-primitive =         (number->  number->) vm-= return-boolean)
(define-primitive <         (real->    real->)   vm-< return-boolean)
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
        (goto raise-exception1 0 (enter-fixnum x))
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
    (stob-of-type? x (next-byte)))
  return-boolean)

(define-primitive stored-object-length
  (any->)
  (lambda (stob)
    (let ((type (next-byte)))
      (if (stob-of-type? stob type)
	  (goto return-fixnum (d-vector-length stob))
	  (goto raise-exception2 1 stob (enter-fixnum type))))))
  
; Fixed sized objects

(define-primitive stored-object-ref
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

(define-primitive stored-object-set!
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

(define-primitive stored-object-indexed-ref
  (any-> fixnum->)
  (lambda (stob index)
    (let ((type (next-byte)))
      (if (and (stob-of-type? stob type)
	       (valid-index? index (d-vector-length stob)))
	  (goto return (d-vector-ref stob index))
	  (goto raise-exception3 1
		stob (enter-fixnum type) (enter-fixnum index))))))

(define-primitive stored-object-indexed-set! (any-> fixnum-> any->)
  (lambda (stob index value)
    (let ((type (next-byte)))
      (cond ((and (stob-of-type? stob type)
		  (valid-index? index (d-vector-length stob))
		  (not (immutable? stob)))
	     (d-vector-set! stob index value)
	     (goto return unspecific))
	    (else
	     (goto raise-exception4 1
		   stob
		   (enter-fixnum type)
		   (enter-fixnum index)
		   value))))))

; Code-vectors

(define-primitive code-vector-length
  (code-vector->)
  (lambda (code-vector)
    (goto return-fixnum (code-vector-length code-vector))))

(define-primitive code-vector-ref
  (code-vector-> fixnum->)
  (lambda (code-vector index)
    (if (valid-index? index (code-vector-length code-vector))
	(goto return-fixnum (code-vector-ref code-vector index))
	(goto raise-exception2 0 code-vector (enter-fixnum index)))))

(define-primitive code-vector-set!
  (code-vector-> fixnum-> fixnum->)
  (lambda (code-vector index value)
    (cond ((valid-index? index (code-vector-length code-vector))
	   (code-vector-set! code-vector index value)
	   (goto return unspecific))
	  (else
	   (goto raise-exception3 0
		 code-vector
		 (enter-fixnum index)
		 (enter-fixnum value))))))

(define-primitive make-code-vector
  (fixnum-> fixnum->)
  (lambda (len init)
    (let ((lose (lambda ()
		  (goto raise-exception2 0 (enter-fixnum len) (enter-fixnum init))))
	  (size (code-vector-size len)))
      (if (or (< len 0)
	      (> size max-stob-size-in-cells))
	  (lose)
	  (maybe-ensure-space-saving-temp
	   size
	   (enter-fixnum 0)
	   (lambda (okay? key ignore)
	     (if (not okay?)
		 (lose)
		 (let ((code-vector (make-code-vector len key)))
		   (do ((i (- len 1) (- i 1)))
		       ((< i 0))
		     (code-vector-set! code-vector i init))
		   (goto return code-vector)))))))))

; Strings

(define-primitive string-length
  (string->)
  (lambda (string)
    (goto return-fixnum (vm-string-length string))))

(define-primitive string-ref
  (string-> fixnum->)
  (lambda (string index)
    (if (valid-index? index (vm-string-length string))
	(goto return-char (vm-string-ref string index))
	(goto raise-exception2 0 string (enter-fixnum index)))))

(define-primitive string-set!
  (string-> fixnum-> char->)
  (lambda (string index char)
    (cond ((valid-index? index (vm-string-length string))
	   (vm-string-set! string index char)
	   (goto return unspecific))
	  (else
	   (goto raise-exception3 0
		 string
		 (enter-fixnum index)
		 (enter-char char))))))

(define-primitive make-string
  (fixnum-> char->)
  (lambda (len init)
    (let ((lose (lambda ()
		  (goto raise-exception2 0 (enter-fixnum len) (enter-char init))))
	  (size (vm-string-size len)))
      (if (or (< len 0)
	      (> size max-stob-size-in-cells))
	  (lose)
	  (maybe-ensure-space-saving-temp
	   size
	   (enter-fixnum 0)
	   (lambda (okay? key ignore)
	     (if (not okay?)
		 (lose)
		 (let ((string (vm-make-string len key)))
		   (do ((i (- len 1) (- i 1)))
		       ((< i 0))
		     (vm-string-set! string i init))
		   (goto return string)))))))))

; Constructors

(define-primitive make-stored-object
  ()
  (lambda ()
    (let* ((key (ensure-space (cells->bytes (+ 1 (this-byte)))))
	   (len (next-byte))  ; can't consume this byte until after ENSURE-SPACE
	   (new (make-d-vector (next-byte) len key)))
      (cond ((>= len 1)
	     (d-vector-set! new (- len 1) *val*)
	     (do ((i (- len 2) (- i 1)))
		 ((> 0 i)
		  (unassigned))  ; for the type checker!
	       (d-vector-set! new i (pop)))))
      new))
  return)

(define-primitive make-vector-object (fixnum-> any->)
  (lambda (len init)
    (let* ((type (next-byte))
	   (lose (lambda (init)
		   (goto raise-exception3 1 (enter-fixnum type)
			 (enter-fixnum len) init)))
	   (size (vm-vector-size len)))
      (if (or (< len 0)
	      (> size max-stob-size-in-cells))
	  (lose init)
	  (maybe-ensure-space-saving-temp size init
            (lambda (okay? key init)
	      (if (not okay?)
		  (lose init)
		  (let ((v (make-d-vector type len key)))
		    (do ((i (- len 1) (- i 1)))
			((< i 0))
		      (d-vector-set! v i init))
		    (goto return v)))))))))

(define-primitive location-defined? (location->)
  (lambda (loc)
    (return-boolean (or (not (undefined? (contents loc)))
			(= (contents loc) unassigned-marker)))))

(define-primitive set-location-defined?! (location-> boolean->)
  (lambda (loc value)
    (cond ((not value)
	   (set-contents! loc unbound-marker))
	  ((undefined? (contents loc))
	   (set-contents! loc unassigned-marker)))
    (goto return unspecific)))

(define-primitive immutable? (any->) immutable? return-boolean)

(define-primitive make-immutable! (any->)
  (lambda (thing)
    (make-immutable! thing)
    (goto return thing)))


; I/O primitives

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

(define-primitive input-port? (any->) vm-input-port? return-boolean)
(define-primitive output-port? (any->) vm-output-port? return-boolean)

(define-consing-primitive open-port (string-> fixnum->)
  (lambda (ignore) port-size)
  (lambda (filename mode key)
    (open-port filename mode key
	       collect-saving-temp       ; a GC may be needed to free up a port
	       (lambda (vm-port)         ; all is okay
		 (goto return vm-port))
	       (lambda (filename)        ; OS couldn't open filename
		 (goto return false))
	       (lambda (filename)        ; VM is out of port descriptors
		 (goto raise-exception2 0 filename (enter-fixnum key))))))

(define-primitive close-port (port->) close-port return-unspecific)

(define-primitive read-char (open-input-port->)
  (lambda (port)
    (let ((c (peeked-char port)))
      (goto return (cond ((false? c)
			  (ps-read-char (extract-port port)
					enter-char
					(lambda () eof-object)))
			 (else
			  (set-peeked-char! port false)
			  c))))))

(define-primitive peek-char (open-input-port->)
  (lambda (port)
    (let ((c (peeked-char port)))
      (goto return (cond ((false? c)
			  (let ((c (ps-read-char (extract-port port)
						 enter-char
						 (lambda () eof-object))))
			    (set-peeked-char! port c)
			    c))
			 (else c))))))

(define-primitive char-ready? (open-input-port->)
  (lambda (port)
    (goto return (enter-boolean (or (not (false? (peeked-char port)))
				    (char-ready? (extract-port port)))))))
    
(define-primitive write-char (char-> open-output-port->)
  (lambda (c port)
    (write-char c port)
    (goto return unspecific)))

(define-primitive write-string (string-> open-output-port->)
  (lambda (s port)
    (write-vm-string s port)
    (goto return unspecific)))

(define-primitive force-output (open-output-port->)
  (lambda (port)
    (force-output port)
    (goto return unspecific)))

; Misc

(define-primitive false ()
  (lambda ()
    (goto return false)))

(define-primitive trap (any->)
  (lambda (arg)
    (goto raise-exception1 0 arg)))

(define-primitive find-all-symbols (vector->)
  (lambda (table)
    (if (walk-over-symbols
	 (lambda (symbol)
	   (if (available? vm-pair-size)
	       (let ((key (preallocate-space vm-pair-size)))
		 (add-to-symbol-table symbol table key)
		 #t)
	       #f)))
	(goto return unspecific)
	(goto raise-exception 0))))

(define-primitive find-all-xs (fixnum->)
  (lambda (type)
    (let ((vector (find-all-xs type)))
      (if (not (false? vector))
	  (goto return vector)
	  (goto raise-exception1 0 (enter-fixnum type))))))

; RESUME-PROC is called when the image is resumed.
; This does a garbage collection rooting from RESUME-PROC, writes the heap
; into a file, and then aborts the garbage collection (which didn't modify
; any VM registers or the stack).

(define-primitive write-image (string-> any-> string->)
  (lambda (filename resume-proc comment-string)
    (if (not (image-writing-okay?))
	(goto raise-exception2 0 filename resume-proc)
	(let ((port (open-output-file (extract-string filename))))
	  (cond ((null-port? port)
		 (goto raise-exception2 0 filename resume-proc))
		(else
		 (write-vm-string comment-string port)
		 (begin-collection)
		 (let ((resume-proc (trace-value resume-proc)))
		   (do-gc)
		   (close-untraced-ports!)
		   (let ((size (write-image port resume-proc)))
		     (close-output-port port)
		     (abort-collection)
		     (goto return (enter-fixnum size))))))))))

(define-primitive collect ()
  (lambda ()
    (set! *val* unspecific)
    (collect)
    (goto return unspecific)))

(define-primitive memory-status (fixnum-> any->)
  (lambda (key other)
    (cond ((= key (enum memory-status-option available))
	   (goto return (enter-fixnum (available))))
	  ((= key (enum memory-status-option heap-size))
	   (goto return
		 (enter-fixnum
		  (bytes->cells (heap-size)))))
	  ((= key (enum memory-status-option stack-size))
	   (goto return (enter-fixnum (stack-size))))
	  ((= key (enum memory-status-option set-minimum-recovered-space!))
	   (cond ((fixnum? other)
		  (let ((old *minimum-recovered-space*))
		    (set! *minimum-recovered-space*
			  (extract-fixnum other))
		    (goto return (enter-fixnum old))))
		 (else
		  (goto raise-exception2 0 (enter-fixnum key) other))))
	  ((= key (enum memory-status-option gc-count))
	   (goto return (enter-fixnum (gc-count))))
	  (else
	   (goto raise-exception2 0 (enter-fixnum key) other)))))

(define-primitive time (fixnum-> any->)
  (lambda (key other)
    (cond ((= key (enum time-option ticks-per-second))
	   (goto return (enter-fixnum (ps-ticks-per-second))))
	  ((= key (enum time-option run-time))
	   (goto return (enter-fixnum (ps-run-time))))
	  ((= key (enum time-option real-time))
	   (goto return (enter-fixnum (ps-real-time))))
	  (else
	   (goto raise-exception2 0 (enter-fixnum key) other)))))
                            
(define-primitive schedule-interrupt (fixnum->)
  (lambda (time)
    (clear-interrupt! (enum interrupt alarm))
    (goto return (enter-fixnum (ps-schedule-interrupt time)))))

(define external-> (input-type external? no-coercion))

(define-primitive external-lookup (external->)
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

(define-primitive external-call ()
  (lambda ()
    (let ((proc (stack-ref (- *nargs* 1))))
      (cond ((not (external? proc))
	     (goto raise-exception1 0 proc))   ; lots of junk on the stack...
	    (else
	     (set! *nargs* (- *nargs* 1))      ; don't count the procedure
	     (set! *val* proc)
	     (enum return-option external-call))))))  ; return to driver loop

(define-primitive vm-extension (fixnum-> any->)
  (lambda (key value)
    (let ((return-value (extended-vm key value)))
      (if (undefined? return-value)
	  (goto raise-exception2 0 (enter-fixnum key) value)
	  (goto return return-value)))))

(define-primitive vm-return (fixnum-> any->)
  (lambda (key value)
    (set! *val* value)
    (enum return-option exit)))            ; the VM returns this value

(define-primitive get-dynamic-state ()
  (lambda () *dynamic-state*)
  return-any)

(define-primitive set-dynamic-state! (any->)
  (lambda (state)
    (set! *dynamic-state* state)
    unspecific)
  return-any)

; Unnecessary primitives

(define-primitive string=? (string-> string->) vm-string=? return-boolean)

; Special primitive called by the reader.
; Primitive for the sake of speed.  Probably should be flushed.

(define-consing-primitive reverse-list->string (any-> fixnum->) 
  (lambda (n) (vm-string-size n))
  (lambda (l n k)
    (if (not (or (vm-pair? l) (vm-eq? l null)))
        (goto raise-exception2 0 l (enter-fixnum n))
        (let ((obj (vm-make-string n k)))
          (do ((l l (vm-cdr l))
               (i (- n 1) (- i 1)))
              ((< i 0) (goto return obj))
            (vm-string-set! obj i (extract-char (vm-car l))))))))

(define-primitive string-hash (string->) vm-string-hash return-fixnum)

(define-consing-primitive intern (string-> vector->)
  (lambda (ignore) (+ vm-symbol-size vm-pair-size))
  intern
  return)

;#|
;(define-primitive vector (fixnum->)
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
