; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Some signatures.  Order of presentation is a bit random.

(define-signature scheme-level-0-signature
  (export ((if begin lambda letrec quote set!
	       define define-syntax let-syntax)
	   syntax)
	  ;; letrec-syntax  -- not yet implemented
	  
	  ;; The basic derived expression types.
	  ((and cond do let let* or) syntax)

	  apply

	  ;; Scalar primitives
	  eq?
	  number? integer? rational? real? complex?
	  exact? exact->inexact inexact->exact
	  + - * / = <
	  quotient remainder
	  floor numerator denominator
	  real-part imag-part
	  exp log sin cos tan asin acos atan sqrt
	  angle magnitude make-polar make-rectangular
	  char?
	  char=? char<?
	  eof-object?

	  ;; Data manipulation
	  pair? cons car cdr set-car! set-cdr!
	  symbol? symbol->string
	  string? make-string string-length string-ref string-set!
	  vector? make-vector vector-length vector-ref vector-set!
	  input-port? output-port?
	  read-char peek-char char-ready?
	  write-char 

	  ;; Unnecessarily primitive
	  string=?

	  ;; New in Revised^5 Scheme
	  values call-with-values

	  ;; Things aren't primitive at the VM level, but they have
	  ;; to be defined using VM primitives.
	  string-copy
	  string->symbol
	  procedure?
	  integer->char char->integer
	  close-output-port close-input-port
	  open-input-file open-output-file))

; Miscellaneous primitives.

(define-signature primitives-signature
  (export close-port			;extended-ports
	  collect			; ,collect command
	  continuation-length
	  continuation-ref
	  continuation-set!
	  continuation?
	  extended-number-length
	  extended-number-ref
	  extended-number-set!
	  extended-number?
	  external-call
	  external-lookup
	  external-name
	  external-value
	  external?
	  find-all-xs			; externals.scm
	  force-output			;port re-exports this.
	  get-dynamic-state		;fluids
	  make-continuation
	  make-extended-number
	  make-external
	  make-record
	  make-template
	  make-weak-pointer
	  memory-status			;interrupts
	  record-length
	  record-ref
	  record-set!
	  record?
	  schedule-interrupt		;interrupts re-exports
	  set-dynamic-state!		;fluids
	  set-enabled-interrupts!	;interrupts
	  set-exception-handler!
	  set-interrupt-handlers!	;interrupts
	  template-length
	  template-ref
	  template-set!
	  template?
	  time				;interrupts
	  unspecific			;record
	  vm-extension
	  vm-return
	  weak-pointer-ref
	  weak-pointer?
	  write-string))		;port

(define-signature bitwise-signature
  (export arithmetic-shift
	  bitwise-not bitwise-and bitwise-ior bitwise-xor))

(define-signature locations-signature
  (export location?
	  location-assigned?
	  location-defined?
	  location-id
	  make-location			;run.scm ? mini-packages.scm ?
	  make-undefined-location	;defined in base.scm
	  set-location-defined?!
	  contents
	  set-contents!))

(define-signature closures-signature
  (export closure?
	  make-closure
	  closure-env
	  closure-template))

(define-signature code-vectors-signature
  (export code-vector?
	  code-vector-length
	  code-vector-ref
	  code-vector-set!
	  make-code-vector))

; Miscellaneous useful primitives used in various random places.

(define-signature features-signature
  (export force-output
	  immutable?
	  make-immutable!
	  string-hash))


; Another hodge-podge.

(define-signature low-level-signature
  (export vector-unassigned?		; inspector
	  flush-the-symbol-table!	;build.scm
	  maybe-open-input-file		;mobot system
	  maybe-open-output-file))

(define-signature vm-exposure-signature
  (export invoke-closure		;eval
	  primitive-catch))		;shadowing, start, command

(define-signature escapes-signature
  (export primitive-cwcc
	  with-continuation))

(define-signature ascii-signature
  (export ascii->char
	  char->ascii
	  ascii-limit
	  ascii-whitespaces))


; Level 1: The rest of Scheme except for I/O.

