
;; give the outer continuation as an extra argument
;; to the handler
(define-syntax with-continuation-handler
  (syntax-rules ()
    ((with-continuation-handler handler thunk)
     (call-with-current-continuation
      (lambda (k)
	(with-exception-handler 
	 (lambda (c)
	   (handler c k))
	 thunk))))))

;; call the handler in the outer continuation
(define-syntax with-except-handler
  (syntax-rules ()
    ((with-except-handler handler thunk)
     ((call-with-current-continuation
       (lambda (k)
	 (lambda ()
	   (with-exception-handler
	    (lambda (c)
	      (k (lambda ()
		   (handler c))))
	    thunk))))))))

;; ----------------
;; conditions

;;
;;                                      +---> kali-reader-eof-error
;;            +---> kali-reader-error --+
;;            |                         +---> kali-reader-insufficient-error
;;            |
;; kali-error +
;;            |                         +---> kali-remote-apply-error
;;            |                         |
;;            +---> kali-remote-error --+
;;

(define-condition-type &kali-error &error
  kali-error?)

(define-condition-type &kali-reader-error &kali-error
  kali-reader-error?
  (port kali-reader-error-port))

(define-condition-type &kali-reader-eof-error &kali-reader-error
  kali-reader-eof-error?)

(define-condition-type &kali-reader-insufficient-error &kali-reader-error
  kali-reader-insufficient-error?
  (got     kali-reader-insufficient-error-got)
  (length  kali-reader-insufficient-error-length))

(define-condition-type &kali-remote-error &kali-error
  kali-remote-error?
  (aspace    kali-remote-error-aspace)
  (procedure kali-remote-error-procedure)
  (arguments kali-remote-error-arguments)
  (condition kali-remote-error-condition))

(define-condition-type &kali-remote-apply-error &kali-remote-error
  kali-remote-apply-error?)

