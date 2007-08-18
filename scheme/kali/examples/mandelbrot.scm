;; ,open threads kali address-spaces srfi-9 placeholders posix-time

;;
;;

;; =========================================================================
;; records:
;;

(define-record-type mandel-piece
  (make-mandel-piece upper-left  ;; point
		     lower-right ;; point
		     x-res
		     y-res)
  mandel-piece?
  (upper-left mandel-piece-upper-left)
  (lower-right mandel-piece-lower-right)
  (x-res mandel-piece-x-res)
  (y-res mandel-piece-y-res))

(define-record-type point
  (really-make-point x y)
  point?
  (x point-x)
  (y point-y))

(define (make-point x y)
  (really-make-point (* x 1.0)   ;; just to make shure...
		     (* y 1.0))) ;; if there is enough power, address-spaces,
				 ;; or time, then you can make it exact!

;; =========================================================================
;; the starter functions:
;;

(define (mandel x-min x-max 
		y-min y-max 
		x-res y-res 
		iter-limit aspaces . divide)
  (let ((divide (if (null? divide)
		    1
		    (car divide))))
    (save-pixel-field-as-tga (calculate-mandel (make-mandel-piece 
						(make-point x-min y-max) ;; upper-left
						(make-point x-max y-min) ;; lower-right
						x-res y-res) ;; resulting tga size
					       iter-limit
					       aspaces
					       divide)
			     (make-file-name mandel-piece
					     iter-limit
					     divide))))

(define (mandel-bench x-min x-max 
		      y-min y-max 
		      x-res y-res 
		      iter-limit aspaces . divide)
  (let* ((divide (if (null? divide)
		     1
		     (car divide)))
	 (mandel-piece (make-mandel-piece (make-point x-min y-max) ;; upper left
					  (make-point x-max y-min) ;; lower right
					  x-res y-res)) ;; resulting tga size
	 (seconds1 (time-seconds (current-time)))
	 (mandel-pixel-field (calculate-mandel mandel-piece 
					       iter-limit 
					       aspaces 
					       divide))
	 (seconds2 (time-seconds (current-time)))
	 (file-name (make-file-name mandel-piece 
				    iter-limit 
				    divide))
	 (save-stat (save-pixel-field-as-tga mandel-pixel-field 
					     file-name))
	 (seconds3 (time-seconds (current-time))))
    (for-each display `("Calculated mandel-pixel-field in " ,(- seconds2
								seconds1)
			" seconds." #\newline
			"Wrote file \"" file-name "\" in " ,(- seconds3 
							       seconds2)
			" seconds." #\newline))))
			
(define (test aspace1 aspace2)
  (save-pixel-field-as-tga (calculate-mandel (make-mandel-piece (make-point -2 2)
								(make-point 2 -2)
								50 50)
					     64
					     `(,aspace1 ,aspace2)
					     1)
			   "MANDEL-TEST.tga"))

;; ============================================================================
;; the actual calculation:


;; first divide the piece in pieces for the address-spaces

(define (calculate-mandel mandel-piece iter-limit aspaces divide)
  (if (zero? divide)
      (calculate-mandel-piece mandel-piece iter-limit)
      (let ((number-pieces (length aspaces)))
	(call-with-values
	    (lambda ()
	      (divide-mandel-piece mandel-piece 
				   number-pieces))
	  (lambda (combine mandel-pieces)
	    (let* ((n (length mandel-pieces))
		   (field-placeholders (construct-list number-pieces
						       make-placeholder)))
	      (let loop ((i 0))
		(if (= i n)
		    (values)
		    (begin
		      (spawn 
		       (lambda () (placeholder-set! 
				   (list-ref field-placeholders i)
				   (remote-apply (list-ref aspaces i)
						 calculate-mandel 
						 (list-ref mandel-pieces
							   i)
						 iter-limit
						 aspaces
						 (- divide 1)))))
		      (loop (+ i 1)))))
	      (apply combine (map (lambda (ph)
				    (placeholder-value ph))
				  field-placeholders))))))))

;; walk the pixel-field an fill it with colour-indices 
;; calculated with iterate

