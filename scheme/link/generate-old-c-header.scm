; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This for compatibility with pre-0.53 code.

; [This is a kludge.  Richard is loathe to include it in the
; distribution.]

; Reads arch.scm and data.scm and writes out a C .h file with constants
; and macros for dealing with Scheme 48 data structures.

; Needs Big Scheme.

; (make-c-header-file "scheme48.h" "vm/arch.scm" "vm/data.scm")

(define (make-c-header-file c-file arch-file data-file)
  (receive (stob-list stob-data)
      (search-file arch-file
		   '("stob enumeration" "(define stob-data ...)")
		   (defines-enum? 'stob)
		   enum-definition-list
		   (lambda (x)
		     (and (eq? (car x) 'define)
			  (eq? (cadr x) 'stob-data)))
		   (lambda (x) (cadr (caddr x))))
    (receive (tag-list immediate-list)
	(search-file data-file
		     '("tag enumeration" "imm enumeration")
		     (defines-enum? 'tag)
		     enum-definition-list
		     (defines-enum? 'imm)
		     enum-definition-list)
      (with-output-to-file c-file
	(lambda ()
	  (format #t "typedef long scheme_value;~%~%")
	  (tag-stuff tag-list)
	  (newline)
	  (immediate-stuff immediate-list)
	  (newline)
	  (stob-stuff stob-list stob-data))))))
	  
(define (tag-stuff tag-list)
  (do ((tags tag-list (cdr tags))
       (i 0 (+ i 1)))
      ((null? tags))
    (let ((name (upcase (car tags))))
      (c-define "~A_TAG ~D" name i)
      (c-define "~AP(x) (((long)(x) & 3L) == ~A_TAG)" name name)))
  (newline)
  (c-define "ENTER_FIXNUM(n)   ((scheme_value)((n) << 2))")
  (c-define "EXTRACT_FIXNUM(x) ((long)(x) >> 2)"))

(define (immediate-stuff imm-list)
  (c-define "MISC_IMMEDIATE(n) (scheme_value)(IMMEDIATE_TAG | ((n) << 2))")
  (do ((imm imm-list (cdr imm))
       (i 0 (+ i 1)))
      ((null? imm))
    (let ((name (upcase (car imm))))
      (c-define "SCH~A    MISC_IMMEDIATE(~D)" name i)))
  (c-define "UNDEFINED SCHUNDEFINED")
  (c-define "UNSPECIFIC SCHUNSPECIFIC")
  (newline)
  (c-define "ENTER_BOOLEAN(n) ((n) ? SCHTRUE : SCHFALSE)")
  (c-define "EXTRACT_BOOLEAN(x) ((x) != SCHFALSE)")
  (newline)
  (c-define "ENTER_CHAR(c) (SCHCHAR | ((c) << 8))")
  (c-define "EXTRACT_CHAR(x) ((x) >> 8)")
  (c-define "CHARP(x) ((((long) (x)) & 0xff) == SCHCHAR)"))

(define (stob-stuff stob-list stob-data)
  (let ((type-mask (let ((len (length stob-list)))
		     (do ((i 2 (* i 2)))
			 ((>= i len) (- i 1))))))
    (c-define "ADDRESS_AFTER_HEADER(x, type) ((type *)((x) - STOB_TAG))")
    (c-define "STOB_REF(x, i) ((ADDRESS_AFTER_HEADER(x, long))[i])")
    (c-define "STOB_TYPE(x)   ((STOB_HEADER(x)>>2)&~D)" type-mask)
    (c-define "STOB_HEADER(x) (STOB_REF((x),-1))")
    (c-define "STOB_BLENGTH(x) (STOB_HEADER(x) >> 8)")
    (c-define "STOB_LLENGTH(x) (STOB_HEADER(x) >> 10)")
    (newline)
    (do ((stob stob-list (cdr stob))
	 (i 0 (+ i 1)))
	((null? stob))
      (let ((name (upcase (if (eq? (car stob) 'byte-vector)
			      'code-vector
			      (car stob)))))
	(c-define "STOBTYPE_~A ~D" name i)
	(c-define "~AP(x) (STOBP(x) && (STOB_TYPE(x) == STOBTYPE_~A))"
		  name name)))
    (newline)
    (for-each (lambda (data)
		(do ((accs (cdddr data) (cdr accs))
		     (i 0 (+ i 1)))
		    ((null? accs))
		  (let ((name (upcase (caar accs))))
		    (c-define "~A(x) STOB_REF(x, ~D)" name i))))
	      stob-data)
    (newline)
    (c-define "VECTOR_LENGTH(x) STOB_LLENGTH(x)")
    (c-define "VECTOR_REF(x, i) STOB_REF(x, i)")
    (c-define "CODE_VECTOR_LENGTH(x)  STOB_BLENGTH(x)")
    (c-define "CODE_VECTOR_REF(x, i)  (ADDRESS_AFTER_HEADER(x, unsigned char)[i])")
    (c-define "STRING_LENGTH(x)  (STOB_BLENGTH(x)-1)")
    (c-define "STRING_REF(x, i)  (ADDRESS_AFTER_HEADER(x, char)[i])")))

; - becomes _ > becomes TO_ (so -> turns into _TO_)
; ? becomes P

(define (upcase symbol)
  (do ((chars (string->list (symbol->string symbol)) (cdr chars))
       (res '() (case (car chars)
		  ((#\>) (append (string->list "_OT") res))
		  ((#\-) (cons #\_ res))
		  ((#\?) (cons #\P res))
		  (else (cons (char-upcase (car chars)) res)))))
      ((null? chars)
       (list->string (reverse res)))))

(define (c-define string . stuff)
  (format #t "#define ~?~%" string stuff))
    
(define (defines-enum? name)
  (lambda (x)
    (and (eq? (car x) 'define-enumeration)
	 (eq? (cadr x) name))))

(define enum-definition-list caddr)

; STUFF is list of ((predicate . extract) . name).  <name> is replaced
; with the value when it is found.

(define (search-file file what-for . pred+extract)
  (let ((stuff (do ((p+e pred+extract (cddr p+e))
		    (names what-for (cdr names))
		    (ps '() (cons (cons (cons (car p+e) (cadr p+e))
					(car names))
				  ps)))
		   ((null? p+e) (reverse ps)))))

    (define (search next not-found)
      (let loop ((n-f not-found) (checked '()))
	(cond ((null? n-f)
	       #f)
	      (((caaar n-f) next)
	       (set-cdr! (car n-f) ((cdaar n-f) next))
	       (append (reverse checked) (cdr n-f)))
	      (else
	       (loop (cdr n-f) (cons (car n-f) checked))))))

    (with-input-from-file file
      (lambda ()
	(let loop ((not-found stuff))
	  (let ((next (read)))
	    (cond ((null? not-found)
		   (apply values (map cdr stuff)))
		  ((eof-object? next)
		   (error "file ~S doesn't have ~A" file (cdar not-found)))
		  (else
		   (loop (or (and (pair? next)
				  (search next not-found))
			     not-found))))))))))
