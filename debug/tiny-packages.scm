; no copyright

; (link-simple-system '(debug tiny) 'start tiny-system)

(define-structure base (make-compiler-base))
(define-package ((tiny-system (export start)))
  (open base)
  (usual-transforms)
  (files tiny))
