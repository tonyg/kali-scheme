; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Additional port types

(define close-port (structure-ref primitives close-port))
(define write-string (structure-ref ports write-string))

; Keeping track of a port's current row and column.

(define-record-type port-location
  ()
  ((row 0)
   (column 0)))

(define make-port-location port-location-maker)

(define (update-row-and-column location char)
  (cond ((eof-object? char) (values))
	((char=? char #\newline)
	 (set-port-location-row! location (+ 1 (port-location-row location)))
	 (set-port-location-column! location 0))
	(else
	 (set-port-location-column! location
				    (+ 1 (port-location-column location))))))

(define (update-row-and-column-from-string location string)
  (let loop ((i 0)
	     (row (port-location-row location))
	     (column (port-location-column location)))
    (cond ((>= i (string-length string))
	   (set-port-location-row!    location row)
	   (set-port-location-column! location column))
	  ((char=? #\newline (string-ref string i))
	   (loop (+ i 1) (+ row 1) 0))
	  (else
	   (loop (+ i 1) row (+ column 1))))))

; Input ports that keep track of the current row and column.

(define-record-type input-port-data
  (sub-port)
  ((location (make-port-location))))

(define input-port-methods
  (make-input-port-methods
   (lambda (data)
     (close-port (input-port-data-sub-port data)))
   (lambda (data)
     (let ((char (read-char (input-port-data-sub-port data))))
       (update-row-and-column (input-port-data-location data) char)
       char))
   (lambda (data)
     (peek-char (input-port-data-sub-port data)))
   (lambda (data)
     (char-ready? (input-port-data-sub-port data)))
   (lambda (data)
     (port-location-column (input-port-data-location data)))
   (lambda (data)
     (port-location-row (input-port-data-location data)))))

(define (make-tracking-input-port sub-port)
  (make-extensible-input-port (input-port-data-maker sub-port)
			       input-port-methods))

; Output ports that keep track of the current row and column.

(define-record-type output-port-data
  (sub-port)
  ((location (make-port-location))))

(define output-port-methods
  (make-output-port-methods
   (lambda (data)
     (close-port (output-port-data-sub-port data)))
   (lambda (data char)
     (write-char char (output-port-data-sub-port data))
     (update-row-and-column (output-port-data-location data) char))
   (lambda (data string)
     (write-string string (output-port-data-sub-port data))
     (update-row-and-column-from-string (output-port-data-location data)
					string))
   (lambda (data)
     (force-output (output-port-data-sub-port data)))
   (lambda (data)
     (let ((location (output-port-data-location data)))
       (cond ((not (= 0 (port-location-column location)))
	      (write-char #\newline (output-port-data-sub-port data))
	      (set-port-location-column! location 0)
	      (set-port-location-row! location
				      (+ 1 (port-location-row location)))))))
   (lambda (data)
     (port-location-column (output-port-data-location data)))
   (lambda (data)
     (port-location-row (output-port-data-location data)))))

(define (make-tracking-output-port sub-port)
  (make-extensible-output-port (output-port-data-maker sub-port)
			       output-port-methods))

;------------------------------------------------------------------------------
; String input ports

(define-record-type string-input-port-data
  (string)
  ((location (make-port-location))
   (index 0)))

(define (make-string-input-port string)
  (make-extensible-input-port (string-input-port-data-maker string)
			      string-input-port-methods))

(define string-input-port-methods
  (make-input-port-methods
   (lambda (data)
     (set-string-input-port-data-index!
      (string-length (string-input-port-data-string data))))
   (lambda (data)
     (let ((string (string-input-port-data-string data))
	   (index (string-input-port-data-index data)))
       (cond ((>= index (string-length string))
	      eof-object)
	     (else
	      (let ((char (string-ref string index)))
		(set-string-input-port-data-index! data (+ index 1))
		(update-row-and-column (string-input-port-data-location data)
				       char)
		char)))))
   (lambda (data)
     (let ((string (string-input-port-data-string data))
	   (index (string-input-port-data-index data)))
       (if (>= index (string-length string))
	   eof-object
	   (string-ref string index))))
   (lambda (data)
     (let ((string (string-input-port-data-string data))
	   (index (string-input-port-data-index data)))
       (< index (string-length string))))
   (lambda (data)
     (port-location-column (string-input-port-data-location data)))
   (lambda (data)
     (port-location-row (string-input-port-data-location data)))))

;------------------------------------------------------------------------------
; String output ports

(define-record-type string-output-port-data
  ()
  ((location (make-port-location))
   (strings '())
   (index string-port-string-length)
   (open? #t)))

(define (make-string-output-port)
  (make-extensible-output-port (string-output-port-data-maker)
			       string-output-port-methods))

; The length of the strings used in STRING-OUTPUT-PORTs.
(define string-port-string-length 80)

; Write a character to a string-output-port.  If there is not room in the
; current string, make a new one and put the character in that; otherwise put
; the character in the current string and increment the index.

(define (write-char-to-string char data)
  (let ((index   (string-output-port-data-index   data))
	(strings (string-output-port-data-strings data)))
    (cond ((>= index string-port-string-length)
	   (let ((new (make-string string-port-string-length #\space)))
	     (string-set! new 0 char)
	     (set-string-output-port-data-strings! data (cons new strings))
	     (set-string-output-port-data-index! data 1)))
	  (else
	   (string-set! (car strings) index char)
	   (set-string-output-port-data-index! data (+ index 1))))))

; UPDATE-ROW-AND-COLUMN-FROM-STRING could be integrated with this.

(define (write-string-to-string from data)
  (let ((index   (string-output-port-data-index   data))
	(strings (string-output-port-data-strings data)))
    (let loop ((i 0) (index index) (strings strings))
      (cond ((>= i (string-length from))
	     (set-string-output-port-data-index! data index)
	     (set-string-output-port-data-strings! data strings))
	    ((>= index string-port-string-length)
	     (let ((new (make-string string-port-string-length #\space)))
	       (string-set! new 0 (string-ref from i))
	       (loop (+ i 1) 1 (cons new strings))))
	    (else
	     (string-set! (car strings) index (string-ref from i))
	     (loop (+ i 1) (+ index 1) strings))))))

; Concatenates all of the strings of characters in WRITER into a single
; string.  Nothing is done if WRITER is not a string-output-port.

(define (string-output-port-output port)
  (let* ((data (extensible-output-port-local-data port))
	 (strings (string-output-port-data-strings data))
	 (index (string-output-port-data-index data)))
    (if (null? strings)
	""
	(let* ((total (+ index (* (length (cdr strings))
				  string-port-string-length)))
	       (result (make-string total #\space)))
	  (do ((i 0 (+ i string-port-string-length))
	       (s (reverse (cdr strings)) (cdr s)))
	      ((null? s)
	       (string-insert result (car strings) i index))
	    (string-insert result (car s) i string-port-string-length))
	  result))))

; Copy the first COUNT characters from FROM to TO, putting them from START
; onwards.

(define (string-insert to from start count)
  (do ((i 0 (+ i 1)))
      ((>= i count))
    (string-set! to (+ start i) (string-ref from i))))

(define string-output-port-methods
  (make-output-port-methods
   (lambda (data)
     (set-string-output-port-data-open?! data #f))
   (lambda (data char)
     (cond ((string-output-port-data-open? data)
	    (write-char-to-string char data)
	    (update-row-and-column (string-output-port-data-location data)
				   char))
	   (else
	    (error "writing to closed port" data)))) ; not a great argument
   (lambda (data string)
     (cond ((string-output-port-data-open? data)
	    (write-string-to-string string data)
	    (update-row-and-column-from-string
	     (string-output-port-data-location data)
	     string))
	   (else
	    (error "writing to closed port" data)))) ; not a great argument
   (lambda (data)
     #f)  ; nothing to do on a force-output
   (lambda (data)
     (let ((location (string-output-port-data-location data)))
       (cond ((not (string-output-port-data-open? data))
	      (error "writing to closed port" data))  ; not a great argument
	     ((not (= 0 (port-location-column location)))
	      (write-char-to-string #\newline data)
	      (set-port-location-column! location 0)
	      (set-port-location-row! location
				      (+ 1 (port-location-row location)))))))
   (lambda (data)
     (port-location-column (string-output-port-data-location data)))
   (lambda (data)
     (port-location-row (string-output-port-data-location data)))))

(define (call-with-string-output-port proc)
  (let ((port (make-string-output-port)))
    (proc port)
    (string-output-port-output port)))

;------------------------------------------------------------------------------
; Output ports from a single character writer

(define char-at-a-time-output-port-methods
  (make-output-port-methods
   (lambda (data) #f) ; nothing to do on a close
   (lambda (data char)
     (data char))
   (lambda (data string)
     (do ((i 0 (+ i 1)))
	 ((>= i (string-length string)))
       (data (string-ref string i))))
   (lambda (data)
     #f)  ; nothing to do on a force-output
   (lambda (data)
     (data #\newline))
   (lambda (data)
     #f)
   (lambda (data)
     #f)))

(define (make-char-at-a-time-output-port proc)
  (make-extensible-output-port proc
			       char-at-a-time-output-port-methods))

(define (write-one-line port count proc)
  (call-with-current-continuation
    (lambda (quit)
      (proc (make-char-at-a-time-output-port
	     (lambda (char)
	       (write-char char port)
	       (set! count (- count 1))
	       (if (<= count 0)
		   (quit #f))))))))

; Unix-specific kludge

(define eof-object (call-with-input-file "/dev/null" read-char))
