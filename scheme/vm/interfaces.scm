; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-interface vm-utilities-interface
  (export adjoin-bits low-bits high-bits unsigned-high-bits
	  assert
	  vector+length-fill!
	  natural-for-each

	  error?
	  write-error-string write-error-integer write-error-newline
	  error-message
	  ))

(define-interface vm-architecture-interface
  (export architecture-version
          (enum :syntax) ;so you don't have to remember to open enumerated
	  bits-used-per-byte byte-limit
	  (interrupt :syntax)
	  interrupt-count
	  (memory-status-option :syntax)
	  (op :syntax)
	  op-count
	  opcode-arg-specs
	  (exception :syntax)
	  (stob :syntax)
	  stob-count
	  least-b-vector-type
	  stob-data
	  (time-option :syntax)
	  (channel-status-option :syntax)
	  (port-status-options :syntax)
	  (current-port-marker :syntax)

	  maximum-stack-args
	  maximum-external-call-args
	  two-byte-nargs-protocol
	  two-byte-nargs+list-protocol
	  args+nargs-protocol
	  big-stack-protocol
	  nary-dispatch-protocol

	  default-stack-space
	  environment-stack-size
	  continuation-stack-size
	  available-stack-space
	  ))

; Memory

(define-interface memory-interface
  (export create-memory memory-begin memory-size

	  bytes-per-cell bits-per-cell
	  bytes->cells
	  cells->bytes
	  a-units->cells
	  cells->a-units
	  bytes->a-units
	  address+ address- address1+ address2+
	  address-difference
	  address= address< address<= address> address>=
	  address->integer integer->address

	  null-address null-address?

	  fetch fetch-byte
	  store! store-byte!

	  fetch-string fetch-nul-terminated-string

	  copy-memory! memory-equal?    ; from ps-memory
	  read-block write-block        ; from ps-memory
	  ))

; Low-level data structures

(define-interface data-interface
  (export vm-eq?
	  fixnum? immediate? header? stob?

	  enter-fixnum extract-fixnum
	  bits-per-fixnum greatest-fixnum-value
	  too-small-for-fixnum? too-big-for-fixnum?
	  descriptor->fixnum
	  fixnum= fixnum< fixnum> fixnum<= fixnum>=
	  fixnum-bitwise-not fixnum-bitwise-and
	  fixnum-bitwise-ior fixnum-bitwise-xor
	  
	  undefined?
	  true false eof-object null unspecific-value quiescent
	  unbound-marker unassigned-marker
	  vm-boolean? false? enter-boolean extract-boolean
	  vm-char? enter-char extract-char vm-char=? vm-char<?

	  make-header header-type
	  header-length-in-bytes header-length-in-cells header-length-in-a-units
	  immutable-header? make-header-immutable
	  d-vector-header? b-vector-header?
	  max-stob-contents-size-in-cells

	  address->stob-descriptor
	  address-after-header
	  stob-header stob-header-set!
	  stob-overhead
	  ))

(define-interface fixnum-arithmetic-interface
  (export add-carefully subtract-carefully
	  multiply-carefully divide-carefully
	  quotient-carefully remainder-carefully
	  shift-carefully
	  abs-carefully
	  ))

(define-interface stob-interface
  (export make-d-vector make-b-vector
          d-vector-length d-vector-ref d-vector-init! d-vector-set!
	  b-vector-length b-vector-ref b-vector-set!
	  max-stob-size-in-cells

	  stob-of-type?
	  header+contents->stob
	  valid-index?
	  	
	  immutable? make-immutable!
	  ))

(define-interface allocation-interface
  (export s48-available?
	  s48-preallocate-space
	  s48-allocate-space
	  s48-write-barrier
          ))

