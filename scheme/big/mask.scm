; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Boxed bit-masks.

; Mask-type operations
;  (make-mask-type name thing? int->thing thing->int size) -> mask-type
;  (mask-type? x) -> boolean
;
; Internal operations
;  (mask? x)
;  (mask-type mask)
;  (mask-has-type? mask mask-type)
;  (integer->mask mask-type integer)
;  (list->mask mask-type list)
;
; Generic operations
;  (mask->integer mask)
;  (mask->list mask) -> things
;  (mask-member? mask x)
;  (mask-set mask . things)
;  (mask-clear mask . things)
;  (mask-union ...)
;  (mask-intersection ...)
;  (mask-subtract x y)

; Mask-types 

(define-record-type mask-type :mask-type
  (make-mask-type name element? integer->element element->integer size)
  mask-type?
  (name mask-type-name)
  (element? mask-type-element?)
  (integer->element mask-type-integer->element)
  (element->integer mask-type-element->integer)
  (size mask-type-size))
  
(define-record-discloser :mask-type
  (lambda (mt)
    (list 'mask-type (mask-type-name mt))))

; Masks - the type and an integer representing the contents.

(define-record-type mask :mask
  (make-mask type contents)
  mask?
  (type mask-type)
  (contents mask->integer))

(define-record-discloser :mask
  (lambda (m)
    (list (mask-type-name (mask-type m))
	  (string-append "#x"
			 (number->string (mask->integer m) 16)))))

(define (mask-has-type? mask type)
  (if (mask-type? type)
      (eq? (mask-type mask)
	   type)
      (call-error mask-has-type? (list mask type))))

(define (integer->mask type integer)
  (if (and (mask-type? type)
	   (integer? integer)
	   (<= 0 integer))	; no infinite masks
      (make-mask type integer)
      (call-error integer->mask (list type integer))))

(define (list->mask type things)
  (make-mask type (list->integer type things)))

(define (list->integer type things)
  (let ((elt->int (mask-type-element->integer type)))
    (do ((things things (cdr things))
	 (m 0 (bitwise-ior m (arithmetic-shift 1 (elt->int (car things))))))
	((null? things)
	 m))))
		    
; Return a list of the elements of the mask.  This would be faster for bignums
; if we broke off fixnum-sized chunks.

(define (mask->list mask)
  (let ((int->elt (mask-type-integer->element (mask-type mask))))
    (do ((mask (mask->integer mask) (arithmetic-shift mask -1))
	 (i 0 (+ i 1))
	 (elts '() (if (odd? mask)
		       (cons (int->elt i) elts)
		       elts)))
	((= 0 mask)
	 (reverse elts)))))

;----------------
; Operations on masks

(define (mask-member? mask thing)
  (not (= 0 (bitwise-and (mask->integer mask)
			 (arithmetic-shift 1
					   ((mask-type-element->integer
					     (mask-type mask))
					    thing))))))

(define (mask-set mask . things)
  (if (null? things)
      mask
      (make-mask (mask-type mask)
		 (bitwise-ior (mask->integer mask)
			      (list->integer (mask-type mask)
					     things)))))

(define (mask-clear mask . things)
  (if (null? things)
      mask
      (make-mask (mask-type mask)
		 (bitwise-and (mask->integer mask)
			      (bitwise-not (list->integer (mask-type mask)
							  things))))))
; Union and intersection

(define (mask-union mask . more-masks)
  (mask-binop mask more-masks bitwise-ior mask-union))

(define (mask-intersection mask . more-masks)
  (mask-binop mask more-masks bitwise-and mask-intersection))

(define (mask-binop mask more-masks bitwise-op mask-op)
  (if (and (mask? mask)
	   (let ((type (mask-type mask)))
	     (every (lambda (mask)
		      (and (mask? mask)
			   (eq? (mask-type mask) type)))
		    more-masks)))
      (make-mask (mask-type mask)
		 (apply bitwise-op
			(mask->integer mask)
			(map mask->integer more-masks)))
      (call-error mask-op (cons mask more-masks))))
			
; Subtraction

(define (mask-subtract x y)
  (if (and (mask? x)
	   (mask? y)
	   (eq? (mask-type x)
		(mask-type y)))
      (make-mask (mask-type x)
		 (bitwise-and (mask->integer x)
			      (bitwise-not (mask->integer y))))
      (call-error mask-subtract (list x y))))

; Negation
; This is legal only for masks with a size limit.

(define (mask-negate mask)
  (if (and (mask? mask)
	   (mask-type-size (mask-type mask)))
      (let ((type (mask-type mask)))
	(make-mask type
		   (bitwise-and (bitwise-not (mask->integer mask))
				(- (arithmetic-shift 1 (mask-type-size type))
				   1))))
      (call-error mask-negate mask)))
