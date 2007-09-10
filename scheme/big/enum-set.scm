; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Sets over finite types.
;
; (define-enum-set-type id type-name predicate constructor
;   element-syntax element-predicate all-elements element-index-ref)
;
; Defines ID to be syntax for constructing sets, PREDICATE to be a predicate
; for those sets, and CONSTRUCTOR an procedure for constructing one
; from a list.
;
; (enum-set->list <enum-set>)                   -> <list>
; (enum-set-member? <enum-set> <enumerand>)     -> <boolean>
; (enum-set=? <enum-set> <enum-set>)            -> <boolean>
; (enum-set-union <enum-set> <enum-set>)        -> <enum-set>
; (enum-set-intersection <enum-set> <enum-set>) -> <enum-set>
; (enum-set-negation <enum-set>)                -> <enum-set>
;
; Given an enumerated type:
;   (define-enumerated-type color :color
;     color?
;     colors
;     color-name
;     color-index
;     (red blue green))
; we can define sets of colors:
;   (define-enum-set-type color-set :color-set
;                         color-set?
;                         make-color-set
;     color color? colors color-index)
;
;   (enum-set->list (color-set red blue))
;     -> (#{Color red} #{Color blue})
;   (enum-set->list (enum-set-negation (color-set red blue)))
;     -> (#{Color green})
;   (enum-set-member? (color-set red blue) (color blue))
;     -> #t

(define-syntax define-enum-set-type
  (syntax-rules ()
    ((define-enum-set-type id type predicate constructor
       element-syntax element-predicate all-elements element-index-ref)
     (begin
       (define type
	 (make-enum-set-type 'id
			     element-predicate
			     all-elements
			     element-index-ref))

       (define (predicate x)
	 (and (enum-set? x)
	      (enum-set-has-type? x type)))

       (define (constructor elements)
	 (elements->enum-set type elements))

       (define-enum-set-maker id constructor element-syntax)))))

; (define-enum-set-maker id constructor element-syntax)

(define-syntax define-enum-set-maker
  (lambda (e r c)
    (let ((id             (list-ref e 1))
	  (constructor    (list-ref e 2))
	  (element-syntax (list-ref e 3))
	  (%define-syntax (r 'define-syntax)))
      `(,%define-syntax ,id
          (syntax-rules ()
	    ((,id element ...)
            (,constructor (list (,element-syntax element) ...))))))))

(define-record-type enum-set-type :enum-set-type
  (make-enum-set-type id predicate values index-ref)
  enum-set-type?
  (id        enum-set-type-id)
  (predicate enum-set-type-predicate)
  (values    enum-set-type-values)
  (index-ref enum-set-type-index-ref))

(define (enum-set-type-element-index enum-set-type element)
  ((enum-set-type-index-ref enum-set-type) element))

(define-record-discloser :enum-set-type
  (lambda (e-s-t)
    (list 'enum-set-type (enum-set-type-id e-s-t))))

; The mask is settable to allow for destructive operations.  There aren't
; any such yet.

; The C code (in external-lib.c) knows the layout of this record type.
(define-record-type enum-set :enum-set
  (make-enum-set type mask)
  enum-set?
  (type enum-set-type)
  (mask enum-set-mask set-enum-set-mask!))

(define-record-discloser :enum-set
  (lambda (e-s)
    (cons (enum-set-type-id (enum-set-type e-s))
         (enum-set->list e-s))))

(define-exported-binding "enum-set-type" :enum-set)

(define (enum-set-has-type? enum-set enum-set-type)
  (eq? (enum-set-type enum-set) enum-set-type))

(define enum-set->integer enum-set-mask)

(define integer->enum-set make-enum-set)

(define (elements->enum-set enum-set-type elements)
  (let ((element-predicate (enum-set-type-predicate enum-set-type)))
    (if (every element-predicate elements)
	(make-enum-set enum-set-type
		       (elements->mask elements 
				       (enum-set-type-index-ref enum-set-type)))
	(error "invalid set elements" enum-set-type elements))))

(define (elements->mask elements index-ref)
  (do ((elements elements (cdr elements))
       (mask 0
	     (bitwise-ior mask
			  (arithmetic-shift 1 (index-ref (car elements))))))
      ((null? elements)
       mask)))
				  
(define (enum-set-member? enum-set element)
  (if (enum-set-type-member? (enum-set-type enum-set) element)
      (not (= (bitwise-and (enum-set-mask enum-set)
			   (element-mask element (enum-set-type enum-set)))
	      0))
      (call-error "invalid arguments" enum-set-member? enum-set element)))

(define (enum-set-type-member? enum-set-type element)
  ((enum-set-type-predicate enum-set-type)
   element))

(define (enum-set=? enum-set0 enum-set1)
  (if (eq? (enum-set-type enum-set0)
          (enum-set-type enum-set1))
      (= (enum-set-mask enum-set0) 
	 (enum-set-mask enum-set1))
      (call-error "invalid arguments" enum-set=? enum-set0 enum-set1)))

(define (enum-set-subset? enum-set0 enum-set1)
  (if (eq? (enum-set-type enum-set0)
	   (enum-set-type enum-set1))
      (let ((mask0 (enum-set-mask enum-set0))
	    (mask1 (enum-set-mask enum-set1)))
	(= (bitwise-ior mask0 mask1)
	   mask1))
      (call-error "invalid arguments" enum-set=? enum-set0 enum-set1)))

(define (element-mask element enum-set-type)
  (arithmetic-shift 1
		    ((enum-set-type-index-ref enum-set-type) element)))

; To reduce the number of bitwise operations required we bite off two bytes
; at a time.

(define (enum-set->list enum-set)
  (let ((values (enum-set-type-values (enum-set-type enum-set))))
    (do ((i 0 (+ i 16))
	 (mask (enum-set-mask enum-set) (arithmetic-shift mask -16))
	 (elts '()
	       (do ((m (bitwise-and mask #xFFFF) (arithmetic-shift m -1))
		    (i i (+ i 1))
		    (elts elts (if (odd? m)
				   (cons (vector-ref values i)
					 elts)
				   elts)))
		   ((= m 0)
		    elts))))
	((= mask 0)
	 (reverse elts)))))

(define (enum-set-union enum-set0 enum-set1)
  (if (eq? (enum-set-type enum-set0)
	   (enum-set-type enum-set1))
      (make-enum-set (enum-set-type enum-set0)
		     (bitwise-ior (enum-set-mask enum-set0)
				  (enum-set-mask enum-set1)))
      (call-error "invalid arguments" enum-set-union enum-set0 enum-set1)))

(define (enum-set-intersection enum-set0 enum-set1)
  (if (eq? (enum-set-type enum-set0)
	   (enum-set-type enum-set1))
      (make-enum-set (enum-set-type enum-set0)
		     (bitwise-and (enum-set-mask enum-set0)
				  (enum-set-mask enum-set1)))
      (call-error "invalid arguments" enum-set-union enum-set0 enum-set1)))

(define (enum-set-negation enum-set)
  (let* ((type (enum-set-type enum-set))
	 (mask (- (arithmetic-shift 1
				    (vector-length (enum-set-type-values type)))
		  1)))
    (make-enum-set type
		   (bitwise-and (bitwise-not (enum-set-mask enum-set))
				mask))))

(define (enum-set-difference enum-set0 enum-set1)
  (enum-set-intersection enum-set0
			 (enum-set-negation enum-set1)))
