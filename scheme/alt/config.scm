; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Stub support for DEFINE-PACKAGE and DEFINE-INTERFACE macros.

; Interfaces are ignored.  Only dependencies are significant.


(define (load-configuration filename . rest)
  (let ((save filename))
    (dynamic-wind (lambda () (set! *source-file-name* filename))
		  (lambda ()
		    (apply load filename rest))
		  (lambda () (set! *source-file-name* save)))))
(define (%file-name%) *source-file-name*)
(define *source-file-name* "")


; This is used to generate file lists that are "included" in "makefiles."

(define (write-file-names target . stuff)
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


; --------------------

(define (make-indirect-interface name thunk)
  (thunk))


(define (make-simple-interface name items)
  (cons 'export items))

(define (make-compound-interface name . sigs)
  (cons 'compound-interface sigs))


; Structures are views into packages.
; In this implementation, interface information is completely ignored.

(define-syntax make-structure
  (syntax-rules ()
    ((make-structure ?package ?interface ?name)
     (vector '<structure> ?name ?package))
    ((make-structure ?package ?interface)
     (make-structure ?package ?interface #f))))

(define (structure-name s) (vector-ref s 1))
(define (structure-package s) (vector-ref s 2))

(define (verify-later! thunk) 'lose)
;(define *all-files* '())


; Packages are not what they appear to be.

(define (make-a-package opens-thunk accesses-thunk tower
			file-name clauses name)
  (vector '<a-package>
	  (delay (opens-thunk))
	  (delay (accesses-thunk))
	  file-name
	  clauses
	  #f))

(define (package-opens p)     (force (vector-ref p 1)))
(define (package-accesses p)  (force (vector-ref p 2)))
(define (package-file-name p) (vector-ref p 3))
(define (package-clauses p)   (vector-ref p 4))
(define (package-loaded? p) (vector-ref p 5))
(define (set-package-loaded?! p ?) (vector-set! p 5 ?))

(define dummy-package
  (make-a-package (lambda () '()) (lambda () '()) #f "" '() #f))

; source-file-names  ?
(define module-system (make-structure dummy-package #f 'module-system))
(define scheme (make-structure dummy-package #f 'scheme))
(define built-in-structures
  (make-structure dummy-package #f 'built-in-structures))

(define (note-name! thing name)
  thing)


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
	(else
	 ;; (error "bogus namelist component" x)
	 "bogus namelist component")))

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

; Types
(define :value ':value)
(define :syntax ':syntax)
(define :structure ':structure)
(define :procedure ':procedure)
(define :number ':number)
(define :type ':type)

(define-syntax proc
  (lambda (e r c) ''proc-lossage))

(define-syntax interface-of
  (lambda (e r c) ''interface-of-lossage))


(define-reflective-tower-maker list)
