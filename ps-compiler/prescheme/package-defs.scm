; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.

; Pre-Scheme packages

; Everything else

(define-structures ((prescheme-compiler (export)))
  (open scheme big-scheme conditions comp-util structure-refs
	prescheme-front-end prescheme-display
	parameters
	node
	front-debug forms
	ps-types
	type-variables
	c
	structure-refs
	primop-data
	c-primop-data
	jump                 ;find-jump-procs procs->jumps 
	record-types         ;reset-record-data!
	node-types
	front                ;simplify-all
	simplify)            ;simplify-node
  (files top))

(define-structures ((prescheme-display (export display-forms-as-scheme)))
  (open scheme big-scheme
	structure-refs
	names
	bindings		;binding-place
	nodes
	variable primop external-values ps-primitives
	flatten-internal	;generated-top-variable?
	external-constants)
  (access forms)		;form-value form-var
  (files display))

(define-structures ((protocol (export normal-protocol
				      goto-protocol
				      goto-protocol?)))
  (open scheme big-scheme comp-util set-parameters ps-primops ps-types node)
  (files spec))

(define-structures ((prescheme-front-end (export prescheme-front-end)))
  (open scheme big-scheme comp-util structure-refs
	linking expand flatten forms
	ps-types inference
	variable
	primitive-data
	primop-data
	inference-internal  ; unify!
	type-variables)     ; reset-type-vars!
  (access nodes		    ; node? schemify
	  node              ; reset-node-id
	  record-types)     ; reset-record-types!    
  (files front-end))

(define-structures ((forms form-interface))
  (open scheme big-scheme comp-util node expand defrecord
	node-vector queues to-cps
	structure-refs
	eval-node         ; closure stuff
	ps-primops        ; get-prescheme-primop
	simplify-internal ; simplify-node simplify-args
	front             ; simplify-all
	ps-types          ; type/undetermined
	type-variables    ; maybe-follow-uvar
	node-types)       ; instantiate-type&value
  (access nodes)          ; node-predicate
  (files form))

; Translating Scheme into evaluated nodes

(define-structures ((expand (export scan-packages)))
  (open scheme big-scheme comp-util structure-refs
	variable
	bindings nodes
	ps-primitives   ;eval-primitive
	eval-node       ;eval-node
	scan-package	;package-source
	locations
	util		;fold
	syntactic)
  (access packages)     ;package->environment
  (files expand))

; Eval and type information on Pre-Scheme primitives

(define-structures ((ps-primitives (export primitive?
					   make-primitive
					   eval-primitive
					   primitive-id
					   primitive-source
					   primitive-expander
					   primitive-expands-in-place?
					   primitive-inference-rule)))
  (open scheme big-scheme comp-util defrecord)
  (files primitive))

(define-structures ((primitive-data (export)))
  (open scheme big-scheme comp-util ps-primitives
	bindings nodes
	ascii structure-refs
	ps-primops		;get-prescheme-primop
	linking			;define-prescheme!
	inference-internal	;check-arg-type
	type-variables		;make-arith-op-uvar
	record-types
	prescheme ps-memory
	ps-types external-constants external-values
	floatnums		;floatnum?
	locations
	eval-node)  ; closure?  (to keep them from being made immutable)
  (access variable)   ; variable-name
  (files (primop scm-scheme)
	 (primop scm-arith)
	 (primop scm-memory)
	 (primop scm-record)))

(define-structures ((eval-node (export eval-node
				       closure? closure-node closure-env
				       make-top-level-closure
				       closure-temp set-closure-temp!
				       apply-closure
				       unspecific? constant?)))
  (open scheme define-record-types
	nodes
	ps-types		;expand-type-spec
	external-values
	external-constants	;external-constant?
	signals			;error
	util)			;unspecific
  (files eval))

; Reducing closures and data structures to simple definitions

(define-structures ((flatten (export flatten-definitions))
		    (flatten-internal (export generated-top-variable?)))
  (open scheme big-scheme comp-util defrecord
	structure-refs
	bindings nodes
	variable
	eval-node 	;closure stuff, constant?
	ps-primitives	;primitive stuff
	ps-types	;type/undetermined expand-type-spec
	linking		;prescheme-compiler-env
	syntactic	;expand
	strong
	external-values
	locations
	features)	;immutable?
  (access forms)	;avoid name conflict with NODE-FORM in nodes
  (files flatten substitute))

