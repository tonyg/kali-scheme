; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; ,open architecture primitives assembler packages

(define (init-space eval assembler)
  (let ((p (make-simple-package (list assembler) #f #f 'foo)))
    (set! length-procedures
	  (do ((i (- stob-count 1) (- i 1))
	       (l '() (cons (eval `(lap foo
					(check-nargs= 1)
					(pop)
					(stored-object-byte-length ,i)
					(return))
				  p)
			    l)))
	      ((< i 0) l)))))

(define length-procedures #f)

(define (space)
  (let loop ((i 0) (total-xs 0) (total-bytes 0))
    (if (< i stob-count)
	(begin
	  (collect)
	  (write-padded (vector-ref stob i) 16)
	  (let* ((xs (find-all-xs i))
		 (count (vector-length xs))
		 (bytes (let ((length
			       (list-ref length-procedures i)))
			  (do ((j (- (vector-length xs) 1) (- j 1))
			       (k 0 (+ k (+ 4 (length (vector-ref xs j))))))
			      ((< j 0) k)))))
	    (write-padded count 7)
	    (write-padded bytes 8)
	    (newline)
	    (loop (+ i 1)
		  (+ total-xs count)
		  (+ total-bytes bytes))))
	(begin
	  (write-padded 'total 16)
	  (write-padded total-xs 7)
	  (write-padded total-bytes 8)
	  (newline)))))

(define least-byte-type (enum stob string))


(define (write-padded x pad)
  (let ((s (if (symbol? x)
	       (symbol->string x)
	       (number->string x))))
    (do ((i (- pad (string-length s)) (- i 1)))
	((<= i 0) (display s))
      (write-char #\space))))
