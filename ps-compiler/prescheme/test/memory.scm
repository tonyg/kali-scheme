
(define (test x i)
  (let ((m (allocate-memory 10)))
    (unsigned-byte-set! (+ m i) x)
    (word-set! (+ m i) (+ 1 (word-ref (+ m i))))
    (unsigned-byte-ref (+ m i))))
