; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.

; Printing the node tree as Scheme code

; Sample output:

;  (LAMBDA (C_11 UNIT_0)
;    (SET-CONTENTS 1 C_11 UNIT_0 UNIT '0
;      (LAMBDA (C_13 N_1)
;        (LET ((LOOP_73 (CONS CELL '0)))
;          (SET-CONTENTS LOOP_73 CELL '0
;            (LAMBDA (C_33 I_9 R_7)
;              (LET* ((V_61 (CONTENTS UNIT_0 UNIT '3))
;                     (V_63 (V_61 I_9 '0)))
;                (IF (TRUE? V_63)
;                    (C_33 0 R_7)
;                    (LET* ((V_46 (CONTENTS UNIT_0 UNIT '2))
;                           (V_56 (V_46 I_9 R_7))
;                           (V_44 (CONTENTS UNIT_0 UNIT '1))
;                           (V_54 (V_44 I_9 '1))
;                           (V_52 (CONTENTS LOOP_73 CELL '0)))
;                      (V_52 1 C_33 V_54 V_56))))))
;          (LET ((V_77 (CONTENTS LOOP_73 CELL '0)))
;            (V_77 1 C_13 N_1 '1))))))

; (CPS->SCHEME node port)
;---------------------------------------------------------------------------
; Print CPS node tree in linear form.  This just dispatches on the type of NODE.

(define (cps->scheme node port)
  (set! *next-pp-id* 0)
  (let* ((port (if (current-column port)
		   port
		   (make-tracking-output-port port))))
    (cond ((lambda-node? node)
           (pp-cps-lambda node 4 port))
          ((call-node? node)
           (write-non-simple-call node port))
          (else
           (write-node-value node port)))
    (newline port)))

(define (indent port count)
  (let ((count (cond ((<= (current-column port) count)
		      (- count (current-column port)))
		     (else
		      (newline port)
		      count))))
    (do ((count count (- count 1)))
	((>= 0 count))
      (writec port #\space))))
		      
(define *next-pp-id* 0)

(define (next-pp-id)
  (let ((id *next-pp-id*))
    (set! *next-pp-id* (+ *next-pp-id* 1))
    id))

; Print a lambda node by printing its identifiers, then its call, and finally
; any other lambdas that it includes.

(define (pp-cps-lambda node indent-to port)
  (format port "~&~%")
  (indent port indent-to)
  (display "(lambda " port)
  (write-lambda-vars node port)
  (pp-cps-body (lambda-body node) indent-to port)
  (writec port #\)))

(define (write-lambda-vars node port)
  (let ((vars (if (proc-lambda? node)
		  (cdr (lambda-variables node))  ; ignore cont var
		  (lambda-variables node))))
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
  (indent indent-to port)
  (cond ((table-ref primop-print-table (primop-id (call-primop call)))
	 => (lambda (proc)
	      (proc call indent-to port)))
	((not (= 1 (call-exits call)))
	 (bug "no printer for primop in ~S" call))
	(else
	 (let* ((cont (call-arg call 0))
		(cont-vars (lambda-variables cont)))
	   (cond ((every? unused? cont-vars)
		  (print-call call 1 port)
		  (pp-cps-body (lambda-body (call-arg call 0))))
		 ((null? (cdr cont-vars))
		  (display "(let ((" port)
		  (print-variable-name (car cont-vars))
		  (display " " port)
		  (print-call call 1 port)
		  (display "))" port)
		  (pp-cps-body call (+ indent-to 2) port))
		 (else
		  (display "(receive " port)
		  (write-lambda-vars cont port)
		  (newline port)
		  (indent (+ indent-to 4) port)
		  (print-call call 1 port)
		  (pp-cps-body call (+ indent-to 2) port)))))))
		  
(define (print-call call start-arg port)
  (display "(" port)
  (display (primop-id (call-primop call)) port)
  (print-args call start-arg port)
  (display ")" port))
	 
(define (print-args call start-arg port)
  (do ((i start-arg (+ i 1)))
      ((= i (call-arg-count call)))
    (display " " port)
    (print-arg (call-arg call i) port)))

(define (print-arg arg port)
  (cond ((lambda-node? node)
	 
	((call-node? node)
	 (format port "(~S" (primop-id (call-primop node)))
	 (print-args node 0 port))
        ((literal-node? node)
         (cps-print-literal (literal-value node) port))
        ((reference-node? node)
         (print-variable-name (reference-variable node) port))
        (else
         (bug "WRITE-NODE-VALUE got funny node ~S" node))))



  (cond ((or (simple-call? call)
             (let-call? call))
         (write-let* call indent-to port))
        (else
         (indent port (+ '2 indent-to))
         (write-non-simple-call call port))))

; Write out a series of calls as a LET*.  The LET* ends when a call is reached
; that is neither a simple call or a call to a lambda.

(define (write-let* call indent-to port)
  (indent port (+ '2 indent-to))
  (writec port '#\()
  (format port "LET* ")
  (writec port '#\()
  (let loop ((call (next-call call))
	     (ns (write-simple-call call indent-to port)))
    (cond ((or (simple-call? call)
               (let-call? call))
           (newline port)
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
	 (let ((id (cond ((variable-flag var)
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
  (let ((id (cond ((node-flag lnode)
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





