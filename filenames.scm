
; Generate filenames.make from *-packages.scm.


; Define DEFINE-STRUCTURE and friends
(for-each load
	  '("bcomp/module-language.scm"
	    "alt/config.scm"
	    "env/flatload.scm"))

(load-configuration "packages.scm")  

; The following defines are unnecessary; they only serve to suppress
; annoying "undefined" warnings for some forward references.
(define syntactic 0) 
(define tables 0) 
(define define-record-types 0)

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

(write-file-names "filenames.make"      
		  'initial-files i-f
		  'usual-files u-f
		  'linker-files q-f)
