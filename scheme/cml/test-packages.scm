(define-structure cml-test (export rendezvous-channels-tests
				   rendezvous-jars-tests
				   rendezvous-placeholders-tests
				   with-nack-tests
				   cml-tests ; all combined
				   )
  (open scheme test-suites
	threads
	srfi-1
	rendezvous rendezvous-channels
	rendezvous-jars
	rendezvous-placeholders)
  (files cml-check))

