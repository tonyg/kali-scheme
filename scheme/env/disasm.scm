; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; This is file assem.scm.

;;;; Disassembler

; This defines a command processor command
;      dis <expression>
; that evaluates <expression> to obtain a procedure or lambda-expression,
; which is then disassembled.

; Needs:
;   template? template-name template-code
;   closure? closure-template
;   code-vector-...
;   location-name

; The assembly language is designed to be rereadable.  See env/assem.scm.

(define-command-syntax 'dis "[<exp>]" "disassemble procedure"
  '(&opt expression))

; The command.  The thing to be disassembled defaults to the focus object (##).

(define (dis . maybe-exp)
  (disassemble (if (null? maybe-exp)
		   (focus-object)
		   (eval (car maybe-exp) (environment-for-commands)))))

(define (disassemble obj)
  (really-disassemble (coerce-to-template obj) 0)
  (newline))

; Instruction loop.  WRITE-INSTRUCTION returns the new program counter.

(define (really-disassemble tem level)
  (write (template-name tem))
  (let ((length (template-code-length (template-code tem))))
    (let loop ((pc 0))
      (if (< pc length)
	  (loop (write-instruction tem pc level #t))))))

; The protocol used for procedures that require extra stack space uses three
; bytes at the end of the code vector.

(define (template-code-length code)
  (if (and (= (enum op protocol)
	      (code-vector-ref code 0))
	   (= big-stack-protocol
	      (code-vector-ref code 1)))
      (- (code-vector-length code) 3)
      (code-vector-length code)))

; Write out the intstruction at PC in TEMPLATE.  LEVEL is the nesting depth
; of TEMPLATE.  If WRITE-SUB-TEMPLATES? is false we don't write out any
; templates found.
;
; Special handling is required for the few instructions that do not use a
; fixed number of code-stream arguments.

(define (write-instruction template pc level write-sub-templates?)
  (let* ((code (template-code template))
         (opcode (code-vector-ref code pc)))
    (newline-indent (* level 3))
    (write-pc pc)
    (display " (")
    (write (enumerand->name opcode op))
    (let ((pc (cond ((= opcode (enum op computed-goto))
 		     (display-computed-goto pc code))
 		    ((= opcode (enum op flat-closure))
 		     (display-flat-lambda (+ pc 1) code template
 					  level write-sub-templates?))
		    ((= opcode (enum op protocol))
		     (display-protocol (code-vector-ref code (+ pc 1)) pc code))
 		    (else
 		     (print-opcode-args opcode pc code template
 					level write-sub-templates?)))))
      (display #\))
      pc)))

; Write out all of the branches of a computed goto.

(define (display-computed-goto start-pc code)
  (display #\space)
  (let ((count (code-vector-ref code (+ start-pc 1))))
    (write count)
    (do ((pc (+ start-pc 2) (+ pc 2))
	 (count count (- count 1)))
	((= count 0) pc)
      (display #\space)
      (write `(=> ,(+ start-pc (get-offset pc code) 2))))))

; Write out the environment specs for the flat-lambda opcode.

(define (display-flat-lambda pc code template level write-sub-templates?)
  (let* ((count (code-vector-ref code pc))
 	 (pc (do ((i 0 (+ i 1))
 		  (pc (+ pc 1) (+ pc 2)))
 		 ((= i count) pc)
 	       (display #\space)
 	       (display (cons (code-vector-ref code pc)
 			      (code-vector-ref code (+ pc 1)))))))
    (let ((template (template-ref template (code-vector-ref code pc))))
      (if write-sub-templates?
 	  (really-disassemble template (+ level 1) #t)
 	  (display "...")))
    (+ pc 1)))

; Display a protocol, returning the number of bytes of instruction stream that
; were consumed.

(define (display-protocol protocol pc code)
  (display #\space)
  (+ pc (cond ((<= protocol maximum-stack-args)
	       (display protocol)
	       2)
	      ((= protocol two-byte-nargs-protocol)
	       (display (get-offset (+ pc 2) code))
	       4)
	      ((= protocol two-byte-nargs+list-protocol)
	       (display (get-offset (+ pc 2) code))
	       (display " +")
	       4)
	      ((= protocol args+nargs-protocol)
	       (display "args+nargs")
	       3)
	      ((= protocol nary-dispatch-protocol)
	       (display "nary-dispatch")
	       (do ((i 0 (+ i 1)))
		   ((= i 4))
		 (let ((offset (code-vector-ref code (+ pc 2 i))))
		   (if (not (= offset 0))
		       (begin
			 (display #\space)
			 (display (list (if (= i 3) '>2 i)
					'=>
					(+ pc offset)))))))
	       6)
	      ((= protocol big-stack-protocol)
	       (display "big-stack")
	       (let ((size (display-protocol (code-vector-ref code
							      (- (code-vector-length code)
								 3))
					     pc
					     code)))
		 (display #\space)
		 (display (get-offset (- (code-vector-length code) 2) code))
		 size))
	      (else
	       (error "unknown protocol" protocol)))))

; Generic opcode argument printer.

(define (print-opcode-args op start-pc code template level write-templates?)
  (let ((specs (vector-ref opcode-arg-specs op)))
    (let loop ((specs specs) (pc (+ start-pc 1)))
      (cond ((or (null? specs)
 		 (= 0 (arg-spec-size (car specs) pc code)))
  	     pc)
  	    ((eq? (car specs) 'junk)  ; avoid printing a space
	     (loop (cdr specs) (+ pc 1)))
	    (else
  	     (display #\space)
  	     (print-opcode-arg specs pc start-pc code template level write-templates?)
 	     (loop (cdr specs) (+ pc (arg-spec-size (car specs) pc code))))))))
  
; The number of bytes required by an argument.

(define (arg-spec-size spec pc code)
  (case spec
    ((nargs byte stob junk) 1)
    ((offset index small-index two-bytes) 2)
    ((env-data) (+ 1 (* 2 (code-vector-ref code pc))))
    (else 0)))

; Print out the particular type of argument.

(define (print-opcode-arg specs pc start-pc code template level write-templates?)
  (case (car specs)
    ((nargs byte)
     (write (code-vector-ref code pc)))
    ((two-bytes)
     (write (get-offset pc code)))
    ((index)
     (let ((thing (template-ref template (get-offset pc code))))
       (write-literal-thing thing level write-templates?)))
    ((small-index)
     (let ((thing (template-ref template (code-vector-ref code pc))))
       (write-literal-thing thing level write-templates?)))
    ((offset)
     (write `(=> ,(+ start-pc (get-offset pc code)))))
    ((stob)
     (write (enumerand->name (code-vector-ref code pc) stob)))
    ((env-data)
     (let ((nobjects (code-vector-ref code pc)))
       (let loop ((offset (+ pc 1)) (n nobjects))
 	 (cond ((not (zero? n))
 		(display (list (code-vector-ref code offset)
 			       (code-vector-ref code (+ offset 1))))
 		(if (not (= n 1))
 		    (write-char #\space))
 		(loop (+ offset 2) (- n 1)))))))))

(define (write-literal-thing thing level write-templates?)
  (cond ((location? thing)
	 (write (or (location-name thing)
		    `(location ,(location-id thing)))))
	((not (template? thing))
	 (display #\')
	 (write thing))
	(write-templates?
	 (really-disassemble thing (+ level 1)))
	(else
	 (display "..."))))

;----------------
; Utilities.

; Turn OBJ into a template, if possible.

(define (coerce-to-template obj)
  (cond ((template? obj) obj)
	((closure? obj) (closure-template obj))
	((continuation? obj) (continuation-template obj))
	(else (error "expected a procedure or continuation" obj))))

; Fetch the two-byte value at PC in CODE.

(define (get-offset pc code)
  (+ (* (code-vector-ref code pc)
	byte-limit)
     (code-vector-ref code (+ pc 1))))

; Indenting and aligning the program counter.

(define (newline-indent n)
  (newline)
  (do ((i n (- i 1)))
      ((= i 0))
    (display #\space)))

(define (write-pc pc)
  (if (< pc 100) (display " "))
  (if (< pc 10) (display " "))
  (write pc))


