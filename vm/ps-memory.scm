
; Scheme implementation of Pre-Scheme memory primitives

; For now, allocations are always on page boundaries.

; No more than 16 megabytes can be allocated at once.

; More than 32 or 64 or so allocations result in addresses being bignums
; (dealloctions have no effect on this).

(define *memory* (make-vector 16 #f))

(define log-max-size 24)

(define max-size (arithmetic-shift 1 log-max-size))

(define address-mask
  (- (arithmetic-shift 1 log-max-size) 1))

(define *next-index* 0)

(define null-pointer? not)

(define (reinitialize-memory)
  (set! *memory* (make-vector 16 #f))
  (set! *next-index* 0))

(define (allocate-memory size)
  (if (> size max-size)
      (error "cannot allocate this much memory at once" size))
  (if (>= *next-index* (vector-length *memory*))
      (let ((new (make-vector (* 2 (vector-length *memory*)))))
	(do ((i 0 (+ i 1)))
	    ((>= i (vector-length *memory*)))
	  (vector-set! new i (vector-ref *memory* i)))
	(set! *memory* new)))
  (let ((index *next-index*))
    (set! *next-index* (+ *next-index* 1))
    (vector-set! *memory* index (make-code-vector size 0))
    (arithmetic-shift index log-max-size)))

(define (decode-address address)
  (let ((high (arithmetic-shift address (- 0 log-max-size)))
	(low  (bitwise-and address address-mask)))
    (values (vector-ref *memory* high) low)))

(define (deallocate-memory address)
  (call-with-values
   (lambda () (decode-address address))
   (lambda (vector byte-address)
     (if (and vector (= byte-address 0))
	 (vector-set! *memory* (arithmetic-shift address (- 0 log-max-size)) #f)
	 (error "bad deallocation address" address)))))

(define (unsigned-byte-ref address)
  (call-with-values
   (lambda () (decode-address address))
   (lambda (vector byte-address)
     (code-vector-ref vector byte-address))))
	    
(define (signed-code-vector-ref bvec i)
  (let ((x (code-vector-ref bvec i)))
    (if (< x 128)
	x
	(bitwise-ior x -128))))

(define (word-ref address)
  (call-with-values
   (lambda () (decode-address address))
   (lambda (vector byte-address)
     (if (not (= 0 (bitwise-and byte-address 3)))
	 (error "unaligned address error" address)
	 (+ (+ (arithmetic-shift (signed-code-vector-ref vector byte-address) 24)
	       (arithmetic-shift (code-vector-ref vector (+ byte-address 1)) 16))
	    (+ (arithmetic-shift (code-vector-ref vector (+ byte-address 2))  8)
	                         (code-vector-ref vector (+ byte-address 3))))))))
	    
(define (unsigned-byte-set! address value)
  (call-with-values
   (lambda () (decode-address address))
   (lambda (vector byte-address)
     (code-vector-set! vector byte-address (bitwise-and 255 value)))))
	    
(define (word-set! address value)
  (call-with-values
   (lambda () (decode-address address))
   (lambda (vector byte-address)
     (if (not (= 0 (bitwise-and byte-address 3)))
	 (error "unaligned address error" address))
     (code-vector-set! vector    byte-address
		       (bitwise-and 255 (arithmetic-shift value -24)))
     (code-vector-set! vector (+ byte-address 1)
		       (bitwise-and 255 (arithmetic-shift value -16)))
     (code-vector-set! vector (+ byte-address 2)
		       (bitwise-and 255 (arithmetic-shift value -8)))
     (code-vector-set! vector (+ byte-address 3)
		       (bitwise-and 255 value)))))

(define (write-block port address count)
  (call-with-values
   (lambda () (decode-address address))
   (lambda (vector byte-address)
     (do ((i 0 (+ i 1)))
	 ((>= i count))
       (write-char (ascii->char (code-vector-ref vector (+ i byte-address)))
		   port)))))

(define (read-block port address count)
  (call-with-values
   (lambda () (decode-address address))
   (lambda (vector byte-address)
     (do ((i 0 (+ i 1)))
	 ((>= i count))
       (code-vector-set! vector
			 (+ i byte-address)
			 (char->ascii (read-char port)))))))