(define (calculate-mandel-piece mandel-piece iter-limit)
  (let* ((x-res (mandel-piece-x-res mandel-piece))
	 (y-res (mandel-piece-y-res mandel-piece))
	 (pixel2point (make-pixel2point mandel-piece))
	 (pixel-field (make-pixel-field x-res y-res 0)))
    (let x-loop ((x 0))
      (let y-loop ((y 0))
	(if (= y y-res)
	    (if (= x (- x-res 1))
		pixel-field
		(x-loop (+ x 1)))
	    (let* ((point (pixel2point x y))
		   (iter (iterate point
				  iter-limit)))
	      (pixel-field-set! pixel-field 
			   x y
			   (if (= iter iter-limit)
			       0
			       (iter->colour-int1 iter iter-limit 50)))
	      (y-loop (+ y 1))))))))

;; make a funtion that
;; transforms pixel-indices to point-coordinates in
;; complex number plane

(define (make-pixel2point mandel-piece)
  (let* ((upper-left (mandel-piece-upper-left mandel-piece))
	 (lower-right (mandel-piece-lower-right mandel-piece))
	 (upper-left-x (point-x upper-left))
	 (upper-left-y (point-y upper-left))
	 (lower-right-x (point-x lower-right))
	 (lower-right-y (point-y lower-right))
	 (delta-x (- lower-right-x upper-left-x))
	 (delta-y (- upper-left-y lower-right-y))
	 (x-res (mandel-piece-x-res mandel-piece))
	 (y-res (mandel-piece-y-res mandel-piece))
	 (step-size-x (/ delta-x x-res))
	 (step-size-y (/ delta-y y-res)))
    (lambda (x y)
      (make-point (+ upper-left-x (* step-size-x x))
		  (- upper-left-y (* step-size-y y))))))

;; iterate ;-)

(define (iterate point iter-limit)
  (let ((x0 (point-x point))
	(y0 (point-y point)))
    (let loop ((n iter-limit)
	       (x x0)
	       (y y0))
      (let ((xx (expt x 2))
	    (yy (expt y 2)))
	(if (or (zero? n)
		(> (+ xx yy)
		   8))
	    (- iter-limit n)
	    (loop (- n 1)
		  (+ (- xx yy)
		     x0)
		  (+ (* 2 x y)
		     y0)))))))

;; ================================================================================
;; divide and conquer - functions for divide mandel-pieces 
;;                      and for recombining the assoziated pixel-fields.
;; 


;; -------
;; divide

;; returns adequate combine-pixel-field-funtion and a list of mandel-pieces
(define (divide-mandel-piece mandel-piece n)
  (if (> (mandel-piece-x-res mandel-piece)
	 (mandel-piece-y-res mandel-piece))
	(values combine-pixel-field-horizontal
		(reverse (cut-mandel-piece mandel-piece
					   cut-piece-vertical
					   n)))
	(values combine-pixel-field-vertical
		(reverse (cut-mandel-piece mandel-piece 
					   cut-piece-horizontal
					   n)))))

