; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; More and more packages.  Some of these get loaded into the initial
; image to create scheme48.image; others can be loaded later using
; ,load-package.


(define-module (make-command-processor scheme)
  (define-structure command-processor command-processor-interface
    (open scheme                ; eval
          records tables fluids signals
          conditions handle
          reading               ; gobble-line, with-sharp-sharp
          display-conditions    ; display-condition
          packages              ; package?
          packages-internal     ; package-name, package-uid
          environments          ; with-interaction-environment
          util                  ; unspecific
          scan                  ; $note-undefined
          features              ; force-output
          interrupts            ; set-enabled-interrupts!
          vm-exposure           ; primitive-catch
          syntactic)            ; for ## kludge
    (files (env version-info)
           (env command)
           (env read-command)))
  command-processor)

(define command-processor (make-command-processor scheme))

(define-structure usual-commands usual-commands-interface
  (open basic-commands
        build-commands
        package-commands
        debug-commands
        inspect-commands
        disassemble-commands))
                
(define-structure basic-commands basic-commands-interface
  (open scheme
        command-processor
        scan                    ; noting-undefined-variables
        packages		; package?
	structure-refs)
  (access scheme)		; (structure-ref scheme load)
  (files (env basic-command)))

; Image builder.

(define-structures ((build (export build-image stand-alone-resumer))
		    (build-commands build-commands-interface))
  (open scheme-level-2
        command-processor
        signals
        conditions handle
        low-level               ; flush-the-symbol-tables!
        scheme-level-2-internal ; usual-resumer
        filenames               ; translate
        display-conditions      ; display-condition
        evaluation              ; package-for-load, eval
        write-images)
  (files (env build)))

; Package commands.

(define-structures ((package-commands package-commands-interface)
		    (package-commands-internal
		       package-commands-internal-interface))
  (open scheme
        command-processor
        signals
        scan                    ; noting-undefined-variables
        packages                ; for creating a user package
        packages-internal       ; set-package-integrate?!, etc.
        package-mutation        ; package-system-sentinel
        environments            ; *structure-ref, etc.
	syntactic		; reflective-tower
        ensures-loaded          ; ensure-loaded
	ascii ports		; for access-scheme-48
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
        debug-data              ; template-names
        enumerated              ; enumerand->name
        weak                    ; weak-pointer?
        templates continuations
        architecture)
  (files (env disclosers)))

;(define-structure generics generics-interface    ;Obsolescent
;  (open scheme-level-1
;        methods)
;  (files (rts generic))
;  (optimize auto-integrate))

; For printing procedures with their names, etc.

(define-structure debuginfo debuginfo-interface
  (open scheme-level-2
        tables
        debug-data
        packages
        packages-internal
        syntactic
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
        fluids
        signals                 ; make-condition
        util                    ; filter
        evaluation              ; eval-from-file, eval
        environments            ; environment-define! (for ,trace)
        conditions              ; define-condition-type
        filenames               ; set-translation!
        debug-data              ; template-name
        packages                ; flush-location-names, package-integrate?
        packages-internal       ; ?
        continuations           ; continuation-template
        architecture            ; op/global, etc.
        interrupts              ; all-interrupts, set-enabled-interrupts!
        vm-exposure             ; fluid-let suppression kludge - fix later
        exceptions              ; continuation-preview
        tables
        syntactic               ; for ,bound? command
        scan                    ; $note-file-package
        primitives              ; memory-status, time
        structure-refs)
  (access primitives           ; want to both use and shadow collect and time
          filenames)            ; and translate
  (files (env debug)))

; Inspector

(define-structures ((inspector (export inspect))
		    (inspect-commands inspect-commands-interface))
  (open scheme-level-2
        command-processor       ; define-command, etc.
        fluids
        debugging               ; command-loop-continuation
        closures                ; closure-template
        debug-data              ; template-debug-data, etc.
        templates
        continuations
        syntactic               ; desyntaxify
        records
	records-internal
        low-level               ; vector-unassigned?
        locations
        signals                 ; error
        ;; tables  - not yet.
        weak
        util                    ; sublist
        primitives              ; unassigned (only)
        display-conditions)     ; limited-write
  (files (env inspect)))


; Package and interface mutation.

(define-structure package-mutation package-mutation-interface
  (open scheme-level-2
        shadowing               ; shadow-location!
        compiler                ; package-undefineds
        packages
        interfaces
        syntactic
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
        debug-data              ; template-name, byte-limit
        enumerated              ; enumerand->name
        disclosers              ; location-name
        templates
        continuations
        locations
        code-vectors
        closures
        architecture
        signals)
  (files (env disasm)))

; Assembler.