(define-interface heap-interface
  (export s48-available
	  s48-heap-size

	  s48-find-all
	  s48-find-all-records

	  s48-*hp*	; to keep these from being `static' in C, as
	  s48-*limit*	; references are introduced elsewhere via a C macro
	  ))

(define heap-gc/image-interface
  (export swap-spaces
	  find-resumer-records
	  heap-start
	  heap-pointer
	  heap-limit
	  set-heap-pointer!
	  have-static-areas?
	  walk-impure-areas))

(define-interface interpreter-gc-interface
  (export collect
	  collect-saving-temp

	  ensure-space
	  ensure-space-saving-temp
	  ensure-space-saving-temps

	  maybe-ensure-space
	  maybe-ensure-space-saving-temp
	  maybe-ensure-space-saving-temps

	  add-gc-root!
	  add-post-gc-cleanup!
	  ))

(define-interface gc-interface
  (export s48-trace-value
	  s48-trace-locations!
	  s48-trace-stob-contents!

	  s48-begin-collection
	  s48-do-gc
	  s48-end-collection

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
	  s48-warn-about-undefined-imported-bindings
	  install-shared-bindings!+gc
	  
	  ; for writing images
	  s48-cleaned-imported-bindings
	  s48-exported-bindings
	  ))

(define-interface external-gc-roots-interface
  (export s48-push-gc-roots!
	  s48-pop-gc-roots!
	  s48-set-gc-roots-base!
	  s48-release-gc-roots-base!
	  s48-register-gc-root!

	  s48-allocate-stob))

(define-interface images-interface
  (export s48-image-writing-okay?
	  s48-write-image
	  s48-check-image-header
	  s48-read-image

	  s48-startup-procedure
	  s48-initial-symbols
	  s48-initial-imported-bindings
	  s48-initial-exported-bindings
	  s48-resumer-records
	  s48-undumpable-records

	  s48-initialization-complete!
	  s48-initializing-gc-root

	  s48-set-image-values!
	  ))

(define-interface heap-init-interface
  (export s48-initialize-heap
	  s48-register-static-areas
	  ))

(define-interface string-table-interface
  (export hash-table-size
	  hash-table-entry-size
	  make-hash-table
	  table-adder
	  table-searcher
	  table-remover
	  table-walker
	  table-cleaner))

(define-interface struct-interface
  (export vm-pair? vm-pair-size vm-cons vm-car vm-set-car! vm-cdr vm-set-cdr!
	  vm-symbol? vm-symbol-size vm-make-symbol vm-symbol->string
	  vm-symbol-next vm-set-symbol-next!
	  closure? closure-size make-closure closure-template closure-env
	  location? location-size make-location contents set-contents! location-id
	  weak-pointer? weak-pointer-size make-weak-pointer weak-pointer-ref

	  shared-binding? shared-binding-size make-shared-binding
	  shared-binding-name shared-binding-is-import?
	  shared-binding-ref shared-binding-set!
	  shared-binding-next set-shared-binding-next!

	  channel? channel-size make-channel
	  channel-status    set-channel-status!
	  channel-id        set-channel-id!
	  channel-os-index  set-channel-os-index!
	  channel-next      set-channel-next!
	  channel-os-status set-channel-os-status!
	  
	  port? port-size make-port
	  port-handler
	  port-status set-port-status!
	  port-data set-port-data!
	  port-buffer set-port-buffer!
	  port-index set-port-index!
	  port-limit set-port-limit!
	  port-lock
	  port-locked? set-port-locked?!
	  port-pending-eof? set-port-pending-eof?!
   
	  vm-vector? vm-vector-size vm-make-vector vm-vector-length
	  vm-vector-ref vm-vector-set!
	  vm-vector-fill!

	  record? record-size make-record record-length
	  record-ref record-set!

	  extended-number? extended-number-size make-extended-number
	  extended-number-length
	  extended-number-ref extended-number-set!

	  continuation? continuation-size make-continuation continuation-length
	  continuation-ref continuation-set!
	  continuation-cont     set-continuation-cont!
	  continuation-pc       set-continuation-pc!
	  continuation-template set-continuation-template!
	  continuation-env      set-continuation-env!
	  continuation-cells

	  code-vector? code-vector-size make-code-vector code-vector-length
	  code-vector-ref code-vector-set!

	  template? template-size make-template template-length
	  template-ref template-set!
	  template-code template-name
	  make-template-containing-ops op-template-size

	  vm-string? vm-string-size vm-make-string vm-string-length
	  vm-string-ref vm-string-set!
	  enter-string extract-string
	  vm-string=? vm-string-hash

	  bignum? ratnum? double?
	  ))

(define-interface vmio-interface
  (export initialize-i/o-system+gc

	  make-registered-channel
	  s48-add-channel
	  s48-close-channel
	  s48-set-channel-os-index
	  close-channel!
	  close-channel-index!
	  os-index->channel
	  open-channels-list

	  close-untraced-channels!
	  s48-mark-traced-channels-closed!
	  trace-io

	  input-channel?
	  output-channel?
	  open?

	  channel-queue-empty?
	  enqueue-channel!
	  dequeue-channel!
	  vm-channel-abort
	  ))

(define-interface environment-interface
  (export current-env set-current-env!
	  env-ref env-set! env-parent env-back
	  pop-args-into-env
          heap-env-space pop-args-into-heap-env
	  current-env-size preserve-current-env
	  ))

(define-interface stack-interface
  (export initialize-stack+gc

	  reset-stack-pointer stack-size
	  push pop stack-ref stack-set!

          ensure-stack-space!
	  ensure-default-procedure-space!

	  push-continuation-on-stack
	  pop-continuation-from-stack

	  current-continuation-size current-continuation
	  set-current-continuation!
	  peek-at-current-continuation skip-current-continuation!
	  get-continuation-from-heap

	  trace-stack

	  arguments-on-stack
	  move-args-above-cont!
	  pointer-to-stack-arguments  ; for calling external procedures
	  remove-stack-arguments

	  report-continuation-uids
	  ))

(define-interface external-interface
  (export extended-vm

	  external-call

	  real-time
	  run-time
	  cheap-time
	  schedule-interrupt
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
	  channel-read-block
	  channel-write-block
	  channel-abort
	  ))

(define external-call-interface
  (export ; called from outside
          s48-define-exported-binding
	  s48-get-imported-binding
	  s48-setup-external-exception
	  s48-resetup-external-exception

	  ; called on startup
	  s48-warn-about-undefined-imported-bindings
	  install-shared-bindings!+gc
	  
	  ; for writing images
	  s48-cleaned-imported-bindings
	  s48-exported-bindings
	  ))

(define-interface interpreter-internal-interface
  (export ((define-primitive define-consing-primitive) :syntax)

	  ((raise-exception raise-exception*) :syntax)
	  raise
	  push-exception-continuation!

	  any-> string-> fixnum-> boolean->
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
	  s48-set-os-signal

	  s48-*callback-return-stack-block*
	  ))

(define-interface interpreter-interface
  (export initialize-interpreter+gc
	  clear-registers
	  interpret
	  s48-set-extension-value!
	  s48-note-event
	  s48-*extension-value*
	  s48-*pending-events?*
	  s48-*pending-interrupt?*
	  ))

; What the external world can call.  Because of C's flat namespace we have
; added s48_ to the front of each of these.

(define-interface vm-interface
  (export s48-initialize-vm
	  s48-call-startup-procedure

	  s48-restart

	  s48-set-extension-value!
	  s48-*extension-value*

	  s48-note-event
	  s48-*pending-events?*
	  s48-*pending-interrupt?*
          s48-disable-interrupts!
          s48-enable-interrupts!
	  s48-set-os-signal
	  
          s48-define-exported-binding
	  s48-get-imported-binding

	  s48-*callback-return-stack-block*

	  ; used for raising exceptions in external code
	  s48-setup-external-exception
	  s48-resetup-external-exception
	  s48-push
	  s48-stack-ref s48-stack-set!

	  ; called when writing an image
	  s48-copy-symbol-table
	  s48-mark-traced-channels-closed!
	  s48-cleaned-imported-bindings
	  s48-exported-bindings

	  ; called when resuming an image
	  s48-warn-about-undefined-imported-bindings

	  ; for C code that wants to manipulate channels
	  s48-close-channel
	  s48-add-channel
	  s48-set-channel-os-index
	  ))
