
(define (binary-flonum-op proc)
  (lambda (x y)
    (save-temp0! x)
    (save-temp1! y)
    (let ((key (ensure-space double-size)))
      (enter-double (proc (extract-double (recover-temp0!))
			  (extract-double (recover-temp1!)))
		    key))))

(define flonum-add      (binary-flonum-op fl+))
(define flonum-subtract (binary-flonum-op fl-))
(define flonum-multiply (binary-flonum-op fl*))

(define (flonum-divide x y)
  (if (fl= (extract-double y)
	   0.0)
      false
      ((binary-flonum-op fl/) x y)))

(define (flonum-comparison proc)
  (lambda (x y)
    (proc (extract-double x)
	  (extract-double y))))

(define flonum=  (flonum-comparison fl=))
(define flonum<  (flonum-comparison fl<))
(define flonum>  (flonum-comparison (lambda (x y)
				      (fl< y x))))
(define flonum<= (flonum-comparison (lambda (x y)
				      (not (fl< y x)))))
(define flonum>= (flonum-comparison (lambda (x y)
				      (not (fl< x y)))))
