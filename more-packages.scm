 ; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; More and more packages.  Some of these get loaded into the initial
; image to create scheme48.image; others can be loaded later using
; ,load-package.


(define-module (make-command scheme)
  (define-package ((command command-signature))
    (open scheme
	  record table fluids signals
	  condition handle
	  reading		; gobble-line, with-sharp-sharp
	  display-conditions	; display-condition
	  packages		; for prompt string, etc.
	  environments		; with-interaction-environment
	  features		; unspecific
	  interrupts		; set-enabled-interrupts!
	  vm-exposure		; primitive-catch
	  locations)		; for ## kludge
    (files (env version-info)
	   (env command)
	   (env read-command)))
  command)

(define-structure command (make-command scheme))


; Image builder.

(define-package ((build (export build-image)))
  (open scheme-level-2
	command
	signals
	condition handle
	low-level		; flush-the-symbol-table!
	scheme-level-2-internal ; usual-resumer
	filenames		; translate
	display-conditions	; display-condition
	evaluation		; package-for-load, eval
	write-images
	features)		; unspecific
  (files (env build)))

; Package commands.

(define-package ((package-commands package-commands-signature))
  (open scheme
	command
	signals
	packages		; for creating a user package
	environments		; *structure-ref, etc.
	ensures-loaded		; ensure-loaded
	fluids)
  (files (env pacman)))


; Debugging aids.

; Disclosers: this makes objects and conditions print nicely.

(define-package ((disclosers disclosers-signature))
  (open scheme-level-1
	table
	condition
	display-conditions
	locations
	code-vectors
	closures
	packages		; location-name
	debug-data		; template-names
	enumerated		; enumerand->name
	weak			; weak-pointer?
	generics
	template continuation
	architecture)
  (files (env disclosers)	;Make error messages be more informative
	 ))

; Most of the debugging commands.

(define-package ((debugging	;additional exports in future
		  (export breakpoint
			  continuation-parent)))
  (open scheme-level-2
	command			; define-command, etc.
	fluids
	signals			; make-condition
	util			; filter
	evaluation		; eval-from-file, eval
	environments		; environment-define! (for ,trace)
	condition		; define-condition-type
	filenames		; translate
	debug-data		; template-name
	packages		; flush-location-names, package-integrate?
	continuation		; continuation-template
	architecture		; op/global, etc.
	interrupts		; all-interrupts, set-enabled-interrupts!
	vm-exposure		; fluid-let suppression kludge - fix later
	table
	syntactic		; for ,bound? command
	scan			; $note-file-package
	primitives)		; memory-status, time
  (files (env debug)))


; Inspector

(define-package ((inspector (export inspect)))
  (open scheme-level-2
	command			; define-command, etc.
	fluids
	debugging		; command-loop-continuation
	closures		; closure-template
	debug-data		; template-debug-data, etc.
	template
	continuation
	syntactic		; desyntaxify
	record-internal
	low-level		; unassigned
	locations
	;; table  - not yet.
	weak
	util			; sublist
	primitives		; unassigned (only)
	display-conditions)	; limited-write
  (files (env inspect)))

; Load database of procedure names 'n' stuff.

(define-package ((debuginfo debuginfo-signature))
  (open scheme-level-2
	table
	debug-data
	packages
	syntactic)
  (files (env debuginfo)))


; Disassembler

(define-package ((disassembler
		  (export disassemble write-instruction)))
  (open scheme-level-2
	command			; define-command
	debug-data		; template-name, byte-limit
	enumerated		; enumerand->name
	packages		; location-name
	template
	continuation
	locations
	code-vectors
	closures
	architecture
	signals)
  (files (env disasm)))

; Assembler.

(define-package ((assembler (export (lap syntax))))
  (open scheme-level-2 compiler architecture
	signals				;error
	enumerated			;name->enumerand
	code-vectors
	locations)			;location?
  (specials lap)
  (files (env assem)))


; Large integers and rational and complex numbers.

(define-package ((extended-number-support extended-number-support-signature))
  (open scheme-level-2
	signals
	record
	exception		; make-opcode-generic!
	primitives
	generics
	architecture
	number-i/o)
  (files (rts xnum)))

(define-package ((bignum (export )))
  (open scheme-level-2
	extended-number-support
	generics signals)
  (files (rts bignum)))

(define-package ((ratnum (export )))	;No exports
  (open scheme-level-2
	extended-number-support
	generics signals
	number-i/o)		;string->integer
  (files (rts ratnum)))

(define-package ((recnum (export )))	;No exports
  (open scheme-level-2
	extended-number-support
	generics signals
	number-i/o)		;really-number->string
  (files (rts recnum)))


; Static linker.  Doesn't work very well this way (debug info is
; screwed up), so it's probably better to continue using linker.image
; instead.

(define-package ((linker linker-signature))
  (open scheme table compiler write-images debuginfo
	signals			;error
	util			;sublist
	packages
	scan
	filenames
	debug-data		;with-fresh-compiler-state
	locations
	syntactic		;$source-file-name
	fluids
	reification)
  (files (link link)))

(define-package ((reification (export reify-structures)))
  (open scheme-level-2 table record
	signals			;error
	packages
	usual-macros		;find-free-names-in-syntax-rules
	syntactic		;name-hash, etc.
	locations
	features		;force-output
	util			;filter
	inline)			;name->extrinsic
  (files (link reify)))

