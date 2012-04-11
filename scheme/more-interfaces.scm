; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; Interfaces for packages that can get loaded after the initial.image
; starts up.

; Command processor

(define-interface command-processor-interface
  (export $write-length $write-depth
	  abort-to-command-level
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
	  focus-object
	  pop-command-level
	  read-command			;take
	  read-command-carefully	;inspect
	  read-form
	  run-sentinels
	  set-focus-object!
	  showing-focus-object		;inspect
	  start-command-processor
	  value->expression		;foo
	  write-carefully
	  write-line
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
	  ))

(define-interface command-levels-interface
  (export start-command-levels
	  with-new-session
	  command-levels
	  top-command-level
	  push-command-level
	  throw-to-command-level
	  restart-command-level
	  proceed-with-command-level
	  kill-paused-thread!

	  make-user-context
	  user-context
	  user-context-accessor
	  user-context-modifier

	  push-command-levels?

	  command-input
	  command-output
	  command-error-output
	  focus-values
	  set-focus-values!
	  batch-mode?
	  set-batch-mode?!
	  break-on-warnings?
	  set-break-on-warnings?!

	  reset-command-input?  ; condition predicate 

	  repl-data set-repl-data!

	  terminate-command-processor!
	  command-level
	  command-level-threads
	  command-level-paused-thread
	  command-level-repl-data
	  command-level?))

(define-interface basic-commands-interface
  (export exit
	  exit-when-done
          go
	  load
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
	  where))
       
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
	  level
	  condition
	  batch
	  bench
	  break-on-warnings
	  form-preferred
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
	  expand))

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
	  debug-data-names))

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
	  &floor
	  &numerator
	  &denominator
	  &exp &log
	  &sin &cos &tan &asin &acos &atan
	  &sqrt
	  &make-rectangular
	  &number->string
	  &really-string->number))

(define-interface bignums-interface  ;Things used by bigbit
  (export integer->bignum bignum-magnitude bignum-sign
	  zero-magnitude?
	  integer->magnitude
	  adjoin-digit
	  low-digit high-digits
	  make-integer
	  zero-magnitude
	  radix log-radix
	  integer-negate integer- integer=))

(define-interface time-interface
  (export real-time
	  run-time))

; Experimental DEFINE-RECORD-TYPE that is now officially a failure.

(define-interface defrecord-interface  ;RK's
  (export (define-record-type :syntax)
	  define-record-discloser))

; --------------------
; Big Scheme

(define-interface externals-interface
  (export get-external
	  lookup-all-externals
	  external-call
	  null-terminate))

(define-interface dump/restore-interface
  (export dump
	  restore
	  note-location!
	  $dump-index
	  $restore-index))

(define-interface extended-ports-interface
  (export make-tracking-input-port make-tracking-output-port
	  make-string-input-port
	  call-with-string-output-port
	  write-one-line
	  current-row current-column fresh-line))

(define-interface arrays-interface
  (export make-array		; <initial-value> <bound1> ...
	  array-shape		; <array>
	  array-ref		; <array> <index1> ...
	  array-set!		; <array> <value> <index1> ...
	  make-shared-array	; <array> <linear-map> <bound1> ...
	  copy-array		; <array>
	  array->vector		; <array>
	  array))		; <bounds> . <elements>

(define-interface search-trees-interface
  (export make-search-tree
          search-tree-ref
          search-tree-set!
          search-tree-modify!
          search-tree-max pop-search-tree-max!
          search-tree-min pop-search-tree-min!
          walk-search-tree))

(define-interface big-util-interface       
  (export concatenate-symbol
	  error breakpoint
	  atom? null-list? neq? n=
	  identity no-op
	  memq? first any? any every?
	  filter filter! filter-map partition-list partition-list!
	  remove-duplicates delq delq! delete
	  reverse!))

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

(define-interface placeholder-interface
  (export make-placeholder
	  placeholder?
	  placeholder-value
	  placeholder-set!))

(define-interface sicp-interface
  (export and or (sequence :syntax)
	  mapcar mapc 1+ -1+ t nil atom? print princ prin1 error
	  (cons-stream :syntax) head tail the-empty-stream empty-stream?
	  explode implode get put))

