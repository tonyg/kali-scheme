
; Copyright 1994 Olin Shivers.  Used with permission.

(define-structures ((cig-processor (export process-define-foreign-file
					   process-define-foreign-stream))
		    (cig-standalone (export cig-standalone-toplevel))

		    ;; This must be opened in the FOR-SYNTAX package.
		    (define-foreign-syntax-support
		      (export define-foreign-expander)))

  (open scheme formats structure-refs
	destructuring receiving)
  (access signals) ; for ERROR
  (begin
    (define error (structure-ref signals error))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; The general syntax of define-foreign is:
;;; (define-foreign scheme-name (c-name arg1 ... argn) [no-declare]
;;;   ret1
;;;    .
;;;   retn)
;;; 
;;; This defines a Scheme procedure, <scheme-name>. It takes the arguments
;;; arg1 ... argn, type-checks them, and then passes them to a C stub,
;;; df_<c-name>. If the Scheme procedure is to return multiple values, the C
;;; stub also gets a return vector passed to return the extra values. The C
;;; stub rep-converts the Scheme data as specified by the <arg>i declarations,
;;; and then calls the C procedure <c-name>.  The C procedure is expected to
;;; return its first value (<ret1>) as its real value. The other return values
;;; are returned by assigning targets passed by-reference to <c-name> by the
;;; stub. These return parameters are passed after the argument parameters.
;;; When <c-name> returns, the C stub df_<c-name> rep-converts the C data,
;;; stuffs extra return values into the Scheme answer vector if there are any,
;;; and returns to the Scheme routine. The Scheme routine completes the
;;; rep-conversion specified by the <ret>i declarations, and return the
;;; values.
;;; 
;;; An ARGi spec is of the form
;;;   (rep [var])
;;; where REP is something like STRING or VECTOR-DESC or one of the hairier
;;; rep specs, and VAR is the name of the formal parameter -- not necessary,
;;; just for documentation purposes, really. Cig will happily gensym something
;;; up for you if you don't give one.
;;;
;;; Example:
;;;   (define-foreign getenv (getenv (string env-var)) string)
;;;
;;; The optional symbol NO-DECLARE means "Do not place an extern declaration
;;; of the C routine in the body of the stub." This is necessary for the
;;; occasional strange ANSI C declaration that cig is incapable of generating
;;; (the only case I know of where the C procedure uses varargs, so the C
;;; declaration needs a ..., e.g.,
;;; 	extern int open(const char *, int flags, ...);
;;; In this case, just use NO-DECLARE, and insert your own a declaration of open()
;;; outside the stub with a
;;; 	(foreign-source "extern int open(const char *, int flags, ...);")
;;; Ugly, yes.)
;;; 
;;; The rep-conversion specs are pretty hairy and baroque. I kept throwing
;;; in machinery until I was able to handle all the Unix syscalls, so that
;;; is what drove the complexity level. See syscalls.scm for examples.


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; The fields of a rep record:
;;; 0. A Scheme predicate for type-testing args. #f means no check.
;;; 1. A C declaration for argument in its C representation --
;;;    the type of the value actually passed to or returned from the foreign
;;;    function. This is a format string; the ~a is where the C variable goes.
;;;    (format #f c-decl "") is used to compute a pure type -- e.g., for
;;;    casts.
;;; 2. The Scheme->C rep-converter. Applied as a C function/macro in the stub.
;;;    The empty string means the null rep-conversion. #f means not a legal
;;;    parameter rep -- just a return-value rep.
;;; 3. A list of C->Scheme rep-converters. A return value from the C function
;;;    can be rep-converted several different ways by the stub, each producing
;;;    a distinct return value from the stub. For example, a C string might
;;;    be rep-converted into (1) a C pointer to the chars and (2) the length
;;;    of the string: ("" "strlen_or_false") (where strlen_or_false returns
;;;    #f if the string is NULL). #T means not a legal return-value rep
;;;    -- just a parameter rep (I didn't use #f for this purpose so that
;;;    #f = () Schemes can win).
;;; 4. This is a Scheme function that is applied to the rep-converted values
;;;    passed back from the multi-rep-conversion of step 3. Its value is
;;;    the actual return value returned to Scheme. #f means just pass a single
;;;    value back as-is. This is mostly used for string hacking.
;;; 5. This field is for args, not return values. If true, the stub gives
;;;    the rep-converted C argument to free after the C function returns.

(define (rep-info:scheme-pred i) (vector-ref i 0))
(define (rep-info:c-decl i)      (vector-ref i 1))
(define (rep-info:->c i) 	 (vector-ref i 2))
(define (rep-info:->scheme i)    (vector-ref i 3))
(define (rep-info:post-scheme i) (vector-ref i 4))
(define (rep-info:free? i)       (vector-ref i 5))

(define *simple-rep-alist* '(

 (char   #(char?	"char ~a"	"RawChar" 	("SchChar")   #f  #f))
 (bool   #(#f		"int ~a"	"RawBool" 	("SchBool")   #f  #f))

 (integer  #(integer? "int ~a"		"RawFix"	("SchFix") #f #f))
 (short_u  #(integer? "unsigned short ~a"   "RawInt"	("SchFix") #f #f))
 (size_t   #(integer? "size_t ~a"	"RawFix"	("SchFix") #f #f))
 (mode_t   #(integer? "mode_t ~a"	"RawFix"	("SchFix") #f #f))
 (gid_t    #(integer? "gid_t  ~a"	"RawFix"	("SchFix") #f #f))
 (uid_t    #(integer? "uid_t ~a"	"RawFix"	("SchFix") #f #f))
 (off_t    #(integer? "off_t ~a"	"RawFix"	("SchFix") #f #f))
 (pid_t    #(integer? "pid_t ~a"	"RawFix"	("SchFix") #f #f))
 (uint_t   #(integer? "unsigned int ~a"	"RawFix"	("SchFix") #f #f))
 (long     #(integer? "long ~a"		"RawFix"	("SchFix") #f #f))

 (desc		 #(#f		"Descriptor ~a" "" ("") #f #f))
 (string-desc	 #(string?	"Descriptor ~a" "" ("") #f #f))
 (char-desc	 #(char?	"Descriptor ~a" "" ("") #f #f))
 (integer-desc	 #(integer?	"Descriptor ~a" "" ("") #f #f))
 (vector-desc	 #(vector?	"Descriptor ~a" "" ("") #f #f))
 (pair-desc	 #(pair?	"Descriptor ~a" "" ("") #f #f))

 (string 	#(string?	"const char *~a" "cig_copy_scheme_string"
				("(Descriptor)" "strlen_or_false")
				C->scheme-string-w/len	#t))

 (var-string 	#(string?	"char *~a" "cig_copy_scheme_string"
				("(Descriptor)" "strlen_or_false")
				C->scheme-string-w/len	#t))

 (string-copy 	#(string?	"char *~a" "cig_copy_scheme_string" #t #f #f))

 ;;; Return-value-only reps
 (string-length		#(#f "char *~a" #f ("strlen_or_false") 	#f #f))

 (static-string		#(#f "char *~a" #f ("(Descriptor)copystring_or_die" "strlen_or_false")
			  C->scheme-string-w/len	#f))
 (aligned-static-string #(#f "char *~a" #f ("(Descriptor)" "strlen_or_false")
			  C->scheme-string-w/len-no-free #f))))

;;; String reps:
;;; -----------
;;; - STRING-COPY
;;;   Parameter only. The C routine is given a private, malloc'd C string.
;;;   The string is not freed when the routine returns.
;;;
;;; - STRING
;;;   Parameter: The C routine is given a C string that it should not alter
;;;   or retain beyond the end of the routine. Right now, the Scheme string
;;;   is copied to a malloc'd C temporary, which is freed after the routine
;;;   returns. Later, we'll just pass a pointer into the actual Scheme
;;;   string, as soon as Richard fixes the S48 string reps.
;;;   Ret value: The C string is from malloc'd storage. Convert it to a
;;;     Scheme string and free the C string.
;;;
;;; - STRING-LENGTH
;;;   Return-value only. Return the length of the C string, as a fixnum.
;;;
;;; - STATIC-STRING
;;;   Return-value only. The C string is not freed after converting it to
;;;   to a Scheme string.
;;;
;;; - ALIGNED-STATIC-STRING
;;;   Return-value only. Just as for STATIC-STRING, but the current version
;;;   of cig can optimise this case. Considering the rest of the overhead,
;;;   probably a silly feature.
;;;
;;; - VAR-STRING
;;;   Same  as STRING, but C type is declared char* instead of const char*.
;;;   Used to keep some broken system call include files happy.

;;; Do #F/#T validity checks!

;;; Parameter reps:
;;; - A simple rep is simply the name of a record in the rep table.
;;;   e.g., integer, string
;;; - (REP scheme-pred c-decl to-c [free?])
;;;   A detailed spec, as outlined above. SCHEME-PRED is a procedure or #f.
;;;   C-DECL is a format string (or a symbol). TO-C is a format string
;;;   (or a symbol).
;;; - (C type)
;;;   The argument is a C value, passed with no type-checking
;;;   or rep-conversion. TYPE is a format string (or a symbol).

;;; A return-value rep is:
;;; - A simple rep, as above.
;;; - (MULTI-REP rep1 ... repn)
;;;   The single value returned from the C function is rep-converted
;;;   n ways, each resulting in a distinct return value from Scheme.
;;; - (TO-SCHEME rep c->scheme)
;;;   Identical to REP, but use the single C->SCHEME form for the return
;;;   rep-conversion. There is no POST-SCHEME processing. This allows you
;;;   to use a special rep-converter on the C side, but otherwise use all
;;;   the properties of some standard rep. C->SCHEME is a string (or symbol).
;;; - (C type)
;;;   Returns a raw C type. No rep-conversion. TYPE is a C type, represented
;;;   as a string (or a symbol).

;;; C Short-hand:
;;; Things that go in the C code are usually specified as strings,
;;; since C is case-sensitive, and Scheme symbols are not. However,
;;; as a convenient short-hand, symbols may also be used -- they
;;; are mapped to strings by lower-casing their print names. This
;;; applies to the TO-C part of (REP ...) and the C->SCHEME part of 
;;; TO-SCHEME.
;;;
;;; Furthermore, C declarations (the TYPE part of (C ...) and the C-DECL part
;;; of (REP ...)) can be either a format string (e.g., "char ~a[]"), or a
;;; symbol (double). A symbol is converted to a string by lower-casing it, and
;;; appending " ~a", so the symbol double is just convenient short-hand for
;;; the C declaration "double ~a".
;;;
;;; Examples: (rep integer? int "RawFix")
;;;           (C char*)
;;;           (C "int ~a[10]")
;;;           (to-scheme integer "HackInt")
;;;
;;; These shorthand forms are not permitted in the actual rep tables; 
;;; only in DEFINE-FOREIGN forms.

(define (parameter-rep->info rep)
  (let* ((hack (lambda (x)
		 (if (symbol? x)
		     (string-append (symbol->string x) " ~a")
		     x)))
	 (do-rep (lambda (scheme-pred c-decl to-c . maybe-free)
		   (vector scheme-pred (hack c-decl)
			   (stringify to-c) #f #f (and (pair? maybe-free)
						       (car maybe-free)))))
	 (you-lose (lambda () (error "Unknown parameter rep" rep)))

	 (info (cond ((symbol? rep)
		      (cond ((assq rep *simple-rep-alist*) => cadr)
			    (else (you-lose))))
	  
		     ((pair? rep)
		      (case (car rep)
			((rep) (apply do-rep (cdr rep)))
			((C) (let ((decl (hack (cadr rep))))
			       (do-rep #f decl
				       (format #f "(~a)" ; Makes a cast.
					       (format #f decl "")))))
			(else (you-lose))))
	  
		     (else (you-lose)))))

    (if (rep-info:->c info) info
	(error "Rep not a parameter-rep" rep))))

(define (copy-vector v)
  (let* ((vlen (vector-length v))
	 (v-new (make-vector vlen)))
    (do ((i (- vlen 1) (- i 1)))
	((< i 0) v-new)
      (vector-set! v-new i (vector-ref v i)))))

(define (stringify x)
  (if (symbol? x)
      (list->string (map char-downcase (string->list (symbol->string x))))
      x))

;;; Return a list of reps (because of MULTI-REP).
(define (return-rep->info rep)
  (let* ((hack (lambda (x)
		 (if (symbol? x)
		     (string-append (symbol->string x) " ~a")
		     x)))
	 (do-rep (lambda (c-decl . to-scheme)
		   (list (vector #f (hack c-decl) #f to-scheme #f #f))))
	 (you-lose (lambda () (error "Unknown return rep" rep)))
    
	 (info (cond ((symbol? rep)
		      (cond ((assq rep *simple-rep-alist*) => cdr)
			    (else (you-lose))))
	  
		     ((pair? rep)
		      (case (car rep)
			((rep) (apply do-rep (cdr rep)))
			((to-scheme)
			 (let ((v (copy-vector (car (return-rep->info (cadr rep))))))
			   (vector-set! v 3 (map stringify (cddr rep)))
			   (vector-set! v 4 #f)
			   (list v)))
			((C) (do-rep (cadr rep) "(Descriptor)"))
			((multi-rep)
			 (apply append (map return-rep->info (cdr rep))))
			(else (you-lose))))
		     (else (you-lose)))))

    (if (eq? #t (rep-info:->scheme (car info)))
	(error "Rep not a return-value rep" rep)
	info)))

;;; Return a type string for IGNORE, or a list of lists of info vectors for
;;; the standard case.

(define (parse-return-reps reps)
  (cond ((or (not (pair? reps))
	     (not (list? reps)))
	 (error "Bad return rep list" reps))
	
	;; (IGNORE c-type) or IGNORE
	((and (null? (cdr reps))
	      (let ((rep (car reps)))
		(or (eq? rep 'ignore)
		    (and (pair? rep)
			 (eq? (car rep) 'ignore)))))
	 (let ((rep (car reps)))
	   (if (pair? rep) (cadr rep) "void ~a")))
	
	(else (map return-rep->info reps))))

(define (insert-commas lis)
  (if (pair? lis)
      (cdr (let rec ((lis lis))
	     (if (pair? lis)
		 (cons ", " (cons (car lis) (rec (cdr lis))))
		 '())))
      '("")))

(define (elts->comma-string lis)
  (apply string-append (insert-commas lis)))

(define (info->decl i . maybe-type)
  (let ((type (if (null? maybe-type) "" (car maybe-type))))
    (format #f (rep-info:c-decl i) type)))

(define (info->var-decl i var)
  (string-append nl "    " (format #f (rep-info:c-decl i) var) ";"))

(define (make-gensym prefix i)
  (lambda (x)
    (set! i (+ i 1))
    (string-append prefix (number->string i))))

(define (make-stub-args nargs mv-return?)
  (do ((i (if mv-return? 1 0) (+ i 1))
       (nargs nargs (- nargs 1))
       (ans '() (cons (format #f "args[~d]" i) ans)))
      ((zero? nargs) ans)))

(define (filter lis)
  (if (pair? lis)
      (let* ((head (car lis))
	     (tail (cdr lis))
	     (new-tail (filter  tail)))
	(if head (if (eq? tail new-tail) lis (cons head new-tail))
	    new-tail))
      '()))

(define nl (string #\newline))
(define (separate-line stmt) (string-append "    " stmt ";" nl))

(define (C-ize info exp)
  (cond ((rep-info:->c info)
	 => (lambda (s)
	      (if (string=? s "") exp
		  (string-append s "(" exp ")"))))
	(else exp)))

;(define (Scheme-ize info exp)
;  (cond ((rep-info:->scheme info)
;	 => (lambda (converter) (Scheme-ize/converter converter exp)))
;	(else exp)))

(define (Scheme-ize converter exp)
  (if (string=? converter "") exp
      (string-append converter "(" exp ")")))

;; Return #f or "    char *f3 = scm2c_string(arg[2]);"
(define (free-var-decl info fvar stub-arg)
  (and (rep-info:free? info)
       (format #f "~%    ~a = ~a;"
	       (format #f (rep-info:c-decl info) fvar)
	       (C-ize info stub-arg))))


(define (make-mv-assigns vars converter-lists)
  (apply string-append
	 (let lp1 ((j 0) (vars vars) (conv-lists converter-lists) (ans '()))
	   (if (pair? vars)
	       
	       (let ((v (car vars))
		     (converter-list (car conv-lists))
		     (vars (cdr vars))
		     (conv-lists (cdr conv-lists)))
		 (do ((j j (+ j 1))
		      (converter-list converter-list (cdr converter-list))
		      (ans2 '()
			    (cons (let ((c (car converter-list)))
				    (format #f  "~%    *VecElt(*args,~d) = ~a;"
					    j (Scheme-ize c v)))
				  ans2)))
		     ((not (pair? converter-list))
		      (lp1 j vars conv-lists (append ans2 ans)))))
	       
	       (reverse ans)))))

(define (stmts strings) (apply string-append strings))

(define (make-free-vars-list infos)
  (do ((j 1 (+ j 1))
       (infos infos (cdr infos))
       (ans '()
	    (cons (let ((i (car infos)))
		    (and (rep-info:free? i) (format #f "f~d" j)))
		  ans)))
      ((not (pair? infos)) (reverse ans))))


;;; Compute the args part of function prototype.
(define (proto-args arg-decls)
  (if (null? arg-decls) "void" ; echh
      (elts->comma-string arg-decls)))


(define (define-foreign->C-stub form)
  (destructure (( (#f scheme-name (c-name . params) . return-reps) form ))
	       (let* ((c-name (stringify c-name))
		      (reps (map car params))
		      
		      (no-declare? (and (pair? return-reps)
					(eq? 'no-declare (car return-reps))))
		      (return-reps (if no-declare? (cdr return-reps)
				       return-reps))

		      (params-info (map parameter-rep->info reps))
		      (ret-infos1 (parse-return-reps return-reps))
		      (ignore? (string? ret-infos1))
		      
		      (ret-infos2 (if (not ignore?)	; Flatten them out.
				      (apply append ret-infos1))) 
		      (ret-infos3 (if (not ignore?)	; A canonical representative
				      (map car ret-infos1)))    ; per item.
		      
		      (primary-retval-info (if (not ignore?) (car ret-infos3)))
		      (primary-retval-decl-template
		       (if ignore?
			   ret-infos1
			   (rep-info:c-decl primary-retval-info)))
		      (primary-retvar-decl (if ignore? ""
					       (format #f "~%    ~a;"
						       (format #f primary-retval-decl-template
							       "r1"))))
		      (primary-retval-converter
		       (if (not ignore?)
			   (car (rep-info:->scheme primary-retval-info))))
		      
		      (retval-converters ; list of converter lists.
		       (if (not ignore?)
			   (map (lambda (infos)
				  (apply append (map rep-info:->scheme infos)))
				ret-infos1)))
		      
		      (mv-return? (and (not ignore?)
				       (or (pair? (cdr ret-infos2))
					   ;; Does r1 return in multiple pieces?
					   (not (null? (cdar retval-converters))))))
		      
		      (nargs (length reps))
		      (stub-nargs (if mv-return? (+ nargs 1) nargs))
		      (other-retvals (if ignore? '() (cdr ret-infos3)))
		      (ret-vars (map (make-gensym "r" 1) other-retvals))
		      (ret-var-decls (stmts (map info->var-decl 
						 other-retvals ret-vars)))
		      
		      (stub-args (make-stub-args nargs mv-return?))
		      
		      (free-vars (make-free-vars-list params-info))
		      (fvar-decls (stmts (filter (map (lambda (i fv)
							(and fv (info->var-decl i fv)))
						      params-info free-vars))))
		      
		      (c-proto (proto-args (append (map info->decl params-info)
						   (map (lambda (i)
							  (info->decl i "*"))
							other-retvals))))
		      
		      (c-fun-decl (format #f primary-retval-decl-template
					  (string-append c-name "(" c-proto ")")))
		      (c-fun-decl (format #f "extern ~a;" c-fun-decl))
		      (c-fun-decl (if no-declare? "" c-fun-decl))
		      
		      (fvar-assigns (stmts (filter (map (lambda (i fv sv)
							  (and fv
							       (format #f "~%    ~a = ~a;"
								       fv (C-ize i sv))))
							params-info
							free-vars
							stub-args))))
		      
		      (c-args (elts->comma-string (append (map (lambda (i fv sv)
								 (or fv (C-ize i sv)))
							       params-info
							       free-vars
							       stub-args)
							  (map (lambda (rv)
								 (string-append "&" rv))
							       ret-vars))))
		      (c-call (string-append c-name "(" c-args ")"))
		      
		      (release-fvars (stmts (filter (map (lambda (fv)
							   (and fv
								(format #f
									"~%    Free(~a);"
									fv)))
							 free-vars))))
		      
		      ;; The little cons/cdar/cdr shuffle is to skip r1's first
		      ;; guy -- he is the *real* return value, not a simulated one.
		      (mv-assigns (if ignore? ""
				      (make-mv-assigns (cons "r1" ret-vars)
						       (cons (cdar retval-converters)
							     (cdr retval-converters)))))

	   (return-stmt (format #f "~%    return ~a;"
				(if ignore? "SCHFALSE"
				    (Scheme-ize primary-retval-converter "r1"))))

	   ;; Do the call, release the free-vars, do the mv-return
	   ;; assignments, then return.
	   (epilog (if ignore?
		       (string-append c-call ";" release-fvars return-stmt)
		       (string-append "r1 = " c-call ";" release-fvars
				      mv-assigns return-stmt))))
;     (breakpoint)
      (format #f cfun-boilerplate
	      c-name
	      c-fun-decl primary-retvar-decl ret-var-decls fvar-decls
	      stub-nargs c-name
	      fvar-assigns
	      epilog))))

(define cfun-boilerplate
  "Descriptor df_~a(long nargs, Descriptor *args)
{
    ~a~a~a~a

    cig_check_nargs(~d, nargs, \"~a\");~a
    ~a
    }

")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(define cfile-header-boilerplate
  "/* This is a Scheme48/C interface file, 
** automatically generated by cig.
*/

#include <stdio.h>
#include <stdlib.h> /* For malloc. */
#include \"cig/cig.h\"

/* Helper function for arg checking. Why bother, actually? */
static void cig_check_nargs(int arity, int nargs, char *fn)
{
   if( arity != nargs ) {
       fprintf(stderr,
	       \"Fatal error (%s) -- C stub expected %d arg%s, \"
	       \"but got %d\\n\",
	       fn, arity, (arity == 1) ? \"\" : \"s\", nargs);
       exit(-1);
       }
   }

/* Helper function for rep-converting string arguments to C. */
static char *cig_copy_scheme_string(Descriptor sstr)
{
    long header;
    char *result;
    int slen;
    extern void *memcpy(void*, const void*, size_t);
    extern int errno;

    if( !IsString(sstr) ) {
        fprintf(stderr,
		\"Fatal error: C stub expected a Scheme string, \"
		\"but got %x instead.\\n\",
		sstr);
	exit(-1);
	}

    slen = StrLen(sstr);
    result = (char*) malloc(slen+1);

    if( result == NULL ) {
        fprintf(stderr,
		\"Fatal error: C stub tried to copy Scheme string,\\n\"
		\"but malloc failed on arg 0x%x, errno %d.\\n\",
		sstr, errno);
	exit(-1);
	}

    memcpy(result, (void *) StobData(sstr), slen);
    result[slen] = '\\000';
    return result;
    }


")

(define (define-foreign-process-form form oport)
  (if (pair? form)
      (case (car form)

	((begin)
	 (if (list? (cdr form))
	     (for-each (lambda (f) (define-foreign-process-form f oport))
		       (cdr form))))

	((define-structures)
	 (if (and (pair? (cdr form))
		  (list? (cddr form)))
	     (let ((clauses (cddr form)))
	       (for-each (lambda (clause)
			   (if (and (pair? clause)
				    (eq? 'begin (car clause)))
			       (define-foreign-process-form clause oport)))
			 clauses))))

	((define-foreign)
	 (display  (define-foreign->C-stub form) oport))

	((foreign-source)
	 (let ((forms (cdr form)))
	   (if (pair? forms)
	       (begin (display (car forms) oport)
		      (map (lambda (x)
			     (newline oport)
			     (display x oport))
			   (cdr forms)))))))))

(define (process-define-foreign-stream iport oport)
  (display cfile-header-boilerplate oport)
  (let lp ()
    (let ((form (read iport)))
      (cond ((not (eof-object? form))
	     (define-foreign-process-form form oport)
	     (lp))))))

(define (process-define-foreign-file fname)
  (call-with-input-file (string-append fname ".scm")
    (lambda (iport)
      (call-with-output-file (string-append fname ".c")
	(lambda (oport)
	  (process-define-foreign-stream iport oport))))))


(define (cig-standalone-toplevel . args) ; ignore your args.
  (process-define-foreign-stream (current-input-port)
				 (current-output-port))
  0)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;(define-syntax define-foreign define-foreign-expander)

(define (define-foreign-expander form rename compare)
  (destructure (( (#f scheme-name (c-name . params) . return-reps) form ))
    (let* ((c-name (string-append "df_" (stringify c-name)))

	   (reps (map car params))
	   (params-info (map parameter-rep->info reps))

	   (return-reps (if (and (pair? return-reps)
				 (eq? 'no-declare (car return-reps)))
			    (cdr return-reps)
			    return-reps))
	   (ret-infos1 (parse-return-reps return-reps))
	   (ignore? (string? ret-infos1))

	   (ret-infos2 (if (not ignore?)
			   (apply append ret-infos1)))
	   (scheme-mv-return? (and (not ignore?)
				   (pair? (cdr ret-infos2))))
	   (c-mv-return? (or scheme-mv-return?
			     (and (not ignore?)
				  (pair? (cdr (rep-info:->scheme
					       (car ret-infos2)))))))
	     
	   (mv-veclen (if (not ignore?)
			  (- (apply + (map (lambda (i)
					     (length (rep-info:->scheme i)))
					   ret-infos2))
			     1)))

	   (%define (rename 'define))
	   (%let (rename 'let))
	   (%lambda (rename 'lambda))
	   (%external-call (rename 'external-call))
	   (%get-external (rename 'get-external))

	   (gensym (let ((gs (make-gensym "g" -1)))
		     (lambda () (string->symbol (gs #f)))))

	   (args (map (lambda (p)
			(let ((tail (cdr p)))
			  (if (pair? tail) (car tail)
			      (gensym))))
		      params))

	   (%string?   (rename 'string?))
	   (%char?     (rename 'char?))
	   (%integer?  (rename 'integer?))
	   (%vector?   (rename 'vector?))
	   (%pair?   (rename 'pair?))
	   (%check-arg (rename 'check-arg))

	   (rep-checker (lambda (i arg)
			  (cond ((rep-info:scheme-pred i) =>
				 (lambda (pred) `(,%check-arg ,pred ,arg
							      ,scheme-name)))
				(else arg))))

	   (c-args (map rep-checker params-info args))
	   (%f (rename 'f)))

      (if (not c-mv-return?)
	  (let* ((xcall `(,%external-call ,%f ,@c-args))
		 (xcall (cond ((and (not ignore?)
				    (rep-info:post-scheme (car ret-infos2)))
			       => (lambda (proc) `(,proc ,xcall))) ; not hygenic
			      (else xcall))))

	  `(,%define ,scheme-name
	     (,%let ((,%f (,%get-external ,c-name)))
	       (,%lambda ,args ,xcall))))

	  (let ((retarg1 (rename 'r1))
		(retarg2 (rename 'r2))
		(%make-vector (rename 'make-vector)))
	    `(,%define ,scheme-name
	       (,%let ((,%f (,%get-external ,c-name))
		       (,retarg1 (,%make-vector ,mv-veclen)))
	         (,%lambda ,args
		   (,%let ((,retarg2 (,%external-call ,%f ,@c-args ,retarg1)))
		     (values ,@(make-values-args retarg1 retarg2
						 ret-infos2
						 rename)))))))))))


(define (c-arg i retarg1 retarg2 %vector-ref)
  (if (zero? i)
      retarg2
      `(,%vector-ref ,retarg1 ,(- i 1))))

;; Return i' and the expression
(define (make-arg info i rename retarg1 retarg2)
  (let ((%vector-ref (rename 'vector-ref)))
    (do ((i i (+ i 1))
	 (converters (rep-info:->scheme info) (cdr converters))
	 (args '()
	       (cons (c-arg i retarg1 retarg2 %vector-ref)
		     args)))
	((not (pair? converters))
	 (values i (let ((ps (rep-info:post-scheme info)))
		     (if ps
			 `(,(rep-info:post-scheme info) ; Not hygenic!
			   ,@(reverse args))
			 (car args))))))))
       
(define (make-values-args arg1 arg2 retvals rename)
  (let lp ((i 0) (ans '()) (retvals retvals))
    (if (pair? retvals)
	(let ((retval (car retvals)))
	  (receive (i exp)
	      (make-arg retval i rename arg1 arg2)
	    (lp i (cons exp ans) (cdr retvals))))
	(reverse ans))))

)) ; serutcurts

(define-structure define-foreign-syntax (export (define-foreign :syntax)
						(foreign-source :syntax))
  (open scheme externals structure-refs)
  (access signals) ; for ERROR
  (for-syntax (open scheme define-foreign-syntax-support))
  (begin
    (define error (structure-ref signals error))

    ;; Sux.
    (define-syntax define-foreign (begin define-foreign-expander))

    ;; Ignore FOREIGN-SOURCE forms.
    (define-syntax foreign-source
      (syntax-rules ()
	((foreign-source . stuff) #f)))

    (define (check-arg pred obj proc)
      (if (not (pred obj))
	  (error "check-arg" pred obj proc)
	  obj))
)) ; erutcurts

