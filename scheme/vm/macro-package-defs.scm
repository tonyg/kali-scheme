; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; VM-ARCHITECTURE is used in a FOR-SYNTAX clause in the VM package definitions.

(define-structures ((vm-architecture (export stob-data)))
  (open scheme enumerated platform)
  (files (interp arch)))
