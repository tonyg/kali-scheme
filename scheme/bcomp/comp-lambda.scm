; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Compiling LAMBDA.  This is easy, except for the flat environments.

(define-compilator 'lambda syntax-type
  (lambda (node level depth cont)
    (let ((exp (node-form node))
	  (name (cont-name cont)))
      (deliver-value
       (sequentially
	 (instruction (enum op closure))
	 (template (compile-lambda exp level #f)
		   (if (name? name)
		       (name->symbol name)
		       #f))
	 (instruction 0)) ; last byte of closure instruction, 0 means use
                          ; *env* for environment
       cont))))

(define (compile-lambda exp level body-name)
  (let* ((formals (cadr exp))
	 (nargs (number-of-required-args formals))
	 (fast-protocol? (and (<= nargs maximum-stack-args)
			      (not (n-ary? formals)))))
    (sequentially
     ;; Insert protocol
     (cond (fast-protocol?
	    (instruction (enum op protocol) nargs))
	   ((<= nargs available-stack-space)
	    (instruction (enum op protocol)
			 (if (n-ary? formals)
			     two-byte-nargs+list-protocol
			     two-byte-nargs-protocol)
			 (high-byte nargs)
			 (low-byte nargs)))
	   (else
	    (error "compiler bug: too many formals"
		   (schemify exp))))
     (compile-lambda-code formals (caddr exp) level body-name))))

; name isn't the name of the procedure, it's the name to be given to
; the value that the procedure will return.

(define (compile-lambda-code formals body level name)
  (if (null? formals)		;+++ Don't make null environment
      (compile body level 0 (return-cont name))
      ;; (if (node-ref node 'no-inferior-lambdas) ...)
      (sequentially
       (let* ((nargs (number-of-required-args formals))
	      (nargs (if (n-ary? formals)
			 (+ nargs 1)
			 nargs)))
	 (instruction (enum op make-env)
		      (high-byte nargs)
		      (low-byte nargs)))
       (let ((vars (normalize-formals formals))
	     (level (+ level 1)))
	 (set-lexical-offsets! (reverse vars) level)
	 (note-environment
	  (map name-node->symbol vars)
	  (compile body level 0 (return-cont name)))))))

(define (name-node->symbol node)
  (let ((form (node-form node)))
    (cond ((name? form)
	   (name->symbol form))
	  ((symbol? form)
	   form)
	  (else
	   #f))))

; Give each name node in NAMES a binding record that has the names lexical
; level and offset.

(define (set-lexical-offsets! names level)
  (let loop ((over 1) (names names))
    (if (not (null? names))
	(begin
	  (node-set! (car names) 
		     'binding
		     (cons level over))
	  (loop (+ over 1) (cdr names))))))

; (flat-lambda (id ...) (free-id ...) body)

(define-compilator 'flat-lambda syntax-type
  (lambda (node level depth cont)
    (let ((exp (node-form node))
	  (name (cont-name cont)))
      (let ((vars (cadr exp))
	    (free (caddr exp))
 	    (body (cadddr exp)))
 	(deliver-value (compile-flat-lambda name vars body free level)
 		       cont)))))
 
; The MAKE-FLAT-ENV instruction is designed to allow us to make nested flat
; environments (i.e. flat environments consisting of a linked chain of vectors)
; but this code doesn't generate them.
;
; We could sort out the two-byte offsets and make a separate big-flat-env that
; becomes the superior env of the regular flat env (instead of the #F that is
; there now).
;
; The env-key is used when replacing locations after changes to the
; package structure (it is actually the uid of the package we're in).

(define (compile-flat-lambda name vars body free level)
  (call-with-values
   (lambda ()
     (compile-flat-environment free level))
   (lambda (env-code free-vars)
     (sequentially
       (instruction-with-literal (enum op literal) (fluid $env-key))
       env-code
       (instruction (enum op closure))
       (really-compile-flat-lambda name vars body free-vars 0)
       (instruction 1))))) ; last byte of closure instruction, 1 means
                           ; use *val* as environment, 0 means use *env*

; Save the current locations of the free variables, assign them new zero-depth
; locations, compile the lambda, and then restore the old locations.

(define (really-compile-flat-lambda name vars body free level)
  (let ((old-locations (map name-node-binding free)))
    (set-lexical-offsets! free level)
    (let ((code (note-environment (reverse (map node-form free))
				  (template (compile-lambda `(lambda ,vars
							       ,body)
							    level
							    #f)
					    (if (name? name)
						(name->symbol name)
						#f)))))
      (for-each (lambda (node location)
		  (node-set! node 'binding location))
		free
		old-locations)
      code)))

; Returns the code to create the flat environment and the VARS list put in
; the order in which the variables appear in the environment.

(define (compile-flat-environment vars level)
  (let ((alist (sort-list (get-variables-offsets vars level)
			  (lambda (p1 p2)
			    (< (cadr p1)
			       (cadr p2))))))
    (values (environment-code (map cdr alist))
	    (map car alist))))

; Looks up VARS in CENV and returns an alist of (<name> . (<level> <over>))
; pairs.

(define (get-variables-offsets vars level)
  (let loop ((vars vars) (locs '()))
    (if (null? vars)
	locs
	(let ((binding (name-node-binding (car vars))))
	  (if (pair? binding)
	      (let ((back (- level (car binding)))
		    (over (cdr binding)))
		(loop (cdr vars)
		      (cons (cons (car vars)
				  (cons back over))
			    locs)))
	      (error "variable in flat-lambda list is not local"
		     (car vars)))))))

; Emit code to make a flat environment.  There are two opcodes, a fast one
; that only works for small (< one-byte) environments with small (< one-byte)
; offsets (in other words, almost all of them) and one for two-byte sizes 
; and offsets.

(define (environment-code back&overs)		  
  (let ((size (+ (length back&overs) 1)))
    (if (null? back&overs)
	empty-segment
	(sequentially
	  (instruction (enum op false)) ; for first slot of *env*
	  (if (or (< byte-limit size)
		  (any (lambda (p)
			 (< byte-limit (cdr p)))
		       back&overs))
	      (apply instruction
		     (enum op make-big-flat-env)
		     1   ; put *val* instead of *env* in first slot
		     (high-byte size)
		     (low-byte size)
		     (figure-env-data back&overs two-byte-offsetter))
	      (apply instruction
		     (enum op make-flat-env)
		     1   ; put *val* instead of *env* in first slot
		     size
		     (figure-env-data back&overs one-byte-offsetter)))))))

; Addresses is a list of (level . over) pairs, sorted by level.
; This returns the reverse of the following data:
;   <back for level>
;   <number of variables from this level>
;   <over of 1st var> ...
;   <back for next level>
;   ...
; OFFSETTER returns either (<low-byte> <high-byte>) or (<byte>) depending
; on whether offsets are one or two values.

(define (figure-env-data addresses offsetter)
  (let level-loop ((addresses addresses) (last-level 0) (data '()))
    (if (null? addresses)
	(reverse data)
	(let ((level (caar addresses)))
	  (let loop ((addresses addresses) (overs '()) (count 0))
	    (if (or (null? addresses)
		    (not (= level (caar addresses))))
		(level-loop addresses
			    level
			    (append overs
				    (offsetter count)
				    (back-env-data (- level last-level)
						   offsetter)
				    data))
		(loop (cdr addresses)
		      (append (offsetter (cdar addresses))
			      overs)
		      (+ count 1))))))))

(define (one-byte-offsetter offset)
  (list offset))

; This is backwards because the result gets reversed.

(define (two-byte-offsetter offset)
  (list (low-byte offset)
	(high-byte offset)))

; Normally this just returns (<back>).  If <back> is more than will fit in a
; byte we add some length-zero levels to reduce it to reduce it.

(define (back-env-data delta offsetter)
  (let loop ((delta delta)
	     (back '()))
    (if (<= delta byte-limit)
	(cons delta back)
	(loop (- delta byte-limit)
	      (append (offsetter 0)
		      `(,byte-limit . ,back))))))

; We should probably just use the sort from big-scheme.

(define (sort-list xs less?)
  (letrec ((insert (lambda (x xs)
		     (if (null? xs)
			 (list x)
			 (if (less? (car xs) x)
			     (cons (car xs)
				   (insert x (cdr xs)))
			     (cons x xs))))))
    (let sort ((xs xs))    
      (if (null? xs)
	  '()
	  (insert (car xs)
		  (sort (cdr xs)))))))

