
; Generate filenames.make from *-packages.scm.


; Define DEFINE-STRUCTURE and friends
(for-each load
	  '("bcomp/module-language.scm"
	    "alt/config.scm"
	    "alt/flatload.scm"))

(load-configuration "packages.scm")  

; The following defines are unnecessary; theyonly serve to suppress
; annoying "undefined" warnings for some forward references.
(define syntactic 0) 
(define tables 0) 

(flatload linker-structures)		 

(define q-f (all-file-names link-config)) 

(flatload usual-structures)		 
(define i-f (all-file-names initial-system)) 
(define u-f (all-file-names usual-features initial-system))

(write-file-names "filenames.make"      
		  'initial-files i-f
		  'usual-files u-f
		  'linker-files q-f)
