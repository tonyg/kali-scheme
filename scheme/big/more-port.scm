; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Additional port types

;----------------
; Ports which keep track of the current row and column.
;
; When the row or column data is requested we need to process the characters
; between the port's current index and the index at the time of the previous
; check.
;
; When a buffer operation is requested we need to process any remaining
; characters in the old buffer.
;
; sub-port:    port being tracked
; index:       the index of the next character to be processed
; row, column: position of the character at BUFFER[INDEX - 1] 

(define-record-type port-location :port-location
  (really-make-port-location sub-port index row column)
  port-location?
  (sub-port port-location-sub-port)
  (index    port-location-index  set-port-location-index!)
  (row      port-location-row    set-port-location-row!)
  (column   port-location-column set-port-location-column!))

(define (make-port-location sub-port)
  (really-make-port-location sub-port 0 0 0))

; Update the data and return what you get.

(define (row-column-accessor accessor)
  (lambda (port)
    (let ((data (port-data port)))
      (if (port-location? data)
	  (begin
	    (update-row-and-column! port data)
	    (accessor data))
	  #f))))

(define current-row    (row-column-accessor port-location-row))
(define current-column (row-column-accessor port-location-column))

; Bring LOCATION up to date.

(define (update-row-and-column! port location)
  (let ((at (port-index port))
	(checked-to (port-location-index location))
	(buffer (port-buffer port)))
    (if (< checked-to at)
	(begin
	  (get-row-and-column! buffer checked-to at location)
	  (set-port-location-index! location at)))))

(define (get-row-and-column! buffer start end location)
  (let loop ((i start)
	     (row (port-location-row location))
	     (column (port-location-column location)))
    (cond ((= i end)
	   (set-port-location-row! location row)
	   (set-port-location-column! location column))
	  ((= (char->ascii #\newline)
	      (byte-vector-ref buffer i))
	   (loop (+ i 1) (+ row 1) 0))
	  (else
	   (loop (+ i 1) row (+ column 1))))))

;----------------
; Input ports that keep track of the current row and column.

(define (make-tracking-input-port port)
  (if (input-port? port)
      (make-buffered-input-port tracking-input-port-handler
				(make-port-location port)
				(make-byte-vector (default-buffer-size) 0)
				0
				0)
      (call-error "not an input port" make-tracking-input-port port)))

(define tracking-input-port-handler
  (make-buffered-input-port-handler
   (lambda (location)
     (list 'tracking-port (port-location-sub-port location)))
   (lambda (port)			; close
     (maybe-commit))
   (lambda (port wait?)
     (if (maybe-commit)
	 (let ((location (port-data port)))
	   (update-row-and-column! port location)
	   (let ((got (read-block (port-buffer port)
				  0
				  (byte-vector-length (port-buffer port))
				  (port-location-sub-port location)
				  wait?)))
	     ;(note-buffer-reuse! port)
	     (if (eof-object? got)
		 (set-port-pending-eof?! port #t)
		 (begin
		   (set-port-location-index! location 0)
		   (set-port-index! port 0)
		   (set-port-limit! port got)))
	     #t))
	 #f))
   (lambda (port)
     (let ((ready? (char-ready? (port-location-sub-port (port-data port)))))
       (if (maybe-commit)
	   (values #t ready?)
	   (values #f #f))))))

;----------------
; Output ports that keep track of the current row and column.

(define tracking-output-port-handler
  (make-buffered-output-port-handler
   (lambda (location)
     (list 'tracking-port (port-location-sub-port location)))
   (lambda (port)			; close
     (maybe-commit))
   (lambda (port necessary?)		; we ignore necessary? and always write
     (if (maybe-commit)			; out whatever we have
	 (let ((location (port-data port)))
	   (update-row-and-column! port location)
	   (write-block (port-buffer port)
			0
			(port-index port)
			(port-location-sub-port location))
	   ;(note-buffer-reuse! port)
	   (set-port-location-index! location 0)
	   (set-port-index! port 0)
	   #t)
	 #f))
   (lambda (port)
     (let ((ready? (output-port-ready?
		    (port-location-sub-port (port-data port)))))
       (if (maybe-commit)
	   (values #t ready?)
	   (values #f #f))))))

(define (make-tracking-output-port port)
  (if (output-port? port)
      (make-buffered-output-port tracking-output-port-handler
				 (make-port-location port)
				 (make-byte-vector (default-buffer-size) 0)
				 0
				 (default-buffer-size))
      (call-error "not an output port" make-tracking-output-port port)))

(define (fresh-line port)
  (let ((column (current-column port)))
    (if (and column (< 0 column))
	(newline port))))

;----------------
; String input ports

; All the work is done by the buffered-port code.

(define string-input-port-handler
  (make-buffered-input-port-handler
   (lambda (ignore)
     (list 'string-input-port))
   (lambda (ignore)
     (maybe-commit))
   (lambda (port wait?)
     (set-port-pending-eof?! port #t)
     (maybe-commit))
   (lambda (port)
     (if (maybe-commit)
	 (values #t #f)
	 (values #f #f)))))

; We copy the input string because (a) string-set! exists and (b) buffers have
; to be byte vectors.

(define (make-string-input-port string)
  (let ((buffer (make-byte-vector (string-length string) 0)))
    (copy-bytes! string 0 buffer 0 (string-length string))
    (make-buffered-input-port string-input-port-handler
			      #f             ; no additional state needed
			      buffer
			      0
			      (string-length string))))

;----------------
; String output ports

; The cdr of the data field of the port is a list of buffers (the car is not
; used).  When the output is wanted the buffers are concatenated together to
; get the final string.
;
; These are thread-safe for no particular reason.

(define (make-string-output-port)
  (make-buffered-output-port string-output-port-handler
			     (list #f)
			     (make-byte-vector (default-buffer-size) 0)
			     0
			     (default-buffer-size)))

; Concatenates all of the buffers into single string.
; Could use a proposal...

(define (string-output-port-output port)
  (ensure-atomicity
    (check-buffer-timestamp! port)	; makes the proposal check this
    (let* ((full (provisional-cdr (port-data port)))
	   (last (port-buffer port))
	   (index (provisional-port-index port))
	   (out (make-string (apply +
				    index
				    (map byte-vector-length full)))))
      (let loop ((full (reverse full)) (i 0))
	(if (null? full)
	    (copy-bytes! last 0 out i index)
	    (let* ((buffer (car full))
		   (count (byte-vector-length buffer)))
	      (copy-bytes! buffer 0 out i count)
	      (loop (cdr full) (+ i count)))))
      out)))

(define string-output-port-handler
  (make-buffered-output-port-handler
   (lambda (port)
     '(string-output-port))
   (lambda (port)			; closer
     (maybe-commit))
   (lambda (port necessary?)		; we ignore necessary? and always write
     (provisional-set-cdr!		; out whatever we have
       (port-data port)
       (cons (let* ((size (provisional-port-index port))
		    (new (make-byte-vector size 0)))
	       (copy-bytes! (port-buffer port)
			    0
			    new
			    0
			    size)
	       new)
	     (provisional-cdr (port-data port))))
     (provisional-set-port-index! port 0)
     ;(note-buffer-reuse! port)
     (maybe-commit))
   (lambda (port)
     (if (maybe-commit)
	 (values #t #f)
	 (values #f #f)))))

(define (call-with-string-output-port proc)
  (let ((port (make-string-output-port)))
    (proc port)
    (string-output-port-output port)))

;----------------
; Output ports from single character consumers

(define (char-sink->output-port proc)
  (make-unbuffered-output-port char-sink-output-port-handler
			       proc))

(define char-sink-output-port-handler
  (make-port-handler
   (lambda (proc)
     (list 'char-sink-output-port))
   make-output-port-closed!
   (lambda (port char)
     ((port-data port) char))
   (lambda (port buffer start count)
     (let ((proc (port-data port)))
       (if (string? buffer)
	   (do ((i 0 (+ i 1)))
	       ((= i count))
	     (proc (string-ref buffer (+ start i))))
	   (do ((i 0 (+ i 1)))
	       ((= i count))
	     (proc (ascii->char (byte-vector-ref buffer (+ start i))))))))
   (lambda (port)		; ready?
     #t)
   (lambda (port error-if-closed?)		; force output
     (unspecific))))

; Call PROC on a port that will transfer COUNT characters to PORT and
; then quit.

(define (limit-output port count proc)
  (call-with-current-continuation
    (lambda (quit)
      (proc (char-sink->output-port
	     (lambda (char)
	       (write-char char port)
	       (set! count (- count 1))
	       (if (<= count 0)
		   (quit #f))))))))

; Old name.
(define write-one-line limit-output)

;----------------
; Input ports from single character producers
;
; (char-source->input-port <next-char-thunk>
;                          [<char-ready?-thunk>
;                          [<close-thunk>]])

(define (char-source->input-port source . more)
  (make-unbuffered-input-port char-source-input-port-handler
			      (make-source-data source
						(if (null? more)
						    (lambda () #t)
						    (car more))
						(if (or (null? more)
							(null? (cdr more)))
						    values
						    (cadr more))
						#f)))

; These are a bit of a mess.  We have to keep a one-character buffer to make
; peek-char work.

(define-record-type source-data :source-data
  (make-source-data source ready? close buffer)
  source-data?
  (source source-data-source)
  (close source-data-close)
  (ready? source-data-ready?)
  (buffer source-data-buffer set-source-data-buffer!))

(define char-source-input-port-handler
  (make-port-handler
   (lambda (proc)
     (list 'char-source-input-port))
   (lambda (port)
     (make-input-port-closed! port)
     ((source-data-close (port-data port))))
   (lambda (port read?)
     (char-source-read-char port (port-data port) read?))
   (lambda (port buffer start count wait?)
     (if (or (= count 0)
	     (port-pending-eof? port))
	 0
	 (char-source-read-block port (port-data port) buffer start count)))
   (lambda (port)
     (if (or (port-pending-eof? port)
	     (source-data-buffer (port-data port)))
	 #t
	 ((source-data-ready? (port-data port)))))
   #f))				; force

; EOF and peeked characters are held in separate places so we have to check
; both.  

(define (char-source-read-char port data read?)   
  (cond ((port-pending-eof? port)
	 (if read?
	     (set-port-pending-eof?! port #t))
	 (eof-object))
	((source-data-buffer data)
	 => (lambda (char)
	      (if read?
		  (set-source-data-buffer! data #f))
	      char))
	(else
	 (let ((char ((source-data-source data))))
	   (if (not read?)
	       (if (eof-object? char)
		   (set-port-pending-eof?! port #t)
		   (set-source-data-buffer! data char)))
	   char))))

; Put any buffered in first and then get the rest from the source.

(define (char-source-read-block port data buffer start count)
  (let loop ((i (if (source-data-buffer data)
		    (let ((char (source-data-buffer data)))
		      (buffer-set! buffer start char)
		      (set-source-data-buffer! data #f)
		      1)
		    0)))
    (if (= i count)
	count
	(let ((next ((source-data-source data))))
	  (cond ((eof-object? next)
		 (if (= 0 i)
		     (eof-object)
		     (begin
		       (set-port-pending-eof?! port #t)
		       i)))
		(else
		 (buffer-set! buffer (+ start i) next)
		 (loop (+ i 1))))))))

(define (buffer-set! buffer index char)
  (if (string? buffer)
      (string-set! buffer index char)
      (byte-vector-set! buffer index (char->ascii char))))

						
