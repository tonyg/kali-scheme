; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Shared binding between Scheme and external C code

;----------------
; Two tables of shared bindings: those we import from the outside and those
; that we provide to the outside.

(define *imported-bindings* false)
(define *exported-bindings* false)

; When resuming a statically-linked image these tables are FALSE.

(define (install-shared-bindings!+gc imported-bindings exported-bindings)
  (if (vm-eq? imported-bindings false)
      (begin
	(set! *imported-bindings* (make-hash-table+gc))
	(set! *exported-bindings* (make-hash-table+gc)))
      (begin
	(set! *imported-bindings* imported-bindings)
	(set! *exported-bindings* exported-bindings))))

(define shared-binding-table-size
  (* hash-table-size 2))

(let ((tracer (table-tracer shared-binding-next
			    set-shared-binding-next!
			    s48-trace-value)))
  (add-gc-root!
    (lambda ()
      (set! *imported-bindings* (tracer *imported-bindings*))
      (set! *exported-bindings* (tracer *exported-bindings*)))))

; These next two procedure are used to write the bindings tables out in images.

(define (s48-exported-bindings)
  *exported-bindings*)

(define (s48-imported-bindings)
  *imported-bindings*)

; Imported bindings.

(define lookup-imported-binding
  (let* ((maker (lambda (string next key)
		  (make-shared-binding string true unspecific-value next key)))
	 (lookup (table-searcher shared-binding-name
				 shared-binding-next
				 maker)))
    (lambda (name key)
      (lookup *imported-bindings* name key))))
    
; Exporting bindings.

(define lookup-exported-binding
  (let* ((maker (lambda (string next key)
		  (make-shared-binding string false unspecific-value next key)))
	 (lookup (table-searcher shared-binding-name
				 shared-binding-next
				 maker)))
    (lambda (name key)
      (lookup *exported-bindings* name key))))

; Print warnings about all imported bindings which the external code
; has not yet defined.

(define (shared-binding-undefined? binding)
  (undefined? (shared-binding-ref binding)))

(define for-each-imported-binding
  (let ((walker (table-while-walker shared-binding-next)))
    (lambda (proc)
      (walker proc *imported-bindings*))))
	 
(define undefine-shared-binding!
  (table-remover shared-binding-name
		 shared-binding-next
		 set-shared-binding-next!))

(define (get-imported-binding name)
  (save-temp0! (enter-string+gc name))
  (let* ((key (ensure-space shared-binding-size))
	 (name (recover-temp0!)))
    (lookup-imported-binding name key)))

;----------------
; The following two functions are exported to C, hence the reversal of the
; export/import terminology.

(define (s48-define-exported-binding name value)
  (save-temp0! value)
  (let ((name (enter-string+gc name)))
    (save-temp1! name)
    (let ((key (ensure-space shared-binding-size)))
      (let ((name (recover-temp1!))
	    (value (recover-temp0!)))
	(shared-binding-set! (lookup-imported-binding name key)
			     value)))))

(define (s48-get-imported-binding name)
  (save-temp0! (enter-string+gc name))
  (let* ((key (ensure-space shared-binding-size))
	 (name (recover-temp0!)))
    (lookup-exported-binding name key)))
