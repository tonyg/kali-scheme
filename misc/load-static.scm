; -*- Mode: Scheme; -*-
;  ,exec ,load misc/load-static.scm

(config
 (lambda ()
   (load "vm/ps-interface.scm")
   (load "vm/interfaces.scm")
   (load "vm/package-defs.scm" "vm/s48-package-defs.scm")))

(load-package 'bigbit)
; The following is for struct's (for-syntax ...) clause
(load-package 'big-scheme)

(in 'heap
    (lambda ()
      (run '(define (newspace-begin) *newspace-begin*))
      (run '(define (heap-pointer) *hp*))
      (structure 'heap-extra
		 '(export newspace-begin
			  heap-pointer
			  header-a-units
			  d-vector?
			  stob-type))))

(config '(run (define-structure static (export do-it)
		(open scheme heap memory data stob struct
		      heap-extra
		      vm-architecture
		      formats
		      ;; enumerated
		      signals)
		(files (misc static)))))

(load-package 'static)
