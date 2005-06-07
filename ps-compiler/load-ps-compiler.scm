; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.

; -*- Mode: Scheme; -*-

; To load the Pre-Scheme compiler into Scheme 48:
;   ,exec ,load load-ps-compiler.scm
; It needs a larger than default sized heap.  4000000 is big enough to
; load the pre-scheme compiler but not big enough to compile the VM,
; 12000000 is enough to compile the VM.
;
; compile-vm.exec is an exec script to compile the Scheme 48 virtual machine.
;
; This requires that Pre-Scheme already be loaded.

(user '(run (let ((minor-number (call-with-input-file
				    "minor-version-number"
				  (lambda (in)
				    (read in)))))
	      (newline)
	      (newline)
	      (display "Pre-Scheme compiler version 1.")
	      (display minor-number)
	      (newline)
	      (display "Copyright (c) 1993-2004 by Richard Kelsey.")
	      (newline)
	      (display "Please report bugs to pre-scheme@s48.org.")
	      (newline)
	      (newline))))


(config)
(structure 'reflective-tower-maker
           '(export-reflective-tower-maker))
(load "interfaces.scm")
(load "package-defs.scm")
(load "prescheme/interfaces.scm")
(load "prescheme/package-defs.scm")
(load-package 'let-nodes)       ; used in FOR-SYNTAX
(load-package 'simp-patterns)   ; used in FOR-SYNTAX
(load-package 'prescheme-compiler)
