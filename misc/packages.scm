; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



(define-package ((remote (export remote-repl
				 serve
				 make-socket)))
  (open scheme
	;; For server
	closures
	handle condition
	vm-exposure
	;; For client
	compiler
	command
	;; For both
	packages built-in-structures
	dump/restore
	sockets
	signals features)
  (files remote))

(define-package ((requirements (export (require syntax))))
  (open scheme-level-2
	packages
	environments
	ensures-loaded
	package-commands
	signals)
  (files require))

