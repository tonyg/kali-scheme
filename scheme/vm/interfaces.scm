; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.

(define-interface vm-utilities-interface
  (export adjoin-bits low-bits high-bits unsigned-high-bits
	  assert
	  vector+length-fill!

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
	  vm-= vm-< vm-> vm-<= vm->=

	  undefined?
	  true false eof-object null unspecific-value quiescent
	  unbound-marker unassigned-marker
	  vm-boolean? false? enter-boolean extract-boolean
	  vm-char? enter-char extract-char vm-char=? vm-char<?

	  make-header header-type
	  header-length-in-bytes header-length-in-cells
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
  (export available?
	  preallocate-space universal-key
	  allocate-space
	  write-barrier
          ))

(define-interface heap-interface
  (export available heap-size
          
	  begin-collection
	  trace-value trace-locations! trace-stob-contents!
	  do-gc
	  end-collection
	  gc-count

	  image-writing-okay?
	  write-image

	  find-all find-all-records

	  *hp*		; to keep these from being `static' in C, as
	  *limit*	; references are introduced elsewhere via a C macro
	  ))

(define-interface heap-init-interface
  (export initialize-heap
	  register-static-areas
	  check-image-header
	  read-image
	  ))

(define-interface struct-interface
  (export vm-pair? vm-pair-size vm-cons vm-car vm-set-car! vm-cdr vm-set-cdr!
	  vm-symbol? vm-symbol-size vm-symbol->string
	  closure? closure-size make-closure closure-template closure-env
	  location? location-size make-location contents set-contents! location-id
	  weak-pointer? weak-pointer-size make-weak-pointer weak-pointer-ref
	  external? external-size make-external external-name external-value

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

	  intern add-to-symbol-table
	  ))

(define-interface vmio-interface
  (export initialize-i/o-system initial-i/o-heap-space

	  make-registered-channel
	  close-channel!
	  close-channel-index!
	  os-index->channel

	  close-untraced-channels!
	  mark-traced-channels-closed!
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
  (export initialize-stack initial-stack-heap-space
	  
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

	  real-time
	  run-time
	  cheap-time
	  schedule-interrupt
	  lookup-external-name
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

; What the external world can call
(define interpreter-interface
  (export required-init-space
	  initialize-vm
	  call-startup-procedure
	  restart
	  set-extension-value!
	  note-event
	  *extension-value*
	  *pending-events?*
	  *pending-interrupt?*
	  ))

