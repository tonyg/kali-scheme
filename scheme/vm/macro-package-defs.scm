; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


; VM-ARCHITECTURE is used in a FOR-SYNTAX clause in the VM package definitions.

(define-structures ((vm-architecture (export stob-data)))
  (open scheme enumerated)
  (files arch))
