; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.



(define (test x)
  (cond ((= x 1)
	 (goto raise-exception 2))
	((= x 2)
	 (goto raise-exception1 2 0))
	((= x 3)
	 (goto raise-exception 3))
	((= x 4)
	 (goto raise-exception1 4 0))))

(define *code-pointer* 0)
(define *stack* 0)

(define (push x)
  (vector-set! *stack* 0 x)
  (set! *stack* (- *stack* 4)))

(define (fetch-byte address)
  (byte-vector-ref address 0))

(define (current-opcode code-args)
  (fetch-byte (- *code-pointer* (+ code-args 1))))

(define (start-exception args)
  (push (current-opcode args)))

(define (raise-exception args)
  (start-exception args)
  (goto raise 0))

(define (raise-exception1 args a1)
  (start-exception args)
  (push a1)
  (goto raise 1))

(define (raise n)
  (goto test n))

(define-local-syntax (define-primitive id nargs)
  (let ((args (reverse (list-tail '(z y x) (- '3 nargs)))))
    `(define (,id . ,args)
       (call-primitively ,id . ,args))))

(define-local-syntax (define-effect-primitive id nargs)
  (let ((args (reverse (list-tail '(z y x) (- '3 nargs)))))
    `(define (,id . ,args)
       (call-primitively ,id . ,args)
       (call-primitively undefined-value))))

(define-primitive + 2)
(define-primitive - 2)
(define-primitive * 2)
(define-primitive < 2)
(define-primitive = 2)

(define-primitive quotient  2)
(define-primitive remainder 2)

(define (unassigned) (call-primitively undefined-value))

(define (byte-vector-ref vec index)
  (call-primitively byte-contents (+ vec index)))

(define (byte-vector-set! vec index value)
  (call-primitively byte-set-contents! (+ vec index) value))

(define (vector-set! vec index value)
  (call-primitively set-contents! (+ vec (* index 4)) value))
