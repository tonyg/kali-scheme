; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Some Unix system calls.  By no means all of them.
; Cf. external.c

(define pipe
  (let ((s48_pipe (get-external "s48_pipe")))
    (lambda ()
      (let ((results (cons 0 0)))
	(unix-system-call s48_pipe 'pipe results)
	(values (car results) (cdr results))))))

(define waitpid
  (let ((s48_waitpid (get-external "s48_waitpid")))
    (lambda (pid options)
      (let ((results (cons 0 0)))
	(unix-system-call s48_waitpid 'waitpid results pid options)
	(values (car results) (cdr results))))))

(define fork
  (let ((s48_fork (get-external "s48_fork")))
    (lambda ()
      (let ((results (cons -2 #f)))
	(unix-system-call s48_fork 'fork results)
	(car results)))))

(define dup
  (let ((s48_dup (get-external "s48_dup")))
    (lambda (fd)
      (let ((result (cons 0 0)))
	(unix-system-call s48_dup 'dup result fd)
	(car result)))))

(define close
  (let ((s48_close (get-external "s48_close")))
    (lambda (fd)
      (unix-system-call s48_close 'close fd))))

(define execv
  (let ((s48_execv (get-external "s48_execv")))
    (lambda (path argv)
      (unix-system-call s48_execv 'execv path argv)
      (error "execv returned?" path argv))))

(define exit
  (let ((s48_exit (get-external "s48_exit")))
    (lambda args
      (apply external-call s48_exit args)
      (error "exit returned!?"))))


; Utility

(define (unix-system-call external id . args)
  (let loop ()
    (let ((errno (apply external-call external args)))
      (if errno
	  (if (= errno 4)		;EINTR
	      (begin (warn "interrupted system call" id)
		     (loop))
	      (apply call-error (strerror errno) id args))
	  #t))))

; Utility for printing error messages

(define strerror
  (let ((s48_strerror (get-external "s48_strerror")))
    (lambda (n)
      (let* ((s (make-string 100))
	     (l (external-call s48_strerror s n)))
	(if (integer? l)
	    (substring s 0 l)
	    (call-error "miscellaneous error" strerror n))))))



; To coerce a file descriptor to a channel:
; ,open architecture channels
;   (open-channel in-fd
;		  (enum open-channel-option
;			raw-input-channel))
;   (open-channel out-fd
;		  (enum open-channel-option
;			raw-output-channel))

; To coerce a channel to a port:
; ,open i/o-internal
;   (input-channel->port <channel> 1024) ; buffer size
;   (output-channel->port <channel> 1024)
; The inverse operation is not available.

; To coerce a channel to a file descriptor:
;   (vm-extension 30 channel) => fd
