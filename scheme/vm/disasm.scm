; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Disassembler that uses the VM's data structures.

(define (disassemble stuff . no-subtemplates)
  (let ((template (cond ((template? stuff) stuff)
			((closure? stuff) (closure-template stuff))
			((and (location? stuff)
			      (closure? (contents stuff)))
			 (closure-template (contents stuff)))
			(else
			 (error "cannot coerce to template" stuff)))))
    (really-disassemble template
			0
			(if (null? no-subtemplates)
			    #f
			    (car no-subtemplates)))
    (newline)))

(define (really-disassemble tem level write-templates?)
  (let loop ((pc 0))
    (if (< pc (code-vector-length (template-code tem)))
        (loop (write-instruction tem pc level write-templates?)))))

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
 		    ((= opcode (enum op make-flat-env))
 		     (display-flat-env (+ pc 1) code))
		    ((= opcode (enum op protocol))
		     (display-protocol pc code))
 		    (else
 		     (print-opcode-args opcode (+ pc 1) code template
 					level write-sub-templates?)))))
      (display #\))
      pc)))

(define (display-computed-goto start-pc code)
  (display #\space)
  (let ((count (code-vector-ref code (+ start-pc 1))))
    (write count)
    (do ((pc (+ start-pc 2) (+ pc 2))
	 (count count (- count 1)))
	((= count 0) pc)
      (display #\space)
      (write `(=> ,(+ start-pc (get-offset pc code)))))))

(define (display-flat-env pc code)
  (let ((total-count (code-vector-ref code (+ pc 1))))
    (display #\space) (write total-count)
    (let loop ((pc (+ pc 2)) (count 0) (old-back 0))
      (if (= count total-count)
	  pc
	  (let ((back (+ (code-vector-ref code pc)
			 old-back))
		(limit (+ pc 2 (code-vector-ref code (+ pc 1)))))
	    (do ((pc (+ pc 2) (+ pc 1))
		 (count count (+ count 1))
		 (offsets '() (cons (code-vector-ref code pc) offsets)))
		((= pc limit)
		 (display #\space)
		 (write `(,back ,(reverse offsets)))
		 (loop pc count back))))))))

(define (display-protocol pc code)
  (let ((protocol (code-vector-ref code (+ pc 1))))
    (display #\space)
    (+ pc (cond ((<= protocol maximum-stack-args)
		 (display protocol)
		 2)
		((= protocol two-byte-nargs-protocol)
		 (display (get-offset (+ pc 2) code))
		 4)
		((= protocol two-byte-nargs+list-protocol)
		 (display (get-offset (+ pc 2) code))
		 (display "+")
		 4)
		((= protocol args+nargs-protocol)
		 (display "args+nargs")
		 3)
		((= protocol ignore-values-protocol)
		 (display "discard all values")
		 2)
		((= protocol call-with-values-protocol)
		 (display "call-with-values")
		 2)
		((= protocol nary-dispatch-protocol)
		 (display "nary-dispatch")
		 (do ((i 0 (+ i 1)))
		     ((= i 4))
		   (let ((offset (code-vector-ref code (+ pc 2 i))))
		     (if (not (= offset 0))
			 (begin
			   (display #\space)
			   (display (list (if (= i 3) "3+" i)
					  '=>
					  (+ pc offset)))))))
		 6)
		(else
		 (error "unknown protocol" protocol))))))

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
    ((nargs byte stob junk) 1)
    ((offset small-index index two-bytes) 2)
    (else 0)))

(define (print-opcode-arg specs pc code template level write-templates?)
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
     (write `(=> ,(+ pc -1 (get-offset pc code)))))  ; -1 to back up over opcode
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
	 (really-disassemble thing (+ level 1) #t))
	(else
	 (display "..."))))


