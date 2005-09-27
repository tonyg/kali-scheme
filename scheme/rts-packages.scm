; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define-structures ((scheme-level-1 scheme-level-1-interface)
		    (util util-interface)
		    (set-text-procedures (export set-char-map-procedures!
						 set-string-ci-procedures!)))
  (open scheme-level-0 ascii simple-signals
        bitwise
	code-quote)			; needed by SYNTAX-RULES
  (usual-transforms case quasiquote syntax-rules)
  (files (rts charmap)
	 (rts base)
	 (rts util)
	 (rts number)
	 (rts lize))	  ; Rationalize
  (optimize auto-integrate))


; "Level 2"

(define-structures ((record-types record-types-interface)
		    (records-internal records-internal-interface))
  (open scheme-level-1 records simple-signals
	primitives)
  (files (rts record))
  (optimize auto-integrate))

; The external code needs this to check the types of records.

(define-structure export-the-record-type (export)
  (open scheme-level-1 records-internal shared-bindings)
  (begin
    (define-exported-binding "s48-the-record-type" :record-type)))

(define-structures ((define-record-types define-record-types-interface)
		    (define-sync-record-types
		      (export (define-synchronized-record-type :syntax))))
  (open scheme-level-1
	records record-types records-internal
	loopholes
	low-proposals	;provisional-checked-record-{ref|set!}
	primitives)	;unspecific
  (files (rts jar-defrecord)))

(define-structures ((methods methods-interface)
		    (meta-methods meta-methods-interface))
  (open scheme-level-1
	define-record-types
	records record-types records-internal
	bitwise util primitives
	simple-signals)
  (files (rts method))
  (optimize auto-integrate))

(define-structure number-i/o number-i/o-interface
  (open scheme-level-1 methods simple-signals ascii)
  (files (rts numio)))

(define-structures ((fluids fluids-interface)
		    (fluids-internal fluids-internal-interface))
  (open scheme-level-1 define-record-types primitives cells)
  (files (rts fluid))
  (optimize auto-integrate))

(define-structure wind wind-interface
  (open scheme-level-1 simple-signals define-record-types
	fluids fluids-internal
	low-proposals
	escapes)
  (files (rts wind))
  (optimize auto-integrate))

(define-structure session-data (export make-session-data-slot!
				       initialize-session-data!
				       session-data-ref
				       session-data-set!)
  (open scheme-level-1
	primitives)
  (files (rts session))
  (optimize auto-integrate))

(define-structure default-string-encodings default-string-encodings-interface
  (open scheme-level-1
	byte-vectors
	unicode)
  (files (rts default-string-encoding)))

(define-structure string/bytes-types string/bytes-types-interface
  (open scheme-level-1
	define-record-types
	byte-vectors
	simple-signals
	(subset primitives (copy-bytes!)))
  (files (rts string-bytes)))

(define-structure file-names file-names-interface
  (open scheme-level-1
	string/bytes-types
	default-string-encodings)
  (files (rts file-name)))

(define-structure text-codecs text-codecs-interface
  (open scheme-level-1
	define-record-types
	bitwise
	unicode
	byte-vectors
	(subset primitives (encode-char decode-char))
	(subset architecture (text-encoding-option))
	(subset ports (port-text-codec-spec set-port-text-codec-spec!))
	enumerated enum-case)
  (files (rts text-codec))
  (optimize auto-integrate))

(define-structures ((i/o i/o-interface)
		    (i/o-internal i/o-internal-interface))
  (open scheme-level-1 simple-signals fluids
	architecture
	primitives
	ascii unicode
	ports byte-vectors bitwise
	define-record-types
	proposals
	(subset threads-internal (maybe-commit-no-interrupts))
	session-data
	debug-messages	; for error messages
	methods         ; &disclose :input-port :output-port
	number-i/o      ; number->string for debugging
	text-codecs
	handle		; report-errors-as-warnings
	vm-exceptions)     ; wrong-number-of-args stuff
  (files (rts port)
	 (rts port-buffer)
	 (rts current-port))
  (optimize auto-integrate))

(define-structure channel-i/o channel-i/o-interface
  (open scheme-level-1 byte-vectors cells
	channels
	i/o i/o-internal
	simple-conditions
	(subset threads-internal (maybe-commit-no-interrupts))
	proposals
	condvars condvars-internal
	interrupts
	architecture
	session-data
	debug-messages)	; for error messages
  (files (rts channel)))

(define-structure channel-ports channel-ports-interface
  (open scheme-level-1 byte-vectors define-record-types ascii
	ports
	i/o i/o-internal text-codecs
	channels channel-i/o
	file-names
	proposals
	condvars
	simple-signals simple-conditions
	architecture		; channel-opening options
	handle
	debug-messages		; for error messages
	(subset util		(unspecific))
	(subset primitives	(add-finalizer!)))
  (files (rts channel-port)))

