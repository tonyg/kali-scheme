; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This little utility converts a heap image into a C file.
;
; For example:
;   (do-it 100000 "~/s48/debug/little.image" "little-heap.c")
;
; The first argument to do-it should be somewhat larger than the size,
; in bytes, of the image file to be converted (which you can obtain with
; "ls -l").
;
; If the image contains 0-length stored objects, then the .c file will
; have to be compiled by gcc, since 0-length arrays aren't allowed in
; ANSI C.  This wouldn't be difficult to work around.
;
; For loading instructions, see load-static.scm.

(define *comments?* #f)

; 800,000 bytes => 200,000 words => at least 100,000 objects
;   50 chunks => 16,000 bytes per chunk => 2,000 objects per chunk
(define *chunk-size* 10000)

(define (do-it bytes infile outfile)
  (let ((start (init bytes infile)))
    (call-with-output-file outfile
      (lambda (port)
	(format port "#define D(x) (long)(&x)+7~%")
	(format port "#define H unsigned long~%")
	(emit-area-declarations "p" immutable? "const " port)
	(emit-area-declarations "i" mutable? "" port)
	(emit-area-initializers "p" immutable? "const " port)
	(emit-area-initializers "i" mutable? "" port)
	(display "const long entry = " port)
	(emit-descriptor start port)
	(write-char #\; port)
	(newline port)))))

(define (init bytes infile)
  (create-memory (quotient bytes 2) quiescent) ;Output of ls -l
  (initialize-heap (memory-begin) (memory-size))
  (let ((start (read-image infile 0)))
    (message (nchunks)
	     " chunks")
    start))

(define (nchunks) (+ (chunk-number (heap-pointer)) 1))

; emit struct declarations for areas

(define (emit-area-declarations name in-area? const port)
  (for-each-stored-object
   (lambda (chunk)
     (message name chunk " declaration")
     (display "struct " port) (display name port) (display chunk port)
     (display " {" port) (newline port))
   (lambda (x)
     (if (in-area? x)
	 (emit-declaration x port)))
   (lambda (chunk)
     (display "};" port)
     (newline port)
     (display const port)
     (display "extern struct " port) (display name port) (display chunk port)
     (write-char #\space port) (display name port) (display chunk port)
     (write-char #\; port) (newline port)
     chunk)))

(define (emit-declaration x port)
  (display "  H x" port)
  (writex x port)
  (cond ((d-vector? x)
	 (display "; long d" port)
	 (writex x port)
	 (write-char #\[ port)
	 (write (d-vector-length x) port))
	((vm-string? x)
	 (display "; char d" port)
	 (writex x port)
	 (write-char #\[ port)
	 ;; Ensure alignment (thanks Ian)
	 (write (cells->bytes (bytes->cells (b-vector-length x)))
		port))
	(else
	 (display "; unsigned char d" port)
	 (writex x port)
	 (write-char #\[ port)
	 ;; Ensure alignment
	 (write (cells->bytes (bytes->cells (b-vector-length x)))
		port)))
  (display "];" port)
  (if *comments?*
      (begin (display " /* " port)
	     (display (enumerand->name (stob-type x) stob) port)
	     (display " */" port)))
  (newline port))

; Emit initializers for areas

(define (emit-area-initializers name in-area? const port)
  (for-each-stored-object
   (lambda (chunk)
     (message name chunk " initializer")

     (display const port)
     (display "struct " port) (display name port) (write chunk port)
     (write-char #\space port) (display name port) (write chunk port)
     (display " =" port) (newline port)

     (write-char #\{ port) (newline port))
   (lambda (x)
     (if (in-area? x)
	 (emit-initializer x port)))
   (lambda (chunk)
     (display "};" port) (newline port)))

  (let ((n (nchunks)))
    (format port "const long ~a_count = ~s;~%" name n)
    (format port "~a long * const ~a_areas[~s] = {" const name n)
    (do ((i 0 (+ i 1)))
	((= i n))
      (format port "(~a long *)&~a~s, " const name i))
    (format port "};~%const long ~a_sizes[~s] = {" name n)
    (do ((i 0 (+ i 1)))
	((= i n))
      (format port "sizeof(~a~s), " name i))
    (format port "};~%")))


(define (message . stuff)
  (for-each display stuff) (newline))

(define (emit-initializer x port)
  (display "  " port)
  (write (stob-header x) port)
  (write-char #\, port)
  (cond ((d-vector? x)
	 (emit-d-vector-initializer x port))
	((vm-string? x)
	 (write-char #\" port)
	 (let ((len (vm-string-length x)))
	   (do ((i 0 (+ i 1)))
	       ((= i len) (write-char #\" port))
	     (let ((c (vm-string-ref x i)))
	       (cond ((or (char=? c #\") (char=? c #\\))
		      (write-char #\\ port))
		     ((char=? c #\newline)
		      (display "\\n\\" port)))
	       (write-char c port)))))
	(else
	 (write-char #\{ port)
	 (let ((len (b-vector-length x)))
	   (do ((i 0 (+ i 1)))
	       ((= i len) (write-char #\} port))
	     (write (b-vector-ref x i) port)
	     (write-char #\, port)))))
  (write-char #\, port)
  (if *comments?*
      (begin (display " /* " port)
	     (writex x port)
	     (display " */" port)))
  (newline port))

(define (emit-d-vector-initializer x port)
  (write-char #\{ port)
  (let ((len (d-vector-length x)))
    (do ((i 0 (+ i 1)))
	((= i len) (write-char #\} port))
      (emit-descriptor (d-vector-ref x i) port)
      (write-char #\, port))))

(define (emit-descriptor x port)
  (if (stob? x)
      (begin (if (immutable? x)
		 (display "D(p" port)
		 (display "D(i" port))
	     (display (chunk-number x) port)
	     (display ".x" port)
	     (writex x port)
	     (write-char #\) port))
      (write x port)))


; Foo

(define (writex x port)
  (write (quotient (- (- x (memory-begin)) 7) 4) port))

(define (chunk-number x)
  (quotient (- (- x (memory-begin)) 7) *chunk-size*))


; Image traversal utility

(define (for-each-stored-object chunk-start proc chunk-end)
  (let ((limit (heap-pointer)))
    (let chunk-loop ((addr (newspace-begin))
		     (i 0)
		     (chunk (+ (newspace-begin) *chunk-size*)))
      (if (addr< addr limit)
	  (begin (chunk-start i)
		 (let loop ((addr addr))
		   (if (and (addr< addr limit)
			    (addr< addr chunk))
		       (let ((d (fetch addr)))
			 (if (not (header? d))
			     (warn "heap is in an inconsistent state" d))
			 (proc (address->stob-descriptor (addr1+ addr)))
			 (loop (addr1+ (addr+ addr (header-a-units d)))))
		       (begin (chunk-end i)
			      (chunk-loop addr
					  (+ i 1)
					  (+ chunk *chunk-size*))))))))))

(define (mutable? x) (not (immutable? x)))
