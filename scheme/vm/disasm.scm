; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; Disassembler that uses the VM's data structures.

(define (disassemble stuff)
  (let ((template (cond ((template? stuff) stuff)
			((closure? stuff) (closure-template stuff))
			((and (location? stuff)
			      (closure? (contents stuff)))
			 (closure-template (contents stuff)))
			(else
			 (error "cannot coerce to template" stuff)))))
    (really-disassemble template 0)
    (newline)))

(define (really-disassemble tem level)
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
    (let ((pc (cond ((= opcode (enum op computed-goto))
 		     (display-computed-goto pc code))
 		    ((= opcode (enum op flat-closure))
 		     (display-flat-lambda (+ pc 1) code template
 					  level write-sub-templates?))
 		    (else
 		     (print-opcode-args opcode (+ pc 1) code template
 					level write-sub-templates?)))))
      (display #\))
      pc)))

(define byte-limit (arithmetic-shift 1 bits-used-per-byte))

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
 	  (really-disassemble template (+ level 1))
 	  (display "...")))
    (+ pc 1)))

(define (print-opcode-args op pc code template level write-templates?)
  (let ((specs (vector-ref opcode-arg-specs op)))
    (let loop ((specs specs) (pc pc))
      (cond ((or (null? specs)
		 (= 0 (arg-spec-size (car specs))))
	     pc)
	    (else
	     (display #\space)
	     (print-opcode-arg specs pc code template level write-templates?)
	     (loop (cdr specs) (+ pc (arg-spec-size (car specs)))))))))

(define (arg-spec-size spec)
  (case spec
    ((nargs byte small-index stob) 1)
    ((offset index) 2)
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
			   (apply + (map arg-spec-size specs)))))))
    ((stob)
     (write (enumerand->name (code-vector-ref code pc) stob)))))

(define (get-offset pc code)
  (+ (* (code-vector-ref code pc)
	byte-limit)
     (code-vector-ref code (+ pc 1))))

(define (write-literal-thing thing level write-templates?)
  (cond ((location? thing)
	 (write `(location ,thing ,(location-id thing))))
	((not (template? thing))
	 (display #\')
	 (write thing))
	(write-templates?
	 (really-disassemble thing (+ level 1)))
	(else
	 (display "..."))))


