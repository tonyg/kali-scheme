; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Pipes.
;
; This would be easy except that we have to deal with threads (and who else
; would be using pipes?).
;
; Pipes either have a fixed set of buffers which are continually recycled or
; create new buffers as needed.  Having a fixed buffer supply keeps readers
; and writers more-or-less synchronised, while creating buffers as needed allows
; the writers to get arbitrarily far ahead of the readers.
;
; A fixed-buffer pipe has two buffers.  At any point one is acting as the
; input buffer and the other as the output buffer.  When the input buffer is
; empty the two are swapped.
;
; The complexity of the code below comes from having to deal with the two
; blocking situations:
;  - a read is done when all buffers are empty
;  - a write is done when the output buffer is full and the input buffer
;    is non-empty (and we aren't allowed to make more buffers)
; 
; If a read occurs when all buffers are empty we swap a zero-length buffer in
; for the output-buffer and block on a condition variable.  The zero-length
; buffer guarantees that the reading thread will be woken when the next write
; occurs.  When a write occurs with a zero-length buffer we swap in the real
; buffer, do the write, and then set the input condition variable.
;
; When a write occurs with the write buffer full for a pipe without a fixed
; set of buffers the full buffer is added to a queue.
;
; For a pipe with only two buffers, if a write occurs with the write buffer
; full and the read buffer non-empty, we set the read-limit to be one shorter
; than its real value and block on a condition variable.  The bogus read-limit
; means that the writing thread will be woken when a read empties the buffer
; and not have to wait until the following read.  When a read reaches the
; read-limit we check to see if there are waiting output threads.  If so, we
; write one more character and then wake the sleepers.
;
; If this were a little more integrated with the threads package pipes
; could use queues instead of making new condition variables all the time.

(define-record-type pipe-data :pipe-data
  (make-pipe-data lock in-condvar out-condvar queue out-buffer)
  pipe-data?
  (lock pipe-lock)   ; a lock for keeping this pipe single threaded
  (in-condvar        ; waiting for a non-empty buffer
   pipe-in-condvar
   set-pipe-in-condvar!)
  (out-condvar       ; waiting for an empty buffer
   pipe-out-condvar
   set-pipe-out-condvar!)
  (queue             ; queue of full buffers, or #f for a pipe with a fixed
   pipe-buffer-queue);   buffer set
  (out-buffer        ; stashed output buffer
   pipe-out-buffer
   set-pipe-out-buffer!))
    
(define (lock pipe)
  (obtain-lock (pipe-lock (port-data pipe))))

(define (unlock pipe)
  (release-lock (pipe-lock (port-data pipe))))

; Swap the buffers and initialize the various buffer pointers.

(define (swap-buffers! port)
  (let ((temp (port-in-buffer port)))
    (set-port-in-buffer! port (port-out-buffer port))
    (set-port-out-buffer! port temp)
    (set-port-in-limit! port (port-out-index port))
    (set-port-in-index! port 0)
    (set-port-out-index! port 0)))

; Get a full buffer from the queue.

(define (use-buffer-from-queue port)
  (let ((queue (pipe-buffer-queue (port-data port))))
    (if (and queue (not (queue-empty? queue)))
	(let ((buffer (dequeue! queue)))
	  (set-port-in-buffer! port buffer)
	  (set-port-in-index! port 0)
	  (set-port-in-limit! port (code-vector-length buffer))
	  #t)
	#f)))

;----------------------------------------------------------------
; Input buffers

; Get a non-empty input buffer, if possible.  We have five options:
;  1. The current buffer isn't empty.
;  2. The current buffer looks empty but really isn't; the limit was
;     decremented by a writer who wants an empty buffer.
;  3. There is a queue and it has a full buffer.
;  4. The output buffer isn't empty.
;  5. The output port is still open and may produce characters in the future.
; If there are no characters and the output port is closed we lose.

(define (get-in-buffer port)
  (cond ((> (port-in-limit port)
	    (port-in-index port))
	 #t)
	((pipe-out-condvar (port-data port))
	 (set-port-in-limit! port (+ 1 (port-in-limit port)))
	 #t)
	((use-buffer-from-queue port)
	 #t)
	((< 0 (port-out-index port))
	 (swap-buffers! port)
	 #t)
	((open-output-port? port)
	 (wait-for-input port)
	 (get-in-buffer port))
	(else
	 #f)))

; Wait on the input condition variable.  If there isn't one, we make a
; new condition variable and swap in a zero-length write buffer to get the
; condition variable set as soon as a write occurs.

(define (wait-for-input port)
  (let* ((data (port-data port))
	 (cv (if (pipe-in-condvar data)
		 (pipe-in-condvar data)
		 (let ((cv (make-condvar)))
		   (set-pipe-out-buffer! (port-data port) (port-out-buffer port))
		   (set-port-out-buffer! port (make-code-vector 0 0))
		   (set-pipe-in-condvar! data cv)
		   cv))))
    (release-lock (pipe-lock data))
    (condvar-ref cv)
    (obtain-lock (pipe-lock data))))

; Wake any threads waiting for input if there are characters available.

(define (wake-any-input-waiters port)
  (let ((data (port-data port)))
    (let ((cv (pipe-in-condvar data)))
      (if (and cv
	       (or (< 0 (port-out-index port))
		   (not (open-output-port? port))))
	  (begin
            (set-pipe-in-condvar! data #f)
	    (condvar-set! cv (unspecific)))))))

;----------------------------------------------------------------
; Output buffers

; Get a non-full output buffer, if possible.  We have five options:
;  1. The current buffer has room.
;  2. The current buffer looks full but really isn't; it is a zero-length
;     buffer swapped in by a reader who wants characters.
;  3. There is a queue for full buffers.
;  4. The input buffer is empty.
;  5. The input port is still open and may empty its buffer later on.
; If there are no empty buffers and the input port is closed we lose.

(define (get-out-buffer port)
  (let ((len (code-vector-length (port-out-buffer port))))
    (cond ((< (port-out-index port) len)
           #t)
	  ((= 0 len)
	   (set-port-out-buffer! port (pipe-out-buffer (port-data port)))
	   #t)
	  ((pipe-buffer-queue (port-data port))
	   (make-new-out-buffer port)
	   #t)
	  ((= (port-in-index port) (port-in-limit port))
           (swap-buffers! port)
	   #t)
	  ((open-input-port? port)
	   (wait-for-output port)
	   (get-out-buffer port))
	  (else
	   #f))))

; Make a new output buffer and put the full one on the queue.

(define (make-new-out-buffer port)
  (let* ((old (port-out-buffer port))
	 (new (make-code-vector (code-vector-length old) 0)))
    (enqueue! (pipe-buffer-queue (port-data port)) old)
    (set-port-out-buffer! port new)
    (set-port-out-index! port 0)))

; Same as above, on a different condition variable and with a different
; wakeup method.

(define (wait-for-output port)
  (let* ((data (port-data port))
	 (cv (if (pipe-out-condvar data)
                 (pipe-out-condvar data)
                 (let ((cv (make-condvar)))
		   (set-port-in-limit! port (- (port-in-limit port) 1))
		   (set-pipe-out-condvar! data cv)
		   cv))))
    (release-lock (pipe-lock data))
    (condvar-ref cv)
    (obtain-lock (pipe-lock data))))

(define (wake-any-output-waiters port)
  (let ((data (port-data port)))
    (let ((cv (pipe-out-condvar data)))
      (if (and cv
	       (or (= (port-in-limit port) (port-in-index port))
		   (not (open-output-port? port))))
	  (begin
            (set-pipe-out-condvar! data #f)
	    (condvar-set! cv (unspecific)))))))

; Used by PEEK-CHAR to reset the wakeup limit.

(define (do-not-disturb-output-waiters port)
  (if (pipe-out-condvar (port-data port))
      (set-port-in-limit! port (- (port-in-limit port) 1))))

; Close both ports and wake up any sleepers.

(define (close-pipe port close-input?)
  (lock port)
  (if close-input? 
      (make-input-port-closed! port))
  (make-output-port-closed! port)
  (wake-any-input-waiters port)
  (wake-any-output-waiters port)
  (unlock port))

;----------------------------------------------------------------
; The actual handler

(define pipe-handler
  (make-port-handler

   ;; discloser
   (lambda (port)
     (list 'pipe))

   ;; input port methods --------------------------

   ;; close-input-port
   (lambda (port)
     (close-pipe port #t))

   ;; The next three methods are called when the input buffer is empty
   ;; read-char
   (lambda (port)
     (lock port)
     (cond ((get-in-buffer port)
	    (let ((c (read-char port)))
	      (wake-any-output-waiters port)
	      (unlock port)
	      c))
	   (else
            (unlock port)
	    (eof-object))))

   ;; peek-char
   (lambda (port)
     (lock port)
     (cond ((get-in-buffer port)
	    (let ((c (peek-char port)))
	      (do-not-disturb-output-waiters port)
	      (unlock port)
	      c))
	   (else
            (unlock port)
	    (eof-object))))

   ;; char-ready?
   (lambda (port)
     (> (port-out-index port) 0))

   ;; read-block - the buffer has fewer than COUNT characters
   (lambda (thing start count port)
     (lock port)
     (let loop ((start start) (count count))
       (let* ((index (port-in-index port))
	      (have (min (- (port-in-limit port) index)
			 count)))
	 (cond ((> have 0)
		(copy! (port-in-buffer port) index thing start have)
		(set-port-in-index! port (+ index have))))
	 (wake-any-output-waiters port)
	 (cond ((= have count)
		(unlock port))
	       ((get-in-buffer port)
                (loop (+ start have) (- count have)))
	       (else
		(unlock port)
		(eof-object))))))

   ;; output port methods -------------------------

   ;; close-output-port
   (lambda (port)
     (close-pipe port #f))

   ;; write-char got a full buffer
   (lambda (char port)
     (lock port)
     (cond ((get-out-buffer port)
	    (write-char char port)
	    (wake-any-input-waiters port)
	    (unlock port))
	   (else
            (unlock port)
	    (assertion-violation 'write-char  "writing to a broken pipe"))))

   ;; write-block couldn't fit COUNT characters into the buffer
   (lambda (thing start count port)
     (lock port)
     (let loop ((start start) (count count))
       (cond ((get-out-buffer port)
	      (let* ((buffer (port-out-buffer port))
		     (index (port-out-index port))
		     (have (min (- (code-vector-length buffer) index)
				count)))
                (cond ((> have 0)
		       (copy! thing start buffer index have)
		       (set-port-out-index! port (+ index have))))
		(wake-any-input-waiters port)
		(if (= have count)
		    (unlock port)
		    (loop (+ start have) (- count have)))))
	     (else
	      (unlock port)
	      (assertion-violation 'write-block "writing to a broken pipe")))))

   ;; force-output
   (lambda (port)
     (values))))

(define pipe-buffer-size 1024)

; Takes an optional size to use for the buffers.  A size of #f indicates
; that buffers should be made as needed (we really need omega).

(define (make-pipe . maybe-buffer-size)
  (call-with-values
   (lambda ()
     (parse-make-pipe-args maybe-buffer-size))
   (lambda (size queue)
     (make-port pipe-handler
		(bitwise-ior open-input-port-status
			     open-output-port-status)
		(make-pipe-data (make-lock) ; the lock
				#f	    ; input condition variable
				#f	    ; output condition variable
				queue       ; full buffer queue
				#f)	    ; stashed output buffer
		(make-code-vector size 0)   ; input buffer
		0			    ; input index
		0			    ; input limit
		(make-code-vector size 0)   ; output buffer
		0))))			    ; output limit
  
(define (parse-make-pipe-args maybe-buffer-size)
  (if (null? maybe-buffer-size)
      (values pipe-buffer-size #f)
      (let ((size (car maybe-buffer-size)))
	(cond ((not size)
	       (values pipe-buffer-size (make-queue)))
	      ((and (integer? size)
		    (exact? size)
		    (< 0 size))
	       (values size #f))
	      (else
	       (assertion-violation 'make-pipe "invalid pipe buffer size" size))))))

; These should probably be moved to I/O

(define (open-input-port? port)
  (= (bitwise-and open-input-port-status
		  (port-status port))
     open-input-port-status))

(define (open-output-port? port)
  (= (bitwise-and open-output-port-status
		  (port-status port))
     open-output-port-status))

; Won't do string->string copies.

(define (copy! from i to j count)
  (if (code-vector? from)
      (if (code-vector? to)
	  (copy-bytes! from i to j count)
	  (copy-bytes->chars! from i to j count))
      (copy-chars->bytes! from i to j count)))

; Copied from more-port.scm.

(define (copy-bytes! from i to j count)
  (let ((limit (+ count i)))
    (do ((i i (+ i 1))
	 (j j (+ j 1)))
	((= i limit))
      (code-vector-set! to j (code-vector-ref from i)))))

(define (copy-chars->bytes! from i to j count)
  (let ((limit (+ count i)))
    (do ((i i (+ i 1))
	 (j j (+ j 1)))
	((= i limit))
      (code-vector-set! to j (char->ascii (string-ref from i))))))

(define (copy-bytes->chars! from i to j count)
  (let ((limit (+ count i)))
    (do ((i i (+ i 1))
	 (j j (+ j 1)))
	((= i limit))
      (string-set! to j (ascii->char (code-vector-ref from i))))))

