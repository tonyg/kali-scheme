; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Some interfaces.  Order of presentation is a bit random.

(define-interface scheme-level-0-interface
  (export ((if begin lambda letrec quote set!
	       define define-syntax let-syntax letrec-syntax)
	   :syntax)
	  
	  ;; The basic derived expression types.
	  ((and cond do let let* or) :syntax)

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

(define-interface primitives-interface
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
	  force-output			;ports re-exports this.
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
	  write-string))		;ports

(define-interface bitwise-interface
  (export arithmetic-shift
	  bitwise-not bitwise-and bitwise-ior bitwise-xor))

(define-interface locations-interface
  (export location?
	  location-defined?
	  location-id
	  set-location-id!
	  make-location			;run.scm ? mini-packages.scm ?
	  make-undefined-location	;defined in base.scm
	  set-location-defined?!
	  contents
	  set-contents!))

(define-interface closures-interface
  (export closure?
	  make-closure
	  closure-env
	  closure-template))

(define-interface code-vectors-interface
  (export code-vector?
	  code-vector-length
	  code-vector-ref
	  code-vector-set!
	  make-code-vector))

; Miscellaneous useful primitives used in various random places.

(define-interface features-interface
  (export force-output
	  immutable?
	  make-immutable!
	  string-hash))


; Another hodge-podge.

(define-interface low-level-interface
  (export vector-unassigned?		; inspector
	  flush-the-symbol-table!	;build.scm
	  maybe-open-input-file		;mobot system
	  maybe-open-output-file))

(define-interface vm-exposure-interface
  (export invoke-closure		;eval
	  primitive-catch))		;shadowing, start, command

(define-interface escapes-interface
  (export primitive-cwcc
	  with-continuation))

(define-interface ascii-interface
  (export ascii->char
	  char->ascii
	  ascii-limit
	  ascii-whitespaces))


; Level 1: The rest of Scheme except for I/O.

(define-interface scheme-level-1-adds-interface
  (export ((case delay quasiquote syntax-rules) :syntax)
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

(define-interface scheme-level-1-interface
  (compound-interface scheme-level-1-adds-interface
		      scheme-level-0-interface))

(define-interface util-interface
  (export any
	  every
	  filter
	  last
	  posq
	  posv
	  position
	  reduce
	  sublist
	  insert
	  unspecific))

; Level 2 consists of harder things built on level 1.

(define-interface records-interface
  (export make-record-type
	  record-constructor
	  record-accessor
	  record-modifier
	  record-predicate
	  define-record-discloser
	  record-type?
	  record-type-field-names
	  record-type-name))

(define-interface records-internal-interface
  (export record?
	  record-type
	  record-length
	  record-ref
	  record-set!
	  :record-type
	  disclose-record))

(define-interface define-record-types-interface
  (export (define-record-type :syntax)
	  define-record-discloser))

(define-interface methods-interface
  (export (define-generic :syntax)
	  (define-method :syntax)
	  (define-simple-type :syntax)
	  :values
	  :value
	  :number
	  :complex
	  :real
	  :rational
	  :integer
	  :exact-integer
	  :boolean
	  :symbol
	  :char
	  :null
	  :pair
	  :vector
	  :string
	  :procedure
	  :input-port
	  :output-port
	  :eof-object
	  :record
	  :record-type
	  :zero
	  singleton
	  disclose &disclose))

(define-interface meta-methods-interface
  (export type-predicate &type-predicate
	  type-priority  &type-priority
	  type-superiors &type-superiors
	  same-type?	 &same-type?
	  add-method!	 &add-method!
	  set-final-method!
	  make-method-table         ;rts/exception.scm
	  method-table-get-perform  ;rts/exception.scm
	  make-simple-type
	  ;; Hooks for DOODL or other future extension
	  :simple-type :method-info :method-table :singleton
	  make-generic
	  (method-info :syntax)
	  methods->perform))

(define-interface number-i/o-interface
  (export number->string &number->string
	  string->number
	  really-string->number &really-string->number ;exactness argument
	  string->integer))


(define-interface fluids-interface
  (export make-fluid
	  let-fluid
	  let-fluids
	  fluid
	  set-fluid!))

(define-interface fluids-internal-interface
  (export initialize-dynamic-state!
	  current-thread
	  set-current-thread!
	  get-dynamic-env		;thread.scm
	  set-dynamic-env!		;wind.scm
	  fluid-lookup))		;wind.scm

(define-interface enumerated-interface
  (export (define-enumeration :syntax)
	  (enum :syntax)
	  (enumerand->name :syntax)
	  (name->enumerand :syntax)))

(define :enumeration :syntax)

(define-interface signals-interface
  (export error warn syntax-error call-error
	  signal signal-condition
	  make-condition))

(define-interface handle-interface
  (export ignore-errors with-handler))

(define-interface conditions-interface
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

(define-interface wind-interface
  (export call-with-current-continuation
	  dynamic-wind))

(define-interface ports-interface
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

(define-interface exceptions-interface
  (export define-exception-handler
	  initialize-exceptions!
	  make-opcode-generic!
	  signal-exception
	  continuation-preview		;env/debug.scm
	  really-signal-condition))	;rts/xprim.scm

(define-interface interrupts-interface
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
	  (enum :syntax)))

(define-interface writing-interface
  (export write
	  display
	  display-type-name
	  recurring-write))

(define-interface reading-interface
  (export read
	  define-sharp-macro		;command.scm
	  reading-error
	  gobble-line))

; Level 2: the harder stuff.
; Various primitives get promoted to n-ary at this point.

(define-interface scheme-level-2-adds-interface
  (export ((+ - * /)     (proc (&rest :number) :number))
	  ((< = > <= >=) (proc (&rest :number) :boolean))
	  (make-vector	 (proc (:exact-integer &opt :value) :vector))
	  (make-string	 (proc (:exact-integer &opt :char) :string))
	  ((write-char)  (proc (:char &opt :output-port) :unspecific))
	  ((read-char peek-char char-ready?)
	   (proc (&opt :input-port) :value))
	  call-with-current-continuation
	  call-with-input-file call-with-output-file
	  current-input-port current-output-port
	  dynamic-wind			;New in R5RS
	  with-input-from-file with-output-to-file
	  number->string string->number
	  newline display write
	  read))

(define-interface scheme-level-2-interface
  (compound-interface scheme-level-2-adds-interface
		      scheme-level-1-interface))

(define-interface scheme-adds-interface
  (export eval load
	  interaction-environment
	  scheme-report-environment))

(define-interface scheme-interface
  (compound-interface scheme-adds-interface
		      scheme-level-2-interface))


; Stuff that comes for free with level 2.

(define-interface scheme-level-2-internal-interface
  (export usual-resumer))

(define-interface continuations-interface
  (export continuation-arg
	  continuation-arg-count
	  continuation-cont
	  continuation-env
	  continuation-overhead		;foo
	  continuation-pc
	  continuation-template
	  continuation-parent
	  continuation?
	  :continuation))

(define-interface templates-interface
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

(define-interface weak-interface
  (export weak-pointer?			;disclosers.scm
	  weak-pointer-ref
	  make-weak-pointer
	  make-population
	  add-to-population!
	  population->list
	  walk-population))

(define-interface filenames-interface
  (export namestring *scheme-file-type* *load-file-type*
	  file-name-directory
	  file-name-nondirectory
	  translate
	  set-translation!
	  translations))


; Things for the compiler.

(define-interface general-tables-interface
  (export make-table
	  make-string-table
	  make-symbol-table
	  make-integer-table
	  make-table-maker
	  table?
	  table-ref
	  table-set!
	  table-walk
	  make-table-immutable!))

(define-interface usual-macros-interface
  (export usual-transform))

(define-interface meta-types-interface
  (export same-type?
	  subtype?
	  meet?
	  join-type
	  meet-type
	  sexp->type type->sexp rail-type->sexp
	  syntax-type
	  any-values-type
	  any-arguments-type
	  value-type
	  error-type

	  make-some-values-type

	  empty-rail-type
	  rail-type
	  make-optional-type
	  make-rest-type
	  empty-rail-type?
	  rest-type?
	  head-type
	  tail-type

	  procedure-type
	  procedure-type-domain
	  procedure-type-codomain
	  restrictive?
	  procedure-type?
	  fixed-arity-procedure-type?
	  procedure-type-argument-types
	  procedure-type-arity
	  any-procedure-type
	  (proc :syntax)
	  (some-values :syntax)

	  boolean-type
	  char-type
	  null-type
	  unspecific-type

	  exact-integer-type
	  integer-type
	  rational-type
	  real-type
	  complex-type
	  number-type
	  exact-type
	  inexact-type

	  pair-type
	  string-type
	  symbol-type
	  vector-type

	  escape-type
	  structure-type))

(define-interface syntactic-interface
  (export binding?
	  binding-place
	  clobber-binding!
	  binding-static
	  binding-type
	  make-binding
	  desyntaxify
	  forget-integration
	  impose-type
	  literal?
	  generate-name			;package.scm, for accesses
	  generated-env
	  generated-parent-name
	  generated-symbol
	  generated?
	  get-operator
	  make-qualified
	  make-operator-table
	  make-transform
	  maybe-transform-call
	  n-ary?
	  name->qualified
	  name->symbol
	  name-hash			;package.scm
	  name?				;assem.scm
	  normalize-formals
	  number-of-required-args
	  operator-name
	  operator-table-ref
	  operator-define!
	  operator-lookup
	  operator-type
	  operator-uid
	  operator?
	  operators-table		;config.scm
	  process-syntax
	  qualified?
	  qualified-parent-name
	  qualified-symbol
	  set-binding-place!
	  same-denotation?		;for redefinition messages
	  syntax?
	  transform-env transform-aux-names ;link/reify.scm
	  transform-source transform-id   ;link/reify.scm
	  transform-type
	  transform?
	  unbound?
	  schemify
	  get-funny
	  reflective-tower 
	  funny-name/reflective-tower
	  ;; Stuff moved from meta-types
	  variable-type
	  variable-type?
	  variable-value-type
	  usual-variable-type
	  compatible-types?
	  undeclared-type
	  bind-source-file-name
	  funny-name/reader
	  environment-reader))

(define-interface nodes-interface
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
	  define-node?  
	  define-syntax-node?
	  scan-body
	  lookup
	  bind
	  bind1))


; Interfaces.

(define-interface interfaces-interface
  (export make-compound-interface
	  make-simple-interface
	  note-reference-to-interface!
	  interface-ref
	  interface?
	  interface-clients
	  for-each-declaration
	  note-interface-name!))


; Packages.

(define-interface packages-interface
  (export make-package
	  make-simple-package		;start.scm
	  make-structure
	  package-define!
	  package-lookup
	  package?			;command.scm
	  structure-lookup		;env.scm
	  generic-lookup		;inline.scm
	  structure-interface		;config.scm
	  package->environment
	  structure?
	  package-uid			;reifier
	  make-new-location		;ctop.scm
	  package-note-caching
	  structure-package
	  extract-package-from-environment
	  package-define-funny!
	  note-structure-name!
	  (:package :type)
	  (:structure :type)))		;for (define-method ...)'s

(define-interface packages-internal-interface
  (export package-loaded?		;env/load-package.scm
	  set-package-loaded?!		;env/load-package.scm
	  package-name
	  flush-location-names
	  package-name-table		;debuginfo
	  location-info-table		;debuginfo, disclosers
	  package-unstable?		;env/pacman.scm
	  package-integrate?		;env/debug.scm
	  package-get
	  package-put!

	  ;; For scanner
	  for-each-export
	  package-accesses
	  package-clauses
	  package-file-name
	  package-opens
	  set-package-integrate?!
	  structure-name

	  ;; For package mutation
	  for-each-definition
	  set-package-opens!
	  set-structure-interface!
	  set-package-opens-thunk!
	  structure-clients
	  package-opens-thunk
	  package-opens-really
	  structure-interface-really
	  make-new-location
	  $get-location
	  set-package-get-location!
	  initialize-structure!
	  initialize-package!

	  extract-package-from-environment ;foo
	  package-cached
	  package-definitions
	  package-clients))

(define-interface scan-interface
  (export scan-forms
	  scan-file
	  scan-structures		;load-package.scm, link/link.scm
	  scan-package
	  set-optimizer!
	  $note-file-package
	  noting-undefined-variables
	  $note-undefined		;env/command.scm
	  note-undefined!
	  check-structure))

(define-interface segments-interface
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

(define-interface compiler-interface
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
	  set-type-check?!		; for timings
	  define-compilator		;assem.scm
	  deliver-value			;assem.scm
	  get-location			;assem.scm
	  environment-level		;assem.scm
	  bind-vars			;assem.scm
	  ))

(define-interface debug-data-interface
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

(define-interface evaluation-interface
  (export eval load load-into eval-from-file eval-scanned-forms))

(define-interface environments-interface  ;cheesy
  (export *structure-ref
	  environment-define!
	  environment-ref
	  environment-set!
	  interaction-environment
	  make-reflective-tower
	  scheme-report-environment
	  set-interaction-environment!
	  set-scheme-report-environment!
	  with-interaction-environment
	  set-reflective-tower-maker!))

(define-interface defpackage-interface
  (export ((def			        ;Formerly define-structure
	    define-interface		;Formerly define-signature
	    define-structures		;Formerly define-package
	    define-structure
	    define-module
	    define-syntax
	    define-reflective-tower-maker
	    export-reflective-tower-maker
	    compound-interface
	    export
	    structure structures let	; New
	    begin			; mostly for macros
	    define			; for compatibility?
	    a-package)			; cf. env/init-defpackage.scm
	   :syntax)
	  values
	  interface-of
	  set-verify-later!))

(define-interface types-interface
  (export ((:syntax :values :arguments :value) :type)
	  (procedure (proc (:type :type) :type))  ; (procedure T1 T2)
	  (proc :syntax)		; (proc (T1 ... Tn) T)
	  (some-values :syntax)		; (some-values T1 ... Tn)
	  ((:boolean
	    :char
	    :number
	    :complex :real :rational :integer :exact-integer
	    :null
	    :unspecific
	    :pair
	    :string
	    :symbol
	    :vector
	    :procedure
	    :input-port :output-port
	    :error
	    :escape)
	   :type)
	  (:structure :type)
	  :type))		;Holy stratification, Batman!


; VM architecture

(define-interface architecture-interface
  (export (enum :syntax)  ;So you don't have to remember to open ENUMERATED
	  (enumerand->name :syntax)
	  (name->enumerand :syntax)
	  bits-used-per-byte
	  (interrupt :enumeration)
	  interrupt-count
	  (memory-status-option :enumeration)
	  memory-status-option-count
	  (op :enumeration)
	  op-count
	  opcode-arg-specs
	  (stob :enumeration)
	  stob-count
	  stob-data
	  (time-option :enumeration)
	  time-option-count))

(define-interface display-conditions-interface
  (export display-condition		;command.scm
	  &disclose-condition      	;env/disclosers.scm
	  limited-write))


(define-interface inline-interface
  (export inline-transform
	  ;; name->qualified qualified?
	  qualified->name
	  substitute))

; Bindings needed by the form composed by REIFY-STRUCTURES.

(define-interface for-reification-interface
  (export operator
	  simple-interface
	  package
	  transform
	  ;; From usual-macros
	  usual-transform
	  ;; From inline
	  inline-transform
	  ;; From packages
	  package-define!
	  make-structure
	  ))

;(define-interface command-interface ...)  -- moved to more-interfaces.scm
