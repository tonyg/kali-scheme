; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define-structures ((scheme-level-1 scheme-level-1-interface)
		    (util util-interface))
  (open scheme-level-0 ascii signals)
  (usual-transforms case quasiquote syntax-rules)
  (files (rts base)
	 (rts util)
	 (rts number)
	 (rts lize))	  ; Rationalize
  (optimize auto-integrate))


; "Level 2"

(define-structures ((records records-interface)
		    (records-internal records-internal-interface))
  (open scheme-level-1 signals
	primitives)
  (files (rts record))
  (optimize auto-integrate))

; The external code needs this to check the types of records.

(define-structure export-the-record-type (export)
  (open scheme-level-1 records-internal shared-bindings)
  (begin
    (define-exported-binding "s48-the-record-type" :record-type)))

(define-structure define-record-types define-record-types-interface
  (open scheme-level-1 records records-internal loopholes
	primitives) ; unspecific
  (files (rts jar-defrecord)))

(define-structures ((methods methods-interface)
		    (meta-methods meta-methods-interface))
  (open scheme-level-1
	define-record-types
	records records-internal
	bitwise util primitives
	signals)
  (files (rts method))
  (optimize auto-integrate))

(define-structure number-i/o number-i/o-interface
  (open scheme-level-1 methods signals ascii)
  (files (rts numio)))

(define-structures ((fluids fluids-interface)
		    (fluids-internal fluids-internal-interface))
  (open scheme-level-1 define-record-types primitives)
  (files (rts fluid))
  (optimize auto-integrate))

(define-structure wind wind-interface
  (open scheme-level-1 signals define-record-types
	fluids fluids-internal
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
	primitives ports code-vectors bitwise
	define-record-types ascii
	threads locks
	methods         ; &disclose :input-port :output-port
	interrupts      ; {en|dis}able-interrupts!
	number-i/o      ; number->string for debugging
	exceptions      ; wrong-number-of-args stuff
	handle)		; report-errors-as-warnings
  (files (rts port)
	 (rts current-port))
  (optimize auto-integrate))

(define-structure channels channels-interface
  (open scheme-level-1
	low-channels
	architecture
	signals)	; error, call-error
  (files (rts channel)))

(define-structure channel-i/o channel-i/o-interface
  (open scheme-level-1 i/o i/o-internal signals
	channels low-channels
	architecture code-vectors wind
	define-record-types
	queues threads threads-internal locks
	exceptions interrupts
	ascii ports util
	session-data
	structure-refs
	debug-messages	; for error messages
	handle)		; report-errors-as-warnings
  (access primitives)	; add-finalizer, channel stuff
  (files (rts channel-port)
	 (rts channel-io)))

(define-structure conditions conditions-interface
  (open scheme-level-1 signals)
  (files (rts condition)))

(define-structure writing writing-interface
  (open scheme-level-1
	number-i/o
	i/o				;output-port-option, write-string
	methods				;disclose
	structure-refs)
  (access low-channels			;channel? channel-id
	  code-vectors)			;code-vector?
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
	channel-i/o))

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
				     :shared-binding)
  (open scheme-level-1 methods
	closures code-vectors locations templates low-channels ports primitives
	shared-bindings)
  (begin (define-simple-type :closure     (:value) closure?)
	 (define-simple-type :code-vector (:value) code-vector?)
	 (define-simple-type :location    (:value) location?)
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
	signals fluids
	conditions	  ;make-exception, etc.
	primitives	  ;set-exception-handlers!, etc.
	wind		  ;CWCC
	methods
	meta-methods
	more-types
	architecture
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
        escapes                 ;primitive-cwcc
        conditions              ;error?
        handle                  ;with-handler
        signals                 ;signal, warn
	loopholes               ;for converting #f to a continuation
	architecture            ;time-option
	session-data
	debug-messages
	structure-refs)
  (access primitives)           ;time current-thread set-current-thread! etc.
  (optimize auto-integrate)
  (files (rts thread) (rts sleep)))

(define-structure scheduler scheduler-interface
  (open scheme-level-1 threads threads-internal enumerated enum-case
	debug-messages
	signals)       		;error
  (files (rts scheduler)))

(define-structure root-scheduler (export root-scheduler
					 spawn-on-root
					 scheme-exit-now
					 call-when-deadlocked!)
  (open scheme-level-1 threads threads-internal scheduler structure-refs
	session-data
	signals        		;error
	handle			;with-handler
	i/o			;current-error-port
	conditions		;warning?, error?
	writing			;display
	i/o-internal            ;output-port-forcer, output-forcer-id
	fluids-internal         ;get-dynamic-env
	interrupts              ;with-interrupts-inhibited
	wind                    ;call-with-current-continuation
	channel-i/o)		;waiting-for-i/o?
  (access primitives)		;unspecific, wait
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
  (open scheme-level-1 define-record-types signals)
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
  (open scheme-level-1 define-record-types interrupts threads threads-internal)
  (optimize auto-integrate)
  (files (rts lock)))

(define-structure usual-resumer (export usual-resumer)
  (open scheme-level-1
	i/o		 ;initialize-i/o, etc.
	channel-i/o      ;{in,out}put-channel->port, initialize-channel-i/o
	session-data     ;initialize-session-data!
	fluids-internal	 ;initialize-dynamic-state!
	exceptions	 ;initialize-exceptions!
	interrupts	 ;initialize-interrupts!
	records-internal ;initialize-records!
	export-the-record-type	;just what it says
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
