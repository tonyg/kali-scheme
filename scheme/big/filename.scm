; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Silly file name utilities
; These try to be operating-system independent, but fail, of course.

; Namelist = ((dir ...) basename type)
;         or ((dir ...) basename)
;	  or (dir basename type)
;	  or (dir basename)
;	  or basename

(define (namestring namelist dir default-type)
  (let* ((namelist (if (list? namelist) namelist (list '() namelist)))
	 (subdirs (if (list? (car namelist))
		      (car namelist)
		      (list (car namelist))))
	 (basename (cadr namelist))
	 (type (if (null? (cddr namelist))
		   (if (string? basename)
		       #f
		       default-type)
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
		       ""))))

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



; Interface copied from gnu emacs:

;file-name-directory           
;  Function: Return the directory component in file name NAME.
;file-name-nondirectory        
;  Function: Return file name NAME sans its directory.
;file-name-absolute-p          
;  Function: Return t if file FILENAME specifies an absolute path name.
;substitute-in-file-name       
;  Function: Substitute environment variables referred to in STRING.
;expand-file-name              
;  Function: Convert FILENAME to absolute, and canonicalize it.

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



; Directory translations.
; E.g. (set-translation! "foo;" "/usr/mumble/foo/")

(define *translations* '())

(define (translations) *translations*)

(define (set-translation! from to)
  (let ((probe (assoc from *translations*)))
    (if probe
	(set-cdr! probe to)
	(set! *translations* (cons (cons from to) *translations*)))))

(define (translate name)
  (let ((len (string-length name)))
    (let loop ((ts *translations*))
      (if (null? ts)
	  name
	  (let* ((from (caar ts))
		 (to (cdar ts))
		 (k (string-length from)))
	    (if (and to
		     (<= k len)
		     (string=? (substring name 0 k) from))
		(string-append to (substring name k len))
		(loop (cdr ts))))))))
