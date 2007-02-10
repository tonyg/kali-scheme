(define-structure posix-core-test (export posix-core-tests)
  (open scheme test-suites sort threads
	util		; every
	posix-files
	posix-time
	posix-users
	posix-i/o
	posix-process-data
	posix-processes
	os-strings)
  (files check))

(define-structure regexp-test (export regexp-tests)
  (open scheme test-suites
	regexps)
  (files regexp-check))

(define-structure posix-test (export posix-tests)
  (open scheme test-suites
	posix-core-test
	regexp-test)
  (begin
    (define-test-suite posix-tests (posix-core-tests regexp-tests))))