(define-structures ((to-cps (export x->cps)))
  (open scheme big-scheme comp-util
	variable
	names bindings nodes
	primop
	structure-refs
	cps-util enumerated
	ps-primops 	;get-prescheme-primop
	ps-types 	;type/unknown
	inference	;node-type lambda-node-return-type
	ps-primitives	;primitive-expander
	protocol)	;goto-protocol normal-protocol
  (access node)
  (files to-cps))
	
; Processing interface and package definitions

(define-structures ((linking linking-interface))
  (open scheme big-scheme structure-refs comp-util
	interfaces packages environments usual-macros
	defpackage types ;for making interfaces
        reflective-tower-maker
	fluids cells
	expand-define-record-type
	scan-package	 ;collect-packages
	bindings         ;binding? binding-place
	nodes		 ;get-operator
	transforms       ;make-transform
	locations)       ;contents
  (access meta-types           		;syntax-type usual-variable-type
	  variable             		;make-global-variable
	  ps-types             		;type/unknown
	  reading-forms			;$note-file-package
	  packages-internal    		;$get-location
	  package-commands-internal	;config-package
	  prescheme            		;we need this loaded
	  built-in-structures) 		;defpackage structure-refs
  (files linking))

;----------------------------------------------------------------
; Types and type inference

(define-structures ((ps-types ps-type-interface)
		    (type-variables type-variable-interface)
		    (record-types record-type-interface)
		    (expand-define-record-type
		     (export expand-define-record-type)))
  (open scheme big-scheme comp-util define-record-types)
  (files type
	 type-scheme
	 type-var
	 record))

(define-structures ((inference inference-interface)
		    (inference-internal inference-internal-interface))
  (open scheme big-scheme front variable comp-util transitive
	ps-types type-variables
	bindings nodes
	structure-refs
	ps-primitives
	ps-primops    ; get-prescheme-primop
	external-values external-constants
	locations)    ; for imported constants
  (access eval-node)  ; unspecific?
  (for-syntax (open scheme big-scheme))
  (files inference infer-early))

(define-structures ((node-types (export instantiate-type&value
					make-monomorphic!)))
  (open scheme big-scheme front node comp-util
	ps-types type-variables
	inference-internal)  ; unify!
  (files node-type))

;----------------------------------------------------------------
; Primops

(define-structures ((ps-primops ps-primop-interface))
  (open scheme big-scheme comp-util node simplify-internal
	linking ps-types front expand)
  (files (primop primop)))

(define-structures ((ps-c-primops ps-c-primop-interface))
  (open scheme big-scheme comp-util node simplify-internal
	define-record-types
	ps-types ps-primops)
  (for-syntax (open scheme big-scheme))
  (files (primop c-primop)))

(define-structures ((primop-data (export)))
  (open scheme big-scheme comp-util node simplify-internal simplify-let
	front expand type-variables inference-internal
	ps-types ps-primops record-types
	parameters node-vector
	node-types)   ; instantiate-type&value
  (files (primop base)
	 (primop arith)
	 (primop io)
	 (primop vector)
	 ))

(define-structures ((c-primop-data (export)))
  (open scheme big-scheme comp-util node simplify
	ps-types ps-primops ps-c-primops
	front
	structure-refs
	c-internal
	ps-types type-variables inference-internal
	inference          ; get-variable-type
	forms
	compiler-byte-vectors
	record-types
	eval-node)         ; unspecific?
  (access ps-primitives prescheme)
  (files (primop c-base)
	 (primop c-arith)
	 (primop c-io)
	 (primop c-vector)
	 ))

(define-structures ((external-values (export external-value?
					     make-external-value
					     external-value-type
					     external-value-string)))
  (open scheme define-record-types)
  (begin
    (define-record-type external-value :external-value
      (make-external-value string type)
      external-value?
      (string external-value-string)
      (type external-value-type))))

;----------------------------------------------------------------
; Translating to C

(define-structures ((c (export write-c-file hoist-nested-procedures))
		    (c-internal c-internal-interface))
  (open scheme big-scheme comp-util strongly-connected node forms
	defrecord
	ps-primops ps-c-primops
	ps-types type-variables
	flatten-internal   ; generated-top-variable?
	inference          ; get-variable-type
	inference-internal ; literal-value-type
	protocol           ; goto-protocol?
	i/o		   ; force-output
	record-types
	external-values
	external-constants
	eval-node)         ; unspecific?
  (begin (define number-of-char-codes 256))  ; should be somewhere else
  (files c
	 c-decl
	 c-call
	 hoist
	 merge))


