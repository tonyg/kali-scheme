; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Interfaces for packages that can get loaded after the initial.image
; starts up.

; Command processor

(define-interface command-processor-interface
  (export $command-levels
	  $write-length $write-depth
	  abort-to-command-level
	  add-sentinel!
	  command-input
	  command-level
	  command-level-condition
	  command-level-interrupts
	  command-level?
	  command-loop
	  command-continuation
	  command-output
	  focus-object
	  error-form ;foo
	  execute-command
	  evaluate			; ???
	  evaluate-and-select
	  exit-command-processor
	  gobble-line
	  greet-user
	  make-user-context
	  environment-for-commands
	  read-command			;take
	  read-command-carefully	;inspect
	  read-form
	  run-sentinels
	  ;; set-command-level-env!
	  set-focus-object!
	  set-focus-values!
	  showing-focus-object		;inspect
	  start-command-processor	;build
	  throw-to-command-level
	  top-command-level
	  user-context
	  user-context-accessor
	  user-context-modifier
	  value->expression		;foo
	  write-carefully
	  write-line
	  y-or-n?
	  break-on-warnings?
	  set-break-on-warnings?!
	  batch-mode?
	  set-batch-mode?!
	  help
	  define-command-syntax
          define-user-command-syntax
	  user-command-environment
	  ;; set-command-structure!        ;startup
	  ;; command-structure             ;pacman
          set-user-command-environment! ;pacman
          read-command-error            ;inspect
	  &environment-id-string
	  &evaluate))

(define-interface basic-commands-interface
  (export exit
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
	  where))
       
(define-interface disassemble-commands-interface
  (export dis))
   
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
   ))

(define-interface package-commands-internal-interface
  (export config-package
	  new-command-processor
	  get-structure
	  get-package
	  set-package-evaluator!))

(define-interface debuginfo-interface
  (export read-debug-info
	  write-debug-info))

(define-interface disclosers-interface
  (export make-print-name
	  template-file-name
	  value->expression
	  error-form
	  location-info
	  location-name))

(define-interface package-mutation-interface
  (export package-system-sentinel	;env/command.scm
	  package-open!			;env/debug.scm
	  package-undefine!
	  ))

(define-interface packages-cruft-interface
  (export 
	  assume-denotation
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
	  lexical-reference-count
	  lexical-operator-count
	  lexical-assignment-count))


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

; --------------------
; Big Scheme

(define-interface defrecord-interface  ;RK's
  (export (define-record-type :syntax)
	  define-record-discloser))

(define-interface externals-interface
  (export get-external
	  lookup-all-externals
	  external-call
	  null-terminate))

(define-interface queues-interface
  (export make-queue enqueue dequeue queue-empty?
	  queue? queue->list queue-length delete-from-queue!))


(define-interface threads-interface
  (export spawn
	  make-lock with-lock obtain-lock release-lock
	  make-condvar condvar-ref condvar-set!
	  relinquish-timeslice current-thread
	  kill-thread start-thread stop-thread ;for debugging
	  thread?
	  interrupt-thread		;documented for mobot system...
	  active-threads
	  sleep time one-second

	  thread-uid thread-continuation ;used by serve.scm
	  thread-read-char		;used by comm.scm
	  thread-peek-char
	  read-char-with-timeout

	  kill-condvar
	  terminate-current-thread
	  with-multitasking

	  with-interrupts-inhibited
	  with-interrupts-allowed))


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
	  current-row current-column fresh-line
	  input-port? output-port?))

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
          search-tree-max
          search-tree-min
          walk-search-tree))

(define-interface big-scheme-interface
  (compound-interface
      (interface-of ascii)
      (interface-of bitwise)
      (interface-of tables)
      (interface-of enumerated)
      defrecord-interface
      extended-ports-interface
      queues-interface
      (interface-of tables)
      (export concatenate-symbol
	      error breakpoint
	      atom? null-list? neq? n=
	      identity no-op
	      memq? first any? any every? 
	      filter filter! filter-map partition-list partition-list!
	      remove-duplicates delq delq! delete
	      reverse!
	      (destructure :syntax)
	      (receive :syntax)
	      format
	      sort-list sort-list!
	      p pretty-print)))

; --------------------
; Miscellaneous

; Copied from interfaces.scm.
(define-interface define-record-types-interface
  (export (define-record-type :syntax)
	  define-record-discloser))

(define-interface sicp-interface
  (export and or (sequence :syntax)
	  mapcar mapc 1+ -1+ t nil atom? print princ prin1 error
	  (cons-stream :syntax) head tail the-empty-stream empty-stream?
	  explode implode get put))

(define-interface generics-interface    ;Obsolescent
  (export make-method-table
	  apply-generic
	  make-generic
	  disclose
	  disclose-methods
	  define-method
	  define-default-method
	  fail
	  make-family))
