; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; [This is a kludge.  Richard is loathe to include it in the
; distribution.  But now the system itself uses it, so we're stuck.]

; Reads arch.scm and data.scm and writes out a C .h file with constants
; and macros for dealing with Scheme 48 data structures.

; Needs Big Scheme.

; (make-c-header-file "scheme48.h" "scheme48.h.in"
;                     "vm/arch.scm" "vm/data.scm" "rts/record.scm")

(define (make-c-header-file c-file c-in-file arch-file data-file record-file)
  (receive (stob-list stob-data exception-list channel-status-list)
      (search-file arch-file
		   '("stob enumeration"
		     "(define stob-data ...)"
		     "exception enumeration"
		     "channel-status enumeration")
		   (defines-enum? 'stob)
		   enum-definition-list
		   (lambda (x)
		     (and (eq? (car x) 'define)
			  (eq? (cadr x) 'stob-data)))
		   (lambda (x) (cadr (caddr x)))
		   (defines-enum? 'exception)
		   enum-definition-list
		   (defines-enum? 'channel-status-option)
		   enum-definition-list)
    (receive (tag-list immediate-list)
	(search-file data-file
		     '("tag enumeration" "imm enumeration")
		     (defines-enum? 'tag)
		     enum-definition-list
		     (defines-enum? 'imm)
		     enum-definition-list)
      (let ((record-type-fields
	     (search-file record-file
			  '("(define record-type-fields ...")
			  (lambda (x)
			    (and (eq? (car x) 'define)
				 (eq? (cadr x) 'record-type-fields)))
			  (lambda (x) (cadr (caddr x))))))
	(with-output-to-file c-file
	  (lambda ()
	    (format #t "/* This file was generated automatically.~%")
	    (format #t "   It's probably not a good idea to change it. */~%")
	    (newline)
	    (format #t "#ifndef _H_SCHEME48~%")
	    (format #t "#define _H_SCHEME48~%")
	    (newline)
	    (copy-file c-in-file)
	    (newline)
	    (tag-stuff tag-list)
	    (newline)
	    (immediate-stuff immediate-list)
	    (newline)
	    (stob-stuff stob-list stob-data)
	    (newline)
	    (enumeration-stuff record-type-fields
			       "S48_RECORD_TYPE_~A(x) S48_RECORD_REF((x), ~D)")
	    (newline)
	    (enumeration-stuff exception-list "S48_EXCEPTION_~A ~D")
	    (newline)
	    (enumeration-stuff channel-status-list
		       "S48_CHANNEL_STATUS_~A S48_UNSAFE_ENTER_FIXNUM(~D)")
	    (newline)
	    (format #t "#endif /* _H_SCHEME48 */")
	    (newline)))))))
	  
(define (tag-stuff tag-list)
  (do ((tags tag-list (cdr tags))
       (i 0 (+ i 1)))
      ((null? tags))
    (let ((name (upcase (car tags))))
      (c-define "S48_~A_TAG ~D" name i)
      (c-define "S48_~A_P(x) (((long)(x) & 3L) == S48_~A_TAG)" name name)))
  (newline)
  (c-define "S48_UNSAFE_ENTER_FIXNUM(n)   ((s48_value)((n) << 2))")
  (c-define "S48_UNSAFE_EXTRACT_FIXNUM(x) ((long)(x) >> 2)"))

(define (immediate-stuff imm-list)
  (c-define "S48_MISC_IMMEDIATE(n) ((s48_value)(S48_IMMEDIATE_TAG | ((n) << 2)))")
  (do ((imm imm-list (cdr imm))
       (i 0 (+ i 1)))
      ((null? imm))
    (let ((name (upcase (car imm))))
      (c-define "S48_~A    (S48_MISC_IMMEDIATE(~D))" name i)))
  (newline)
  (c-define "S48_UNSAFE_ENTER_CHAR(c) (S48_CHAR | ((c) << 8))")
  (c-define "S48_UNSAFE_EXTRACT_CHAR(x) ((unsigned char)((x) >> 8))")
  (c-define "S48_UNSAFE_ENTER_SCALAR_VALUE(s) (S48_CHAR | ((s) << 8))")
  (c-define "S48_UNSAFE_EXTRACT_SCALAR_VALUE(x) ((x) >> 8)")
  (c-define "S48_CHAR_P(x) ((((long) (x)) & 0xff) == S48_CHAR)"))

(define (stob-stuff stob-list stob-data)
  (let ((type-mask (let ((len (length stob-list)))
		     (do ((i 2 (* i 2)))
			 ((>= i len) (- i 1))))))
    (c-define "S48_ADDRESS_AFTER_HEADER(x, type) ((type *)((x) - S48_STOB_TAG))")
    (c-define "S48_STOB_REF(x, i) (S48_ADDRESS_AFTER_HEADER(x, s48_value)[i])")
    (c-define (string-append
	       "S48_STOB_BYTE_REF(x, i) "
	       "(((char *)S48_ADDRESS_AFTER_HEADER(x, s48_value))[i])"))
    (c-define (string-append
	       "S48_STOB_SET(x, i, v) "
	       "do { "
	       "s48_value __stob_set_x = (x); "
	       "long __stob_set_i = (i); "
	       "s48_value __stob_set_v = (v); "
	       "if (S48_STOB_IMMUTABLEP(__stob_set_x)) "
	       "s48_raise_argument_type_error(__stob_set_x); "
	       "else { "
	       "S48_WRITE_BARRIER((__stob_set_x), "
	       "(char *) (&S48_STOB_REF((__stob_set_x), (__stob_set_i))),"
	       "(__stob_set_v)); "
	       "*(&S48_STOB_REF((__stob_set_x), (__stob_set_i))) = (__stob_set_v); "
	       "} "
	       "} while (0)"))
    (c-define (string-append
	       "S48_STOB_BYTE_SET(x, i, v) "
	       "do { "
	       "s48_value __stob_set_x = (x); "
	       "long __stob_set_i = (i); "
	       "char __stob_set_v = (v); "
	       "if (S48_STOB_IMMUTABLEP(__stob_set_x)) "
	       "s48_raise_argument_type_error(__stob_set_x); "
	       "else "
	       "*(&S48_STOB_BYTE_REF((__stob_set_x), (__stob_set_i))) = (__stob_set_v); "
	       "} while (0)"))
    (c-define "S48_STOB_TYPE(x)   ((S48_STOB_HEADER(x)>>2)&~D)" type-mask)
    (c-define "S48_STOB_HEADER(x) (S48_STOB_REF((x),-1))")
    (c-define "S48_STOB_ADDRESS(x) (&(S48_STOB_HEADER(x)))")
    (c-define "S48_STOB_BYTE_LENGTH(x) (S48_STOB_HEADER(x) >> 8)")
    (c-define "S48_STOB_DESCRIPTOR_LENGTH(x) (S48_STOB_HEADER(x) >> 10)")
    (c-define "S48_STOB_IMMUTABLEP(x) ((S48_STOB_HEADER(x)>>7) & 1)")
    (c-define "S48_STOB_MAKE_IMMUTABLE(x) ((S48_STOB_HEADER(x)) |= (1<<7))")
    (newline)
    (do ((stob stob-list (cdr stob))
	 (i 0 (+ i 1)))
	((null? stob))
      (let ((name (upcase (car stob))))
	(c-define "S48_STOBTYPE_~A ~D" name i)
	(c-define "S48_~A_P(x) (s48_stob_has_type(x, ~D))" name i)))
    (newline)
    (for-each (lambda (data)
		(let ((type (upcase (car data))))
		  (do ((accs (cdddr data) (cdr accs))
		       (i 0 (+ i 1)))
		      ((null? accs))
		    (let ((name (upcase (caar accs))))
		      (c-define "S48_~A_OFFSET ~D" name i)
		      (c-define "S48_~A(x) (s48_stob_ref((x), S48_STOBTYPE_~A, ~D))"
				name type i)
		      (c-define "S48_UNSAFE_~A(x) (S48_STOB_REF((x), ~D))" name i))
		    (if (not (null? (cdar accs)))
			(let ((name (upcase (cadar accs))))
			  (c-define "S48_~A(x, v) (s48_stob_set((x), S48_STOBTYPE_~A, ~D, (v)))"
				    name type i)
			  (c-define "S48_UNSAFE_~A(x, v) S48_STOB_SET((x), ~D, (v))" name i))))))
	      stob-data)
    (newline)
    (for-each (lambda (type index)
		(c-define "S48_~A_LENGTH(x) (s48_stob_length((x), S48_STOBTYPE_~A))"
			  type type)
		(c-define "S48_UNSAFE_~A_LENGTH(x) (S48_STOB_DESCRIPTOR_LENGTH(x))"
			  type)
		(c-define "S48_~A_REF(x, i) (s48_stob_ref((x), S48_STOBTYPE_~A, ~A))"
			  type type index)
		(c-define "S48_~A_SET(x, i, v) (s48_stob_set((x), S48_STOBTYPE_~A, ~A, (v)))"
			  type type index)
		(c-define "S48_UNSAFE_~A_REF(x, i) (S48_STOB_REF((x), ~A))"
			  type index)
		(c-define "S48_UNSAFE_~A_SET(x, i, v) S48_STOB_SET((x), ~A, (v))"
			  type index))
	      '("VECTOR" "RECORD")
	      '("(i)" "(i) + 1"))
    (c-define "S48_RECORD_TYPE(x) (s48_stob_ref((x), S48_STOBTYPE_RECORD, 0))")
    (c-define "S48_UNSAFE_RECORD_TYPE(x) (S48_STOB_REF((x), 0))")
    (for-each (lambda (type)
		(c-define "S48_~A_LENGTH(x) (s48_stob_byte_length((x), S48_STOBTYPE_~A))"
			  type type)
		(c-define "S48_~A_REF(x, i) (s48_stob_byte_ref((x), S48_STOBTYPE_~A, (i)))"
			  type type)
		(c-define "S48_~A_SET(x, i, v) (s48_stob_byte_set((x), S48_STOBTYPE_~A, (i), (v)))"
			  type type)
		(c-define "S48_UNSAFE_~A_REF(x, i) (S48_STOB_BYTE_REF((x), (i)))"
			  type)
		(c-define "S48_UNSAFE_~A_SET(x, i, v) S48_BYTE_STOB_SET((x), (i), (v))"
			  type))
	      '("BYTE_VECTOR"))
    (c-define "S48_UNSAFE_BYTE_VECTOR_LENGTH(x) (S48_STOB_BYTE_LENGTH(x))")
    (c-define "S48_UNSAFE_EXTRACT_BYTE_VECTOR(x) (S48_ADDRESS_AFTER_HEADER((x), char))")

    (c-define (string-append "S48_EXTRACT_EXTERNAL_OBJECT(x, type) "
			     "((type *)(S48_ADDRESS_AFTER_HEADER(x, long)+1))"))))

(define (enumeration-stuff names format-string)
  (do ((names names (cdr names))
       (i 0 (+ 1 i)))
      ((null? names))
    (let ((name (upcase (car names))))
      (c-define format-string name i))))

; - becomes _ > becomes TO_ (so -> turns into _TO_)
; ? becomes P
; ! disappears

(define (upcase symbol)
  (do ((chars (string->list (symbol->string symbol)) (cdr chars))
       (res '() (case (car chars)
		  ((#\>) (append (string->list "_OT") res))
		  ((#\-) (cons #\_ res))
		  ((#\?) (cons #\P res))
		  ((#\/ #\!) res)
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

; Copy the file to the current-output-file.

(define (copy-file filename)
  (call-with-input-file filename
      (lambda (in)
	(let loop ()
	  (let ((c (read-char in)))
	    (if (not (eof-object? c))
		(begin
		  (write-char c)
		  (loop))))))))

; WHAT-FOR is a list of names, used only for debugging.
; PRED+EXTRACT is a list of <predicate0> <extract0> <predicate1> <extract1> ... .
; Each form in the file is read and passed to the predicates that haven't yet
; matched.  If the predicate matches the corresponding extractor is called on
; the form.  The results of the extractors are returned.
;
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
