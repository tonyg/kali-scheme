; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Byte-code assembler (Richard's version)
;
; This assembler can assemble the output of the disassembler (as long as you
; add the identifier and the list of free names).
;
; (lap <identifier> (<free name> ...) <insts>*)
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
; (<free name> ...) is a list of all names used in GLOBAL and SET-GLOBAL!
; instructions.  These names are required.
;              
; QUOTE is optional for literals, unless the value is itself quoted.
;
; The assembler uses opcode-arg-specs to check the number and type of arguments
; to the opcodes.

; ENV is an a-list mapping names to bindings.

(define-compilator 'lap syntax-type
  (lambda (node level depth cont)
    (let* ((exp (node-form node))
	   (bindings (map (lambda (name-node)
			    (cons (node-form name-node)
				  (node-ref name-node 'binding)))
			  (caddr exp)))
	   (template (compile-lap (cadr exp)
				  (cdddr exp)
				  bindings)))
      (fixup-template-refs! template)
      (deliver-value (instruction-with-literal (enum op closure) template 0)
		     cont))))

;----------------
; To allow for circular templates, templates can be referred to by name
; (the <identifier> in <spec> above).  This code fixes up the references
; after assembly is otherwise complete.
; 
; The first LABEL recursively finds all named templates.  The second
; replaces template markers with the appropriate template.

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

; Marking where a template should be inserted.

(define template-marker (cons #f #f))

(define (make-template-marker name)
  (cons template-marker name))

(define (template-marker? x)
  (and (pair? x)
       (eq? (car x) template-marker)))

(define template-marker-name cdr)

;----------------

(define (compile-lap id insts bindings)
  (segment->template (really-compile-lap insts bindings) id #f #f))
    
; Assemble each instruction, keeping track of which ones use labels.
; STUFF is a list of lists of the form (<inst> <offset> . <preceding-insts>)
; which indicates that <inst> uses a label, that it begins at <offset>, and is
; preceded by <preceding-insts>.

(define (really-compile-lap insts bindings)
  (let loop ((insts insts) (segments '()) (stuff '()) (offset 0) (labels '()))
    (cond ((null? insts)
	   (fixup-lap-labels segments stuff labels))
	  ((pair? (car insts))
	   (call-with-values
	    (lambda ()
	      (assemble-instruction (car insts) bindings))
	    (lambda (segment label-use?)
	      (let ((new-offset (+ offset (segment-size segment))))
		(if label-use?
		    (loop (cdr insts)
			  '()
			  `((,(car insts) ,offset . ,segments) . ,stuff)
			  new-offset
			  labels)
		    (loop (cdr insts)
			  (cons segment segments)
			  stuff
			  new-offset
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

(define (assemble-instruction inst bindings)
  (really-assemble-instruction inst bindings (lambda (label) (values 0 0))))

; Same as the above, except that labels are resolved and no flag is returned.

(define (reassemble-instruction inst offset labels)
  (call-with-values
   (lambda ()
     (really-assemble-instruction inst #f (resolve-label offset labels)))
   (lambda (inst ignore)
     inst)))

; Return the high and low bytes of the distance between OFFSET and LABEL,
; using the known label offsets in LABELS.

(define (resolve-label offset labels)
  (lambda (label)
    (cond ((assoc label labels)
	   => (lambda (p)
		(let ((delta (- (cdr p) offset)))
		  (values (quotient delta byte-limit)
			  (remainder delta byte-limit)))))
	  (else
	   (error "LAP label is not defined" label)))))

;----------------
; Actually do some assembly.  A few opcodes need special handling; most just
; use the argument specifications from the architecture.

(define (really-assemble-instruction inst bindings labels)
  (let ((opname (car inst))
	(args (cdr inst)))
    (cond ((assemble-special-op opname args bindings)
	   => (lambda (inst)
		(values inst #f)))
	  ((name->enumerand opname op)
	   => (lambda (opcode)
		(assemble-general-instruction opcode inst bindings labels)))
	  (else
	   (error "unknown LAP instruction" inst)))))

; The optional ' is optionally stripped off the argument to LITERAL.

(define (assemble-special-op opname args bindings)
  (case opname
    ((literal small-literal)
     (let* ((arg (car args))
	    (arg (if (and (pair? arg)
			  (eq? (car arg) 'quote))
		     (cadr arg)
		     arg)))
       (instruction-with-literal (enum op literal) arg)))
    ((global)
     (lap-global #f (car args) bindings))
    ((set-global!)
     (lap-global #t (car args) bindings))
;    ((local)
;     (if (null? (cdr args))
;         (lap-local (car args) bindings)
;         #f))
;    ((set-local!)
;     (if (null? (cdr args))
;         (lap-set-local! (car args) bindings)
;         #f))
    ((protocol)
     (apply instruction
	    (enum op protocol)
	    (assemble-protocol args)))
    (else
     #f)))

; Lookup NAME in BINDINGS to the location.

(define (lap-global assign? name bindings)
  (let ((binding (assq bindings name)))
    (if (not binding)
	(error "LAP variable is not in free list" name)
	(let ((binding (cdr binding)))
	  (cond ((and (binding?  binding)
		      (pair? (binding-place binding)))
		 (error "LAP variable is not global" name))
		(assign?
		 (instruction-with-location (enum op set-global!)
					    binding
					    name
					    usual-variable-type))
		(else
		 (instruction-with-location (enum op global)
					    binding
					    name
					    value-type)))))))

; This no longer works and I doubt if anyone uses it.  To fix it the
; assembler should fix-up BINDINGS to incorporate LEVEL passed to the
; compilator.

;; Lookup NAME in BINDINGS and pick out the appropriate local op.
;
;(define (lap-local name bindings)
;  (let ((binding (lookup bindings name)))
;    (if (and (binding? binding)
;             (pair? (binding-place binding)))
;        (let* ((level+over (binding-place binding))
;               (back (- (environment-level bindings)
;                        (car level+over)))
;               (over (cdr level+over)))
;          (case back
;            ((0) (instruction (enum op local0) over))
;            ((1) (instruction (enum op local1) over))
;            ((2) (instruction (enum op local2) over))
;            (else (instruction (enum op local) back over))))
;        (error "LAP local variable is not locally bound" name))))
;          
;; Same thing, except that there is only one opcode.
;
;(define (lap-set-local! name bindings)
;  (let ((binding (lookup bindings name)))
;    (if (and (binding? binding)
;             (pair? (binding-place binding)))
;        (let* ((level+over (binding-place binding))
;               (back (- (environment-level bindings)
;                        (car level+over)))
;               (over (cdr level+over)))
;          (instruction (enum op set-local!)
;                       back
;                       (quotient over byte-limit)
;                       (remainder over byte-limit)))
;        (error "LAP local variable is not locally bound" name))))

; Assembling protocols.

(define (assemble-protocol args)
  (if (integer? (car args))
      (let ((count (car args)))
	(cond ((not (null? (cdr args)))
	       (if (not (eq? (cadr args) '+))
		   (error "unknown assembly protocol" args))
	       (list two-byte-nargs+list-protocol
		     (quotient count byte-limit)
		     (remainder count byte-limit)))
	      ((<= count maximum-stack-args)
	       (list count))
	      (else
	       (list two-byte-nargs-protocol
		     (quotient count byte-limit)
		     (remainder count byte-limit)))))
      (case (car args)
	((args+nargs)
	 (cons args+nargs-protocol (cdr args)))
	((nary-dispatch)
	 (cons nary-dispatch-protocol
	       (parse-nary-dispatch (cdr args))))
	((big-stack)
	 (error "can't assemble big-stack protocol"))
	(else
	 (error "unknown assembly protocol" args)))))

; This is fairly bogus, because it uses the targets as addresses instead
; of treating them as labels.  Fixing this is too much work, seeing as
; no one is likely to use it.

(define (parse-nary-dispatch targets)
  (let ((results (vector 0 0 0 0)))
    (warn "LAP compiler treats nary-dispatch targets as addresses, not as labels.")
    (for-each (lambda (target)
		(if (and (pair? target)
			 (pair? (cdr target))
			 (pair? (cddr target))
			 (or (eq? (car target) '>2)
			     (and (integer? (car target))
				  (<= 0 (car target) 2)))
			 (eq? (cadr target) '=>)
			 (integer? (caddr target)))
		    (vector-set! results
				 (if (eq? (car target) '>2)
				     3
				     (car target))
				 (caddr target))
		    (error "bad nary-dispatch label in LAP" target)))
	      targets)
    (vector->list results)))

;----------------
; This returns two values, the assembled instruction and a flag indicating
; whether or not the instruction used a label.

(define (assemble-general-instruction opcode inst bindings labels)
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
				 (compile-lap (car args) (cdr args) bindings))))
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
	    ((two-bytes)
	     (let ((number (check-lap-arg args 'byte inst)))
	       (loop (cdr specs) (cdr args)
		     `(,(remainder number byte-limit)
		       ,(quotient number byte-limit)
		       . ,ops)
		     label-use?)))
	    ((junk)
	     (loop (cdr specs) args (cons 0 ops) label-use?))
	    (else
	     (if (or (eq? (car specs) '+)
		     (integer? (car specs)))
		 (finish ops label-use?)
		 (error "LAP internal error, unknown opcode argument specification" (car specs)))))))))

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
	    
