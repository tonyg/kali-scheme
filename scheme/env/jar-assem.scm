; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file assem.scm.

;;;; Assembler

; Courtesy John Ramsdell.

; LAP syntax is much like that of the output of the disassembler except
; that global and set-global! take a symbol as an argument, 
; statements may be labeled, and jump, jump-if-false, and make-cont 
; may make a forward reference to a label to give an offset.
;
; Example: a translation of (define (dog) (if x 0 1)).
; (define dog
;   (lap dog
;     (check-nargs= 0)
;     (global x)
;     (jump-if-false 8)
;     (literal '0)
;   8 (jump out)
;     (literal '1)
; out (return)))

(define-compilator '(lap syntax)
  (let ((op/closure (enum op closure)))
    (lambda (node cenv depth cont)
      (let ((exp (node-form node)))
	(deliver-value
	 (instruction-with-template op/closure
				    (compile-lap (cddr exp) cenv)
				    (cadr exp))
	 cont)))))

; Assembler label environments are simply a-lists.
(define assembler-empty-env '())
(define (assembler-extend sym val env) (cons (cons sym val) env))
(define (assembler-lookup sym env)
  (let ((val (assv sym env)))
    (if (pair? val) (cdr val) #f)))

(define (compile-lap instruction-list cenv)
  (assemble instruction-list
	    assembler-empty-env
	    cenv))

; ASSEMBLE returns a segment.

(define (assemble instruction-list lenv cenv)
  (if (null? instruction-list)
      (sequentially)
      (let ((instr (car instruction-list))
	    (instruction-list (cdr instruction-list)))
	(cond ((pair? instr)		; Instruction
	       (sequentially
		(assemble-instruction instr lenv cenv)
		(assemble instruction-list
			  lenv
			  cenv)))
	      ((or (symbol? instr)	; Label
		   (number? instr))
	       (let ((label (make-label)))
		 (attach-label
		  label
		  (assemble instruction-list
			    (assembler-extend instr label lenv)
			    cenv))))
	      (else (error "invalid instruction" instr))))))

; ASSEMBLE-INSTRUCTION returns a segment.

(define (assemble-instruction instr lenv cenv)
  (let* ((opcode (name->enumerand (car instr) op))
	 (arg-specs (vector-ref opcode-arg-specs opcode)))
    (cond ((or (not (pair? arg-specs))
	       (not (pair? (cdr instr))))
	   (instruction opcode))
	  ((eq? (car arg-specs) 'index)
	   (assemble-instruction-with-index opcode arg-specs (cdr instr) cenv))
	  ((eq? (car arg-specs) 'offset)
	   (let ((operand (cadr instr)))
	     (apply instruction-using-label
		    opcode
		    (let ((probe (assembler-lookup operand lenv)))
		      (if probe
			  probe
			  (begin
			    (syntax-error "can't find forward label reference"
					  operand)
			    empty-segment)))
		    (assemble-operands (cddr instr) arg-specs))))
	  (else
	   (apply instruction
		  opcode
		  (assemble-operands (cdr instr) arg-specs))))))

; <index> ::= (quote <datum>) | (lap <name> <instr>) | <name>

(define (assemble-instruction-with-index opcode arg-specs operands cenv)
  (let ((operand (car operands)))
    (if (pair? operand)
	(case (car operand)
	  ((quote)
	   (instruction-with-literal opcode
				     (cadr operand)))
	  ((lap)
	   (instruction-with-template opcode
				      (compile-lap (cddr operand))
				      (cadr operand)))
	  (else
	   (syntax-error "invalid index operand" operand)
	   empty-segment))
	;; Top-level variable reference
	(instruction-with-location
	 opcode
	 (get-location (lookup cenv operand)
		       cenv
		       operand
		       value-type)))))

(define (assemble-operands operands arg-specs)
  (map (lambda (operand arg-spec)
	 (case arg-spec
	   ((stob) (or (name->enumerand operand stob)
		       (error "unknown stored object type" operand)))
	   ((byte nargs) operand)
	   (else (error "unknown operand type" operand arg-spec))))
       operands
       arg-specs))

