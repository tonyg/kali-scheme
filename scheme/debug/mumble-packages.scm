; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is for version 0.28
; We define these two because they aren't reified:
;   scheme-level-0
;   silly

; We redefine these two so as not to compromise the security of the
; built-in exception and interrupt systems:
;   primitives
;   signals


; Suppose you have just done "make image" to build the scheme48 heap image.
; Suppose that the linker produces an initial.image, but that when that
; image is resumed you get the error
;     exception handler is not a closure
; This is not informative.  To find out what really happened, you have
; two choices:
;   (1) Run the image under the VM running in Scheme.
;   (2) Run the image using the following handy dandy tool.
; For choice (2), you would do something like the following:
;
;  ,translate =scheme48/ ./
;  ,config ,load debug/mumble-packages.scm
;  ,in link-config
;  y
;  ;; Cf. Makefile rule for initial.image
;  ,load interfaces.scm packages.scm debug/fix-low.scm
;  (flatload initial-structures)
;  ,load initial.scm
;  (define test (link-initial-system))

; primitives-internal
; ,open ##

; Replacement for the structure defined in link-packages.scm
(define-structure linker (export link-simple-system
				 link-reified-system
				 (struct-list :syntax))
  (open scheme
        packages		;make-simple-package
        reification
        ensures-loaded
	)		; (enum interrupt keyboard)
  (files linker))

; Copied from link-packages.scm
(define-structure loadc (export load-configuration
				(structure-ref :syntax))
  (open scheme
	environments		; *structure-ref
	fluids)
  (files ((".." link) loadc)))

; Replacement for the structure defined in link-packages.scm

(define-structure link-config (export )  ;dummy structure...
  (open scheme
	linker
	;; low-structures
	;; start-debugging
	defpackage
	types
	analysis
	structure-refs    ;the real one
	loadc		  ;defines structure-ref, but not the one we want
	flatloading
	ensures-loaded
	interfaces)
  (access built-in-structures)
  (begin 0))
