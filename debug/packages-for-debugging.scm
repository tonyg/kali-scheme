; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Suppose you have just done "make image" to build the scheme48 heap image.
; Suppose that the linker produces an initial.image, but that when that
; image is resumed you get the error
;     exception handler is not a closure
; This is not informative.  To find out what really happened, you have
; two choices:
;   (1) Run the image under the VM running in Scheme.
;   (2) Run the image using the following handy dandy tool.
; For choice (2), you would do something like the following:
"
 cd ~/s48
 x48 -h 4000000
 ,translate =scheme48/ ./
 ,config
 ,load interfaces.scm packages.scm debug/packages-for-debugging.scm
 ,open flatloading
 (flatload initial-structures)
 ,in debug-config 
 y
 ,config make-compiler-base
 (define make-compiler-base ##)

 ,load initial.scm    ;Or whatever, to load interfaces.scm and the rest.

 (define go (link-initial-system))
 (?start go #f)
"

; Things provided by the byte compiler / VM, together with a few
; things with rather sensitive definitions (low.scm).

(define-structure low-structures low-structures-interface
  (open module-system the-interfaces)
  (begin  ))


; After loading packages.scm, clobber the definition of low-level-packages
; with ... what ...?
;
;  (define-all-operators)		; Primitive Scheme, in the LSC paper
;
; with
;
;  (files (debug for-debugging))
;  



(define-structure cont-primitives
    (export make-continuation
	    continuation-length
	    continuation-ref
	    continuation-set!
	    continuation?)
  (open (structure-ref built-in-structures primitives)))


; cf. definition in alt-packages.scm

(define-structures ((primitives primitives-interface)
		    (primitives-internal (export maybe-handle-interrupt
						 raise-exception
						 get-exception-handler
						 ?start)))
  (open scheme-level-2
	bitwise record
	features
	signals
	cont-primitives
	template)
  (files ("../alt" primitives)
	 ("../alt" weak)))


(define-interface all-primitives-interface ;Cf. rts-packages.scm
  (interface-of ((structure-ref defpackage make-compiler-base))))




(define-structure all-primitives all-primitives-interface
  (open scheme
	signals			;error
	structure-refs
	primitives
	locations escapes features code-vectors template ascii bitwise
	closures vm-exposure)
  (access scheme
	  signals		;error
	  locations)
  (files ("../alt" all-prims)))


; Some system startup stuff, exception handling hooks, etc.

(define-structure start-debugging
		  (export ?start
			  ?start-with-exceptions
			  in
			  link-simple-system
			  link-reified-system
			  struct-list)
  (open scheme
	primitives-internal
	exception
	handle			;with-handler
	condition		;exception?
	signals			;error
	packages
	reification
	ensures-loaded
	command			;y-or-n?
	structure-refs
	primitives		; extended-number?
	architecture)		; (enum interrupt keyboard)
  (access scheme
	  signals		;error
	  locations)
  (files for-debugging))



; Configuration package into which you can load rts-packages.scm or
; whatever.

(define-structure debug-config (export )
  (open defpackage start-debugging ensures-loaded syntactic fluids
	scheme)
  (begin (define (load-configuration filename) ;copied from link/link.scm
	   (let-fluid $source-file-name filename
	     (lambda () (load filename))))))
