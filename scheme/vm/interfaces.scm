; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-interface vm-utilities-interface
  (export adjoin-bits low-bits high-bits unsigned-high-bits
	  assert
	  vector+length-fill!
	  natural-for-each natural-for-each-while

	  error?
	  write-error-string write-error-integer write-error-newline
	  error-message
	  ))

; Memory

(define-interface memory-interface
  (export create-memory memory-begin memory-size

	  address+ address- address1+ address2+
	  address-difference
	  address= address< address<= address> address>=
	  address->integer integer->address

	  null-address null-address?

	  fetch fetch-byte fetch-flonum
	  store! store-byte! store-flonum!

	  fetch-string fetch-nul-terminated-string

	  copy-memory! memory-equal?    ; from ps-memory
	  read-block write-block        ; from ps-memory

	  address->stob-descriptor
	  address-at-header
	  address-after-header
	  stob-header stob-header-set!
	  stob-overhead
	  ))

; Low-level data structures

(define-interface fixnum-arithmetic-interface
  (export add-carefully subtract-carefully
	  multiply-carefully divide-carefully
	  quotient-carefully remainder-carefully
	  shift-carefully
	  abs-carefully
	  fixnum-bit-count
	  ))

(define-interface stob-interface
  (export make-d-vector make-b-vector
	  maybe-make-d-vector+gc maybe-make-b-vector+gc
          d-vector-length d-vector-ref d-vector-init! d-vector-set!
	  b-vector-length b-vector-ref b-vector-set!
	  max-stob-size-in-cells

	  ensure-space
	  checking-preallocation?

	  stob-of-type?
	  header+contents->stob
	  valid-index?
	  	
	  immutable? make-immutable! make-mutable!
	  ))

(define-interface allocation-interface
  (export s48-make-available+gc
	  s48-allocate-small
	  s48-allocate-traced+gc
	  s48-allocate-untraced+gc
	  s48-allocate-stob
          ))

