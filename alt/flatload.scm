; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Stub support for DEFINE-PACKAGE and DEFINE-SIGNATURE macros.

; This only supports two operations on packages:
;   package-all-filenames
;   flatload-package	     - load relevant files.
; Signatures are ignored.  Only package dependencies are significant.


(define (load-configuration filename . rest)
  (let ((save filename))
    (dynamic-wind (lambda () (set! *config-file-name* filename))
		  (lambda ()
		    (apply load filename rest))
		  (lambda () (set! *config-file-name* save)))))
(define (%file-name%) *config-file-name*)
(define *config-file-name* "")

(define (make-simple-signature items)
  '<a-signature>)

(define (make-compound-signature . sigs)
  '<a-signature>)


; Structures are views into packages.
; In this implementation, signature information is completely ignored.

(define-syntax make-structure
  (syntax-rules ()
    ((make-structure name signature package)
     (vector '<structure> name package))))

(define (structure-name s) (vector-ref s 1))
(define (structure-package s) (vector-ref s 2))

(define (verify-later! thunk) 'lose)
;(define *all-files* '())


; Packages are not what they appear to be.

(define (make-a-package opens-thunk accesses-thunk file-name clauses)
  (vector '<a-package>  opens-thunk accesses-thunk file-name clauses))

(define (package-opens p)     ((vector-ref p 1)))
(define (package-accesses p)  ((vector-ref p 2)))
(define (package-file-name p) (vector-ref p 3))
(define (package-clauses p)   (vector-ref p 4))

(define (package-directory p)
  (file-name-directory (package-file-name p)))

(define dummy-package
  (make-a-package (lambda () '()) (lambda () '()) "" '()))

(define (make-compiler-base)
  (make-structure 'compiler-base #f dummy-package))

(define *inhibited-structures* '())

(define (inhibit-structures! names)
  (set! *inhibited-structures*
	(append names *inhibited-structures*)))

(define (inhibit-usual-structures!)
  (inhibit-structures! '(scheme-level-1
			 scheme-level-2
			 scheme
			 signals handle features  ; too featureless?
			 record		; too undebuggable
			 ascii		; too slow
			 bitwise	; too slow
			 code-vectors	; too big
			 )))

; It's called walk-package, but its argument is actually a structure.

(define (walk-package s action)
  (let ((seen '()))
    (let recur ((s s))
      (let ((p (structure-package s)))
	(if (and (not (memq p seen))
		 (not (memq (structure-name s) *inhibited-structures*)))
	    (begin
	      (set! seen (cons p seen))
	      ;; (write (structure-name s)) (display " ")
	      (action p
		      (structure-name s)
		      (lambda ()
			(for-each recur (package-opens p))
			(for-each (lambda (name+struct)
				    (recur (cdr name+struct)))
				  (package-accesses p)))
		      (lambda (file-action)
			(for-each (lambda (clause)
				    (case (car clause)
				      ((files)
				       (for-each file-action
						 (cdr clause)))))
				  (package-clauses p))))))))))

; Load a package's files into a single-toplevel-environment Scheme.
; Arguments past the first are simply passed along to LOAD.

(define (flatload-package s . rest)
  (for-each (lambda (filename)
	      (apply load filename rest))
	    (package-all-filenames s #t))
  (newline))

		    
; This is used to generate file lists that are "included" in "makefiles."

(define (write-package-filenames target . stuff)
  (call-with-output-file target
    (lambda (port)
      (display "Writing ") (display target) (newline)
      (display "#### This file was generated automatically. ####"
	       port)
      (newline port)
      (let ((mumble (lambda (name filenames)
		      (newline port)
		      (display name port)
		      (display " = " port)
		      (for-each (lambda (filename)
				  (display filename port)
				  (display " " port))
				filenames)
		      (newline port))))
	(do ((stuff stuff (cddr stuff)))
	    ((null? stuff))
	  (mumble (car stuff) (cadr stuff)))
	;(mumble 'all-files (reverse *all-files*))
	))))

(define (package-all-filenames struct . complain-option)
  (let ((l '())
	(funny-complain? (if (null? complain-option)
			     #f
			     (car complain-option))))
    (walk-package struct
		  (lambda (p name action walk-files)
		    (action)
		    (let ((dir (package-directory p)))
		      (walk-files
		       (lambda (file)
			 (set! l (cons (namestring file dir *load-file-type*)
				       l)))))))
    (reverse l)))

; Handy

(define (setdiff l1 l2)
  (cond ((null? l2) l1)
	((null? l1) l1)
	((member (car l1) l2)
	 (setdiff (cdr l1) l2))
	(else (cons (car l1)
		    (setdiff (cdr l1) l2)))))


; Stuff copied from rts/filename.scm... ugh...

; Namelist = ((dir ...) basename type)
;         or ((dir ...) basename)
;	  or (dir basename type)
;	  or (dir basename)
;	  or basename

(define (namestring namelist dir default-type)
  (let ((namelist (if (list? namelist) namelist (list '() namelist))))
    (let ((subdirs (if (list? (car namelist))
		       (car namelist)
		       (list (car namelist))))
	  (basename (cadr namelist))
	  (type (if (null? (cddr namelist))
		    default-type
		    (caddr namelist))))
      (string-append (or dir "")
		     (apply string-append
			    (map (lambda (subdir)
				   (string-append
				      (namestring-component subdir)
				      directory-component-separator))
				 subdirs))
		     (namestring-component basename)
		     (if type
			 (string-append type-component-separator
					(namestring-component type))
			 "")))))

(define directory-component-separator "/") ;unix sux
(define type-component-separator ".")

(define (namestring-component x)
  (cond ((string? x) x)
	((symbol? x)
	 (list->string (map file-name-preferred-case
			    (string->list (symbol->string x)))))
	(else (error "bogus namelist component" x))))

(define file-name-preferred-case char-downcase)

(define *scheme-file-type* 'scm)
(define *load-file-type* *scheme-file-type*)  ;#F for Pseudoscheme or T

(define (file-name-directory filename)
  (substring filename 0 (file-nondirectory-position filename)))

(define (file-name-nondirectory filename)
  (substring filename
	     (file-nondirectory-position filename)
	     (string-length filename)))

(define (file-nondirectory-position filename)
  (let loop ((i (- (string-length filename) 1)))
    (cond ((< i 0) 0)
	  ;; Heuristic.  Should work for DOS, Unix, VMS, MacOS.
          ((string-posq (string-ref filename i) "/:>]\\") (+ i 1))
          (else (loop (- i 1))))))

(define (string-posq thing s)
  (let loop ((i 0))
    (cond ((>= i (string-length s)) #f)
          ((eq? thing (string-ref s i)) i)
          (else (loop (+ i 1))))))
