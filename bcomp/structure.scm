; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Aignatures and structures.

; Signature items
; An item is either name or (name kind) or (name type).

(define (signature-item-name item)
  (cond ((pair? item) (car item))
	(else item)))

(define (signature-item-type item)
  (cond ((pair? item) (cadr item))
	(else 'value)))

(define (variable-signature-item? item)
  (not (syntax-signature-item? item)))

(define (syntax-signature-item? item)
  (eq? (signature-item-type item) 'syntax))


; Signatures

(define-record-type signature type/signature
  (really-make-signature items ref references)
  signature?
  (items      really-signature-items)
  (ref        really-signature-ref)
  (references really-signature-references))

(define (make-signature items-thunk ref)
  (really-make-signature items-thunk ref (make-population)))

(define (make-simple-signature items)	;(export '(item ...))
  (make-signature (lambda () items)
		  (let ((table (make-table name-hash)))
		    (for-each (lambda (item)
				(table-set! table
					    (signature-item-name item)
					    (signature-item-type item)))
			      items)
		    (lambda (name) (table-ref table name)))))

(define (make-compound-signature . sigs)
  (let ((sig (make-signature (lambda ()
			       (apply append (map signature-items sigs)))
			     (lambda (name)
			       (let loop ((sigs sigs))
				 (if (null? sigs)
				     #f
				     (or (signature-ref (car sigs) name)
					 (loop (cdr sigs)))))))))
    (for-each (lambda (s)
		(note-reference-to-signature! s sig))
	      sigs)
    sig))

(define (signature-items sig)
  ((really-signature-items (if (procedure? sig) (sig) sig))))

(define (signature-ref sig name)
  ((really-signature-ref (if (procedure? sig) (sig) sig)) name))

(define (signature-references sig)
  (really-signature-references (if (procedure? sig) (sig) sig)))

(define (note-reference-to-signature! sig thing)
  (if (procedure? sig)
      (add-to-population! thing (signature-references sig))))



; Structures

(define-record-type structure type/structure
  (really-make-structure name signature package clients)
  structure?
  (name	     structure-name)
  (signature structure-signature)
  (package   structure-package)
  (clients   structure-clients))

(define-record-discloser type/structure
  (lambda (s) (list 'structure
		    (package-uid (structure-package s))
		    (structure-name s))))

(define (make-structure name sig package)
  (set-package-name! package name)
  (let ((struct (really-make-structure name sig package
				       (make-population))))
    (add-to-population! struct (package-structures package))
    (note-reference-to-signature! sig struct)
    struct))

(define (structure-unstable? struct)
  (package-unstable? (structure-package struct)))


; Exported names

(define (structure-exports? s name)
  (signature-ref (structure-signature s) name))


; Return a list of names that are exported but not defined.

(define (undefined-exports s)
  (let ((p (structure-package s))
	(names '()))
    (for-each (lambda (item)
		(let ((name (signature-item-name item)))
		  (if (unbound? (probe-package p name))
		      (set! names (cons name names)))))
	      (signature-items (structure-signature s)))
    (reverse names)))


; Deal with editing operations

(define (verify-later! thunk)
  (let ((loser (ignore-errors thunk)))
    (cond ((or (structure? loser)
	       (signature? loser))
	   (set-fluid! $losers (cons loser (fluid $losers))))))
  #f)

(define $losers (make-fluid '()))
(define $package-losers (make-fluid '()))

(define (package-system-sentinel)
  (let ((drain (lambda (flu check)
		 (let loop ()
		   (let ((losers (fluid flu)))
		     (if (not (null? losers))
			 (let ((loser (car losers)))
			   (set-fluid! flu (cdr losers))
			   (check loser)
			   (loop))))))))
    (letrec ((check
	      (lambda (loser)
		(cond ((structure? loser)
		       (walk-population (lambda (p)
					  (reinitialize-package! p)
					  (let ((ps (fluid $package-losers)))
					    (if (not (memq p ps))
						(set-fluid! $package-losers
							    (cons p ps)))))
					(structure-clients loser)))
		      ((signature? loser)
		       (walk-population check
					(signature-references loser)))))))
      (drain $losers check))
    (drain $package-losers verify-package)))

(define (package-open! p struct-thunk)
  (if (not (memq (struct-thunk) (package-opens p)))
      (let ((thunk (package-opens-thunk p)))
	(set-package-opens-thunk! p (lambda ()
				      (cons (struct-thunk) (thunk))))
	(reinitialize-package! p)
	(verify-package p))))


(define (check-structure s)
  (let ((names (undefined-exports s)))
    (if (not (null? names))
	(warn "Structure has undefined exports"
	      s
	      (reverse names)))))
