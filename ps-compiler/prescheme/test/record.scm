

(define-record-type stream make-stream
  (port   port  stream-port   set-stream-port!)
  (buffer int32 stream-buffer set-stream-buffer!)
  (size   int32 stream-size   set-stream-size!)
  (loc    int32 stream-loc    set-stream-loc!)
  (limit  int32 stream-limit  set-stream-limit!))

(define buffer-size 1024)

(define (port->stream port)
  (let ((buffer (allocate-memory buffer-size))
	(stream (make-stream)))
    (set-stream-port!   stream port)
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
