
; Script to load the Scheme48 linker into Lucid Common Lisp.
; Not tested recently.

(defvar pseudoscheme-directory "../pseudo/")
(load (string-append pseudoscheme-directory "loadit"))
(setq *use-scheme-read* t)
(load-pseudoscheme pseudoscheme-directory)

(progn (revised^4-scheme::define-sharp-macro #\.
	 #'(lambda (c port)
	     (read-char port)
	     (eval (let ((*readtable* ps::scheme-readtable))
		     (read port)))))
       (values))

(defun enable-lisp-packages ()
  (setq *scheme-read* ps:scheme-readtable)
  (values))

(defun disable-lisp-packages ()
  (setq *readtable* ps::roadblock-readtable)
  (values))

(ps:scheme)

(benchmark-mode)

(load "alt/config.scm")
(load "alt/flatload.scm")
(load "bcomp/defpackage.scm")
(load-configuration "packages.scm")

(flatload linker-structures)

; Make no more bootstrap structures - clobber its definition as syntax.
(define (make-structure . rest) (error "make-structure" rest))

(let ((#.'clever-load:*compile-if-necessary-p* #t))
  (let ((#.'ps:*scheme-read* #.'#'ps::scheme-read-using-commonlisp-reader))
    (load "alt/pseudoscheme-record")
    (load "alt/pseudoscheme-features"))
  (flatload link-config))

(load "alt/init-defpackage.scm")
(set! *package-uid* 0)

(define-syntax struct-list    ;not in link.sbin
  (syntax-rules ()
    ((struct-list name ...) (list (cons 'name name) ...))))

(quit)

(defun disksave-restart-function ()
  (format t "~&Scheme48 linker.~2%")
  ;; (hax:init-interrupt-delivery)
  (ps:scheme)
  (terpri))
(defun dump-linker ()
  (lcl:disksave "link/linker-in-lucid" :gc t :full-gc t :verbose t
		:restart-function #'disksave-restart-function))
;(dump-linker)
;(lcl:quit)
