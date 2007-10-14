; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.


; (port->stream port type) -> stream or error value
; (
;
;
;


(define-record-type stream :stream
  (make-stream port type buffer size loc limit)
  (port   input-port stream-port set-stream-port!)
  (type   integer stream-type set-stream-type!)
  ;; pointer the start of the buffer
  (buffer address stream-buffer set-stream-buffer!)
  (size   integer stream-size set-stream-size!)
  ;; pointer to the next char to be read or the next slot to be written
  (loc    address stream-loc set-stream-loc!)
  ;; end of the available caharacters
  (limit  address stream-limit set-stream-limit!))

(define-record-type z :z
  (make-z a)
  (a stream z-a set-z-a!))

(define buffer-size 1024)

(define (port->stream port type)
  (let* ((buffer (allocate-memory buffer-size))
	 (stream (make-stream port type buffer buffer-size buffer buffer)))
    (if (or ; (null-memory? buffer)
	 (null-pointer? stream))
	(error "out of memory"))
    (make-z stream)))

(define (stream-read-char stream)
  (let ((loc (stream-loc stream)))
    (cond ((address< loc (stream-limit stream))
	   (let ((ch (unsigned-byte-ref loc)))
	     (set-stream-loc! stream (address+ (stream-loc stream) 1))
	     ch))
	  (else
	   (let ((buffer (stream-buffer stream)))
	     (call-with-values
	      (lambda ()
		(read-block (stream-port stream)
			    buffer
			    (stream-size stream)))
	      (lambda (count ignore status)
		(cond ((= count 0)   ; EOF
		       0)
		      (else
		       (set-stream-loc! stream (address+ buffer 1))
		       (set-stream-limit! stream (address+ buffer count))
		       (unsigned-byte-ref buffer))))))))))

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
