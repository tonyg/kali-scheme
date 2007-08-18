
;; =======================================================================

(define-interface address-space-interface
  (export address-space?
	  address-space<
          socket-id->address-space
	  initialize-local-address-space!
	  local-address-space
	  all-address-spaces
	  initialize-shared-address-space!

	  address-space-ip-address
	  address-space-socket
	  address-space-uid
	  address-space-lock
	  address-space-decode-vector set-address-space-decode-vector!
	  address-space-proxy-vector set-address-space-proxy-vector!
	  address-space-pending-vector set-address-space-pending-vector!
	  address-space-in-channel set-address-space-in-channel!
	  address-space-out-channel set-address-space-out-channel!
	  address-space-placeholder set-address-space-placeholder!

	  server-running?
	  ))

(define-structure address-spaces address-space-interface
  (open scheme address-space-internals
	define-record-types
	srfi-13			; string-drop-right
	ascii tables
	srfi-34 srfi-35         ;; error handling
	kali-conditions         ;; error handling
	architecture		; stob
	templates locations	; set-template-id! set-location-uid!
	interrupts		; with-interrupts-inhibited
	proxy-internals		; set-proxy-uid! set-proxy-owner-uid!
	locks			; make-lock
	(subset sockets (get-host-name get-host-by-name))
	(subset posix-time (time->string current-time))
	(subset primitives (encode collect find-all)))		; vm-extension copy-bytes! find-all-xs encode
  (files aspace))

(define-structure disclose-aspaces (export)
  (open scheme-level-1 methods more-types
	address-spaces)
  (begin
    (define-method &disclose ((obj :address-space))
      (list 'address-space
	    (address-space-ip-address obj)
	    (address-space-socket obj)))))

;; ===========================================================================

