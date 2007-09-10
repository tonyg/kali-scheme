; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Interfaces for packages that can get loaded after the initial.image
; starts up.

; Command processor

(define-interface command-processor-interface
  (export abort-to-command-level
	  add-sentinel!
	  command-continuation
	  command-threads
	  command-loop
	  command-level-condition
	  command-processor
	  error-form ;foo
	  execute-command
	  exit-command-processor
	  evaluate-and-select
	  gobble-line
	  greet-user
	  environment-for-commands
	  pop-command-level
	  read-command			;take
	  read-command-carefully	;inspect
	  read-form
	  run-sentinels
	  set-command-results!
	  start-command-processor
	  restart-command-processor
	  value->expression		;foo
	  y-or-n?
	  help
	  define-command-syntax
          define-user-command-syntax
	  user-command-environment
	  ;; set-command-structure!        ;startup
	  ;; command-structure             ;pacman
          set-user-command-environment! ;pacman
          read-command-error            ;inspect
	  &environment-id-string
	  ;&evaluate

	  ; Settings
	  add-setting
	  lookup-setting
	  setting-boolean?
	  setting-value
	  setting-set!
	  setting-doc
	  list-settings
	  ))

(define-interface command-levels-interface
  (export start-command-levels
	  command-levels
	  top-command-level
	  push-command-level
	  throw-to-command-level
	  restart-command-level
	  proceed-with-command-level
	  kill-paused-thread!

	  reset-command-input?  ; condition predicate 

	  terminate-command-processor!
	  command-level
	  command-level-threads
	  command-level-condition
	  command-level-paused-thread
	  command-level?))

(define-interface command-state-interface
  (export user-context
	  user-context-accessor
	  user-context-modifier

	  push-command-levels?
	  set-push-command-levels?!

	  focus-object
	  set-focus-object!
	  pop-value-stack!

	  command-input
	  command-output
	  command-error-output
	  batch-mode? set-batch-mode?!
	  break-on-warnings? set-break-on-warnings?!
	  load-noisily? set-load-noisily?!
	  trace-writing-depth set-trace-writing-depth!
	  inspector-menu-limit set-inspector-menu-limit!
	  inspector-writing-depth set-inspector-writing-depth!
	  inspector-writing-length set-inspector-writing-length!
	  condition-writing-depth set-condition-writing-depth!
	  condition-writing-length set-condition-writing-length!
	  translations set-translations!

	  maybe-menu
	  set-menu!
	  menu-position
	  set-menu-position!
	  value-stack
	  set-value-stack!
	  pop-value-stack!))

(define-interface menus-interface
  (export present-menu
	  present-more-menu
	  current-menu-length
	  current-menu-ref
	  
	  selection-command?

	  write-line
	  write-carefully
	  with-limited-output))

(define-interface basic-commands-interface
  (export exit
	  exit-when-done
          go
	  load
	  load-script
          help
          run
	  ?))
   
(define-interface build-commands-interface
  (export dump
          build))
       
(define-interface inspect-commands-interface
  (export inspect
	  debug
	  threads
	  where

	  menu
	  m
	  q
	  u
	  d
	  template
	  select-menu-item))
       
(define-interface disassemble-commands-interface
  (export dis))
   
(define-interface profile-commands-interface
  (export profile))
   
(define-interface package-commands-interface
  (export in
	  new-package
	  load-package
	  reload-package
	  structure
	  open
	  for-syntax
	  exec
	  user
	  user-package-is
	  config
	  config-package-is
	  undefine))

(define-interface debug-commands-interface
  (export translate
	  preview
	  proceed
	  push
	  pop
          reset
	  resume
	  level
	  condition
	  set
	  unset
	  batch
	  bench
	  break-on-warnings
	  levels
	  flush
	  keep
          collect
	  trace
	  untrace
	  time
          from-file
	  forget
	  bound?
	  expand
	  expand-once))

