; Copyright (c) 1994 Richard Kelsey.  See file COPYING.


; Type schemes

(define-record-type type-scheme
  (
   type           ; a type
   free-uvars     ; uvars that are free and unconstrained
   lattice-uvars  ; uvars that are free but constrained
   )
  ())

(define make-type-scheme type-scheme-maker)

(define-record-discloser type/type-scheme
  (lambda (type-scheme)
    (list 'type-scheme
	  (map uvar-id (type-scheme-free-uvars type-scheme))
	  (type-scheme-type type-scheme))))

(define (simple-polymorphic-scheme? type-scheme)
  (null? (type-scheme-lattice-uvars type-scheme)))

; If TYPE has any variables bound at DEPTH this returns a type scheme making
; those variables polymorphic; otherwise TYPE is returned.

; Would like to do limited finalizing of uvars, but can't.
; Consider (lambda (g x) (tuple (g 3) (g x) x))
; (a -> b) -> c -> [d, e, f] with
; a > int8, d > b, a > c, e > b, f > c
; No polymorphism, and no simplification without restricting someone
; But consider NOT  a ->b, bool > a, b > bool
; It could just as well be bool -> bool.
; Simplification okay on variables that are not used inside other types?

(define *free-uvars* '())
(define *lattice-uvars* '())

(define (schemify-type type depth)
  (let ((overload? #f))
  (set! *free-uvars*    '())
  (set! *lattice-uvars* '())
  (let* ((type (find-free-uvars type depth overload?))
	 (lattice-uvars (map (lambda (uvar)
			       (let ((new (uvar-place uvar)))
				 (set-uvar-place! uvar #f)
				 new))
			     *lattice-uvars*))
	 (free-uvars *free-uvars*))
    (set! *free-uvars*    '()) ; drop pointers
    (set! *lattice-uvars* '())
    (for-each (lambda (uvar)
		(set-uvar-place! uvar #f))
	      free-uvars)
    (if (or (not (null? lattice-uvars))
	    (not (null? free-uvars)))
	(make-type-scheme type free-uvars lattice-uvars)
	type))))
  
(define (find-free-uvars type depth overload?)
  (let label ((type type))
    (cond ((other-type? type)
	   (make-other-type (other-type-kind type)
			    (map label
				 (other-type-subtypes type))))
	  ((not (uvar? type))
	   type)
	  ((uvar-binding type)
	   => label)
	  (else
	   (if (and (not (uvar-place type))
		    (<= depth (uvar-depth type)))
	       (find-free-uvar type depth overload?))
	   type))))

(define (find-free-uvar uvar depth overload?)
  (cond ((or (not (uvar-constrained? uvar))
	     (not (uvar-constrained-at-depth? uvar depth)))
	 (set-uvar-place! uvar uvar)
	 (set! *free-uvars* (cons uvar *free-uvars*)))))

;	(overload?
;	 (copy-lattice-uvar uvar depth))

(define (uvar-constrained-at-depth? uvar depth)
  (let ((flag (make-vector 1)))
    (define (do-source relation)
      (do-uvar (relation-source relation)))
    (define (do-sink relation)
      (do-uvar (relation-sink relation)))
    (define (do-uvar uvar)
      (cond ((or (not (uvar? uvar))
		 (uvar-arith-op? uvar)
		 (> depth (uvar-depth uvar)))
	     #t)
	    ((eq? flag (uvar-temp uvar))
	     #f)
	    (else
	     (set-uvar-temp! uvar flag)
	     (or (any? do-sink (uvar-sinks uvar))
		 (any? do-source (uvar-sources uvar))))))
    (cond ((do-uvar uvar) #t)
	  (else
	   (bind-related-uvars! uvar)
	   (set-uvar-constraint-count! uvar 0)
	   (set-uvar-sources! uvar '())
	   (set-uvar-sinks! uvar '())
	   #f))))

(define (bind-related-uvars! initial-uvar)
  (define (do-uvar uvar)
    (cond ((not (or (uvar-binding uvar)
		    (eq? uvar initial-uvar)))
	   (set-uvar-binding! uvar initial-uvar)
	   (for-each do-relation-source (uvar-sources uvar))
	   (for-each do-relation-sink (uvar-sinks uvar))
	   (set-uvar-sources! uvar '())
	   (set-uvar-sinks! uvar '()))))

  (define (do-relation-source relation)
    (do-uvar (relation-source relation)))
  (define (do-relation-sink relation)
    (do-uvar (relation-sink relation)))

  (for-each do-relation-source (uvar-sources initial-uvar))
  (for-each do-relation-sink (uvar-sinks initial-uvar)))

; This code is completely bogus.

;(define (copy-lattice-uvar uvar depth)
;  (define (copy-relation relation)
;    (make-relation (copy (relation-sink    relation))
;                   (copy (relation-source1 relation))
;                   (copy (relation-source2 relation))
;                   (relation-site          relation)
;                   (relation-transmitter   relation)))
;  (define (copy type)
;    (cond ((not (uvar? type))  ; lattice type
;           type)
;          ((uvar-place uvar)
;           => identity)
;          ((> depth (uvar-depth uvar))
;           uvar)
;          (else
;           (let ((new (make-uvar (uvar-prefix uvar) (uvar-depth uvar))))
;             (set! *lattice-uvars* (cons uvar *lattice-uvars*))
;             (set-uvar-place! uvar new)
;             (make-uvar-constrained! new)
;             (set-uvar-sinks! new (map copy-relation (uvar-sinks uvar)))
;             (set-uvar-sources!   new (map copy-relation (uvar-sources   uvar)))
;             new))))
;  (copy uvar))
    
; Instantiate SCHEME at DEPTH.
;
; New sequence:
;   (instantiate-type-scheme scheme depth)
;   ... elide bindings in new copy ...
;   (clean-type-scheme scheme)

(define (instantiate-type-scheme scheme depth . maybe-thunk)
  (instantiate-type-scheme! scheme depth)
  (let ((type (copy-type (type-scheme-type scheme))))
    (if (not (null? maybe-thunk))
	((car maybe-thunk)))
    (clean-type-scheme! scheme)
    type))

(define (instantiate-type-scheme! scheme depth)
  (let ((constrained (type-scheme-lattice-uvars scheme))
	(uid (unique-id)))
    (for-each (lambda (uvar)
		(set-uvar-place!
		 uvar
		 (make-uvar (uvar-prefix uvar) depth uid)))
	      (type-scheme-free-uvars scheme))
    (for-each (lambda (uvar)
		(set-uvar-place!
		 uvar
		 (make-uvar (uvar-prefix uvar) depth uid)))
	      constrained)))
;    (for-each instantiate-uvar constrained)

(define (clean-type-scheme! scheme)
  (for-each (lambda (uvar)
	      (set-uvar-place! uvar #f))
	    (type-scheme-free-uvars scheme))
  (for-each (lambda (uvar)
	      (set-uvar-place! uvar #f))
	    (type-scheme-lattice-uvars scheme)))

;(define (instantiate-uvar uvar)
;  (let ((new (uvar-place uvar)))
;    (mark-uvar-constrained! new)
;    (set-uvar-sinks! new (map instantiate-relation (uvar-sinks uvar)))
;    (set-uvar-sources!   new (map instantiate-relation (uvar-sources uvar)))))
;
;(define (instantiate-relation relation)
;  (make-relation (instantiate-x (relation-sink    relation))
;                 (instantiate-x (relation-source1 relation))
;                 (instantiate-x (relation-source2 relation))
;                 (relation-site  relation)   ; WRONG - need to make a new copy
;                 (relation-transmitter? relation))) ; of the source as well
;
;(define (instantiate-x x)
;  (cond ((and (uvar? x)
;              (uvar-place x))
;         => identity)
;        (else x)))

(define (copy-type type)
  (cond ((other-type? type)
	 (make-other-type (other-type-kind type)
			  (map copy-type
			       (other-type-subtypes type))))
	((not (uvar? type))
	 type)
	((uvar-place type)
	 => identity)
	((uvar-binding type)
	 => copy-type)
	(else
	 type)))


