(define-structure trans-ids trans-ids-interface
  (open scheme
	big-util
	cells
	threads (subset threads-internal (maybe-commit-and-block))
	proposals)
  (files trans-id))

(define-structures ((rendezvous rendezvous-interface)
		    (make-rendezvous make-rendezvous-interface))
  (open scheme
	define-record-types
	trans-ids
	threads
	threads-internal		; ####can we remove this?
	proposals
	big-util
	queues
	(subset util (unspecific))
	debug-messages
	)
  (files rendezvous))

(define-structure rendezvous-channels rendezvous-channels-interface
  (open scheme
	define-record-types
	trans-ids proposals
	rendezvous make-rendezvous
	queues
	big-util
	(subset util (unspecific))
	debug-messages
	)
  (files channel))

(define-structure rendezvous-async-channels rendezvous-async-channels-interface
  (open scheme
	rendezvous
	rendezvous-channels
	threads
	queues
	define-record-types)
  (files async-channel))

(define-structure rendezvous-placeholders rendezvous-placeholders-interface
  (open scheme
	define-record-types
	(subset threads-internal (maybe-commit-and-make-ready))
	proposals
	trans-ids rendezvous make-rendezvous
	queues
	signals
	(subset util (unspecific)))
  (files placeholder))

(define-structure rendezvous-jars rendezvous-jars-interface
  (open scheme
	define-record-types
	proposals (subset threads-internal (maybe-commit-and-make-ready))
	trans-ids rendezvous make-rendezvous
	queues
	signals
	(subset util (unspecific)))
  (files jar))