(define (cut-mandel-piece mandel-piece cutter n)
  (let loop ((n n)
	     (pieces '())
	     (mp mandel-piece))
    (if (= n 1)
	(cons mp pieces)
	(call-with-values 
	    (lambda ()
	      (cutter mp n))
	  (lambda (piece rest-piece)
	    (loop (- n 1)
		  (cons piece pieces)
		  rest-piece))))))

(define (cut-piece-vertical mandel-piece n)
  (let* ((pixel2point (make-pixel2point mandel-piece))
	 (x-res (mandel-piece-x-res mandel-piece))
	 (y-res (mandel-piece-y-res mandel-piece))
	 (x-res1 (quotient x-res n))
	 (x-res2 (- x-res x-res1)))
    (values (make-mandel-piece (mandel-piece-upper-left mandel-piece)
			       (pixel2point x-res1 y-res)
			       x-res1 y-res)
	    (make-mandel-piece (pixel2point (+ x-res1 1) 0)
			       (mandel-piece-lower-right mandel-piece)
			       x-res2 y-res))))

(define (cut-piece-horizontal mandel-piece n)
  (let* ((pixel2point (make-pixel2point mandel-piece))
	 (x-res (mandel-piece-x-res mandel-piece))
	 (y-res (mandel-piece-y-res mandel-piece))
	 (y-res1 (quotient y-res n))
	 (y-res2 (- y-res y-res1)))
    (values (make-mandel-piece (mandel-piece-upper-left mandel-piece)
			       (pixel2point x-res y-res1)
			       x-res y-res1)
	    (make-mandel-piece (pixel2point 0 (+ y-res1 1))
			       (mandel-piece-lower-right mandel-piece)
			       x-res y-res2))))

;; -------
;; combine

(define (combine-pixel-field-horizontal pixel-field . rest)
  (if (null? rest)
      pixel-field
      (apply combine-pixel-field-horizontal
	     (apply vector
		    (map (lambda (v1 v2)
			   (apply vector 
				  (append (vector->list v1)
					  (vector->list v2))))
			 (vector->list pixel-field)
			 (vector->list (car rest))))
	     (cdr rest))))

(define (combine-pixel-field-vertical pixel-field . rest)
  (if (null? rest)
      pixel-field
      (apply combine-pixel-field-vertical 
	     (apply vector
		    (append (vector->list pixel-field)
			    (vector->list (car rest))))
	     (cdr rest))))

;; ========================================================================
;; tga helpers

(define (save-pixel-field-as-tga pixel-field path)
  (let ((x-dim (vector-length (vector-ref pixel-field 0)))
	(y-dim (vector-length pixel-field)))
    (with-output-to-file path
      (lambda ()
	;; header ...
	(write-list
		  (list #x00 #x00  #x02 #x00  #x00 #x00  #x00 #x00
			#x00 #x00  #x00 #x00))
	;; header resolution
	(write-list (integer->two-byte-list x-dim))
	(write-list (integer->two-byte-list y-dim))
	;; header bits per pixel
	(write-list 
		  (list #x18 #x00))
	(write-pixel-field-reversed pixel-field x-dim y-dim)))))

(define (write-pixel-field-reversed pixel-field x-dim y-dim)
  (let y-loop ((y (- y-dim 1)))
    (let x-loop ((x 0))
      (if (= x x-dim)
	  (if (= y 0)
	      (values)
	      (y-loop (- y 1)))
	  (begin
	    (write-list 
	     (integer->rgb-list (pixel-field-ref pixel-field 
						 x y)))
	    (x-loop (+ x 1)))))))

(define (write-list lst)
  (for-each (lambda (int)
	      (display (integer->char int)))
	    lst))

(define (integer->two-byte-list integer)
  (list (modulo integer 256)
	(quotient integer 256)))

;; ========================================================================
;; helpers

(define (make-pixel-field x y init)
  (apply vector 
	 (construct-list y 
			 (lambda ()
			   (make-vector x init)))))

(define (pixel-field-ref pixel-field x y)
  (vector-ref (vector-ref pixel-field y) x))

(define (pixel-field-set! pixel-field x y value)
  (vector-set! (vector-ref pixel-field y)
	       x
	       value))

(define (construct-list n constructor)
  (let lp ((n n))
    (if (zero? n)
	'()
	(cons (constructor)
	      (lp (- n 1))))))

(define (make-file-name mandel-piece iter-limit divide)
  (let* ((upper-left (mandel-piece-upper-left mandel-piece))
	 (lower-right (mandel-piece-lower-right mandel-piece))
	 (upper-left-x (point-x upper-left))
	 (upper-left-y (point-y upper-left))
	 (lower-right-x (point-x lower-right))
	 (lower-right-y (point-y lower-right))
	 (x-res (mandel-piece-x-res mandel-piece))
	 (y-res (mandel-piece-y-res mandel-piece)))
    (string-append "LO_" (number->string upper-left-x)
		   "_" (number->string upper-left-y)
		   "_RU_" (number->string lower-right-x)
		   "_" (number->string lower-right-y)
		   "_res_" (number->string x-res)
		   "x" (number->string y-res)
		   "_iter_" (number->string iter-limit)
		   "_devi_" (number->string divide)
		   ".tga")))

;; ----------------------------------------------
;; colour stuff...

;; see integer->rgb-list
;; (+ 256 256 256) => 768
;; (* 256 256 256) => 16777216
(define (iter->colour-int1 iter deep scale-up)
  (inexact->exact (round (+ (* iter 
			       (/ (- 768 
				     scale-up) 
				  deep))
			    scale-up))))

(define (integer->rgb-list integer)
  (let ((red (if (> integer 255)
		 255
		 integer))
	(yellow (if (> integer 511)
		    255
		    (max 0
			 (- integer 256))))
	(blue (max 0
		   (- integer 512))))
  (list blue yellow red)))
