; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


;  packages packages-internal scan compiler table
;  syntactic vm-exposure signals locations fluids template
;  closures types inline dump/restore
;  environments

; Separate compilation

; Setting the get-location method isn't sufficient because it won't
; intercept locations in already existing structures (e.g. scheme)...
;
;   cf. compile-structures in link.scm

; Hacking the environment lookup mechanism to modify bindings on the
; way out won't work, because it might cause denotation comparison to
; fail during macro expansion...

; So I think the best we can do is to maintain a location -> reference map.
; There may be many routes to any particular location, but we'll only
; be able to remember one of them.
; (Actually, we _could_ remember all of them and then check at load time to
; make sure that they all agree.)

; The filtered environment also ought to be passed to the scanner,
; because it caches looked-up bindings in nodes.  The effect of not
; doing this is to get warning at compile time, and unbound variables
; at load time.


(define (compile-package-to-file p filename)
  (let* ((table (make-table location-id))
	 (env (package->separate p table))
	 (stuff (scan-package p env))
	 (templates '()))
    (for-each (lambda (filename+nodes)
		(set! templates
		      (cons (compile-scanned-forms
			     (cdr filename+nodes)
			     p
			     (car filename+nodes)
			     (current-output-port)
			     env)
			    templates)))
	      stuff)
    (call-with-output-file filename
      (lambda (port)
	(fasdump (reverse templates) p table port)))))

(define (package->separate p table)
  (let ((cenv (package->environment p)))
    (lambda (name)
      (let ((probe (cenv name)))
        (if (and (pair? probe)
		 (location? (cdr probe))
		 (not (table-ref table (cdr probe))))
	    (table-set! table
			(cdr probe)
			(cons (name->qualified name)
			      (let ((type (binding-type probe)))
				(if (equal? type usual-variable-type)
				    #f
				    type)))))
	probe))))

(define *level* 0)

(define (fasdump templates p table port)
  (let* ((write-char (lambda (c)
		       (write-char c port)))
	 (dump (lambda (thing)
		 (dump thing write-char -1))))
    (dump *level*)
    (dump (map structure-name (package-opens p)))  ;lose
    (dump (map car (package-accesses p)))
    (table-walk (lambda (loc qname+type)
		  (dump (location-id loc))
		  (dump qname+type))
		table)
    (dump '-)
    (let-fluid $dump-index (lambda (loc)
			     (if (table-ref table loc)
				 (location-id loc)
				 (begin (warn "lose" loc) #f)))
      (lambda ()
	(dump templates)))))

(define (fasload filename name->structure)
  (call-with-input-file filename
    (lambda (port)
      (let* ((read-char (lambda () (read-char port)))
	     (restore (lambda () (restore read-char)))
	     (table (make-table))
	     (level (restore)))
	(if (not (equal? level *level*))
	    (warn "format revision level disagreement - try recompiling"
		  `(file: ,level current: ,*level*)))
	(let* ((open-names (restore))
	       (access-names (restore))
	       (p (make-package (lambda () (map name->structure open-names))
				(lambda ()
				  (map (lambda (name)
					 (cons name
					       (name->structure name)))
				       access-names))
				#f #f filename '()
				#f	;uid
				#f)))	;name
	  (let loop ()
	    (let ((uid (restore)))
	      (if (not (eq? uid '-))
		  (let ((qname+type (restore)))
		    (table-set! table uid (reference->location qname+type p))
		    (loop)))))
	  (let-fluid $restore-index (lambda (id define?)
				      (table-ref table id))
	    (lambda ()
	      (let ((templates (restore)))
		(for-each (lambda (template)
			    (if (not (template? template))
				(error "lossage" template))
			    (invoke-closure (make-closure template
							  (package-uid p))))
			  templates))))
	  p)))))


(define (reference->location qname+type p)
  (let* ((compile-time-type (or (cdr qname+type) usual-variable-type))
	 (name (qualified->name (car qname+type) p))
	 (binding (package-lookup p name)))
    (if (pair? binding)
	(let ((type (binding-type binding)))
	  (if (not (equal? type compile-time-type))
	      (warn "type inconsistency" 
		    `(compile time: ,compile-time-type)
		    `(load time: ,type)))
	  (cdr binding))
	(package-define! p name compile-time-type))))
