; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Signatures

(define-record-type signature type/signature
  (really-make-signature ref walk clients name)
  signature?
  (ref ref-method)
  (walk walk-method)
  (clients signature-clients)
  (name signature-name))

(define-record-discloser type/signature
  (lambda (sig) (list 'signature (signature-name sig))))


(define (signature-ref sig name)
  ((ref-method sig) name))

(define (for-each-declaration proc sig)
  ((walk-method sig) proc))

(define (note-reference-to-signature! sig thing)
  (let ((pop (signature-clients sig)))
    (if pop
	(add-to-population! thing pop)
	;; If it's compound, we really ought to descend into its components
	)))

; If name is #f, then the signature is anonymous, so we don't need to
; make a population.

(define (make-signature ref walk name)
  (really-make-signature ref walk (if name (make-population) #f) name))


; Simple signatures (export (name type) ...)

(define (make-simple-signature name items)
  (let ((table (make-table name-hash)))
    (for-each (lambda (item)
		(if (pair? item)
		    (let ((name (car item))
			  (type (cadr item)))
		      (if (or (null? name) (pair? name))
			  ;; Allow ((name1 name2 ...) type)
			  (for-each (lambda (name)
				      (table-set! table name type))
				    name)
			  (table-set! table name type)))
		    (table-set! table item 'undeclared)))
	      (if (vector? items)
		  (vector->list items)
		  items))
    (really-make-simple-signature table name)))

(define (really-make-simple-signature table name)
  (make-signature (lambda (name) (table-ref table name))
		  (lambda (proc) (table-walk proc table))
		  name))


; Compoune signatures

(define (make-compound-signature name . sigs)
  (let ((sig
	 (make-signature (lambda (name)
			   (let loop ((sigs sigs))
			     (if (null? sigs)
				 #f
				 (or (signature-ref (car sigs) name)
				     (loop (cdr sigs))))))
			 (lambda (proc)
			   (for-each (lambda (sig)
				       (for-each-declaration proc sig))
				     sigs))
			 name)))
    (for-each (lambda (s)
		(note-reference-to-signature! s sig))
	      sigs)
    sig))