(define-interface usual-commands-interface
  (compound-interface
   basic-commands-interface
   build-commands-interface
   package-commands-interface
   debug-commands-interface
   inspect-commands-interface
   disassemble-commands-interface
   ;profile-commands-interface
   ))

(define-interface package-commands-internal-interface
  (export config-package
	  new-command-processor
	  get-structure
	  ;get-package
	  ;set-package-evaluator!
	  ))

(define-interface display-conditions-interface
  (export display-condition		;command.scm
	  &disclose-condition      	;env/disclosers.scm
	  limited-write))

(define-interface debuginfo-interface
  (export read-debug-info
	  write-debug-info))

(define-interface disclosers-interface
  (export make-print-name
	  template-file-name
	  value->expression
	  error-form
	  location-info
	  location-name
	  template-debug-data
	  template-id
	  template-name
	  template-names
	  debug-data-names
	  disclose-vm-condition))

(define-interface package-mutation-interface
  (export package-system-sentinel	;env/command.scm
	  package-open!			;env/debug.scm
	  package-undefine!
	  ))

(define-interface packages-cruft-interface
  (export assume-denotation
	  ;; new-location-uid    ;?
	  interface-ref
	  structure-interface
	  verify-package		;for debugging the package system
	  ))


; --------------------
; Linker

(define-interface linker-interface
  (export link-simple-system
	  link-reified-system
	  link-semireified-system
	  (struct-list :syntax)
	  compile-structures))

(define-interface expander-interface
  (export expand-form
	  expand-stuff
	  expand
	  usage-reference-count
	  usage-operator-count
	  usage-assignment-count
	  free-top-level-variables))

; --------------------
; Extended numbers: bignums, ratnums, etc.

(define-interface extended-numbers-interface
  (export (define-extended-number-type :syntax)
	  (define-opcode-extension :syntax)
	  :exact :inexact
	  string-position
	  &+
	  &-
	  &*
	  &/
	  &=
	  &<
	  &quotient
	  &remainder
	  &integer?
	  &rational?
	  &real?
	  &complex?
	  &number?
	  &exact?
	  &exact->inexact
	  &inexact->exact
	  &real-part
	  &imag-part
	  &magnitude
	  &angle
	  &floor
	  &numerator
	  &denominator
	  &exp &log
	  &sin &cos &tan &asin &acos &atan1 &atan2
	  &sqrt
	  &make-rectangular
	  &number->string
	  &really-string->number))

(define-interface time-interface
  (export real-time
	  run-time))

(define-interface unicode-char-maps-interface
  (export (primary-category :syntax)
	  primary-category?

	  (general-category :syntax)
	  general-category?
	  general-category-id
	  general-category-symbol
	  general-category-primary-category

	  char-general-category
	  char-titlecase
	  char-title-case?
	  char-foldcase

	  string-upcase string-downcase
	  string-foldcase
	  string-titlecase))

(define-interface conditions-interface
  (export make-condition-type
	  condition-type?
	  make-condition
	  condition?
	  condition-has-type?
	  condition-ref
	  make-compound-condition
	  extract-condition

	  (define-condition-type :syntax)
	  (condition :syntax)

	  &condition
	  &message message-condition? condition-message
	  &serious serious-condition?
	  &error error?
	  &bug bug?
	  &irritants irritants?
	  condition-irritants
	  &call-error call-error?
	  call-error-proc call-error-args
	  &vm-exception vm-exception?
	  vm-exception-opcode vm-exception-reason vm-exception-arguments
	  &warning warning?
	  &note note?
	  &syntax-error syntax-error?
	  &interrupt interrupt?
	  interrupt-type
	  &decoding-error decoding-error?
	  decoding-error-encoding-name
	  &simple-condition simple-condition?
	  simple-condition-type simple-condition-stuff))

