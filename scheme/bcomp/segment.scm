; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; The byte code compiler's assembly phase.

(define make-segment cons)
(define segment-size car);number of bytes that will be taken in the code vector
(define segment-emitter cdr)

(define (segment->template segment frame)
  (let* ((big-stack? (check-stack-use (frame-size frame)))
	 (cv (make-code-vector (+ (segment-size segment)
				  (if big-stack? 3 0))
			       0))
	 (astate (make-astate cv)))
    (emit-segment! astate segment)
    (if big-stack?
	(add-big-stack-protocol! cv (frame-size frame)))
    (set-debug-data-env-maps! (frame-debug-data frame)
			      (astate-env-maps astate))
    (make-immutable! cv)
    (segment-data->template cv
			    (debug-data->info (frame-debug-data frame))
			    (reverse (frame-literals frame)))))

(define (segment-data->template cv debug-data literals)
  (let ((template (make-template (+ template-overhead (length literals)) 0)))
    (set-template-code! template cv)
    (set-template-info! template debug-data)
    (set-template-package-id! template (fluid $package-key))
    (do ((lits literals (cdr lits))
	 (i template-overhead (+ i 1)))
	((null? lits) template)
      (template-set! template i (car lits)))
    template))

(define $package-key (make-fluid #f))

(define (with-package-key package-key thunk)
  (let-fluid $package-key package-key thunk))

; If CV needs more than the default allotment of stack space we add a new
; protocol onto the front.

(define (check-stack-use frame-size)
  (cond ((<= frame-size default-stack-space)
	 #f)
	((<= frame-size available-stack-space)
	 #t)
	(else
	 (error "VM limit exceeded: procedure requires too much stack space"
		frame-size))))

; We put the length and the original protocol at the end of the code vector
; so that the original protocol's data doesn't have to be moved (which would
; complicate the already-complicated VM code for protocol dispatch).

(define (add-big-stack-protocol! cv frame-size)
  (let ((length (code-vector-length cv)))
    (code-vector-set! cv (- length 3) (code-vector-ref cv 1))
    (code-vector-set! cv (- length 2) (high-byte frame-size))
    (code-vector-set! cv (- length 1) (low-byte  frame-size))
    (code-vector-set! cv 1 big-stack-protocol)))

; "astate" is short for "assembly state"

(define-record-type assembly-state :assembly-state
  (make-assembly-state cv pc env-maps)
  (cv       astate-code-vector)
  (pc       astate-pc       set-astate-pc!)
  (env-maps astate-env-maps set-astate-env-maps!))

(define (make-astate cv)
  (make-assembly-state cv 0 '()))

(define (emit-byte! a byte)
  (code-vector-set! (astate-code-vector a) (astate-pc a) byte)
  (set-astate-pc! a (+ (astate-pc a) 1)))

(define (emit-segment! astate segment)
  ((segment-emitter segment) astate))

; Segment constructors

(define empty-segment
  (make-segment 0 (lambda (astate) #f)))

(define (instruction opcode . operands)
  (make-segment (+ 1 (length operands))
		(lambda (astate)
;                  (format #t "[emit ~D(~D) -> ~S ~S]~%"
;                          (astate-pc astate)
;                          (code-vector-length (astate-code-vector astate))
;                          (enumerand->name opcode op)
;                          (cons opcode operands))
		  (emit-byte! astate opcode)
		  (for-each (lambda (operand)
			      (emit-byte! astate operand))
			    operands))))

(define (sequentially . segments)
  (if (not (car segments))
      (error "bad call to SEQUENTIALLY"))
  ;;  (reduce sequentially-2 empty-segment segments)
  ;;+++ this sped the entire compilation process up by several percent
  (cond ((null? segments)
         empty-segment)
        ((null? (cdr segments))
         (car segments))
        ((null? (cddr segments))
         (sequentially-2 (car segments) (cadr segments)))
        (else
         (make-segment (let loop ((segs segments) (s 0))
                         (if (null? segs)
                             s
                             (loop (cdr segs) (+ s (segment-size (car segs))))))
                       (lambda (astate)
                         (let loop ((segs segments))
                           (if (not (null? segs))
                               (begin (emit-segment! astate (car segs))
                                      (loop (cdr segs))))))))))

(define (sequentially-2 seg1 seg2)
  (cond ((eq? seg1 empty-segment) seg2) ;+++ speed up the compiler a tad
	((eq? seg2 empty-segment) seg1) ;+++
	(else
	 (make-segment (+ (segment-size seg1)
			  (segment-size seg2))
		       (lambda (astate)
			 (emit-segment! astate seg1)
			 (emit-segment! astate seg2)))))) ;tail call

(define continuation-data-size 8)

(define (continuation-data depth)
  (make-segment continuation-data-size
		(lambda (astate)
		  (let ((offset (+ (astate-pc astate)
				   continuation-data-size)))
		    (emit-byte! astate (enum op cont-data))
		    (emit-byte! astate 0)    ; high and low bytes of the size
		    (emit-byte! astate continuation-data-size)
		    (emit-byte! astate (high-byte offset))
		    (emit-byte! astate (low-byte  offset))
		    (emit-byte! astate 0)		; GC mask size
		    (emit-byte! astate (high-byte depth))
		    (emit-byte! astate (low-byte depth))))))

; Labels.  Each label maintains a list of pairs (location . origin).
; Instr is the index of the first of two bytes that will hold the jump
; target offset, and the offset stored will be (- jump-target origin).
;
; The car of a forward label is #F, the car of a backward label is the
; label's PC.

(define (make-label) (list #f))

(define (label-reference before label after)
  (let ((segment (sequentially before
			       (instruction 0 0)
			       after)))
    (make-segment (segment-size segment)
		  (lambda (astate)
		    (let* ((origin (astate-pc astate))
			   (location (+ origin (segment-size before))))
		      (emit-segment! astate segment)
		      (if (car label)
			  (insert-label! (astate-code-vector astate)
					 location
					 (- (car label) origin))
			  (set-cdr! label
				    (cons (cons location origin)
					  (cdr label)))))))))

(define (instruction-using-label opcode label . rest)
  (label-reference (instruction opcode)
		   label
		   (bytes->segment rest)))

(define (optional-label-reference before maybe-label after)
  (if maybe-label
      (label-reference before maybe-label after)
      (sequentially before
		    (instruction 0 0)
		    after)))

(define (using-optional-label opcode maybe-label . rest)
  (optional-label-reference (instruction opcode)
			    maybe-label
			    (bytes->segment rest)))

(define (bytes->segment bytes)
  (make-segment (length bytes)
		(lambda (astate)
		  (for-each (lambda (operand)
			      (emit-byte! astate operand))
			    bytes))))

; computed-goto
; # of labels
; label0
; label1
; ...

(define computed-goto-label-size 2)

(define (computed-goto-instruction labels)
  (let* ((count (length labels))
	 (segment (instruction (enum op computed-goto) count)))
    (make-segment (+ (segment-size segment)
		     (* count computed-goto-label-size))
		  (lambda (astate)
		    (let ((base-address (astate-pc astate)))
		      (emit-segment! astate segment)
		      (set-astate-pc! astate
				      (+ (astate-pc astate)
					 (* count computed-goto-label-size)))
		      (do ((location (+ base-address 2)
				     (+ location computed-goto-label-size))
			   (labels labels (cdr labels)))
			  ((null? labels))
			(let ((label (car labels)))
			  (if (car label)
			      (warn "backward jumps not supported")
			      (set-cdr! label
					(cons (cons location base-address)
					      (cdr label)))))))))))

; LABEL is the label for SEGMENT.  The current PC is used as the value of LABEL.

(define (attach-label label segment)
  (make-segment
     (segment-size segment)
     (lambda (astate)
       (let ((pc (astate-pc astate))
	     (cv (astate-code-vector astate)))
	 (for-each (lambda (instr+origin)
		     (insert-label! cv
				    (car instr+origin)
				    (- pc (cdr instr+origin))))
		   (cdr label))
	 (set-car! label pc)
	 (emit-segment! astate segment)))))

(define (insert-label! cv location offset)
  (code-vector-set2! cv location offset))

(define (code-vector-set2! cv i value)
  (code-vector-set! cv i       (high-byte value))
  (code-vector-set! cv (+ i 1) (low-byte  value)))

(define (high-byte n)
  (quotient n byte-limit))

(define (low-byte n)
  (remainder n byte-limit))

; Keep track of source code at continuations.

(define (note-source-code info segment frame)
  (make-segment (segment-size segment)
		(lambda (astate)
		  (let ((dd (frame-debug-data frame)))
		    (set-debug-data-source!
		     dd
		     (cons (cons (astate-pc astate) info)
			   (debug-data-source dd))))
		  (emit-segment! astate segment))))

; Keep track of variable names from lexical environments.
; Each environment map has the form
;    #(pc-before pc-after (var ...) (env-map ...))
;
; It's a bit more complex now.  Variables are found in the frame itself and
; in vectors within the frame.
;  #(pc-before pc-after offset names more)
; We need a way to distinguish between names in the frame and names in vectors.
; Put the vector ones in lists.
;  (lambda (x y)
;    (lambda (a b)
;       ...))
;  -> (0 <last-pc> 0 (a b (x y)) . more)
; The (X Y) are in the free-variable vector.
;
; Could also add PC's that correspond to calls to mark the values with
; the source that they were returned from.

(define (note-environment vars offset segment)
  (if (keep-environment-maps?)
      (make-segment
        (segment-size segment)
	(lambda (astate)
	  (let* ((pc-before (astate-pc astate))
		 (old (astate-env-maps astate)))
	    (set-astate-env-maps! astate '())
	    (emit-segment! astate segment)
	    (let ((new (astate-env-maps astate)))
	      (set-astate-env-maps! astate
				    (cons (vector pc-before
						  (astate-pc astate)
						  offset
						  (list->vector vars)
						  new)
					  old))))))
      segment))
