; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file assem.scm.

;;;; Assembler

; Courtesy John Ramsdell.

; This version is intended to be loaded into the Scheme48 system
; environment.  It makes the lap syntax available in the user environment.
;
; lap syntax is much like that of the output of the disassembler except
; that global and set-global! take a symbol as an argument, 
; statements may be labeled, and jump, jump-if-false, and make-cont 
; may make a forward reference to a label to give an offset.  The offset 
; may be given as a number using (+> N) syntax.  [Doesn't work any more --
; JAR 12/92]
;
; Example: a translation of (define (dog) (if x 0 1)).
; (define dog
;   (lap dog
;     (check-nargs= 0)
;     (global x)
;     (jump-if-false (+> 5))
;     (literal '0)
;     (jump out)
;     (literal '1)
; out (return)))

(define-compilator 'lap 'syntax
  (lambda (exp cenv depth cont)
    (deliver-value
     (instruction-with-template op/closure
				(compile-lap (cddr exp) cenv)
				(cadr exp))
     cont)))

;;; Assembler label environments are simply a-lists.
(define assembler-empty-env '())
(define (assembler-extend sym val env) (cons (cons sym val) env))
(define (assembler-lookup sym env)
  (let ((val (assq sym env)))
    (and (pair? val) (cdr val))))

(define (compile-lap instruction-list cenv)
  (assemble (reverse instruction-list)
	    assembler-empty-env
	    (sequentially) ;empty-segment
	    cenv))

(define (assemble rev-instruction-list lenv after cenv)
  (if (null? rev-instruction-list)
      after
      (let ((instr (car rev-instruction-list))
	    (rev-instruction-list (cdr rev-instruction-list)))
	(cond ((pair? instr)		; Instruction
	       (assemble rev-instruction-list
			 lenv
			 (assemble-instruction instr lenv after cenv)
			 cenv))
	      ((symbol? instr)		; Label
	       (assemble rev-instruction-list
			 (assembler-extend instr
					   (segment-size after)
					   lenv)
			 after
			 cenv))
	      (else (error "bad instruction" instr))))))

(define (assemble-instruction instr lenv after cenv)
  (sequentially
   (let ((opcode (name->enumerand (car instr) op)))
     (cond ((= opcode op/closure)
	    (instruction-with-template op/closure
				       (compile-lap (cddr instr))
				       (cadr instr)))
	   ((or (= opcode op/global) (= opcode op/set-global!))
	    (instruction-with-literal opcode
				      (assemble-global-ref (cadr instr))))
	   ((and (pair? (cdr instr))
		 (pair? (cadr instr))
		 (eq? (caadr instr) 'quote))
	    (instruction-with-literal opcode
				      (cadr (cadr instr))))
	   (else
	    (apply instruction
		   opcode
		   (assemble-operands (cdr instr)
				      lenv
				      (segment-size after))))))
   after))

(define (assemble-operands operands lenv following)
  (if (null? operands)
      '()
      (let ((operand (car operands))
	    (after (assemble-operands (cdr operands) lenv following)))
	(cond ((integer? operand) (cons operand after))
	      ((symbol? operand)
	       (let ((offset (assembler-lookup operand lenv)))
		 (if (integer? offset)
		     (let ((offset (- (+ following (length after)) offset)))
		       (cons (quotient offset byte-limit)
			     (cons (remainder offset byte-limit) after)))
		     (error "cannot find forward reference" operand))))
	      (else (error "unknown operand type" operand))))))

;  +> isn't a good R4RS symbol.
;        ((eq? (car operand) '+>)         ; offset
;         (let ((offset (cadr operand)))
;           (cons (quotient offset byte-limit)
;		  (cons (remainder offset byte-limit) after))))

(define (assemble-global-ref exp cenv)
  (if (symbol? exp)
      (let ((den (clookup-variable cenv exp)))
	(if (location? den)
	    den
	    (error "lap: invalid variable reference" exp)))
      (error "invalid variable name" exp)))

(define byte-limit (expt 2 bits-used-per-byte))

(define op/closure (enum op closure))
(define op/global (enum op global))
(define op/set-global! (enum op set-global!))
