
(define-interface vm-utilities-interface
  (export adjoin-bits low-bits high-bits unsigned-high-bits
	  assert
	  read-number write-number
	  vector+length-fill!))

(define-interface vm-architecture-interface
  (export (enum :syntax) ;so you don't have to remember to open enumerated
	  bits-used-per-byte
	  maximum-stack-args
	  interrupt
	  interrupt-count
	  memory-status-option
	  op
	  op-count
	  opcode-arg-specs
	  stob
	  stob-count
	  least-b-vector-type
	  stob-data
	  time-option
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
	  addr+ addr- addr1+ addr= addr< addr<= addr> addr>= 

	  fetch fetch-byte
	  store! store-byte!
	  write-bytes read-bytes
	  ))

; Low-level data structures

(define-interface data-interface
  (export vm-eq?
	  fixnum? immediate? header? stob?

	  enter-fixnum extract-fixnum
	  bits-per-fixnum too-small-for-fixnum? too-big-for-fixnum?
	  vm-= vm-<

	  undefined?
	  true false eof-object null unspecific quiescent
	  unbound-marker unassigned-marker
	  vm-boolean? false? enter-boolean extract-boolean
	  vm-char? enter-char extract-char vm-char=? vm-char<?

	  make-header header-type
	  header-length-in-bytes header-length-in-cells
	  immutable-header? make-header-immutable
	  d-vector-header? b-vector-header?
	  okay-stob-size?

	  address->stob-descriptor
	  stob-descriptor->address
	  ))
	  
(define-interface fixnum-arithmetic-interface
  (export add-carefully subtract-carefully
	  multiply-carefully divide-carefully
	  quotient-carefully remainder-carefully
	  shift-carefully
	  abs-carefully
	  ))

(define-interface stob-interface
  (export make-d-vector make-b-vector universal-key
	  stob-header stob-header-set!
          d-vector-length d-vector-ref d-vector-set!
	  b-vector-length b-vector-ref b-vector-set!

	  stob-length-in-bytes stob-of-type?
	  copy-stob copy-cells!
	  stob-equal?
	  valid-index?
	  	
	  immutable? make-immutable!
	  address-at-header address-after-header address-after-stob
	  ))

(define-interface heap-interface
  (export initialize-heap
	  register-static-areas

          available? available heap-size
	  preallocate-space
          
	  begin-collection
	  trace-value trace-locations
	  trace-impure-areas
	  do-gc
	  end-collection abort-collection
	  gc-count

	  image-writing-okay? write-image
	  check-image-header read-image

	  walk-over-symbols find-all-xs
	  ))

(define-interface struct-interface
  (export vm-pair? vm-pair-size vm-cons vm-car vm-set-car! vm-cdr vm-set-cdr!
	  vm-symbol? vm-symbol-size vm-symbol->string
	  closure? closure-size make-closure closure-template closure-env
	  location? location-size make-location contents set-contents! location-id
	  weak-pointer? weak-pointer-size make-weak-pointer weak-pointer-ref
	  external? external-size make-external external-name external-value

	  port? port-size make-port
	  port-mode   set-port-mode!
	  port-index  set-port-index!
	  peeked-char set-peeked-char!
	  port-id     set-port-id!
   
	  vm-vector? vm-vector-size vm-make-vector vm-vector-length
	  vm-vector-ref vm-vector-set!
	  vm-vector-fill!

	  record? record-size vm-make-vector record-length
	  record-ref record-set!

	  extended-number? extended-number-size vm-make-vector extended-number-length
	  extended-number-ref extended-number-set!

	  continuation? continuation-size vm-make-vector continuation-length
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
	  write-vm-string vm-string=? vm-string-hash

	  intern add-to-symbol-table
	  ))

(define-interface vmio-interface
  (export initialize-i/o-system initial-i/o-heap-space
	  initial-input-port initial-output-port

	  extract-port
          vm-input-port? vm-output-port?
	  open-port
	  open?
	  close-port
	  close-untraced-ports!
	  ))

(define-interface environment-interface
  (export current-env set-current-env!
	  env-ref env-set! env-parent env-back
	  stack-env-space pop-args-into-env
          heap-env-space pop-args-into-heap-env
	  current-env-size preserve-current-env
	  ))

(define-interface stack-interface
  (export initialize-stack initial-stack-heap-space
	  
	  reset-stack-pointer stack-size
	  push pop stack-ref stack-set!
	  arg-stack-overflow-nargs

          ensure-stack-space
	  reserve-exception-space allow-exception-consing

	  stack-continuation-size
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
	  ))

(define-interface external-interface
  (export extended-vm

	  ps-real-time
	  ps-run-time
	  ps-ticks-per-second
	  ps-schedule-interrupt
	  lookup-external-name
	  call-external-value

	  ))

; From Scheme 48
(define-interface ps-enumerated-interface
  (export (define-enumeration :syntax)
	  (enum :syntax)
	  (enumerand->name :syntax)
	  (name->enumerand :syntax)))

; What the external world can call
(define interpreter-interface
  (export required-init-space
	  initialize-vm
	  register-static-areas
	  call-startup-procedure
	  check-image-header
	  read-image
	  do-gc
	  note-interrupt!))
