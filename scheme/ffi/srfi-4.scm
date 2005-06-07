(define-syntax make-srfi-4
  (lambda (form r compare)
    (destructure (((make-srfi-4 tag type) form))
      