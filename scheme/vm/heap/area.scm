
; Areas
;
; Memory is divided into areas, each of which has:
;  start address
;  end pointer
;  allocation pointer (points to next unused space)
;  next area in this category
;
; Each generation is a set of areas, grouped into lists by metatype and
; fixed-ness

(define-record-type area :area
  (real-make-area start end frontier)
  (action   integer area-gc-action set-area-gc-action!)
  (start    integer area-start     set-area-start!)      ; inclusive
  (end      integer area-end       set-area-end!)        ; exclusive
  (frontier integer area-frontier  set-area-frontier!)
  (trace    integer area-trace     set-area-trace!)
  (next     area    area-next      set-area-next!))

(define (make-area start end frontier)
  (let ((area (real-make-area start end frontier)))
    (if (null-pointer? area)
	(error "out of memory"))
    area))

(define (area-remaining area)
  (address- (area-end area)
	    (area-frontier area)))

(define (clear-area-next! area)
  (set-area-next! area (null-pointer)))

(define (delete-area! start area)
  (cond ((null-pointer? start)
	 start)
	((eq? start area)
	 (let ((next (area-next area)))
	   (clear-area-next! area)
	   next))
	(else
	 (let loop ((prev start))
	   (let ((next (area-next prev)))
	     (cond ((eq? next area)
		    (set-area-next! prev next))
		   ((not (null-pointer? next))
		    (loop next)))))
	 (clear-area-next! area)
	 start)))

(define-enumeration gc-action
  (ignore
   error
   copy-mixed
   copy-small
   mark-large
   copy-weak))

; Allocate an area of between MINIMUM and MAXIMUM pages, inclusive.

(define (allocate-area minimum maximum)
  (receive (start size)
      (allocate-pages minimum maximum)
    (let ((area (make-area start			; start
			   (add-pages start size)	; end
			   start)))			; alloc
      (do ((i 0 (+ i 1)))
	  ((= i size))
	(memory-map-set! (add-pages start i) area))
      area)))



