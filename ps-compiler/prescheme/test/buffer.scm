; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.


; (port->stream port type) -> stream or error value
; (
;
;
;


(define-record-type stream
  make-stream
  (port   port)
  (type   int8u)
  (buffer int32)   ; pointer the start of the buffer
  (size   int32)
  (loc    int32)   ; pointer to the next char to be read or the next slot to
                   ; be written
  (limit  int32))  ; end of the available characters

(define buffer-size 1024)

(define (port->stream port type)
  (let ((buffer (allocate-memory buffer-size))
	(stream (make-stream)))
    (if (or (null-memory? buffer)
	    (null-pointer? stream))
	(error "out of memory"))
    (set-stream-port!   stream port)
    (set-stream-type!   stream type)
    (set-stream-buffer! stream buffer)
    (set-stream-size!   stream buffer-size)
    (set-stream-loc!    stream buffer)
    (set-stream-limit!  stream buffer)
    buffer))

(define (stream-read-char stream)
  (let ((loc (stream-loc stream)))
    (cond ((< loc (stream-limit stream))
	   (let ((ch (unsigned-byte-ref loc)))
	     (set-stream-loc! stream (+ 1 (stream-loc stream)))
	     ch))
	  (else
	   (let* ((buffer (stream-buffer stream))
		  (count (read-block (stream-port stream)
				     buffer
				     (stream-size stream))))
	     (cond ((= count 0)   ; EOF
		    0)
		   (else
		    (set-stream-loc! stream (+ buffer 1))
		    (set-stream-limit! stream (+ buffer count))
		    (unsigned-byte-ref buffer))))))))

; this will need to be PCLUSR'd.

(define (stream-write-char stream char)
  (let ((loc (stream-loc stream)))
    (cond ((< loc (stream-limit stream))
	   (unsigned-byte-set! loc char)
	   (set-stream-loc! stream (+ 1 (stream-loc stream))))
	  (else
	   (let* ((buffer (stream-buffer stream))
		  (count (write-block (stream-port stream)
				      buffer
				      (stream-limit stream))))
	     (cond ((= count 0)   ; EOF
		    0)
		   (else
		    (set-stream-loc! stream (+ buffer 1))
		    (set-stream-limit! stream (+ buffer count))
		    (unsigned-byte-ref buffer))))))))
