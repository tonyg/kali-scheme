; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; The byte code compiler's assembly phase.

(define make-segment cons)
(define segment-size car)
(define segment-emitter cdr)

(define (segment->template segment name pc-in-parent)
  (let* ((cv (make-code-vector (segment-size segment) op/trap))
	 (astate (make-astate cv))
	 (parent-data (fluid $debug-data))
	 (name (if (if (name? name)
		       (keep-procedure-names?)
		       (keep-file-names?))  ;string, or pair, or something
		   name #f))
	 (debug-data (new-debug-data (if (name? name) (name->symbol name) name)
				     parent-data ;(debug-data-if-interesting ?)
				     pc-in-parent)))
    (let-fluid $debug-data debug-data
      (lambda ()
	(let ((maps (emit-with-environment-maps! astate segment)))
	  (set-debug-data-env-maps! debug-data maps)
	  (segment-data->template cv
				  (debug-data->info debug-data)
				  (astate-literals astate)))))))

(define (segment-data->template cv debug-data literals)
  (let ((template (make-template (+ template-overhead (length literals)) 0)))
    (set-template-code! template cv)
    (set-template-info! template debug-data)
    (do ((lits literals (cdr lits))
	 (i 2 (+ i 1)))
	((null? lits) template)
      (template-set! template i (car lits)))))

; "astate" is short for "assembly state"

(define (make-astate cv)
  (let ((pc 0)
	(count 0)
	(lits '()))

    (define (emit-byte! byte)
      (code-vector-set! cv pc byte)
      (set! pc (+ pc 1)))

    (define (emit-literal! thing)
      (emit-byte!
       (let ((probe (position thing lits)))
	 (if probe
	     ;; +++  Eliminate duplicate entries.
	     ;; Not necessary, just a modest space saver [how much?].
	     ;; Measurably slows down compilation.
	     (+ 1 (- count probe))
	     (begin
	       (if (>= count byte-limit)
		   (error "compiler bug: too many literals"
			  thing))
	       (set! lits (cons thing lits))
	       (set! count (+ count 1))
	       (+ 1 count))))))

    (define (get-literals) (reverse lits))

    (define (get-pc) pc)

    (vector emit-byte! emit-literal! get-literals get-pc)))

(define (emit-byte! astate byte)
  ((vector-ref astate 0) byte))

(define (emit-literal! astate thing)
  ((vector-ref astate 1) thing))

(define (astate-literals astate)
  ((vector-ref astate 2)))

(define (astate-pc astate)
  ((vector-ref astate 3)))

(define (emit-segment! astate segment)
  ((segment-emitter segment) astate))

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
  (reduce sequentially-2 empty-segment segments))

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

(define (instruction-with-variable opcode name den assign?)
  (make-segment 2
		(lambda (astate)
		  (emit-byte! astate opcode)
		  (emit-location! astate name den assign?))))

; Name is ignored for now, but is passed in because it will eventually
; be needed for separate compilation.  The name (possibly generated)
; will have to be written to the object file so that it can be looked
; up when the object file is loaded relative to some existing
; environment.

(define (emit-location! astate name den assign?)
  ;; Should have different entries depending on what the lookup path was.
  (emit-literal! astate
		 (package-lookup-location (car den) (cdr den) assign?)))


; Templates for inferior closures are also obtained from the
; (parent's) template.

(define (instruction-with-template opcode segment name)
  (make-segment 2
		(lambda (astate)
		  (emit-byte! astate opcode)
		  (emit-literal! astate
				 (segment->template segment
						    name
						    (astate-pc astate))))))

; 

(define (instruction-with-offset opcode offset)
  (instruction opcode
	       (quotient offset byte-limit)
	       (remainder offset byte-limit)))

(define (instruction-with-offset&byte opcode offset byte)
  (instruction opcode
	       (quotient offset byte-limit)
	       (remainder offset byte-limit)
	       byte))

(define byte-limit (expt 2 bits-used-per-byte))


(define $debug-data (make-fluid #f))


; Special segments for maintaining debugging information.  Not
; essential for proper functioning of compiler.

; Keep track of source code at continuations.

(define (note-source-code info segment)
  (if (keep-source-code?)
      (make-segment (segment-size segment)
		    (lambda (astate)
		      (emit-segment! astate segment)
		      (let ((dd (fluid $debug-data)))
			(set-debug-data-source!
			 dd
			 (cons (cons (astate-pc astate) (desyntaxify info))
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
