; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

; This is file arch.scm.

;;;; Architecture description

(define architecture-version "Vanilla 18")

; Things that the VM and the runtime system both need to know.

(define bits-used-per-byte 8)

; Maximum number of arguments that can be pushed on the stack to make a call,
; also the maximum number of arguments + temporaries allowed on the stack.
(define maximum-stack-args 63)

; Bytecodes: for compiler and interpreter

; Instruction specification is 
; (op . args)
; OP may be a name or a list of names
; ARGS are
;  nargs     - a byte
;  byte      - a byte
;  index     - a byte indexing into the current template
;  offset    - two bytes giving an offset into the current instruction stream
;  stob      - a byte specifying a type for a stored object
;  0 1 2 ... - the number of non-instruction-stream arguments (some
;              instructions take a variable number of arguments; the first
;              number is the argument count implemented by the VM)
;  +         - any number of additional arguments are allowed

(define-syntax define-instruction-set
  (lambda (form rename compare)
    (let ((data (do ((data (reverse (cdr form)) (cdr data))
		     (new '() (let ((next (car data)))
				(if (pair? (car next))
				    (append (map (lambda (op)
						   (cons op (cdr next)))
						 (car next))
					    new)
				    (cons next new)))))
		    ((null? data) new))))
      `(begin (define-enumeration op
		,(map car data))
	      (define opcode-arg-specs
		'#(,@(map cdr data)))))))

(define-instruction-set
  (check-nargs=   nargs)         ; error if *nargs* not= operand
  (check-nargs>=  nargs)         ; error if *nargs* < operand
  (nargs)		         ; move *nargs* to *val*
  (make-env       nargs)         ; cons an environment
  (make-heap-env  nargs)         ; cons an environment in the heap
  (pop-env)		         ; use superior env
  (make-rest-list nargs +)       ; pop all but nargs things off the stack
			         ; into a list
  (literal        index)         ; value to *val*
  (local          byte byte)     ; back and over
  ((local0 local1 local2)
   byte)		         ; back encoded into op-code for efficiency
  (set-local!     byte byte 1)   ; back over value
  (global         index)         ; value to *val*
  (set-global!    index 1)
  (closure        index)         ; use environment in *env*
  (flat-closure   byte var-data index) ; make new environment from var-data
  (push 1)		         ; push *val* onto stack
  (pop)			         ; pop top of stack into *val*
  (stack-ref      byte)	         ; index'th element of stack into *val*
  (stack-set!     byte 1)        ; *val* to index'th element of stack

  (make-cont      offset nargs)  ; save state in *cont*
  (current-cont)	         ; copy *cont* to *val*, use WITH-CONTINUATION
			         ; to use copied continuation
  (get-cont-from-heap)	         ; copy next continuation from heap (this
			         ; op-code is used internally by the VM)

  ;; five different ways to call procedures
  (call               nargs 1 +) ; last argument is the procedure to call
  (move-args-and-call nargs 1 +) ; same, move args to just above *cont* first
  (apply              nargs 1 +) ; last argument is a list of additional 
				 ; arguments, second to last is procedure to
				 ; call
  (with-continuation          2) ; first arg is cont, second is procedure
  (call-with-values           +) ; values are on stack, consumer is in the
				 ; continuation pointed to by *cont*

  ;; Three different ways to return from calls and one way to ignore any
  ;; returned values
  (return 1)			 ; return to continuation in *cont*
  (values +)			 ; values are on stack, count is in *nargs*
  (return-values nargs +)	 ; values are on stack, count is next byte
  (ignore-values +)		 ; ignore (and dispose of) returned values

  ;; Five different ways to jump
  (goto-template        index)	 ; jump to another template
				 ; does not poll for interrupts
  (call-template  nargs index)	 ; call a template instead of a procedure
				 ; nargs is needed for interrupt handling
  (jump-if-false  offset 1)	 ; boolean in *val*
  (jump           offset)
  (computed-goto  byte offset 1) ; jump using delta specified by *val*
				 ; default to instruction after deltas


  ;; Scalar primitives
  (eq? 2)

  ((number? integer? rational? real? complex? exact?) 1)
  ((exact->inexact inexact->exact) 1)

  ((+ *) 2 0 1 +)
  ((- /) 2 1)
  ((= <) 2 +)
  ((quotient remainder) 2)
  ((floor numerator denominator
     real-part imag-part
     exp log sin cos tan asin acos sqrt
     angle magnitude)
   1)
  (atan 2)
  ((make-polar make-rectangular) 2)
  (bitwise-not 1)
  ((bitwise-and bitwise-ior bitwise-xor) 2)
  (arithmetic-shift 2)
  (char? 1)
  ((char=? char<?) 2)
  ((char->ascii ascii->char) 1)
  (eof-object? 1)

  ;; Data manipulation
  (stored-object-has-type? stob 1)
  (stored-object-length stob 1)

  (make-stored-object byte stob)
  (closed-make-stored-object stob) ; size is *nargs*
  (stored-object-ref  stob byte 1) ; byte is the offset
  (stored-object-set! stob byte 2)

  (make-vector-object stob 2)	      ; size + init
  (stored-object-indexed-ref  stob 2) ; vector + offset
  (stored-object-indexed-set! stob 3) ; vector + offset + value

  (make-code-vector 2)
  (code-vector-length 1)
  (code-vector-ref 2)
  (code-vector-set! 3)

  (make-string 2)
  (string-length 1)
  (string-ref 2)
  (string-set! 3)
                     
  (location-defined? 1)
  (set-location-defined?! 2)
  ((immutable? make-immutable!) 1)

  ;; channels (unbuffered, non-blocking I/O)
  (open-channel 2)
  (close-channel 1)
  (channel-maybe-read 5)
  (channel-maybe-write 4)
  (channel-abort 1)             ; stop channel operation
  
  ;; Misc
  ((unassigned unspecific))
  (trap 1)			; raise exception
  (false)			; return #f (for bootstrapping)
  (eof-object)                  ; hard to get otherwise
  (write-image 3)
  (collect)
  (add-finalizer! 2)
  (memory-status 2)
  (find-all 1)	                ; makes a vector of all objects of a given type
  (find-all-symbols 1)		; puts the symbols in a table
  (find-all-records 1)          ; makes a vector of all records of a given type
  (current-thread)
  (set-current-thread! 1)
  (session-data)                ; session specific data
  (set-session-data! 1)
  (set-exception-handlers! 1)
  (set-interrupt-handlers! 1)
  (set-enabled-interrupts! 1)
  (return-from-interrupt)
  (schedule-interrupt 1)
  (wait 2)                      ; do nothing until something happens
  (external-lookup 1)
  (external-call 1 +)
  (time 2)
  (vm-extension 2)		; access to extensions of the virtual machine
  (vm-return 2)			; return from the vm in a restartable fashion

  ;; Unnecessary primitives
  (string=? 2)
  (string-hash 1)
  (reverse-list->string 2)
  (intern 2)
  (assq 2)
  (checked-record-ref 3)
  (checked-record-set! 4)
  (copy-bytes! 5)

  ;; ports (buffered I/O) - these are all unnecessary
  ((read-char peek-char) 1 0)
  (write-char 2 1)
  )

(define-enumeration interrupt
  (alarm           ; order matters - higher priority first
   keyboard
   post-gc         ; handler is passed a list of finalizers
   exit
   i/o-completion  ; handler is passed channel and status
   ))

; Possible problems

(define-enumeration exception
  (unassigned-local
   undefined-global
   unbound-global
   bad-procedure
   wrong-number-of-arguments
   wrong-type-argument
   arithmetic-overflow
   index-out-of-range
   heap-overflow
   out-of-memory
   cannot-open-channel
   channel-os-index-already-in-use
   closed-channel
   pending-channel-i/o
   buffer-full/empty
   unimplemented-instruction
   trap
   bad-option
   unbound-external-name
   extension-exception
   extension-return-error
   os-error
   ))

; Options for op/time

(define-enumeration time-option
  (run-time
   real-time
   cheap-time     ; cheap (no system call) access to the polling clock
   ;current-time
   ))

; Options for op/memory-status

(define-enumeration memory-status-option
  (available
   heap-size
   stack-size
   gc-count
   expand-heap!
   pointer-hash
   ))

; The two types of special channels cannot be used for normal I/O.

(define-enumeration channel-status-option
  (closed
   input
   output
   special-input   ; socket accept, ???
   special-output  ; ???
   ))

; Indicies into a port's status word

(define-enumeration port-status-options
  (input
   output
   open-for-input
   open-for-output
   ))

(define-enumeration stob
  (;; D-vector types (traced by GC)
   pair
   symbol
   vector
   closure
   location
   channel
   port
   ratnum
   record
   continuation
   extended-number
   template
   weak-pointer
   external
   unused-d-header1
   unused-d-header2

   ;; B-vector types (not traced by GC)
   string        ; = least b-vector type
   code-vector
   double        ; double precision floating point
   bignum
   ))

; This is here to try to ensure that it is changed when STOB changes.
(define least-b-vector-type (enum stob string))

; (stob predicate constructor . (accessor modifier)*)
; If nothing else, the run-time system and the VM need to agree on
; which slot of a pair is the car and which is the cdr.

(define stob-data
  '((pair pair? cons
      (car set-car!) (cdr set-cdr!))
    (symbol symbol? #f       ; symbols actually made using op/intern
      (symbol->string))
    (location location? make-location
      (location-id set-location-id!)
      (contents set-contents!))
    (closure closure? make-closure
      (closure-template) (closure-env))
    (weak-pointer weak-pointer? make-weak-pointer
      (weak-pointer-ref))
    (external external? make-external
      (external-name) (external-value))
    (port port? make-port
      (port-handler)
      (port-status  set-port-status!)
      (port-lock    set-port-lock!)
      (port-locked? set-port-locked?!)
      (port-data    set-port-data!)
      (port-buffer  set-port-buffer!)
      (port-index   set-port-index!)
      (port-limit   set-port-limit!)
      (port-pending-eof? set-port-pending-eof?!))
    (channel channel? #f
      (channel-status)
      (channel-id)
      (channel-os-index))
    ))

