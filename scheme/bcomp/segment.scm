; Copyright (c) 1993, 1994 by Richard Kelsey and Jonathan Rees.
; Copyright (c) 1996 by NEC Research Institute, Inc.    See file COPYING.


; The byte code compiler's assembly phase.

(define make-segment cons)
(define segment-size car);number of bytes that will be taken in the code vector
(define segment-emitter cdr)

(define (segment->template segment name pc-in-parent parent-data)
  (let* ((cv (make-code-vector (segment-size segment) 0))
	 (astate (make-astate cv))
	 (name (if (if (name? name)
		       (keep-procedure-names?)
		       (keep-file-names?))  ;string, or pair, or something
		   name #f))
	 (debug-data (new-debug-data (if (name? name) (name->symbol name) name)
				     parent-data
				     pc-in-parent)))
    (let-fluid $debug-data debug-data
      (lambda ()
	(let ((maps (emit-with-environment-maps! astate segment)))
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
      (template-set! template i (car lits)))))


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

(define (emit-literal! a thing)
  (emit-byte! a
	      (let ((probe (position thing (astate-literals a)))
		    (count (astate-count a)))
		(if probe
		    ;; +++  Eliminate duplicate entries.
		    ;; Not necessary, just a modest space saver [how much?].
		    ;; Measurably slows down compilation.
		    ;; when 1 thing, lits = (x), count = 3, probe = 0, want 2
		    (- (- count probe) 1)
		    (begin
		      (if (>= count byte-limit)
			  (error "compiler bug: too many literals"
				 thing))
		      (set-astate-literals! a (cons thing (astate-literals a)))
		      (set-astate-count! a (+ count 1))
		      count)))))


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

(define (instruction-with-literal opcode thing)
  (make-segment 2
		(lambda (astate)
		  (emit-byte! astate opcode)
		  (emit-literal! astate thing))))

; So are locations.

(define (instruction-with-location opcode thunk)
  (make-segment 2
		(lambda (astate)
		  (emit-byte! astate opcode)
		  ;; But: there really ought to be multiple entries
		  ;; depending on how the name is qualified.  
		  (emit-literal! astate (thunk)))))


; Templates for inferior closures are also obtained from the
; (parent's) template.

(define (instruction-with-template opcode segment name)
  (make-segment 2
		(lambda (astate)
		  (emit-byte! astate opcode)
		  (emit-literal! astate
				 (segment->template segment
						    name
						    (astate-pc astate)
						    (fluid $debug-data))))))

; Labels.  Each label maintains a list of pairs (instr . origin).
; Instr is the index of the first of two bytes that will hold the jump
; target offset, and the offset stored will be (- jump-target origin).

(define (make-label) (list #f))

(define (instruction-using-label opcode label . rest)
  (let ((segment (apply instruction opcode 0 0 rest)))
    (make-segment (segment-size segment)
		  (lambda (astate)
		    (let ((instr (+ (astate-pc astate) 1)))
		      (emit-segment! astate segment)
		      (if (car label)
			  (warn "backward jumps not supported")
			  (set-cdr! label
				    (cons (cons instr (astate-pc astate))
					  (cdr label)))))))))

(define (attach-label label segment)
  (make-segment
     (segment-size segment)
     (lambda (astate)
       (let ((pc (astate-pc astate))
	     (cv (astate-code-vector astate)))
	 (for-each (lambda (instr+origin)
		     (let ((instr (car instr+origin))
			   (origin (cdr instr+origin)))
		       (let ((offset (- pc origin)))
			 (code-vector-set! cv instr
					   (quotient offset byte-limit))
			 (code-vector-set! cv (+ instr 1)
					   (remainder offset byte-limit)))))
		   (cdr label))
	 (set-car! label pc)
	 (emit-segment! astate segment)))))

; byte-limit is larger than the largest value that will fit in one opcode
; byte.

(define byte-limit (expt 2 bits-used-per-byte))


; Special segments for maintaining debugging information.  Not
; essential for proper functioning of compiler.

(define $debug-data (make-fluid #f))

; Keep track of source code at continuations.

(define (note-source-code info segment)
  (if (keep-source-code?)
      (make-segment (segment-size segment)
		    (lambda (astate)
		      (emit-segment! astate segment)
		      (let ((dd (fluid $debug-data)))
			(set-debug-data-source!
			 dd
			 (cons (cons (astate-pc astate)
				     ;; Abbreviate this somehow?
				     (if (pair? info)
					 (cons (car info)
					       (schemify (cdr info)))
					 ;; Name might be generated...
					 info))
			       (debug-data-source dd))))))
      segment))

; Keep track of variable names from lexical environments.
; Each environment map has the form
;    #(pc-before pc-after (var ...) (env-map ...))

(define (note-environment vars segment)
  (if (keep-environment-maps?)
      (make-segment (segment-size segment)
		    (lambda (astate)
		      (let* ((pc-before (astate-pc astate))
			     (env-maps
			      (emit-with-environment-maps! astate segment)))
			(set-fluid! $environment-maps
				    (cons (vector pc-before
						  (astate-pc astate)
						  (list->vector
						   (map name->symbol vars))
						  env-maps)
					  (fluid $environment-maps))))))
      segment))

(define (emit-with-environment-maps! astate segment)
  (let-fluid $environment-maps '()
    (lambda ()
      (emit-segment! astate segment)
      (fluid $environment-maps))))

(define $environment-maps (make-fluid '()))
(define environment-maps-table (make-table))
