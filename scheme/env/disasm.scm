; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

;;;; Disassembler

; This will need to track the template's offset.  Drat.

; This defines a command processor command
;      dis <expression>
; that evaluates <expression> to obtain a procedure or lambda-expression,
; which is then disassembled.

; The assembly language is designed to be rereadable.  See env/assem.scm.

(define-command-syntax 'dis "[<exp>]" "disassemble procedure"
  '(&opt expression))

; The command.  The thing to be disassembled defaults to the focus object (##).

(define (dis . maybe-exp)
  (disassemble (if (null? maybe-exp)
		   (focus-object)
		   (eval (car maybe-exp) (environment-for-commands)))))

(define (disassemble obj)
  (really-disassemble (coerce-to-template-or-code obj) 0)
  (newline))

; Instruction loop.  WRITE-INSTRUCTION returns the new program counter.

(define (really-disassemble template-or-code level)
  (let* ((template (if (template? template-or-code)
		       template-or-code
		       #f))
	 (code (if template
		   (template-code template)
		   template-or-code)))
    (if (and template
	     (template-name template))
	(write (template-name template)))
    (let ((length (template-code-length code)))
      (let loop ((pc (display-entry-protocol code level)))
	(if (< pc length)
	    (loop (write-instruction code template pc level #t)))))))

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

(define (write-instruction code template pc level write-sub-templates?)
  (let* ((opcode (code-vector-ref code pc)))
    (show-opcode code pc level)
    (let ((pc (cond ((= opcode (enum op computed-goto))
 		     (display-computed-goto pc code))
 		    ((= opcode (enum op make-flat-env))
 		     (display-flat-env (+ pc 1) code level write-sub-templates?
				       1 code-vector-ref))
 		    ((= opcode (enum op make-big-flat-env))
 		     (display-flat-env (+ pc 1) code level write-sub-templates?
				       2 get-offset))
		    ((= opcode (enum op protocol))
		     (display-protocol (code-vector-ref code (+ pc 1)) pc code))
		    ((= opcode (enum op cont-data))
		     (display-cont-data pc code)
		     (+ pc (get-offset code (+ pc 1))))
		    ((or (= opcode (enum op global))
			 (= opcode (enum op set-global!)))
		     (display-global-reference pc code template))
 		    (else
 		     (print-opcode-args opcode pc code)))))
      (display #\))
      pc)))

(define (show-opcode code pc level)    
  (newline-indent (* level 3))
  (write-pc pc)
  (display " (")
  (write (enumerand->name (code-vector-ref code pc) op)))

(define (display-global-reference pc code template)
  (let ((loc (if template
		 (template-ref template (get-offset code (+ pc 3)))
		 #f)))
    (print-opcode-args (code-vector-ref code pc) pc code)
    (display #\space)
    (cond ((location? loc)
	   (write (or (location-name loc)
		      `(location ,(location-id loc)))))
	  (else
	   (display #\')
	   (write loc)))
    (+ pc 5)))

; Write out all of the branches of a computed goto.

(define (display-computed-goto start-pc code)
  (display #\space)
  (let ((count (code-vector-ref code (+ start-pc 1))))
    (write count)
    (do ((pc (+ start-pc 2) (+ pc 2))
	 (count count (- count 1)))
	((= count 0) pc)
      (display #\space)
      (write `(=> ,(+ start-pc (get-offset code pc) 2))))))

; (enum op make-[big-]flat-env)
; number of vars
; number of closures
; [offset of template in frame
;  offsets of templates in template]
; number of variables in frame (size)
; offsets of vars in frame
; [offset of env in frame
;  number of vars in env
;  offsets of vars in level]*

(define (display-flat-env pc code level write-subs? size fetch)
  (let ((total-count (fetch code pc))
	(closure-count (fetch code (+ pc size))))
    (display #\space)
    (write total-count)
    (let* ((pc (if (< 0 closure-count)
		   (begin
		     (display-flat-env-closures closure-count
						pc code
						level write-subs?
						size fetch)
		     (+ pc
			(* 2 size)		; counts
			size			; template offset
			(* closure-count size))); subtemplates
		   (+ pc (* 2 size))))		; counts
	   (frame-count (fetch code pc))
	   (pc (+ pc size)))
      (display #\space)
      (display (get-offsets code pc size fetch frame-count))
      (let ((pc (+ pc (* frame-count size)))
	    (count (+ closure-count frame-count)))
	(let loop ((pc pc) (count count))
	  (if (< count total-count)
	      (let* ((more (fetch code pc))
		     (offset (fetch code (+ pc size)))
		     (indexes (get-offsets code pc size fetch more)))
		(display #\space)
		(display offset)
		(display #\space)
		(display indexes)
		(loop (+ pc (* (+ 2 more) size))
		      (+ count more)))
	      pc))))))

(define (get-offsets code pc size fetch count)
  (do ((pc pc (+ pc size))
       (i 0 (+ i 1))
       (r '() (cons (fetch code pc) r)))
      ((= i count)
       (reverse r))))

(define (display-flat-env-closures count pc code level write-subs? size fetch)
  (do ((i 0 (+ i 1))
       (pc pc (+ pc size)))
      ((= i count))
;    (if write-subs?
;	(begin
;	  (newline-indent (* (+ level 1) 3))
;	  (really-disassemble (template-ref template (get-offset code pc))
;			      (+ level 1))))
    (display " #f")))

;----------------------------------------------------------------
(define (display-entry-protocol code level)
  (if (not (= (code-vector-ref code 0)
	      (enum op protocol)))
      (error "entry point has no protocol" code))
  (show-opcode code 0 level)
  (let ((size (display-protocol (code-vector-ref code 1) 0 code)))
    (display (vector-ref '#(""
			    " (push env)"
			    " (push template)"
			    " (push env template)")
			 (code-vector-ref code size))) 
    (display #\))
    (+ size 1)))

; Display a protocol, returning the number of bytes of instruction stream that
; were consumed.

(define (display-protocol protocol pc code)
  (display #\space)
  (+ pc (cond ((<= protocol maximum-stack-args)
	       (display protocol)
	       2)
	      ((= protocol two-byte-nargs-protocol)
	       (display (get-offset code (+ pc 2)))
	       4)
	      ((= protocol two-byte-nargs+list-protocol)
	       (display (get-offset code (+ pc 2)))
	       (display " +")
	       4)
	      ((= protocol ignore-values-protocol)
	       (display "discard all values")
	       2)
	      ((= protocol call-with-values-protocol)
	       (display "call-with-values")
	       (let ((offset (get-offset code (+ pc 2))))
		 (if (not (= offset 0))
		     (begin
		       (display #\space)
		       (write `(=> ,(+ pc offset))))))
	       4)
	      ((= protocol args+nargs-protocol)
	       (display "args+nargs ")
	       (display (code-vector-ref code (+ pc 2)))
	       (display "+")
	       3)
	      ((= protocol nary-dispatch-protocol)
	       (display "nary-dispatch")
	       (do ((i 0 (+ i 1)))
		   ((= i 4))
		 (display-dispatch code pc (+ 3 i) i))
	       (display-dispatch code pc 2 "3+")
	       6)
	      ((= protocol big-stack-protocol)
	       (display "big-stack")
	       (let ((size (display-protocol
			     (code-vector-ref code
					      (- (code-vector-length code)
						 3))
			     pc
			     code)))
		 (display #\space)
		 (display (get-offset code
				      (- (code-vector-length code) 2)))
		 size))
	      (else
	       (error "unknown protocol" protocol)))))

(define (display-cont-data pc code)
  (let* ((size (get-offset code (+ pc 1)))
	 (gc-mask-size (code-vector-ref code (- (+ pc size) 3)))
	 (gc-mask-start (+ pc 3))
	 (gc-mask-bytes
	  (let loop ((i (- gc-mask-size 1)) (r '()))
	    (if (>= i 0)
		(loop (- i 1)
		      (cons (code-vector-ref code (+ gc-mask-start i)) r))
		r)))
	 (gc-mask (bytes->bits gc-mask-bytes)))
    (let loop ((mask gc-mask) (i 0))
      (if (not (zero? mask))
	  (begin
	    (if (bitwise-and mask 1)
		(begin
		  (write-char #\space)
		  (display i)))
	    (loop (arithmetic-shift mask -1) (+ 1 i)))))))
		  
(define (bytes->bits l)
  (let loop ((n 0) (l l))
    (if (null? l)
	n
	(loop (+ (arithmetic-shift n 8) (car l))
	      (cdr l)))))

(define (display-dispatch code pc index tag)
  (let ((offset (code-vector-ref code (+ pc index))))
    (if (not (= offset 0))
	(begin
	  (display #\space)
	  (display (list tag '=> (+ pc offset)))))))

; Generic opcode argument printer.

(define (print-opcode-args op start-pc code)
  (let ((specs (vector-ref opcode-arg-specs op)))
    (let loop ((specs specs) (pc (+ start-pc 1)))
      (cond ((or (null? specs)
 		 (= 0 (arg-spec-size (car specs) pc code)))
  	     pc)
  	    ((eq? (car specs) 'junk)  ; avoid printing a space
	     (loop (cdr specs) (+ pc 1)))
	    (else
  	     (display #\space)
  	     (print-opcode-arg specs pc start-pc code)
 	     (loop (cdr specs) (+ pc (arg-spec-size (car specs) pc code))))))))
  
; The number of bytes required by an argument.

(define (arg-spec-size spec pc code)
  (case spec
    ((nargs byte stob literal) 1)
    ((offset offset- index small-index two-bytes) 2)
    ((moves-data)
     (let ((n-moves (code-vector-ref code pc)))
       (+ 1 (* 2 n-moves))))
    ((big-moves-data)
     (let ((n-moves (code-vector-ref code pc)))
       (+ 2 (* 4 n-moves))))
    (else 0)))

; Print out the particular type of argument.

(define (print-opcode-arg specs pc start-pc code)
  (case (car specs)
    ((nargs byte)
     (write (code-vector-ref code pc)))
    ((literal)
     (write (- (code-vector-ref code pc) 128)))
    ((two-bytes)
     (write (get-offset code pc)))
    ((index)
     (write (get-offset code pc)))
    ((small-index)
     (write (code-vector-ref code pc)))
    ((offset)
     (write `(=> ,(+ start-pc (get-offset code pc)))))
    ((offset-)
     (write `(=> ,(- start-pc (get-offset code pc)))))
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
 		(loop (+ offset 2) (- n 1)))))))
    ((moves-data)
     (let ((n-moves (code-vector-ref code pc)))
       (display n-moves)
       (write-char #\space)
       (let loop ((offset (+ pc 1))
		  (n n-moves))
	 (cond ((not (zero? n))
		(display (list (code-vector-ref code offset)
			       (code-vector-ref code (+ offset 1))))
		(if (not (= n 1))
		    (write-char #\space))
		(loop (+ offset 2) (- n 1)))))))
    ((big-moves-data)
     (let ((n-moves (get-offset code pc)))
       (display n-moves)
       (write-char #\space)
       (let loop ((offset (+ pc 2))
		  (n n-moves))
	 (cond ((not (zero? n))
		(display (list (get-offset code offset)
			       (get-offset code (+ offset 2))))
		(if (not (= n 1))
		    (write-char #\space))
		(loop (+ offset 4) (- n 1)))))))))

;(define (write-literal-thing thing level write-templates?)
;  (cond ((location? thing)
;         (write (or (location-name thing)
;                    `(location ,(location-id thing)))))
;        ((not (template? thing))
;         (display #\')
;         (write thing))
;        (write-templates?
;         (really-disassemble thing (+ level 1)))
;        (else
;         (display "..."))))

;----------------
; Utilities.

; Turn OBJ into a template, if possible.

(define (coerce-to-template-or-code obj)
  (cond ((template? obj)
	 obj)
	((closure? obj)
	 (closure-template obj))
	((continuation? obj)
	 (or (continuation-template obj)
	     (continuation-code obj)))
	(else
	 (error "expected a procedure or continuation" obj))))

; Fetch the two-byte value at PC in CODE.

(define (get-offset code pc)
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
