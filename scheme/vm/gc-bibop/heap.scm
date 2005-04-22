; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Variables shared by various parts of the BIBOP code

(define *max-heap-size* 0)

(define (s48-max-heap-size)
  *max-heap-size*)

(define (s48-set-max-heap-size! size)
  (set! *max-heap-size* size))

; addresses of the new allocated heap areas
; <= s48_initialize_heap()
(define *new-small-start-addr* null-address)
(define *new-large-start-addr* null-address)
(define *new-weaks-start-addr* null-address)

(define (s48-get-new-small-start-addr) *new-small-start-addr*)

(define (s48-get-new-large-start-addr) *new-large-start-addr*)

(define (s48-get-new-weaks-start-addr) *new-weaks-start-addr*)

(define (s48-set-new-small-start-addr! addr)
  (set! *new-small-start-addr* addr))

(define (s48-set-new-large-start-addr! addr)
  (set! *new-large-start-addr* addr))

(define (s48-set-new-weaks-start-addr! addr)
  (set! *new-weaks-start-addr* addr))

;; ** Availability ***************************************************

(define (s48-available? cells)
  (>= (s48-available) cells))

(define (bytes-available? bytes)
  (>= (s48-available) (bytes->cells bytes)))

;; ** Initialization *************************************************

; the bibop-gc doesn't look at these areas at all yet... TODO?!

;; (initial values for the type-checker)
(define *pure-areas*)
(define *impure-areas*)
(define *pure-sizes*)
(define *impure-sizes*)
(define *pure-area-count* 0)
(define *impure-area-count* 0)


(define (s48-initialize-heap max-heap-size image-start-address image-size)
  (address= image-start-address null-address) ; for the type checker
  (= image-size 0)			; for the type checker

  (set! *max-heap-size* max-heap-size)

  (s48-initialize-bibop-heap)

  ;; just some silly things for the type-checker...
  (set! *pure-areas*  (make-vector 0 (integer->address 0)))
  (set! *impure-areas*  *pure-areas*)
  (set! *pure-sizes*  (make-vector 0 0))
  (set! *impure-sizes* *pure-sizes*))

;----------------
; Keeping track of all the areas.

(define (s48-register-static-areas pure-count pure-areas pure-sizes
				   impure-count impure-areas impure-sizes)
  (set! *pure-area-count* pure-count)
  (set! *pure-areas* pure-areas)
  (set! *pure-sizes* pure-sizes)
  (set! *impure-area-count* impure-count)
  (set! *impure-areas* impure-areas)
  (set! *impure-sizes* impure-sizes))

(define (walk-areas proc areas sizes count)
  (let loop ((i 0))
    (cond ((>= i count)
	   #t)
	  ((proc (vector-ref areas i)
		 (address+ (vector-ref areas i)
			   (vector-ref sizes i)))
	   (loop (+ i 1)))
	  (else
	   #f))))

(define (walk-pure-areas proc)
  (if (< 0 *pure-area-count*)
      (walk-areas proc *pure-areas* *pure-sizes* *pure-area-count*)
      #t))

(define (walk-impure-areas proc)
  (if (< 0 *impure-area-count*)
      (walk-areas proc *impure-areas* *impure-sizes* *impure-area-count*)
      #t))
