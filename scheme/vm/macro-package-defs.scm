; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; VM-ARCHITECTURE is used in a FOR-SYNTAX clause in the VM package definitions.

(define-structures ((vm-architecture (export stob-data)))
  (open scheme enumerated)
  (files arch))
