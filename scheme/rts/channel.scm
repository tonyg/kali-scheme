; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Provides input and output channels.

(define (input-channel? thing)
  (and (channel? thing)
       (or (= (channel-status thing)
	      (enum channel-status-option input))
	   (= (channel-status thing)
	      (enum channel-status-option special-input)))))

(define (output-channel? thing)
  (and (channel? thing)
       (or (= (channel-status thing)
	      (enum channel-status-option output))
	   (= (channel-status thing)
	      (enum channel-status-option special-output)))))

(define (open-input-channel filename)
  (let ((channel (open-channel filename (enum channel-status-option input))))
    (if (channel? channel)
	channel
	(error "cannot open input file" filename))))

(define (open-output-channel filename)
  (let ((channel (open-channel filename (enum channel-status-option output))))
    (if (channel? channel)
	channel
	(error "cannot open output file" filename))))

(define (close-input-channel channel)
  (if (input-channel? channel)
      (close-channel channel)
      (call-error close-input-channel channel)))

(define (close-output-channel channel)
  (if (output-channel? channel)
      (close-channel channel)
      (call-error close-output-channel channel)))
