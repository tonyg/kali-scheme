
; Two interfaces that are shared with the Scheme code.  We can't share the
; structure definitions, but this is a start.

(define-interface vm-architecture-interface
  (export architecture-version
          (enum :syntax) ;so you don't have to remember to open enumerated
	  bits-used-per-byte byte-limit two-byte-limit
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
	  ignore-values-protocol
	  args+nargs-protocol
	  nary-dispatch-protocol
	  bottom-of-stack-protocol
	  call-with-values-protocol
	  big-stack-protocol
	  maximum-external-call-args
	  native-protocol-mask

	  default-stack-space
	  continuation-stack-size
	  available-stack-space

	  continuation-cells
	  continuation-cont-index
	  continuation-pc-index
	  continuation-code-index
	  
          exception-continuation-cells
	  exception-cont-size-index
	  exception-cont-pc-index
	  exception-cont-code-index
	  exception-cont-exception-index
	  exception-cont-instruction-size-index
          
          native-exception-continuation-cells
          native-exception-cont-size-index
          native-exception-cont-exception-index
          native-exception-cont-bc-pc-index
          native-exception-cont-bc-code-index
	  ))

; Data structures

(define-interface vm-data-interface
  (export bytes-per-cell bits-per-cell addressing-units-per-cell
	  bytes->cells
	  cells->bytes
	  a-units->cells
	  cells->a-units
	  bytes->a-units

	  vm-eq?

	  (tag :syntax)
	  fixnum? immediate? header? stob?
	  tag-field-width
	  make-tag&immediate-type

	  enter-fixnum extract-fixnum
	  bits-per-fixnum greatest-fixnum-value
	  too-small-for-fixnum? too-big-for-fixnum?
	  descriptor->fixnum fixnum->stob
	  fixnum= fixnum< fixnum> fixnum<= fixnum>=
	  fixnum-bitwise-not fixnum-bitwise-and
	  fixnum-bitwise-ior fixnum-bitwise-xor
	  
	  (imm :syntax)
	  immediate-type-field-width
	  undefined?
	  true false eof-object null unspecific-value unreleased-value quiescent
	  unbound-marker unassigned-marker
	  vm-boolean? false? enter-boolean extract-boolean
	  vm-char? enter-char extract-char vm-char=? vm-char<?

	  make-header header-type
	  header-type-field-width
	  header-immutable-bit-mask
	  header-length-in-bytes header-length-in-cells header-length-in-a-units
	  immutable-header? make-header-immutable
	  d-vector-header? b-vector-header?
	  max-stob-contents-size-in-cells

	  add-stob-tag remove-stob-tag
	  ))