(define-package ((analysis (export alpha-forms analyze-forms)))
  (open scheme
	syntactic packages scan inline
	signals
	locations
	features		;force-output
	table
	fluids
	util)
  (files (opt node)
	 (opt alpha)
	 (opt analyze)))

(define-package ((linker-etc (export ))) ;dummy for flatload-package
  (open linker defpackage analysis))

(define-package ((define-record-types define-record-types-signature))
  (open scheme-level-1 record)
  (files (rts jar-defrecord)))


; Various Big Scheme things

(define-package ((queues queue-signature))
  (open scheme-level-1 define-record-types signals)
  (files (big queue))
  (optimize auto-integrate))

(define-package ((random (export make-random)))
  (open scheme-level-2 bitwise)
  (files (big random)))

(define-package ((sort (export sort-list sort-list!)))
  (open scheme-level-2)
  (files (big sort)))

(define-package ((pp (export p pretty-print define-indentation)))
  (open scheme-level-2
	table
	generics)		;disclose
  (files (big pp)))


; Bigbit can replace bignum.

(define-package ((bigbit (export )))  ;No exports
  (open scheme-level-2
        extended-number-support
        exception architecture
        bitwise generics signals
	number-i/o)
  (files (rts bignum)
	 (big bigbit)))


; Richard's stuff

(define-package ((defrecord defrecord-signature))
  (open scheme-level-1 record)
  (files (big defrecord)))

(define-package ((formats (export format)))
  (open extended-ports		;Exports input-port? and output-port?
	scheme-level-2 signals)
  (files (big format)))

(define-package ((extended-ports extended-ports-signature))
  (open scheme-level-2 record defrecord
	architecture
	condition signals exception structure-refs)
  (access port primitives) ;write-string, close-port, etc.
  (files (big xport)
	 (big new-ports)))

(define-package ((destructuring (export (destructure syntax))))
  (open scheme-level-2)
  (files (big destructure)))

(define-package ((big-scheme big-scheme-signature))
  (open extended-ports		;Exports input-port? and output-port?
	scheme-level-2 formats queues table sort defrecord
	pp enumerated
	bitwise
	ascii
	bigbit
	debugging		; for breakpoint	
	structure-refs		; for structure-ref
	bigbit
	destructuring)
  (access signals)		; for error
  (files (big rk-util)
	 (big receive)))

; Externals

(define-package ((externals externals-signature))
  (open scheme-level-2 handle exception table
	primitives
	architecture			;stob
	condition)			;exception-arguments
  (files (big external)))

; Rudimentary object dump and restore

(define-package ((dump/restore dump/restore-signature))
  (open scheme-level-1
	number-i/o
	table
	record
        signals			;error
	locations		;make-undefined-location
	closures
        code-vectors		;code vectors
	;; features ?
	ascii
	bitwise
        generics		;disclose
	template)		;template-info
  (files (big dump)))

; Threads

(define-package ((threads threads-signature))
  (open scheme-level-1
	wind			; still needed?
	define-record-types queues fluids
	interrupts		;interrupt-handlers, one-second
	escapes			;primitive-cwcc
	fluids-internal		;get-dynamic-state, etc.
	condition		;error?
	handle			;with-handler
	signals			;signal
	architecture		;(enum time-option real-time)
	structure-refs)
  (access primitives)		;time
  (files (big compose-cont)
	 (big thread)
	 (big sleep))
  (optimize auto-integrate))

; Glue to connect the threads package with command processor.

(define-package ((more-threads (export threads)))
  (open scheme threads
	handle condition interrupts signals
	command
	debugging		;breakpoint
	display-conditions
	architecture)		;interrupt
  (files (env more-thread)))


; Heap traverser

(define-package ((traverse
		  (export traverse-depth-first traverse-breadth-first trail
			  set-leaf-predicate! usual-leaf-predicate)))
  (open scheme-level-2
	primitives		; ?
	queues table
	low-level)		; flush-the-symbol-table!
  (files (env traverse)))

; Structure & Interpretation compatibility

(define-package ((sicp sicp-signature))
  (open scheme-level-2 signals table)
  (files (misc sicp)))

; Unix sockets

(define-package ((sockets (export socket-client socket-server)))
  (open scheme-level-2
	primitives)
  (files (misc socket)))



; ... end of package definitions.



; Stuff to load into standard image.

(define-package ((usual-features (export )))  ;No exports
  (open disclosers
	command
	package-commands
	build
	debugging
	debuginfo
	bignum
	ratnum
	inspector
	disassembler
	pp))

; Must list all the packages defined in this file that are to be
; visible in the command processor's config package.

(define-signature more-structures-signature
  (export more-structures
	  usual-features
	  analysis
	  assembler
	  bigbit
	  bignum ratnum recnum
	  build
	  command
	  debugging
	  debuginfo
	  define-record-types
	  destructuring
	  disassembler
	  disclosers
	  dump/restore
	  extended-number-support
	  extended-ports
	  externals
	  formats defrecord
	  inspector
	  linker
	  linker-etc
	  more-threads
	  package-commands
	  pp
	  queues
	  random
	  reification
	  sicp
	  sockets
	  sort
	  threads
	  traverse
	  big-scheme))
