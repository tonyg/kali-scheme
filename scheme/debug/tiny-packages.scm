; no copyright

; (link-simple-system '(debug tiny) 'start tiny-system)

(define-structure tiny-system (export start)
  (define-all-operators)
  (files tiny))
