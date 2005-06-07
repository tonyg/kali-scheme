
; Script to load the Scheme 48 linker into Common Lisp.
; Requires Pseudoscheme 2.11.

(defvar pseudoscheme-directory "../pseudo/")
(load (concatenate 'string pseudoscheme-directory "loadit.lisp"))
; or perhaps (load (merge-pathnames "loadit.lisp" pseudoscheme-directory))
(load-pseudoscheme pseudoscheme-directory)

(progn (revised^4-scheme::define-sharp-macro #\.
	 #'(lambda (c port)
	     (read-char port)
	     (eval (let ((*readtable* ps::scheme-readtable))
		     (read port)))))
       (values))

(ps:scheme)
;--------------------
; Scheme forms

(benchmark-mode)

(define config-env     ; (interaction-environment) would also work here.
  (#.'scheme-translator:make-program-env
     '%config
     (list #.'scheme-translator:revised^4-scheme-structure)))

(load "bcomp/module-language" config-env)
(load "alt/config" config-env)
(load "env/flatload" config-env)
(eval '(set! *load-file-type* #f) config-env)

(define load-config
  (let ((load-config (eval 'load-configuration config-env)))
    (lambda (filename)
      (load-config filename config-env))))

(load-config "packages")

(define flatload-package (eval 'flatload config-env))

(flatload-package (eval 'linker-structures config-env) config-env)

(let ((#.'clever-load:*compile-if-necessary-p* #t))
  (let ((#.'ps:*scheme-read* #.'#'ps::scheme-read-using-commonlisp-reader))
    (load "alt/pseudoscheme-record")
    (load "alt/pseudoscheme-features")))

(let ((#.'clever-load:*compile-if-necessary-p* #t))
  (flatload-package (eval 'link-config config-env)))

(load "alt/init-defpackage.scm")

(define-syntax struct-list    ;not in link.sbin
  (syntax-rules ()
    ((struct-list ?name ...) (list (cons '?name ?name) ...))))

;--------------------
(quit)

#+Lucid
(defun disksave-restart-function ()
  (format t "~&Scheme 48 linker.~2%")
  ;; (hax:init-interrupt-delivery)   - for threads
  (ps:scheme)
  (terpri))
#+Lucid
(defun dump-linker ()
  (lcl:disksave "link/linker-in-lucid" :gc t :full-gc t :verbose t
		:restart-function #'disksave-restart-function))
;(dump-linker)
;(lcl:quit)


; Debugging hacks
;(defun enable-lisp-packages ()
;  (setq *readtable* ps:scheme-readtable)
;  (values))
;(defun disable-lisp-packages ()
;  (setq *readtable* ps::roadblock-readtable)
;  (values))

