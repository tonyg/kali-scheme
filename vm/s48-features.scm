; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


(define-syntax define-vm-syntax
  (lambda (form rename compare)
    (let ((pattern (cadr form))
	  (body (cddr form)))
      `(define-syntax ,(car pattern)
	 (lambda (form rename compare)
	   (apply (lambda ,(cdr pattern) . ,body)
		  (cdr form)))))))

; Byte vectors

(define (make-byte-vector size)
  ((access-scheme48 'make-code-vector) size 0))
(define byte-vector-length (access-scheme48 'code-vector-length))
(define byte-vector-ref    (access-scheme48 'code-vector-ref))
(define byte-vector-set!   (access-scheme48 'code-vector-set!))

(define (signed-byte-vector-ref bvec i)
  (let ((x (byte-vector-ref bvec i)))
    (if (< x 128)
	x
	(bitwise-ior x -128))))

(define (byte-vector-word-ref bvec i)
  (let* ((high (bitwise-ior (byte-vector-ref bvec (+ i 2))
			    (ashl (signed-byte-vector-ref bvec (+ i 3)) 8)))
	 (low  (bitwise-ior (byte-vector-ref bvec (+ i 0))
			    (ashl (byte-vector-ref bvec (+ i 1)) 8))))
    (bitwise-ior low (ashl high 16))))

(define (byte-vector-word-set! bvec i value)
  (do ((j i (+ j 1))
       (value value (ashr value 8)))
      ((> j (+ i 3)))
    (byte-vector-set! bvec j (bitwise-and value 255))))

(define (byte-vector-fill! bytev value)
  (let ((size (byte-vector-length bytev)))
    (do ((i 0 (+ i 1)))
        ((>= i size)
         bytev)
      (byte-vector-set! bytev i value))))

(define (byte-vector-word-fill! bytev value)
  (let ((size (byte-vector-length bytev)))
    (do ((i 0 (+ i 4)))
        ((>= i size)
         bytev)
      (byte-vector-word-set! bytev i value))))

(define ashl (access-scheme48 'arithmetic-shift))
(define ashr (lambda (n i) (ashl n (- 0 i))))

(define bitwise-not (access-scheme48 'bitwise-not))
(define bitwise-and (access-scheme48 'bitwise-and))
(define bitwise-ior (access-scheme48 'bitwise-ior))
(define bitwise-xor (access-scheme48 'bitwise-xor))

(define force-output (access-scheme48 'force-output))

; 32 bits per machine word minus Scheme48's two tag bits
(define useful-bits-per-word 30)

(define char->ascii (access-scheme48 'char->ascii))
(define ascii->char (access-scheme48 'ascii->char))

(define write-string (access-scheme48 'write-string))

(define error (access-scheme48 'error))

(define (read-block port bytev start-addr count)
  (do ((i 0 (+ i 1)))
      ((>= i count))
    (let ((char (read-char port)))
      (if (eof-object? char)
	  (error "READ-BLOCK read off end of file" port))
      (byte-vector-set! bytev (+ i start-addr) (char->ascii char)))))

(define (write-block port bytev start-addr count)
  (do ((i 0 (+ i 1)))
      ((>= i count))
    (write-char (ascii->char (byte-vector-ref bytev (+ i start-addr))) port)))
