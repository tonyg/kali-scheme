(define-record-type async-channel :async-channel
  (really-make-async-channel in-channel out-channel)
  async-channel?
  (in-channel async-channel-in-channel)
  (out-channel async-channel-out-channel))

(define (make-async-channel)
  (let ((in-channel (make-channel))
	(out-channel (make-channel)))
    (spawn
     (lambda ()
       (let ((queue (make-queue)))
	 (let loop ()
	   (if (queue-empty? queue)
	       (begin
		 (enqueue! queue (receive in-channel))
		 (loop))
	       (select
		(wrap (receive-rv in-channel)
		      (lambda (message)
			(enqueue! queue message)
			(loop)))
		(wrap (send-rv out-channel (queue-head queue))
		      (lambda (ignore)
			(dequeue! queue)
			(loop)))))))))
    (really-make-async-channel in-channel
			       out-channel)))

(define (send-async channel message)
  (send (async-channel-in-channel channel) message))

(define (receive-async-rv channel)
  (receive-rv (async-channel-out-channel channel)))

(define (receive-async channel)
  (sync (receive-async-rv channel)))
