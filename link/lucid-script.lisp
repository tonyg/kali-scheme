
; Script to load the Scheme48 linker into Lucid Common Lisp.

(defvar pseudoscheme-directory "../pseudo/")
(load (string-append pseudoscheme-directory "loadit"))
(setq *use-scheme-read* t)
(load-pseudoscheme (string-append pseudoscheme-directory))

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

(load "defpackage.scm")
(load "flatload.scm")
(load-configuration "alt-packages.scm")
(load-configuration "comp-packages.scm")
(load-configuration "more-packages.scm")



; Make no more bootstrap structures - clobber its definition as syntax.
(define (make-structure . rest) (error "make-structure" rest))

(let ((#.'clever-load:*compile-if-necessary-p* #t))
  (let ((#.'ps:*scheme-read* #.'#'ps::scheme-read-using-commonlisp-reader))
    (load "alt/pseudoscheme-record")
    (load "alt/pseudoscheme-features"))
  ;; Inhibit FLATLOAD from loading the vanilla versions of the packages that
  ;; we have just made available.
  (inhibit-usual-structures!)
  (flatload-package linker-etc))

(define-syntax struct-list    ;not in config.sbin
  (syntax-rules ()
    ((struct-list name ...) (list (cons 'name name) ...))))
(set-kludge-eval-for-syntax! (lambda () eval-for-syntax))
(reset-packages-state!)
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
