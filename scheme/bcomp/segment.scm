; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; The byte code compiler's assembly phase.

(define make-segment cons)
(define segment-size car);number of bytes that will be taken in the code vector
(define segment-emitter cdr)

(define (segment->template segment name pc-in-parent parent-data)
  (let* ((cv (make-code-vector (segment-size segment) 0))
	 (astate (make-astate cv))
	 (name (if (if (string? name)	; only files have strings for names
		       (keep-file-names?)
		       (keep-procedure-names?))
		   name
		   #f))
	 (debug-data (new-debug-data name parent-data pc-in-parent)))
    (let-fluid $debug-data debug-data
      (lambda ()
	(let* ((maps (emit-with-environment-maps! astate segment))
	       (cv (check-stack-use cv)))
	  (set-debug-data-env-maps! debug-data maps)
	  (make-immutable! cv)
	  (segment-data->template cv
				  (debug-data->info debug-data)
				  (reverse (astate-literals astate))))))))

(define (segment-data->template cv debug-data literals)
  (let ((template (make-template (+ template-overhead (length literals)) 0)))
    (set-template-code! template cv)
    (set-template-info! template debug-data)
    (do ((lits literals (cdr lits))
	 (i template-overhead (+ i 1)))
	((null? lits) template)
      (template-set! template i (car lits)))
    template))

; If CV needs more than the default allotment of stack space replace its
; protocol with one that checks that the needed space is available.  The
; original protocol is preserved at the end of the new code vector (to
; preserve the debugging indicies into the original).

(define (check-stack-use cv)
  (let ((uses (maximum-stack-use cv)))
    (cond ((<= uses default-stack-space)
	   cv)
	  ((<= uses available-stack-space)
	   (let* ((length (code-vector-length cv))
		  (new (make-code-vector (+ length 3) 0)))
	     (do ((i 0 (+ i 1)))
		 ((= i length))
	       (code-vector-set! new i (code-vector-ref cv i)))
	     (code-vector-set! new length (code-vector-ref cv 1))
	     (code-vector-set! new 1 big-stack-protocol)
	     (code-vector-set2! new (+ length 1) uses)
	     new))
	  (else
	   (error "VM limit exceeded: procedure requires too much stack space")))))

; "astate" is short for "assembly state"

(define-record-type assembly-state :assembly-state
  (make-assembly-state cv pc count lits)
  (cv    astate-code-vector)
  (pc    astate-pc    set-astate-pc!)
  (count astate-count set-astate-count!)
  (lits  astate-literals  set-astate-literals!))

(define (make-astate cv)
  (make-assembly-state cv 0 template-overhead '()))

(define (emit-byte! a byte)
  (code-vector-set! (astate-code-vector a) (astate-pc a) byte)
  (set-astate-pc! a (+ (astate-pc a) 1)))

(define (literal->index a thing)
  (let ((probe (literal-position thing (astate-literals a)))
	(count (astate-count a)))
    (if probe
	;; +++  Eliminate duplicate entries.
	;; Not necessary, just a modest space saver [how much?].
	;; Measurably slows down compilation.
	;; when 1 thing, lits = (x), count = 3, probe = 0, want 2
	(- (- count probe) 1)
	(begin
	  (if (>= count two-byte-limit)
	      (error "compiler bug: too many literals"
		     thing))
	  (set-astate-literals! a (cons thing (astate-literals a)))
	  (set-astate-count! a (+ count 1))
	  count))))

(define (literal-position thing literals)
  (position (if (thingie? thing)
		(lambda (thing other-thing)
		  (and (thingie? other-thing)
		       (equal? (thingie-name thing)
			       (thingie-name other-thing))))
		equal?)
	    thing
	    literals))

(define (position pred elt list)
  (let loop ((i 0) (l list))
    (cond ((null? l)
	   #f)
	  ((pred elt (car l))
	   i)
	  (else
	   (loop (+ i 1) (cdr l))))))

(define (emit-literal! a thing)
  (let ((index (literal->index a thing)))
    (emit-byte! a (high-byte index))
    (emit-byte! a (low-byte index))))

(define (emit-segment! astate segment)
  ((segment-emitter segment) astate))

; Segment constructors

(define empty-segment
  (make-segment 0 (lambda (astate) #f)))

(define (instruction opcode . operands)
  (make-segment (+ 1 (length operands))
		(lambda (astate)
		  (emit-byte! astate opcode)
		  (for-each (lambda (operand)
			      (emit-byte! astate operand))
			    operands))))

(define (sequentially . segments)
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

; Literals are obtained from the template.

(define (instruction-with-literal opcode thing . operands)
  (make-segment (+ 3 (length operands))
		(lambda (astate)
		  (let ((index (literal->index astate thing)))
		    (if (and (= opcode (enum op literal))
                             (< index byte-limit))
			(begin
			  (emit-byte! astate (enum op small-literal))
			  (emit-byte! astate index)
			  (emit-byte! astate 0))
			(begin
			  (emit-byte! astate opcode)
			  (emit-byte! astate (high-byte index))
			  (emit-byte! astate (low-byte index))))
		    (for-each (lambda (operand)
				(emit-byte! astate operand))
			      operands)))))

; So are locations.

(define (instruction-with-location opcode binding name want-type)
  (make-segment 3
		(lambda (astate)
		  (emit-byte! astate opcode)
		  (emit-literal! astate (make-thingie binding name want-type)))))

; Templates for inferior closures are also obtained from the
; (parent's) template.

(define (template segment name)
  (make-segment 2
  		(lambda (astate)
  		  (emit-literal! astate
  				 (segment->template segment
  						    name
						    (astate-pc astate)
						    (fluid $debug-data))))))

; Labels.  Each label maintains a list of pairs (location . origin).
; Instr is the index of the first of two bytes that will hold the jump
; target offset, and the offset stored will be (- jump-target origin).
;
; The car of a forward label is #F, the car of a backward label is the
; label's PC.

(define (make-label) (list #f))

(define (instruction-using-label opcode label . rest)
  (let ((segment (apply instruction opcode 0 0 rest)))
    (make-segment (segment-size segment)
		  (lambda (astate)
		    (let* ((origin (astate-pc astate))
			   (location (+ origin 1)))
		      (emit-segment! astate segment)
		      (if (car label)
			  (insert-label! (astate-code-vector astate)
					 location
					 (- (car label) origin))
			  (set-cdr! label
				    (cons (cons location origin)
					  (cdr label)))))))))

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

(define two-byte-limit (expt 2 (* 2 bits-used-per-byte)))

(define (high-byte n)
  (quotient n byte-limit))

(define (low-byte n)
  (remainder n byte-limit))

; Special segments for maintaining debugging information.  Not
; essential for proper functioning of compiler.

(define $debug-data (make-fluid #f))

; Keep track of source code at continuations.

(define (note-source-code info segment)
  (make-segment (segment-size segment)
		(lambda (astate)
		  (emit-segment! astate segment)
		  (let ((dd (fluid $debug-data)))
		    (set-debug-data-source!
		     dd
		     (cons (cons (astate-pc astate) info)
			   (debug-data-source dd)))))))

; Keep track of variable names from lexical environments.
; Each environment map has the form
;    #(pc-before pc-after (var ...) (env-map ...))

(define (note-environment vars segment)
  (if (keep-environment-maps?)
      (make-segment (segment-size segment)
		    (lambda (astate)
		      (let* ((pc-before (astate-pc astate))
			     (env-maps
			      (emit-with-environment-maps! astate segment))
			     (cell (fluid $environment-maps)))
			(if cell
			    (cell-set! cell
				       (cons (vector pc-before
						     (astate-pc astate)
						     (list->vector vars)
						     env-maps)
					     (cell-ref cell)))))))
      segment))

(define (emit-with-environment-maps! astate segment)
  (let ((cell (make-cell '())))
    (let-fluid $environment-maps cell
      (lambda ()
	(emit-segment! astate segment)))
    (cell-ref cell)))

(define $environment-maps (make-fluid #f))
