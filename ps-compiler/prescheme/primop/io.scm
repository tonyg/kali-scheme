; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.


;(define-scheme-primop cast-to-long)

(define-scheme-primop stdin  type/input-port)
(define-scheme-primop stdout type/output-port)
(define-scheme-primop stderr type/output-port)

(define-nonsimple-scheme-primop read-char io)
(define-nonsimple-scheme-primop peek-char io)
(define-nonsimple-scheme-primop read-integer io)

(define type/status type/integer)

(define-nonsimple-scheme-primop write-char io)

(define-scheme-primop write-string  io type/status)
(define-scheme-primop write-integer io type/status)
(define-scheme-primop force-output  io type/status)

(define-nonsimple-scheme-primop open-input-file)
(define-nonsimple-scheme-primop open-output-file)

(define-scheme-primop close-input-port  io type/status)
(define-scheme-primop close-output-port io type/status)

(define-scheme-primop abort io type/unit)

(define-nonsimple-scheme-primop error io)

(define-scheme-primop error-string type/string)

(define-scheme-primop call-external-value io type/integer)

(define-nonsimple-scheme-primop read-block io)
(define-scheme-primop write-block io type/status)

