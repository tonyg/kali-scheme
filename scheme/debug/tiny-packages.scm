; no copyright

; (link-simple-system '(debug tiny) 'start tiny-system)

(define-structure tiny-system (export start)
  (define-all-operators)
  (usual-transforms and cond do let let* or)
  (files tiny))
