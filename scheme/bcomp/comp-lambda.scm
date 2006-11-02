; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; We only do flat lambdas now.

(define-compilator 'lambda syntax-type
  (lambda (node depth frame cont)
    (generate-trap depth
		   frame
		   cont
		   "cannot compile non-flat lambda")))

;----------------------------------------------------------------
; (flat-lambda (id ...) (free-id ...) body)

(define-compilator 'flat-lambda syntax-type
  (lambda (node depth frame cont)
    (let ((exp (node-form node))
	  (name (cont-name cont)))
      (let ((vars (cadr exp))
	    (free (caddr exp))
 	    (body (cadddr exp)))
 	(deliver-value (compile-flat-lambda name vars body free depth frame)
 		       cont)))))
 
; The MAKE-FLAT-ENV instruction is designed to allow us to make nested flat
; environments (i.e. flat environments consisting of a linked chain of vectors)
; but this code doesn't generate them.
;
; We could sort out the two-byte offsets and make a separate big-flat-env that
; becomes the superior env of the regular flat env (instead of the #f that is
; there now).

(define (compile-flat-lambda name vars body free depth frame)
  (receive (env-code free-vars)
      (if (null? free)			; avoid ENVIRONMENT-OFFSET if no env
	  (values (instruction (enum op false)) '())
	  (compile-environment free (+ depth 1)))
    (sequentially
      (really-compile-flat-lambda name vars body free-vars depth frame)
      env-code
      (instruction (enum op make-stored-object) 2 (enum stob closure))
      (instruction (enum op make-immutable!)))))

; Save the current locations of the free variables, compile the lambda, and
; then restore the old locations.

(define (really-compile-flat-lambda name vars body free depth frame)
  (let ((old-locations (map name-node-binding free)))
    (receive (proc-code proc-frame)
	(compile-lambda `(lambda ,vars ,body)
			free
			name
			#f
			frame)
      (for-each (lambda (node location)
		  (node-set! node 'binding location))
		free
		old-locations)
      (let ((template (segment->template proc-code proc-frame)))
	(let ((offset (template-offset frame depth))
	      (index (literal->index frame template)))
	  (or (push+stack-indirect-instruction offset index #f)
	      (sequentially (stack-indirect-instruction offset index)
			    push-instruction)))))))

; This is called by REALLY-COMPILE-FLAT-LAMBDA above and by the compilator
; for PURE-LETREC.

(define (compile-lambda exp free lambda-name body-name frame)
  (let* ((formals (cadr exp))
	 (nargs (number-of-required-args formals))
	 (n-ary? (n-ary? formals))
	 (stack-nargs (if n-ary? (+ nargs 1) nargs))
	 (need-env? (not (null? free)))		;+++  ; could just be #t
	 (frame (make-frame frame lambda-name stack-nargs #t need-env? #f))
	 (extras (if need-env? 2 1)))
    (set-lexical-offsets! free stack-nargs)
    (let ((code (compile-lambda-code formals
				     free
				     (caddr exp)
				     (+ stack-nargs extras)
				     extras
				     frame
				     body-name)))
      (values (sequentially
	        (if n-ary?
                    (nary-lambda-protocol nargs #t need-env? #f)
                    (lambda-protocol nargs #t need-env? #f))
		code)
	      frame))))

; Give each name node in NAMES a binding record that has its environment's
; stack index and the name's offset within that environment.

(define (set-lexical-offsets! names stack-index)
  (let loop ((over 0) (names names))
    (if (not (null? names))
	(begin
	  (node-set! (car names) 
		     'binding
		     (list stack-index over))
	  (loop (+ over 1) (cdr names))))))

; NAME isn't the name of the procedure, it's the name to be given to
; the value that the procedure will return.
;
; EXTRA is a count of any additional values that may be on the stack above
; the arguments, for example the environment and template.

(define (compile-lambda-code formals free body depth extra frame name)
  (let* ((plain-nargs (number-of-required-args formals))
	 (is-n-ary? (n-ary? formals))
	 (nargs (if is-n-ary?
		    (+ plain-nargs 1)
		    plain-nargs))
	 (vars (normalize-formals formals)))
    (set-frame-locations! vars (- depth extra))
    (note-environment (let ((args (map name-node->symbol vars)))
			(if (null? free)
			    args
			    (append args
				    (list (map node-form free)))))
		      0
		      (compile body depth frame (return-cont name)))))

; Mark NAMES as being at (- DEPTH 1) and on down in the current frame.

(define (set-frame-locations! names depth)
  (let loop ((index (- depth 1)) (names (reverse names)))
    (if (not (null? names))
	(begin
	  (node-set! (car names) 
		     'binding
		     (list index))
	  (loop (- index 1) (cdr names))))))

(define (name-node->symbol node)
  (let ((form (node-form node)))
    (cond ((name? form)
	   (name->symbol form))
	  ((symbol? form)
	   form)
	  (else
	   #f))))

;----------------------------------------------------------------
; Returns the code to create the flat environment and the VARS list put in
; the order in which the variables appear in the environment.
;
; An [BIG-]FLAT-ENV instruction looks like:
;
; (enum op make-[big-]flat-env)
; number of vars
; number of closures 
; [offset of template in frame
;  offsets of templates in template]
; number of variables in frame
; offsets of vars in frame
; [offset of env in frame
;  number of vars in env
;  offsets of vars in level]*
;
; For MAKE-FLAT-ENV all values are one byte and for MAKE-BIG-FLAT-ENV they
; are two bytes.
;
; COMPILE-ENVIRONMENT produces flat environments with no closures.  The
; PURE-LETREC compilator calls COMPILE-RECURSIVE-ENVIRONMENT to create
; environments that contain closures closed over that same environment.

(define (compile-environment vars depth)
  (compile-recursive-environment vars
				 depth
				 0
				 (lambda (vars-in-order) '())))

; The code generator for PURE-LETREC calls this.  It needs the VARS-IN-ORDER
; list in order to compile the templates that are used in the recursive
; procedures closed over the flat enviornment.

(define (compile-recursive-environment vars depth template-offset index-proc)
  (receive (env-code vars-in-order)
      (flat-environment-code vars depth)
    (values (finish-flat-env (length vars-in-order)
			     env-code
			     template-offset
			     (index-proc vars-in-order))
	    vars-in-order)))

; Emit code to make a flat environment.  There are two opcodes, a fast one
; that only works for small (< one-byte) environments with small (< one-byte)
; offsets (in other words, almost all of them) and one for two-byte sizes 
; and offsets.

(define (finish-flat-env var-count env-code template-offset template-indexes)
  (let ((code-bytes `(,(+ var-count
			  (length template-indexes))
		      ,(length template-indexes)
		      ,@(if (null? template-indexes)
			    '()
			    (cons template-offset template-indexes))
		      . ,env-code)))
    (if (any (lambda (b)
	       (<= byte-limit b))
	     code-bytes)
	(apply instruction
	       (enum op make-big-flat-env)
	       (one-byte->two-byte code-bytes))
	(apply instruction
	       (enum op make-flat-env)
	       code-bytes))))

; Break up a list of numbers into their high bytes and low bytes.

(define (one-byte->two-byte code-bytes)
  (let loop ((data (reverse code-bytes)) (res '()))
    (if (null? data)
	res
	(loop (cdr data)
	      (cons (high-byte (car data))
		    (cons (low-byte (car data))
			  res))))))

; Actually make the code.  FRAME is a list of (<variable> . <offset>) for
; variables in VARS that are in the current stack frame.  INDIRECT is a list
; of lists of the form (<offset> (<variable> <index>) ...) indicating that
; <variable> is found at <index> in the vector at <offset> in the current
; frame.  This calls FIGURE-ENV-DATA to make the actual code and constructs
; a copy of VARS that has the variables in the order in which they will appear
; in the environment (to be passed to NOTE-ENVIRONMENT for eventual use by
; the debugger).

(define (flat-environment-code vars depth)
  (receive (frame indirect)
      (get-variables-locations vars depth)
    (values (figure-env-data (map cdr frame)
			     indirect)
	    (apply append
		   (map car frame)
		   (map (lambda (indirect)
			  (map car (cdr indirect)))
			indirect)))))

; Translates VARS into two lists:
; - ((<variable> . <offset>) ...) for those variables that are in the
;    current frame
; - ((<offset> (<variable> <index>) ...) ...) indicating <variable> is at
;    <index> in the vector at <offset> in the current frame

(define (get-variables-locations vars depth)
  (let loop ((vars vars) (frame '()) (other '()))
    (if (null? vars)
	(values frame other)
	(let* ((var (car vars))
	       (binding (name-node-binding var)))
	  (if (pair? binding)
	      (let ((offset (index->offset (car binding) depth)))
		(if (null? (cdr binding))
		    (loop (cdr vars)
			  (cons (cons var offset)
				frame)
			  other)
		    (loop (cdr vars)
			  frame
			  (add-variable var offset (cdr binding) other))))
	      (error "variable in flat-lambda list is not local"
		     (car vars)))))))

; Add VAR, with stack-offset OFFSET and MORE other indexes, to OTHER, an alist
; indexed by offsets.  Currently MORE always has lenth one.

(define (add-variable var offset more other)
  (let ((have (assq offset other)))
    (if have
	(begin
	  (set-cdr! have (cons (cons var more)
			       (cdr have)))
	  other)
	`((,offset (,var . ,more))
	  . ,other))))

; Convert the frame offsets and indirect information into the form used by the
; MAKE{-BIG}-FLAT-ENV opcode by adding length information at appropriate points
; and eliding the variables in INDIRECTS.

(define (figure-env-data frame-offsets indirects)
  `(,(length frame-offsets)
    ,@frame-offsets
    . ,(let loop ((indirects indirects) (data '()))
	 (if (null? indirects)
	     (reverse data)
	     (loop (cdr indirects)
		   (append (reverse (map cadr (cdar indirects)))
			   (list (length (cdar indirects))
				 (caar indirects))
			   data))))))