(define-structure simple-conditions simple-conditions-interface
  (open scheme-level-1 simple-signals
	(subset primitives (os-error-message)))
  (files (rts simple-condition)))

(define-structure writing writing-interface
  (open scheme-level-1
	unicode
	number-i/o
	i/o				;output-port-option, write-string
	methods				;disclose
	(subset i/o-internal	(open-output-port?))
	(subset simple-signals	(call-error))
	(subset channels	(channel? channel-id))
	(subset code-vectors	(code-vector?)))
  (files (rts write)))
	 
(define-structure reading reading-interface
  (open scheme-level-1
	number-i/o
	i/o		;input-port-option
	ascii		;for dispatch table
	unicode
	simple-signals	;warn, signal-condition, make-condition
	simple-conditions	;define-condition-type
	primitives	;make-immutable!
	silly)		;reverse-list->string
  (files (rts read))
  (optimize auto-integrate))

(define-structure scheme-level-2 scheme-level-2-interface
  (open scheme-level-1
	number-i/o
	writing
	reading
	wind
	i/o
	channel-ports))

(define-structure features features-interface
  (open primitives i/o))

; Hairier stuff now.

(define-structure templates templates-interface
  (open scheme-level-1 primitives methods)
  (files (rts template))
  (optimize auto-integrate))

(define-structure continuations continuations-interface
  (open scheme-level-1 primitives
	architecture code-vectors
	templates closures all-operators
	methods)
  (files (rts continuation))
  (optimize auto-integrate))

