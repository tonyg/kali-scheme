; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; An implementation of Pre-Scheme's memory interface that can detect some
; stray reads and writes.  It has numerous limitiations:
;    Allocations are always on page boundaries.
;    No more than 16 megabytes can be allocated at once.
;    More than 32 or 64 or so allocations result in addresses being
;       bignums (dealloctions have no effect on this).
;
; Memory is represented as a vector of byte-vectors, with each byte-vector
; representing a 16-megabyte page.  Allocations are always made on page
; boundaries, so the byte-vectors only need be as large as the allocated
; areas.  Pages are never re-used.

; Memory is one big vector, with markers at beginning and end of free blocks
; and allocationg by by address ordered first fit.

; Strings and things end up as bignums...

(define *memory* (make-vector 0 0))

(define-record-type address :address
  (make-address index)
  address?
  (index address-index))
  
(define-record-discloser :address
  (lambda (addr) (list 'address (address-index addr))))

; We add 100000000 to addresses to make them

(define address-offset 100000000)

(define (address->integer addr)
  (+ (address-index addr) address-offset))

(define (integer->address int)
  (make-address (- int address-offset)))

(define (word-ref addr)
  (if (address? addr)
      (let ((index (address-index addr)))
	(if (and (= 0
  (vector-ref *memory* (address-index addr)))

(define (word-ref addr)
  (vector-ref *memory* (address-index addr)))









(define *memory* (make-vector 16 #f))    ; vector of pages
(define log-max-size 24)                 ; log of page size
(define address-shift (- log-max-size))  ; turns addresses into page indices

(define max-size (arithmetic-shift 1 log-max-size))  ; page size

(define address-mask                     ; mask to get address within page
  (- (arithmetic-shift 1 log-max-size) 1))

(define *next-index* 0)                  ; next available page

(define (reinitialize-memory)
  (set! *memory* (make-vector 16 #f))
  (set! *next-index* 0))

; Extend the page vector if necessary, and then make a page of the
; appropriate size.

(define (allocate-memory size)
  (cond ((> size max-size)
	 #f)  ; the null pointer
	(else
	 (if (>= *next-index* (vector-length *memory*))
	     (let ((new (make-vector (* 2 (vector-length *memory*)))))
	       (do ((i 0 (+ i 1)))
		   ((>= i (vector-length *memory*)))
		 (vector-set! new i (vector-ref *memory* i)))
	       (set! *memory* new)))
	 (let ((index *next-index*))
	   (set! *next-index* (+ *next-index* 1))
	   (vector-set! *memory* index (make-code-vector size 0))
	   (arithmetic-shift index log-max-size)))))

; Turning an address into a page or page index

(define (address->vector address)
  (vector-ref *memory* (arithmetic-shift address address-shift)))

(define (address->vector-index address)
  (bitwise-and address address-mask))

; Throw away the page containing ADDRESS, which must be the first address in
; that page, 

(define (deallocate-memory address)
  (let ((vector (address->vector address))
	(byte-address (address->vector-index address)))
    (if (and vector (= byte-address 0))
	(vector-set! *memory* (arithmetic-shift address address-shift) #f)
	(error "bad deallocation address" address))))

; Various ways of accessing memory

(define (unsigned-byte-ref address)
  (code-vector-ref (address->vector address)
		   (address->vector-index address)))

(define (signed-code-vector-ref bvec i)
  (let ((x (code-vector-ref bvec i)))
    (if (< x 128)
	x
	(bitwise-ior x -128))))

(define (word-ref address)
  (let ((vector (address->vector address))
	(byte-address (address->vector-index address)))
    (if (not (= 0 (bitwise-and byte-address 3)))
	(error "unaligned address error" address)
	(+ (+ (arithmetic-shift (signed-code-vector-ref vector byte-address) 24)
	      (arithmetic-shift (code-vector-ref vector (+ byte-address 1)) 16))
	   (+ (arithmetic-shift (code-vector-ref vector (+ byte-address 2))  8)
	      (code-vector-ref vector (+ byte-address 3)))))))
	    
(define (unsigned-byte-set! address value)
  (code-vector-set! (address->vector address)
		    (address->vector-index address)
		    (bitwise-and 255 value)))
	    
(define (word-set! address value)
  (let ((vector (address->vector address))
	(byte-address (address->vector-index address)))
    (if (not (= 0 (bitwise-and byte-address 3)))
	(error "unaligned address error" address))
    (code-vector-set! vector    byte-address
		      (bitwise-and 255 (arithmetic-shift value -24)))
    (code-vector-set! vector (+ byte-address 1)
		      (bitwise-and 255 (arithmetic-shift value -16)))
    (code-vector-set! vector (+ byte-address 2)
		      (bitwise-and 255 (arithmetic-shift value -8)))
    (code-vector-set! vector (+ byte-address 3)
		      (bitwise-and 255 value))))

; Block I/O procedures.

(define (write-block port address count)
  (let ((vector (address->vector address))
	(byte-address (address->vector-index address)))
    (do ((i 0 (+ i 1)))
	((>= i count))
      (write-char (ascii->char (code-vector-ref vector (+ i byte-address)))
		  port))
    (values count (enum errors no-errors))))

(define (read-block port address count)
  (cond ((not (char-ready? port))
	 (values 0 #f (enum errors no-errors)))
	((eof-object? (scheme:peek-char port))
	 (values 0 #t (enum errors no-errors)))
	(else
	 (let ((vector (address->vector address))
	       (byte-address (address->vector-index address)))
	   (let loop ((i 0))
	     (if (or (= i count)
		     (not (char-ready? port)))
		 (values i #f (enum errors no-errors))
		 (let ((c (scheme:read-char port)))
		   (cond ((eof-object? c)
			  (values i #f (enum errors no-errors)))
			 (else
			  (code-vector-set! vector
					    (+ i byte-address)
					    (char->ascii c))
			  (loop (+ i 1)))))))))))

(define (copy-memory! from to count)
  (let ((from-vector (address->vector from))
	(from-address (address->vector-index from))
	(to-vector (address->vector to))
	(to-address (address->vector-index to)))
    (do ((i 0 (+ i 1)))
	((>= i count))
      (code-vector-set! to-vector
			(+ i to-address)
			(code-vector-ref from-vector
					 (+ i from-address))))))

(define (memory-equal? from to count)
  (let ((from-vector (address->vector from))
	(from-address (address->vector-index from))
	(to-vector (address->vector to))
	(to-address (address->vector-index to)))
    (let loop ((i 0))
      (cond ((>= i count)
	     #t)
	    ((= (code-vector-ref to-vector (+ i to-address))
		(code-vector-ref from-vector (+ i from-address)))
	     (loop (+ i 1)))
	    (else
	     #f)))))

; Turn the LENGTH bytes starting from ADDRESS into a string.

(define (char-pointer->string address length)
  (let ((vector (address->vector address))
	(byte-address (address->vector-index address))
	(string (make-string length)))
    (do ((i 0 (+ i 1)))
	((= i length))
      (string-set! string
		   i
		   (ascii->char (code-vector-ref vector (+ byte-address i)))))
    string))

; Turn the bytes from ADDRESS to the next nul (byte equal to 0) into a
; string.  This is a trivial operation in C.

(define (char-pointer->nul-terminated-string address)
  (let ((vector (address->vector address))
	(byte-address (address->vector-index address)))
    (char-pointer->string address (index-of-first-nul vector byte-address))))

(define (index-of-first-nul vector address)
  (let loop ((i address))
    (cond ((= i (code-vector-length vector))
	   (error "CHAR-POINTER->STRING called on pointer with no nul termination"))
	  ((= 0 (code-vector-ref vector i))
	   (- i address))
	  (else
	   (loop (+ i 1))))))
	  
; We really need these to work.

(define address+ +)
(define address- -)
(define address-difference -)
(define address->integer identity)
(define integer->address identity)
(define ...)
