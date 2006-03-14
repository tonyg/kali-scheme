; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; To use PSD with Scheme 48:
;  - Put (setq psd-using-slib nil) in your .emacs
;  - Put this file in psd-directory
;  - Remove (define *psd-tab-char* (integer->char 9)) from psd's read.scm
;  - Do something horrible to primitives.scm to circumvent bug in
;    Scheme 48's byte code compiler... those big long lists have to be
;    split up: `((x ,x) ... (y ,y) ...) =>
;     (let ((foo (lambda () `((x ,x) ...)))
;	    (bar (lambda () `((y ,y) ...))))
;	(append (foo) (bar)))

; JAR's remarks:
;   - The variable *PSD-PREVIOUS-LINE* was undefined (not consequentially so)
;   - It doesn't support DELAY
;   - It doesn't support the => syntax in COND
;   - It doesn't like (write-char c port):
;     ERROR: Wrong number of arguments to primitive procedure
;     write-char
;   - It would be awfully nice if there were a "quit" command
;   - It leaves that little "=>" arrow in my buffer (except it's not
;     really there, is it?)
;   - It opens the source file multiple (maybe 6) times without
;     closing it (not really a problem since the GC cleans these up, but
;     sort of annoying)

;;;;
;;;; $Id: psd-s48.scm,v 1.1 1994/07/13 04:28:45 bdc Exp $
;;;;
;;;; psd -- a portable Scheme debugger, version 1.1
;;;; Copyright (C) 1992 Pertti Kellomaki, pk@cs.tut.fi

;;;; This program is free software; you can redistribute it and/or modify
;;;; it under the terms of the GNU General Public License as published by
;;;; the Free Software Foundation; either version 1, or (at your option)
;;;; any later version.

;;;; This program is distributed in the hope that it will be useful,
;;;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;;;; GNU General Public License for more details.

;;;; You should have received a copy of the GNU General Public License
;;;; along with this program; if not, write to the Free Software
;;;; Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;;;; See file COPYING in the psd distribution.
;;;;
;;;;  $Log: psd.scm,v $
;;;; Revision 1.6  1993/10/07  08:20:14  pk
;;;; Define force-output, redefinition hurts less than getting an error
;;;; message about nonexistent procedure.
;;;;
;;;; Revision 1.5  1993/10/06  13:06:16  pk
;;;; Removed references to slib. Commented out force-output, let the user
;;;; uncomment it if needed.
;;;;
;;;; Revision 1.4  1993/09/29  08:45:11  pk
;;;; Removed reference to long and deep lists.
;;;;
;;;; Revision 1.3  1993/09/24  08:01:18  pk
;;;; Changed version number from 1.0 to 1.1.
;;;; Added loading of version.scm and announcing of version.
;;;;
;;;; Revision 1.2  1993/09/23  06:50:00  pk
;;;; Moved definition of the Scheme variable psd-directory from the psd*.scm
;;;; files to psd.el, which sends it to the Scheme process. This way, the path
;;;; to psd needs to be specified only once.
;;;;
;;;; Revision 1.1  1993/09/22  12:45:32  pk
;;;; Initial revision
;;;;
;;;;  

;;;; 
;;;; Written by Pertti Kellomaki, pk@cs.tut.fi
;;;;
;;;; SLIB interface to load psd files.

;;;; This is the file that takes care of loading psd into the Scheme
;;;; interpreter. If you want to modify psd to work with a particular
;;;; implementation, say "foo", this is the way to do it:

;;;; 1) Make a copy of the file "psd.scm" under the name "psd-foo.scm",
;;;;    and modify it to load "primitives-foo.scm" instead of
;;;;    "primitives.scm". You can also do other things.
;;;;
;;;; 2) Make a copy of the file "primitives.scm" under the name
;;;;    "primitives-foo.scm", and modify the definitions in it to
;;;;    know about the additional primitives in your implementation
;;;;
;;;; 3) When you now set the Emacs variable scheme-program-name to
;;;;    "foo" and give the commands ``M-x run-scheme'' ``M-x psd-mode'',
;;;;    you have a psd system that knows about your additional
;;;;    primitives.

;;; this is not portable.
(define psd:control-z ((access-scheme-48 'ascii->char) 26))

; Neither is this.
(define *psd-tab-char* ((access-scheme-48 'ascii->char) 9))

;;; FORCE-OUTPUT flushes any pending output on optional arg output port
;;; use this definition if your system doesn't have such a procedure.
(define (force-output . arg)
  ((access-scheme-48 'force-output)
   (if (null? arg) (current-output-port) (car arg))))

(load (string-append psd-directory "qp.scm"))
(load (string-append psd-directory "version.scm"))
(load (string-append psd-directory "instrum.scm"))
(load (string-append psd-directory "pexpr.scm"))
(load (string-append psd-directory "read.scm"))
(load (string-append psd-directory "runtime.scm"))
(load (string-append psd-directory "primitives.scm"))

(define *psd-previous-line* #f)

;;;
;;; Say hello
;;;

(psd-announce-version)

(define error (access-scheme-48 'error))
