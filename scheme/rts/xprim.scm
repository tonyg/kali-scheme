; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Hairier VM exceptions & interrupts.
; Enable generic arithmetic, informative error messages, etc.

; Deal with optional arguments, etc. to primitives.
; This is not necessarily the cleanest way to do this, and certainly not
; the most efficient, but for the time being it's the most expedient.

; We don't want to depend on tables.  But if we did, we might do this:
;(define (closure-hash closure)
;  (let ((cv (vector-ref (closure-template closure) 0)))  ;template-ref
;    (do ((h 0 (+ h (code-vector-ref cv i)))
;         (i (- (code-vector-length cv) 1) (- i 1)))
;        ((< i 0) h))))
;(define wna-handlers (make-table closure-hash))

;(define-vm-exception-handler (enum op check-nargs=)
;  (lambda (opcode reason proc args)
;    (let ((probe (assq proc *wna-handlers*)))
;      (if probe
;          ((cdr probe) args)
;          (signal-vm-exception opcode reason proc args)))))

(define *wna-handlers* '())

(define (define-wna-handler proc handler)
  (set! *wna-handlers* (cons (cons proc handler) *wna-handlers*)))

(define op/check-nargs= (enum op protocol)) ; temporary hack

(define (wna-lose proc args)
  (signal-vm-exception op/check-nargs= #f proc args))  ; lost our reason

