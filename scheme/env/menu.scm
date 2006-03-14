; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This breaks abstractions left and right.

; Inspector state:
;    menu     ; cached result of (prepare-menu thing).  This is a list of
;               lists (<name-or-#f> <value>).
;    position ; position within menu; modified by M (more) command
;    stack    ; list of other things
;
; The current thing being inspected is the focus object.

(define (current-menu)
  (or (maybe-menu)
      (let ((menu (prepare-menu (focus-object))))
	(set-menu! menu)
	(set-menu-position! 0)
	menu)))

(define (present-menu)
  (let ((menu (current-menu)))	; may set menu position
    (display-menu menu
		  (menu-position)
		  (command-output))))

(define (present-more-menu)
  (let* ((menu (current-menu))
	 (position (menu-position)))
    (if (> (menu-length menu)
	   (+ (inspector-menu-limit) position))
	(begin
	  (set-menu-position! (- (+ position
				    (inspector-menu-limit))
				 1))
	  (present-menu))
	(write-line "There is no more." (command-output)))))

;----------------
; These two are used by the inspector.

(define (current-menu-length)
  (menu-length (current-menu)))

(define (current-menu-ref n)
  (cadar (menu-refs (current-menu) n 1)))

; The menu ADT has two functions, length and refs.  A menu is a list
; (<length> <refs-function>)

(define (menu-length menu)
  (car menu))

; Return a list of the next COUNT items starting from N, where each items is
; a list (<name-or-#f> <thing>).  The returned list may be shorter than N if
; there aren't N possible items, or longer, for no reason at all.

(define (menu-refs menu n count)
  ((cadr menu) n count))

(define (list->menu items)
  (list (length items)
	(lambda (i count)
	  (list-tail items i))))

(define (long-list->menu contents length)
  (list length
	(lambda (start count)
	  (do ((i 0 (+ i 1))
	       (contents (list-tail contents start) (cdr contents))
	       (r '() (cons (list #f (car contents)) r)))
	      ((or (= i count)
		   (null? contents))
	       (reverse r))))))

(define (indexed->menu thing length ref)
  (list length
	(lambda (start count)
	  (do ((i 0 (+ i 1))
	       (r '() (cons (list #f (ref thing (+ start i))) r)))
	      ((or (= i count)
		   (= (+ i start) length))
	       (reverse r))))))

; Get a menu for THING.  We know about a fixed set of types.

(define (prepare-menu thing)
  (cond ((vector? thing)
	 (indexed->menu thing (vector-length thing) vector-ref))
	((template? thing)
	 (indexed->menu thing (template-length thing) template-ref))
	((pair? thing)
	 (let ((length (careful-length thing)))
	   (if (eq? length 'improper)
	       (list->menu `((car ,(car thing)) (cdr ,(cdr thing))))
	       (long-list->menu thing
				(if (eq? length 'circular)
				    9999999
				    length)))))
	(else
	 (list->menu
	  (cond ((closure? thing)
		 (prepare-environment-menu
		  (closure-env thing)
		  (debug-data-env-shape (template-debug-data
					 (closure-template thing))
					#f)))
		
		((continuation? thing)
		 (prepare-continuation-menu thing))
		
		((record? thing)
		 (prepare-record-menu thing))
		
		((location? thing)
		 `((id ,(location-id thing))
		   (contents ,(contents thing))))
		
		((cell? thing)
		 `((ref ,(cell-ref thing))))
		
		((weak-pointer? thing)
		 `((ref ,(weak-pointer-ref thing))))
		
		(else '()))))))
	
(define (careful-length list)
  (let loop ((fast list) (len 0) (slow list) (move-slow? #f))
    (cond ((eq? '() fast)
	   len)
	  ((not (pair? fast))
	   'improper)
	  ((not move-slow?)
	   (loop (cdr fast) (+ len 1) slow #t))
	  ((eq? fast slow)
	   'circular)
	  (else
	   (loop (cdr fast) (+ len 1) (cdr slow) #f)))))

; Some values in the operand stack are vectors that represent either the
; saved environment or a newly created one for recursive procedures.
; The debug data has names for some values in the stack and for those
; in the environments.

(define (prepare-continuation-menu thing)
  (let ((shape (debug-data-env-shape (continuation-debug-data thing)
				     (continuation-pc thing)))
	(args (do ((i 0 (+ i 1))
		   (v '() (cons (continuation-arg thing i) v)))
		  ((= i (continuation-arg-count thing))
		   v))))
    (extend-cont-menu 0 args shape '())))

(define (extend-cont-menu i args shape menu)
  (if (null? args)
      menu
      (let ((names (assq i shape)))
	(if (and names
		 (not (null? (cdr names))))
	    (extend-cont-menu-with-names (cdr names) i args shape menu)
	    (extend-cont-menu (+ i 1)
			      (cdr args)
			      shape
			      (cons (list #f (car args))
				    menu))))))

(define (extend-cont-menu-with-names names i args shape menu)
  (cond ((null? names)
	 (extend-cont-menu i args shape menu))
	((pair? (car names))
	 (let ((values (car args)))
	   (do ((ns (car names) (cdr ns))
		(j 0 (+ j 1))
		(menu menu (cons (list (car ns) (vector-ref values j))
				 menu)))
	       ((null? ns)
		(extend-cont-menu-with-names (cdr names)
					     (+ i 1)
					     (cdr args)
					     shape
					     menu)))))
	(else
	 (extend-cont-menu-with-names (cdr names)
				      (+ i 1)
				      (cdr args)
				      shape
				      (cons (list (car names) (car args))
					    menu)))))

(define (continuation-debug-data thing)
  (let ((template (continuation-template thing)))
    (if template
	(template-debug-data template)
	#f)))

; Records that have record types get printed with the names of the fields.

(define (prepare-record-menu thing)
  (let ((rt (record-type thing))
        (z (record-length thing)))
    (if (record-type? rt)
        (do ((i (- z 1) (- i 1))
             (f (reverse (record-type-field-names rt)) (cdr f))
             (l '() (cons (list (car f) (record-ref thing i)) l)))
            ((< i 1) l))
        (do ((i (- z 1) (- i 1))
             (l '() (cons (list #f (record-ref thing i)) l)))
            ((< i 0) l)))))

; We may have the names (`shape') for environments, in which case they
; are used in the menus.

(define (prepare-environment-menu env shape)
  (if (vector? env)
      (let ((values (rib-values env)))
        (if (pair? shape)
            (append (map list (car shape) values)
                    (prepare-environment-menu (vector-ref env 0)
                                              (cdr shape)))
            (append (map (lambda (x)
			   (list #f x))
			 values)
                    (prepare-environment-menu (vector-ref env 0)
					      shape))))
      '()))

(define (rib-values env)
  (let ((z (vector-length env)))
    (do ((i 0 (+ i 1))
	 (l '() (cons (if (vector-unassigned? env i)
			  'unassigned
			  (vector-ref env i))
		      l)))
	((>= i z)
	 (reverse l)))))

;----------------
; Printing menus.
;
; If the current thing is a continuation we print its source code first.
; Then we step down the menu until we run out or we reach the menu limit.

(define (display-menu menu start port)
  (newline port)
  (maybe-display-source (focus-object) #f)
  (let ((items (menu-refs menu start (+ (inspector-menu-limit) 1)))
	(limit (+ start (inspector-menu-limit))))
    (let loop ((i start) (items items))
      (with-limited-output
       (lambda ()
	 (cond ((null? items))
	       ((and (>= i limit)
		     (not (null? items)))
		(display " [m] more..." port) (newline port))
	       (else
		(let ((item (car items)))
		  (display " [" port)
		  (write i port)
		  (if (car item)
		      (begin (display ": " port)
			     (write-carefully (car item) port)))
		  (display "] " port)
		  (write-carefully
		   (value->expression (cadr item))
		   port)
		  (newline port)
		  (loop (+ i 1) (cdr items))))))))))

; Exception continuations don't have source, so we get the source from
; the next continuation if it is from the same procedure invocation.

(define (maybe-display-source thing vm-exception?)
  (cond ((not (continuation? thing))
	 (values))
	((vm-exception-continuation? thing)
	 (let ((next (continuation-cont thing)))
	   (if (not (eq? next (continuation-cont thing)))
	       (maybe-display-source next #t))))
	(else
	 (let ((dd (continuation-debug-data thing)))
	   (if dd
	       (let ((source (assoc (continuation-pc thing)
				    (debug-data-source dd))))
		 (if source
		     (display-source-info (cdr source) vm-exception?))))))))
  
; Show the source code for a continuation, if we have it.

(define (display-source-info info vm-exception?)
  (let ((o-port (command-output)))
    (if (pair? info)
	(let ((exp (car info)))
	  (display (if vm-exception?
		       "Next call is "
		       "Waiting for ")
		   o-port)
	  (limited-write exp o-port
			 (inspector-writing-depth)
			 (inspector-writing-length))
	  (newline o-port)
	  (if (and (pair? (cdr info))
		   (integer? (cadr info)))
	      (let ((i (cadr info))
		    (parent (cddr info)))
		(display "  in " o-port)
		(limited-write (append (sublist parent 0 i)
				       (list '^^^)
				       (list-tail parent (+ i 1)))
			       o-port
			       (inspector-writing-depth)
			       (inspector-writing-length))
		(newline o-port)))))))

;----------------
; Selection commands

(define (selection-command? x)
  (or (integer? x)
      (memq x '(u d template))))

;----------------
; I/O Utilities

(define $write-depth (make-fluid -1))
(define $write-length (make-fluid -1))

(define (with-limited-output thunk . limits)
  (let-fluids $write-length (if (pair? limits)
				(car limits)
				(inspector-writing-length))
	      $write-depth (if (and (pair? limits)
				    (pair? (cdr limits)))
			       (cadr limits)
			       (inspector-writing-depth))
    thunk))

(define (write-carefully x port)
  (if (error? (ignore-errors (lambda ()
                               (limited-write x port
                                              (fluid $write-depth)
                                              (fluid $write-length))
                               #f)))
      (display "<Error while printing.>" port)))

(define (write-line string port)
  (display string port)
  (newline port))

