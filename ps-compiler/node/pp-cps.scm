; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.

; Pretty-printing the node tree

; Sample output:

;  34  (F_12 (C_11 UNIT_0)
;        (SET-CONTENTS 1 C_11 UNIT_0 UNIT '0 ^F_14))
;
;  35   (F_14 (C_13 N_1)
;  36     (LET* (((LOOP_73)  (CONS CELL '0))
;  37            (()         (SET-CONTENTS LOOP_73 CELL '0 ^F_34))
;  38            ((V_77)     (CONTENTS LOOP_73 CELL '0)))
;           (V_77 1 C_13 N_1 '1)))
;
;  39    (F_34 (C_33 I_9 R_7)
;  40      (LET* (((V_61)     (CONTENTS UNIT_0 UNIT '3))
;  41             ((V_63)     (V_61 I_9 '0)))
;            (TRUE? 2 ^C_58 ^C_41 V_63)))
;
;  42     (C_58 ()
;           (C_33 0 R_7))
;
;  43     (C_41 ()
;  44       (LET* (((V_46)     (CONTENTS UNIT_0 UNIT '2))
;  45              ((V_56)     (V_46 I_9 R_7))
;  46              ((V_44)     (CONTENTS UNIT_0 UNIT '1))
;  47              ((V_54)     (V_44 I_9 '1))
;  48              ((V_52)     (CONTENTS LOOP_73 CELL '0)))
;             (V_52 1 C_33 V_54 V_56)))

; What it means:

; Variables    `<name>_<id>'         V_61
; Primops      `<primop name>'       CONTENTS
; Lambdas      `^<self variable>'    ^F_34
; Literals     `'<value>'            '0

;  35   (F_14 (C_13 N_1)
; This is the header for a lambda node.  `35' is the object hash of the node.
; `F_14' is the LAMBDA-NAME and LAMBDA-ID, `(C_13 N_1)' is the variable list.  The
; start of this line (not counting the object hash) is indented one column
; more than the start of the lexically superior lambda.

;  36     (LET* (((LOOP_73)  (CONS CELL '0))
;  37            (()         (SET-CONTENTS LOOP_73 CELL '0 ^F_34))
;  38            ((V_77)     (CONTENTS LOOP_73 CELL '0)))
;           (V_77 1 C_13 N_1 '1)))
; This is the body of the lambda.  It is a block consisting of three simple
; calls and then a tail recursive call.  The simple calls are in the form
; of a LET* that allows multiple value returns.  The actual body of the
; lambda is the call `(CONS CELL '0)'.  The continuation to this call is
; a lambda node `(LAMBDA (LOOP_73) (SET-CONTENTS ...))'.  `36' is the
; object hash of this continuation lambda.
;    After the block any lambdas in the block are printed.  This lambda is
; followed by `F_34'.

; (PP-CPS node . port)
;---------------------------------------------------------------------------
; Print CPS node tree in linear form.  Port defaults to the current output port.
; This just dispatches on the type of NODE.

(define (pp-cps node . port)
  (let* ((port (if (null? port) (current-output-port) (car port)))
	 (port (if (current-column port)
		   port
		   (make-tracking-output-port port))))
    (set! *rereadable?* #f)
    (cond ((lambda-node? node)
           (pp-cps-lambda node 4 port))
          ((call-node? node)
           (write-non-simple-call node port))
          (else
           (write-node-value node port)))
    (newline port)
    ((structure-ref i/o force-output) port)))

(define (rereadable-pp-cps node port)
  (set! *rereadable?* #t)
  (pp-cps-lambda node 4 port)
  (values))

(define (indent port count)
  (let ((count (cond ((<= (current-column port) count)
		      (- count (current-column port)))
		     (else
		      (newline port)
		      count))))
    (do ((count count (- count 1)))
	((>= 0 count))
      (writec port #\space))))
		      
(define *rereadable?* #f)
(define *next-pp-id* 0)

(define (reset-pp-cps)
  (set! *next-pp-id* 0))

(define (next-pp-id)
  (let ((id *next-pp-id*))
    (set! *next-pp-id* (+ *next-pp-id* 1))
    id))

; Print a lambda node by printing its identifiers, then its call, and finally
; any other lambdas that it includes.

(define (pp-cps-lambda node indent-to port)
  (format port "~&~%")
  (cond ((not *rereadable?*)
	 (node-hash node)
	 (format port "~D" (lambda-id node))))
  (indent port indent-to)
  (write-lambda-header node port)
  (let ((internal (pp-cps-body (lambda-body node) indent-to port)))
    (writec port #\))
    (for-each (lambda (n)
		(pp-cps-lambda n (+ indent-to 1) port))
	      internal)))

(define (write-lambda-header node port)
  (writec port '#\()
  (writec port (case (lambda-type node)
                 ((proc known-proc) #\P)
                 ((cont)            #\C)
                 ((jump)            #\J)
                 ((escape)          #\E)))
  (writec port #\space)
  (print-lambda-name node port)
  (writec port #\space)
  (write-lambda-vars node port))

(define (write-lambda-vars node port)
  (let ((vars (lambda-variables node)))
    (cond ((not (null? vars))
           (writec port '#\()
           (print-variable-name (car vars) port)
           (do ((v (cdr vars) (cdr v)))
               ((null? v))
             (writec port '#\space)
             (print-variable-name (car v) port))
           (writec port '#\)))
          (else
           (format port "()")))))

; Print the body of a lambda node.  A simple call is one that has exactly
; one exit.  They and calls to lambda nodes are printed as a LET*.

(define (pp-cps-body call indent-to port)
  (newline port)
  (cond ((or (simple-call? call)
             (let-call? call))
         (write-let* call indent-to port))
        (else
         (indent port (+ '2 indent-to))
         (write-non-simple-call call port))))

; Write out a series of calls as a LET*.  The LET* ends when a call is reached
; that is neither a simple call or a call to a lambda.

(define (write-let* call indent-to port)
  (cond ((not *rereadable?*)
	 (node-hash (call-arg call 0))
	 (format port "~D" (lambda-id (call-arg call '0)))))
  (indent port (+ '2 indent-to))
  (writec port '#\()
  (format port "LET* ")
  (writec port '#\()
  (let loop ((call (next-call call))
	     (ns (write-simple-call call indent-to port)))
    (cond ((or (simple-call? call)
               (let-call? call))
           (newline port)
	   (cond ((not *rereadable?*)
		  (format port "~D" (lambda-id (call-arg call '0)))
		  (node-hash (call-arg call 0))))
           (indent port (+ '9 indent-to))
           (loop (next-call call)
                 (append (write-simple-call call indent-to port) ns)))
          (else
           (writec port '#\))
           (newline port)
           (indent port (+ '4 indent-to))
           (let ((ns (append (write-non-simple-call call port) ns)))
             (writec port '#\))
             ns)))))

(define (simple-call? call)
  (and (= '1 (call-exits call))
       (not (lambda-block (call-arg call 0)))))

(define (let-call? call)
  (calls-this-primop? call 'let))

; Get the call that follows CALL in a LET*.

(define (next-call call)
  (lambda-body (call-arg call '0)))

; Write out one line of a LET*.

(define (write-simple-call call indent-to port)
  (if (let-call? call)
      (write-let-call call indent-to port)
      (really-write-simple-call call indent-to port)))

; Write the variables bound by the continuation and then the primop and
; non-continuation arguments of the call.

(define (really-write-simple-call call indent-to port)
  (writec port '#\()
  (write-lambda-vars (call-arg call '0) port)
  (indent port (+ indent-to '21))
  (writec port '#\()
  (format port "~S" (primop-id (call-primop call)))
  (write-call-args call '1 port)
  (writec port '#\))
  (find-lambda-nodes call 1))

; Write the variables of the lambda and then the values of the arguments.

(define (write-let-call call indent-to port)
  (writec port '#\()
  (write-lambda-vars (call-arg call '0) port)
  (cond ((= '1 (vector-length (call-args call)))
         (writec port '#\))
         '())
        (else
	 (writec port #\*)
         (indent port (+ indent-to '21))
         (write-node-value (call-arg call '1) port)
         (write-call-args call '2 port)
	 (find-lambda-nodes call 1))))

(define (find-lambda-nodes call start)
  (reverse (let label ((call call) (start start) (ls '()))
	     (do ((i start (+ i 1))
		  (ls ls (let ((arg (call-arg call i)))
			   (cond ((call-node? arg)
				  (label arg 0 ls))
				 ((lambda-node? arg)
				  (cons arg ls))
				 (else ls)))))
		 ((>= i (call-arg-count call))
		  ls)))))

; Write out a call that ends a LET* block.

(define (write-non-simple-call call port)
  (writec port '#\()
  (format port "~A ~D" (primop-id (call-primop call)) (call-exits call))
  (write-call-args call '0 port)
  (find-lambda-nodes call 0))

; Write out the arguments of CALL starting with START.

(define (write-call-args call start port)
  (let* ((vec (call-args call))
         (len (vector-length vec)))
    (do ((i start (+ i '1)))
        ((>= i len))
      (writec port '#\space)
      (write-node-value (vector-ref vec i) port))
    (writec port '#\))))

; Print out a literal value.

(define (cps-print-literal value port)
  (format port "'~S" value))

; Dispatch on the type of NODE to get the appropriate printing method.

(define (write-node-value node port)
  (cond ((not (node? node))
         (format port "{not a node}"))
        ((lambda-node? node)
         (writec port '#\^)
         (print-lambda-name node port))
	((call-node? node)
	 (format port "(~S" (primop-id (call-primop node)))
	 (write-call-args node '0 port))
        ((literal-node? node)
         (cps-print-literal (literal-value node) port))
        ((reference-node? node)
         (print-variable-name (reference-variable node) port))
        (else
         (bug "WRITE-NODE-VALUE got funny node ~S" node))))

; Printing variables and lambda nodes

; #T if variables are supposed to print as the name of the register containing
; them instead of their name.

(define *pp-register-names?* '#f)

; A whole bunch of different entry points for printing variables in slightly
; different ways.

(define (print-variable-name var port)
  (cond ((not var)
         (format port "#f"))
;        ((and *pp-register-names?*
;              (reg? (variable-register var)))
;         (format port "~S" (reg-name (variable-register var))))
        (else
	 (let ((id (cond ((not *rereadable?*)
			  (variable-id var))
			 ((variable-flag var)
			  => identity)
			 (else
			  (let ((id (next-pp-id)))
			    (set-variable-flag! var id)
			    id)))))
	   (format port "~S_~S" (variable-name var) id)))))

; Same as the above without the check for a register.

(define (print-variable-plain-name var port)
  (cond ((not var)
         (format port "#f"))
        (else
         (format port "~S_~D" (variable-name var) (variable-id var)))))

; Return the name as a string.

(define (variable-print-name var)
  (print-variable-name var '#f))

; Return the name as a symbol.

(define (variable-unique-name var)
  (string->symbol (variable-print-name var)))

; Printing lambda-nodes as variables

(define (print-lambda-name lnode port)
  (let ((id (cond ((not *rereadable?*)
		    (lambda-id lnode))
		  ((node-flag lnode)
		   => identity)
		  (else
		   (let ((id (next-pp-id)))
		     (set-node-flag! lnode id)
		     id)))))
    (format port "~S_~D" (lambda-name lnode) id)))

(define (lambda-print-name lnode)
  (print-lambda-name lnode '#f))

(define (lambda-unique-name lnode)
  (string->symbol (lambda-print-name lnode)))





