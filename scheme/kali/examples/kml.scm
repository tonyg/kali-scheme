
(define-syntax make-proxy-remote-func
  (syntax-rules ()
    ((make-proxy-remote-func org-func-name
			     proxy
			     rest ...)
     (lambda (proxy rest ...)
       (remote-apply (proxy-owner proxy)
	 (lambda (p) 
	   (org-func-name (proxy-local-ref p)
			  rest ...))
	 proxy)))))

;; sync-channels:
;; ==============

;; :: -> kml-channel
(define (make-kml-channel)
  (make-proxy (make-channel)))

;; :: obj -> boolean
(define (kml-channel? channel)
  (and (proxy? channel)
       (remote-apply (proxy-owner channel)
	 (lambda (p)
	   (channel? (proxy-local-ref p)))
	 channel)))

;; kml-channel message -> rendezvous
(define kml-send-rv 
  (make-proxy-remote-funk send-rv
			  channel 
			  message))

;; kml-channel message -> unspecific (my block)
(define kml-send
  (make-proxy-remote-func send
			  channel 
			  message))

;; kml-channel -> rendezvous
(define kml-receive-rv 
  (make-proxy-remote-func receive-rv
			  channel))

;; kml-channel -> value (may block)
(define kml-receive
  (make-proxy-remote-func receive
			  channel))



;; async-channels:
;; ===============

;; -> async-kml-channel
(define make-kml-async-channel
  (make-proxy (make-async-channel)))

;; obj -> boolean
(define (kml-async-channel? channel)
  (and (proxy? channel)
       (remote-apply (proxy-owner channel)
	 (lambda (p)
	   (async-channel? (proxy-local-ref p)))
	 channel)))

;; async-kml-channel message -> unspecific
(define kml-send-async
  (make-proxy-remote-func send-async
			  channel
			  message))

;; async-kml-channel -> rendezvous
(define kml-receive-async-rv
  (make-proxy-remote-func receive-async-rv
			  channel))
			  
;; async-kml-channel -> value (may block)
(define kml-receive-async
  (make-proxy-remote-func receive-async
			  channel))



;; placeholders:
;; =============

;; -> empty-placeholder
(define (make-kml-placeholder)
  (make-proxy (make-placeholder)))

;; object -> boolean
(define (kml-placeholder? placeholder)
  (and (proxy? placeholder)
       (remote-apply (proxy-owner placeholder)
	 (lambda (p)
	   (placeholder? (proxy-local-ref p)))
	 placeholder)))

;; placeholder -> rendezvous
(define kml-placeholder-value-rv
  (make-proxy-remote-func placeholder-value-rv
			  placeholder))

;; placeholder -> value (may block)
(define kml-placeholder-value
  (make-proxy-remote-func placeholder-value
			  placeholder))

;; placeholder value -> unspecific
(define kml-placeholder-set!
  (make-proxy-remote-func placeholder-set!
			  placeholder))
