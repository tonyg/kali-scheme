;; Generic byte code parser

(define-record-type attribution :attribution
  (make-attribution init-template template-literal 
                           opcode-table make-label at-label)
  attribution?
  (init-template attribution-init-template)
  (template-literal attribution-template-literal)
  (opcode-table attribution-opcode-table)
  (make-label attribution-make-label)
  (at-label attribution-at-label))

(define (opcode-table-ref table i)
  (vector-ref table i))

(define (opcode-table-set! table i new)
  (vector-set! table i new))

(define (make-opcode-table default)
  (make-vector op-count default))


;; Example attribution
(define (disass)
  (define (disass-init-template state template p-args push-template? push-env?)
    (cons (list 0 'protocol p-args push-template? push-env?)
          state))

  (define instruction-set-table 
    (make-opcode-table 
     (lambda (opcode template state pc . args)
       (cons `(,pc ,(enumerand->name opcode op) ,@(map cdr args)) state))))

  (define (attribute-literal literal i state)
    state)
  
  (define (make-label target-pc)
    target-pc)

  (define (at-label label state)
    (cons `(,label :) state))

  (make-attribution disass-init-template attribute-literal 
                    instruction-set-table make-label at-label))

(define (parse-template x state attribution)
  (let* ((tem (coerce-to-template x))
         (template-len (template-length tem)))
        (let lp ((i 1) (state state))
          (if (= i template-len)
              (parse-template-code tem (template-code tem) state attribution)
              (let ((literal (template-ref tem i)))
                (if (template? literal)
                    (lp (+ i 1) (parse-template literal state attribution))
                    (lp (+ i 1) ((attribution-template-literal attribution) literal i state))))))))

(define (parse-template-code tem code state attribution)
  (with-template 
   tem code state attribution
   (lambda (pc length state)
     (let loop ((pc pc)
                (state state))
       (if (< pc length)
           (receive (size state)
               (parse-instruction tem code pc state attribution)
             (loop (+ pc size) state))
           state)))))

(define (with-template tem code state attribution fun)
  (let ((length (template-code-length code)))
    (let-fluid 
        *bc-make-labels* '()
      (lambda ()
        (for-each 
         (lambda (pc) (pc->label pc attribution))
         (debug-data-jump-back-dests (template-debug-data tem)))
        (receive (size protocol-arguments)
            (parse-protocol code 1 attribution)
          (receive (push-template? push-env?)
              (case (code-vector-ref code (+ size 1))
                ((0) (values #f #f))
                ((1) (values #f #t))
                ((2) (values #t #f))
                ((3) (values #t #t))
                (else (error "invalid init-template spec" (code-vector-ref code (+ size 1)))))
            (fun (+ size 2)
                 length
                 ((attribution-init-template attribution) 
                  state tem protocol-arguments push-template? push-env?))))))))
          

(define (parse-instruction template code pc state attribution)
  (let* ((opcode (code-vector-ref code pc))
	 (len.rev-args (cond ((= opcode (enum op computed-goto)) ; unused?
			      (error "computed-goto in parse-bytecode"))
			     (else
			      (parse-opcode-args opcode 
                                                 pc 
                                                 code 
                                                 template 
                                                 attribution)))))
    (values (+ 1 (car len.rev-args))  ; 1 for the opcode
            (really-parse-instruction pc opcode template state 
                                      (reverse (cdr len.rev-args)) attribution))))

(define (really-parse-instruction pc opcode template state args attribution)
  (let ((new-state (if (label-at-pc? pc)
                       ((attribution-at-label attribution) 
                        (pc->label pc attribution)
                        state)
                       state)))
  (let ((opcode-attribution 
         (opcode-table-ref (attribution-opcode-table attribution) opcode)))
    (if opcode-attribution
	(apply opcode-attribution opcode template new-state pc args)
	(error "cannot attribute " (enumerand->name opcode op) args)))))

;;--------------------
;; labels

(define *bc-make-labels* (make-fluid '()))

(define (add-pc! pc attribution)
  (set-fluid! *bc-make-labels*
	      (cons (cons pc ((attribution-make-label attribution) pc))
                    (fluid *bc-make-labels*))))

(define (pc->label pc attribution)
  (let ((maybe-pc.label (assq pc (fluid *bc-make-labels*))))
    (if maybe-pc.label
	(cdr maybe-pc.label)
	(begin 
	  (add-pc! pc attribution)
	  (pc->label pc attribution)))))

(define (label-at-pc? pc)
  (if (assq pc (fluid *bc-make-labels*)) #t #f))

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

(define-record-type env-data :env-data
  (make-env-data total-count frame-offsets maybe-template-index closure-offsets
                 env-offsets)
  env-data?
  (total-count env-data-total-count)
  (frame-offsets env-data-frame-offsets)
  (maybe-template-index env-data-maybe-template-index)
  (closure-offsets env-data-closure-offsets)
  (env-offsets env-data-env-offsets))

(define (parse-flat-env-args pc code size fetch)
  (let ((start-pc pc)
	(total-count (fetch code pc))
	(closure-count (fetch code (+ pc size))))
    (receive (template-index closure-offsets)
        (if (< 0 closure-count)
            (values (fetch code (+ pc size size))
                    (get-offsets code (+ pc size size size)
                                 size fetch closure-count))
            (values #f '()))
      (let* ((pc (if (< 0 closure-count)
                     (+ pc
                        (* 2 size)      ; counts
                        size            ; template offset
                        (* closure-count size)) ; subtemplates
                     (+ pc (* 2 size)))) ; counts
             (frame-count (fetch code pc))
             (pc (+ pc size)))
        (let ((frame-offsets (get-offsets code pc size fetch frame-count)))
          (let ((pc (+ pc (* frame-count size)))
                (count (+ closure-count frame-count)))
            (let loop ((pc pc) (count count) (rev-env-offsets '()))
              (if (= count total-count)
                  (values (- pc start-pc)
                          (make-env-data total-count frame-offsets 
                                         template-index closure-offsets
                                         (reverse rev-env-offsets)))
                  (let* ((env (fetch code pc))
                         (count-here (fetch code (+ pc size)))
                         (indexes (get-offsets code 
                                               (+ pc size size) 
                                               size 
                                               fetch
                                               count-here)))
                    (loop (+ pc (* (+ 2 count-here) size))
                          (+ count count-here)
                          (cons (cons env indexes) rev-env-offsets)))))))))))
		

(define (get-offsets code pc size fetch count)
  (do ((pc pc (+ pc size))
       (i 0 (+ i 1))
       (r '() (cons (fetch code pc) r)))
      ((= i count)
       (reverse r))))


; Parse a protocol, returning the number of bytes of instruction stream that
; were consumed. PC has to point behind the PRTOCOL opcode

(define (parse-protocol code pc attribution)
  (let ((protocol (code-vector-ref code pc)))
    (really-parse-protocol protocol code pc attribution)))

(define (really-parse-protocol protocol code pc attribution)
  (cond ((<= protocol maximum-stack-args)
         (values 1 (list protocol)))
        ((= protocol two-byte-nargs-protocol)
         (values 3 (list protocol (get-offset code (+ pc 1)))))
        ((= protocol two-byte-nargs+list-protocol)
         (values 3 (list protocol (get-offset code (+ pc 1)))))
        ((= protocol ignore-values-protocol)
         (values 1 (list protocol)))
        ((= protocol call-with-values-protocol)
         (let ((offset (get-offset code (+ pc 1))))
           (values 3 (list protocol 
                           (pc->label (+ offset pc)
                                      attribution)
                           (zero? offset)))))
        ((= protocol args+nargs-protocol)
         (values 2 (list protocol (code-vector-ref code (+ pc 1)))))
        ((= protocol nary-dispatch-protocol)
         (values 5 (cons protocol (parse-dispatch code pc attribution))))
        ((= protocol big-stack-protocol)
         (let ((real-protocol (code-vector-ref code
                                               (- (code-vector-length code) 3)))
               (stack-size (get-offset code (- (code-vector-length code) 2))))
           (receive (size real-attribution)
               (really-parse-protocol real-protocol code pc attribution)
             (values size
                     (list protocol real-attribution stack-size)))))
        (else
         (error "unknown protocol" protocol pc))))

(define (parse-dispatch code pc attribution)
  (define (maybe-parse-one-dispatch index)
    (let ((offset (code-vector-ref code (+ pc index))))
      (if (= offset 0)
          #f
          (pc->label (+ offset pc) attribution))))

  (map maybe-parse-one-dispatch (list 3 4 5 2)))

(define (protocol-protocol p-args)
  (car p-args))

(define (n-ary-protocol? p-args)
  (let ((protocol (car p-args)))
    (if (or (= protocol two-byte-nargs+list-protocol)
            (= protocol call-with-values-protocol)
            (= protocol ignore-values-protocol))
        #t
        (if (or (<= protocol maximum-stack-args)
                (= protocol two-byte-nargs-protocol))
            #f
            (if (= protocol big-stack-protocol)
                (n-ary-protocol? (cadr p-args))
                (error "unknown protocol in n-ary-protocol?" p-args))))))

(define (protocol-nargs p-args)
  (let ((protocol (car p-args)))
    (cond ((<= protocol maximum-stack-args)
           protocol)
	  ((= protocol two-byte-nargs-protocol)
	   (cadr p-args))
	  ((= protocol two-byte-nargs+list-protocol)
	   (cadr p-args))
	  ((= protocol args+nargs-protocol)
           (cadr p-args))
	  ((= protocol big-stack-protocol)
           (protocol-nargs (cadr p-args)))
          ((= protocol ignore-values-protocol)
           0)
          ((= protocol call-with-values-protocol)
           (error "call-with-values-protocol in protocol-nargs"))
	  (else
	   (error "unknown protocol in protocol-nargs" p-args)))))

(define (protocol-cwv-tailcall? p-args)
  (let ((protocol (protocol-protocol p-args)))
    (if (not (= protocol call-with-values-protocol))
        (error "invalid protocol in protocol-cwv-tailcall?" protocol))
    (caddr p-args)))

(define (call-with-values-protocol-target p-args)
  (let ((protocol (protocol-protocol p-args)))
    (if (not (= protocol call-with-values-protocol))
        (error "invalid protocol in protocol-cwv-tailcall?" protocol))
    (cadr p-args)))
  
; Generic opcode argument parser

(define (parse-opcode-args op start-pc code template attribution)
  (let ((specs (vector-ref opcode-arg-specs op)))
    (let loop ((specs specs) (pc (+ start-pc 1)) (len 0) (args '()))
      (cond ((null? specs)
  	     (cons len args))
            ((eq? (car specs) 'protocol)
             (receive (size p-args)
                 (parse-protocol code pc attribution)
               (loop (cdr specs) 
                     (+ pc size) 
                     (+ len size) 
                     (cons (cons 'protocol p-args) args))))
            ((eq? (car specs) 'env-data)
             (receive (size env-data)
                 (parse-flat-env-args pc code 1 code-vector-ref)
               (loop (cdr specs)
                     (+ pc size)
                     (+ len size)
                     (cons (cons 'env-data env-data) args))))
            ((= 0 (arg-spec-size (car specs) pc code))
             (cons len args))
	    (else
  	     (let ((arg (parse-opcode-arg specs 
					  pc 
					  start-pc 
					  code 
					  template
                                          attribution)))
 	     (loop (cdr specs) 
		   (+ pc (arg-spec-size (car specs) pc code))
		   (+ len (arg-spec-size (car specs) pc code))
		   (cons arg args))))))))
  
; The number of bytes required by an argument.

(define (arg-spec-size spec pc code)
  (case spec
    ((byte nargs stack-index index literal stob) 1)
    ((two-bytes two-byte-narg two-byte-stack-index two-byte-index offset offset-) 2)
    ((env-data) (error "env-data in arg-spec-size"))
    ((protocol) (error "protocol in arg-spec-size"))
    ((moves-data)
     (let ((n-moves (code-vector-ref code pc)))
       (+ 1 (* 2 n-moves))))
    ((big-moves-data)
     (let ((n-moves (code-vector-ref code pc)))
       (+ 2 (* 4 n-moves))))
    ((cont-data)
     (- (get-offset code pc) 1)) ; size includes opcode
    (else 0)))

; Parse the particular type of argument.

(define (parse-opcode-arg specs pc start-pc code template attribution)
  (cons 
   (car specs)
   (case (car specs)
     ((byte nargs stack-index index)
      (code-vector-ref code pc))
     ((two-bytes two-byte-nargs two-byte-stack-index two-byte-index)
      (get-offset code pc))
     ((literal)
      (- (code-vector-ref code pc) 128))
     ((offset)
      (let ((offset (get-offset code pc)))
        (if (zero? offset)
            #f
            (pc->label (+ start-pc offset) attribution))))
     ((offset-)
      (pc->label (- start-pc (get-offset code pc)) attribution))
     ((stob)
      (code-vector-ref code pc))
     ((cont-data)
      (parse-cont-data-args pc code template attribution))
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
     (else (error "unknown arg spec: " (car specs))))))

(define-record-type cont-data :cont-data
  (make-cont-data length mask-bytes pc gc-mask-size depth)
  cont-data?
  (length cont-data-length)
  (mask-bytes cont-data-mask-bytes)
  (pc cont-data-pc)
  (gc-mask-size cont-data-gc-mask-size)
  (depth cont-data-depth))

(define (parse-cont-data-args pc code template attribution)
  (let* ((len (get-offset code pc))
	 (end-pc (- (+ pc len) 1))      ; len includes opcode
	 (gc-mask-size (code-vector-ref code (- end-pc 3)))
	 (depth (get-offset code (- end-pc 2)))
	 (offset (get-offset code (- end-pc 5)))
	 (mask-bytes
	  (let lp ((the-pc (+ pc 3)))
	    (if (> the-pc (+ pc 2 gc-mask-size))
		'()
		(cons (code-vector-ref code the-pc)
		      (lp (+ the-pc 1)))))))
    (make-cont-data len
                    mask-bytes
                    (pc->label offset attribution)
                    gc-mask-size
                    depth)))
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
