; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.



; Structure reification.

; For scheme48 development system:
; (reify-structures struct-alist
;		    (lambda (loc) loc))
;
; For mobot scheme system:
; (let ((l-table (make-table))) ;for later traversal
;   (reify-structures struct-alist
;		      (lambda (loc)
;		        (table-set! l-table loc #t)
;		        location-id)))


(define package-info-type
  (make-record-type 'reify-info
		    '(bindings  ;List of (name static-info location)
		      inits     ;List of initialization forms
		      form      ;'(vector-ref the-packages <index>)
		      table)))  ;Caches (assq? name bindings)

(define make-package-info
  (let ((make (record-constructor package-info-type
				  '(bindings inits form table))))
    (lambda ()
      (make '() '()
	    ;; The "???" gets clobberred later on.
	    (list 'vector-ref 'the-packages '???)
	    (make-table name-hash)))))

(define package-info-bindings (record-accessor package-info-type 'bindings))
(define package-info-inits    (record-accessor package-info-type 'inits))
(define package-info-form     (record-accessor package-info-type 'form))
(define package-info-table    (record-accessor package-info-type 'table))

(define set-package-info-bindings!
  (record-modifier package-info-type 'bindings))
(define set-package-info-inits!    (record-modifier package-info-type 'inits))


; REIFY-STRUCTURES returns a form that evaluates to an alist of
; (name . structure).

(define (reify-structures struct-alist deal-with-location)

  ;; List of (package . info), topologically ordered.
  (define packages '())

  ;; Entries are package-info structures.
  (define p-table (make-table package-uid))

  ;; Next index to be allocated for vector of packages.
  (define i 0)

  ;; Takes a list of signature-items and a package, and returns a form
  ;; that evaluates to p.
  (define (process-items items p)
    (let* ((old? (table-ref p-table p))
	   (info (or old?
		     (let ((info (make-package-info)))
		       (table-set! p-table p info)
		       info))))
      (letrec ()
	(for-each (lambda (item)
		    (if (not (process-one-binding (signature-item-name item)
						  p info))
			(warn "undefined export" item (package-name p))))
		  items)
	(let ((form (package-info-form info)))
	  (if (not old?)
	      (begin (set! packages (cons p packages))
		     (set-car! (cddr form) i)
		     (set! i (+ i 1))))
	  form))))

  (define (process-one-binding name p info)	; => #t iff bound
    (let ((table (package-info-table info)))
      (if (table-ref table name)
	  #t
	  (let ((static (package-lookup p name)))
	    (if static
		(begin
		  (table-set! table name #t)
		  (let ((form (cond ((operator? static)
				     `(get-operator ',(operator-name static)
						    ',(operator-type static)))
				    ((transform? static)
				     ;; Side effects
				     (process-transform static name p info))
				    (else #f))))
		    (if form
			(set-package-info-inits!
			 info
			 (cons `(package-define! the-package ',name ,form)
			       (package-info-inits info)))))
		  (set-package-info-bindings!
		   info
		   (cons (list name
			       (cond ((syntax? static) 'syntax)
				     ((operator? static) 'operator)
				     ((transform? static) 'transform)
				     (else static))
			       (let ((loc (package-find-location p name #f)))
				 (if loc
				     (deal-with-location loc)
				     #f)))
			 (package-info-bindings info)))
		    #t)
		#f)))))

  (define (process-transform mac name p info)
    (cond ((eq? (transform-env mac) p)
	   (let* ((source (transform-source mac))
		  (aux-names (or (transform-aux-names mac) ; () must be true
				 (find-aux-names source)))
		  (aux-extrinsics
		   (map (lambda (name)
			  (cond ((generated? name)
				 (warn "reified extrinsics NYI" name)
				 ;(do ((n name (foo n))
				 ;     (q p (fuu q)))
				 ;    ((bar? n))
				 ;  (process-one-binding (zz n) (gz q) (ii q)))
				 (name->extrinsic name p))
				(else
				 (process-one-binding name p info)
				 name)))
			aux-names)))
	     `(make-transform ,source	;transformer
			      the-package ;env
			      ',(transform-type mac) ;type
			      ;; (map (lambda (x)
			      ;;        (extrinsic->name x the-package) ...))
			      ',aux-extrinsics
			      #f  ;',source  -- omitted to save space
			      ',name)))
	  ((structure? (transform-aux-names mac))    ;mac names a structure
	   (let ((items (signature-items
			 (structure-signature
			  (transform-aux-names mac)))))
	     `(make-transform (transform-for-structure-ref
			         (make-simple-signature ',items))
			      ,(process-items items (transform-env mac))
			      ',(transform-type mac) ;type
			      'uluz
			      #f
			      ',name)))
	  (else
	   `(package-lookup ,(process-items (list name)
					    (transform-env mac))
			    ',name))))

  (define (make-structure-form name+struct)
    (let* ((name (car name+struct))
	   (struct (cdr name+struct))
	   (items (signature-items (structure-signature struct)))
	    ;; Side effects happen here:
	   (p-form (process-items items (structure-package struct))))

      (write-char #\.) (force-output (current-output-port))

      `(cons ',name
	     (make-structure
	      ',(structure-name struct)
	      (make-simple-signature ',items)
	      ,p-form))))
  
  (define (make-package-init-form p)
    (let* ((info (table-ref p-table p))
	   (bindings (package-info-bindings info))
	   (inits (package-info-inits info)))
      `(let ((the-package ,(package-info-form info)))
	 (initialize-reified-package!
	  the-package
	  ;; Each binding is a list (name static loc)
	  ',(list->vector (map car bindings))    ;names
	  ',(list->vector (map cadr bindings))   ;static info (type)
	  ',(list->vector (map caddr bindings))) ;locations
	 ;; Define macros & operators
	 ,@inits)))

  (display "Reifying") (force-output (current-output-port))

  (let* ((struct-forms (map make-structure-form struct-alist))
    
	 (shebang
	  `(let ((the-packages
		  (vector ,@(map (lambda (p) `(make-simple-package '() #f #f))
				 packages))))
	     ,@(map make-package-init-form
		    (reverse packages))
	     (list ,@struct-forms))))

    (newline)
    (if *reify-debug* (*reify-debug* shebang))
    shebang))

(define *reify-debug*  ;#f
  (let ((fn "reify-debug.tmp"))
    (lambda (x) (call-with-output-file fn
		  (lambda (port)
		    (display "Writing linker debug file ")
		    (display fn) (force-output (current-output-port))
		    (write x port)
		    (newline))))))


(define (find-aux-names form)
  (cond ((not (pair? form)) '()) ;lose
	((eq? (car form) 'syntax-rules)
	 (find-free-names-in-syntax-rules (cadr form) (cddr form)))
	((and (eq? (car form) 'lambda)
	      (let ((bvl (cadr form)))
		(and (pair? bvl) (pair? (cdr bvl)))))
	 ;; Unmentionable kludge.  I don't think it's necessary, but...
	 (let ((r (cadr (cadr form)))
	       (losers '()))
	   (let recur ((form (cddr form)))
	     (if (pair? form)
		 (if (eq? (car form) r)
		     (if (and (pair? (cadr form))
			      (eq? (car (cadr form)) 'quote))
			 (let ((name (cadr (cadr form))))
			   (if (not (memq name losers))
			       (set! losers
				     (cons name losers))))
			 (warn "rename procedure oddly used" form))
		     (begin (recur (car form))
			    (recur (cdr form))))))
	   losers))
	(else
	 (warn "can't understand auxiliary transform bindings" form)
	 '())))
