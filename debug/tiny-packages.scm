; no copyright

; (link-simple-system '(debug tiny) 'start tiny-system)

(define base (make-compiler-base))
(define-structure tiny-system (export start)
  (open base)
  (usual-transforms)
  (files tiny))
