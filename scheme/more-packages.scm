; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


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
	;; bignums		; now in the VM
	ratnums recnums
	;; Choose either innums, floatnums, or neither
	innums			;Silly inexact numbers
        ;; floatnums		;Still don't print correctly
	;; pp
	;; The following is listed because this structure is used to
	;; generate a dependency list used by the Makefile...
	usual-commands))

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
	more-types		;:double
        primitives)             ;vm-extension double?
  (files (rts floatnum))
  (optimize auto-integrate))

(define-structure time time-interface
  (open scheme-level-1 primitives architecture enumerated)
  (begin
    (define (real-time)
      (time (enum time-option real-time) #f))

    (define (run-time)
      (time (enum time-option run-time) #f))))

(define-structure placeholders placeholder-interface
  (open scheme-level-1 proposals queues
	(subset util (unspecific))
	threads threads-internal
	interrupts
	signals)
  (files (big placeholder))
  (optimize auto-integrate))

(define-structure locks locks-interface
  (open scheme-level-1 queues
	threads threads-internal
	interrupts
	proposals)
  (optimize auto-integrate)
  (files (big lock)))

;----------------
; Big Scheme

(define-structure random (export make-random)
  (open scheme-level-2 bitwise
	signals)		;call-error
  (files (big random)))

(define-structure sort (export sort-list sort-list!)
  (open scheme-level-2
	vector-heap-sort list-merge-sort)
  (begin
    (define (sort-list l obj-<)
      (let ((v (list->vector l)))
	(vector-heap-sort! obj-< v)
	(vector->list v)))
    (define (sort-list! l obj-<)
      (list-merge-sort! obj-< l))))

(define-structure pp (export p pretty-print define-indentation)
  (open scheme-level-2
        tables
        methods)               ;disclose
  (files (big pp)))

(define-structure formats (export format)
  (open scheme-level-2 ascii signals
	extended-ports)
  (files (big format)))

(define-structure extended-ports extended-ports-interface
  (open scheme-level-2 define-record-types ascii byte-vectors
	ports
	i/o i/o-internal
	proposals
	util			; unspecific
	signals
	(subset primitives	(copy-bytes!)))
  (files (big more-port)))

(define-structure destructuring (export (destructure :syntax))
  (open scheme-level-2)
  (files (big destructure)))

(define-structure mvlet (export ((mvlet mvlet*) :syntax))
  (open scheme-level-2)
  (files (big mvlet)))

(define-structure reduce (export ((reduce iterate)
				  :syntax)
				 ((list* list%
				   vector* vector%
				   string* string%
				   count* count%
				   bits* bits%
				   input* input%
				   stream* stream%)
				  :syntax))
  (open scheme-level-2
	bitwise
	signals)
  (files (big iterate)))

(define-structure arrays arrays-interface
  (open scheme-level-2 define-record-types signals)
  (files (big array)))

(define-structure lu-decompositions lu-decompositions-interface
  (open scheme receiving arrays floatnums signals)
  (files (big lu-decomp)))

(define-structure receiving (export (receive :syntax))
  (open scheme-level-2)
  (files (big receive)))

(define-structure defrecord defrecord-interface
  (open scheme-level-1 records record-types loopholes
	primitives)			; unspecific, low-level record ops
  (files (big defrecord)))

(define-structures ((masks masks-interface)
		    (mask-types mask-types-interface))
  (open scheme-level-1 define-record-types
	bitwise
	util			; every
	number-i/o		; number->string
	signals)		; call-error
  (files (big mask)))

(define-structures ((enum-sets enum-sets-interface)
		    (enum-sets-internal enum-sets-internal-interface))
  (open scheme define-record-types
	finite-types
	bitwise 
	util
	signals
	external-calls)
  (optimize auto-integrate)
  (files (big enum-set)))

(define general-tables tables)    ; backward compatibility

(define-structure big-util big-util-interface
  (open scheme-level-2
	formats
	features		; immutable? make-immutable!
	(modify signals		(rename (error rts-error))
		                (expose error))
	(modify debugging	(rename (breakpoint rts-breakpoint))
		                (expose breakpoint))
	(subset primitives	(copy-bytes!)))
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
	big-util
        tables
        destructuring
        receiving))

; Things needed for connecting with external code.

(define-structure external-calls (export call-imported-binding
					 lookup-imported-binding
					 define-exported-binding
					 shared-binding-ref
					 ((import-definition
					   import-lambda-definition)
					  :syntax)
					 add-finalizer!
					 define-record-resumer
					 call-external-value)
  (open scheme-level-2 define-record-types
	primitives
        architecture
	exceptions interrupts signals
	placeholders
	shared-bindings
	byte-vectors
	;bitwise		;for {enter|extract}_integer() helpers
	(subset record-types		(define-record-resumer))
	(subset records-internal	(:record-type)))
  (files (big import-def)
	 (big callback)))

(define-structure dynamic-externals dynamic-externals-interface
  (open scheme-level-2 define-record-types tables
        signals                 ;warn
	primitives		;find-all-records
	i/o			;current-error-port
        code-vectors
	external-calls)
  (files (big external)))

(define-structure shared-objects shared-objects-interface
  (open scheme-level-2
	define-record-types
	external-calls)
  (files (big shared-object)))

(define-structure c-system-function (export have-system? system)
  (open scheme-level-2 external-calls signals)
  (begin
    (import-lambda-definition s48-system (string))

    (define (have-system?)
      (not (= 0 (s48-system #f))))

    (define (system string)
      (if (string? string)
	  (s48-system string)
	  (call-error "not a string" system string)))))
    
; Rudimentary object dump and restore

(define-structure dump/restore dump/restore-interface
  (open scheme-level-1
        number-i/o
        tables
        records record-types
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

; Pipes containing values.

(define-structure value-pipes value-pipes-interface
  (open scheme queues
        proposals
        threads-internal
	signals)		;call-error
  (optimize auto-integrate)
  (files (big value-pipe)))

; Unix Sockets

(define-structures ((sockets (export open-socket
				     close-socket
				     socket-accept
				     socket-port-number
				     socket-client
				     get-host-name

				     ; From the old interface
				     ; I would like to get rid of these.
				     socket-listen
				     socket-listen-channels
				     socket-client-channels))
		    (udp-sockets (export get-host-name
					 close-socket
					 open-udp-socket
					 udp-send
					 udp-receive
					 lookup-udp-address
					 socket-port-number
					 udp-address?
					 udp-address-address
					 udp-address-hostname
					 udp-address-port)))
  (open scheme define-record-types
	external-calls
	channels		; channel? close-channel
	signals			; error call-error
	proposals		; atomically!
	interrupts		; enable-interrupts! disable-interrupts!
	channel-ports		; {in|out}put-channel->port
	channel-i/o		; wait-for-channel
	condvars)		; for wait-for-channel
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

; Reinitializing upon image resumption

(define-structure reinitializers reinitializers-interface
  (open scheme-level-2
	define-record-types
	(subset record-types (define-record-resumer)))
  (files (big reinitializer)))

; Space analyzer

(define-structure spatial (export space vector-space record-space)
  (open scheme
	architecture primitives assembler packages enumerated 
	features sort locations display-conditions)
  (files (env space)))

; Listing what is in an interface.  Here because it needs sort.

(define-structure list-interfaces (export list-interface)
  (open scheme-level-2 interfaces packages meta-types sort bindings)
  (files (env list-interface)))

; red-black balanced binary search trees

(define-structure search-trees search-trees-interface
  (open scheme-level-2 define-record-types)
  (optimize auto-integrate)
  (files (big search-tree)))

; vectors that grow as large as they need to

(define-structure sparse-vectors sparse-vectors-interface
  (open scheme
	bitwise
	define-record-types)
  (files (big hilbert)))

; record types with a fixed number of instances

(define-structure finite-types (export ((define-finite-type
					 define-enumerated-type) :syntax))
  (open scheme-level-2 code-quote define-record-types
	enumerated
	features)		; make-immutable
  (files (big finite-type)))

; nondeterminism via call/cc

(define-structure nondeterminism (export with-nondeterminism
					 ((either one-value all-values) :syntax)
					 fail)
  (open scheme-level-2
	fluids cells
	(subset signals (error)))
  (files (big either)))

(define-structure libscheme48 (export dump-libscheme48-image)
  (open scheme
 	(subset escapes (with-continuation))
	build)
  (files (big libscheme48)))

;----------------
; Obsolete packages

; Bignums and bitwise logical operators on bignums.  These are now handled
; by the VM.   These packages are here to keep from breaking scripts that
; load them.  They will be removed in a later release.

(define-structure bignums (export)
  (open scheme-level-2))

(define-structure bigbit (export)
  (open scheme-level-2))

; Externals - this is obsolete; use external-calls and dynamic-externals
; instead.

(define-structure externals (compound-interface
			       dynamic-externals-interface
			       (export external-call
				       null-terminate))
  (open scheme-level-2 dynamic-externals
	(subset external-calls (import-lambda-definition)))
  (begin
   ; We fake the old external-call primitive using the new one and a
   ; a C helper procedure from c/unix/dynamo.c.

    (define (external-call proc . args)
      (let ((args (apply vector args)))
	(old-external-call (external-value proc) args)))
    
    (import-lambda-definition old-external-call
			      (proc args)
			      "s48_old_external_call")

    ; All strings are now null terminated.
    (define (null-terminate string) string)))

;----------------
; ... end of package definitions.

; Temporary compatibility stuff
(define-syntax define-signature
  (syntax-rules () ((define-signature . ?rest) (define-interface . ?rest))))
(define-syntax define-package
  (syntax-rules () ((define-package . ?rest) (define-structures . ?rest))))
(define table tables)
(define record records)
