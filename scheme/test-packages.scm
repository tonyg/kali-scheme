; Random tests, mostly for stuff in scheme/big

(define-structure misc-big-test (export misc-big-tests)
  (open scheme test-suites
	;assembler
	byte-vectors
	ports
	queues
	random
	sort
	big-scheme
	arrays
	dump/restore
	search-trees
	threads
	placeholders
	locks
	interrupts
	mask-types
	masks
	finite-types
	sockets
	(subset i/o (read-byte))
	(subset i/o-internal (open-input-port? eof-object)))
  (files (big check)))

(define-structure inversion-lists-test (export inversion-lists-tests)
  (open scheme test-suites
	inversion-lists)
  (files (big inversion-list-check)))

(define-structure big-test (export big-tests)
  (open scheme test-suites
	misc-big-test inversion-lists-test)
  (begin
    (define-test-suite big-tests
      (misc-big-tests inversion-lists-tests))))
