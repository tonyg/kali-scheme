; Copyright (c) 1993-1999 by Richard Kelsey.  See file COPYING.


; Syntax used by the compiler

; Subrecords
;
; SUPER is the name of the existing record
; SUB is the name of the subrecord
; SLOT is the name of the slot to use in the existing sturcture
; STUFF is the usual stuff from DEFINE-RECORD-TYPE

(define-syntax define-subrecord
  (lambda (form rename compare)
    (let ((super (cadr form))
	  (sub (caddr form))
	  (slot (cadddr form))
	  (stuff (cddddr form)))
      (let ((access-names (map (lambda (spec)
				 (if (pair? spec) (car spec) spec))
			       (append (car stuff) (cadr stuff))))
	    (set-names (append (filter-map (lambda (spec)
					     (if (pair? spec) (car spec) #f))
					   (car stuff))
			       (map (lambda (spec)
				      (if (pair? spec) (car spec) spec))
				    (cadr stuff)))))
	`(begin (define-record-type ,sub . ,stuff)
		,@(map (lambda (name)
			 `(define ,(concatenate-symbol super '- name)
			    (lambda (v)
			      (,(concatenate-symbol sub '- name)
			       (,slot v)))))
		       access-names)
		,@(map (lambda (name)
			 `(define ,(concatenate-symbol 'set- super '- name '!)
			    (lambda (v n)
			      (,(concatenate-symbol 'set- sub '- name '!)
			       (,slot v)
			       n))))
		       set-names))))))

;(define-syntax define-simple-record-type
;  (lambda (form rename compare)
;    (let ((name (cadr form))
;          (slots (cddr form)))
;      `(begin (define-record-type ,name ,slots ())
;              (define ,(concatenate-symbol 'make- name)
;                ,(concatenate-symbol name '- 'maker))))))

; Nothing actually local about it...

(define-syntax define-local-syntax
  (lambda (form rename compare)
    (let ((pattern (cadr form))
	  (body (cddr form)))
      `(,(rename 'define-syntax) ,(car pattern)
	 (,(rename 'lambda) (form rename compare)
	   (,(rename 'destructure) ((,(cdr pattern)
				     (,(rename 'cdr) form)))
	     . ,body))))))
