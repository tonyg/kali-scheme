; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Bindings: used to store bindings in packages.

; Representation is #(type place operator-or-transform-or-#f [path]).
; PLACE is a unique (to EQ?) value, usually a location.

; Why aren't these records?  Because they need to be written out in
; the initial image?

(define binding? vector?)

(define (binding-type b)   (vector-ref b 0))
(define (binding-place b)  (vector-ref b 1))
(define (binding-static b) (vector-ref b 2))

(define (binding-path binding)
  (if (= 4 (vector-length binding))
      (vector-ref binding 3)
      #f))

(define (set-binding-place! binding place)
  (vector-set! binding 1 place))

(define (make-binding type place static)
  (vector type place static))

(define (add-path binding path)
  (vector (binding-type binding)
	  (binding-place binding)
	  (binding-static binding)
	  path))

; Used when updating a package binding.

(define (clobber-binding! binding type place static)
  (vector-set! binding 0 type)
  (if place
      (vector-set! binding 1 place))
  (vector-set! binding 2 static))

; Return a binding that's similar to the given one, but has its type
; replaced with the given type.

(define (impose-type type binding integrate?)
  (if (or (eq? type syntax-type)
	  (not (binding? binding)))
      binding
      (make-binding (if (eq? type undeclared-type)
			(let ((type (binding-type binding)))
			  (if (variable-type? type)
			      (variable-value-type type)
			      type))
			type)
		    (binding-place binding)
		    (if integrate?
			(binding-static binding)
			#f))))

; Return a binding that's similar to the given one, but has any
; procedure integration or other unnecesary static information
; removed.  But don't remove static information for macros (or
; structures, interfaces, etc.)

(define (forget-integration binding)
  (if (and (binding-static binding)
	   (subtype? (binding-type binding) any-values-type))
      (make-binding (binding-type binding)
		    (binding-place binding)
		    #f)
      binding))

; Do X and Y denote the same thing?

(define (same-denotation? x y)
  (or (eq? x y)	    ; was EQUAL? because of names, now just for nodes
      (and (binding? x)
	   (binding? y)
	   (eq? (binding-place x)
		(binding-place y)))))

; Special kludge for shadowing and package mutation.
; Ignore this on first reading.  See env/shadow.scm.

(define (maybe-fix-place! binding)
  (let ((place (binding-place binding)))
    (if (and (location? place)
             (vector? (location-id place)))
        (set-binding-place! binding (follow-forwarding-pointers place))))
  binding)

(define (follow-forwarding-pointers place)
  (let ((id (location-id place)))
    (if (vector? id)
        (follow-forwarding-pointers (vector-ref id 0))
        place)))

