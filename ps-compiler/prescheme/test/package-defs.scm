; Copyright (c) 1993-2004 by Richard Kelsey.  See file COPYING.


(define-structures ((fact (export *one* fact all)))
  (open prescheme)
  (files fact))

(define-structures ((list (export test)))
  (open prescheme ps-memory)
  (files list))

(define-structures ((dispatch (export main)))
  (open prescheme)
  (files dispatch))

(define-structures ((vector (export test)))
  (open prescheme)
  (files vector))

(define-structures ((string (export test)))
  (open prescheme)
  (files string))

(define-structures ((goto (export test odd?)))
  (open prescheme)
  (files goto))

(define-structures ((external (export test)))
  (open prescheme)
  (files external))

(define-structures ((eval (export f4)))
  (open prescheme)
  (files eval))

(define-structures ((memory (export test)))
  (open prescheme)
  (files memory))

(define-structures ((poly (export test)))
  (open prescheme)
  (files poly))

(define-structures ((select (export test)))
  (open prescheme)
  (files select))

(define-structures ((hoist (export test)))
  (open prescheme)
  (files hoist))

(define-structures ((record (export init cons member? reverse!)))
  (open prescheme ps-record-types)
  (files record))

(define-structures ((buffer (export port->stream stream-read-char)))
  (open prescheme)
  (files buffer))

(define-structures ((values (export test)))
  (open prescheme)
  (files values))

(define-structures ((write (export test)))
  (open prescheme)
  (files write))

