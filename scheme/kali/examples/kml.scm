;; writing a kali-interface for the cml-package is quiet
;; a bit harder than i thought at first ->
;; about pretty much in this file i'm not shure wether
;; it will work or not... and even worse, about some things
;; i'm shure they won't.

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

(define-syntax make-proxy-remote-func-rv
  (syntax-rules ()
    ((make-proxy-remote-func org-func-name
			     proxy
			     rest ...)
     (lambda (proxy rest ...)
       (rv->kml-rv (remote-apply (proxy-owner proxy)
		     (lambda (p) 
		       (org-func-name (proxy-local-ref p)
				      rest ...))
		     proxy))))))

;; -------

(define rv->kml-rv make-proxy)

(define (kml-rv->rv kml-rv)
  (remote-apply (proxy-owner kml-rv)
    (lambda (p)
      (proxy-local-ref p))
    kml-rv))

;(define choose->kml-choose make-proxy)
;(define guard->kml-guard make-proxy)
;(define with-nack->kml-with-nack make-proxy)

;; -------

;; rendezvous:
;; ===========

(define kml-never-rv (rv->kml-rv never-rv))

(define (kml-always-rv val)
  (rv->kml-rv (always-rv val)))

;; probably guard, with-nack, wrap
;; and rv->kml-rv in special
;; should run the server-side...
;; chnx todo
 
;; how do get server-side here?
;; -> probably we need a complete other
;; concept for such an interface...
(define (kml-guard kml-rv-generator)
  (rv->kml-rv 
   (guard (lambda ()
	    (kml-rv->rv (kml-rv-generator))))))

;; again i can't find out server-side
;; an perhaps there is not only one anyway!
(define (kml-with-nack kml-rv-generator)
  (rv->kml-rv 
   (with-nack (lambda ()
		(kml-rv->rv (kml-rv-generator))))))

;; chnx todo:
;; this won't work like this...
;; there can be more server-sides again...
(define (kml-choose . kml-rvs)
  (rv->kml-rv (choose (map kml-rv->rv
			   kml-rvs))))

;; this looks quiet good again - i hope.
(define (kml-wrap kml-rv procedure)
  (remote-apply (proxy-owner kml-rv)
    (lambda (p)
      (make-proxy (wrap (proxy-local-ref p)
			procedure)))
    kml-rv))

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

;; :: kml-channel message -> rendezvous
(define kml-send-rv 
  (make-proxy-remote-func send-rv
			  channel 
			  message))

;; :: kml-channel message -> unspecific (my block)
(define kml-send
  (make-proxy-remote-func send
			  channel 
			  message))

;; :: kml-channel -> rendezvous
(define kml-receive-rv 
  (make-proxy-remote-func receive-rv
			  channel))

;; :: kml-channel -> value (may block)
(define kml-receive
  (make-proxy-remote-func receive
			  channel))



;; async-channels:
;; ===============

;; :: -> async-kml-channel
(define (make-kml-async-channel)
  (make-proxy (make-async-channel)))

;; :: obj -> boolean
(define (kml-async-channel? channel)
  (and (proxy? channel)
       (remote-apply (proxy-owner channel)
	 (lambda (p)
	   (async-channel? (proxy-local-ref p)))
	 channel)))

;; :: async-kml-channel message -> unspecific
(define kml-send-async
  (make-proxy-remote-func send-async
			  channel
			  message))

;; :: async-kml-channel -> rendezvous
(define kml-receive-async-rv
  (make-proxy-remote-func receive-async-rv
			  channel))
			  
;; :: async-kml-channel -> value (may block)
(define kml-receive-async
  (make-proxy-remote-func receive-async
			  channel))



;; placeholders:
;; =============

;; :: -> empty-placeholder
(define (make-kml-placeholder)
  (make-proxy (make-placeholder)))

;; :: object -> boolean
(define (kml-placeholder? placeholder)
  (and (proxy? placeholder)
       (remote-apply (proxy-owner placeholder)
	 (lambda (p)
	   (placeholder? (proxy-local-ref p)))
	 placeholder)))

;; :: placeholder -> rendezvous
(define kml-placeholder-value-rv
  (make-proxy-remote-func placeholder-value-rv
			  placeholder))

;; :: placeholder -> value (may block)
(define kml-placeholder-value
  (make-proxy-remote-func placeholder-value
			  placeholder))

;; :: placeholder value -> unspecific
(define kml-placeholder-set!
  (make-proxy-remote-func placeholder-set!
			  placeholder))
