; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Two macros:
;
; (import-definition <id>)
;   ->
; (define <id> (lookup-imported-binding "<id with - becoming _>"))
;
; (import-definition <id> <string id>)
;   ->
; (define <id> (lookup-imported-binding <string-id>))
;
; (import-lambda-definition <id> (<formal> ...))
;   ->
; (begin
;   (define temp (lookup-imported-binding "<id with - becoming _>"))
;   (define <id>
;     (lambda (<formal> ...)
;       (call-imported-binding temp <formal> ...))))
;
; (import-lambda-definition <id> (<formal> ...) <string id>)
;   ->
; ...same again using <string id> as the imported name...

(define-syntax import-definition
  (lambda (exp rename compare)
    (let ((id (cadr exp))
	  (%define (rename 'define))
	  (%lookup-imported-binding (rename 'lookup-imported-binding)))
      (let ((external-id (if (null? (cddr exp))
			     (list->string (map (lambda (ch)
						  (if (char=? ch #\-)
						      #\_
						      ch))
						(string->list
						 (symbol->string id))))
			     (caddr exp))))
	`(,%define ,id
	    (,%lookup-imported-binding ,external-id))))))

; (import-lambda-definition id (formal ...) [external name])

(define-syntax import-lambda-definition
  (lambda (exp rename compare)
    (let ((id (cadr exp))
	  (formals (caddr exp))
	  (%define (rename 'define))
	  (%begin (rename 'begin))
	  (%lambda (rename 'lambda))
	  (%call-imported-binding   (rename 'call-imported-binding))
	  (%lookup-imported-binding (rename 'lookup-imported-binding))
	  (%binding 		    (rename 'binding)))
      (let ((external-id (if (null? (cdddr exp))
			     (list->string (map (lambda (ch)
						  (if (char=? ch #\-)
						      #\_
						      ch))
						(string->list
						 (symbol->string id))))
			     (cadddr exp))))
	`(,%begin
	   (,%define ,%binding
	    (,%lookup-imported-binding ,external-id))
	   (,%define ,id
	    (,%lambda ,formals
	      (,%call-imported-binding ,%binding . ,formals))))))))

