; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Interfaces for packages that can get loaded after the initial.image
; starts up.

; Command processor

(define-interface command-interface
  (export command-processor
	  $command-levels
	  $write-length
	  $write-depth
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
	  define-command
	  error-form ;foo
	  execute-command
	  evaluate			; ???
	  evaluate-and-select
	  exit				;more-threads
	  gobble-line
	  greet-user
	  make-user-context
	  environment-for-commands
	  read-command			;take
	  read-command-carefully	;inspect
	  read-form
	  run-sentinels
	  set-command-level-env!
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
	  set-batch-mode?!))

(define-interface package-commands-interface
  (export config-package
	  new-command-processor
	  get-structure
	  get-package))

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
	  (struct-list syntax)
	  compile-structures))

(define-interface expander-interface
  (export expand-forms
	  expand-form
	  expand-stuff
	  expand
	  lexical-reference-count
	  lexical-operator-count
	  lexical-assignment-count))


; --------------------
; Extended numbers: bignums, ratnums, etc.

(define-interface extended-number-support-interface
  (export make-extended-number-type
	  extended-number-constructor
	  extended-number-accessor
	  extended-number-predicate
	  plus-table
	  minus-table
	  *-table
	  /-table
	  =-table
	  <-table
	  quotient-table
	  remainder-table
	  integer?-table
	  rational?-table
	  real?-table
	  complex?-table
	  number?-table
	  exact?-table
	  exact->inexact-table
	  inexact->exact-table
	  real-part-table
	  imag-part-table
	  floor-table
	  numerator-table
	  denominator-table
	  sqrt-table
	  make-rectangular-table
	  number->string-table
	  string->number-table))


; --------------------
; Big Scheme

(define-interface defrecord-interface  ;The competition.
  (export (define-record-type syntax)
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

(define-interface general-tables-interface
  (export make-table
	  make-string-table
	  make-symbol-table
	  make-integer-table
	  make-table-maker
	  table?
	  table-ref
	  table-set!
	  table-walk))

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
      general-tables-interface
      (export concatenate-symbol
	      error breakpoint
	      atom? null-list? neq? n=
	      identity no-op
	      memq? first any? any every? 
	      filter filter! filter-map partition-list partition-list!
	      remove-duplicates delq delq! delete
	      reverse!
	      (destructure syntax)
	      (receive syntax)
	      format
	      sort-list sort-list!
	      p pretty-print)))

; --------------------
; Miscellaneous

; Copied from interfaces.scm.
(define-interface define-record-types-interface
  (export (define-record-type syntax)
	  define-record-discloser))

(define-interface sicp-interface
  (export and or (sequence syntax)
	  mapcar mapc 1+ -1+ t nil atom? print princ prin1 error
	  (cons-stream syntax) head tail the-empty-stream empty-stream?
	  explode implode get put))