(define-structure more-types (export :closure :code-vector :location :double
				     :template :channel :port :weak-pointer
				     :shared-binding :cell)
  (open scheme-level-1 methods
	closures code-vectors locations cells templates channels ports
	primitives shared-bindings)
  (begin (define-simple-type :closure     (:value) closure?)
	 (define-simple-type :code-vector (:value) code-vector?)
	 (define-simple-type :location    (:value) location?)
	 (define-simple-type :cell        (:value) cell?)
	 (define-simple-type :template    (:value) template?)
	 (define-simple-type :channel     (:value) channel?)
	 (define-simple-type :port        (:value) port?)
	 (define-simple-type :double      (:rational) double?)
	 (define-simple-type :weak-pointer (:value) weak-pointer?)
	 (define-method &disclose ((obj :weak-pointer)) (list 'weak-pointer))
	 (define-simple-type :shared-binding (:value) shared-binding?)
	 (define-method &disclose ((obj :shared-binding))
	   (list (if (shared-binding-is-import? obj)
		     'imported-binding
		     'exported-binding)
		 (shared-binding-name obj)))))

(define-structure enumerated enumerated-interface
  (open scheme-level-1 simple-signals)
  (files (rts defenum scm)))

(define-structure architecture vm-architecture-interface
  (open scheme-level-1 simple-signals enumerated)
  (files (vm/interp arch)))

(define-structure vm-data vm-data-interface
  (open scheme-level-1 enumerated bitwise ascii
        architecture
        (subset simple-signals (error)))
  (begin
    ; Scheme/Pre-Scheme differences
    (define (arithmetic-shift-right n k)
      (arithmetic-shift n (- k)))
    (define shift-left arithmetic-shift)
    
    ; From vm/vm-utilities.scm
    (define (adjoin-bits high low k)
      (+ (arithmetic-shift high k) low))
    
    (define (low-bits n k)
      (bitwise-and n (- (arithmetic-shift 1 k) 1)))
    
    (define high-bits arithmetic-shift-right)
    
    (define unsigned-high-bits high-bits)
    
    (define-syntax assert
      (syntax-rules ()
        ((assert foo) #t)))
    
    ; We just know this.
    (define useful-bits-per-word 32))
  (files (vm/data data)))

(define-structure vm-exceptions vm-exceptions-interface
  (open scheme-level-1
	simple-conditions
	enumerated
	architecture
	(subset primitives (set-exception-handlers! unspecific)))
  (files (rts vm-exception)))

(define-structures ((exceptions exceptions-interface)
		    (exceptions-internal exceptions-internal-interface)
		    (handle handle-interface))
  (open scheme-level-1
	simple-signals fluids cells
	simple-conditions
	vm-exceptions
	primitives	  ;set-exception-handlers!, etc.
	wind		  ;CWCC
	methods
	meta-methods
	more-types
	architecture
	enumerated
	debug-messages	  ; for printing from last-resort-condition handler
	vm-exposure	  ;primitive-catch
	templates	  ;template-code, template-info
	continuations	  ;continuation-pc, etc.
	locations	  ;location?, location-id
	closures	  ;closure-template
	number-i/o)       ; number->string, for backtrace
  (files (rts exception)))  ; Needs generic, arch
	
(define-structure interrupts interrupts-interface
  (open scheme-level-1
	simple-signals fluids simple-conditions
	bitwise
	escapes
	session-data
	primitives
	architecture)
  (files (rts interrupt))
  (optimize auto-integrate)) ;mostly for threads package...

(define-structures ((threads threads-interface)
		    (threads-internal threads-internal-interface))
  (open scheme-level-1 enumerated queues cells
	(subset proposals            (define-synchronized-record-type))
	define-record-types
	interrupts
        wind
        fluids
	fluids-internal         ;get-dynamic-env
	proposals		;maybe-commit
        escapes                 ;primitive-cwcc
        simple-conditions              ;error?
        handle                  ;with-handler
        simple-signals          ;signal, warn, call-error
	loopholes               ;for converting #f to a continuation
	architecture            ;time-option
	session-data
	debug-messages
	(subset primitives	(find-all-records
				 current-thread set-current-thread!
				 unspecific
				 collect
				 time)))
  (optimize auto-integrate)
  (files (rts thread)
	 (rts sleep)))

(define-structure proposals proposals-interface
  (open scheme-level-1 low-proposals
	define-record-types define-sync-record-types
	primitives)		 ;unspecific
  (files (rts proposal)))

(define-structure scheduler scheduler-interface
  (open scheme-level-1 threads threads-internal enumerated enum-case queues
	debug-messages
	simple-signals)       		;error
  (files (rts scheduler)))

(define-structure root-scheduler (export root-scheduler
					 spawn-on-root
					 scheme-exit-now
					 call-when-deadlocked!)
  (open scheme-level-1 threads threads-internal scheduler queues
	session-data
	simple-conditions	;warning?, error?
	writing			;display
	debug-messages		;for debugging
	(subset i/o		(current-error-port newline))
	(subset simple-signals	(error))
	(subset handle		(with-handler))
	(subset i/o-internal	(output-port-forcers output-forcer-id))
	(subset fluids-internal (get-dynamic-env))
	(subset interrupts      (with-interrupts-inhibited
				 all-interrupts
				 set-enabled-interrupts!))
	(subset wind            (call-with-current-continuation))
	(subset channel-i/o	(waiting-for-i/o?
				 initialize-channel-i/o!
				 abort-unwanted-reads!))
	(modify primitives      (rename (wait primitive-wait))
		                (expose wait unspecific)))
  (files (rts root-scheduler)))

(define-structure enum-case (export (enum-case :syntax))
  (open scheme-level-1 enumerated util)
  (begin
    (define-syntax enum-case
      (syntax-rules (else)
	((enum-case enumeration (x ...) clause ...)
	 (let ((temp (x ...)))
	   (enum-case enumeration temp clause ...)))
	((enum-case enumeration value ((name ...) body ...) rest ...)
	 (if (or (= value (enum enumeration name)) ...)
	     (begin body ...)
	     (enum-case enumeration value rest ...)))
	((enum-case enumeration value (else body ...))
	 (begin body ...))
	((enum-case enumeration value)
	 (unspecific))))))

(define-structure queues queues-interface
  (open scheme-level-1 proposals simple-signals)
  (files (big queue))
  (optimize auto-integrate))

; No longer used
;(define-structure linked-queues (compound-interface 
;                                 queues-interface
;                                 (export delete-queue-entry!
;                                         queue-head))
;  (open scheme-level-1 define-record-types simple-signals primitives)
;  (files (big linked-queue))
;  (optimize auto-integrate))

(define-structures ((condvars condvars-interface)
		    (condvars-internal (export condvar-has-waiters?)))
  (open scheme-level-1 queues
	proposals
	threads threads-internal)
  (optimize auto-integrate)
  (files (rts condvar)))

(define-structure usual-resumer (export usual-resumer
					make-usual-resumer
					add-initialization-thunk!)
  (open scheme-level-1
	i/o		 ;initialize-i/o, etc.
	channel-i/o      ;initialize-channel-i/o
	channel-ports    ;{in,out}put-channel->port
	(subset text-codecs (find-text-codec set-port-text-codec!))
	session-data     ;initialize-session-data!
	fluids-internal	 ;initialize-dynamic-state!
	exceptions-internal
	vm-exceptions
	interrupts	 ;initialize-interrupts!
	records-internal ;initialize-records!
	shared-bindings	 ;find-undefined-imported-bindings
	debug-messages	 ;warn about undefined bindings 
	threads-internal ;start threads
	root-scheduler)  ;start a scheduler
  (files (rts init)))

; Weak pointers & populations

(define-structure weak weak-interface
  (open scheme-level-1 simple-signals
	primitives)	;Open primitives instead of loading (alt weak)
  (files ;;(alt weak)   ;Only needed if VM's weak pointers are buggy
	 (rts population)))

