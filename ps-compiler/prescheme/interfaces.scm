; Copyright (c) 1994 Richard Kelsey.  See file COPYING.

(define-interface ps-primop-interface
  (export get-prescheme-primop
          (define-scheme-primop :syntax)
          (define-nonsimple-scheme-primop :syntax)
          (define-scheme-cond-primop :syntax)

	  prescheme-integer-size
	  lshr))

(define-interface ps-c-primop-interface
  (export simple-c-primop?
          primop-generate-c
          (define-c-generator :syntax)))

(define-interface ps-type-interface
  (export type/int7u
          type/int8
          type/int8u
          type/int32
          type/float64
	  type/char

          type/null
          type/unit
          type/boolean
          type/undetermined
          type/input-port
          type/output-port

          type/unknown
          type/byte-pointer
          type/string

          other-type?
          other-type-kind
          other-type-subtypes
          make-other-type

          base-type?
          base-type-name
	  base-type-uid
	  make-atomic-type

          make-arrow-type
          arrow-type?
          arrow-type-result
          arrow-type-args

          make-pointer-type
          pointer-type?
          pointer-type-to

	  make-tuple-type
	  tuple-type?
	  tuple-type-types

	  record-type?

          lookup-type

          type-scheme?
          schemify-type
          instantiate-type-scheme
          copy-type
          type-scheme-type
          make-nonpolymorphic!

          type-scheme-free-uvars           ; for error messages
          type-scheme-lattice-uvars
          type-scheme-type

          type-eq?
          type>
          type>=

          lattice-type?

	  expand-type-spec

          finalize-type

          display-type

	  make-base-type-table
          ))

(define-interface type-variable-interface
  (export make-uvar
	  make-arith-op-uvar
	  make-tuple-uvar
          uvar?
          maybe-follow-uvar
          set-uvar-source!

          reset-type-vars!

          uvar-binding set-uvar-binding!
          uvar-sources
          uvar-sinks
          uvar-prefix
          uvar-id
          uvar-arith-op?
	  uvar-temp set-uvar-temp!

          bind-uvar!

          unique-id

          make-constrained-type

          relation-source
          relation-sink
          relation-site
          relation-temp set-relation-temp!

          constrained-uvars
	  uvar-constraint-count
          ))

(define-interface record-type-interface
  (export reset-record-data!
	  all-record-types
	  get-record-type
	  record-type-name
	  record-type-fields
	  get-record-type-field
	  record-field-record-type
	  record-field-name
	  record-field-type))

(define-interface inference-interface
  (export infer-definition-type
	  get-package-variable-type
	  get-variable-type
          add-type-coercions
	  node-type
	  lambda-node-return-type))

(define-interface inference-internal-interface
  (export unify!
	  infer-type
          check-arg-type
          literal-value-type
          ))

(define-interface form-interface
  (export make-form
          form?
          form-value
          set-form-value!
	  node-form
          set-form-node!
          set-form-integrate!
          set-form-exported?!
	  form-node
          form-var
          form-exported?
          form-type
          set-form-type!
          form-free set-form-free!
	  suspend-form-use!
	  use-this-form!
	  also-use-this-form!
	  set-form-lambdas!
          form-lambdas
          form-name
          form-merge set-form-merge!
          form-providers set-form-providers!
          form-clients set-form-clients!
	  form-shadowed set-form-shadowed!
            
	  variable-set!? note-variable-set!!
	  make-form-unused!
          variable->form
          maybe-variable->form
            
          ; high level stuff
          sort-forms
          expand-and-simplify-form
          remove-unreferenced-forms
          integrate-stob-form
          resimplify-form
          ))

(define-interface linking-interface
  (export package-specs->scanner+exports
          define-prescheme!
	  prescheme-compiler-env
          ))

(define-interface c-internal-interface
  (export c-assignment
          indent-to
          c-argument-var
          form-tail-called?
          *doing-tail-called-procedure?*
          merged-procedure-reference
          goto-protocol?
          save-renamed-variables
          restore-renamed-variables
          c-ify
          c-value
          form-value
          form-name
          form-c-name
          form-type
          c-assign-to-variable
          write-c-identifier
          c-called-value
          write-value-list
	  write-value-list-with-extras
          write-value+result-var-list
          form-return-count
          set-form-return-count!
          simple-c-primop
          c-variable
          *current-merged-procedure*
          *extra-tail-call-args*
          write-c-block
          write-c-coercion
          no-value-node?
          display-c-type
	  add-c-type-declaration!
	  note-jump-generated!
          ))

