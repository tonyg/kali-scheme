; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

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
    (if (> (length menu)
	   (+ *menu-limit* position))
	(begin
	  (set-menu-position! (- (+ position
				    *menu-limit*)
				 1))
	  (present-menu))
	(write-line "There is no more." (command-output)))))

;----------------
; A menu is a list of lists (<name-or-#f> <thing>).

(define (menu-length)
  (length (current-menu)))

(define (menu-ref n)
  (cadr (list-ref (current-menu) n)))

; Get a menu for THING.  We know about a fixed set of types.

(define (prepare-menu thing)
  (cond ((list? thing)
         (map (lambda (x)
		(list #f x))
              thing))

        ((pair? thing)
         `((car ,(car thing)) (cdr ,(cdr thing))))

        ((vector? thing)
         (prepare-menu (vector->list thing)))

        ((closure? thing)
         (prepare-environment-menu
              (closure-env thing)
              (debug-data-env-shape (template-debug-data
				      (closure-template thing))
				    0)))

	((template? thing)
	 (prepare-menu (template->list thing)))

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

        (else '())))

(define (template->list template)
  (do ((i (- (template-length template) 1) (- i 1))
       (r '() (cons (template-ref template i) r)))
      ((< i 0) r)))

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
  (let ((menu (list-tail menu start))
	(limit (+ start *menu-limit*)))
    (let loop ((i start) (menu menu))
      (with-limited-output
       (lambda ()
	 (cond ((null? menu))
	       ((and (>= i limit)
		     (not (null? (cdr menu))))
		(display " [m] more..." port) (newline port))
	       (else
		(let ((item (car menu)))
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
		  (loop (+ i 1) (cdr menu))))))))))

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

