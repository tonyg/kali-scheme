; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Some signatures.  Order of presentation is a bit random.

; Procedures that are compiler primitives, but also belong to Scheme.

(define-signature scheme-primitives-signature
  (export (begin syntax)
	  (if syntax) (lambda syntax) (quote syntax)
	  (set! syntax)
	  (define syntax) (define-syntax syntax)
	  ;; let-syntax letrec-syntax  -- not yet implemented

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
	  values call-with-values))

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
	  string-hash
	  unspecific))


; Another hodge-podge.

(define-signature low-level-signature
  (export halt				;exception.scm

	  ;; Things re-exported by scheme-level-1
	  string-copy
	  string->symbol
	  procedure?
	  integer->char char->integer
	  close-output-port close-input-port
	  open-input-file open-output-file

	  ;; VM primitives that we re-export to modules that don't
	  ;; want to open the PRIMITIVES package.
	  unassigned			;LETREC macro
	  unspecific			;base.scm, util.scm
	  signal-condition error	;signal.scm

	  flush-the-symbol-table!	;build.scm
	  maybe-open-input-file		;mobot system
	  maybe-open-output-file
	  (structure-ref syntax)	;re-exported by defpackage
	  (%file-name% syntax)))

(define-signature vm-exposure-signature
  (export invoke-closure		;eval
	  primitive-catch))		;shadowing, start, command

(define-signature escapes-signature
  (export primitive-cwcc with-continuation))

(define-signature ascii-signature
  (export ascii->char
	  char->ascii
	  ascii-limit
	  ascii-whitespaces))


; Level 1: The rest of Scheme except for I/O.

(define-signature scheme-level-1-adds-signature
  (export (and syntax) (case syntax) (cond syntax)
	  (do syntax) (let syntax) (let* syntax) (or syntax)
	  (delay syntax)
	  (quasiquote syntax)
	  (letrec syntax)		;Hmm.
	  (syntax-rules syntax)

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
  (compound-signature scheme-primitives-signature
		      scheme-level-1-adds-signature))

(define-signature util-signature
  (export any
	  every
	  filter
	  posq
	  posv
	  position
	  reduce			;command.scm
	  sublist))

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

(define-signature shadowing-signature
  (export shadow-location!))


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
  (export for-each-usual-macro
	  usual-transform-procedure
	  find-free-names-in-syntax-rules))  ;link/reify.scm

(define-signature syntactic-signature
  (export $source-file-name
	  define-lhs
	  define-rhs 
	  define?
	  desyntaxify
	  literal?
	  generate-name			;package.scm, for accesses
	  generated-env
	  generated-symbol
	  generated-parent-name
	  generated?
	  get-operator
	  make-compilator-table
	  make-define
	  make-transform
	  n-ary?
	  name->symbol
	  name-hash			;package.scm
	  name?				;assem.scm
	  normalize-definition		;scan.scm
	  normalize-formals
	  number-of-required-args
	  operator-name
	  operator-transform
	  operator-type
	  operator-uid
	  operator?
	  operators-table		;config.scm
	  set-operator-transform!
	  same-denotation?		;for redefinition messages
	  syntax?
	  transform
	  transform-env transform-aux-names transform-source ;link/reify.scm
	  transform-type
	  transform?
	  unbound?))

; Things used in reification code
(define-signature packages-1-signature
  (export initialize-reified-package!
	  make-simple-package		;start.scm
	  make-simple-signature
	  make-structure
	  package-define!
	  package-define-location!
	  package-ensure-defined!
	  package-lookup
	  set-package-name!
	  structure-name
	  structure-package
	  transform-for-structure-ref))

(define-signature packages-2-signature	;Much too big.
  (export $note-undefined
	  check-structure
	  package-integrate?
	  flush-location-names
	  for-each-definition
	  location-name
	  location-name-table
	  location-package-name
	  make-compound-signature
	  make-package
	  new-location-uid
	  new-package-uid
	  noting-undefined-variables
	  package-accesses
	  package-check-assigned
	  package-check-variable
	  package-clauses
	  package-find-location		;env.scm
	  package-file-name		;scan.scm (scan-package)
	  package-for-syntax
	  package-evaluator
	  package-loaded?		;load-package.scm
	  package-lookup-location	;segment.scm
	  package-name			;command.scm
	  package-name-table		;debuginfo.scm
	  package-opens
	  package-open!
	  package-system-sentinel	;command.scm
	  package-uid			;eval.scm
	  package-unstable?		;pacman.scm
	  package?			;command.scm
	  probe-package
	  reinitialize-package!
	  reset-packages-state!
	  set-package-integrate?!	;debug.scm
	  set-package-loaded?!		;load-package.scm
	  set-shadow-action!		;eval.scm
	  signature-item-name
	  signature-item-type
	  signature-items
	  signature-ref
	  structure?
	  structure-signature
	  unbound?     ;re-exported from syntactic
	  undefined-exports
	  undefined-variables
	  verify-later!
	  verify-package))		;for debugging package system

(define-signature packages-signature
  (compound-signature packages-1-signature
		      packages-2-signature))

(define-signature scan-signature
  (export scan-forms
	  scan-file
	  scan-body
	  scan-structures		;load-package.scm, link/link.scm
	  scan-package
	  set-optimizer!
	  $note-file-package))

(define-signature compiler-signature
  (export clookup-assigned      	;assem.scm
	  clookup-variable		;assem.scm
	  compile-and-run-file		;for LOAD
	  compile-and-run-forms		;for EVAL
	  compile-file			;link/link.scm
	  compile-form			;link/link.scm
	  compile-scanned-forms		;link/link.scm
	  define-compilator		;assem.scm
	  deliver-value			;assem.scm
	  instruction			;assem.scm
	  instruction-with-literal	;assem.scm
	  instruction-with-template	;assem.scm
	  make-startup-procedure	;link/link.scm
	  segment-size			;assem.scm
	  sequentially))		;assem.scm

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
  (export eval load eval-from-file))

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

(define-signature scheme-adds-signature
  (export eval load
	  interaction-environment
	  scheme-report-environment))
	  

(define-signature scheme-signature
  (compound-signature scheme-level-2-signature
		      scheme-adds-signature))


(define-signature defpackage-signature
  (export (define-package syntax)
	  (define-signature syntax)
	  (define-structure syntax)
	  (define-module syntax)
	  (define-syntax syntax)
	  (compound-signature syntax)
	  (export syntax)
	  (begin syntax)
	  (letrec syntax)		;because of compiler bug
	  (structure-ref syntax)	;mumble
	  make-compiler-base
	  signature-of
	  init-defpackage!))


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
  (export make-procedure-for-inline-transform
	  really-make-procedure-for-inline-transform
	  name->extrinsic extrinsic->name extrinsic?
	  operator->oplet oplet->operator oplet?  ;Ugh
	  substitute))

; Bindings needed by the form composed by REIFY-STRUCTURES.

(define-signature for-reification-signature
  (compound-signature
    packages-1-signature
    (export ;; From usual-macros
	    usual-transform-procedure
	    ;; From syntactic
	    get-operator
	    make-transform
	    ;; From inline
	    make-procedure-for-inline-transform
	    ;; From Scheme
	    cons
	    lambda
	    let
	    quote
	    vector
	    vector-ref)))

;(define-signature command-signature ...)  -- moved to more-signatures.scm
