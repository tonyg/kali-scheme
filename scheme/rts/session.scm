; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.
; Session data

; The initializers are saved in images.

(define *session-data-initializers* '())

(define (make-session-data-slot! init)
  (let ((slot (length *session-data-initializers*)))
    (set! *session-data-initializers* (cons init *session-data-initializers*))
    (if (vector? (session-data))
	(set-session-data! (list->vector
			    (reverse
			     (cons init
				   (reverse (vector->list (session-data))))))))
    slot))

(define (session-data-ref slot)
  (vector-ref (session-data) slot))

(define (session-data-set! slot value)
  (vector-set! (session-data) slot value))

(define (initialize-session-data!)
  (set-session-data! (list->vector (reverse *session-data-initializers*))))
