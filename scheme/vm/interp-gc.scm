; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Different ways to call the GC, depending on how many temporaries need to
; be traced.

; ENSURE-SPACE and friends make sure that there is at least SPACE remaining
; in the heap, triggering a GC if necessary.  A key to the pre-allocated space
; is returned (keys and pre-allocation are described in heap.scm).  If SPACE
; is not available, even after a GC, we kill the process.

(define (ensure-space space)
  (receive (key temp)
      (ensure-space-saving-temp space false)
    key))

(define (ensure-space-saving-temp space temp)
  (receive (okay? key temp ignore)
      (maybe-ensure-space-saving-temps space temp false)
    (if (not okay?)
	(error "Scheme48 heap overflow"))
    (values key temp)))

(define (ensure-space-saving-temps space temp0 temp1)
  (receive (okay? key temp0 temp1)
      (maybe-ensure-space-saving-temps space temp0 temp1)
    (if (not okay?)
	(error "Scheme48 heap overflow"))
    (values key temp0 temp1)))

; Same thing, except that we return a flag instead of dying if the space is
; not available.

(define (maybe-ensure-space space)
  (receive (okay? key ignore0 ignore1)
      (maybe-ensure-space-saving-temps space false false)
    (values okay? key)))

(define (maybe-ensure-space-saving-temp space temp)
  (receive (okay? key temp ignore0)
      (maybe-ensure-space-saving-temps space temp false)
    (values okay? key temp)))

(define (maybe-ensure-space-saving-temps space temp0 temp1)
  (if (s48-available? space)
      (values #t (s48-preallocate-space space) temp0 temp1)
      (receive (temp0 temp1)
	  (collect-saving-temps temp0 temp1)
	(if (s48-available? space)
	    (values #t (s48-preallocate-space space) temp0 temp1)
	    (values #f 0 temp0 temp1)))))

;----------------
; The GC itself, with versions that handle one or two temporaries.

(define (collect)
  (receive (ignore0 ignore1)
      (collect-saving-temps (enter-fixnum 0) (enter-fixnum 0))
    0))  ; type checker rejected (values) here; why?

(define (collect-saving-temp value)
  (receive (value ignore1)
      (collect-saving-temps value (enter-fixnum 0))
    value))

; This is a front for the real GC, which is a separate program.  All that the
; interpreter contributes is the root set and some post-gc cleanup code.

(define (collect-saving-temps value0 value1)
  (s48-begin-collection)
  (gc-root)
  (let ((value0 (s48-trace-value value0))
	(value1 (s48-trace-value value1)))
    (s48-do-gc)
    (post-gc-cleanup)
    (s48-end-collection)
    (values value0 value1)))

;----------------
; GC-ROOT and POST-GC-CLEANUP are defined incrementally.
;
; (ADD-GC-ROOT! <thunk>)           ; call <thunk> when tracing the GC roots
; (ADD-POST-GC-CLEANUP! <thunk>)   ; call <thunk> when a GC has finished
;
; (GC-ROOT)                        ; call all the root thunks
; (POST-GC-CLEANUP)                ; call all the cleanup thunks

(define-syntax define-extensible-proc
  (syntax-rules ()
    ((define-extensible-proc proc extender temp)
     (begin
       (define temp unspecific)
       (define (proc) (temp))
       (define (extender more)
	 (let ((old temp))
	   (set! temp (lambda ()
			(more)
			(old)))))))))

(define-extensible-proc gc-root
  add-gc-root!
  *gc-root-proc*)

(define-extensible-proc post-gc-cleanup
  add-post-gc-cleanup!
  *post-gc-cleanup*)

