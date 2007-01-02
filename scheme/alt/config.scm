; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Stub support for DEFINE-PACKAGE and DEFINE-INTERFACE macros.

(define (load-configuration filename . rest)
  (let ((save filename))
    (dynamic-wind (lambda () (set! *source-file-name* filename))
		  (lambda ()
		    (apply load filename rest))
		  (lambda () (set! *source-file-name* save)))))
(define (%file-name%) *source-file-name*)
(define *source-file-name* "")


; --------------------

; Structures are views into packages.

(define (make-structure package int-thunk . name-option)
  (let ((struct (vector '<structure>
			#f
			package
			(if (procedure? int-thunk)
			    int-thunk
			    (lambda () int-thunk))
			#f)))
    (if (not (null? name-option))
	(note-structure-name! struct (car name-option)))
    struct))

(define (structure? thing)
  (and (vector? thing)
       (not (zero? (vector-length thing)))
       (eq? '<structure> (vector-ref thing 0))))

(define (structure-name s) (vector-ref s 1))
(define (set-structure-name! s name) (vector-set! s 1 name))
(define (structure-package s) (vector-ref s 2))
(define (structure-interface-thunk s) (vector-ref s 3))
(define (structure-interface-really s) (vector-ref s 4))
(define (set-structure-interface! s i) (vector-set! s 4 i))
(define (structure-interface s)
  (or (structure-interface-really s)
      (begin (initialize-structure! s)
	     (structure-interface-really s))))

(define (initialize-structure! s)
  (let ((int ((structure-interface-thunk s))))
    (begin (set-structure-interface! s int)
	   (note-reference-to-interface! int s))))

(define (verify-later! thunk) 'lose)
(define (set-verify-later! proc) 'lose)
;(define *all-files* '())------------


; We assume that the commands are not actually necessary.

(define (make-modified-structure struct commands)
  struct)

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

(define (initialize-package! p) 'lose)

; The package hierarchy
(define (first p l)
  (let loop ((l l))
    (and (not (null? l))
	 (or (and (p (car l)) (car l))
	     (loop (cdr l))))))

(define *structures* '())
(define (all-structures) *structures*)
(define (find-structure name)
  (first (lambda (struct)
	   (eq? name (structure-name struct)))
	 *structures*))
(define *packages* '())
(define *interfaces* '())

(define (register-structure! struct)
  (set! *structures* (cons struct *structures*)))
(define (register-interface! int)
  (set! *interfaces* (cons int *interfaces*)))
(define (register-package! p)
  (set! *packages* (cons p *packages*)))

(define (initialize-module-system!)
  (set! *structures* '())
  (set! *packages* '())
  (set! *interfaces* '()))

(define (note-name! thing name)
  (cond ((interface? thing)
	 (note-interface-name! thing name))
	((structure? thing)
	 (note-structure-name! thing name)))
  thing)

(define (note-structure-name! struct name)
  (if (and name (not (structure-name struct)))
      (begin
	(set-structure-name! struct name)
	(note-package-name! (structure-package struct) name)
	(register-structure! struct))))

(define (note-package-name! package name)
  (register-package! package))

(define dummy-package
  (make-a-package (lambda () '()) (lambda () '()) #f "" '() #f))
(define dummy-interface
  (make-simple-interface 'dummy-interface '())) 
	 
; source-file-names  ?
(define module-system
  (make-structure dummy-package dummy-interface 'module-system))
(define scheme
  (make-structure dummy-package dummy-interface 'scheme))
(define built-in-structures
  (make-structure dummy-package dummy-interface 'built-in-structures))

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

(define interface-of structure-interface)

(define-reflective-tower-maker list)
