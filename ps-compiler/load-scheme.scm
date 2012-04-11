; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1998 by NEC Research Institute, Inc.    See file COPYING.

; Load the Scheme front-end

(config)
(load "interfaces.scm")
(load "package-defs.scm")
(load "scheme-to-c/package-defs.scm")
(load-package 'let-nodes)       ; used in FOR-SYNTAX
(load-package 'simp-patterns)   ; used in FOR-SYNTAX
(load-package 'scheme-test)
