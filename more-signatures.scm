; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Signatures for packages that can get loaded after the initial.image
; starts up.

; Command processor

(define-signature command-signature
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

(define-signature package-commands-signature
  (export config-package
	  new-command-processor
	  get-structure
	  get-package))

(define-signature debuginfo-signature
  (export read-debug-info
	  write-debug-info))

(define-signature disclosers-signature
  (export make-print-name
	  template-file-name
	  value->expression
	  error-form
	  location-info
	  location-name))

(define-signature package-mutation-signature
  (export package-system-sentinel	;env/command.scm
	  package-open!			;env/debug.scm
	  ))

(define-signature packages-cruft-signature
  (export 
	  assume-denotation
	  ;; new-location-uid    ;?
	  signature-ref
	  structure-signature
	  verify-package		;for debugging the package system
	  ))


; --------------------
; Linker

(define-signature linker-signature
  (export link-simple-system
	  link-reified-system
	  link-semireified-system
	  (struct-list syntax)
	  compile-structures))

(define-signature expander-signature
  (export expand-forms
	  expand-form
	  expand-stuff
	  expand
	  lexical-reference-count
	  lexical-operator-count
	  lexical-assignment-count))


; --------------------
; Extended numbers: bignums, ratnums, etc.

(define-signature extended-number-support-signature
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

(define-signature defrecord-signature  ;The competition.
  (export (define-record-type syntax)
	  define-record-discloser))

(define-signature externals-signature
  (export get-external
	  lookup-all-externals
	  external-call
	  null-terminate))

(define-signature queue-signature
  (export make-queue enqueue dequeue queue-empty?
	  queue? queue->list queue-length delete-from-queue!))


(define-signature threads-signature
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

	  (with-interrupts-inhibited syntax)
	  (with-interrupts-allowed syntax)))


(define-signature dump/restore-signature
  (export dump
	  restore
	  note-location!
	  $dump-index
	  $restore-index))

(define-signature extended-ports-signature
  (export make-tracking-input-port make-tracking-output-port
	  make-string-input-port
	  call-with-string-output-port
	  write-one-line
	  current-row current-column fresh-line
	  input-port? output-port?))

(define-signature general-tables-signature
  (export make-table
	  make-string-table
	  make-symbol-table
	  make-integer-table
	  make-table-maker
	  table?
	  table-ref
	  table-set!
	  table-walk))


(define-signature arrays-signature
  (export make-array		; <initial-value> <bound1> ...
	  array-shape		; <array>
	  array-ref		; <array> <index1> ...
	  array-set!		; <array> <value> <index1> ...
	  make-shared-array	; <array> <linear-map> <bound1> ...
	  copy-array		; <array>
	  array->vector		; <array>
	  array))		; <bounds> . <elements>

(define-signature big-scheme-signature
  (compound-signature
      (signature-of ascii)
      (signature-of bitwise)
      (signature-of table)
      (signature-of enumerated)
      defrecord-signature
      extended-ports-signature
      queue-signature
      general-tables-signature
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

; Copied from signatures.scm.
(define-signature define-record-types-signature
  (export (define-record-type syntax)
	  define-record-discloser))

(define-signature sicp-signature
  (export and or (sequence syntax)
	  mapcar mapc 1+ -1+ t nil atom? print princ prin1 error
	  (cons-stream syntax) head tail the-empty-stream empty-stream?
	  explode implode get put))