(define-signature scheme-level-1-adds-signature
  (export ((case delay quasiquote syntax-rules) syntax)
	  <= > >=
	  abs
	  append  assoc assq assv
	  boolean?
	  caaaar caaadr caadar caaddr caaar caadr caar
	  cadaar cadadr caddar cadddr cadar caddr cadr
	  cdaaar cdaadr cdadar cdaddr cdaar cdadr cdar
	  cddaar cddadr cdddar cddddr cddar cdddr cddr
	  char->integer char-alphabetic?
	  ceiling
	  char-ci<=? char-ci<? char-ci=? char-ci>=? char-ci>?
	  char-downcase char-lower-case? char-numeric?
	  char-upcase
	  char-upper-case? char-whitespace? char<=?
	  char>=? char>? close-input-port close-output-port
	  equal? eqv? even? expt
	  for-each force
	  gcd
	  inexact?
	  integer->char
	  lcm length list list->string list->vector
	  list?				;New in R4RS
	  list-ref list-tail
	  map max member memq memv min modulo
	  negative? not null?
	  odd? open-input-file open-output-file
	  positive?
	  procedure?			;from low-level
	  rationalize
	  reverse
	  round 
	  string string->list string->symbol
	  string-append
	  string-ci<=? string-ci<? string-ci=? string-ci>=? string-ci>?
	  string-copy			;from low-level
	  string-fill!
	  string<=? string<? string=? string>=? string>?
	  substring
	  truncate
	  vector vector->list vector-fill!
	  zero?))

(define-signature scheme-level-1-signature
  (compound-signature scheme-level-0-signature
		      scheme-level-1-adds-signature))

(define-signature util-signature
  (export any
	  every
	  filter
	  last
	  posq
	  posv
	  position
	  reduce			;command.scm
	  sublist
	  unspecific))

; Level 2 consists of harder things built on level 1.

(define-signature generics-signature
  (export define-default-method
	  define-method
	  disclose
	  disclose-methods
	  fail
	  make-family
	  make-generic
	  make-generic-exception-handler
	  make-method-table))

(define-signature number-i/o-signature
  (export number->string
	  number->string-table
	  really-number->string
	  really-string->number
	  string->integer
	  string->number
	  string->number-table))


(define-signature record-signature
  (export make-record-type
	  record-constructor
	  record-accessor
	  record-modifier
	  record-predicate
	  define-record-discloser))

(define-signature record-internal-signature
  (export record?
	  record-length
	  record-ref
	  record-set!
	  record-type?
	  record-type
	  record-type-field-names))

(define-signature define-record-types-signature
  (export (define-record-type syntax)
	  define-record-discloser))

(define-signature fluids-signature
  (export make-fluid
	  let-fluid
	  let-fluids
	  fluid
	  set-fluid!))

(define-signature fluids-internal-signature
  (export initialize-dynamic-state!
	  current-thread
	  set-current-thread!
	  get-dynamic-env		;thread.scm
	  set-dynamic-env!		;wind.scm
	  fluid-lookup))		;wind.scm

(define-signature enumerated-signature
  (export (define-enumeration syntax)
	  (enum syntax)
	  enumerand->name
	  name->enumerand))

(define-signature signals-signature
  (export error warn syntax-error call-error
	  signal signal-condition
	  make-condition))

(define-signature handle-signature
  (export ignore-errors with-handler))

(define-signature condition-signature
  (export define-condition-type
	  condition-predicate
	  condition-stuff condition-type
	  error? warning? syntax-error? call-error? read-error?
	  interrupt?

	  ;; Do these belong here?... not really.
	  exception-arguments
	  exception-opcode
	  exception?
	  make-exception))

(define-signature wind-signature
  (export call-with-current-continuation
	  dynamic-wind))

(define-signature port-signature
  (export call-with-input-file call-with-output-file
	  current-input-port current-output-port
	  with-input-from-file with-output-to-file
	  newline
	  input-port-option		;read.scm
	  output-port-option		;write.scm
	  write-string			;write.scm
	  with-initial-ports		;init.scm
	  error-output-port
	  force-output			;xport.scm
	  input-port?
	  output-port?))

(define-signature exception-signature
  (export define-exception-handler
	  initialize-exceptions!
	  make-opcode-generic!
	  signal-exception
	  continuation-preview		;env/debug.scm
	  really-signal-condition))	;rts/xprim.scm

(define-signature interrupts-signature
  (export initialize-interrupts!	;init.scm
	  interrupt-handlers		;thread.scm
	  interrupt?
	  set-enabled-interrupts!	;command.scm
	  enabled-interrupts
	  one-second
	  all-interrupts
	  with-interrupts-allowed
	  with-interrupts-inhibited
	  interrupt-before-heap-overflow!
	  schedule-interrupt
	  interrupt
	  (enum syntax)))

(define-signature writing-signature
  (export write
	  display
	  display-type-name
	  recurring-write))

(define-signature reading-signature
  (export read
	  define-sharp-macro		;command.scm
	  reading-error
	  gobble-line))

; Level 2: the harder stuff.

(define-signature scheme-level-2-adds-signature
  (export call-with-current-continuation
	  call-with-input-file call-with-output-file
	  current-input-port current-output-port
	  dynamic-wind			;New in R5RS
	  with-input-from-file with-output-to-file
	  number->string string->number
	  newline display write
	  read))

(define-signature scheme-level-2-signature
  (compound-signature scheme-level-1-signature
		      scheme-level-2-adds-signature))

