; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; More and more packages.  Some of these get loaded into the initial
; image to create scheme48.image; those that aren't can be loaded later
; using ,load-package.


; Things to load into initial.image to make scheme48.image.

(define-structure usual-features (export )  ;No exports
  (open analysis		;auto-integration
	disclosers
        command-processor
        debuginfo
        ;; Choose any combination of bignums, ratnums, recnums
	bignums ratnums recnums
	;; Choose either innums, floatnums, or neither
	innums			;Silly inexact numbers
        ;; floatnums		;Still don't print correctly
	;; pp
	;; The following is listed because this structure is used to
	;; generate a dependency list used by the Makefile...
	usual-commands))


; Command processor

(define-structures ((command-processor command-processor-interface)
		    (command (export command-processor)))
  (open scheme ;;-level-2     ; eval, interaction-environment
	tables fluids
	conditions handle
	define-record-types
	command-levels
	reading			; gobble-line, with-sharp-sharp
	i/o                     ; current-error-port
	display-conditions	; display-condition
	methods
	;; environments		; interaction-environment
	util			; unspecific
	undefined		; $note-undefined
	features		; force-output
	interrupts		; set-enabled-interrupts!, all-interrupts
	vm-exposure		; primitive-catch
	fluids-internal         ; get-dynamic-env, set-dynamic-env!
	nodes			; for ## kludge
	signals
	structure-refs
	root-scheduler)		; scheme-exit-now
  (access threads		; thread?
	  threads-internal      ; thread-continuation
	  continuations         ; continuation?
	  ports)
  (files (env version-info)
	 (env command)
	 (env read-command)))

(define-structure command-levels command-levels-interface
  (open scheme
	enumerated enum-case 
	tables fluids
	define-record-types
	threads threads-internal
	scheduler
	interrupts
	weak
	user-messages		; for debugging
	signals			; error
	i/o			; current-error-port
	util                    ; unspecific
	channel-i/o             ; steal-channel-port
	fluids-internal         ; get-dynamic-env, set-dynamic-env!
	root-scheduler          ; call-when-deadlocked!
	conditions)             ; define-condition-type
  (files (env command-level)))

(define-structure basic-commands basic-commands-interface
  (open scheme-level-2
        command-processor
	command-levels
        undefined               ; noting-undefined-variables
	environments		; with-interaction-environment
	evaluation		; eval, load-into
        ;; packages		; package?
	root-scheduler		; scheme-exit-now
	)
  (files (env basic-command)))

; Usual command set
               
(define-structure usual-commands usual-commands-interface
  (open basic-commands
        build-commands
        package-commands
        debug-commands
        inspect-commands
        disassemble-commands
	;profile-commands
	))

; Image builder.

(define-structures ((build (export build-image stand-alone-resumer))
		    (build-commands build-commands-interface))
  (open scheme-level-2
        command-processor
	command-levels
        conditions handle
        low-level               ; flush-the-symbol-tables!
        usual-resumer
        filenames               ; translate
        display-conditions      ; display-condition
        evaluation              ; package-for-load, eval
	environments		; with-interaction-environment
	i/o			; current-error-port
        write-images
        signals)
  (files (env build)))

; Package commands.

(define-structures ((package-commands package-commands-interface)
		    (package-commands-internal
		       package-commands-internal-interface))
  (open scheme
        command-processor
	command-levels
	methods
        undefined               ; noting-undefined-variables
        packages                ; for creating a user package
        packages-internal       ; set-package-integrate?!, etc.
        package-mutation        ; package-system-sentinel
        environments            ; *structure-ref, etc.
	compiler-envs		; reflective-tower
        ensures-loaded          ; ensure-loaded
	interfaces
	ascii
	i/o			; force-output, current-error-port
        signals
	util			; every
        fluids)
  (files (env pacman)))


; Debugging aids.

; Disclosers: this makes objects and conditions print nicely.

