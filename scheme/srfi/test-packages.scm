(define-structure srfi-14-test (export srfi-14-tests srfi-14-slow-tests)

  (open scheme test-suites
	unicode
	srfi-14)
  (files srfi-14-check))
