; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file arch.scm.

;;;; Architecture description

(define architecture-version "Vanilla 37")

; Things that the VM and the runtime system both need to know.

(define bits-used-per-byte 8)

(define byte-limit (expt 2 bits-used-per-byte))
(define two-byte-limit (* byte-limit byte-limit))

; Bytecodes: for compiler and interpreter

; Instruction specification is 
; (op . args)
; OP may be a name or a list of names
; ARGS are

;  byte                   - A byte used for some random purpose
;  two-bytes              - ditto, but two bytes
;  nargs                  - number of arguments on the stack
;  two-byte-nargs         - ditto, but two bytes
;  stack-index            - A one-byte index into the stack
;  two-byte-stack-index   - ditto, but two bytes
;  index                  - A index into some stob
;  two-byte-index         - ditto, but two bytes
;  literal      - a one-byte value
;  offset       - two bytes giving an offset into the current instruction stream
;  offset-      - same thing, negative offset
;  stob         - a byte specifying a type for a stored object
;  env-data     - environment specification with one-byte values
;  big-env-data - environment specification with two-byte values
;  moves-data   - specification of stack shuffle moves
;  big-moves-data - specification of stack shuffle moves
;  cont-data    - cont-data specification
;  protocol     - protocol specification
;  0 1 2 ...    - the number of non-instruction-stream arguments (some
;                 instructions take a variable number of arguments; the first
;                 number is the argument count implemented by the VM)
;  +            - any number of additional arguments are allowed

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

; Instructions marked *EXP* are experimental and are not normally used by
; byte-code compiler.

