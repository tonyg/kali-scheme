; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; More and more packages.  Some of these get loaded into the initial
; image to create scheme48.image; others can be loaded later using
; ,load-package.


(define-module (make-command scheme)
  (define-package ((command command-signature))
    (open scheme		; eval
	  record table fluids signals
	  condition handle
	  reading		; gobble-line, with-sharp-sharp
	  display-conditions	; display-condition
	  packages		; package?
	  packages-internal	; package-name, package-uid
	  environments		; with-interaction-environment
	  util			; unspecific
	  scan			; $note-undefined, noting-undefined-variables
	  features		; force-output
	  interrupts		; set-enabled-interrupts!
	  vm-exposure		; primitive-catch
	  syntactic)		; for ## kludge
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
	write-images)
  (files (env build)))

; Package commands.

(define-package ((package-commands package-commands-signature))
  (open scheme
	command
	signals
	scan			; noting-undefined-variables
	packages		; for creating a user package
	packages-internal	; set-package-integrate?!, etc.
	package-mutation	; package-system-sentinel
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
	packages-internal	; location-info-table
	debug-data		; template-names
	enumerated		; enumerand->name
	weak			; weak-pointer?
	generics
	template continuation
	architecture)
  (files (env disclosers)))

; For printing procedures with their names, etc.

(define-package ((debuginfo debuginfo-signature))
  (open scheme-level-2
	table
	debug-data
	packages
	packages-internal
	syntactic)
  (files (env debuginfo)))

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
	packages-internal	; ?
	continuation		; continuation-template
	architecture		; op/global, etc.
	interrupts		; all-interrupts, set-enabled-interrupts!
	vm-exposure		; fluid-let suppression kludge - fix later
	exception		; continuation-preview
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
	low-level		; vector-unassigned?
	locations
	signals			; error
	;; table  - not yet.
	weak
	util			; sublist
	primitives		; unassigned (only)
	display-conditions)	; limited-write
  (files (env inspect)))


; Package and signature mutation.

(define-package ((package-mutation package-mutation-signature))
  (open scheme-level-2
	shadowing		; shadow-location!
	compiler		; package-undefineds
	packages
	signatures
	syntactic
	packages-internal
	defpackage		; set-verify-later!
	locations
	disclosers		; location-info
	handle
	table fluids weak signals)
  (files (env pedit)))

; The following hooks the compiler up with an exception handler for
; unbound variables.

(define-package ((shadowing (export shadow-location!)))
  (open scheme-level-1
	vm-exposure		;primitive-catch
	continuation template locations code-vectors
	exception signals
	architecture)	;(enum op global)
  (files (rts shadow)))     ;Exception handler to support package system


; Disassembler

(define-package ((disassembler
		  (export disassemble write-instruction)))
  (open scheme-level-2
	command			; define-command
	debug-data		; template-name, byte-limit
	enumerated		; enumerand->name
	disclosers		; location-name
	template
	continuation
	locations
	code-vectors
	closures
	architecture
	signals)
  (files (env disasm)))

; Assembler.

(define-package ((assembling (export )))
  (open scheme-level-2 compiler segments architecture
	syntactic		;lookup
	meta-types		;value-type
	template		; for Richard's version
	signals			;error
	enumerated		;name->enumerand
	code-vectors
	locations)		;location?
  (files (env assem)))

(define-package ((assembler (export (lap syntax))))
  ;; Open the assembling structure to make sure it gets loaded.
  ;; (Ensure-loaded neglects to examine the package for syntax.)
  (open scheme-level-2 assembling)
  (for-syntax (open scheme-level-2 syntactic meta-types assembling))
  (begin
    (define-syntax lap
      (lambda (e r c)
	(make-node (get-operator 'lap syntax-type) e)))))


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

(define-package ((floatnum (export )))	;No exports
  (open scheme-level-2
	extended-number-support
	generics signals
	primitives)             ;vm-extension
  (files (rts floatnum)))

(define-package ((define-record-types define-record-types-signature))
  (open scheme-level-1 record)
  (files (rts jar-defrecord)))


; --------------------
; Big Scheme

(define-package ((queues queue-signature))
  (open scheme-level-1 bummed-define-record-types signals)
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

(define-package ((general-tables general-tables-signature))
  (open scheme-level-2 record define-record-types signals structure-refs)
  (access features) ; string-hash
  (files (big general-table)))

(define-package ((arrays arrays-signature))
  (open scheme-level-2 defrecord signals)
  (files (big array)))


(define-package ((big-scheme big-scheme-signature))
  (open extended-ports		;Exports input-port? and output-port?
	scheme-level-2 formats queues sort defrecord
	pp enumerated
	bitwise
	ascii
	bigbit
	general-tables
	structure-refs		; for structure-ref
	bigbit
	destructuring)
  (access signals		; for error
	  debugging)		; for breakpoint	
  (files (big big-util)
	 (big receive)))

; Externals

(define-package ((externals externals-signature))
  (open scheme-level-2 handle exception table
	primitives
	architecture		;stob
	condition		;exception-arguments
	ascii code-vectors)
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
	fluids
	ascii
	bitwise
        generics		;disclose
	template)		;template-info
  (files (big dump)))

; Threads

(define-package ((threads threads-signature))
  (open scheme-level-1
	wind			; still needed?
	bummed-define-record-types queues fluids
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
	bitwise locations closures code-vectors
	debug-data		; foo
	features		; string-hash
	low-level)		; flush-the-symbol-table!, vector-unassigned?
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



; Stuff to load into initial.image to make scheme48.image.

(define-package ((usual-features (export )))  ;No exports
  (open expander     ;Allow (optimize expand)
	disclosers
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
  (export ((more-structures
	    usual-features
	    analysis
	    arrays
	    assembler
	    assembling
	    general-tables
	    bigbit
	    bignum ratnum recnum floatnum
	    build
	    command
	    debugging
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
	    more-threads
	    package-commands
	    package-mutation
	    pp
	    queues
	    random
	    sicp
	    sockets
	    sort
	    threads
	    traverse
	    big-scheme
	    ;; From link-packages.scm:
	    analysis
	    debuginfo
	    expander
	    flatloading
	    linker
	    link-config
	    reification  ;?
	    shadowing
	    )
	   structure)))
