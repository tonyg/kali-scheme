; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This breaks abstractions left and right.

; Inspector state:
;    menu     ; cached result of (prepare-menu thing).  This is a list of
;               lists (<name-or-#f> <value>).
;    position ; position within menu; modified by M (more) command
;    stack    ; list of other things
;
; The current thing being inspected is the focus object.

(define *menu-limit* 15)	; maximum menu entries
(define *write-depth* 3)	; limit for recursive writes
(define *write-length* 5)       ; ditto

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
	   (+ *menu-limit* position))
	(begin
	  (set-menu-position! (- (+ position
				    *menu-limit*)
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
					0)))
		
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

; Continuation menus have the both the saved operand stack and the
; save environment, for which names may be available.

(define (prepare-continuation-menu thing)
  (let ((next (continuation-parent thing)))
    `(,@(let recur ((c thing))
	  (if (eq? c next)
	      '()
	      (let ((z (continuation-arg-count c)))
		(do ((i (- z 1) (- i 1))
		     (l (recur (continuation-cont c))
			(cons (list #f (continuation-arg c i))
			      l)))
		    ((< i 0) l)))))
      ,@(prepare-environment-menu
	  (continuation-env thing)
	  (debug-data-env-shape (continuation-debug-data thing)
				(continuation-pc thing))))))

(define (continuation-debug-data thing)
  (template-debug-data (continuation-template thing)))

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
    (do ((i 1 (+ i 1))
	 (l '() (cons (if (vector-unassigned? env i)
			  'unassigned
			  (vector-ref env i))
		      l)))
	((>= i z) l))))

;----------------
; Printing menus.
;
; If the current thing is a continuation we print its source code first.
; Then we step down the menu until we run out or we reach the menu limit.

(define (display-menu menu start port)
  (newline port)
  (maybe-display-source (focus-object) #f)
  (let ((items (menu-refs menu start (+ *menu-limit* 1)))
	(limit (+ start *menu-limit*)))
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

(define (maybe-display-source thing exception?)
  (cond ((not (continuation? thing))
	 (values))
	((exception-continuation? thing)
	 (let ((next (continuation-cont thing)))
	   (if (not (eq? next (continuation-parent thing)))
	       (maybe-display-source next #t))))
	(else
	 (let ((dd (continuation-debug-data thing)))
	   (if dd
	       (let ((source (assoc (continuation-pc thing)
				    (debug-data-source dd))))
		 (if source
		     (display-source-info (cdr source) exception?))))))))
  
; Show the source code for a continuation, if we have it.

(define (display-source-info info exception?)
  (if (pair? info)
      (let ((o-port (command-output))
	    (i (car info))
	    (exp (cdr info)))
	(if (and (integer? i) (list? exp))
	    (begin
	      (display (if exception?
			   "Next call is "
			   "Waiting for ")
		       o-port)
	      (limited-write (list-ref exp i) o-port
			     *write-depth* *write-length*)
	      (newline o-port)
	      (display "  in " o-port)
	      (limited-write (append (sublist exp 0 i)
				     (list '^^^)
				     (list-tail exp (+ i 1)))
			     o-port
			     *write-depth* *write-length*)
	      (newline o-port))))))

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
				*write-length*)
	      $write-depth (if (and (pair? limits)
				    (pair? (cdr limits)))
			       (cadr limits)
			       *write-depth*)
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

