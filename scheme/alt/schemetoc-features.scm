; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; BUG: (+ (expt 2 28) (expt 2 28)), (* (expt 2 28) 2)

(define-external schemetoc-error ;(schemetoc-error symbol format-string . args)
  "scdebug" "error_v")

(eval-when (eval)
  (define schemetoc-error error))


; SIGNALS

(define (error who message . irritants)
  (if (symbol? message)
      (apply schemetoc-error message irritants)
      (apply schemetoc-error
	     "Error:"
	     (apply string-append
		    message
		    (map (lambda (x) "~%      ~s")
			 irritants))
	     irritants)))

(define (assertion-violation who message . irritants)
  (apply error who message irritants))

(define (implementation-restriction-violation who message . irritants)
  (apply error who message irritants))

(define (warning  who message . irritants)
  (display-error-message "Warning: " message irritants))

(define (note who message . irritants)
  (display-error-message "Note: " message irritants))

(define (display-error-message heading message irritants)
  (display heading)
  (display message)
  (newline)
  (let ((spaces (list->string
		 (map (lambda (c) #\space) (string->list heading)))))
    (for-each (lambda (irritant)
		(display spaces)
		(write irritant)
		(newline))
	      irritants)))

(define (syntax-violation who message . irritants)
  (apply warning who message irritants)
  ''syntax-error)

; FEATURES

(define force-output flush-buffer)

(define (string-hash s)
  (let ((n (string-length s)))
    (do ((i 0 (+ i 1))
         (h 0 (+ h (char->ascii (string-ref s i)))))
        ((>= i n) h))))

(define (make-immutable! thing) thing)
(define (immutable? thing) #f)


; BITWISE

(define (arithmetic-shift x n)
  (if (< x 0)
      (let ((r (- -1 (arithmetic-shift (- -1 x) n))))
	(if (> n 0)
	    (- r (- (arithmetic-shift 1 n) 1))
	    r))
      (if (>= n 0)			;shift left?
	  (if (and (<= n 8)
		   (exact? x)
		   (< x 4194304))
	      (bit-lsh x n)
	      (* x (expt 2 n)))
	  (if (and (<= n 28) (exact? x))
	      (bit-rsh x (- n))
	      (floor (* x (expt 2. n)))))))

(define (bitwise-and x y)
  (if (and (< x 0) (< y 0))
      (- -1 (bit-or (- -1 x) (- -1 y)))
      (bit-and x y)))

(define (bitwise-ior x y)
  (if (or (< x 0) (< y 0))
      (- -1 (bit-and (- -1 x) (- -1 y)))
      (bit-or x y)))

(define (bitwise-not x) (- -1 x))

; ASCII

(define char->ascii char->integer)
(define ascii->char integer->char)
(define ascii-limit 256)
(define ascii-whitespaces '(32 10 9 12 13))


; CODE-VECTORS (= alt/code-vectors.scm)

(define *code-vector-marker* (list '*code-vector-marker*))

(define (make-code-vector len init)
  (let ((t (make-vector (+ len 1) init)))
    (vector-set! t 0 *code-vector-marker*)
    t))

(define (code-vector? obj)
  (and (vector? obj)
       (> (vector-length obj) 0)
       (eq? (vector-ref obj 0) *code-vector-marker*)))

(define (code-vector-length t) (- (vector-length t) 1))
(define (code-vector-ref t i) (vector-ref t (+ i 1)))
(define (code-vector-set! t i x) (vector-set! t (+ i 1) x))

(define (write-byte byte port)
  (write-char (ascii->char byte) port))
  

