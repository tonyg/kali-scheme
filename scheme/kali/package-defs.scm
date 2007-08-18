
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
	  ))

(define-structure address-spaces address-space-interface
  (open scheme address-space-internals
	define-record-types
	ascii tables
	signals			; error
	architecture		; stob
	templates locations	; set-template-id! set-location-uid!
	interrupts		; with-interrupts-inhibited
	proxy-internals		; set-proxy-uid! set-proxy-owner-uid!
	locks			; make-lock
	(subset sockets (get-host-name))
	(subset posix-time (time->string current-time))
	(subset primitives (encode collect find-all)))		; vm-extension copy-bytes! find-all-xs encode
  (files aspace))

;; ===========================================================================

(define-structure message-types (export (message-type :syntax))
  (open scheme enumerated)
  (begin
    (define-enumeration message-type
      (run			; (proc . args)
       apply			; (return-id proc . args)
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