(define-interface heap-interface
  (export s48-available
	  s48-heap-size

	  s48-gather-objects

	  s48-find-all
	  s48-find-all-records

	  s48-write-barrier

	  ; for write-image
	  s48-heap-pointer
	  s48-oldspace-begin
	  s48-oldspace-end
	  s48-heap-begin

	  s48-*hp*	; to keep these from being `static' in C, as
	  s48-*limit*	; references are introduced elsewhere via a C macro

	  ; for debugging
	  s48-check-heap
	  ))

(define heap-gc-util-interface
  (export swap-spaces
	  heap-limit
	  set-heap-pointer!
	  heap-pointer
	  walk-impure-areas
	  allocate
	  bytes-available?))

(define-interface gc-roots-interface
  (export add-gc-root!
	  add-post-gc-cleanup!
	  ))

(define-interface interpreter-gc-interface
  (export s48-gc-root
	  s48-post-gc-cleanup
	  ))

(define-interface gc-util-interface
  (export save-temp0!
	  recover-temp0!

	  save-temp1!
	  recover-temp1!))

(define-interface gc-interface
  (export s48-collect

          s48-trace-value
	  s48-trace-locations!
	  s48-trace-stob-contents!
	  s48-trace-continuation-contents!
	  
	  s48-extant?

	  s48-gc-count
	  ))

(define external-opcodes-interface
  (export ; called from outside
          s48-define-exported-binding
	  s48-get-imported-binding
	  s48-setup-external-exception
	  s48-resetup-external-exception

	  ; called on startup
	  install-shared-bindings!+gc
	  
	  ; for writing images
	  s48-imported-bindings
	  s48-exported-bindings
	  ))

(define-interface external-gc-roots-interface
  (export s48-push-gc-roots!
	  s48-pop-gc-roots!
	  s48-set-gc-roots-base!
	  s48-release-gc-roots-base!
	  s48-register-gc-root!
	  s48-unregister-gc-root!
	  s48-reset-external-roots!))

(define-interface read-image-interface
  (export s48-read-image

	  s48-relocate-all
	  s48-reverse-byte-order!

	  s48-startup-procedure
	  s48-initial-symbols
	  s48-initial-imported-bindings
	  s48-initial-exported-bindings
	  s48-resumer-records

	  s48-initialization-complete!
	  s48-initializing-gc-root

	  s48-set-image-values!))

(define-interface image-gc-interface
  (export begin-making-image
	  trace-image-value
	  make-image
	  restore-heap

	  image-start
	  image-pointer

	  make-image-symbol-table
	  copy-exported-bindings
	  clean-imported-bindings
	  mark-image-channels-closed!
	  find-resumer-records))

(define-interface heap-init-interface
  (export s48-initialize-heap
	  s48-register-static-areas))

(define-interface string-table-interface
  (export hash-table-size
	  hash-table-entry-size
	  make-hash-table+gc
	  table-adder
	  table-searcher
	  table-remover
	  table-walker table-while-walker
	  table-tracer
	  table-cleaner
	  table-relocator

	  value->link
	  link->value))

(define-interface struct-interface
  (export vm-pair? vm-pair-size vm-cons vm-car vm-set-car! vm-cdr vm-set-cdr!
	  vm-symbol? vm-symbol-size vm-make-symbol vm-symbol->string
	  vm-symbol-next vm-set-symbol-next!
	  closure? closure-size make-closure closure-template closure-env
	  set-closure-template! set-closure-env!
	  location? location-size make-location contents set-contents! location-id
	  cell? cell-size make-cell cell-ref cell-set!
	  weak-pointer? weak-pointer-size make-weak-pointer weak-pointer-ref

	  shared-binding? shared-binding-size make-shared-binding
	  shared-binding-name shared-binding-is-import?
	  shared-binding-ref shared-binding-set!
	  shared-binding-next set-shared-binding-next!

	  channel? channel-size make-channel
	  channel-status          set-channel-status!
	  channel-id              set-channel-id!
	  channel-os-index        set-channel-os-index!
	  channel-close-silently? set-channel-close-silently?!
	  channel-next            set-channel-next!
	  channel-os-status       set-channel-os-status!
	  channel-error?          set-channel-error?!
	  
	  port? port-size make-port
	  port-handler
	  port-status set-port-status!
	  port-data set-port-data!
	  port-buffer set-port-buffer!
	  port-index set-port-index!
	  port-limit set-port-limit!
	  port-lock
	  port-pending-eof? set-port-pending-eof?!
   
	  vm-vector? vm-vector-size vm-make-vector vm-vector-length
	  vm-vector-ref vm-vector-set!
	  vm-make-vector+gc
	  vm-vector-fill!

	  record? record-size make-record record-length
	  record-ref record-set!

	  extended-number? extended-number-size make-extended-number
	  extended-number-length
	  extended-number-ref extended-number-set!

	  double? enter-double extract-double double-size

	  make-bignum bignum? bignum-length bignum-size

	  continuation? continuation-size make-continuation continuation-length
	  continuation-ref continuation-set!
	  continuation-cont set-continuation-cont!
	  continuation-pc   set-continuation-pc!
	  continuation-code set-continuation-code!
	  continuation-cells

	  code-vector? code-vector-size make-code-vector code-vector-length
	  code-vector-ref code-vector-set!

	  template? template-size make-template template-length
	  template-ref template-set!
	  template-code template-byte-code template-name

	  vm-string? vm-string-size vm-make-string vm-string-length
	  vm-string-ref vm-string-set!
	  enter-string enter-string+gc extract-low-string
	  vm-string=? vm-string-hash
	  copy-vm-string-chars!
	  write-vm-string
	  copy-string-to-vm-string/latin-1!
	  copy-vm-string-to-string/latin-1!

	  ratnum?

	  ensure-space
	  ))

(define-interface vmio-interface
  (export initialize-i/o-system+gc

	  make-registered-channel
	  s48-really-add-channel
	  s48-close-channel
	  s48-set-channel-os-index
	  s48-channels
	  s48-channel-count

	  close-channel!
	  close-channel-index!
	  os-index->channel
	  open-channels-list

	  trace-channel-names
	  close-untraced-channels!
	  trace-io

	  input-channel?
	  output-channel?
	  open?

	  channel-queue-empty?
	  enqueue-channel!
	  dequeue-channel!
	  vm-channel-abort
	  ))

;(define-interface environment-interface
;  (export current-env set-current-env!
;          env-ref env-set! env-parent env-back
;          pop-args-into-env
;          heap-env-space pop-args-into-heap-env
;          current-env-size preserve-current-env
;          ))

(define-interface stack-interface
  (export initialize-stack+gc

	  reset-stack-pointer stack-size
	  push pop stack-ref stack-set!
	  add-cells-to-stack!

          ensure-stack-space!
	  ensure-default-procedure-space!

	  make-continuation-on-stack
	  push-continuation!
	  push-adlib-continuation!
	  push-exception-continuation!
	  push-native-exception-continuation!
	  pop-continuation-from-stack
	  pop-exception-data
          pop-native-exception-data

	  set-current-continuation!
	  current-continuation-size
	  copy-current-continuation-to-heap
	  skip-current-continuation!
	  get-continuation-from-heap
	  copy-continuation-from-heap!

	  current-continuation-code-pointer
	  shrink-and-reset-continuation!
	  current-continuation-ref

	  pointer-to-stack-arguments
	  move-stack-arguments!
	  move-args-above-cont!
	  pointer-to-stack-arguments  ; for calling external procedures
	  remove-stack-arguments
	  remove-current-frame

	  report-continuation-uids

	  s48-copy-stack-into-heap

	  interrupt-flag-set?
	  clear-interrupt-flag!
	  set-interrupt-flag!

	  *cont*
	  *stack*
	  s48-*stack-limit*
	  check-stack
	  ))

(define-interface external-interface
  (export extended-vm

	  external-call

	  real-time
	  run-time
	  cheap-time
	  schedule-interrupt

 	  external-bignum-make-cached-constants
 	  external-bignum-add
 	  external-bignum-subtract
 	  external-bignum-multiply
 	  external-bignum-quotient
 	  external-bignum-remainder
 	  external-bignum-divide
 	  external-bignum-equal?
 	  external-bignum-compare
 	  external-bignum-test
 	  external-bignum-negate
 	  external-bignum-arithmetic-shift
 	  external-bignum-bitwise-not
 	  external-bignum-bit-count
 	  external-bignum-bitwise-and
 	  external-bignum-bitwise-ior
 	  external-bignum-bitwise-xor
 	  external-bignum-from-long
 	  external-bignum->long
 	  external-bignum-fits-in-word?

	  s48-call-native-procedure
	  s48-invoke-native-continuation
	  s48-native-return

	  get-proposal-lock!
	  release-proposal-lock!

	  shared-ref
	  (shared-set! :syntax)
	  ))

(define-interface event-interface
  (export initialize-events
	  get-next-event
	  wait-for-event
	  (events :syntax)
	  ))

(define-interface channel-interface
  (export current-input-channel
	  current-output-channel
	  current-error-channel
	  input-port->channel
	  output-port->channel
	  open-input-file-channel
	  open-output-file-channel
	  input-channel->port
	  output-channel->port
	  close-input-channel
	  close-output-channel
	  channel-ready?
	  channel-read-block
	  channel-write-block
	  channel-buffer-size
	  channel-console-encoding
	  channel-abort
	  ))

(define external-call-interface
  (export ; called from outside
          s48-define-exported-binding
	  s48-get-imported-binding
	  s48-setup-external-exception
	  s48-resetup-external-exception

	  ; called on startup
	  install-shared-bindings!+gc
	  
	  ; for writing images
	  s48-cleaned-imported-bindings
	  s48-exported-bindings
	  ))

(define-interface bignum-low-interface
  (export initialize-bignums

	  extract-bignum
	  enter-bignum
	  
	  bignum-digits
	  bignum-digit-bits
	  bignum-digits->size
	  bignum-bits-to-size
	  fixnum-as-bignum-digits
	  fixnum-as-bignum-size

	  ensure-bignum-space!
	  set-bignum-preallocation-key!

	  s48-allocate-bignum
	  s48-shorten-bignum))

(define-interface bignum-arithmetic-interface
  (export bignum-add
	  bignum-subtract
	  bignum-multiply
	  bignum-quotient
	  bignum-remainder
	  bignum-divide
	  bignum=
	  bignum<
	  bignum-abs
	  bignum-positive?
	  bignum-nonnegative?
	  bignum-arithmetic-shift
	  bignum-bitwise-not
	  bignum-bit-count
	  bignum-bitwise-and
	  bignum-bitwise-ior
	  bignum-bitwise-xor
	  long->bignum
	  bignum-bits-to-size))

(define-interface integer-arithmetic-interface
  (export integer-add     
	  integer-subtract  
	  integer-multiply  
	  integer-quotient  
	  integer-remainder
	  integer-divide
	  integer-abs
	  integer=
	  integer<
	  integer<=
	  integer>
	  integer>=
	  integer-arithmetic-shift
	  integer-bitwise-not
	  integer-bit-count
	  integer-bitwise-and
	  integer-bitwise-ior
	  integer-bitwise-xor
	  enter-integer
	  long-as-integer-size))

(define-interface interpreter-internal-interface
  (export ((define-primitive define-consing-primitive) :syntax)

	  ((raise-exception raise-exception*) :syntax)
	  raise
	  push-exception-setup!

	  any-> string-> boolean-> fixnum-> vm-integer-> char->
	  vector-> code-vector-> 
	  input-type no-coercion
	  
	  no-result
	  return
	  return-fixnum
	  return-boolean
	  return-unspecific
	  continue
	  continue-with-value

	  code-byte
	  val set-val!
	  code-pointer set-code-pointer!
	  current-thread
	  disable-interrupts!
	  enable-interrupts!
          s48-add-os-signal
	  s48-os-signal-pending

	  s48-*callback-return-stack-block*

	  current-proposal
	  set-current-proposal!
	  ))

(define-interface interpreter-interface
  (export initialize-interpreter+gc
	  clear-registers
	  perform-application
	  s48-set-extension-value!
	  s48-note-event
	  s48-*extension-value*
	  s48-*pending-events?*
	  s48-*pending-interrupt?*
	  s48-reset-interrupts!
	  s48-initialize-shared-registers!
	  s48-*native-protocol*
	  s48-set-native-protocol!
	  
	  *code-pointer*
	  *last-code-called*
	  *native-exception-cont*
	  *val*
          s48-*nc-template*
          s48-*nc-environment*
	  ))

; What the external world can call.  Because of C's flat namespace we have
; added s48_ to the front of each of these.

(define-interface vm-interface
  (export s48-initialize-vm
	  s48-call-startup-procedure

	  s48-restart

	  s48-gc-root
	  s48-post-gc-cleanup

	  s48-set-extension-value!
	  s48-*extension-value*

	  s48-note-event
	  s48-*pending-events?*
	  s48-*pending-interrupt?*
          s48-disable-interrupts!
          s48-enable-interrupts!
	  s48-add-os-signal
          s48-os-signal-pending
	  s48-reset-interrupts!
	  
          s48-define-exported-binding
	  s48-get-imported-binding

	  s48-*callback-return-stack-block*

	  ; used for raising exceptions in external code
	  s48-setup-external-exception
	  s48-resetup-external-exception
	  s48-push
	  s48-stack-ref s48-stack-set!

	  ; called when writing an image
	  s48-symbol-table
	  s48-channels
	  s48-channel-count
	  s48-imported-bindings
	  s48-exported-bindings

	  ; for C code that wants to manipulate channels
	  s48-close-channel
	  s48-really-add-channel
	  s48-set-channel-os-index

 	  ; bignums
	  s48-allocate-bignum
 	  s48-shorten-bignum
 	  s48-enter-integer

	  ; for initializing additional processes
	  s48-reset-external-roots!

	  ; for native code access
	  *code-pointer*
	  *val*
	  *cont*
	  *stack*
	  *last-code-called*
	  *native-exception-cont*
          s48-*nc-template*
          s48-*nc-environment*
	  s48-*stack-limit*
	  s48-*native-protocol*
	  s48-set-native-protocol!
	  s48-copy-stack-into-heap
	  check-stack
	  s48-make-blank-return-code
	  s48-integer-or-floanum-add
	  s48-integer-or-floanum-sub
	  s48-integer-or-floanum-mul
	  s48-integer-or-floanum-=
	  s48-integer-or-floanum-<
	  s48-integer-or-floanum->
	  s48-integer-or-floanum-<=
	  s48-integer-or-floanum->=
          s48-integer-remainder
          s48-integer-quotient
          s48-integer-divide
	  s48-integer-bitwise-not
	  s48-integer-bit-count
	  s48-integer-bitwise-and
	  s48-integer-bitwise-ior
	  s48-integer-bitwise-xor

	  s48-string-set
	  s48-string-ref
	  s48-string-length
	  s48-allocate-string
	  s48-enter-string-latin-1
	  s48-copy-string-to-scheme-string-latin-1
	  s48-copy-scheme-string-to-string-latin-1
	  ))
