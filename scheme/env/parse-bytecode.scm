;; Generic bytecode parser

(define-record-type attribution :attribution
  (make-attribution init-template template-literal opcode-table)
  attribution?
  (init-template attribution-init-template)
  (template-literal attribution-template-literal)
  (opcode-table attribution-opcode-table))

(define (opcode-table-ref table i)
  (vector-ref table i))

(define (opcode-table-set! table i new)
  (vector-set! table i new))

(define (make-opcode-table default)
  (make-vector op-count default))


;; Example attribution
(define (disass)
  (define (disass-init-template state protocol p-args push-template? push-env?)
    (cons (list 0 'protocol protocol p-args push-template? push-env?)
          state))

  (define instruction-set-table 
    (make-opcode-table 
     (lambda (opcode state pc . args)
       (cons `(,pc ,(enumerand->name opcode op) ,@args) state))))

  (define (attribute-literal literal i state)
    state)

  (make-attribution disass-init-template attribute-literal instruction-set-table))

(define (parse-template x state attribution)
  (let ((tem (coerce-to-template x)))
    (let* ((template-len (template-length tem)))
      (let lp ((i 1) (state state))
        (if (= i template-len)
            (parse-template-code tem state attribution)
            (let ((literal (template-ref tem i)))
              (if (template? literal)
                  (lp (+ i 1) (parse-template literal state attribution))
                  (lp (+ i 1) ((attribution-template-literal attribution) literal i state)))))))))

(define (parse-template-code tem state attribution)
  (let* ((code (template-code tem))
         (length (template-code-length code))
         (protocol (code-vector-ref code 1)))
    (receive (size protocol-arguments)
        (parse-protocol protocol code 0)
      (receive (push-template? push-env?)
          (case (code-vector-ref code (+ size 1))
            ((0) (values #f #f))
            ((1) (values #f #t))
            ((2) (values #t #f))
            ((3) (values #t #t)))
        (let loop ((pc (+ size 2)) 
                   (state ((attribution-init-template attribution) 
                           state protocol protocol-arguments push-template? push-env?)))
          (if (< pc length)
              (receive (size state)
                  (parse-instruction tem code pc state attribution)
                (loop (+ pc size) state))
              state))))))

(define (parse-instruction template code pc state attribution)
  (let* ((opcode (code-vector-ref code pc))
	 (len.rev-args (cond ((= opcode (enum op computed-goto))
			      (error "computed-goto in byte-code"))
			     ((= opcode (enum op make-flat-env))
			      (parse-flat-env-args (+ pc 1) code 1 code-vector-ref))
			     ((= opcode (enum op protocol))
                              (let ((protocol (code-vector-ref code (+ pc 1))))
                                (receive (size state)
                                    (parse-protocol protocol code pc)
                                  (cons size (list state)))))
			     ((= opcode (enum op cont-data))
			      (parse-cont-data-args pc code template))
			      ;; undo the peephole optimization:
;			     ((= opcode (enum op local0-push))
;			      (set! opcode (enum op local0))
;			      (cons 1 (list (code-vector-ref code (+ pc 1)))))
			     ;; we don't undo push-local0 as there should be no jumps inside it
			     (else
			      (parse-opcode-args opcode pc code template)))))
    (values (+ 1 (car len.rev-args))  ; 1 for the opcode
            (really-parse-instruction pc opcode (reverse (cdr len.rev-args)) state attribution))))

(define (really-parse-instruction pc opcode args state attribution)
;  (if (exists-label-for-bc-pc? pc)
;      (emit! out (label (label-for-bc-pc pc))))
  (let ((opcode-attribution (opcode-table-ref (attribution-opcode-table attribution) opcode)))
    (if opcode-attribution
	(apply opcode-attribution opcode state pc args)
	(error "cannot attribute " (enumerand->name opcode op) args))))


; returns the reversed list...
(define (parse-flat-env-args pc code size fetch)
  (let ((start-pc pc)
	(total-count (fetch code pc))
	(closure-count (fetch code (+ pc size))))
    (let* ((closure-offsets (if (< 0 closure-count)
				(cons (fetch code (+ pc size size))
				      (get-offsets code (+ pc size size size)
						   size fetch closure-count))
				#f))
	   (pc (if (< 0 closure-count)
		   (+ pc
		      (* 2 size)		; counts
		      size			; template offset
		      (* closure-count size)); subtemplates
		   (+ pc (* 2 size))))		; counts
	   (frame-count (fetch code pc))
	   (pc (+ pc size)))
      (let ((frame-offsets (get-offsets code pc size fetch frame-count)))
	(let ((pc (+ pc (* frame-count size)))
	      (count (+ closure-count frame-count)))
	  (let loop ((pc pc) (count count) (rev-env-offsets 
					    (list closure-offsets frame-offsets 
						  total-count)))
	    (if (= count total-count)
		(cons (- pc start-pc) rev-env-offsets)
		(let* ((env (fetch code pc))
		       (count-here (fetch code (+ pc size)))
		       (indexes (get-offsets code 
					     (+ pc size size) 
					     size 
					     fetch
					     count-here)))
		  (loop (+ pc (* (+ 2 count-here) size))
			(+ count count-here)
			(cons (cons env indexes) rev-env-offsets))))))))))
		

(define (get-offsets code pc size fetch count)
  (do ((pc pc (+ pc size))
       (i 0 (+ i 1))
       (r '() (cons (fetch code pc) r)))
      ((= i count)
       (reverse r))))


; Display a protocol, returning the number of bytes of instruction stream that
; were consumed.

(define (parse-protocol protocol code pc)
  (cond ((<= protocol maximum-stack-args)
         (values 1 '()))
        ((= protocol two-byte-nargs-protocol)
         (values 3 (list (get-offset code (+ pc 2)))))
        ((= protocol two-byte-nargs+list-protocol)
         (values 4 (list (get-offset code (+ pc 2)))))
        ((= protocol ignore-values-protocol)
         (values 1 (list ignore-values-protocol)))
        ((= protocol call-with-values-protocol)
         (values 3 (list (offset->label (get-offset code (+ pc 2)) pc))))
        ((= protocol args+nargs-protocol)
         (values 2 (list (code-vector-ref code (+ pc 2)))))
        ((= protocol nary-dispatch-protocol)
         (values 5 (parse-dispatch code pc)))
        ((= protocol big-stack-protocol)
         (let ((real-protocol (code-vector-ref code
                                               (- (code-vector-length code) 3)))
               (stack-size (get-offset code (- (code-vector-length code) 2))))
           (receive (size real-attribution)
               (parse-protocol real-protocol code pc)
             (values size
                     (list big-stack-protocol real-protocol real-attribution stack-size)))))
        (else
         (error "unknown protocol" protocol))))

(define (offset->label offset pc)
  (+ offset pc))

(define (parse-dispatch code pc)
  (define (maybe-parse-one-dispatch index)
    (let ((offset (code-vector-ref code (+ pc index))))
      (if (= offset 0)
          #f
          (offset->label offset pc))))

  (map maybe-parse-one-dispatch (list 3 4 5 2)))

; Generic opcode argument parser

(define (parse-opcode-args op start-pc code template)
  (let ((specs (vector-ref opcode-arg-specs op)))
    (let loop ((specs specs) (pc (+ start-pc 1)) (len 0) (args '()))
      (cond ((or (null? specs)
 		 (= 0 (arg-spec-size (car specs) pc code)))
  	     (cons len args))
  	    ((eq? (car specs) 'junk)  ; avoid parsing junk
	     (loop (cdr specs) (+ pc 1) (+ len 1) (cons 'junk args)))
	    (else
  	     (let ((arg (parse-opcode-arg specs 
					  pc 
					  start-pc 
					  code 
					  template)))
 	     (loop (cdr specs) 
		   (+ pc (arg-spec-size (car specs) pc code))
		   (+ len (arg-spec-size (car specs) pc code))
		   (cons arg args))))))))
  
; The number of bytes required by an argument.

(define (arg-spec-size spec pc code)
  (case spec
    ((nargs byte stob literal) 1)
    ((offset offset- index small-index two-bytes) 2)
    ((env-data) (error "env data is still there")) ;(+ 1 (* 2 (code-vector-ref code pc))))
    ((moves-data)
     (let ((n-moves (code-vector-ref code pc)))
       (+ 1 (* 2 n-moves))))
    ((big-moves-data)
     (let ((n-moves (code-vector-ref code pc)))
       (+ 2 (* 4 n-moves))))
    (else 
     (if (number? spec)
	 0
	 (error "unknown spec" spec)))))

; Parse the particular type of argument.

(define (parse-opcode-arg specs pc start-pc code template)
  (case (car specs)
    ((nargs byte)
     (code-vector-ref code pc))
    ((literal)
     (- (code-vector-ref code pc) 128))
    ((two-bytes)
     (get-offset code pc))
    ((index)
     (get-offset code pc))
    ((small-index)
     (code-vector-ref code pc))
    ((offset)
     (offset->label (get-offset code pc) start-pc))
    ((offset-)
     (offset->label (- (get-offset code pc)) start-pc))
    ((stob)
     (code-vector-ref code pc))
    ((env-data)
     (error "env-data in bytecode"))
    ((moves-data)
     (let ((n-moves (code-vector-ref code pc)))
       (let loop ((offset (+ pc 1))
		  (n n-moves))
	 (if (zero? n)
	     '()
	     (cons (cons (code-vector-ref code offset)
			 (code-vector-ref code (+ offset 1)))
		   (loop (+ offset 2) (- n 1)))))))
    ((big-moves-data)
     (let ((n-moves (get-offset code pc)))
       (let loop ((offset (+ pc 2))
		  (n n-moves))
	 (if (zero? n)
	     '()
	     (cons (cons (get-offset code offset)
			 (get-offset code (+ offset 2)))
		   (loop (+ offset 4) (- n 1)))))))
    (else (error "unknown arg spec: " (car specs)))))

(define (parse-cont-data-args pc code template)
  (let* ((len (get-offset code (+ pc 1)))
	 (end-pc (+ pc len))
	 (gc-mask-size (code-vector-ref code (- end-pc 3)))
	 (depth (get-offset code (- end-pc 2)))
	 (offset (get-offset code (- end-pc 5)))
	 (mask-bytes
	  (let lp ((the-pc (+ pc 3 1)))
	    (if (> the-pc (+ pc 3 gc-mask-size))
		'()
		(cons (code-vector-ref code the-pc)
		      (lp (+ the-pc 1)))))))
    (cons (- len 1) ; caller will skip opcode
	  (reverse
	       (list len
		     mask-bytes
		     (offset->label offset pc)
		     gc-mask-size
		     depth)))))
;----------------
; Utilities.

; TODO: Put the template-related stuff into a separate module?

; Turn OBJ into a template, if possible.

(define (coerce-to-template obj)
  (cond ((template? obj) obj)
	((closure? obj) (closure-template obj))
	((continuation? obj) (continuation-template obj))
	(else (error "expected a procedure or continuation" obj))))

(define (template-code-length code)
  (if (and (= (enum op protocol)
	      (code-vector-ref code 0))
	   (= big-stack-protocol
	      (code-vector-ref code 1)))
      (- (code-vector-length code) 3)
      (code-vector-length code)))


; Fetch the two-byte value at PC in CODE.

(define (get-offset code pc)
  (+ (* (code-vector-ref code pc)
	byte-limit)
     (code-vector-ref code (+ pc 1))))

; Should this be in its own spot?

(define-syntax receive
  (syntax-rules ()
    ((receive ?vars ?producer . ?body)
     (call-with-values (lambda () ?producer)
       (lambda ?vars . ?body)))))