(define-signature scheme-adds-signature
  (export eval load
	  interaction-environment
	  scheme-report-environment))

(define-signature scheme-signature
  (compound-signature scheme-level-2-signature
		      scheme-adds-signature))


; Stuff that comes for free with level 2.

(define-signature scheme-level-2-internal-signature
  (export usual-resumer))

(define-signature continuation-signature
  (export continuation-arg
	  continuation-arg-count
	  continuation-cont
	  continuation-env
	  continuation-overhead		;foo
	  continuation-pc
	  continuation-template
	  continuation?
	  continuation-parent))

(define-signature template-signature
  (export make-template
	  template-code
	  template-info
	  template-length
	  template-ref
	  template-overhead
	  set-template-code!
	  set-template-info!
	  template-set!
	  template?))

(define-signature weak-signature
  (export weak-pointer?			;disclosers.scm
	  weak-pointer-ref
	  make-weak-pointer
	  make-population
	  add-to-population!
	  population->list
	  walk-population))

(define-signature filenames-signature
  (export namestring *scheme-file-type* *load-file-type*
	  file-name-directory
	  file-name-nondirectory
	  translate
	  set-translation!
	  translations))


; Things for the compiler.

(define-signature table-signature
  (export make-table
	  table?
	  table-ref
	  table-set!
	  table-walk))

(define-signature usual-macros-signature
  (export usual-transform))

(define-signature meta-types-signature
  (export syntax-type
	  any-values-type
	  undeclared-type

	  make-some-values-type
	  some-values-type?
	  some-values-type-components
	  some-values-type        ; (some-values-type T1 ... Tn)

	  value-type

	  procedure-type
	  procedure-type-domain
	  procedure-type-codomain
	  procedure-type?
	  fixed-arity-procedure-type?
	  procedure-type-argument-types
	  procedure-type-arity
	  any-procedure-type
	  (proc syntax)

	  variable-type
	  variable-type?
	  variable-value-type

	  compatible-types?
	  compatible-type-lists?

	  boolean-type
	  char-type
	  number-type
	  pair-type
	  unspecific-type
	  usual-variable-type))

(define-signature syntactic-signature
  (export $source-file-name
	  binding?
	  binding-place
	  clobber-binding!
	  binding-static
	  binding-type
	  binding-transform
	  make-binding
	  desyntaxify
	  forget-integration
	  impose-type
	  literal?
	  generate-name			;package.scm, for accesses
	  generated-env
	  generated-symbol
	  generated-parent-name
	  generated?
	  get-operator
	  make-operator-table
	  make-transform
	  maybe-transform-call
	  n-ary?
	  name->symbol
	  name-hash			;package.scm
	  name?				;assem.scm
	  normalize-formals
	  number-of-required-args
	  operator-name
	  operator-table-ref
	  operator-define!
	  operator-lookup
	  operator-transform
	  operator-type
	  operator-uid
	  operator?
	  operators-table		;config.scm
	  process-syntax
	  set-operator-transform!
	  same-denotation?		;for redefinition messages
	  syntax?
	  transform-env transform-aux-names ;link/reify.scm
	  transform-source transform-id   ;link/reify.scm
	  transform-type
	  transform?
	  unbound?
	  schemify))

(define-signature nodes-signature
  (export classify
	  make-node
	  ;; name-node-binding
	  node?
	  node-operator
	  node-operator-id
	  node-form
	  node-ref
	  node-set!
	  node-predicate
	  make-similar-node
	  scan-body
	  lookup
	  bind
	  bind1
	  bind-evaluator-for-syntax))


; Signatures.

(define-signature signatures-signature
  (export make-compound-signature
	  make-simple-signature
	  note-reference-to-signature!
	  signature-ref
	  signature?
	  signature-clients
	  for-each-declaration))


; Packages.

(define-signature packages-signature
  (export make-package
	  make-simple-package		;start.scm
	  make-structure
	  package-define!
	  package-lookup
	  package?			;command.scm
	  structure-lookup		;env.scm
	  generic-lookup		;inline.scm
	  package-lookup-type		;reify.scm
	  structure-signature		;config.scm
	  package->environment
	  structure?
	  package-uid			;reifier
	  make-new-location		;ctop.scm
	  package-note-caching
	  structure-package))

