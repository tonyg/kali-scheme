;;;; Initialisation for SLIB for Scheme-48        -*-scheme-*-
;;; Copyright (C) 1992, 1993 Aubrey Jaffer.

;Permission to copy this software, to redistribute it, and to use it
;for any purpose is granted, subject to the following restrictions and
;understandings.

;1.  Any copy made of this software must include this copyright notice
;in full.

;2.  I have made no warrantee or representation that the operation of
;this software will be error-free, and I am under no obligation to
;provide any services, by way of maintenance, update, or otherwise.

;3.  In conjunction with products arising from the use of this
;material, there shall be no use of my name in any advertising,
;promotional, or sales literature without prior written consent in
;each case.


; For Scheme 48 version 0.30 and after.
;
; JAR's remarks:
;
;  FORMAT does (INTEGER->CHAR 9) and (INTEGER->CHAR 12), which are
;  unportable.
;
;  RECORD-TYPE-NAME (in record.scm) calls PERROR, which is undefined.
;
;  RANDOM:SIZE-FLOAT (in randinex.scm) assumes that inexact numbers
;  have finite precision, which is not necessarily true.
;
;  I couldn't find any documentation of interfaces.  For example, if
;  one claims that MACRO is provided, what does that mean?  What
;  definitions are supposed to become available?  What do I need to do
;  in order to provide an alternative implementation?

;;; (software-type) should be set to the generic operating system type.
;;; UNIX, VMS, MACOS, AMIGA and MSDOS are supported.

(define (software-type) 'UNIX)

;;; *FEATURES* should be set to a list of symbols describing features
;;; of this implementation.  See Template.scm for the list of feature
;;; names.

(define *features*
      '(
	rev4-report			;conforms to
	ieee-p1178			;conforms to
	rev4-optional-procedures
	multiarg/and-
	multiarg-apply
	rationalize
	delay				;has delay and force
	with-file
	char-ready?			;has
	values				;proposed multiple values
	eval				;slib:eval is single argument eval.
	dynamic-wind			;proposed dynamic-wind
	full-continuation		;can return multiple times
	macro				;R4RS appendix's DEFINE-SYNTAX
	))

;;; (OUTPUT-PORT-WIDTH <port>)
(define (output-port-width . arg) 79)

;;; (OUTPUT-PORT-HEIGHT <port>)
(define (output-port-height . arg) 24)

;;; (CURRENT-ERROR-PORT)
(define current-error-port
  (access-scheme-48 'current-error-port))

;;; (TMPNAM) makes a temporary file name.
(define tmpnam
  (let ((cntr 100))
    (lambda () (set! cntr (+ 1 cntr))
	    (let ((tmp (string-append "slib_" (number->string cntr))))
	      (if (file-exists? tmp) (tmpnam) tmp)))))

;;; (FILE-EXISTS? <string>)
(define (file-exists? f) #f)

;;; (DELETE-FILE <string>)
(define (delete-file f) #f)

;;; FORCE-OUTPUT flushes any pending output on optional arg output port
;;; use this definition if your system doesn't have such a procedure.
(define (force-output . arg)
  ((access-scheme-48 'force-output)
   (if (null? arg) (current-output-port) (car arg))))

;;; CHAR-CODE-LIMIT is one greater than the largest integer which can
;;; be returned by CHAR->INTEGER.
(define char-code-limit
  (+ 1 (char->integer ((access-scheme-48 'ascii->char) 255))))

;;; MOST-POSITIVE-FIXNUM is used in modular.scm
(define most-positive-fixnum #x1FFFFFFF)

;;; If your implementation provides eval, SLIB:EVAL is single argument
;;; eval using the top-level (user) environment.
(define slib:eval
  (let ((eval eval)
	(interaction-environment interaction-environment))
    (lambda (form)
      (eval form (interaction-environment)))))

;;; If your implementation provides R4RS macros:
(define macro:eval slib:eval)
(define macro:load load)

;;; define an error procedure for the library
(define slib:error (access-scheme-48 'error))

;;; define these as appropriate for your system.
(define slib:tab ((access-scheme-48 'ascii->char) 9))
(define slib:form-feed ((access-scheme-48 'ascii->char) 12))

;;; Define these if your implementation's syntax can support them and if
;;; they are not already defined.

(define (1+ n) (+ n 1))
(define (-1+ n) (+ n -1))
;(define 1- -1+)

;;; (implementation-vicinity) should be defined to be the pathname of
;;; the directory where any auxiliary files to your Scheme
;;; implementation reside.

(define (implementation-vicinity)
  (case (software-type)
    ((UNIX)      "=scheme48/")    ; Translated
;    ((VMS)      "scheme$src:")
;    ((MSDOS)    "C:\\scheme\\")
    (else (slib:error "unrecognized software-type" software-type))))

;;; (library-vicinity) should be defined to be the pathname of the
;;; directory where files of Scheme library functions reside.

(define (library-vicinity) "/usr/local/lib/slib/")

(define scheme-file-suffix
  (case (software-type)
    ((NOSVE) (lambda () "_scm"))
    (else (lambda () ".scm"))))

(define in-vicinity string-append)

(load (in-vicinity (library-vicinity) "require" (scheme-file-suffix)))