(define-structure disclosers disclosers-interface
  (open scheme-level-1
        methods more-types
        tables
        conditions
        display-conditions
        locations
        code-vectors
        closures
        packages-internal       ; location-info-tables
        debug-data
	segments                ; get-debug-data
        enumerated              ; enumerand->name
        weak                    ; weak-pointer?
	i/o                     ; disclose-port
        templates continuations channels
        architecture)
  (files (env disclosers)))

; For printing procedures with their names, etc.

(define-structure debuginfo debuginfo-interface
  (open scheme-level-2
        tables
        debug-data
	segments		; debug-data-table
        ;; packages
        packages-internal	; package-name-table
        names			; generated?
	features
	weak)
  (files (env debuginfo)))

; Most of the debugging commands.

(define-structures ((debugging		;additional exports in future
		     (export breakpoint
			     continuation-parent))
		    (debug-commands debug-commands-interface))
  (open scheme-level-2
        command-processor       ; define-command, etc.
	command-levels
        fluids
        signals                 ; make-condition
        util                    ; filter
        evaluation              ; eval-from-file, eval
        environments            ; environment-define! (for ,trace)
        conditions              ; define-condition-type
        filenames               ; set-translation!
        disclosers              ; template-name, debug-data-names
        packages                ; flush-location-names, package-integrate?
        packages-internal       ; [set-]package-integrate?[!], flush-location-names
	undefined		; noting-undefined-variables
        continuations           ; continuation-template
        architecture            ; op/global, etc.
        interrupts              ; all-interrupts, set-enabled-interrupts!
        vm-exposure             ; fluid-let suppression kludge - fix later
        exceptions              ; continuation-preview
        tables
        nodes		        ; schemify for ,expand command
        reading-forms           ; $note-file-package
	debug-data segments     ;  yucko
	time                    ; real-time
        primitives              ; memory-status
        structure-refs)
  (access primitives            ; want to both use and shadow collect
          filenames             ;  and translate
	  syntactic)		;  and expand
  (files (env debug)))

; Inspector

(define-structures ((inspector (export inspect))
		    (inspector-internal (export prepare-menu))
		    (inspect-commands inspect-commands-interface))
  (open scheme-level-2
        command-processor       ; define-command, etc.
	command-levels
	define-record-types
        fluids
        debugging               ; command-loop-continuation
        closures                ; closure-template
        disclosers              ; template-debug-data, etc.
        debug-data
	evaluation		; eval
	segments                ; get-debug-data
        templates
        continuations
        names	                ; desyntaxify
        records
	records-internal
        low-level               ; vector-unassigned?
        locations
        signals                 ; error
        ;; tables  - not yet.
        weak
        util                    ; sublist
        display-conditions)     ; limited-write
  (files (env inspect)))

(define-structure list-interfaces (export list-interface)
  (open scheme-level-2 interfaces packages meta-types sort bindings)
  (files (env list-interface)))


; Package and interface mutation.

(define-structure package-mutation package-mutation-interface
  (open scheme-level-2
        shadowing               ; shadow-location!
        packages
        interfaces
	bindings
        packages-internal
        defpackage              ; set-verify-later!
        locations
        disclosers              ; location-info
        handle
        tables fluids weak signals)
  (files (env pedit)))

; The following hooks the compiler up with an exception handler for
; unbound variables.

(define-structure shadowing (export shadow-location!)
  (open scheme-level-1
        vm-exposure             ;primitive-catch
        continuations templates locations code-vectors
        exceptions signals
        architecture)   ;(enum op global)
  (files (env shadow)))     ;Exception handler to support package system


; Disassembler

(define-structures ((disassembler
		       (export disassemble write-instruction))
		    (disassemble-commands disassemble-commands-interface))
  (open scheme-level-2
        command-processor       ; define-command
	disclosers              ; template-name
        enumerated              ; enumerand->name
        disclosers              ; location-name
	evaluation		; eval
        templates
        continuations
        locations
        code-vectors
        closures
        architecture
        signals)
  (files (env disasm)))

; Assembler.

