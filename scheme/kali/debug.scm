
; This has various enumeration (tags, immediates, message elements) written
; in.

(define (print-cv cv)
  (do ((i 0 (+ i 4)))	
      ((= i (code-vector-length cv)))
    (display i) (display ": ")
    (display (list (code-vector-ref cv (+ i 3))
		   (code-vector-ref cv (+ i 2))
		   (code-vector-ref cv (+ i 1))
		   (code-vector-ref cv i)))
    (newline)))

(define (show-cv cv)
  (display "[Incoming message")
  (newline)
  (decode-message cv)
  (display "]")
  (newline))

; From vm/architecture

(define stob-types
  '#(pair
     symbol
     vector
     closure
     location
     channel
     port
     ratnum
     record
     continuation
     extended-number
     template
     weak-pointer
     external
     proxy			; Kali code
     proxy-data			; Kali code
     address-space		; Kali code
     unused-d-header1
     unused-d-header2
     string        ; = least b-vector type
     code-vector
     double        ; double precision floating point
     bignum))

(define string-type
  (do ((i 0 (+ i 1)))
      ((eq? 'string (vector-ref stob-types i))
       i)))

(define (decode-message cv)
  (let loop ((i 0))
    (if (= i (code-vector-length cv))
	(newline)
	(begin
	  (display i) (display ": ")
	  (let ((elt (decode-element cv i)))
	    (display elt)
	    (newline)
	    (loop (if (string? elt)
		      (+ i 4 (* 4 (quotient (+ (string-length elt) 3) 4)))
		      (+ i 4))))))))
			      
(define (decode-element cv i)
  (let* ((byte (code-vector-ref cv i))
	 (data (quotient byte 4)))
    (case (remainder (code-vector-ref cv i) 4)
      ((0)
       (quotient (extract-fixnum cv i 4) 4))
      ((1)
       (case data
	 ((0) #f)
	 ((1) #t)
	 ((2) (ascii->char (code-vector-ref cv (+ i 1))))
	 ((3) '<unspecific>)
	 ((4) '<undefined>)
	 ((5) '<eof>)
	 ((6) '())
	 (else
	  'unknown-immediate)))
      ((2)
       (let ((type (remainder data 128))
	     (len (extract-fixnum cv (+ i 1) 3)))
	 (cond ((< type string-type)
		(list 'header
		      (vector-ref stob-types type)
		      (quotient len 4)))
	       ((= type string-type)
		(let ((str (make-string len)))
		  (do ((j 0 (+ j 1)))
		      ((= j len))
		    (string-set! str j (ascii->char (code-vector-ref cv (+ i j 4)))))
		  str))
	       (else
		(list 'header
		      (vector-ref stob-types type)
		      len)))))
      ((3)
       (case (remainder data 4)
	 ((0)
	  (list 'local (- (extract-fixnum cv (+ i 1) 3) 8)))
	 ((1)
	  (list 'uid (extract-fixnum cv (+ i 1) 3)))
	 ((2)
	  (list 'uid+owner (- (extract-fixnum cv (+ i 1) 3) 8)))
	 ((3)
	  (list 'proxy (- (extract-fixnum cv (+ i 1) 3) 8))))))))

(define (extract-fixnum cv i len)
  (do ((j (+ i len -1) (- j 1))
       (r 0 (+ (code-vector-ref cv j) (* r 256))))
      ((< j i) r)))

