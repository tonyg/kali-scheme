; Copyright (c) 1994 Richard Kelsey.  See file COPYING.


;(define-scheme-primop cast-to-long #f  default-simplifier)

(define (r type)
  (lambda (call) type))

(define-scheme-primop stdin  #f  (r type/input-port)  default-simplifier)
(define-scheme-primop stdout #f  (r type/output-port) default-simplifier)
(define-scheme-primop stderr #f  (r type/output-port) default-simplifier)

(define-nonsimple-scheme-primop read-char 'io default-simplifier)
(define-nonsimple-scheme-primop peek-char 'io default-simplifier)
(define-nonsimple-scheme-primop read-integer 'io default-simplifier)

(define type/status type/int8u)

(define-nonsimple-scheme-primop write-char 'io default-simplifier)
(define-scheme-primop write-string  'io (r type/status)    default-simplifier)
(define-scheme-primop write-integer 'io (r type/status)    default-simplifier)

(define-scheme-primop force-output  'io (r type/status)    default-simplifier)
(define-nonsimple-scheme-primop open-input-file  #f default-simplifier)
(define-nonsimple-scheme-primop open-output-file #f default-simplifier)
(define-scheme-primop close-input-port  'io (r type/status) default-simplifier)
(define-scheme-primop close-output-port 'io (r type/status) default-simplifier)

(define-scheme-primop  abort        'io (r type/unit)    default-simplifier)
(define-nonsimple-scheme-primop error 'io default-simplifier)
(define-scheme-primop error-string #f (r type/string) default-simplifier)

(define-scheme-primop call-external-value 'io (r type/int32) default-simplifier)

(define-nonsimple-scheme-primop read-block 'io default-simplifier)
(define-scheme-primop write-block 'io (r type/status) default-simplifier)

