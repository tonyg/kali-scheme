
(define-structure encode-decode-tests (export encode-decode-test)
  (open scheme
	primitives 
	kali 
	address-spaces 
	messages 
	threads 
	signals 
	srfi-42 
	srfi-66 
	srfi-78 
	srfi-9 
	reset/shift ;; ,config ,load kali/scheme/misc/packages.scm
	)
  (files encode-decode))

(define-structure remote-tests (export remote-test)
  (open scheme
	kali 
	address-spaces 
	threads 
	define-record-types 
	srfi-42 
	srfi-78)
  (files remote))

(define-structure record-tests (export record-test)
  (open scheme
	kali 
	address-spaces 
	threads 
	define-record-types
	srfi-78)
  (files records))

(define-structure fluid-tests (export fluid-test)
  (open scheme
	fluids
	kali
	threads
	srfi-78)
  (files fluids))

;; there is no structure for the gc test,
;; because it probably will lead to a 
;; stack-overflow...

(define-structure kali-tests (export test-all)
  (open scheme
	encode-decode-tests
	remote-tests
	record-tests
	fluid-tests)
  (files do-them-all.scm))

