; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Package / structure / interface mutation operations.

; None of these is essential for the static semantics of packages.
; They only come into play in order to implement dynamic package
; mutation, which is solely a debugging operation.

; This is work in progress.  There are surely numerous bugs.

; (define (package-system-sentinel) #f)

(define (package-open! p struct-thunk)
  (if (not (memq (struct-thunk) (package-opens p)))
      (let ((thunk (package-opens-thunk p)))
        (set-package-opens-thunk! p (lambda ()
                                      (cons (struct-thunk) (thunk))))
        (reinitialize-package! p)
        (verify-package p)
	)))

(define (reinitialize-package! p)
  (if *debug?*
      (begin (write `(reinitialize ,p)) (newline)))
  ;; (set-package-opens! p #f)
  (if (package-opens-really p)
      (initialize-package! p)))

(define (reinitialize-structure! s)
  (if *debug?*
      (begin (write `(reinitialize ,s)) (newline)))
  ;; (set-structure-interface! s #f)
  (if (structure-interface-really s)
      (initialize-structure! s)))


; GET-NEW-LOCATION-CAREFULLY is called to obtain a location when 
; a variable becomes newly defined.
; If the variable has already been referenced in any code that ought
; to see the new binding, we need to alter all such references to
; access the new location instead of whatever location they saw
; before.

(define (get-new-location-carefully p name)
  (let* ((prev (package-lookup p name))
	 (new (if (binding? prev)
		  (let ((new (make-new-location p name))
			(cached (table-ref (package-cached p) name)))
		    (copy-shadowed-contents! (binding-place prev) new)
		    (cond (cached
			   (if (eq? (binding-place prev) cached)
			       (cope-with-mutation p name new cached)
			       (error "binding cache inconsistency"
				      p name new cached))))
		    new)
		  (get-new-location-non-shadowing p name)))
	 (aloc (table-ref (package-undefined-but-assigneds p)
			  name)))
    (if aloc				; Assigned?
	(begin (if *debug?*
		   (note "assigned -> defined" name))
	       (table-set! (package-undefined-but-assigneds p) name #f)
	       (set-location-forward! aloc new name p)))
    new))

(define (get-new-location-non-shadowing p name)
  (let* ((uloc (table-ref (package-undefineds p) name))
	 (loc
	  (if uloc
	      (begin (if *debug?* (note "undefined -> defined" name uloc))
		     (table-set! (package-undefineds p) name #f)
		     uloc)
	      (make-new-location p name))))
    (if (and (not uloc)
	     (symbol? name))	; interfaces can't handle generated names
	(let recur ((q p))
	  (let loop ((opens (package-opens q)))
	    (if (not (null? opens))
		(call-with-values
		 (lambda ()
		   (interface-ref (structure-interface (car opens))
				  name))
		 (lambda (base-name type)
		   (if base-name
		       ;; Shadowing
		       (let* ((q (structure-package (car opens)))
			      (probe (table-ref (package-undefineds q)
						base-name)))
			 (if probe
			     (begin (if *debug?*
					(note "undefined -> shadowed"
					      name loc probe))
				    (cope-with-mutation p name loc probe))
			     (recur q)))
		       (loop (cdr opens)))))))))
    loc))

; COPE-WITH-MUTATION:
; A package system mutation has newly caused NAME to be bound in
; package P to NEW, but prior references assumed that its binding was
; PREV (perhaps inherited from another package).  If PREV is a
; location, then each occurrence of it stowed away in templates and
; packages must eventually be replaced by either location NEW, if P's
; binding is the one that's visible at that occurrence, or to a fresh
; location to replace PREV, if not.

(define (cope-with-mutation p name new prev)
  (if (eq? new prev)
      (error "lossage in cope-with-mutation" p name new prev))
  (let ((replacement (make-new-location p name)))
    (copy-location-info! prev replacement)
    (table-set! (package-cached p) name new)
    (if *debug?*
	(begin (write `(mutation ,prev ,new ,replacement)) (newline)))
    (shadow-location! prev
		      (map package-uid
			   (packages-seeing-location p name prev))
		      new
		      replacement)))

;(define (set-binding-place! b foo) (vector-set! b 1 foo))


; Hmm.  It ought to be possible to turn this into an RPC.

(define (copy-shadowed-contents! import loc)
  (if (location-defined? import)
      (set-contents! loc (contents import))))


; Return a list of all packages that might have cached a particular
; inherited binding.

(define (packages-seeing-location package name loc)
  (if (not (symbol? name)) ; interfaces cannot handle generated names
      (list package)
      (let ((losers '())) ; was (list package) but that disables the
	                  ; entire procedure
	(let recur ((package-or-structure package))
	  (let ((package (if (package? package-or-structure)
			     package
			     (structure-package package-or-structure))))
	    (if (and (not (memq package losers))
		     (not (table-ref (package-definitions package) name)))
		(begin (set! losers (cons package losers))
		       (walk-population
			(lambda (struct)
			  (if (interface-member? (structure-interface struct)
						 name)
			      (walk-population recur
					       (structure-clients struct))))
			(package-clients package))))))
        losers)))

(define (set-location-forward! loser new name p)
  (if *debug?*
      (begin (write `(forward ,loser ,new)) (newline)))
  (for-each (lambda (q)
	      (package-note-caching! q name new))
	    (packages-seeing-location p name loser))
  (shadow-location! loser '() #f new))


(fluid-cell-set! $get-location get-new-location-carefully)  ;foo


(define (copy-location-info! from to)
  (let ((probe (location-info from)))
    (if probe
        (table-set! location-info-table (location-id to) probe))))


;; Deal with editing operations

(define (really-verify-later! thunk)	;cf. define-structure macro
  (let ((premature (ignore-errors thunk))) ; old value if we're redefining
    (let ((cell (fluid $losers))
	  (loser (if (or (structure? premature)
			 (interface? premature))
		     (cons premature thunk)
		     (cons #f thunk))))
      (cell-set! cell
		 (cons loser
		       (cell-ref cell))))))
 
; each element is a a pair (thing, thunk)
(define $losers (make-fluid (make-cell '())))
(define $package-losers (make-fluid (make-cell '())))

(define (package-system-sentinel)
  (drain $losers verify-loser)
  (drain $package-losers verify-package))

(define (verify-loser loser)
  (really-verify-loser (car loser)
		       (ignore-errors (cdr loser))))

(define (really-verify-loser premature thing)
  (if *debug?*
      (begin (write `(verify-loser ,premature ,thing)) (newline)))
  (cond
   ((structure? thing) (verify-structure premature thing))
   ((interface? thing) (verify-interface premature thing))))
 
(define (verify-structure premature struct)
  (reinitialize-structure! struct)
  (if *debug?*
      (begin
	(write `(verify-structure ,struct ,(population->list (structure-clients struct))))
	(newline)))
  (walk-population note-verify-package (structure-clients struct)))

(define (verify-interface premature int)
  ;; add clients of old interface to new one
  (if (interface? premature)
      (walk-population
       (lambda (client)
	 (note-reference-to-interface! int client))
       (interface-clients premature)))
  (if *debug?*
      (begin
	(write `(verify-interface ,(population->list (interface-clients int))))
	(newline)))
  (walk-population (lambda (thing) (verify-structure #f thing))
		   (interface-clients int)))

(define (note-verify-package pack)
  (reinitialize-package! pack)
  (let* ((cell (fluid $package-losers))
	 (losers (cell-ref cell)))
    (if (not (memq pack losers))
	(cell-set! cell
		   (cons pack losers)))))

(define (drain flu check)
  (let ((cell (fluid flu)))
    (let loop ()
      (let ((losers (cell-ref cell)))
	(if (not (null? losers))
	    (let ((loser (car losers)))
	      (cell-set! cell (cdr losers))
	      (check loser)
	      (loop)))))))

(define *debug?* #f)

; --------------------
; If any looked-up location is now invalid, do something about it.  We
; might have a previously unbound variable that is now exported by
; some structure; or, we might have a variable that was bound in some
; structure before, but now either that structure doesn't export it at
; all, or we're now getting that binding from some new structure.

(define (verify-package p)
  (verify-package-cached p)
  (verify-package-undefineds p))

; unbound -> bound

(define (verify-package-undefineds p)
  (if *debug?*
      (begin (write `(verify undefineds ,p)) (newline)))
  (let ((newly-defined '())
        (defs (package-definitions p))
        (undefs (package-undefineds p)))
    (table-walk (lambda (name prev)
                  (if (table-ref defs name)
		      (error "lossage in verify-package-undefineds" p name))
		  (let ((binding (package-lookup p name)))
		    (if (binding? binding)
			(let ((place (binding-place binding)))
			  (if (eq? place prev)
			      (error "lossage - verify-package-undefineds"
				     p name binding))
			  (set-location-forward! prev place name p)
			  (set! newly-defined
				(cons name newly-defined)))
			(let ((loc (location-for-reference p name)))
			  (if (not (eq? loc prev))
			      ;; Newly imported
			      (begin (set-location-forward! prev loc name p)
				     (set! newly-defined
					   (cons name newly-defined))))))))
		undefs)
    (if (not (null? newly-defined))
        (begin (display "Newly accessible in ")
               (write (package-name p))
               (display ": ")
               (write newly-defined)
               (newline)))
    (for-each (lambda (winner)
		(table-set! undefs winner #f))
              newly-defined)))

(define (verify-package-cached p)
  ;; (write `(verify ,p cached)) (newline)
  (let ((newly-moved '())
	(newly-undefined '())
        (cached (package-cached p)))
    (table-walk (lambda (name prev)
                  (let ((binding (package-lookup p name)))
                    (if (binding? binding)
			(if (not (eq? (binding-place binding) prev))
			    (set! newly-moved
				  (cons name newly-moved)))
			;; Otherwise either (a) it was previously
			;; defined but has become newly undefined, or
			;; (b) it was undefined before and is still, but
			;; is inherited (or not) from a different place.
			(let ((u (location-for-reference p name)))
			  (if (not (eq? u prev))
			      (set! newly-undefined
				    (cons name newly-undefined)))))))
                cached)
    (if (not (null? newly-moved))
        (begin (display "Definitions visible in ")
               (write (package-name p))
               (display " have moved: ")
               (write newly-moved)
               (newline)))
    (for-each (lambda (name)
		(let ((new (binding-place (package-lookup p name))))
		  (cope-with-mutation p name new
				      (table-ref cached name))
		  (table-set! cached name new)))
              newly-moved)
    (if (and *debug?* (not (null? newly-undefined)))
	(begin (display "Newly undefined in ")
               (write (package-name p))
               (display ": ")
               (write newly-undefined)
               (newline)))
    (for-each (lambda (name)
		(let ((new (location-for-reference p name)))
		  (cope-with-mutation p name new
				      (table-ref cached name))
		  (table-set! cached name new)))
	      newly-undefined)))

(set-verify-later! really-verify-later!)


;(define (maybe-note-redefinition p name seen new)
;  (if (and seen (not (compatible? new seen)))
;      (let ((old-description (binding-description-string seen))
;            (new-description (binding-description-string new))
;            (doing (if (package-definition p name)
;                       "redefining"
;                       "shadowing")))
;        (warn (if (equal? old-description new-description)
;                  doing
;                  (string-append doing
;                                 " as "
;                                 new-description
;                                 " (prior references assumed "
;                                 old-description
;                                 ")"))
;              name))))
;
;; Foo.  This should really set SEEN for all the packages up the
;; inheritance chain.
;
;(define (assume-denotation p name info)
;  (if (package-unstable? p)    ;***** save space?  and time?
;      (table-set! (package-seen p) name info))
;  info)
;
;; Little utility for warning messages.
;
;(define (binding-description-string d)
;  (cond ((transform? d)
;         (if (syntax? d) "macro" "integrated procedure"))
;        ((operator? d)
;         (if (syntax? d) "special operator" "primitive procedure"))
;        (else "variable")))



(define (package-undefine! p name)
  (let ((probe (table-ref (package-definitions p) name)))
    (if probe
	(begin (table-set! (package-definitions p) name #f)
	       (set-location-defined?! (binding-place probe) #f)
	       (set-location-forward!
		 (binding-place probe)
		 (let ((new (package-lookup p name)))
		   (if (binding? new)
		       (binding-place new)
		       (location-for-reference p name)))
		 name p))
	(warn "can't undefine - binding is inherited"
	      p name))))
