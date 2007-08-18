
(define-structure encode-decode-tests (export encode-decode-test)
  (open scheme
	primitives 
	kali 
	address-spaces 
	messages 
	proxy-count-requests
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

(define-structure gc-tests (export gc-test)
  (open scheme
	kali
	address-spaces
	threads
	primitives)
  (files gc))

(define-structure kali-tests (export test-all)
  (open scheme
	encode-decode-tests
	remote-tests
	record-tests
	fluid-tests
	gc-tests)
  (files do-them-all))

