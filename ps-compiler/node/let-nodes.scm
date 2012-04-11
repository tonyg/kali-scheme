; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1998 by NEC Research Institute, Inc.    See file COPYING.


;   This is a backquote-like macro for building nodes.
;
;   One goal is to produce code that is as efficient as possible.
;
; (LET-NODES (<spec1> ... <specN>) . <body>)
;
; <spec> ::= (<ident> <real-call>) |                            ; call node
;            (<ident> (<var1> ... <varN>) <call>) |             ; lambda node
;            (<ident> (<var1> ... <varN> . <last-vars>) <call>) ; lambda node
;
; <var>  ::= #f |             Ignored variable position
;            <ident> |        Evaluate <ident> and copy it, rebinding <ident>
;            '<ident> |       Evaluate <ident> to get the variable
;            (<ident> <rep>)  (MAKE-VARIABLE <ident> <rep>)
;
; <last-vars> ::= <ident>
;
; <call> ::= <ident> | <real-call>
;
; <real-call> ::= (<primop-id> <exits> . <arg-list>)
;
; <arg-list> ::= (<arg1> ... <argN>) | (<arg1> ... <argN> . <last-args>)
;
; <last-args> ::= <ident>
;
; <arg>  ::= 'foo        literal node containing the value of foo, no rep
;            '(foo rep)     "     "       "       "    "   "   " , using rep
;            (* foo)     reference to foo (which evaluates to a variable)
;            (! foo)     foo evaluates to a node
;            foo         short for (! foo) when foo is an atom
;            #f          put nothing here
;            (<primop-id> . <args>)   a nested call
;--------------------------------------
;
; Example:
;
;  (let-nodes ((c1 (l1 1 cont))
;              (l1 ((j type/pointer)) (proc 2 l2 l3 . rest))
;              (l2 () ((jump cont (* j)) '(true-value type/boolean)))
;              (l3 () ((jump cont (* j)) '(false-value type/boolean))))
;    (replace-body node c1))
;
;  ==>
;
;  (LET ((J (CREATE-VARIABLE 'J TYPE/POINTER)))
;    (LET ((C1 (CREATE-CALL-NODE '2 1))
;          (C.1225 (CREATE-CALL-NODE (+ '3 (LENGTH REST)) '2))
;          (L1 (CREATE-LAMBDA-NODE 'C 'CONT (FLIST1 J '())))
;          (C.1224 (CREATE-CALL-NODE '4 0))
;          (L2 (CREATE-LAMBDA-NODE 'C 'CONT '()))
;          (C.1223 (CREATE-CALL-NODE '4 0))
;          (L3 (CREATE-LAMBDA-NODE 'C 'CONT '())))
;      (ATTACH 0 C1 L1)
;      (ATTACH 1 C1 CONT)
;      (ATTACH 0 C.1225 PROC)
;      (ATTACH-CALL-ARGS C.1225 (APPEND (LIST L2 L3) REST))
;      (ATTACH-BODY L1 C.1225)
;      (ATTACH 0 C.1224 (CREATE-PRIMOP-NODE PRIMOP/JUMP))
;      (ATTACH-THREE-CALL-ARGS C.1224
;                              (CREATE-JUMP-MARKER CONT)
;                              (CREATE-REFERENCE-NODE J)
;                              (CREATE-LITERAL-NODE TRUE-VALUE TYPE/BOOLEAN))
;      (ATTACH-BODY L2 C.1224)
;      (ATTACH 0 C.1223 (CREATE-PRIMOP-NODE PRIMOP/JUMP))
;      (ATTACH-THREE-CALL-ARGS C.1223
;                              (CREATE-JUMP-MARKER CONT)
;                              (CREATE-REFERENCE-NODE J)
;                              (CREATE-LITERAL-NODE FALSE-VALUE TYPE/BOOLEAN))
;      (ATTACH-BODY L3 C.1223)
;      (REPLACE-BODY NODE C1)))
; 

(define (expand-let-nodes form rename compare)
  (destructure (((#f specs . body) form))
    (receive (vars nodes code)
	(parse-node-specs specs rename compare)
      `(,(rename 'let) ,vars
         (,(rename 'let) ,nodes
	    ,@code
	    ,@body)))))

(define (test form)
  (destructure (((#f specs . body) form))
    (receive (vars nodes code)
	(parse-node-specs specs identity eq?)
      `(let ,vars
	 (let ,nodes
	    ,@code
	    ,@body)))))

; Parse the specs, returning a list of variable specs, a list of node specs,
; and a list of construction forms.  An input spec is either a call or a
; lambda, each is parsed by an appropriate procedure.

(define (parse-node-specs specs r c)
  (let loop ((specs (reverse specs)) (vars '()) (nodes '()) (codes '()))
    (if (null? specs)
        (values vars nodes codes)
        (destructure ((((name . spec) . rest) specs))
          (cond ((null? (cdr spec))
                 (receive (node code)
		     (construct-call name (car spec) r c)
                   (loop rest vars
                         `((,name ,node) . ,nodes) (append code codes))))
                ((= 2 (length spec))
                 (receive (vs node new-spec call)
		     (construct-lambda (car spec) (cadr spec) r c)
                   (loop (if new-spec (cons new-spec rest) rest)
                         (append vs vars)
                         `((,name ,node) . ,nodes)
                         (if call 
                             `((attach-body ,name ,call) . ,codes)
                             codes))))
                (else
                 (error "illegal spec in LET-NODES ~S" (cons name spec))))))))

; The names of the call-arg relation procedures, indexed by the number of
; arguments handled.

(define call-attach-names
  '#(#f
     #f
     attach-two-call-args
     attach-three-call-args
     attach-four-call-args
     attach-five-call-args))

; Return the node spec and construction forms for a call.  This dispatches
; on whether the argument list is proper or not.
;
; <real-call> ::= (<arg0> <exits> <arg1> ... <argN>) |
;                 (<arg0> <exits> <arg1> ... <argN> . <last-args>))
;                 ((JUMP l-node value) <arg1> ... <argN>)
;                 ((JUMP l-node value) <arg1> ... <argN> . <last-args>)

(define (construct-call name specs r c)
  (destructure (((proc . args) specs))
    (really-construct-call name proc (car args) '() (cdr args) r c)))

(define (construct-nested-call specs r c)
  (destructure (((primop-id . args) specs))
    (let ((name (r 'call)))
      (receive (node code)
	  (really-construct-call name primop-id 0 '() args r c)
	`(,(r 'let) ((,name ,node)) ,@code ,name)))))
		    
(define (really-construct-call name primop-id exits extra args r c)
  (receive (arg-count arg-code)
      (parse-call-args name extra args r c)
    (let ((primop-code (get-primop-code primop-id r)))
      (values `(,(r 'make-call-node) ,primop-code ,arg-count ,exits)
	      arg-code))))

(define (get-primop-code id r)
  (cond ((name->enumerand id primop)
	 => (lambda (n)
	      `(,(r 'get-primop) ,n)))
	(else
	 `(,(r 'lookup-primop) ',id))))

; NAME     = the call node which gets the arguments
; EXTRA    = initial, already expanded arguments
; ARGS     = unexpanded arguments
; LAST-ARG = an atom whose value is added to the end of the arguments
; Returns ARG-COUNT-CODE and ARG-CODE

(define (parse-call-args name extra args r c)
  (receive (args last-arg)
      (decouple-improper-list args)
    (let* ((args (append extra (map (lambda (a) (construct-node a r c)) args)))
	   (count (length args)))
      (if (not (null? last-arg))
	  (values `(,(r '+) ,count (,(r 'length) ,last-arg))
		  `((,(r 'attach-call-args)
		     ,name
		     ,(if (null? args)
			  last-arg
			  `(,(r 'append) (,(r 'list) . ,args) ,last-arg)))))
	  (values count
		  (cond ((= count 0)
			 '())
			((and (= count 1) (car args))
			 `((,(r 'attach) ,name 0 ,(car args))))
			((and (< count 6)
			      (every? identity args))
			 `((,(r (vector-ref call-attach-names count))
			    ,name
			    ,@args)))
			(else
			 `((,(r 'attach-call-args) ,name (list . ,args))))))))))

; Return proper part of the list and its last-cdr separately.

(define (decouple-improper-list list)
  (do ((list list (cdr list))
       (res '() (cons (car list) res)))
      ((atom? list)
       (values (reverse! res) list))))

; Dispatch on the type of the SPEC and return the appropriate code.
;
; <arg>  ::= 'foo         literal node containing the value of foo, no rep
;            '(foo rep)   literal node containing the value of foo
;            (* foo)      reference to foo (which evaluates to a variable)
;            (! foo)      foo evaluates to a node
;            name         short for (! name) when foo is an atom

(define (construct-node spec r c)
  (cond ((atom? spec) spec)
        (else
         (destructure (((key data) spec))
           (case key
             ((*)     `(,(r 'make-reference-node) ,data))
             ((quote) (if (pair? data)
                          `(,(r 'make-literal-node) . ,data)
                          `(,(r 'make-literal-node) ,data type/unknown)))
             ((!)     data)
             (else
	      (construct-nested-call spec r c)))))))

; Parse a lambda spec.  This returns a list of variable specs, code to
; construct the lambda node, a spec for the body if necessary, and
; the code needed to put it all together.

(define (construct-lambda vars call r c)
  (receive (vars node)
           (construct-vars vars r c)
    (cond ((null? call)
           (values vars node #f #f))
          ((atom? call)
           (values vars node #f call))
          (else
           (let ((sym (r (generate-symbol 'c))))
             (values vars node `(,sym ,call) sym))))))

; Returns the code needed to construct the variables and the code to make
; the lambda node that binds the variables.
;
; <var>  ::= #f |             Ignored variable position
;            <ident> |        Evaluate <ident> and copy it, rebinding <ident>
;            '<ident> |       Evaluate <ident> to get the variable
;            (<ident> <rep>)  (MAKE-VARIABLE <ident> <rep>)

(define (construct-vars vars r c)
  (let loop ((vs vars) (vlist '()) (code '()))
    (cond ((atom? vs)
           (let ((vars (if (null? vs)
			   `(,(r 'list) . ,(reverse! vlist))
			   `(,(r 'append) (,(r 'list) . ,(reverse! vlist))
					  ,vs))))
             (values code `(,(r 'make-lambda-node) 'c 'cont ,vars))))
          (else
           (let ((spec (car vs))
                 (rest (cdr vs)))
             (cond ((null? spec)
                    (loop rest (cons #f vlist) code))
                   ((atom? spec)
                    (loop rest (cons spec vlist)
                          `((,spec (,(r 'copy-variable) ,spec)) . ,code)))
                   ((c (car spec) 'quote)
                    (loop rest (cons (cadr spec) vlist) code))
                   (else
                    (loop rest (cons (car spec) vlist)
                          `((,(car spec)
                             (,(r 'make-variable) ',(car spec) ,(cadr spec)))
                            . ,code)))))))))

;------------------------------------------------------------------------------
; GENSYM utility

(define *generate-symbol-index* 0)

(define (generate-symbol sym)
  (let ((i *generate-symbol-index*))
    (set! *generate-symbol-index* (+ i 1))
    (concatenate-symbol sym "." i)))

