; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.

; Byte-code assembler (Richard's version)

; (lap <spec> <insts>*)
; <spec> ::= <identifier> | (<identifier> <arg>*)
; <arg>  ::= <identifier>
; <inst> ::= (<op-code> . <operands>) |
;            <label> |
;            (global <identifier>) |
;            (set-global! <identifier>) |
;            (local <identifer>) |
;            (set-local! <identifier>) |
;            (literal <anything>) | (literal (quote <anything>)) |
;            (computed-goto <label>* <default-label>)  ; not yet implemented
; <operand> ::= <number> | <label> | <stob-name> |
;               (lap <spec> <insts>*) ; only where a template is expected
;              
; If any <arg>'s are specified, you have to do a MAKE-ENV to get LOCAL and
; SET-LOCAL! to work properly (the assembler assumes that you are using
; environments in the same way as the byte-code compiler).
;
; QUOTE is optional for literals, unless the value is itself quoted.
;
; The assembler uses opcode-arg-specs to check the number and type of arguments
; to the opcodes.

(define-compilator (list 'lap syntax-type)
  (lambda (node cenv depth cont)
    (let* ((exp (node-form node))
	   (template (compile-lap (cadr exp) (cddr exp) cenv)))
      (fixup-template-refs! template)
      (deliver-value (instruction-with-literal op/closure template)
		     cont))))

(define template-marker (cons #f #f))

(define (make-template-marker name)
  (cons template-marker name))

(define (template-marker? x)
  (and (pair? x)
       (eq? (car x) template-marker)))

(define template-marker-name cdr)

(define (fixup-template-refs! template)
  (let ((templates '()))
    (let label ((template template))
      (if (symbol? (template-info template))
	  (set! templates (cons (cons (template-info template) template)
				templates)))
      (do ((i 0 (+ i 1)))
	  ((>= i (template-length template)))
	(if (template? (template-ref template i))
	    (label (template-ref template i)))))
    (let label ((template template))
      (do ((i 0 (+ i 1)))
	  ((>= i (template-length template)))
	(let ((x (template-ref template i)))
	  (cond ((template? x)
		 (label x))
		((not (template-marker? x)))
		((assq (template-marker-name x) templates)
		 => (lambda (t)
		      (template-set! template i (cdr t))))
		(else
		 (error "no template of this name available"
			(template-marker-name x)))))))))

(define (compile-lap spec insts cenv)
  (let ((id (if (pair? spec) (car spec) spec))
	(cenv (if (pair? spec) (bind-vars (cdr spec) cenv) cenv)))
    (segment->template (really-compile-lap insts cenv) id 0)))
    
; Assemble each instruction, keeping track of which ones use labels.
; STUFF is a list of lists of the form (<inst> <offset> . <preceding-insts>)
; which indicates that <inst> uses a label, the it ends at <offset>, and is
; preceded by <preceding-insts>.

(define (really-compile-lap insts cenv)
  (let loop ((insts insts) (segments '()) (stuff '()) (offset 0) (labels '()))
    (cond ((null? insts)
	   (fixup-lap-labels segments stuff labels))
	  ((pair? (car insts))
	   (call-with-values
	    (lambda () (assemble-instruction (car insts) cenv))
	    (lambda (segment label-use?)
	      (let ((offset (+ offset (segment-size segment))))
		(if label-use?
		    (loop (cdr insts)
			  '()
			  `((,(car insts) ,offset . ,segments) . ,stuff)
			  offset
			  labels)
		    (loop (cdr insts)
			  (cons segment segments)
			  stuff
			  offset
			  labels))))))
	  ((or (symbol? (car insts))
	       (integer? (car insts)))
	   (loop (cdr insts) segments stuff offset
		 (cons (cons (car insts) offset) labels)))
	  (else
	   (error "bad LAP instruction" (car insts))))))

; Reassemble the instruction at the beginning of each STUFF list to resolve
; the label reference and glue everything together using SEQUENTIALLY.  The
; label code assumes that the machine calculates the label from the end of
; the instruction.

(define (fixup-lap-labels segments stuff labels)
  (let loop ((stuff stuff) (segment (apply sequentially (reverse segments))))
    (if (null? stuff)
	segment
	(let* ((data (car stuff))
	       (inst (car data))
	       (offset (cadr data))
	       (segments (cddr data)))
	  (loop (cdr stuff)
		(sequentially (apply sequentially (reverse segments))
			      (reassemble-instruction inst offset labels)
			      segment))))))

; This returns two values, the assembled instruction and a flag indicating
; whether or not the instruction used a label.

(define (assemble-instruction inst cenv)
  (really-assemble-instruction inst cenv (lambda (label) (values 0 0))))

; Same as the above, except that labels are resolved and no flag is returned.

(define (reassemble-instruction inst offset labels)
  (call-with-values
   (lambda ()
     (really-assemble-instruction inst #f (resolve-label offset labels)))
   (lambda (inst ignore)
     inst)))

(define (resolve-label offset labels)
  (lambda (label)
    (cond ((assoc label labels)
	   => (lambda (p)
		(let ((delta (- (cdr p) offset)))
		  (values (quotient delta byte-limit)
			  (remainder delta byte-limit)))))
	  (else
	   (error "LAP label is not defined" label)))))

(define (really-assemble-instruction inst cenv labels)
  (let ((opname (car inst))
	(args (cdr inst)))
    (cond ((assemble-special-op opname args cenv)
	   => (lambda (inst)
		(values inst #f)))
	  ((name->enumerand opname op)
	   => (lambda (opcode)
		(assemble-general-instruction opcode inst cenv labels)))
	  (else
	   (error "unknown LAP instruction" inst)))))

; The optional ' is optionally stripped off the argument to LITERAL.

(define (assemble-special-op opname args cenv)
  (case opname
    ((literal)
     (let* ((arg (car args))
	    (arg (if (and (pair? arg)
			  (eq? (car arg) 'quote))
		     (cadr arg)
		     arg)))
       (instruction-with-literal op/literal arg)))
    ((global)
     (lap-global #f (car args) cenv))
    ((set-global!)
     (lap-global #t (car args) cenv))
    ((local)
     (if (null? (cdr args))
	 (lap-local (car args) cenv)
	 #f))
    ((set-local!)
     (if (null? (cdr args))
	 (lap-set-local! (car args) cenv)
	 #f))
    (else
     #f)))

(define (lap-global assign? name cenv)
  (let ((binding (lookup cenv name)))
    (if (and (binding? binding) (not (location? (binding-place binding))))
	(error "LAP variable is not global" name)
	(instruction-with-location
	     (if assign? op/set-global! op/global)
	     (get-location binding
			   cenv
			   name
			   (if assign? usual-variable-type value-type))))))

(define (lap-local name cenv)
  (let ((binding (lookup cenv name)))
    (if (and (binding? binding)
	     (pair? (binding-place binding)))
	(let* ((level+over (binding-place binding))
	       (back (- (environment-level cenv)
			(car level+over)))
	       (over (cdr level+over)))
	  (case back
	    ((0) (instruction op/local0 over))
	    ((1) (instruction op/local1 over))
	    ((2) (instruction op/local2 over))
	    (else (instruction op/local back over))))
	(error "LAP local variable is not locally bound" name))))
	  
(define (lap-set-local! name cenv)
  (let ((binding (lookup cenv name)))
    (if (and (binding? binding)
	     (pair? (binding-place binding)))
	(let* ((level+over (binding-place binding))
	       (back (- (environment-level cenv)
			(car level+over)))
	       (over (cdr level+over)))
	  (instruction op/set-local! back over))
	(error "LAP local variable is not locally bound" name))))

; This returns two values, the assembled instruction and a flag indicating
; whether or not the instruction used a label.

(define (assemble-general-instruction opcode inst cenv labels)
  (let ((specs (vector-ref opcode-arg-specs opcode))
	(args (cdr inst))
	(finish (lambda (ops label-use?)
		  (values (apply instruction opcode (reverse ops))
			  label-use?))))
    (let loop ((specs specs) (args args) (ops '()) (label-use? #f))
      (if (null? specs)
	  (finish ops label-use?)
	  (case (car specs)
	    ((index) ; segment.scm allows no other operands if an index is used.
                     ; All non-template indexed operands should have been taken
                     ; care of above (probably should fix arch.scm).
	     (let ((template (if (null? (cdr args))
				 (make-template-marker (car args))
				 (compile-lap (car args) (cdr args) cenv))))
	       (values (instruction-with-final-literal opcode
						       (reverse ops)
						       template)
		       label-use?)))
	    ((offset)
	     (let ((label (check-lap-arg args 'label inst)))
	       (call-with-values
		(lambda () (labels label))
		(lambda (high low)
		  (loop (cdr specs) (cdr args) `(,low ,high . ,ops) #t)))))
	    ((stob)
	     (let ((byte (check-lap-arg args 'stob inst)))
	       (loop (cdr specs) (cdr args) (cons byte ops) label-use?)))
	    ((nargs byte)
	     (let ((byte (check-lap-arg args 'byte inst)))
	       (loop (cdr specs) (cdr args) (cons byte ops) label-use?)))
	    (else
	     (if (or (eq? (car specs) '+)
		     (integer? (car specs)))
		 (finish ops label-use?)
		 (error "unknown opcode argument specification" (car specs)))))))))

; Compiler doesn't provide this so we hack it up.

(define (instruction-with-final-literal opcode operands literal)
  (cond ((null? operands)
	 (instruction-with-literal opcode literal))
	(else
	 (sequentially (apply instruction
			      opcode
			      (reverse (cdr (reverse operands))))
		       (instruction-with-literal (car (reverse operands))
						 literal)))))

; Check that the car of ARGS is an argument of the appropriate type and
; return it.

(define (check-lap-arg args type inst)
  (if (null? args)
      (error "not enough arguments in LAP instruction" inst))
  (let ((arg (car args)))
    (case type
      ((byte)
       (if (integer? arg)
	   arg
	   (error "numeric operand expected in LAP instruction" inst)))
      ((stob)
       (cond ((name->enumerand arg stob)
	      => (lambda (x) x))
	     (else
	      (error "unknown STOB argument in LAP instruction" inst))))
      ((label)
       (cond ((symbol? arg)
	      arg)
	     ((and (pair? arg)
		   (eq? (car arg) '=>))
	      (cadr arg))
	     (else
	      (error "bad label in LAP instruction" inst))))
      (else
       (error "LAP internal error, unknown LAP argument specifier" type)))))
	    
(define byte-limit (expt 2 bits-used-per-byte))

(define op/closure (enum op closure))
(define op/global  (enum op global))
(define op/literal (enum op literal))
(define op/local   (enum op local))
(define op/local0  (enum op local0))
(define op/local1  (enum op local1))
(define op/local2  (enum op local2))
(define op/set-global! (enum op set-global!))
(define op/set-local!  (enum op set-local!))
