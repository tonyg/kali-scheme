; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This determines the maximum stack depth needed by a code vector.

(define (maximum-stack-use code-vector)
  (cond ((not (= (code-vector-ref code-vector 0)
		 (enum op protocol)))
	 0)
	((= (code-vector-ref code-vector 1)
	    nary-dispatch-protocol)		; has unjumped-to targets
	 (stack-max code-vector
		    6
		    0
		    0
		    (do ((i 2 (+ i 1))
			 (r '() (let ((target (code-vector-ref code-vector i)))
				  (if (= 0 target)
				      r
				      (cons (cons target 0) r)))))
			((= i 6)
			 r))))
	(else
	 (stack-max code-vector
		    (protocol-skip code-vector)
		    0
		    0
		    '()))))

(define (protocol-skip code-vector)
  (let ((protocol (code-vector-ref code-vector 1)))
    (cond ((or (= protocol two-byte-nargs-protocol)
	       (= protocol two-byte-nargs+list-protocol))
	   4)
	  ((= protocol args+nargs-protocol)
	   3)
	  (else
	   2))))

;----------------
; A vector of procedures, one for each opcode.

(define stack-delta (make-vector op-count #f))

(define-syntax define-delta
  (syntax-rules ()
    ((define-delta opcode fun)
     (vector-set! stack-delta (enum op opcode) fun))))

; Handle the opcode at I.  DEPTH is the current stack depth, MAXIMUM is the
; maximum so far, and JUMPS is a list of (<index> . <depth>) giving the stack
; depth at jump targets.

(define (stack-max code-vector i depth maximum jumps)
  ((vector-ref stack-delta (code-vector-ref code-vector i))
     code-vector
     (+ i 1)
     depth
     maximum
     jumps))

; Do nothing and advance BYTE-SIZE bytes.

(define (nothing byte-size)
  (lambda (code-vector i depth maximum jumps)
    (stack-max code-vector
	       (+ i byte-size)
	       depth
	       maximum
	       jumps)))

; Pop COUNT values from the stack and advance BYTE-SIZE bytes.

(define (popper count byte-args)
  (lambda (code-vector i depth maximum jumps)
    (stack-max code-vector
	       (+ i byte-args)
	       (- depth count)
	       maximum
	       jumps)))

; Push COUNT values onto the stack and advance BYTE-SIZE bytes.

(define (pusher count byte-args)
  (lambda (code-vector i depth maximum jumps)
    (stack-max code-vector
	       (+ i byte-args)
	       (+ depth count)
	       (imax maximum (+ depth count))
	       jumps)))

; Continue on at opcode I.  This is used for opcodes that do not fall through
; to the next instruction.  I is either the end of the code vector or the target
; of a jump or continuation.

(define (continue code-vector i maximum jumps)
  (cond ((= i (code-vector-length code-vector))
	 maximum)
	((assq i jumps)
	 => (lambda (pair)
	      (stack-max code-vector i (cdr pair) maximum jumps)))
	((= (code-vector-ref code-vector i)
	    (enum op cont-data))
	 (continue code-vector
		   (+ i 4)		; how do I know this?
		   maximum
		   jumps))
	(else
	 (error "stack-max: no one jumps to target" i))))

; Skip BYTE-ARGS and then continue.

(define (continuer byte-args)
  (lambda (code-vector i depth maximum jumps)
    (continue code-vector (+ i byte-args) maximum jumps)))

;----------------
; Two-byte offsets, here because it is used at top-level.

(define (get-offset code pc)
  (+ (* (code-vector-ref code pc)
	byte-limit)
     (code-vector-ref code (+ pc 1))))

;----------------
; All the special opcodes

(define-delta make-env           (pusher environment-stack-size 2))
;(define-delta push               (pusher 1 0))
(define-delta pop                (popper 1 0))
(define-delta call               (continuer 1))
(define-delta big-call           (continuer 2))
(define-delta apply              (continuer 2))
(define-delta closed-apply       (continuer 0))
(define-delta with-continuation  (nothing 0))     ; what the compiler requires
(define-delta return             (continuer 0))
(define-delta values             (continuer 2))
(define-delta closed-values      (continuer 0))
(define-delta goto-template      (continuer 2))
(define-delta call-template      (continuer 3))

; We should only reach PROTOCOL opcodes in continuations.

(define-delta protocol
  (lambda (cv pc depth maximum jumps)
    (let ((protocol (code-vector-ref cv pc)))
      (if (= protocol call-with-values-protocol)
	  (continue cv (+ pc 1) maximum jumps)
	  (call-with-values
	   (lambda ()
	     (cond ((or (<= protocol 1)
			(= protocol ignore-values-protocol))
		    (values 1 0))
		   ((<= protocol maximum-stack-args)
		    (values 1 protocol))
		   ((= protocol two-byte-nargs+list-protocol)
		    (values (+ (get-offset cv (+ pc 1))
			       1)		; the rest list
			    3))
		   ((= protocol two-byte-nargs-protocol)
		    (values (get-offset cv (+ pc 1))
			    3))
		   (else
		    (error "unknown protocol" protocol))))
	   (lambda (bytes on-stack)
	     (stack-max cv
			(+ pc bytes)
			(+ depth on-stack)
			(imax maximum (+ depth on-stack))
			jumps)))))))

; Peephole optimizations

(define-delta push
  (lambda (cv pc depth maximum jumps)
    (if (= (enum op local0)
	   (code-vector-ref cv pc))
	(begin
	  (code-vector-set! cv (- pc 1) (enum op push-local0))
	  (stack-max cv
		     (+ pc 2)
		     (+ depth 1)
		     (imax maximum (+ depth 1))
		     jumps))
	(stack-max cv
		   pc
		   (+ depth 1)
		   (imax maximum (+ depth 1))
		   jumps))))

(define-delta local0
  (lambda (cv pc depth maximum jumps)
    (if (= (enum op push)
	   (code-vector-ref cv (+ pc 1)))
	(begin
	  (code-vector-set! cv (- pc 1) (enum op local0-push))
	  (stack-max cv
		     (+ pc 2)
		     (+ depth 1)
		     (imax maximum (+ depth 1))
		     jumps))
	(stack-max cv
		   (+ pc 1)
		   depth
		   maximum
		   jumps))))

; Pop the given numbers of stack values.

(define-delta make-stored-object
  (lambda (cv pc depth maximum jumps)
    (let ((args (code-vector-ref cv pc)))
      (stack-max cv (+ pc 2) (- depth (- args 1)) maximum jumps))))

; Skip over the environment specification.

(define (flat-env-checker size fetch)
  (lambda (cv pc depth maximum jumps)
    (let ((include-*val*? (= 1 (code-vector-ref cv pc)))
	  (count (fetch cv (+ pc 1))))
      (let loop ((i (+ pc 1 size))
		 (count (if include-*val*?
			    (- count 1)
			    count)))
	(if (= count 0)
	    (stack-max cv i depth maximum jumps)
	    (let ((level-count (fetch cv (+ i 1))))
	      (loop (+ i 1 size (* level-count size))
		    (- count level-count))))))))

(define-delta make-flat-env     (flat-env-checker 1 code-vector-ref))
(define-delta make-big-flat-env (flat-env-checker 2 get-offset))

; Temporarily puts COUNT values on the stack.

(define-delta letrec-closures
  (lambda (cv pc depth maximum jumps)
    (let ((count (get-offset cv pc)))
      (stack-max cv
		 (+ pc (* 2 (+ count 1)))
		 depth
		 (max maximum (+ depth count environment-stack-size))
		 jumps))))

; Adds the target to the list of jumps.
; The -1 is to back up over the opcode.
; Could check that the we agree with the compiler on the size of the stack.

(define-delta make-cont
  (lambda (code-vector i depth maximum jumps)
    (let ((target (+ i -1 (get-offset code-vector i))))
      (stack-max code-vector
		 (+ i 2)  ; eat offset
		 (+ depth continuation-stack-size)
		 (max maximum (+ depth continuation-stack-size))
		 (cons (cons target depth) jumps)))))

; Add the jump target(s) and either fall-through or not.
; The -1 is to back up over the opcode.

(define-delta jump-if-false
  (lambda (code-vector i depth maximum jumps)
    (let ((target (+ i -1 (get-offset code-vector i))))
      (stack-max code-vector
		 (+ i 2) ; eat label
		 depth
		 maximum
		 (cons (cons target depth) jumps)))))

(define-delta jump
  (lambda (code-vector i depth maximum jumps)
    (let ((target (+ i -1 (get-offset code-vector i))))
      (continue code-vector
		(+ i 2)  ; eat label
		maximum
		(cons (cons target depth) jumps)))))

(define-delta computed-goto
  (lambda (code-vector i depth maximum jumps)
    (let ((count (code-vector-ref code-vector i))
	  (base (- i 1))			; back up over opcode
	  (i (+ i 1)))
      (let loop ((c 0) (jumps jumps))
	(if (= c count)
	    (stack-max code-vector
		       (+ i (* 2 count))
		       depth
		       maximum
		       jumps)
	    (loop (+ c 1)
		  (cons (cons (+ base (get-offset code-vector (+ i (* c 2))))
			      depth)
			jumps)))))))

;----------------
; Fill in the `normal' opcodes using the information in OPCODE-ARG-SPECS.

(define (stack-function arg-specs)
  (let loop ((specs arg-specs) (skip 0))
    (cond ((null? specs)
	   (nothing skip))
	  ((integer? (car specs))
	   (if (> (car specs) 1)
	       (popper (- (car specs) 1) skip)
	       (nothing skip)))
	  (else
	   (loop (cdr specs) (+ skip (arg-spec-size (car specs))))))))

(define (arg-spec-size spec)
  (case spec
    ((nargs byte stob junk) 1)
    ((two-bytes offset small-index index) 2)
    (else
     (error "unknown arg-spec" spec))))

(do ((i 0 (+ i 1)))
    ((= i (vector-length stack-delta)))
  (if (not (vector-ref stack-delta i))
      (vector-set! stack-delta i (stack-function (vector-ref opcode-arg-specs i)))))

;----------------
; Utilities

; Much faster then Scheme's generic function.

(define (imax x y)
  (if (< x y) y x))

