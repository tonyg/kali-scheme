; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


; Generating C code for a call

(define (call->c node port indent)
  (let loop ((node node))
    (if (primop-call->c node port indent)
	(loop (lambda-body (call-arg node 0))))))

(define (primop-call->c node port indent)
  (let ((primop (call-primop node)))
    (if (and (simple-c-primop? primop)
	     (= 0 (call-exits node)))
	(generate-simple-assignment primop node port indent)
	(primop-generate-c primop node port indent))
    (and (= 1 (call-exits node))
	 (not (goto-call? node)))))

(define (c-value node port)
  (cond ((string? node)
	 (display node port))
	((not (node? node))
	 (display node port))
	((reference-node? node)
	 (c-variable (reference-variable node) port))
	((literal-node? node)
	 (c-literal-value (literal-value node) (literal-type node) port))
	((call-node? node)        ; must be simple
	 (let ((parens? (call-needs-parens? node)))
	   (if parens? (write-char #\( port))
	   (primop-generate-c (call-primop node) node port 0)
	   (if parens? (write-char #\) port))))
	(else
	 (bug "odd node in C-VALUE ~S" node))))

(define (c-literal-value value type port)
  (let ((value (cond ((integer? value) value)
		     ((real? value) value)
		     ((eq? value #f) 0)
		     ((eq? value #t) 1)
		     ((string? value) value)
		     ((external-value? value) value)
		     ((external-constant? value) value)
		     ((char? value) (char->ascii value))
		     (else
		      (error "cannot translate literal to C ~A" value)))))
    (cond ((integer? value)
	   (format port "~D" value))
	  ((string? value)
	   (c-string-constant value port))
	  ((external-value? value)
	   (display (external-value-string value) port))
	  ((external-constant? value)
	   (display (external-constant-c-string value) port))
	  (else
	   (display value port)))))

(define (c-string-constant string port)
  (write-char #\" port)
  (do ((i 0 (+ i 1)))
      ((= i (string-length string)))
    (let ((char (string-ref string i)))
      (case char
	((#\newline)
	 (write-char #\\ port)
	 (write-char #\n port))
	((#\")
	 (write-char #\\ port)
	 (write-char #\" port))
	((#\\)
	 (write-char #\\ port)
	 (write-char #\\ port))
	(else
	 (write-char char port)))))
  (write-char #\" port))

;	(case (base-type-size (maybe-follow-uvar type))
;	  ((1)
;	   (let ((new-value (if (>= value 0)
;				(bitwise-and value 255)
;				(error "can't translate negative character constants to C ~S"
;				       value))))
;	     (format port "'\\~D~D~D'"
;		     (remainder (quotient new-value 64) 8)
;		     (remainder (quotient new-value 8) 8)
;		     (remainder new-value 8))))
;	  ((2)
;	   (format port "~D" value))
;	  ((4)
;	   (format port "~DL" value))
;	  (else
;	   (error "cannot translate literal type to C ~S" type)))

; Cut down on the number of unnecessary parentheses.  We don't go so far as
; to pay attention to C's precedence rules.

(define (call-needs-parens? call)
  (and (not (and (eq? 'contents (primop-id (call-primop call)))
		 (eq? 'global (literal-value (call-arg call loc/type)))))
       (let ((parent (node-parent call)))
	 (and (node? parent)
	      (call-node? parent)
	      (not (eq? 'let
			(primop-id (call-primop parent))))))))

; Each local variable has a unique integer used to disambiguate in the
; C code.  Using our own, instead of what variables already have, keeps
; the numbers smaller and more readable.

(define *c-variable-id* '0)

(define (next-c-variable-id)
  (let ((id *c-variable-id*))
    (set! *c-variable-id* (+ *c-variable-id* 1))
    id))

(define (c-variable-id var)
  (if (integer? (variable-generate var))
      (variable-generate var)
      (let ((id (next-c-variable-id)))
	(set! *local-vars* (cons var *local-vars*))
	(set-variable-generate! var id)
	id)))

(define (c-variable var port)
  (really-c-variable var port #t))

(define (c-variable-no-shadowing var port)
  (really-c-variable var port #f))

(define (really-c-variable var port shadow?)
  (cond ((string? var)
	 (display var port))
	((symbol? var)
	 (display var port))
	((not (variable? var))
	 (bug "funny value for C-VARIABLE ~S" var))
	((not (variable-binder var))
	 (cond ((and shadow?
		     (memq? 'shadowed (variable-flags var)))
		(writec port '#\R))
	       ((generated-top-variable? var)
		(writec port '#\H)))
	 (write-c-identifier (variable-name var) port)
	 (if (generated-top-variable? var)
	     (display (c-variable-id var) port)))
	(else
	 ;		  (if (= (c-variable-id var) 944)
	 ;		      (breakpoint "writing 944"))
	 (write-c-identifier (variable-name var) port)
	 (write-char '#\_ port)
	 (display (c-variable-id var) port)
	 (write-char '#\X port))))

;==============================================================================;
; Scheme identifiers contain many characters that are not legal in C
; identifiers.  Luckily C is case-sensitive and Scheme is not.

(define char-translations
  (let* ((count number-of-char-codes)
         (string (make-string count)))
    (do ((i '0 (+ i '1)))
        ((>= i count))
      (let ((char (ascii->char i)))
        (string-set! string i
		     (cond ((and (char-alphabetic? char)
				 (char=? char
					 (string-ref (symbol->string
						      (string->symbol
						       (list->string
							(list char))))
						     0)))
			    (char-downcase char))
			   ((char-numeric? char)
			    char)
			   (else
			    (ascii->char 0))))))
    (string-set! string (char->ascii '#\+) '#\A)
    (string-set! string (char->ascii '#\!) '#\B)
    (string-set! string (char->ascii '#\:) '#\C)
    (string-set! string (char->ascii '#\.) '#\D)
    (string-set! string (char->ascii '#\=) '#\E)
    (string-set! string (char->ascii '#\>) '#\G)
    ; used for flattened closures             H
    ; precedes C keywords                     K
    (string-set! string (char->ascii '#\<) '#\L)
    (string-set! string (char->ascii '#\?) '#\P)
    (string-set! string (char->ascii '#\%) '#\Q)
    (string-set! string (char->ascii '#\*) '#\S)
    ; used for tail-recursive procedures      T
    (string-set! string (char->ascii '#\/) '#\U)
    (string-set! string (char->ascii '#\#) '#\W)
    ; follows lexical identifiers             X
    ; used by the multi-procedure block code  Z
    (string-set! string (char->ascii '#\-) '#\_)
    string))

; This needs to check for C keywords (just precede with K)

(define (write-c-identifier symbol port)
  (if (table-ref c-keywords symbol)
      (writec port '#\K))
  (let ((string (symbol->string symbol)))
    (do ((i 0 (+ i 1)))
	((>= i (string-length string)))
      (let* ((char (string-ref string i))
	     (out (string-ref char-translations (char->ascii char))))
	(if (= 0 (char->ascii out))
	    (bug "cannot translate ~S from ~A into C" char string)
	    (writec port out))))
    (values)))

(define (c-ify symbol)
  (call-with-string-output-port
    (lambda (port)
      (write-c-identifier symbol port))))

(define c-keywords (make-table))

(for-each (lambda (k)
	    (table-set! c-keywords k #t))
	  '(
	    auto     double int      struct
	    break    else   long     switch
	    case     enum   register typedef
	    char     extern return   union
	    const    float  short    unsigned
	    continue for    signed   void
	    default  goto   sizeof   volatile
	    do       if     static   while
	    ))

;==============================================================================;

(define (simple-c-primop op call port)
  (case (call-arg-count call)
    ((1)
     (generate-simple-c-monop-call op (call-arg call 0) port))
    ((2)
     (destructure ((#(arg1 arg2) (call-args call)))
       (generate-simple-c-binop-call op arg1 arg2 port)))
    (else
     (bug "funny call to SIMPLE-C-PRIMOP ~S" call))))

(define (generate-simple-c-binop-call op arg1 arg2 port)
  (c-value arg1 port)
  (writec port '#\space)
  (display op port)
  (writec port '#\space)
  (c-value arg2 port)
  (values))

(define (generate-simple-c-monop-call op arg1 port)
  (display op port)
  (writec port '#\space)
  (c-value arg1 port)
  (values))

(define (generate-simple-assignment primop call port indent)
  (let ((var (car (lambda-variables (call-arg call 0)))))
    (c-assign-to-variable var port indent)
    (primop-generate-c primop call port #f)
    (writec port '#\;)
    (values)))

(define (c-assignment var value port indent)
  (c-assign-to-variable var port indent)
  (c-value value port)
  (writec port '#\;))

(define (c-assign-to-variable var port indent)
  (indent-to port indent)
  (cond ((or (not (variable? var))
	     (and (or (used? var)
		      (global-variable? var))
		  (not (eq? type/unit (final-variable-type var)))))
	 (c-variable var port)
	 (display " = " port))))

;==============================================================================;

(define (known-variable-reference node)
  (cond ((reference-node? node)
	 (let ((var (reference-variable node)))
	   (if (global-variable? var) var #f)))
	(else #f)))

(define (write-value-list args start port)
  (writec port '#\()
  (really-write-value-list args start '() port)
  (writec port '#\)))

(define (write-value-list-with-extras args start extras port)
  (writec port '#\()
  (really-write-value-list args start extras port)
  (writec port '#\)))

(define (really-write-value-list args start extras port)
  (let ((len (vector-length args)))
    (cond ((> len start)
           (c-value (vector-ref args start) port)
           (do ((i (+ start '1) (+ i '1)))
               ((>= i len) (values))
             (writec port '#\,)
             (writec port '#\space)
	     (c-value (vector-ref args i) port))
	   (write-comma-value-list extras port))
	  ((not (null? extras))
	   (c-value (car extras) port)
	   (write-comma-value-list (cdr extras) port)))))
	   
(define (write-comma-value-list args port)
  (for-each (lambda (arg)
	      (writec port '#\,)
	      (writec port '#\space)
	      (c-value arg port))
	    args))

(define (write-value+result-var-list args start vars port)
  (writec port '#\()
  (really-write-value-list args start '() port)
  (cond ((not (null? vars))
	 (if (> (vector-length args) start)
	     (display ", " port))
	 (writec port #\&)
	 (c-variable (car vars) port)
	 (for-each (lambda (var)
		     (display ", &" port)
		     (c-variable var port))
		   (cdr vars))))
  (writec port #\)))

(define (c-system-call proc args port)
  (display proc port)
  (writec port '#\()
  (if (not (null? args))
      (let loop ((args args))
        (c-value (car args) port)
        (cond ((not (null? (cdr args)))
               (writec port '#\,)
               (writec port '#\space)
               (loop (cdr args))))))
  (writec port '#\))
  (values))

(define (indent-to port indent)
  (if (> (current-column port) indent)
      (newline port))
  (do ((c (current-column port) (+ c 1)))
      ((>= c indent))
    (write-char #\space port)))


