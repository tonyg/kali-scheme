; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; The DEFINE-PACKAGE macro expands into code containing the form
; (%FILE-NAME%), which is supposed to get the name of the file in
; which the DEFINE-PACKAGE form occurs.  If the file is loaded using
; the LOAD-CONFIGURATION in link/link.scm, then the following will
; work.  This may also work in other circumstance as well, but I'm not
; saying anything about that.

(define-syntax %file-name%
  (syntax-rules ()
    ((%file-name%) (fluid $source-file-name))))

