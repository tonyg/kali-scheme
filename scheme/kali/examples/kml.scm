
(define (make-kml-channel)
  (make-proxy (make-channel)))

(define (kml-channel? kml-ch)
  (and (proxy? kml-ch)
       (remote-apply (proxy-owner kml-ch)
		     (lambda (p)
		       (channel? (proxy-value p)))
		     kml-ch)))

;; TODO!
;; ...
(define kml-send-rv #f)
(define kml-send #f)
(define kml-receive-rv #f)
(define kml-receive #f)

(define make-kml-async-channel #f)
(define kml-async-channel? #f)
(define kml-send-async-rv #f)
(define kml-send-async #f)
(define kml-receive-async-rv #f)
(define kml-receive-async #f)

(define make-kml-placeholder #f)
(define kml-placeholder? #f)
(define kml-placeholder-value #f)
(define kml-placeholder-set! #f)
(define kml-placeholder-value-rv #f)