(define-interface i/o-conditions-interface
  (export &i/o-error i/o-error?
	  &i/o-port-error i/o-port-error?
	  i/o-error-port
	  &i/o-read-error i/o-read-error?
	  &i/o-write-error i/o-write-error?
	  &i/o-closed-error i/o-closed-error?
	  &i/o-filename-error i/o-filename-error?
	  i/o-error-filename
	  &i/o-malformed-filename-error i/o-malformed-filename-error?
	  &i/o-file-protection-error i/o-file-protection-error?
	  &i/o-file-is-read-only-error i/o-file-is-read-only-error?
	  &i/o-file-already-exists-error i/o-file-already-exists-error?
	  &i/o-no-such-file-error i/o-no-such-file-error?
	  &read-error read-error?
	  read-error-line read-error-column read-error-position read-error-span

	  &primitive-i/o-error primitive-i/o-error?
	  i/o-error-status i/o-error-operation i/o-error-arguments))

(define-interface signals-internal-interface
  (export simple-condition->condition
	  condition->simple-condition
	  coerce-to-condition
	  coerce-to-simple-condition))

; Experimental DEFINE-RECORD-TYPE that is now officially a failure.

(define-interface defrecord-interface  ;RK's
  (export (define-record-type :syntax)
	  define-record-discloser))

; Unicode
; -------

(define-interface text-codec-utils-interface
  (export guess-port-text-codec-according-to-bom
	  set-port-text-codec-according-to-bom!))

(define-interface unicode-normalizations-interface
  (export string-normalize-nfd
	  string-normalize-nfkd
	  string-normalize-nfc
	  string-normalize-nfkc))

; --------------------
; Big Scheme

(define-interface dynamic-externals-interface
  (export dynamic-load

          get-external
	  lookup-external
	  lookup-all-externals

	  external?
	  external-name
	  external-value

	  call-external))

(define-interface shared-objects-interface
  (export open-shared-object
	  close-shared-object
	  shared-object?
	  shared-object-name
	  shared-object-address shared-object-address-or-false
	  shared-object-address?
	  shared-object-address-value
	  call-shared-object-address))

(define-interface load-dynamic-externals-interface
  (export load-dynamic-externals
	  import-dynamic-externals
	  reload-dynamic-externals
	  unload-dynamic-externals))

(define-interface dump/restore-interface
  (export dump
	  restore
	  note-location!
	  $dump-index
	  $restore-index))

(define-interface extended-ports-interface
  (export byte-source->input-port char-source->input-port
	  byte-sink->output-port char-sink->output-port
	  make-tracking-input-port make-tracking-output-port
	  make-byte-vector-input-port make-string-input-port
	  make-byte-vector-output-port make-string-output-port
	  byte-vector-output-port-output string-output-port-output
	  limit-output
	  current-row current-column fresh-line

	  call-with-string-output-port		; deprecated
	  write-one-line))			; deprecated


(define-interface arrays-interface
  (export make-array		; <initial-value> <bound1> ...
	  array?
	  array-shape		; <array>
	  array-ref		; <array> <index1> ...
	  array-set!		; <array> <value> <index1> ...
	  make-shared-array	; <array> <linear-map> <bound1> ...
	  copy-array		; <array>
	  array->vector		; <array>
	  array))		; <bounds> . <elements>

(define-interface lu-decompositions-interface
  (export lu-decomposition))

(define-interface compact-tables-interface
  (export compute-compact-table))

(define-interface inversion-lists-interface
  (export make-empty-inversion-list
	  inversion-list?
	  inversion-list-member?
	  inversion-list-complement
	  inversion-list-union inversion-list-intersection
	  inversion-list-difference
	  number->inversion-list numbers->inversion-list
	  range->inversion-list ranges->inversion-list
	  inversion-list-adjoin inversion-list-remove
	  inversion-list-size
	  inversion-list-copy
	  inversion-list=?
	  inversion-list-hash
	  inversion-list-fold/done?
	  inversion-list-cursor?
	  inversion-list-cursor inversion-list-cursor-at-end?
	  inversion-list-cursor-next inversion-list-cursor-ref))

