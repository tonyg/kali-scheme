; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define-structures ((scheme-level-1 scheme-level-1-interface)
		    (util util-interface))
  (open scheme-level-0 ascii signals
	code-quote)			; needed by SYNTAX-RULES
  (usual-transforms case quasiquote syntax-rules)
  (files (rts base)
	 (rts util)
	 (rts number)
	 (rts lize))	  ; Rationalize
  (optimize auto-integrate))


; "Level 2"

(define-structures ((record-types record-types-interface)
		    (records-internal records-internal-interface))
  (open scheme-level-1 records signals
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
	signals)
  (files (rts method))
  (optimize auto-integrate))

(define-structure number-i/o number-i/o-interface
  (open scheme-level-1 methods signals ascii)
  (files (rts numio)))

(define-structures ((fluids fluids-interface)
		    (fluids-internal fluids-internal-interface))
  (open scheme-level-1 define-record-types primitives cells)
  (files (rts fluid))
  (optimize auto-integrate))

(define-structure wind wind-interface
  (open scheme-level-1 signals define-record-types
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

(define-structures ((i/o i/o-interface)
		    (i/o-internal i/o-internal-interface))
  (open scheme-level-1 signals fluids
	architecture
	primitives
	ports byte-vectors bitwise
	define-record-types ascii
	proposals
	session-data
	debug-messages	; for error messages
	methods         ; &disclose :input-port :output-port
	number-i/o      ; number->string for debugging
	handle		; report-errors-as-warnings
	exceptions)     ; wrong-number-of-args stuff
  (files (rts port)
	 (rts port-buffer)
	 (rts current-port))
  (optimize auto-integrate))

(define-structure channel-i/o channel-i/o-interface
  (open scheme-level-1 byte-vectors queues
	channels
	i/o i/o-internal
	signals
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
	i/o i/o-internal
	channels channel-i/o
	proposals
	condvars
	signals
	architecture		; channel-opening options
	debug-messages		; for error messages
	(subset util		(unspecific))
	(subset primitives	(add-finalizer!)))
  (files (rts channel-port)))

(define-structure conditions conditions-interface
  (open scheme-level-1 signals)
  (files (rts condition)))

(define-structure writing writing-interface
  (open scheme-level-1
	number-i/o
	i/o				;output-port-option, write-string
	methods				;disclose
	(subset channels	(channel? channel-id))
	(subset code-vectors	(code-vector?)))
  (files (rts write)))
	 
(define-structure reading reading-interface
  (open scheme-level-1
	number-i/o
	i/o		;input-port-option
	ascii		;for dispatch table
	signals		;warn, signal-condition, make-condition
	conditions	;define-condition-type
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
  (open scheme-level-1 primitives templates methods architecture code-vectors)
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
  (open scheme-level-1 signals)
  (files (rts defenum scm)))

(define-structure architecture architecture-interface
  (open scheme-level-1 signals enumerated)
  (files (vm arch)))

(define-structures ((exceptions exceptions-interface)
		    (handle handle-interface))
  (open scheme-level-1
	signals fluids cells
	conditions	  ;make-exception, etc.
	primitives	  ;set-exception-handlers!, etc.
	wind		  ;CWCC
	methods
	meta-methods
	more-types
	architecture
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
	signals fluids conditions
	bitwise
	escapes
	session-data
	primitives
	architecture)
  (files (rts interrupt))
  (optimize auto-integrate)) ;mostly for threads package...

(define-structures ((threads threads-interface)
		    (threads-internal threads-internal-interface))
  (open scheme-level-1 enumerated define-record-types queues
	interrupts
        wind
        fluids
	fluids-internal         ;get-dynamic-env
	proposals		;maybe-commit
        escapes                 ;primitive-cwcc
        conditions              ;error?
        handle                  ;with-handler
        signals                 ;signal, warn, call-error
	loopholes               ;for converting #f to a continuation
	architecture            ;time-option
	session-data
	debug-messages
	(subset primitives	(find-all-records
				 current-thread set-current-thread!
				 unspecific
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
  (open scheme-level-1 threads threads-internal enumerated enum-case
	debug-messages
	signals)       		;error
  (files (rts scheduler)))

(define-structure root-scheduler (export root-scheduler
					 spawn-on-root
					 scheme-exit-now
					 call-when-deadlocked!)
  (open scheme-level-1 threads threads-internal scheduler
	session-data
	conditions		;warning?, error?
	writing			;display
	debug-messages		;for debugging
	(subset i/o		(current-error-port newline))
	(subset signals		(error))
	(subset handle		(with-handler))
	(subset i/o-internal	(output-port-forcers output-forcer-id))
	(subset fluids-internal (get-dynamic-env))
	(subset interrupts      (with-interrupts-inhibited
				 all-interrupts
				 set-enabled-interrupts!))
	(subset wind            (call-with-current-continuation))
	(subset channel-i/o	(waiting-for-i/o?
				 initialize-channel-i/o!
				 abort-unwanted-i/o!))
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
  (open scheme-level-1 proposals signals)
  (files (big queue))
  (optimize auto-integrate))

; No longer used
;(define-structure linked-queues (compound-interface 
;                                 queues-interface
;                                 (export delete-queue-entry!
;                                         queue-head))
;  (open scheme-level-1 define-record-types signals primitives)
;  (files (big linked-queue))
;  (optimize auto-integrate))

(define-structure locks locks-interface
  (open scheme-level-1
	threads threads-internal
	interrupts define-record-types
	proposals)
  (optimize auto-integrate)
  (files (rts lock)))

(define-structures ((condvars condvars-interface)
		    (condvars-internal (export condvar-has-waiters?)))
  (open scheme-level-1
	proposals
	threads threads-internal)
  (optimize auto-integrate)
  (files (rts condvar)))

(define-structure usual-resumer (export usual-resumer)
  (open scheme-level-1
	i/o		 ;initialize-i/o, etc.
	channel-i/o      ;initialize-channel-i/o
	channel-ports    ;{in,out}put-channel->port
	session-data     ;initialize-session-data!
	fluids-internal	 ;initialize-dynamic-state!
	exceptions	 ;initialize-exceptions!
	interrupts	 ;initialize-interrupts!
	records-internal ;initialize-records!
	threads-internal ;start threads
	root-scheduler)  ;start a scheduler
  (files (rts init)))

; Weak pointers & populations

(define-structure weak weak-interface
  (open scheme-level-1 signals
	primitives)	;Open primitives instead of loading (alt weak)
  (files ;;(alt weak)   ;Only needed if VM's weak pointers are buggy
	 (rts population)))

; Utility for displaying error messages

(define-structure display-conditions display-conditions-interface
  (open scheme-level-2
	writing
	methods
	handle)			;ignore-errors
  (files (env dispcond)))