(define-structure message-types (export (message-type :syntax))
  (open scheme enumerated)
  (begin
    (define-enumeration message-type
      (run			; (proc . proxy . args)
       really-run               ; (proc . args)
       results			; (return-id . values)
       uid-request      	; (element-type . uid)*
       uid-reply        	; #(aspace-uid uid element-type contents)
       proxy-counts-request	; proxy-id*
       proxy-counts     	; proxy*
       return-proxy-counts	; (id . counts)*
       drop-proxy-values	; (key . #(aspace-id proxy-id count)*)
       proxy-values-dropped     ; key
       ))))

;; ============================================================================

(define-interface proxy-count-requests-interface
  (export make-proxy-requests
	  make-proxy-rerequests
	  adjust-proxy-counts!
	  proxy-uid->proxy
	  add-proxy-counts!
	  find-dead-proxies))

(define-structure proxy-count-requests proxy-count-requests-interface
  (open scheme define-record-types
	proxy-internals
	address-spaces address-space-internals
	placeholders
	architecture			; max-proxy-count, max-proxy-debit
	weak				; weak-pointer-ref
	(subset signals	(warn))
	interrupts)			; with-interrupts-inhibited
  (files proxy-count))

;; ============================================================================

(define-structure connect (export connection-server
				  send-message send-admin-message)
  (open scheme ascii
	code-vectors 
	srfi-34 srfi-35    ;; error handling
	kali-conditions    ;; error handling
	display-conditions ;; error-handling
	(subset conditions (warning?))
	(subset i/o (current-error-port))
	address-spaces
	sockets channels
	channel-i/o
	enumerated enum-case
	message-types
	proxy-count-requests
	placeholders locks
	condvars
	proposals
	interrupts			; call-after-gc!
	bitwise				; arithmetic-shift
	threads			; spawn
	(subset primitives (eof-object  ;; because of channel-read
			    memory-status 
			    encode 
			    decode 
			    unspecific))
	(subset architecture (memory-status-option)))
  (files connect))

;; =======================================================================

(define-interface uid-request-interface
  (export wait-for-missing-uids
	  process-uid-replies
	  make-uid-reply))

(define-structure uid-requests uid-request-interface
  (open scheme receiving define-record-types
	srfi-34 srfi-35         ;; error handling
	kali-conditions         ;; error handling
	enumerated enum-case architecture
	connect message-types
	closures locations templates
	proxy-internals address-spaces
	;weak
	records			; record?, record-ref (abstraction breaking)
	(subset primitives (untyped-indexed-set! 
			    record 
			    record-ref 
			    set-enabled-interrupts!)))
  (files uid-request))

;; =======================================================================

(define-structure messages (export remote-run! 
				   remote-apply 
				   start-server
				   alive?
				   return-counts ;; just for encode-decode-test
				   )
  (open scheme
	connect message-types uid-requests
	proxy-count-requests
	(subset signals	(warn))
	(subset primitives (unspecific))
	srfi-34 srfi-35  ;; error handling
	kali-conditions  ;; error handling
	address-spaces
	enumerated enum-case
	threads	threads-internal
	placeholders
	interrupts			; with-interrupts-inhibited
	proxy-internals)
  (files message))

;; =======================================================================

(define-interface kali-interface
  (export socket-id->address-space
	  address-space?

	  remote-run!
	  remote-apply
	  alive?

	  make-proxy
	  proxy?
	  proxy-owner
	  proxy-local-ref proxy-local-set!
	  proxy-remote-ref proxy-remote-set!
	  any-proxy-value

	  move-to!

	  start-server
	  server-running?))

(define-structure kali kali-interface
  (open scheme address-spaces
	disclose-aspaces ;; just to make shure it loads
	messages proxy-internals threads
	fluids-internal)		; current-thread
  (files utils))

;; ========================================================================

(define-interface kali-conditions-interface  
  (export (with-continuation-handler :syntax)
	  (with-except-handler :syntax)
	  &kali-error                         kali-error?

	  &kali-reader-error                  kali-reader-error?
	  kali-reader-error-channel

	  &kali-reader-eof-error              kali-reader-eof-error?

	  &kali-reader-insufficient-error     kali-reader-insufficient-error?
	  kali-reader-insufficient-error-got
	  kali-reader-insufficient-error-length

	  &kali-reader-condvar-error          kali-reader-condvar-error?

	  &kali-remote-error                  kali-remote-error?
	  kali-remote-error-aspace
	  kali-remote-error-procedure
	  kali-remote-error-arguments
	  kali-remote-error-condition
	  
	  &kali-send-message-to-self-error    kali-send-message-to-self-error?
	  kali-send-message-to-self-error-type
	  kali-send-message-to-self-error-message

	  &kali-connect-error                 kali-connect-error?
	  kali-connect-error-need-counts

	  &kali-memory-layout-error           kali-memory-layout-error?
	  kali-memory-layout-error-alien-vector
	  kali-memory-layout-error-local-vector

	  &kali-no-server-started-error       kali-no-server-started-error?
	  
	  &kali-server-already-started-error  kali-server-already-started-error?

	  &kali-bad-stob-type-error           kali-bad-stob-type-error?
	  kali-bad-stob-type-error-type

	  &kali-bad-object-error              kali-bad-object-error?
	  kali-bad-object-error-object))

(define-structure kali-conditions kali-conditions-interface
  (open scheme
	srfi-34
	srfi-35)
  (files kali-conditions))

;; ===============================================================================

(define-structure mandelbrot (export mandel
				     mandel-bench
				     test)
  (open scheme 
	threads
	kali
	address-spaces
	srfi-9
	placeholders
	posix-time)
  (files (examples mandelbrot)))

;; -------------------------------------------------

(define-interface kml-sync-interface
  (export make-kml-channel
	  kml-channel?
	  kml-send-rv
	  kml-send
	  kml-receive-rv
	  kml-receive))

(define-interface kml-async-interface
  (export make-kml-async-channel
	  kml-async-channel?
	  kml-send-async
	  kml-receive-async-rv
	  kml-receive-async))

(define-interface kml-placeholders-interface
  (export make-kml-placeholder
	  kml-placeholder?
	  kml-placeholder-value
	  kml-placeholder-set!
	  kml-placeholder-value-rv))


(define-structures ((kml-sync         kml-sync-interface)
		    (kml-async        kml-async-interface)
		    (kml-placeholders kml-placeholders-interface))
  (open scheme
	kali
	rendezvous-channels
	rendezvous-async-channels
	rendezvous-placeholders)
  (files (examples kml)))
