; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.

; Miscellaneous things.

(define-structures ((sockets (export socket-client socket-server)))
  (open scheme-level-2
        primitives)
  (files socket))  ;file removed from distribution due to copyright worries

(define-structure remote (export remote-repl
				 serve
				 make-socket)
  (open scheme
	;; For server
	closures
	handle conditions
	vm-exposure
	;; For client
	compiler
	command-processor
	interfaces
 	environments
	;; For both?
	packages
	;; packages-internal ?
	syntactic
	built-in-structures
	dump/restore
	sockets
	signals features)
  (files remote))

(define-structure requirements (export (require :syntax))
  (open scheme-level-2
	packages
	environments
	ensures-loaded
	package-commands-internal
	signals)
  (files require))


; Procedure annotations

(define-structure annotations
    (export annotate-procedure procedure-annotation)
  (open scheme-level-1 assembler)
  (files annotate))


; DOODL

(define-structure doodl doodl-interface
  (open scheme
	methods
	meta-methods
	annotations
	define-record-types
	records
	;; handle	; ignore-errors
	;; conditions	; error?
	util 
	signals)
  (files doodl))


(define-interface doodl-interface
  (export ((method
	    define-class
	    define-method
	    define-generic-function
	    define-class
	    set)
	   :syntax)
	  <function>
	  <generic-function>
	  <method>
	  <class>

	  <object>
	  <number>
	  <complex>
	  <real>
	  <rational>
	  <integer>
	  <pair>
	  <symbol>
	  <char>
	  <null>
	  <vector>
	  <string>
	  <eof-object>
	  <input-port>
	  <output-port>

	  <list>    ;foo
	  make
	  initialize
	  car-setter cdr-setter vector-ref-setter
	  id?
	  (bind :syntax)
	  ;; etc. etc.
	  ))