(define-structure assembling (export)	; No exports, this defines a compilator.
  (open scheme-level-2
	compiler		;define-compilator
	segments
	architecture
	nodes			;node-form node-ref
	bindings		;binding? binding-place
        meta-types              ;value-type
        templates               ; for Richard's version
        signals                 ;error
        enumerated              ;name->enumerand
        code-vectors)
  (files (env assem)))

; Foo

(define-structure assembler (export (lap :syntax))
  (open scheme-level-2)
  (for-syntax (open scheme-level-2 nodes meta-types assembling))
  (begin
    (define-syntax lap
      (lambda (e r c)
        (make-node (get-operator 'lap syntax-type) e)))))

; Execution profiler.

(define-structures ((profile (export run-with-profiling))
		    (profile-commands profile-commands-interface))
  (open scheme
	command-processor
        continuations
        architecture
        interrupts
        tables
        primitives     ; schedule-interrupt
        wind
        disclosers
	time
        sort
        escapes)       ; primitive-cwcc
  (files (env profile)))

; Large integers and rational and complex numbers.

(define-structure extended-numbers extended-numbers-interface
  (open scheme-level-2
        methods meta-methods
        define-record-types
        exceptions              ; make-opcode-generic!
        primitives
        architecture
        signals
	util
        number-i/o)
  (files (rts xnum)))

(define-structure bignums bignums-interface
  (open scheme-level-2
        extended-numbers
        methods signals)
  (files (rts bignum))
  (optimize auto-integrate))

(define-structure innums (export )    ;inexact numbers
  (open scheme-level-2
        extended-numbers
        methods signals
        number-i/o)             ;string->integer
  (files (rts innum)))

(define-structure ratnums (export )    ;No exports
  (open scheme-level-2
        extended-numbers
        methods signals
        number-i/o)             ;string->integer
  (files (rts ratnum)))

(define-structure recnums (export )    ;No exports
  (open scheme-level-2
        extended-numbers
        methods signals
        number-i/o)             ;really-number->string
  (files (rts recnum)))

(define-structure floatnums
		  (export floatnum? exp log sin cos tan asin acos atan sqrt)
  (open scheme-level-2
        extended-numbers
        code-vectors
        methods signals
	enumerated
	loopholes
        primitives)             ;vm-extension
  (files (rts floatnum))
  (optimize auto-integrate))

(define-structure time time-interface
  (open scheme-level-1 primitives architecture enumerated)
  (files (rts time)))

(define-structure placeholders placeholder-interface
  (open scheme-level-1 define-record-types
	threads threads-internal
	interrupts
	signals)
  (files (big placeholder))
  (optimize auto-integrate))

;----------------
; Big Scheme

(define-structure random (export make-random)
  (open scheme-level-2 bitwise
	signals)		;call-error
  (files (big random)))

(define-structure sort (export sort-list sort-list!)
  (open scheme-level-2)
  (files (big sort)))

(define-structure pp (export p pretty-print define-indentation)
  (open scheme-level-2
        tables
        methods)               ;disclose
  (files (big pp)))


; Bitwise logical operators on bignums.

(define-structure bigbit (export )  ;No exports
  (open scheme-level-2
	bignums
	methods
        extended-numbers
        ;; exceptions
	;; architecture
        bitwise
	signals)
  (files (big bigbit)))

(define-structure formats (export format)
  (open scheme-level-2 ascii signals
	extended-ports)
  (files (big format)))

(define-structure extended-ports extended-ports-interface
  (open scheme-level-2 records define-record-types ports i/o i/o-internal
	code-vectors ascii
	structure-refs
	signals)
  (access primitives)     ; copy-bytes!
  (files (big more-port)))

(define-structure destructuring (export (destructure :syntax))
  (open scheme-level-2)
  (files (big destructure)))

(define-structure arrays arrays-interface
  (open scheme-level-2 define-record-types signals)
  (files (big array)))

(define-structure receiving (export (receive :syntax))
  (open scheme-level-2)
  (files (big receive)))

(define-structure defrecord defrecord-interface
  (open scheme-level-1 records loopholes
	primitives) ; unspecific
  (files (big defrecord)))

(define general-tables tables)    ; backward compatibility

(define-structure big-util big-util-interface
  (open scheme-level-2
	formats
        structure-refs)         ; for structure-ref
  (access signals               ; for error
          debugging)            ; for breakpoint        
  (files (big big-util)))

(define-structure big-scheme big-scheme-interface
  (open scheme-level-2
	formats
	sort
        extended-ports
	pp
	enumerated
        bitwise
        ascii
        bigbit
	big-util
        tables
	;defrecord
        destructuring
        receiving))

; Externals

(define-structure externals externals-interface
  (open scheme-level-2 handle exceptions tables
        primitives
        architecture            ;stob
        conditions              ;exception-arguments
        signals                 ;warn
        ascii code-vectors)
  (files (big external)))

; Rudimentary object dump and restore

(define-structure dump/restore dump/restore-interface
  (open scheme-level-1
        number-i/o
        tables
        records
        signals                 ;error
        locations               ;make-undefined-location
        closures
        code-vectors            ;code vectors
        fluids
        ascii
        bitwise
        methods                 ;disclose
        templates)              ;template-info
  (files (big dump)))

; Unix Sockets

(define-structure sockets
    (export open-socket close-socket
	    socket-listen
	    socket-listen-channels
	    socket-port-number
	    socket-client
	    socket-client-channels)
  (open scheme
	define-record-types
	channel-i/o
	interrupts              ;disable-interrupts! enable-interrupts!
	primitives
	channels		;open-channel
	architecture)		;open-channel-option
  (files (big socket)))

; Heap traverser

(define-structure traverse
                  (export traverse-depth-first traverse-breadth-first trail
                          set-leaf-predicate! usual-leaf-predicate)
  (open scheme-level-2
        primitives              ; ?
        queues tables
        bitwise locations closures code-vectors
        disclosers              ; foo
        features                ; string-hash
        low-level)              ; flush-the-symbol-table!, vector-unassigned?
  (files (env traverse)))

; Space analyzer

(define-structure spatial (export space vector-space record-space)
  (open scheme
	architecture primitives assembler packages enumerated 
	features sort locations display-conditions)
  (files (env space)))

; Structure & Interpretation compatibility

(define-structure sicp sicp-interface
  (open scheme-level-2 signals tables)
  (files (misc sicp)))

; red-black balanced binary search trees

(define-structure search-trees search-trees-interface
  (open scheme-level-2 define-record-types)
  (optimize auto-integrate)
  (files (big search-tree)))


; ... end of package definitions.


; Temporary compatibility stuff
(define-syntax define-signature
  (syntax-rules () ((define-signature . ?rest) (define-interface . ?rest))))
(define-syntax define-package
  (syntax-rules () ((define-package . ?rest) (define-structures . ?rest))))
(define table tables)
(define record records)


; Must list all the packages defined in this file that are to be
; visible in the command processor's config package.

(define-interface more-structures-interface
  (export ((more-structures
	    usual-features
	    arrays
	    assembler
	    assembling
	    general-tables
	    bigbit
	    bignums ratnums recnums floatnums
	    build
	    command-levels
	    command-processor
	    debugging
	    define-record-types
	    defrecord
	    destructuring
	    disassembler
	    disclosers
	    dump/restore
	    enum-case
	    extended-numbers
	    extended-ports
	    externals
	    formats
	    innums
	    inspector
	    inspector-internal
	    ;linked-queues
	    list-interfaces
	    ;more-threads
	    package-commands-internal
	    package-mutation
	    placeholders
	    pp
	    ;profile
	    queues
	    time
	    random
	    receiving
	    search-trees
	    sicp
	    sockets
	    sort
	    strong
	    traverse
	    spatial
	    big-scheme
	    big-util
	    ;; From link-packages.scm:
	    analysis
	    debuginfo
	    expander
	    flatloading
	    linker
	    link-config
	    reification			;?
	    shadowing
	    ;; Compatibility
	    record table
	    )
	   :structure)
	  ((define-signature define-package) :syntax)))
