; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Minimal full-I/O test system

(define (start arg in-channel out-channel error-channel)
  (set! *error-channel* error-channel)
  (set-exception-handlers! exception-handlers)
  (let* ((ch (open-channel "small-test.image"
			   "small-test.image"
			   (enum open-channel-option input-file)
			   #f))
	 (out (output-channel->port out-channel))
	 (in (input-channel->port in-channel)))
    (write-string "Hello " out)
    (collect)
    (if (< 0 (vector-length arg))
	(write-block (vector-ref arg 0)
		     0
		     (string-length (vector-ref arg 0))
		     out))
    (newline out)
    (force-output out)
    (let ((b (make-string 12 #\space)))
      (channel-read b 0 12 ch)
      (close-channel ch)
      (write-string b out)
      (newline out)
      (force-output out))
    (write-string "Eight chars> " out)
    (force-output out)
    (do ((i 0 (+ i 1)))
	((= i 8))
      (write-char (peek-char in) out)
      (read-char in))
    (newline out)
    (force-output out)
    (write-image "small-test.image" start "A small image")
    0))

(define buffer-size 4)  ; for testing

(define (output-channel->port channel)
  (make-port #f
	     'text-codec
	     #f
	     (bitwise-ior (arithmetic-shift 1 (enum port-status-options
						    output))
			  (arithmetic-shift 1 (enum port-status-options
						    open-for-output)))
	     #f		; lock
	     channel
	     (make-code-vector buffer-size 0)
	     0 buffer-size
	     #f #f))

(define (input-channel->port channel)
  (make-port #f
	     'text-codec
	     #f
	     (bitwise-ior (arithmetic-shift 1 (enum port-status-options
						    input))
			  (arithmetic-shift 1 (enum port-status-options
						    open-for-input)))
	     #f		; lock
	     channel
	     (make-code-vector buffer-size 0)
	     0 buffer-size
	     #f #f))

(define *error-channel* #f)

(define (error string . stuff)
  (channel-write-string string *error-channel*)
  (channel-newline *error-channel*)
  (exit -1))

(define (message string)
  (channel-write-string string *error-channel*)
  (channel-newline *error-channel*))

(define (channel-write-string string channel)
  (channel-write string
		 0
		 (string-length string)
		 channel))

(define (channel-newline channel)
  (channel-write-string "
" channel))

(define (define-vm-exception-handler opcode proc)
  (vector-set! vm-exception-handlers opcode proc))

(define vm-exception-handlers
  (make-vector op-count #f))

(define-vm-exception-handler (enum op write-char)
  (lambda (opcode reason char port)
    (cond ((= reason (enum exception buffer-full/empty))
	   (force-output port)
	   (message "[overflow]")
	   (write-char char port))
	  (else
	   (apply signal-vm-exception opcode reason args)))))

(define-vm-exception-handler (enum op read-char)
  (lambda (opcode reason port)
    (cond ((= reason (enum exception buffer-full/empty))
	   (fill-buffer port)
	   (message "[underflow]")
	   (read-char port))
	  (else
	   (apply signal-vm-exception opcode reason args)))))

(define-vm-exception-handler (enum op peek-char)
  (lambda (opcode reason port)
    (cond ((= reason (enum exception buffer-full/empty))
	   (fill-buffer port)
	   (message "[underflow]")
	   (peek-char port))
	  (else
	   (apply signal-vm-exception opcode reason args)))))

(define-vm-exception-handler (enum op write-block)
  (lambda (opcode reason thing start count port)
    (cond ((= reason (enum exception buffer-full/empty))
	   (force-output port)
	   (write-buffer thing start count (port-data port)))
	  (else
	   (apply signal-vm-exception opcode reason args)))))

(define (force-output port)
  (write-buffer (port-out-buffer port) 0 (port-out-index port) (port-data port))
  (set-port-out-index! port 0))

(define (write-buffer buffer start count channel)
  (let loop ((start start) (count count))
    (let ((sent (channel-write buffer start count channel)))
      (if (< sent count)
	  (loop (+ start sent) (- count sent))))))

(define (fill-buffer port)
  (let ((got (channel-read (port-in-buffer port)
			   0
			   (code-vector-length (port-in-buffer port))
			   (port-data port))))
    (cond ((= got 0)
	   (fill-buffer port))
	  (else
	   (set-port-in-index! port 0)
	   (set-port-in-limit! port got)))))

(define (write-string string port)
  (let ((l (string-length string)))
    (do ((i 0 (+ i 1)))
	((= i l))
      (write-char (string-ref string i) port))))

(define (newline port)
  (write-char #\newline port))
