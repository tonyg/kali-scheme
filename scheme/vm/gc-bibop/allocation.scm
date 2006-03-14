; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Interface to the VM for allocation

;; Everything else is defined via bibop-gc-external

;; For allocation done outside the VM.

(define (s48-allocate-stob type size)
  (let* ((traced? (< type least-b-vector-type))
	 (length-in-bytes (if traced?
			      (cells->bytes size)
			      size))
	 (needed (+ length-in-bytes (cells->bytes stob-overhead)))
	 (thing (if traced?
		    (s48-allocate-traced+gc needed)
		    (s48-allocate-untraced+gc needed))))
    (if (null-address? thing)
	(error "insufficient heap space for external allocation"))
    (store! thing (make-header type length-in-bytes))
    (address->stob-descriptor (address+ thing
                                        (cells->bytes stob-overhead)))))
