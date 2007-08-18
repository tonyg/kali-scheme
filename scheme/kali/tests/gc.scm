;; open kali threads primitives
;; (spawn start-server)
;; (define as (socket-id->address-space <machine-name> <port>))



(define (gc-test-vector)
  (let lp ((i 10000))
    (if (zero? i)
	(collect)
	(begin
	  (let ((v (vector 1 2 3 4 5 6 7 8 9 10)))
	    (remote-apply as display v)
	    (lp (- i 1)))))))
;; -> 
;; server called 6 times gc-proxies
;; client called 3 times gc-proxies + the one forced by (collect)
;;
;; -> after (gc-test-proxy) with 1000 - after it went wrong
;; server called 7 times gc-proxies
;;         everytime with ... (wnet wrong)
;;
;; client called 3 times gc-proxies + the one forced by (collect)


;; =======================================================================
;; =======================================================================


(define (gc-test-proxy)
  (let lp ((i 1000)) ;; was 10000
    (if (zero? i)
	(collect)
	(begin
	  (let ((pv (make-proxy (vector 1 2 3 4 5 6 7 8 9 10))))
	    (remote-apply as 
			  (lambda (p)
			    (display (any-proxy-value p)))
			  pv)
	    (lp (- i 1)))))))
;; -> after (gc-test-vector) i=10000 i=1000
;; segmentation fault (core dumped)
;;
;; =======================================================================
;;
;; -> called with "fresh heap" i=10000
;; server called 4 times gc-proxies 
;;        everytime there was a
;; Error while running root thread, thread killed: #{Thread 10950 post-gc-handler}
;;
;; VM Exception: 102
;;     wrong-type-argument
;;     #((1 0 #{Procedure 9786 (unnamed in lp in gc-test-proxy)} #{Proxy #{Address-space 192.168.0.127 1479} 3}) () () 0)
;;     13
;;     0
;;
;; before
;; then "segmentation fault (corde dumped)" again
;;
;; client called 3 times gc-proxies
;;
;; =======================================================================
;;
;; -> with "fresh heap" and i = 1000
;; server called 1 time gc-proxies - went wrong...
;; clinet called 0 times gc-proxies + the one forced by (collect)