(define-instruction-set
  (protocol       protocol)      ; first opcode in a procedure, never actually
				 ; executed

  (integer-literal      literal) ; optimization for one-byte integers (also
				 ; used in the closed-compiled +, *)
  (push+integer-literal literal) ; preceded by a push
  (integer-literal+push literal) ; followed by a push

  (global      two-byte-stack-index two-byte-index)   ; first is template, second within template
  (set-global! two-byte-stack-index two-byte-index 1) ; first is template, second within template

  (make-flat-env  env-data)      ; make new environment from env-data
  (make-big-flat-env big-env-data) ; same, but with two-byte size and offsets

  ; the following four emitted from the byte-code optimizer, for the
  ; benefit of the native-code compiler
  (env-set! stack-index index 1)             ; set environment slot
  (big-env-set! two-byte-stack-index two-byte-index 1)
  (template-ref stack-index index) ; same thing as stack-indirect
  (big-template-ref two-byte-stack-index two-byte-index) ; same thing as stack-indirect

  (make-flat-closure two-bytes)  ; create flat closure

  (push 1)		         ; push *val* onto stack
  (push-false)			 ; a common combination
  (pop)			         ; pop top of stack into *val*
  (pop-n	  two-bytes)     ; remove the top N values from the stack
				 ; leaving *val* unchanged
  (push-n	  two-bytes)     ; allocate space for N values on stack
				 ; leaving *val* unchanged
  (stack-ref      stack-index)   ; index'th element of stack into *val*
  (push+stack-ref stack-index)   ; preceded by a push
  (stack-ref+push stack-index)   ; followed by a push
  (big-stack-ref  two-byte-stack-index)
  (stack-set!     stack-index 1) ; *val* to index'th element of stack
  (big-stack-set! two-byte-stack-index 1)
  (stack-indirect stack-index index)     ; first is index into stack, second is index
			  	 ; into what you find there
  (push+stack-indirect stack-index index) ; preceded by a push
  (stack-indirect+push stack-index index) ; followed by a push
  (big-stack-indirect two-byte-stack-index two-byte-index)

  (stack-shuffle! moves-data)	 ; shuffle stack elements around
  (big-stack-shuffle! big-moves-data)	 ; shuffle stack elements around with two-byte offsets

  (current-cont)	         ; copy *cont* to *val*, use WITH-CONTINUATION
			         ; to use copied continuation
  (cont-data      cont-data)	 ; continuation data; never executed

  ;; Different ways to call procedures
  (call     offset nargs 1 +)    ; last argument is the procedure to call,
				 ; offset is to return pointer
  (tail-call nargs two-bytes 1 +)   ; same, no return pointer but stack frame size, moves arguments
  (big-call offset two-byte-nargs 1 +) ; ditto, nargs counts are two bytes

  (known-tail-call nargs two-bytes 1 +)
  (known-call offset nargs 1 +) ; like CALL, but no protocol conversion
  (big-known-call offset two-byte-nargs 1 +) ; ditto, nargs count are two bytes

  (poll)
  (apply offset two-byte-nargs 2 +)   ; last argument is procedure to call, second to
				 ; last is a list of additional arguments, next
                                 ; two bytes are the number of stack arguments
  (closed-apply 2 +)		 ; arguments are as for Scheme's APPLY, with
				 ; the number of non-list arguments pushed on
  				 ; the top of the stack
  (with-continuation 2)		 ; first arg is cont, second is procedure

  ;; Three different ways to return from calls
  (return 1)			 ; return one value
  (values two-bytes +)		 ; values are on stack, count is next two bytes
  (closed-values +)		 ; values are on stack, count is pushed on stack

  ;; Six different ways to jump
  (goto-template two-byte-index)		 ; jump to another template (*EXP*)
				 ; does not poll for interrupts
  (call-template offset two-byte-stack-index two-byte-index nargs)
				 ; call a template instead of a procedure
				 ; nargs is needed for interrupt handling
  (jump-if-false offset 1)	 ; boolean in *val*


  ;; conditional jump; the offset is the jump target
  (test-op+jump-if-false offset instr instr)

  (jump          offset)
  (jump-back     offset-)	 ; same, but subtract the offset
  (computed-goto byte offset 1)	 ; jump using delta specified by *val*
				 ; defaults to instruction after deltas (*EXP*)

  ;; For the closed-compiled definitions of n-ary arithmetic functions.
  ;; The opcode sequences used are:
  ;;       binary-reduce1 binary-op binary-reduce2 return
  ;; and
  ;;       binary-reduce1 binary-op binary-comparison-reduce2 return
  ((binary-reduce1 binary-reduce2 binary-comparison-reduce2))

  ;; Scalar primitives
  (eq? 2)

  ((number? integer? rational? real? complex? exact?) 1)
  ((exact->inexact inexact->exact) 1)

  ((+ *) 2 0 1 +)
  ((- /) 2 1)
  ((= < > <= >=) 2 +)
  ((quotient remainder) 2)
  ((floor numerator denominator
     real-part imag-part
     exp log sin cos tan asin acos sqrt
     angle magnitude)
   1)
  (atan1 1)
  (atan2 2)
  ((make-polar make-rectangular) 2)
  (bitwise-not 1)
  (bit-count 1)
  ((bitwise-and bitwise-ior bitwise-xor) 2)
  (arithmetic-shift 2)
  (char? 1)
  ((char=? char<?) 2)
  ((char->scalar-value scalar-value->char scalar-value?) 1)
  (eof-object? 1)

  ;; Data manipulation
  (stored-object-has-type? stob 1)
  (stored-object-length stob 1)

  (make-stored-object byte stob)
  (closed-make-stored-object stob)	; size pushed on stack
  (stored-object-ref  stob index 1)
  (stored-object-set! stob index byte 2)	; byte = 0
  					; means not to log in the proposal

  (make-vector-object stob 2)			; size + init
  ; If the byte = 0 then do not log in the current proposal
  (stored-object-indexed-ref  stob byte 2)	; vector + offset
  (stored-object-indexed-set! stob byte 3)	; vector + offset + value

  (make-double)

  (make-byte-vector 2)
  (byte-vector-length 1)
  (byte-vector-ref 2)
  (byte-vector-set! 3)

  (make-string 2)
  (string-length 1)
  (string-ref 2)
  (string-set! 3)
  (copy-string-chars! 5)

  (intern 1)
                     
  (location-defined? 1)
  (set-location-defined?! 2)
  ((immutable? make-immutable! make-mutable!) 1)

  ;; channels (unbuffered, non-blocking I/O)
  (open-channel 4)
  (close-channel 1)
  (channel-maybe-read 5)
  (channel-maybe-write 4)
  (channel-parameter 1)
  (channel-ready? 1)
  (channel-abort 1)             ; stop channel operation
  (open-channels-list)		; return a list of the open channels

  ;; weak-pointers
  (make-weak-pointer 1)

  ;; Optimistic concurrency
  (current-proposal)
  (set-current-proposal! 1)
  (maybe-commit)
  (stored-object-logging-ref stob index 1)
  (copy-bytes! byte 5)		; byte = 0 -> don't log
  (byte-vector-logging-ref 2)
  (byte-vector-logging-set! 3)

  ;; Misc
  ((unassigned unspecific))
  (trap 1)			; raise exception
  (false)			; return #f (for bootstrapping)
  (eof-object)                  ; hard to get otherwise
  (write-image-low 4)
  (collect)
  (string-hash 1)		; used by the static linker for the initial table
  (add-finalizer! 2)
  (memory-status 2)
  (find-all 1)	                ; makes a vector of all objects of a given type
  (find-all-records 1)          ; makes a vector of all records of a given type
  (current-thread)
  (set-current-thread! 1)
  (session-data)                ; session specific data
  (set-session-data! 1)
  (set-exception-handlers! 1)
  (return-from-exception 1)
  (return-from-native-exception 1)
  (set-interrupt-handlers! 1)
  (set-enabled-interrupts! 1)
  (resume-interrupted-opcode-to-byte-code)
  (resume-interrupted-call-to-native-code)
  (resume-native-poll)
  (schedule-interrupt 1)
  (wait 2)                      ; do nothing until something happens
  (call-external-value 1 +)
  (lookup-shared-binding 2)
  (undefine-shared-binding 2)
  (find-undefined-imported-bindings)
  (time 2)
  (system-parameter 1)
  (vm-extension 2)		; access to extensions of the virtual machine
  (return-from-callback 2)	; return from an callback
  (op-with-cell-literal byte byte byte byte ;word-literal
                       byte +)
  
  ;; Unnecessary primitives
  (string=? 2)
  (reverse-list->string 2)
  (assq 2)
  (unassigned-check 1)
  ; If the byte = 0 then do not log in the current proposal
  (checked-record-ref index 3)
  (checked-record-set! index 4)

  (encode-char 5)
  (encode-char! 5)

  (decode-char 4)
  (decode-char! 4)

  ;; ports (buffered I/O) - these are all unnecessary
  ;; byte = 0 -> port is supplied
  ;;      = 1 -> get port from dynamic environment
  ((read-byte peek-byte) byte 1 0)
  (write-byte byte 2 1)

  ((read-char peek-char) byte 1 0)
  (write-char byte 2 1)

  (os-error-message 1)

  ;; For writing informative messages when debugging
  (message 1)
  )

