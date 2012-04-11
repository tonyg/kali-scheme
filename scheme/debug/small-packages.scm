; no copyright

; (link-simple-system '(debug small) 'start tiny-system)

(define-structure small-system (export start)
  (define-all-operators)
  (usual-transforms and cond do let let* or)
  (files (rts defenum) (rts arch) (debug small)))
