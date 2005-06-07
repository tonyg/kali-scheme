
; A frame contains information about a procedure's current stack frame.  It
; also has a list of the literals that will go in the procedure's template
; and the debugging data for the template.
;
; env-index      - the location of this procedure's environment in the frame
;                  (#f if the procedure does not use its environment)
; template-index - the location of this procedure's template in the frame
;                  (again, this is #f if the template is not needed)
; size		 - largest size reached by the frame, in descriptors
; literals	 - list of literals and bindings referenced
; count		 - length of LITERALS
; debug-data	 - debug information (see ddata.scm)

(define-record-type frame :frame
  (really-make-frame literals count debug-data env-index template-index size)
  frame?
  (env-index      frame-env-index)
  (template-index frame-template-index)
  (size           frame-size     set-frame-size!)
  (literals       frame-literals set-frame-literals!)
  (count	  frame-count    set-frame-count!)
  (debug-data     frame-debug-data))

; SIZE is the number of values on the stack when the procedure is entered
; (typically the number of arguments).  ENV? is true if the environment was
; pushed on after the arguments, TEMPLATE? is true if the template was
; pushed as well.

(define (make-frame parent name size env? template?)
  (let* ((ddata (new-debug-data (adjust-procedure-name name)
				(if parent
				    (frame-debug-data parent)
				    #f)))
	 (finish (lambda (env-index template-index size)
		   (really-make-frame '()
				      0
				      ddata
				      env-index
				      template-index
				      size))))
    (if env?
	(if template?
	    (finish size (+ size 1) (+ size 2))
	    (finish size #f         (+ size 1)))
	(if template?
	    (finish #f   size       (+ size 1))
	    (finish #f   #f         size)))))

(define (adjust-procedure-name name)
  (cond ((string? name)			; only files have strings for names
	 (if (keep-file-names?)
	     name
	     #f))
	((and (keep-procedure-names?)
	      (name? name))
	 (name->symbol name))
	(else
	 #f)))

; Convert an index, which is relative to the base of the frame, to an offset
; from the current stack pointer.

(define (index->offset index depth)
  (- depth (+ index 1)))

; Offsets for the template and environment.

(define (template-offset frame depth)
  (index->offset (frame-template-index frame)
		 depth))

(define (environment-offset frame depth)
  (index->offset (frame-env-index frame)
		 depth))

; Note that FRAME reaches a size of DEPTH.

(define (depth-check! frame depth)
  (if (< (frame-size frame)
	 depth)
      (set-frame-size! frame depth)))

; These two procedures look up bindings and literals in the list of values
; to go in the template.  They're added if not already present.  The returned
; index is that of template, not the frame's list.

(define (binding->index frame binding name want-type)
  (literal->index frame
		  (make-thingie binding name want-type)))

(define (literal->index frame thing)
  (let ((probe (literal-position thing (frame-literals frame)))
	(count (frame-count frame)))
    (if probe
	;; +++  Eliminate duplicate entries.
	;; Not necessary, just a modest space saver [how much?].
	;; Measurably slows down compilation.
	;; when 1 thing, lits = (x), count = 1, probe = 0, want 2
	(+ (- count probe)
	   (- template-overhead 1))
	(begin
	  (if (>= count two-byte-limit)
	      (error "compiler bug: too many literals"
		     thing))
	  (set-frame-literals! frame
			       (cons thing
				     (frame-literals frame)))
	  (set-frame-count! frame (+ count 1))
	  ;; when 1st thing, count = 0, want 2
	  (+ count template-overhead)))))

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