(define-enumeration interrupt
  (alarm           ; order matters - higher priority first
   keyboard

   ;; "Major" means the collector made a maximal effort to reclaim
   ;; memory; everything else is "minor".
   post-minor-gc post-major-gc ; handler is passed a list of finalizers
   i/o-completion  ; handler is passed channel, error flag and status
   os-signal
   external-event  ; handler is passed event type uid
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
   buffer-full/empty
   unimplemented-instruction
   trap
   proceeding-after-exception
   bad-option
   unbound-external-name
   too-many-arguments-to-external-procedure
   too-many-arguments-in-callback
   callback-return-uncovered
   extension-exception
   extension-return-error
   os-error
   gc-protection-mismatch
   no-current-proposal
   native-code-not-supported
   illegal-exception-return
   ))

; Used by (READ-BYTE) and (WRITE-BYTE) to get the appropriate ports from
; the fluid environment.

(define-enumeration current-port-marker
  (current-input-port
   current-output-port))

;----------------
; Call and return protocols
;
; Protocols 0..maximum-stack-args are just the number of arguments sitting on
; the stack.

(define maximum-stack-args 63)

(define *last-protocol* maximum-stack-args)

(define (next-protocol)
  (set! *last-protocol* (+ *last-protocol* 1))
  *last-protocol*)

; The next two bytes gives the expected number of arguments.

(define two-byte-nargs-protocol      (next-protocol))

; Used for all n-ary procedures.  The next two bytes gives the minimum
; number of arguments.

(define two-byte-nargs+list-protocol (next-protocol))

; Drop any and all arguments on the floor.  The compiler doesn't use this
; for procedures, but does generate it for continuations.

(define ignore-values-protocol (next-protocol))

; Real protocol is at the end of the code vector, along with the required
; stack size:
;   ... real-protocol stack-size0 stack-size1
; This stuff has to be at the end of the code vector because the necessary stack
; size is not determined until after the code vector has been assembled.

(define big-stack-protocol (next-protocol))

; The rest are used only for the definitions of various Scheme primitives.

; For VECTOR, RECORD, VALUES, EXTERNAL-CALL, APPLY
; Next byte is the minimum number of arguments (1 for EXT-CALL, 2 for APPLY,
; 0 for the rest).
; Stack = arg0 arg1 ... argN rest-list N+1 total-arg-count
; The first two arguments are always on the stack.

(define args+nargs-protocol (next-protocol))

; Followed by four bytes: the offsets of code for the 3+, 0, 1, and 2 arg cases.
; A zero indicates that the primitive doesn't accept that number of arguments.
; If there are fewer than three arguments they are all on the stack.  In the
; 3+ case the setup is the same as args+nargs above (it's first so that it can
; share code in the VM with args+nargs).

(define nary-dispatch-protocol (next-protocol))

; The following is used to mark continuations created for the first argument
; to CALL-WITH-VALUES when the second argument is not a LAMBDA expression.
; The continuation contains a single value, the procedure to be called on the
; values returned by the first argument.

(define call-with-values-protocol (next-protocol))

; Used to mark the continuation at the bottom of the stack cash.

(define bottom-of-stack-protocol (next-protocol))

