; -*- Mode: Scheme; -*-
; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

;  ,exec ,load misc/load-static.scm
;  (do-it 100000 "debug/little.image" "debug/little-heap.c")

(translate "=scheme48/" "./")

(config
 (lambda ()
   (load "vm/ps-interface.scm")
   (load "vm/interfaces.scm")
   (load "vm/package-defs.scm" "vm/s48-package-defs.scm")))

(load-package 'bigbit)
; The following is for struct's (for-syntax ...) clause
; (load-package 'destructuring)

(load-package 'heap)
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
		      enumerated
		      signals)
		(files (misc static)))))

(load-package 'static)
(user '(open static))

