; Little proxy utilities.

(define (proxy-owner proxy)
  (or (proxy-data-owner (proxy-data proxy))
      (local-address-space)))

(define (proxy-remote-ref proxy)
  (remote-apply (proxy-owner proxy) proxy-local-ref proxy))

(define (proxy-remote-set! proxy value)
  (remote-run! (proxy-owner proxy) proxy-local-set! proxy value))

;; Handles are shared global data structures

(define make-handle make-proxy)

(define handle-owner proxy-owner)

(define (set-handle! handle value)
  (remote-apply (handle-owner handle)
		proxy-local-set! 
		handle 
		value))

(define (handle-value handle)
  (remote-apply (handle-owner handle)
		proxy-local-ref
		handle))

; To avoid circular module dependencies we provide PROXY-REMOTE-REF to
; the rts low module.
    
(initialize-any-proxy-value! proxy-remote-ref)
    
; Move the current thread to another aspace.
(define move-to!
  (lambda (aspace)
    (call-with-current-continuation
     (lambda (k)
       (remote-run! aspace
		    spawn
		    (lambda ()
		      (k #F))
		    (thread-name (current-thread)))
       (terminate-current-thread)))))