; Native protocols are the same, except with the high bit set.

(define native-protocol-mask #x80)

;----------------
; The number of stack slots available to each procedure by default.
; Procedures that need more than this must use one of the two-byte-nargs
; protocols.  All of these are given in terms of descriptors.

(define default-stack-space 64)

; The maximum number of arguments that can be passed to EXTERNAL-CALL.
; This is determined by the C procedure `external_call()'.

; This leaves space for the non-argument stuff on the stack---see
; external-call.scm.  Note that Mike isn't quite sure this is right.
(define maximum-external-call-args (- default-stack-space 5))

(define continuation-stack-size 4)  ; header + continuation + pc + code

(define available-stack-space 8000) ; how much stack space is available for
                                    ; any one procedure

; The number of values that the VM adds to continuations.
(define continuation-cells 3)

; Offsets of saved registers in continuations
(define continuation-pc-index   0)
(define continuation-code-index 1)
(define continuation-cont-index 2)

; Offsets in the CONT-DATA instruction
					; -1 -2        frame size
(define gc-mask-size-offset -3)		; -3           gc mask size
					; -4 -5        offset
                                        ; -6 -7        template
(define gc-mask-offset      -8)         ; -8 ...       mask (low bytes first)

; The number of additional values that the VM adds to exception continuations.
(define exception-continuation-cells 5)

; The number of additional values that the VM adds to native exception continuations.
(define native-exception-continuation-cells 4)

; Offsets of saved registers in exception continuations.  Size must come
; first because the VM expects it there.
(define exception-cont-size-index             (+ continuation-cells 0))
(define exception-cont-pc-index               (+ continuation-cells 1))
(define exception-cont-code-index             (+ continuation-cells 2))
(define exception-cont-exception-index        (+ continuation-cells 3))
(define exception-cont-instruction-size-index (+ continuation-cells 4))

(define native-exception-cont-size-index      (+ continuation-cells 0))
(define native-exception-cont-exception-index (+ continuation-cells 1))
(define native-exception-cont-bc-pc-index     (+ continuation-cells 2))
(define native-exception-cont-bc-code-index   (+ continuation-cells 3))

;----------------

; Options for op/time

(define-enumeration time-option
  (run-time
   real-time
   cheap-time     ; cheap (no system call) access to the polling clock
   gc-run-time
   ;current-time
   ))

; Options for op/memory-status

(define-enumeration memory-status-option
  (available
   heap-size
   max-heap-size
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

; Parameters that configure a channel

(define-enumeration channel-parameter-option
  (buffer-size crlf?))

; Built-in text encodings

(define-enumeration text-encoding-option
  (us-ascii
   latin-1
   utf-8
   utf-16le utf-16be
   utf-32le utf-32be))

; Options for op/system-parameter

(define-enumeration system-parameter-option
  (host-architecture
   os-string-encoding))

(define-enumeration stob
  (;; D-vector types (traced by GC)
   pair
   symbol
   vector
   closure
   location
   cell
   channel
   port
   ratnum
   record
   continuation
   extended-number
   template
   weak-pointer
   shared-binding
   unused-d-header1

   ;; B-vector types (not traced by GC)
   string        ; = least b-vector type
   byte-vector
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
    (symbol symbol? #f       ; RTS calls op/intern/string->symbol
      (symbol->string))
    (location location? make-location
      (location-id set-location-id!)
      (contents set-contents!))
    (cell cell? make-cell
      (cell-ref cell-set!))
    (closure closure? make-closure
      (closure-template set-closure-template!) (closure-env set-closure-env!))
    (weak-pointer weak-pointer? #f ; make-weak-pointer is an op
      (weak-pointer-ref))
    (shared-binding shared-binding? make-shared-binding
      (shared-binding-name)
      (shared-binding-is-import?)
      (shared-binding-ref shared-binding-set!))
    (port port? make-port
      (port-handler)
      ;; either an integer from the TEXT-ENCODING-OPTION for encodings
      ;; handled by the VM, or a :TEXT-CODEC object for things handled
      ;; purely by the RTS
      (port-text-codec-spec set-port-text-codec-spec!)
      (port-crlf?   set-port-crlf?!)
      (port-status  set-port-status!)
      (port-lock    set-port-lock!)		; used for buffer timestamps
      (port-data    set-port-data!)
      (port-buffer  set-port-buffer!)
      (port-index   set-port-index!)
      (port-limit   set-port-limit!)
      ;; for CR/LF handling
      (port-pending-cr?  set-port-pending-cr?!)
      (port-pending-eof? set-port-pending-eof?!))
    (channel channel? #f
      (channel-status)
      (channel-id)
      (channel-os-index)
      (channel-close-silently?))
    ))

