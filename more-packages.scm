; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; More and more packages.  Some of these get loaded into the initial
; image to create scheme48.image; others can be loaded later using
; ,load-package.


(define-module (make-command-processor scheme)
  (define-structures ((command-processor command-processor-interface))
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

(define-structures ((usual-commands usual-commands-interface))
  (open basic-commands
        build-commands
        package-commands
        debug-commands
        inspect-commands
        disassemble-commands))
                
(define-structures ((basic-commands basic-commands-interface))
  (open scheme
        command-processor
        scan                    ; noting-undefined-variables
        packages		; package?
	structure-refs)
  (access scheme)		; (structure-ref scheme load)
  (files (env basic-command)))

; Image builder.

(define-structures ((build (export build-image))
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
        ensures-loaded          ; ensure-loaded
        fluids)
  (files (env pacman)))


; Debugging aids.

; Disclosers: this makes objects and conditions print nicely.

(define-structures ((disclosers disclosers-interface))
  (open scheme-level-1
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
        generics
        templates continuations
        architecture)
  (files (env disclosers)))

; For printing procedures with their names, etc.

(define-structures ((debuginfo debuginfo-interface))
  (open scheme-level-2
        tables
        debug-data
        packages
        packages-internal
        syntactic)
  (files (env debuginfo)))

; Most of the debugging commands.

(define-structures ((debugging     ;additional exports in future
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

(define-structures ((package-mutation package-mutation-interface))
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

(define-structures ((shadowing (export shadow-location!)))
  (open scheme-level-1
        vm-exposure             ;primitive-catch
        continuations templates locations code-vectors
        exceptions signals
        architecture)   ;(enum op global)
  (files (rts shadow)))     ;Exception handler to support package system


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

(define-structures ((assembling (export )))
  (open scheme-level-2 compiler segments architecture
        syntactic               ;lookup
        meta-types              ;value-type
        templates               ; for Richard's version
        signals                 ;error
        enumerated              ;name->enumerand
        code-vectors
        locations)              ;location?
  (files (env assem)))

(define-structures ((assembler (export (lap :syntax))))
  ;; Open the assembling structure to make sure it gets loaded.
  ;; (Ensure-loaded neglects to examine the package for syntax.)
  (open scheme-level-2 assembling)
  (for-syntax (open scheme-level-2 syntactic meta-types assembling))
  (begin
    (define-syntax lap
      (lambda (e r c)
        (make-node (get-operator 'lap syntax-type) e)))))


; Large integers and rational and complex numbers.

(define-structures ((extended-number-support extended-number-support-interface))
  (open scheme-level-2
        signals
        records
        exceptions              ; make-opcode-generic!
        primitives
        generics
        architecture
        number-i/o)
  (files (rts xnum)))

(define-structures ((bignum (export )))
  (open scheme-level-2
        extended-number-support
        generics signals)
  (files (rts bignum)))

(define-structures ((ratnum (export )))    ;No exports
  (open scheme-level-2
        extended-number-support
        generics signals
        number-i/o)             ;string->integer
  (files (rts ratnum)))

(define-structures ((recnum (export )))    ;No exports
  (open scheme-level-2
        extended-number-support
        generics signals
        number-i/o)             ;really-number->string
  (files (rts recnum)))

(define-structures ((floatnum
		  (export exp log sin cos tan asin acos atan sqrt)))
  (open scheme-level-2
        extended-number-support
        code-vectors
        generics signals
        primitives)             ;vm-extension
  (files (rts floatnum)))

(define-structures ((define-record-types define-record-types-interface))
  (open scheme-level-1 records)
  (files (rts jar-defrecord)))


; --------------------
; Big Scheme

(define-structures ((queues queues-interface))
  (open scheme-level-1 bummed-define-record-types signals)
  (files (big queue))
  (optimize auto-integrate))

(define-structures ((random (export make-random)))
  (open scheme-level-2 bitwise)
  (files (big random)))

(define-structures ((sort (export sort-list sort-list!)))
  (open scheme-level-2)
  (files (big sort)))

(define-structures ((pp (export p pretty-print define-indentation)))
  (open scheme-level-2
        tables
        generics)               ;disclose
  (files (big pp)))


; Bigbit can replace bignum.

(define-structures ((bigbit (export )))  ;No exports
  (open scheme-level-2
        extended-number-support
        exceptions architecture
        bitwise generics signals
        number-i/o)
  (files (rts bignum)
         (big bigbit)))


; Richard's stuff

(define-structures ((defrecord defrecord-interface))
  (open scheme-level-1 records)
  (files (big defrecord)))

(define-structures ((formats (export format)))
  (open extended-ports          ;Exports input-port? and output-port?
	ascii
        scheme-level-2 signals)
  (files (big format)))

(define-structures ((extended-ports extended-ports-interface))
  (open scheme-level-2 records defrecord
        architecture
        conditions signals exceptions structure-refs)
  (access ports primitives) ;write-string, close-port, etc.
  (files (big xport)
         (big new-ports)))

(define-structures ((destructuring (export (destructure :syntax))))
  (open scheme-level-2)
  (files (big destructure)))

(define-structures ((general-tables general-tables-interface))
  (open scheme-level-2 records define-record-types signals structure-refs)
  (access features) ; string-hash
  (files (big general-table)))

(define-structures ((arrays arrays-interface))
  (open scheme-level-2 defrecord signals)
  (files (big array)))

(define-structures ((receiving (export (receive :syntax))))
  (open scheme-level-2)
  (files (big receive)))


(define-structures ((big-scheme big-scheme-interface))
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

(define-structures ((externals externals-interface))
  (open scheme-level-2 handle exceptions tables
        primitives
        architecture            ;stob
        conditions              ;exception-arguments
        signals                 ;warn
        ascii code-vectors)
  (files (big external)))

; Rudimentary object dump and restore

(define-structures ((dump/restore dump/restore-interface))
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
        generics                ;disclose
        templates)              ;template-info
  (files (big dump)))

; Threads

(define-structures ((threads threads-interface))
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

(define-structures ((more-threads (export threads)))
  (open scheme threads
        handle conditions interrupts signals
        command-processor
        debugging               ;breakpoint
        display-conditions
        architecture)           ;interrupt
  (files (env more-thread)))


; Heap traverser

(define-structures ((traverse
                  (export traverse-depth-first traverse-breadth-first trail
                          set-leaf-predicate! usual-leaf-predicate)))
  (open scheme-level-2
        primitives              ; ?
        queues tables
        bitwise locations closures code-vectors
        debug-data              ; foo
        features                ; string-hash
        low-level)              ; flush-the-symbol-table!, vector-unassigned?
  (files (env traverse)))

; Structure & Interpretation compatibility

(define-structures ((sicp sicp-interface))
  (open scheme-level-2 signals tables)
  (files (misc sicp)))

; Unix sockets

(define-structures ((sockets (export socket-client socket-server)))
  (open scheme-level-2
        primitives)
  (files (misc socket)))

(define-structures ((search-trees search-trees-interface))
  (open scheme-level-2 defrecord receiving)
  (optimize auto-integrate)
  (files (big search-tree)))


; ... end of package definitions.



; Stuff to load into initial.image to make scheme48.image.

(define-structures ((usual-features (export )))  ;No exports
  (open disclosers
        command-processor
        package-commands-internal
        build
        debugging
        debuginfo
        bignum
        ratnum
        inspector
        disassembler
        pp
        ))

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
            bignum ratnum recnum floatnum
            build
            command-processor
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
            package-commands-internal
            package-mutation
            pp
            queues
            random
            receiving
            search-trees
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
            ;; Compatibility
            record table
            )
           :structure)
          ((define-signature define-package) :syntax)))
