; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.

; BREAKPOINT also needs to be defined somehow


(define s48-directory "~/s48/new/")

(define vm-files
  '(
    s48-features
    scheme-util
    macros
    
    memory
    arch
    data
    struct
    vmio
    init
    gc

    istruct
    stack
    interp
    prim

    resume
    ))

(define compiler-files
  '(
    (boot features)
    (boot s48-record)
    (boot s48-features)

    (vm arch)
    (rts cenv)
    (rts comp)
    (rts cprim)
    (rts derive)
    ))
    
(define debug-files
  '(
    s48-debug
    transport
    debug
    ))

(define (load-file spec)
  (load (string-append s48-directory
		       (symbol->string (car spec))
		       "/"
		       (symbol->string (cadr spec))
		       ".scm")))

(define (load-vm)
  (for-each load-file (map (lambda (n) (list 'vm n)) vm-files))
  (set! no-exceptions? #t))

(define (load-debug)
  (for-each load-file compiler-files)
  (for-each load-file (map (lambda (n) (list 'vm n)) debug-files)))

; useful debugging proc
(define (binary x)
  (do ((x x (quotient x 2))
       (r '() (cons (bitwise-and x 1) r)))
      ((= x 0) r)))

(define (heap-walk proc)
  (let loop ((addr *newspace-begin*))
    (if (>= addr *hp*)
	#t
	(let ((d (fetch addr)))
	  (if (header? d)
	      (proc addr))
	  (loop (addr1+ (addr+ addr (header-a-units d))))))))

(define (find-location name)
  (heap-walk (lambda (addr)
	       (let ((d (fetch addr)))
		 (if (= stob/location (header-type d))
		     (let ((loc (make-stob-descriptor (addr1+ addr))))
		       (if (string=? name
				     (extract-string
				      (vm-symbol->string (location-id loc))))
			   (format #t "~D~%" loc))))))))

(define (preview)
  (do ((c *cont* (continuation-cont c)))
      ((not (continuation? c)))
    (let ((id (template-name (continuation-template c))))
      (display (if (vm-symbol? id)
		   (extract-string
		    (vm-symbol->string
		     id))
		   #f))
      (newline))))

(define (template-locations temp)
  (do ((i 2 (+ i 1))
       (r '() (cons
	       (let ((x (vm-vector-ref temp i)))
		 (if (location? x)
		     (extract-string (vm-symbol->string (location-id x)))
		     #f)) r)))
      ((>= i (vm-vector-length temp))
       (reverse r))))

(define (stack-locations)
  (do ((c *cont* (continuation-cont c)))
      ((not (continuation? c)))
    (let ((id (template-name (continuation-template c))))
      (format #t "~A: ~S~%"
	      (if (vm-symbol? id)
		  (extract-string (vm-symbol->string id))
		  #f)
	      (xtemplate (continuation-template c))))))
  
(define (disasm . maybe-stuff)
  (let* ((template (if (null? maybe-stuff)
		       *template*
		       (let ((x (car maybe-stuff)))
			 (cond ((template? x) x)
			       ((closure? x) (closure-template x))
			       ((and (location? x)
				     (closure? (contents x)))
				(closure-template (contents x)))
			       (else
				(error "cannot coerce to template" x)))))))
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
    (let ((pc (if (= opcode op/computed-goto)
		  (display-computed-goto pc code)
		  (print-opcode-args opcode (+ pc 1) code template
				     level write-sub-templates?))))
      (display #\) )
      pc)))

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
		 (= 0 (arg-spec-size (car specs))))
	     pc)
	    (else
	     (display #\space)
	     (print-opcode-arg specs pc code template level write-templates?)
	     (loop (cdr specs) (+ pc (arg-spec-size (car specs)))))))))

(define (arg-spec-size spec)
  (case spec
    ((nargs byte index stob) 1)
    ((offset) 2)
    (else 0)))

(define (print-opcode-arg specs pc code template level write-templates?)
  (case (car specs)
    ((nargs byte)
     (write (code-vector-ref code pc)))
    ((index)
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
	 (write `(location ,(or (location-name thing)
				(location-id thing)))))
	((not (template? thing))
	 (display #\')
	 (write thing))
	(write-templates?
	 (really-disassemble thing (+ level 1)))
	(else
	 (display "..."))))


