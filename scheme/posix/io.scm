; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Calls from Section 6 of POSIX.

; First some port manipulation routines.

(define (fd-port? port)
  (if (port->channel port)
      #t
      #f))

(define (port->fd port)
  (let ((channel (port->channel port)))
    (if channel
	(channel-os-index channel)
	#f)))

;----------------
; 4.7 Terminal Identification
; Out of place, but it makes more sense here.

(define (port-is-a-terminal? port)
  (cond ((not (port? port))
	 (call-error "not a port" port-is-a-terminal? port))
	((port->channel port)
	 => channel-is-a-terminal?)
	(else
	 #f)))

(define (port-terminal-name port)
  (cond ((not (port? port))
	 (call-error "not a port" port-terminal-name port))
	((port->channel port)
	 => (lambda (channel)
	      (byte-vector->os-string (channel-terminal-name channel))))
	(else
	 #f)))

(import-lambda-definition channel-is-a-terminal? (channel) "posix_is_a_tty")
(import-lambda-definition channel-terminal-name (channel) "posix_tty_name")

;----------------
; 6.1 Pipes

(define (open-pipe)
  (let ((in-out (call-imported-binding posix-pipe)))
    (values (input-channel->port (car in-out))
            (output-channel->port (cdr in-out)))))

(import-definition posix-pipe)

;----------------
; 6.2 File descriptor manipulation.

; (DUP <fd-port>)                    -> fd-port
; (DUP2 <fd-port> <file descriptor>) -> fd-port
; (DUP-SWITCHING_MODE <fd-port>)     -> fd-port
; (CHANNEL-DUP <channel>)                    -> channel
; (CHANNEL-DUP2 <channel> <file descriptor>) -> channel
; These change a ports file descriptor and return a new port (or channel)
; port that uses the old one's file descriptor.  DUP uses the lowest unused
; file descriptor, DUP2 uses the one provided.  If any existing channel
; uses the file descriptor passed to DUP2, that channel is closed.
; DUP-SWITCHING-MODE is the same as DUP except that the returned port has
; the opposite polarity.

(define (dup port)
  (let ((channel (maybe-x->channel port)))
    (if channel
	((if (input-port? port)
	     input-channel->port
	     output-channel->port)
	 (channel-dup channel))
	(call-error "argument cannot be coerced to channel" dup port))))

(define (channel-dup channel)
  (really-dup channel #f))

(define (dup-switching-mode port)
  (let ((channel (maybe-x->channel port)))
    (if channel
	(if (input-port? port)
	    (output-channel->port
	     (really-dup channel (enum channel-status-option output)))
	    (input-channel->port
	     (really-dup channel (enum channel-status-option input))))
	(call-error "argument cannot be coerced to channel" dup-switching-mode port))))

(define (dup2 port fd)
  (let ((channel (maybe-x->channel port)))
    (if channel
	((if (input-port? port)
	     input-channel->port
	     output-channel->port)
	 (channel-dup2 channel fd))
	(call-error "argument cannot be coerced to channel" dup2 port fd))))

(import-lambda-definition really-dup (channel new-status) "posix_dup")
(import-lambda-definition channel-dup2 (channel fd) "posix_dup2")

; A higher-level interface for DUP and DUP2.
;
; (remap-file-descriptors! . ports&channels)
;
; PORTS&CHANNELS gives the desired locations of the file descriptors associated
; with the ports and channels.  (REMAP-FILE-DESCRIPTORS! P1 #F P2) moves P1's
; file descriptor to 0 and P2's to 2.  All other channels are closed.  The same
; file descriptor may be moved to multiple locations.
;
; It would be nice if this closed the port associated with a closed channel,
; but it doesn't.
;
; This is a classical parallel assignment problem.  What we do is figure out a
; series of DUP()'s and DUP2()'s that produce the desired arrangement.
; FIND-TARGETS separates out the channels that must be moved to multiple file
; descriptors.  We do the parallel assignment, and then do any duplications.
; Finally, any channels which were not mentioned in PORTS&CHANNELS are
; marked close-on-exec.

(define (remap-file-descriptors! . ports&channels)
  (let ((channels (maybe-xs->channels ports&channels #t)))
    (if channels
	(call-with-values    
	 (lambda ()
	   (find-targets channels))
	 (lambda (targets extras)
	   (do-dups targets)
	   (for-each (lambda (pair)
		       (channel-dup2 (cdr pair) (car pair)))
		     extras)
	   (let ((channels (list->vector channels)))
	     (for-each (lambda (channel)
			 (let ((index (channel-os-index channel)))
			   (if (or (<= (vector-length channels) index)
				   (not (vector-ref channels index)))
			       (set-close-on-exec?! channel #t))))
		       (open-channels-list)))))
	(apply call-error "not all arguments can be mapped to channels"
	       remap-file-descriptors!
	       ports&channels))))

(define (close-all-but . ports&channels)
  (let ((channels (maybe-xs->channels ports&channels #f)))
    (if channels
	(for-each (lambda (channel)
		    (if (not (memq channel channels))
			(close-channel channel)))
		  (open-channels-list))
	(apply call-error "not all arguments can be mapped to channels" 
	       close-all-but
	       ports&channels))))

; Coerce PORT-OR-CHANNEL to a channel, if possible.

(define (maybe-x->channel port-or-channel)
  (cond ((channel? port-or-channel)
	 port-or-channel)
	((fd-port? port-or-channel)
	 (port->channel port-or-channel))
	(else
	 #f)))

; Coerce PORTS&CHANNELS to a list of channels, returning #F if any cannot
; be coerced.  If FALSE-OKAY? is true, then any #F's in the list are just
; passed along.

(define (maybe-xs->channels ports&channels false-okay?)
  (let loop ((todo ports&channels) (res '()))
    (cond ((null? todo)
	   (reverse res))
	  ((and false-okay?
		(not (car todo)))
	   (loop (cdr todo)
		 (cons #f res)))
	  ((maybe-x->channel (car todo))
	   => (lambda (channel)
		(loop (cdr todo)
		      (cons channel res))))
	  (else #f))))

; Returns two lists of pairs (<target-fd> . <channel>).  No channel appears twice
; in the first list and every channel in the second list appears in the first.

(define (find-targets channels)
  (call-with-values
   (lambda () 
     (fold->3 (lambda (channel i targets extras)
		(cond ((not channel)
		       (values (+ i 1)
			       targets
			       extras))
		      ((any (lambda (pair)
			      (eq? channel (cdr pair)))
			    targets)
		       (values (+ i 1)
			       targets
			       `((,i . ,channel) . ,extras)))
		      (else
		       (values (+ i 1)
			       `((,i . ,channel) . ,targets)
			       extras))))
	      channels
	      0
	      '()
	      '()))
   (lambda (i targets extras)
     (values targets extras))))

; TARGETS is a list of pairs (<wanted-fd> . <channel>).  We loop down doing
; DUP-TO-TARGET, which is guarenteed to make progress, but not guarenteed to
; actually move the argument we give it.
;
; All this depends on DUP and DUP2 switching the original channel to the new
; file descriptor and returning a new channel with the original file descriptor.

(define (do-dups targets)
  (if (not (null? targets))
      (let ((channel (cdar targets))
	    (target-fd (caar targets))
	    (rest (cdr targets)))
	(dup-to-target channel target-fd rest '())
	(do-dups (if (= (channel-os-index channel)
			target-fd)
		     rest
		     targets)))))

; Move CHANNEL to TARGET-FD.  TARGETS is a list of yet-to-be-done
; (<fd> . <channel>) pairs.  PENDING is a list of fd's we are waiting to move
; out of.  If TARGET-FD is in PENDING, then we have a loop and use dup() to move
; from HAVE-FD so some other location, thus breaking the loop.  If there is
; already someone in the location we want, we move them and then ourselves.

(define (dup-to-target channel target-fd targets pending)
  (let ((have-fd (channel-os-index channel)))
    (cond ((= target-fd have-fd))
	  ((memq target-fd pending)
	   (channel-dup channel))
	  (else
	   (let ((occupant (find-occupant target-fd targets)))
	     (if occupant
		 (dup-to-target (cdr occupant)
				(car occupant)
				targets
				(cons have-fd pending)))
	     (channel-dup2 channel target-fd))))))

; Return the (<wanted-fd> . <channel>) pair from TARGETS where <channel>
; currently has FD, if there is such.

(define (find-occupant fd targets)
  (let loop ((targets targets))
    (cond ((null? targets)
	   #f)
	  ((= fd (channel-os-index (cdar targets)))
	   (car targets))
	  (else
	   (loop (cdr targets))))))

;----------------
; 6.3 File Descriptor Reassignment
;
; int close(int fd)     ; Use close-{input|output}-{port|channel}
;
; 6.4 Input and Output
;
; read() and write()    ; Already available in various forms.
;----------------
; 6.5 Control Operations on Files

; fcntl(fd, F_DUPFD, target_fd)            ; Use DUP instead.

; Descriptor flags
; fcntl(fd, F_GETFD)
; fcntl(fd, F_SETFD, flags)
;
; The only POSIX flag is FD_CLOEXEC, so that's all we do.

(import-lambda-definition set-close-on-exec?! (channel bool)
			  "posix_set_close_on_exec")
(import-lambda-definition close-on-exec? (channel) "posix_close_on_exec_p")

; Status flags
; fcntl(fd, F_GETFL)
; fcntl(fd, F_SETFL, flags)

(define (i/o-flags port-or-channel)
  (let ((channel (maybe-x->channel port-or-channel)))
    (if channel
	(call-imported-binding posix-io-flags channel #f)
	(call-error "argument cannot be coerced to channel" i/o-flags port-or-channel))))

(define (set-i/o-flags! port-or-channel options)
  (let ((channel (maybe-x->channel port-or-channel)))
    (if (and channel
	     (file-options? options))
	(call-imported-binding posix-io-flags channel options)
	(call-error "argument type error" set-i/o-flags! port-or-channel options))))

(import-definition posix-io-flags)

; off_t lseek(int fd, off_t offset, int whence)

;----------------
; 6. File Synchronization
;
; int fsync(int fd)              ; optional
; int fdatasync(int fd)          ; optional
;
; 7. Asynchronous Input and Output
;
; All optional

