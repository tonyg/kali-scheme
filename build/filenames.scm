; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Generate filenames.make from *-packages.scm.

; Define DEFINE-STRUCTURE and friends
(for-each load
	  '("scheme/bcomp/module-language.scm"
	    "scheme/alt/dummy-interface.scm"
	    "scheme/alt/config.scm"
	    "scheme/env/flatload.scm"))

; The following bogus structures are required in order to load
; scheme/more-interfaces.scm.
(define ascii      (structure (make-simple-interface 'ascii      '())))
(define bitwise    (structure (make-simple-interface 'bitwise    '())))
(define vm-data    (structure (make-simple-interface 'vm-data    '())))
(define enumerated (structure (make-simple-interface 'enumerated '())))
(define tables     (structure (make-simple-interface 'tables     '())))
(define cells      (structure (make-simple-interface 'cells      '())))

; The following loads are unnecessary; they only serve to suppress
; annoying "undefined" warnings for interfaces.
(for-each load
	  '("scheme/interfaces.scm"
	    "scheme/vm/shared-interfaces.scm"
	    "scheme/more-interfaces.scm"
	    "scheme/sort/interfaces.scm"))

(load-configuration "scheme/packages.scm")

; The following defines are unnecessary; they only serve to suppress
; annoying "undefined" warnings for some forward references.
(define methods 0) 
(define tables 0) 

(flatload linker-structures)

(define q-f (all-file-names link-config)) 

; (display "Initial structures") (newline)
(flatload initial-structures)

(define scheme (make-scheme environments evaluation))

(define initial-system
  (structure (export)
    (open ;; Cf. initial.scm
	  (make-initial-system scheme (make-mini-command scheme))
	  module-system
	  ensures-loaded
	  for-reification))) ;foo...

(define i-f (all-file-names initial-system))

; (display "Usual structures") (newline)
(flatload usual-structures)

(define u-f (all-file-names usual-features initial-system))

(define (write-file-names mumble comment . stuff)
  (comment "#### This file was generated automatically. ####")
  (do ((stuff stuff (cddr stuff)))
      ((null? stuff))
    (mumble (car stuff) (cadr stuff))
    ;; (mumble 'all-files (reverse *all-files*))
    ))

;; Unix

(begin
  (display "Writing ") (display "build/filenames.make") (newline)
  (call-with-output-file "build/filenames.make"
    (lambda (port)
      (write-file-names (lambda (name filenames)
			  (newline port)
			  (display name port)
			  (display " = " port)
			  (for-each (lambda (filename)
				      (display filename port)
				      (display " " port))
				    filenames)
			  (newline port))
			(lambda (comment)
			  (display "#" port)
			  (display comment port)
			  (newline port))
			'initial-files i-f
			'usual-files u-f
			'linker-files q-f))))

;; Windows

(begin
  (display "Writing ") (display "build/filenames.bat") (newline)
  (call-with-output-file "build/filenames.bat"
    (lambda (port)
      (write-file-names (lambda (name filenames)
			  (newline port)
			  (display "@set " port)
			  (display name port)
			  (display "=" port)
			  (for-each (lambda (filename)
				      (display filename port)
				      (display " " port))
				    filenames)
			  (newline port))
			(lambda (comment)
			  (display "@rem " port)
			  (display comment port)
			  (newline port))
			'initial-files i-f
			'usual-files u-f
			'linker-files q-f))))

