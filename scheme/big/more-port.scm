; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Additional port types

;----------------
; Ports which keep track of the current row and column.
;
; When the row or column data is requested we need to process the characters
; between the port's current index and the index at the time of the previous
; check.
;
; When a buffer operation is requested we need to process any remaining
; characters in the old buffer.  If the operation is a block read or write
; we also need to process whatever is read or written.
;
; port:        the tracking port - needed for its buffer and indicies
; sub-port:    port being tracked
; index:       the index of the next character to be processed
; row, column: position of the character at BUFFER[INDEX - 1] 

(define-record-type port-location :port-location
  (really-make-port-location sub-port index row column)
  port-location?
  (port     port-location-port set-port-location-port!) ; setter for circularity
  (sub-port port-location-sub-port)
  (index    port-location-index  set-port-location-index!)
  (row      port-location-row    set-port-location-row!)
  (column   port-location-column set-port-location-column!))

(define (make-port-location sub-port)
  (really-make-port-location sub-port (port-index sub-port) 0 0))

; Update the data and return what you get.

(define (current-row port)
  (let ((data (port-data port)))
    (if (port-location? data)
	(begin
	  (obtain-port-lock port)
	  (update-row-and-column! data)
	  (let ((row (port-location-row data)))
	    (release-port-lock port)
	    row))
	#f)))

(define (current-column port)
  (let ((data (port-data port)))
    (if (port-location? data)
	(begin
	  (obtain-port-lock port)
	  (update-row-and-column! data)
	  (let ((column (port-location-column data)))
	    (release-port-lock port)
	    column))
	#f)))

; Bring LOCATION up to date.

(define (update-row-and-column! location)
  (let ((at (port-index (port-location-port location)))
	(checked-to (port-location-index location))
	(buffer (port-buffer (port-location-port location))))
    (if (< checked-to at)
	(begin
	  (update-row-and-column-from-bytes! buffer checked-to at location)
	  (set-port-location-index! location at)))))

; Two nearly identical procedures to deal with code-vectors and strings.

(define (update-row-and-column-from-bytes! code-vec start end location)
  (let loop ((i start)
	     (row (port-location-row location))
	     (column (port-location-column location)))
    (cond ((= i end)
	   (set-port-location-row! location row)
	   (set-port-location-column! location column))
	  ((= (char->ascii #\newline) (code-vector-ref code-vec i))
	   (loop (+ i 1) (+ row 1) 0))
	  (else
	   (loop (+ i 1) row (+ column 1))))))

(define (update-row-and-column-from-chars! string start count location)
  (let loop ((i start)
	     (row (port-location-row location))
	     (column (port-location-column location)))
    (cond ((= i (+ start count))
	   (set-port-location-row! location row)
	   (set-port-location-column! location column))
	  ((char=? #\newline (string-ref string i))
	   (loop (+ i 1) (+ row 1) 0))
	  (else
	   (loop (+ i 1) row (+ column 1))))))

;----------------
; Input ports that keep track of the current row and column.

(define tracking-input-port-handler
  (make-port-handler
   (lambda (location)
     (list 'tracking-port (port-location-sub-port location)))
   (lambda (location)  ; nothing to do
     (values))
   (lambda (location buffer start needed)
     (update-row-and-column! location)  ; finish off old buffer
     (let ((res (read-block buffer start needed
			    (port-location-sub-port location))))
       (cond ((eof-object? res))
	     ((eq? buffer (port-buffer (port-location-port location)))
	      (set-port-location-index! location 0))
	     ((code-vector? buffer)
	      (update-row-and-column-from-bytes! buffer start res location))
	     (else
	      (update-row-and-column-from-chars! buffer start res location)))
       res))))

(define (make-tracking-input-port port)
  (if (input-port? port)
      (let ((new-port (make-input-port tracking-input-port-handler
				       (make-port-location port)
				       (make-code-vector default-buffer-size 0)
				       0
				       0)))
	; make the circular link
	(set-port-location-port! (port-data new-port) new-port)
	new-port)
      (call-error "not an input port" make-tracking-input-port port)))

;----------------
; Output ports that keep track of the current row and column.

(define tracking-output-port-handler
  (make-port-handler
   (lambda (location)
     (list 'tracking-port (port-location-sub-port location)))
   ; flush the buffer when closing
   (lambda (location)
     (let ((port (port-location-port location)))
       (if (< 0 (port-index port))
	   (write-block (port-buffer port) 0 (port-index port)
			(port-location-sub-port location)))))
   ; Finish off the old buffer, send the characters to the child port, and
   ; then update the row and column if necessary.
   (lambda (location buffer start count)
     (update-row-and-column! location)
     (write-block buffer start count (port-location-sub-port location))
     (cond ((eq? buffer (port-buffer (port-location-port location)))
	    (set-port-location-index! location 0))
	   ((code-vector? buffer)
	    (update-row-and-column-from-bytes! buffer start count location))
	   (else
	    (update-row-and-column-from-chars! buffer start count location))))))

(define (make-tracking-output-port port)
  (if (output-port? port)
      (let ((new-port (make-output-port tracking-output-port-handler
					(make-port-location port)
					(make-code-vector default-buffer-size 0)
					0
					default-buffer-size)))
	; make the circular link
	(set-port-location-port! (port-data new-port) new-port)
	new-port)
      (call-error "not an output port" make-tracking-output-port port)))

(define (fresh-line port)
  (let ((column (current-column port)))
    (if (and column (< 0 column))
	(newline port))))

;----------------
; String input ports

; All the work is done by the port code.

(define string-input-port-handler
  (make-port-handler
   (lambda (ignore)
     (list 'string-input-port))
   (lambda (ignore)
     (values))
   (lambda (ignore buffer start needed)
     (eof-object))))

(define (make-string-input-port string)
  (let ((buffer (make-code-vector (string-length string) 0)))
    (copy-bytes! string 0 buffer 0 (string-length string))
    (make-input-port string-input-port-handler
		     #f                          ; no additional state needed
		     buffer
		     0
		     (string-length string))))   ; number of bytes available

(define copy-bytes! (structure-ref primitives copy-bytes!))

;----------------
; String output ports

; The data field of the port is a list of (<buffer> . <char-count>) pairs
; (the car is the port itself).  When the output is wanted the buffers are
; concatenated together to get the final string.

(define buffer-size 1024)

; Concatenates all of the buffers into single string.

(define (string-output-port-output port)
  (let* ((full (cdr (port-data port)))
	 (last (port-buffer port))
	 (index (port-index port))
	 (count (apply + index (map cdr full)))  ; Scheme is a trip
	 (out (make-string count)))
    (let loop ((full (reverse full)) (i 0))
      (if (null? full)
	  (copy-bytes! last 0 out i index)
	  (let ((buffer (caar full))
		(count (cdar full)))
	    (copy-bytes! buffer 0 out i count)
	    (loop (cdr full) (+ i count)))))
    out))

(define string-output-port-handler
  (make-port-handler
   (lambda (port)
     '(string-output-port))
   (lambda (port)
     (values))
   (lambda (data thing start count)
     (let ((port (car data)))
       (set-cdr! (port-data port)
		 (cons (cons (full-buffer port thing start count)
			     count)
		       (cdr (port-data port))))))))

(define (full-buffer port thing start count)
  (cond ((eq? thing (port-buffer port))
	 (set-port-buffer! port (make-code-vector default-buffer-size 0))
	 thing)
	(else
	 (let ((b (make-code-vector count 0)))
	   (copy-bytes! thing start b 0 count)
	   b))))

(define (make-string-output-port)
  (let ((port (make-output-port string-output-port-handler
				(list #f)
				(make-code-vector default-buffer-size 0)
				0
				default-buffer-size)))
    (set-car! (port-data port) port)
    port))

(define (call-with-string-output-port proc)
  (let ((port (make-string-output-port)))
    (proc port)
    (string-output-port-output port)))

;----------------
; Output ports from single character consumers

(define char-sink-output-port-handler
  (make-port-handler
   (lambda (proc)
     (list 'char-sink-output-port))
   (lambda (proc)
     (values))
   (lambda (proc char)
     (proc char))))

(define (char-sink->output-port proc)
  (make-unbuffered-output-port char-sink-output-port-handler
			       proc))

; Call PROC on a port that will transfer COUNT characters to PORT and
; then quit.

(define (write-one-line port count proc)
  (call-with-current-continuation
    (lambda (quit)
      (proc (char-sink->output-port
	     (lambda (char)
	       (write-char char port)
	       (set! count (- count 1))
	       (if (<= count 0)
		   (quit #f))))))))

;----------------
; Input ports from single character producers
; The producer is passed #T if a character is needed and #F if not.
; If #F is passed and no character is ready, then #F is returned.

(define char-source-input-port-handler
  (make-port-handler
   (lambda (proc)
     (list 'char-source-input-port))
   (lambda (proc)			; nothing to do
     (values))
   (lambda (proc buffer start needed)
     (if (integer? needed)
	 (let loop ((got 0))
	   (if (= got needed)
	       got
	       (let ((next (proc #t)))
		 (cond ((char? next)
			(buffer-set! buffer (+ start got) next)
			(loop (+ got 1)))
		       ((= got 0)
			(eof-object))
		       (else
			got)))))
	 (let ((next (proc (eq? needed 'any))))
	   (cond ((not next)
		  0)
		 ((eof-object? next)
		  (eof-object))
		 (else
		  (buffer-set! buffer start next)
		  1)))))))

(define (buffer-set! buffer index char)
  (if (string? buffer)
      (string-set! buffer index char)
      (code-vector-set! buffer index (char->ascii char))))

(define (char-source->input-port proc)
  (make-input-port char-source-input-port-handler
		   proc
		   (make-code-vector 1 0)
		   0
		   0))
