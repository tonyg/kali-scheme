; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; load into initial image

(init-defpackage! eval
		  (lambda ()
		    (delay (make-simple-package (list scheme-level-1)
						eval (delay #f) 'for-syntax))))

(define-package ((assembler (export (lap syntax))))
  (open scheme-level-2 compiler architecture
	signals				;error
	enumerated			;name->enumerand
	code-vectors
	locations)			;location?
  (specials lap)
  (files (env assem)))

(ensure-loaded assembler)

(define-package ((spatial (export space init-space)))
   (open scheme-level-2 architecture primitives packages)
   (files (debug space)))

(ensure-loaded spatial)

((*structure-ref spatial 'init-space) eval assembler)

(define-signature define-record-types-signature
  (export (define-record-type syntax)
	  define-record-discloser))
(define-package ((define-record-types define-record-types-signature))
  (open scheme-level-1 record)
  (files (rts jar-defrecord)))
(define-signature queue-signature
  (export make-queue enqueue dequeue queue-empty?
	  queue? queue->list queue-length delete-from-queue!))
(define-package ((queues queue-signature))
  (open scheme-level-1 define-record-types signals)
  (files (big queue))
  (optimize auto-integrate))
(define-package ((traverse
		  (export traverse-depth-first traverse-breadth-first trail
			  set-leaf-predicate! usual-leaf-predicate)))
  (open scheme-level-2
	primitives		; ?
	queues table
	bitwise locations closures code-vectors
	features		; string-hash
	low-level)		; flush-the-symbol-table!
  (files (env traverse)))

(ensure-loaded traverse)

(define foo
  (make-simple-package (list scheme-level-2
			     spatial traverse vm-exposure)
		       eval (delay #f) 'foo))

; (define command-processor (*structure-ref command 'command-processor))

; (set-interaction-environment! foo)
