; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; SRFI 36

(define-condition-type &i/o-error &error
  i/o-error?)

(define-condition-type &i/o-port-error &i/o-error
  i/o-port-error?
  (port i/o-error-port))

(define-condition-type &i/o-read-error &i/o-port-error
  i/o-read-error?)

(define-condition-type &i/o-write-error &i/o-port-error
  i/o-write-error?)

(define-condition-type &i/o-closed-error &i/o-port-error
  i/o-closed-error?)

(define-condition-type &i/o-filename-error &i/o-error
  i/o-filename-error?
  (filename i/o-error-filename))

(define-condition-type &i/o-malformed-filename-error &i/o-filename-error
  i/o-malformed-filename-error?)

(define-condition-type &i/o-file-protection-error &i/o-filename-error
  i/o-file-protection-error?)

(define-condition-type &i/o-file-is-read-only-error &i/o-file-protection-error
  i/o-file-is-read-only-error?)

(define-condition-type &i/o-file-already-exists-error &i/o-filename-error
  i/o-file-already-exists-error?)

(define-condition-type &i/o-no-such-file-error &i/o-filename-error
  i/o-no-such-file-error?)

(define-condition-type &read-error &error
  read-error?
  (line read-error-line)
  (column read-error-column)
  (position read-error-position)
  (span read-error-span))

; Local additions

(define-condition-type &primitive-i/o-error &i/o-error
  primitive-i/o-error?
  (status i/o-error-status)
  (operation i/o-error-operation)
  (arguments i/o-error-arguments))