(define-interface mask-types-interface
  (export make-mask-type
	  mask-type?
	  mask-type
	  mask-has-type?
	  integer->mask
	  list->mask
	  mask?))

(define-interface masks-interface
  (export mask->integer
	  mask->list
	  mask-member?
	  mask-set
	  mask-clear
	  mask-union
	  mask-intersection
	  mask-subtract
	  mask-negate))

(define-interface enum-sets-interface
  (export (define-enum-set-type :syntax)
	  enum-set->list
	  enum-set-member?
	  enum-set=?
	  enum-set-union
	  enum-set-intersection
	  enum-set-negation))

(define-interface enum-sets-internal-interface
  (export enum-set-has-type?
	  enum-set?
	  enum-set-type
	  enum-set->integer
	  integer->enum-set))

(define-interface search-trees-interface
  (export make-search-tree
	  search-tree?
          search-tree-ref
          search-tree-set!
          search-tree-modify!
          search-tree-max pop-search-tree-max!
          search-tree-min pop-search-tree-min!
          walk-search-tree))

(define-interface sparse-vectors-interface
  (export make-sparse-vector
	  sparse-vector-ref sparse-vector-set!
	  sparse-vector->list))

(define-interface variable-argument-lists-interface
  (export (opt-lambda :syntax)))

; This is getting to be a hodge-podge.

(define-interface big-util-interface       
  (export concatenate-symbol
	  error breakpoint
	  atom? null-list? neq? n=
	  identity no-op
	  memq? first any? any every?
	  filter filter! filter-map partition-list partition-list!
	  remove-duplicates delq delq! delete
	  reverse!
	  string->immutable-string
	  ))

(define-interface big-scheme-interface
  (compound-interface
      (interface-of ascii)
      (interface-of bitwise)
      (interface-of tables)
      (interface-of enumerated)
      ;defrecord-interface
      extended-ports-interface
      big-util-interface
      (export (destructure :syntax)
	      (receive :syntax)
	      format
	      p pretty-print
	      sort-list sort-list!)))

; --------------------
; Miscellaneous

; Copied from interfaces.scm.
(define-interface define-record-types-interface
  (export (define-record-type :syntax)
	  define-record-discloser))

(define-interface parse-bytecode-interface
  (export byte-code?
          parse-template
          parse-template-code
          parse-instruction
          parse-protocol
          with-template
          make-attribution
          make-opcode-table
          opcode-table-set! opcode-table-ref
          protocol-protocol protocol-nargs n-ary-protocol? 
          protocol-cwv-tailcall? call-with-values-protocol-target
          env-data? env-data-total-count env-data-frame-offsets
          env-data-maybe-template-index env-data-closure-offsets 
          env-data-env-offsets
          cont-data? cont-data-length cont-data-mask-bytes cont-data-live-offsets cont-data-pc
          cont-data-template cont-data-gc-mask-size cont-data-depth))

(define-interface reinitializers-interface
  (export (define-reinitializer :syntax)))

(define-interface locks-interface
  (export lock?
	  make-lock
	  obtain-lock
	  maybe-obtain-lock
	  release-lock
	  with-lock
	  lock-owner))		;really should be internal

(define-interface value-pipes-interface
  (export make-pipe
	  empty-pipe?
	  pipe-read!
	  pipe-write!
	  pipe-push!
	  empty-pipe!
	  pipe-maybe-read!
	  pipe-maybe-read?!
	  pipe-maybe-write!))

(define-interface placeholder-interface
  (export make-placeholder
	  placeholder?
	  placeholder-value
	  placeholder-set!))

(define-interface test-suites-interface 
  (export ((define-test-suite define-test-case define-test-cases) :syntax)
	  ((check check-exception) :syntax)
	  run-test-suite
	  =within
	  zap-test-suite!))
