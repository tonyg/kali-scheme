; Copyright (c) 1993-2000 by Richard Kelsey.  See file COPYING.

; This code determines which procedures are called from one other form, and
; thus can be compiled as part of that form and called with a `goto' instead
; of a normal procedure call.  This saves much of the overhead of a normal
; procedure call.
;
; The procedures to be merged are annotated; no code is changed.

(define-subrecord form form-merge form-merge
  ((head)           ; self or the form into which this one will be merged
   )
  (
   (status #f)      ; one of #F, DO-NOT-MERGE, MERGED
   tail-clients     ; forms that call this one tail-recursively, this is an
                    ; a-list of forms and reference nodes
   (tail-providers '()) ; forms that are used by this one, this is a simple list
   (merged '())     ; forms merged with this one
   (return-count 0) ; how many returns have been generated so far
   temp             ; handy utility field
   ))

; Two procedures for letting the user know what is going on.

(define (show-merges form)
  (let ((merges (form-merged form)))
    (if (not (null? merges))
	(format #t " ~S: ~S~%" (form-name form) (map form-name merges)))))

(define (show-providers form)
  (cond ((eq? (form-type form) 'lambda)
	 (format #t "~S ~A~%"
		 (form-name form)
		 (if (form-exported? form) " (exported)" ""))
	 (cond ((or (not (null? (form-providers form)))
		    (not (null? (form-tail-providers form))))
		(format #t "  ~S~%  ~S~%"
			(map form-name (form-providers form))
			(map form-name (form-tail-providers form))))))))

; Note that OTHERS should be merged with FORM.

(define (do-merge form others)
  (let ((form (form-head form))
	(secondary (apply append (map form-merged others))))
    (set-form-merged! form (append others
				   secondary
				   (form-merged form)))
    (for-each (lambda (f)
		(set-form-head! f form))
	      secondary)
    (for-each (lambda (f)
		(set-form-head! f form)
		(set-form-status! f 'merged)
		(set-form-type! f 'merged)
		(set-form-merged! f '()))
	      others)))

; Returns the merged form, if any, to which NODE is a reference.

;(define (merged-procedure-reference node)
;  (let ((res (real-merged-procedure-reference node)))
;    (if (and (reference-node? node)
;             (eq? 'trace-value (variable-name (reference-variable node))))
;        (format "  [m-p-r ~S -> ~S]~%" node res))
;    res))
;
(define (merged-procedure-reference node)
  (cond ((and (reference-node? node)
	      (maybe-variable->form (reference-variable node)))
	 => (lambda (form)
	      (if (eq? 'merged (form-type form))
		  form
		  #f)))
	(else
	 #f)))

; Is FORM ever tail called?

(define (form-tail-called? form)
  (and (or (eq? 'lambda (form-type form))
	   (eq? 'merged (form-type form)))
       (memq? 'tail-called (variable-flags (form-var form)))))

; Annotate FORM if it is in fact called tail-recursively anywhere.

(define (note-tail-called-procedure form)
  (if (and (eq? 'lambda (form-type form))
	   (or (any (lambda (r)
		      (used-as-label? r))
		    (variable-refs (form-var form)))
	       (eq? 'tail-called (lambda-protocol (form-value form)))))
      (set-variable-flags! (form-var form)
			   (cons 'tail-called
				 (variable-flags (form-var form))))))

(define (used-as-label? node)
  (and (node? (node-parent node))
       (goto-call? (node-parent node))
       (= 1 (node-index node))))

;------------------------------------------------------------
; Entry point.
;
; First marks the tail-called procedures and adds the MERGE slots to the
; forms.  The C code generator expects FORM-MERGED to work, even if no
; actual merging was done.
;
; Three steps:
;  Find the call graph.
;  Merge the tail-called forms.
;  Merge the non-tail-called forms. 

(define *merge-forms?* #t)

(define (merge-forms forms)
  (for-each (lambda (f)
	      (note-tail-called-procedure f)
	      (set-form-merge! f (form-merge-maker f))
	      (set-form-providers! f '()))
	    forms)
  (if *merge-forms?*
      (let ((mergable-forms (filter determine-merger-graph forms)))
	(format #t "Call Graph:~%<procedure name>~%")
	(format #t "  <called non-tail-recursively>~%")
	(format #t "  <called tail-recursively>~%")
	(for-each show-providers forms)
	(format #t "Merging forms~%")
	(receive (tail other)
	    (partition-list (lambda (f) (null? (form-clients f)))
			    mergable-forms)
	  (merge-tail-forms tail)
	  (for-each merge-non-tail-forms forms)
	  (for-each show-merges forms)
	  (values)))))

; The only forms that can be merged are those that:
;  are lambdas,
;  all uses are calls,
;  are not exported, and
;  every loop containing a non-tail-recursive call must contain a call to
;  at least one non-merged procedure.
; 
; This code doesn't use the last criterion.  Instead it makes sure that each
; procedure is called exclusively tail-recursively or non-tail-recursively
; and doesn't allow non-tail-recursion in loops at all.

(define (determine-merger-graph form)
  (cond ((and (eq? 'lambda (form-type form))
	      (really-determine-merger-graph form)
	      (not (form-exported? form))
	      (or (null? (form-clients form))
		  (null? (form-tail-clients form))))
	 #t)
	(else
	 (set-form-status! form 'do-not-merge)
	 #f)))

; Loop down the references to FORM's variable adding FORM to the providers
; lists of the forms that reference the variable, and adding those forms
; to FORM's clients lists.  OKAY? is #T if all references are calls.

; The full usage graph is needed, even if there are uses of the form's value
; that are not calls.

(define (really-determine-merger-graph form)
  (let loop ((refs (variable-refs (form-var form)))
	     (clients '()) (tail-clients '()) (okay? #t))
    (cond ((null? refs)
	   (set-form-clients! form clients)
	   (set-form-tail-clients! form tail-clients)
	   okay?)
	  (else
	   (let* ((r (car refs))
		  (f (node-form (car refs))))
 	     (if (and (called-node? r)
 		      (or (calls-this-primop? (node-parent r) 'tail-call)
 			  (calls-this-primop? (node-parent r) 'unknown-tail-call)))
 		 (loop (cdr refs)
 		       clients
 		       (add-to-client-list tail-clients r form f
 					   form-tail-providers
 					   set-form-tail-providers!)
 		       okay?)
 		 (loop (cdr refs)
 		       (add-to-client-list clients r form f
 					   form-providers
 					   set-form-providers!)
 		       tail-clients
 		       (and okay? (called-node? r)))))))))

(define (add-to-client-list client-list ref form f getter setter)
  (cond ((assq f client-list)
	 => (lambda (p)
	      (set-cdr! p (cons ref (cdr p)))
	      client-list))
	(else
	 (setter f (cons form (getter f)))
	 (cons (list f ref) client-list))))

; These forms are non-exported procedures that are always tail-called.
; Strongly connected components of the call graph that have a single
; entry point, whether in the component or not, are merged.
; This depends on STRONGLY-CONNECTED-COMPONENTS returning the components
; in a reverse topologically sorted order (which it does).

(define (merge-tail-forms forms)
  (for-each merge-tail-loop
	    (reverse (strongly-connected-components
		      forms
		      (lambda (f)
			(filter (lambda (f) (memq? f forms))
				(map car (form-tail-clients f))))
		      form-temp
		      set-form-temp!))))

; ENTRIES are the forms in the loop that are called from outside.
; FORMS is used as a unique identifier here.

(define (merge-tail-loop forms)
  (for-each (lambda (f) (set-form-temp! f forms)) forms)
  (receive (entries other)
      (partition-list (lambda (f)
			(any? (lambda (p)
				(not (eq? forms
					  (form-temp (car p)))))
			      (form-tail-clients f)))
		      forms)
    (cond ((single-outside-client (if (null? entries)
				      other
				      entries)
				  forms)
	   => (lambda (f) (do-merge f forms)))
	  ((and (not (null? entries))
		(null? (cdr entries))
		(not (null? other)))
	   (do-merge (car entries) other)))
    (for-each (lambda (f) (set-form-temp! f #f)) forms)))

; This checks to see if all non-FLAGged clients of ENTRIES are in
; fact a single form, and then returns that form.
; Forms that have already been merged into another form are treated as that
; other form (by using FORM-HEAD).

(define (single-outside-client entries flag)
  (let loop ((entries entries) (form #f))
    (if (null? entries)
	form
	(let loop2 ((clients (form-tail-clients (car entries))) (form form))
	  (cond ((null? clients)
		 (loop (cdr entries) form))
		((eq? (form-temp (caar clients)) flag)
		 (loop2 (cdr clients) form))
		((not form)
		 (loop2 (cdr clients) (form-head (caar clients))))
		((eq? (form-head (caar clients)) form)
		 (loop2 (cdr clients) form))
		(else
		 #f))))))

; Merge the forms used by FORM into it if possible.

(define (merge-non-tail-forms form)
  (for-each (lambda (f)
	      (maybe-merge-non-tail-form f (form-head form)))
	    (form-providers form)))

; If FORM is not INTO, has not been merged before, and is only used by
; INTO, then merge FORM into INTO and recursively check the forms used
; by FORM.

(define (maybe-merge-non-tail-form form into)
  (cond ((and (not (eq? form into))
	      (not (form-status form))
	      (every? (lambda (p)
			(eq? (form-head (car p)) into))
		      (form-clients form)))
	 (do-merge into (list form))
	 (for-each tail-call->call
		   (variable-refs (form-var form)))
	 (for-each tail-call->call
		   (variable-refs (car (lambda-variables (form-node form)))))
	 (for-each (lambda (f)
		     (maybe-merge-non-tail-form f into))
		   (form-providers form)))))

; Replace tail calls with calls to make the code generator's job easier.
; The user didn't say that these calls had to be tail-recursive.

(define (tail-call->call ref)
  (let ((call (node-parent ref)))
    (if (or (calls-this-primop? call 'tail-call)
	    (calls-this-primop? call 'unknown-tail-call))
	(let ((type (arrow-type-result
		      (maybe-follow-uvar (node-type (call-arg call 1))))))
	  (move (call-arg call 0)
		(lambda (cont)
		  (let-nodes ((new-cont ((v type)) (return 0 cont (* v))))
		    new-cont)))
	  (set-call-exits! call 1)
	  (set-call-primop! call
			    (get-primop (if (calls-this-primop? call 'tail-call)
					    (enum primop call)
					    (enum primop unknown-call))))))))
  