(define-structure assembling (export )
  (open scheme-level-2 compiler segments architecture
        syntactic               ;lookup
        meta-types              ;value-type
        templates               ; for Richard's version
        signals                 ;error
        enumerated              ;name->enumerand
        code-vectors
        locations)              ;location?
  (files (env assem)))

; Foo

(define-structure assembler (export (lap :syntax))
  ;; Open the assembling structure to make sure it gets loaded.
  ;; (Ensure-loaded neglects to examine the package for syntax.)
  (open scheme-level-2 assembling)
  (for-syntax (open scheme-level-2 syntactic meta-types assembling))
  (begin
    (define-syntax lap
      (lambda (e r c)
        (make-node (get-operator 'lap syntax-type) e)))))


; Large integers and rational and complex numbers.

(define-structure extended-numbers extended-numbers-interface
  (open scheme-level-2
        methods meta-methods
        bummed-define-record-types
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
		  (export exp log sin cos tan asin acos atan sqrt)
  (open scheme-level-2
        extended-numbers
        code-vectors
        methods signals
	enumerated
        primitives)             ;vm-extension
  (files (rts floatnum))
  (optimize auto-integrate))

(define-structure define-record-types define-record-types-interface
  (open scheme-level-1 records)
  (files (rts jar-defrecord)))


; --------------------
; Big Scheme

(define-structure queues queues-interface
  (open scheme-level-1 bummed-define-record-types signals)
  (files (big queue))
  (optimize auto-integrate))

(define-structure random (export make-random)
  (open scheme-level-2 bitwise)
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


; Richard's stuff

(define-structure defrecord defrecord-interface
  (open scheme-level-1 records)
  (files (big defrecord)))

(define-structure formats (export format)
  (open extended-ports          ;Exports input-port? and output-port?
	ascii
        scheme-level-2 signals)
  (files (big format)))

(define-structure extended-ports extended-ports-interface
  (open scheme-level-2 records defrecord
        architecture
        conditions signals exceptions structure-refs)
  (access ports primitives) ;write-string, close-port, etc.
  (files (big xport)
         (big new-ports)))

(define-structure destructuring (export (destructure :syntax))
  (open scheme-level-2)
  (files (big destructure)))

(define-structure arrays arrays-interface
  (open scheme-level-2 defrecord signals)
  (files (big array)))

(define-structure receiving (export (receive :syntax))
  (open scheme-level-2)
  (files (big receive)))

(define general-tables tables)    ; backward compatibility

(define-structure big-scheme big-scheme-interface
  (open extended-ports          ;Exports input-port? and output-port?
        scheme-level-2 formats queues sort defrecord
        pp enumerated
        bitwise
        ascii
        bigbit
        general-tables
        structure-refs          ; for structure-ref
        destructuring
        receiving)
  (access signals               ; for error
          debugging)            ; for breakpoint        
  (files (big big-util)))

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

; Threads

(define-structure threads threads-interface
  (open scheme-level-1
        wind                    ; still needed?
        bummed-define-record-types queues fluids
        interrupts              ;interrupt-handlers, one-second
        escapes                 ;primitive-cwcc
        fluids-internal         ;get-dynamic-state, etc.
        conditions              ;error?
        handle                  ;with-handler
        signals                 ;signal
        architecture            ;(enum time-option real-time)
        structure-refs)
  (access primitives)           ;time
  (files (big compose-cont)
         (big thread)
         (big sleep))
  (optimize auto-integrate))

; Glue to connect the threads package with command processor.

(define-structure more-threads (export threads)
  (open scheme threads
        handle conditions interrupts signals
        command-processor
        debugging               ;breakpoint
        display-conditions
        architecture)           ;interrupt
  (files (env more-thread)))


; Heap traverser

(define-structure traverse
                  (export traverse-depth-first traverse-breadth-first trail
                          set-leaf-predicate! usual-leaf-predicate)
  (open scheme-level-2
        primitives              ; ?
        queues tables
        bitwise locations closures code-vectors
        debug-data              ; foo
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

(define-structure search-trees search-trees-interface
  (open scheme-level-2 defrecord receiving)
  (optimize auto-integrate)
  (files (big search-tree)))


; ... end of package definitions.


; Stuff to load into initial.image to make scheme48.image.

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
	    command-processor
	    debugging
	    define-record-types
	    destructuring
	    disassembler
	    disclosers
	    dump/restore
	    extended-numbers
	    extended-ports
	    externals
	    formats defrecord
	    innums
	    inspector
	    more-threads
	    package-commands-internal
	    package-mutation
	    pp
	    queues
	    random
	    receiving
	    search-trees
	    sicp
	    ;; sockets
	    sort
	    threads
	    traverse
	    spatial
	    big-scheme
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
