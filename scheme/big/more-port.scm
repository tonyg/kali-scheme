; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Additional port types

;----------------
; Ports which keep track of the current row and column.
;
; Note that we're only counting character access---single-byte access
; or block access are ignored.
;
; sub-port:    port being tracked
; row, column: position of the next character

(define-record-type port-location :port-location
  (really-make-port-location sub-port row column)
  port-location?
  (sub-port port-location-sub-port)
  (row      port-location-row    set-port-location-row!)
  (column   port-location-column set-port-location-column!))

(define (make-port-location sub-port)
  (really-make-port-location sub-port 0 0))

(define (row-column-accessor accessor)
  (lambda (port)
    (let ((data (port-data port)))
      (if (port-location? data)
	  (accessor data)
	  #f))))

(define current-row    (row-column-accessor port-location-row))
(define current-column (row-column-accessor port-location-column))

(define (port-location-update! location ch)
  (if (char=? ch #\newline)
      (begin
	(set-port-location-row! location
				(+ 1 (port-location-row location)))
	(set-port-location-column! location 0))
      (set-port-location-column! location
				 (+ 1 (port-location-column location)))))

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

(define (make-tracking-one-char-input plain-one-char-input)
  (lambda (port mode)
    (let ((thing (plain-one-char-input port mode)))
      (cond
       (mode thing)			; PEEK or READY?
       ((eof-object? thing) thing)
       (else
	(port-location-update! (port-data port) thing)
	thing)))))

(define tracking-input-port-handler
  (let ((plain-handler
	 (make-buffered-input-port-handler
	  (lambda (location)
	    (list 'tracking-port (port-location-sub-port location)))
	  (lambda (port)		; close
	    (maybe-commit))
	  (lambda (port wait?)
	    (if (maybe-commit)
		(let ((got (read-block (port-buffer port)
				       0
				       (byte-vector-length (port-buffer port))
				       (port-location-sub-port (port-data port))
				       wait?)))
		  ;;(note-buffer-reuse! port)
		  (if (eof-object? got)
		      (set-port-pending-eof?! port #t)
		      (begin
			(set-port-index! port 0)
			(set-port-limit! port got)))
		  #t)
		#f))
	  (lambda (port)
	    (let ((ready? (byte-ready? (port-location-sub-port (port-data port)))))
	      (if (maybe-commit)
		  (values #t ready?)
		  (values #f #f)))))))
    (make-port-handler
     (port-handler-discloser plain-handler)
     (port-handler-close plain-handler)
     (port-handler-byte plain-handler)
     (make-tracking-one-char-input (port-handler-char plain-handler))
     (port-handler-block plain-handler)
     (port-handler-ready? plain-handler)
     (port-handler-force plain-handler))))

;----------------
; Output ports that keep track of the current row and column.

(define (make-tracking-one-char-output plain-one-char-output)
  (lambda (port ch)
    (plain-one-char-output port ch)
    (port-location-update! (port-data port) ch)))

(define tracking-output-port-handler
  (let ((plain-handler
	 (make-buffered-output-port-handler
	  (lambda (location)
	    (list 'tracking-port (port-location-sub-port location)))
	  (lambda (port)			; close
	    (maybe-commit))
	  (lambda (port necessary?)		; we ignore necessary? and always write
	    (if (maybe-commit)			; out whatever we have
		(begin
		  (write-block (port-buffer port)
			       0
			       (port-index port)
			       (port-location-sub-port (port-data port)))
		  ;;(note-buffer-reuse! port)
		  (set-port-index! port 0)
		  #t)
		#f))
	  (lambda (port)
	    (let ((ready? (output-port-ready?
			   (port-location-sub-port (port-data port)))))
	      (if (maybe-commit)
		  (values #t ready?)
		  (values #f #f)))))))
    (make-port-handler
     (port-handler-discloser plain-handler)
     (port-handler-close plain-handler)
     (port-handler-byte plain-handler)
     (make-tracking-one-char-output (port-handler-char plain-handler))
     (port-handler-block plain-handler)
     (port-handler-ready? plain-handler)
     (port-handler-force plain-handler))))

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

; We copy the input because it's mutable.

(define (make-byte-vector-input-port bytes)
  (let* ((size (byte-vector-length bytes))
	 (buffer (make-byte-vector size 0)))
    (copy-bytes! bytes 0 buffer 0 size)
    (make-byte-vector-input-port-internal buffer)))

(define (make-byte-vector-input-port-internal buffer)
  (make-buffered-input-port string-input-port-handler
			    #f		; no additional state needed
			    buffer
			    0
			    (byte-vector-length buffer)))

(define (make-string-input-port string)
  (let* ((string-size (string-length string))
	 (encoding-size (string-encoding-length/utf-8 string 0 string-size))
	 (buffer (make-byte-vector encoding-size 0)))
    (encode-string/utf-8 string 0 string-size buffer 0)
    (make-byte-vector-input-port-internal buffer)))

;----------------
; String output ports

; The cdr of the data field of the port is a list of buffers (the car is not
; used).  When the output is wanted the buffers are concatenated together to
; get the final string.
;
; These are thread-safe for no particular reason.

(define (make-byte-vector-output-port)
  (make-buffered-output-port string-output-port-handler
			     (list #f)
			     (make-byte-vector (default-buffer-size) 0)
			     0
			     (default-buffer-size)))

(define make-string-output-port make-byte-vector-output-port)

; Concatenates all of the buffers into single string.
; Could use a proposal...

(define (byte-vector-output-port-output port)
  (ensure-atomicity
    (check-buffer-timestamp! port)	; makes the proposal check this
    (let* ((full (provisional-cdr (port-data port)))
	   (last (port-buffer port))
	   (index (provisional-port-index port))
	   (out (make-byte-vector (apply +
					 index
					 (map byte-vector-length full))
				  0)))
      (let loop ((full (reverse full)) (i 0))
	(if (null? full)
	    (copy-bytes! last 0 out i index)
	    (let* ((buffer (car full))
		   (count (byte-vector-length buffer)))
	      (copy-bytes! buffer 0 out i count)
	      (loop (cdr full) (+ i count)))))
      out)))

(define (string-output-port-output port)
  (utf-8->string (byte-vector-output-port-output port) #\?))

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
; Output ports from single byte consumers

(define (byte-sink->output-port proc)
  (make-unbuffered-output-port byte-sink-output-port-handler
			       proc))

(define byte-sink-output-port-handler
  (make-port-handler
   (lambda (proc)
     (list 'byte-sink-output-port))
   make-output-port-closed!
   (lambda (port byte)
     ((port-data port) byte))
   (lambda (port ch)
     'lose) ; ####
   (lambda (port buffer start count)
     (let ((proc (port-data port)))
       (do ((i 0 (+ i 1)))
	   ((= i count))
	 (proc (byte-vector-ref buffer (+ start i))))))
   (lambda (port)		; ready?
     #t)
   (lambda (port error-if-closed?)		; force output
     (unspecific))))

; Call PROC on a port that will transfer COUNT bytes to PORT and
; then quit.

(define (limit-output port count proc)
  (call-with-current-continuation
    (lambda (quit)
      (proc (byte-sink->output-port
	     (lambda (byte)
	       (write-byte byte port)
	       (set! count (- count 1))
	       (if (<= count 0)
		   (quit #f))))))))

; Old name.
(define write-one-line limit-output)

;----------------
; Input ports from single byte producers
;
; (byte-source->input-port <next-byte-thunk>
;                          [<byte-ready?-thunk>
;                          [<close-thunk>]])

(define (byte-source->input-port source . more)
  (make-unbuffered-input-port byte-source-input-port-handler
			      (make-source-data source
						(if (null? more)
						    (lambda () #t)
						    (car more))
						(if (or (null? more)
							(null? (cdr more)))
						    values
						    (cadr more))
						#f)))

; These are a bit of a mess.  We have to keep a one-byte buffer to make
; peek-byte work.

(define-record-type source-data :source-data
  (make-source-data source ready? close buffer)
  source-data?
  (source source-data-source)
  (close source-data-close)
  (ready? source-data-ready?)
  (buffer source-data-buffer set-source-data-buffer!))

(define byte-source-input-port-handler
  (make-port-handler
   (lambda (proc)
     (list 'byte-source-input-port))
   (lambda (port)
     (make-input-port-closed! port)
     ((source-data-close (port-data port))))
   (lambda (port read?)
     (byte-source-read-byte port (port-data port) read?))
   (lambda (port mode)
     'lose) ; ####
   (lambda (port buffer start count wait?)
     (if (or (= count 0)
	     (port-pending-eof? port))
	 0
	 (byte-source-read-block port (port-data port) buffer start count)))
   (lambda (port)
     (if (or (port-pending-eof? port)
	     (source-data-buffer (port-data port)))
	 #t
	 ((source-data-ready? (port-data port)))))
   #f))				; force

; EOF and peeked bytes are held in separate places so we have to check
; both.  

(define (byte-source-read-byte port data read?)   
  (cond ((port-pending-eof? port)
	 (if read?
	     (set-port-pending-eof?! port #t))
	 (eof-object))
	((source-data-buffer data)
	 => (lambda (byte)
	      (if read?
		  (set-source-data-buffer! data #f))
	      byte))
	(else
	 (let ((byte ((source-data-source data))))
	   (if (not read?)
	       (if (eof-object? byte)
		   (set-port-pending-eof?! port #t)
		   (set-source-data-buffer! data byte)))
	   byte))))

; Put any buffered in first and then get the rest from the source.

(define (byte-source-read-block port data buffer start count)
  (let loop ((i (if (source-data-buffer data)
		    (let ((byte (source-data-buffer data)))
		      (byte-vector-set! buffer start byte)
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
		 (byte-vector-set! buffer (+ start i) next)
		 (loop (+ i 1))))))))


						
