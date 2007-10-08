; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Scheme 48's data representations, for writing heap images.
; Defines things needed by TRANSPORT (which is called by WRITE-IMAGE, which
; is called by the static linker).

(define (fixnum-value? x)
  (and (integer? x)
       (>= x least-fixnum-value)
       (<= x greatest-fixnum-value)))

(define (immediate-value? x)
  (or (fixnum-value? x)
      (char? x)
      (eq? x #t)
      (eq? x '())    ; I think order is important here as () is a literal while
      (eq? x #f)     ; #F is generated with an instruction.  If () = #f then
		     ; we still lose on something like '(#F) (which is
		     ; present in the system). 
      ;; (eq? thing (unspecific))
      ))

(define car-offset 0)
(define cdr-offset 1)

(define closure-template-offset 0)
(define closure-env-offset	1)

(define location-id-offset     0)

(define vm-true	 (make-immediate (enum imm true)  0))
(define vm-false (make-immediate (enum imm false) 0))
(define vm-null	 (make-immediate (enum imm null)  0))
(define vm-unspecific  (make-immediate (enum imm unspecific) 0))
(define vm-unbound     (make-immediate (enum imm undefined) 1))
(define vm-unassigned  (make-immediate (enum imm undefined) 2))

(define (make-stob-descriptor addr)
  (bitwise-ior (enum tag stob) addr))
