(define-structure srfi-14-test (export srfi-14-tests srfi-14-slow-tests)

  (open scheme test-suites
	unicode
	srfi-14)
  (files srfi-14-check))

(define-structure srfi-19-test (export srfi-19-tests)
  (open scheme 
	srfi-9 ; DEFINE-RECORD-PROCEDURES
        srfi-19
        srfi-34 ; exceptions
        srfi-35 ; conditions
	test-suites
        formats)
  (files srfi-19-check))

(define-structure srfi-test (export srfi-tests)
  (open scheme test-suites
	srfi-14-test
	srfi-19-test)
  (begin
    (define-test-suite srfi-tests (srfi-14-tests srfi-19-tests))))
