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
    (let ((mandel-piece (make-mandel-piece 
			 (make-point x-min y-max) ;; upper-left
			 (make-point x-max y-min) ;; lower-right
			 x-res y-res))) ;; resulting tga size
      (save-pixel-field-as-tga (calculate-mandel mandel-piece
						 iter-limit
						 aspaces
						 divide)
			       (make-file-name mandel-piece
					       iter-limit
					       (length aspaces)
					       divide)))))

(define (mandel-bench x-min x-max 
		      y-min y-max 
		      x-res y-res 
		      iter-limit aspaces . divide)
  (let* ((divide (if (null? divide)
		     1
		     (car divide)))
	 (mandel-piece (make-mandel-piece (make-point x-min y-max) ;; upper left
					  (make-point x-max y-min) ;; lower right
					  x-res y-res)) ;; resolution resulting tga size
	 (seconds1 (time-seconds (current-time)))
	 (mandel-pixel-field (calculate-mandel mandel-piece 
					       iter-limit 
					       aspaces 
					       divide))
	 (seconds2 (time-seconds (current-time)))
	 (file-name (make-file-name mandel-piece 
				    iter-limit
				    (length aspaces)
				    divide
				    (- seconds2 seconds1)))
	 (save-stat (save-pixel-field-as-tga mandel-pixel-field 
					     file-name))
	 (seconds3 (time-seconds (current-time))))
    (for-each display `("Calculated mandel-pixel-field in " ,(- seconds2
								seconds1)
			" seconds." #\newline
			"Wrote file \"" ,file-name "\" in " ,(- seconds3 
								seconds2)
			" seconds." #\newline))))
			
(define (mandel-test aspace1 aspace2 . n)
  (let* ((seconds1 (time-seconds (current-time)))
	 (pixel-field (calculate-mandel (make-mandel-piece (make-point -2 2)
							   (make-point 2 -2)
							   50 50)
					64
					`(,aspace1 ,aspace2)
					(if (null? n)
					    1
					    (car n))))
	 (seconds2 (time-seconds (current-time))))
    (for-each display
	      `("Test finished in " ,(- seconds2 seconds1) " seconds."
		#\newline #\newline))
    (save-pixel-field-as-tga pixel-field
			     "MANDEL-TEST.tga")))

;; ============================================================================
;; the actual calculation:


;; first divide the piece in pieces for the address-spaces

(define (calculate-mandel mandel-piece iter-limit aspaces divide)
  (debug-message "calculate-mandel")
  (if (zero? divide)
      (begin
	(debug-message "calculate-mandel - really-calc!")
	(let ((result (calculate-mandel-piece mandel-piece iter-limit)))
	  (debug-message "calculate-mandel - really-calc returned!")
	  result))
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
		       (lambda () 
			 (let ((calculate-res (remote-apply (list-ref aspaces i)
						calculate-mandel 
						(list-ref mandel-pieces
							  i)
						iter-limit
						aspaces
						(- divide 1))))
			   (debug-message "calculate-mandel - got calculate-res - going to set!")
			   (placeholder-set! (list-ref field-placeholders i)
					     calculate-res))))
		      (loop (+ i 1)))))
	      (debug-message "calculate-mandel - going to combine")
	      (apply combine (map (lambda (ph)
				    (placeholder-value ph))
				  field-placeholders))))))))

;; walk the pixel-field an fill it with colour-indices 
;; calculated with iterate

(define (calculate-mandel-piece mandel-piece iter-limit)
  (debug-message "calculate-amdel-piece")
  (display "Going to calculate mandel-piece: ")
  (newline)
  (display-mandel-piece mandel-piece)
  (let* ((seconds1 (time-seconds (current-time)))
	 (x-res (mandel-piece-x-res mandel-piece))
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
	      (y-loop (+ y 1))))))
    (let ((seconds2 (time-seconds (current-time))))
      (display "Finished calculation in ")
      (display (- seconds2 seconds1))
      (display " seconds.")
      (newline)
      pixel-field)))

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
	(values combine-pixel-field-columns
		(reverse (cut-mandel-piece mandel-piece
					   cut-piece-into-columns
					   n)))
	(values combine-pixel-field-rows
		(reverse (cut-mandel-piece mandel-piece 
					   cut-piece-into-rows
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

(define (cut-piece-into-columns mandel-piece n)
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

(define (cut-piece-into-rows mandel-piece n)
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

(define (combine-pixel-field-columns pixel-field . rest)
  (debug-message "combine-pixel-field-columns")
  (if (null? rest)
      pixel-field
      (apply combine-pixel-field-columns
	     (apply vector
		    (map (lambda (v1 v2)
			   (apply vector 
				  (append (vector->list v1)
					  (vector->list v2))))
			 (vector->list pixel-field)
			 (vector->list (car rest))))
	     (cdr rest))))

(define (combine-pixel-field-rows pixel-field . rest)
  (debug-message "combine-pixel-field-rows")
  (if (null? rest)
      pixel-field
      (apply combine-pixel-field-rows
	     (apply vector
		    (append (vector->list pixel-field)
			    (vector->list (car rest))))
	     (cdr rest))))

;; ========================================================================
;; tga helpers

(define (save-pixel-field-as-tga pixel-field path)
  (debug-message "save-pixel-field-as-tga")
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
	(write-pixel-field-reversed pixel-field x-dim y-dim)))
    (debug-message "save-pixel-field-as-tga - did it")
    #t))

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

(define (make-file-name mandel-piece iter-limit no-as divide . time)
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
		   "_no-as_" (number->string no-as)
		   "_devi_" (number->string divide)
		   (if (null? time)
		       ""
		       (string-append "_dt_"
				      (number->string (car time))))
		   ".tga")))

(define (display-mandel-piece mandel-piece)
  (let* ((upper-left  (mandel-piece-upper-left mandel-piece))
	 (lower-right (mandel-piece-lower-right mandel-piece))
	 (x-min (point-x upper-left))
	 (x-max (point-x lower-right))
	 (y-min (point-y lower-right))
	 (y-max (point-y upper-left))
	 (x-res (mandel-piece-x-res mandel-piece))
	 (y-res (mandel-piece-y-res mandel-piece)))
    (for-each display
	      `(#\newline
		"range     : " ,x-min " <= x <= " ,x-max #\newline
		"            " ,y-min " <= y <= " ,y-max #\newline
		"resolution: " ,x-res " x " ,y-res #\newline
		#\newline))))

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

;; ----------------

(define (debug-message . objs)
  (for-each display objs)
  (newline))