(define-signature packages-internal-signature
  (export package-loaded?		;env/load-package.scm
	  set-package-loaded?!		;env/load-package.scm
	  package-name
	  set-package-name!		;env/pacman.scm
	  flush-location-names
	  package-name-table		;debuginfo
	  location-info-table		;debuginfo, disclosers
	  package-unstable?		;env/pacman.scm
	  package-integrate?		;env/debug.scm
	  package-get
	  package-put!

	  ;; For linker
	  initialize-reified-package!	;reify.scm

	  ;; For scanner
	  for-each-export
	  package-accesses
	  package-clauses
	  package-evaluator
	  package-file-name
	  package-for-syntax
	  package-opens
	  set-package-integrate?!
	  structure-name

	  ;; For package mutation
	  for-each-definition
	  set-package-opens!
	  set-structure-signature!
	  set-package-opens-thunk!
	  structure-clients
	  package-opens-thunk
	  package-opens-really
	  structure-signature-really
	  make-new-location
	  $get-location
	  set-package-get-location!
	  initialize-structure!
	  initialize-package!

	  package-cached
	  package-definitions
	  package-clients))

(define-signature scan-signature
  (export scan-forms
	  scan-file
	  scan-structures		;load-package.scm, link/link.scm
	  scan-package
	  set-optimizer!
	  $note-file-package
	  noting-undefined-variables
	  $note-undefined		;env/command.scm
	  note-undefined!))

(define-signature segments-signature
  (export attach-label
	  byte-limit
	  empty-segment
	  instruction
	  instruction-using-label
	  instruction-with-literal
	  instruction-with-location
	  instruction-with-template
	  make-label
	  note-environment
	  note-source-code
	  segment->template
	  segment-size
	  sequentially))

(define-signature compiler-signature
  (export compile
	  compile-and-run-file		;for LOAD
	  compile-and-run-forms		;for EVAL
	  compile-and-run-scanned-forms ;for eval.scm / ensure-loaded
	  compile-file			;link/link.scm
	  compile-form			;link/link.scm
	  compile-scanned-forms		;link/link.scm
	  make-startup-procedure	;link/link.scm
	  package-undefineds		; env/pedit.scm
	  package-undefined-but-assigneds
	  location-for-reference
	  ;; (*type-check?* (variable boolean))
	  define-compilator		;assem.scm
	  deliver-value			;assem.scm
	  get-location			;assem.scm
	  environment-level		;assem.scm
	  bind-vars			;assem.scm
	  ))

(define-signature debug-data-signature
  (export debug-data-env-maps
	  debug-data-name
	  debug-data-names
	  debug-data-parent
	  debug-data-pc-in-parent
	  debug-data-source
	  debug-data-table
	  debug-data-uid
	  debug-data?
	  get-debug-data
	  make-debug-data
	  template-debug-data
	  template-id
	  template-name
	  template-names

	  debug-flag-names
	  debug-flag-accessor
	  debug-flag-modifier
	  with-fresh-compiler-state))  ;for linker

(define-signature evaluation-signature
  (export eval load eval-from-file eval-scanned-forms))

(define-signature environments-signature  ;cheesy
  (export *structure-ref
	  environment-define!
	  environment-ref
	  environment-set!
	  interaction-environment
	  make-package-for-syntax
	  scheme-report-environment
	  set-interaction-environment!
	  set-scheme-report-environment!
	  with-interaction-environment))

(define-signature defpackage-signature
  (export ((define-package 
	    define-signature
	    define-structure
	    define-module
	    define-syntax
	    ;; compound-signature
	    ;; export
	    begin)
	   syntax)
	  signature-of
	  init-defpackage!
	  set-verify-later!))

(define-signature types-signature
  (export (syntax type)
	  (any-values type)
	  some-values			; (some-values T1 ... Tn)
	  (value type)
	  (variable (proc (type) type))         ; (variable T)
	  (procedure (proc (type type) type))   ; (procedure T1 T2)
	  (proc syntax)			; (proc (T1 ... Tn) T)

	  ((boolean
	    number
	    pair
	    unspecific
	    char
	    structure)		;etc. etc.
	   type)

	  type))		;Holy stratification, Batman!


; VM architecture

(define-signature architecture-signature
  (export (enum syntax) ;so you don't have to remember to open enumerated
	  bits-used-per-byte
	  interrupt
	  interrupt-count
	  memory-status-option
	  memory-status-option-count
	  op
	  op-count
	  opcode-arg-specs
	  stob
	  stob-count
	  stob-data
	  time-option
	  time-option-count))

(define-signature display-conditions-signature
  (export display-condition		;command.scm
	  disclose-condition-methods	;misc/disclosers.scm
	  limited-write))


(define-signature inline-signature
  (export inline-transform
	  name->qualified qualified->name qualified?
	  substitute))

; Bindings needed by the form composed by REIFY-STRUCTURES.

(define-signature for-reification-signature
  (compound-signature
    packages-signature
    (export ;; From usual-macros
	    usual-transform
	    ;; From syntactic
	    get-operator
	    make-transform
	    ;; From inline
	    inline-transform)))

;(define-signature command-signature ...)  -- moved to more-signatures.scm
