; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define-structure low-structures low-structures-interface
  ;; Flatloaded
  (open ))

(define ascii (structure-ref built-in-structures ascii))
(define signals (structure-ref built-in-structures signals))
(define loopholes (structure-ref built-in-structures loopholes))
(define escapes (structure-ref built-in-structures escapes))
(define vm-exposure (structure-ref built-in-structures vm-exposure))

; (define-structure locations locations-interface
;   (open scheme-level-2 ...))
(define locations (structure-ref built-in-structures locations))

(define closures (structure-ref built-in-structures closures))
(define bitwise (structure-ref built-in-structures bitwise))

;; For initial system
(define write-images (structure-ref built-in-structures write-images))
(define structure-refs (structure-ref built-in-structures structure-refs))
(define low-level (structure-ref built-in-structures low-level))

;; For compiler
(define features (structure-ref built-in-structures features))
(define code-vectors (structure-ref built-in-structures code-vectors))
(define source-file-names
  (structure-ref built-in-structures source-file-names))

(define true-scheme (structure-ref built-in-structures scheme))


(define-structure scheme-level-0 scheme-level-0-interface
  (open true-scheme
	primitives		; only for extended-number?
	structure-refs)
  (access true-scheme)
  (files level-0))

(define-structure silly (export reverse-list->string)
  (open true-scheme)
  (begin (define (reverse-list->string l n)
	   (list->string (reverse l)))))

(define-structure cont-primitives
    (export make-continuation
	    continuation-length
	    continuation-ref
	    continuation-set!
	    continuation?)
  (open (structure-ref built-in-structures primitives)))

(define-structures ((primitives primitives-interface)
		    (primitives-internal (export maybe-handle-interrupt
						 raise-exception
						 get-exception-handler
						 ?start)))
  (open true-scheme
        cont-primitives
        (structure-ref built-in-structures bitwise)
        (structure-ref built-in-structures records)
        (structure-ref built-in-structures signals)
        (structure-ref built-in-structures features)
        (structure-ref built-in-structures templates)
	)
  (files ("../alt" primitives)
	 ("../alt" weak)))

; How about signals?

