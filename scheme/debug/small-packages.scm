; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.
; no copyright

; (link-simple-system '(debug small) 'start tiny-system)

(define-structure small-system (export start)
  (define-all-operators)
  (usual-transforms and cond do let let* or)
  (files (rts defenum) (rts arch) (debug small)))
