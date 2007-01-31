; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Say <srcdir> is the Scheme 48 source directory.
; cd <srcdir>, then start DrScheme like so:
; env PLTCOLLECTS=:<srcdir>/scheme drscheme
; in the source directory, select the "MzScheme" language,
; copy this into an empty buffer, and press run.

; If you want to run within MzScheme, you have to
; (require (lib "errortrace.ss" "errortrace"))
; (Mike doesn't know why.)

(require "build/plt-link.ss")

(eval
 '(begin
    (load-configuration "scheme/interfaces.scm")
    (load-configuration "scheme/vm/shared-interfaces.scm")
    (load-configuration "scheme/packages.scm")
    (flatload initial-structures)
    (load "build/initial.scm")
    (link-initial-system))
 (module->namespace "build/plt-link.ss"))
