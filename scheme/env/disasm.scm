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

(define-command-syntax 'dis "[<exp>]" "disassemble procedure"
  '(&opt expression))

(define (dis . maybe-exp)
  (disassemble (if (null? maybe-exp)
		   (focus-object)
		   (evaluate (car maybe-exp) (environment-for-commands)))))

(define (disassemble obj)
  (really-disassemble (coerce-to-template obj) 0)
  (newline))

(define (really-disassemble tem level)
  (write (template-name tem))
  (let loop ((pc 0))
    (if (< pc (code-vector-length (template-code tem)))
        (loop (write-instruction tem pc level #t)))))

(define (newline-indent n)
  (newline)
  (do ((i n (- i 1)))
      ((= i 0))
    (display #\space)))

(define (write-pc pc)
  (if (< pc 100) (display " "))
  (if (< pc 10) (display " "))
  (write pc))

(define (write-instruction template pc level write-sub-templates?)
  (let* ((code (template-code template))
         (opcode (code-vector-ref code pc)))
    (newline-indent (* level 3))
    (write-pc pc)
    (display " (")
    (write (enumerand->name opcode op))
    (let ((pc (if (= opcode op/computed-goto)
		  (display-computed-goto pc code)
		  (print-opcode-args opcode (+ pc 1) code template
				     level write-sub-templates?))))
      (display #\) )
      pc)))

(define op/computed-goto (enum op computed-goto))

(define (display-computed-goto pc code)
  (display #\space)
  (let ((count (code-vector-ref code (+ pc 1))))
    (write count)
    (do ((pc (+ pc 2) (+ pc 2))
	 (count count (- count 1)))
	((= count 0) pc)
      (display #\space)
      (write `(=> ,(+ pc (+ (+ (* (code-vector-ref code pc)
				  byte-limit)
			       (code-vector-ref code (+ pc 1)))
			    2)))))))

(define (print-opcode-args op pc code template level write-templates?)
  (let ((specs (vector-ref opcode-arg-specs op)))
    (let loop ((specs specs) (pc pc))
      (cond ((or (null? specs)
 		 (= 0 (arg-spec-size (car specs) pc code)))
  	     pc)
  	    (else
  	     (display #\space)
  	     (print-opcode-arg specs pc code template level write-templates?)
 	     (loop (cdr specs) (+ pc (arg-spec-size (car specs) pc code))))))))
  
(define (arg-spec-size spec pc code)
  (case spec
    ((nargs byte stob) 1)
    ((offset index small-index) 2)
    ((env-data) (+ 1 (* 2 (code-vector-ref code pc))))
    (else 0)))

(define (print-opcode-arg specs pc code template level write-templates?)
  (case (car specs)
    ((nargs byte)
     (write (code-vector-ref code pc)))
    ((index)
     (let ((thing (template-ref template (get-offset pc code))))
       (write-literal-thing thing level write-templates?)))
    ((small-index)
     (let ((thing (template-ref template (code-vector-ref code pc))))
       (write-literal-thing thing level write-templates?)))
    ((offset)
     (write `(=> ,(+ pc (+ (get-offset pc code)
 			   (apply + (map (lambda (spec)
 					   (arg-spec-size spec pc code))
 					 specs)))))))
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

(define (get-offset pc code)
  (+ (* (code-vector-ref code pc)
	byte-limit)
     (code-vector-ref code (+ pc 1))))

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

(define byte-limit (expt 2 bits-used-per-byte))


(define (coerce-to-template obj)	;utillity for various commands
  (cond ((template? obj) obj)
	((closure? obj) (closure-template obj))
	((continuation? obj) (continuation-template obj))
	(else (error "expected a procedure or continuation" obj))))
