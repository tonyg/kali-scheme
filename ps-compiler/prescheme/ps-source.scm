; Copyright (c) 1994 Richard Kelsey.  See file COPYING.

(define prescheme-source
  '(

; Stolen from scheme48/rts/base.scm

;;;; Fundamental definitions

; Order of appearance is approximately that of the Revised^4 Report.

(define (not x) (if x #f #t))

; Simple number stuff

(define (> x y) (< y x))
(define (<= x y) (not (< y x)))
(define (>= x y) (not (< x y)))

(define (abs n) (if (< n 0) (- 0 n) n))
(define (min x y) (if (< y x) y x))
(define (max x y) (if (> y x) y x))

(define (zero? x) (= x 0))
(define (positive? x) (< 0 x))
(define (negative? x) (< x 0))

(define (even? n) (= 0 (remainder n 2)))
(define (odd? n) (not (even? n)))

(define (expt x y)
  (do ((r 1 (* r x))
       (i y (- i 1)))
      ((<= i 0) r)))

; Characters

(define (char>? x y) (char<? y x))
(define (char>=? x y) (not (char<? x y)))
(define (char<=? x y) (not (char>? x y)))

; IO

(define *current-input-port* (stdin))

(define *current-output-port* (stdout))

(define (current-input-port)
  *current-input-port*)

(define (current-output-port)
  *current-output-port*)

(define current-error-port stderr)

(define (open-input-file spec)
  (open-file spec 'read))

(define (open-output-file spec)
  (open-file spec 'write))

(define (close-input-port port)
  (close-port port))

(define (close-output-port port)
  (close-port port))

(define (call-with-input-file name proc)
  (let* ((port (open-input-file name))
	 (result (proc port)))
    (close-input-port port)
    result))

(define (newline port)
  (write-char #\newline port))

; data structures

(define (unsigned-byte-ref index)
  (contents-int8u index))

(define (unsigned-byte-set! index value)
  (set-contents-int8u! index value))

(define (word-ref index)
  (contents-int32 index))

(define (word-set! index value)
  (set-contents-int32! index value))

; case

(define-syntax case
  (lambda (exp rename compare)
    (let ((pred (lambda (const)
		  (cond ((char?   const) (rename 'char=?))
			((number? const) (rename '=))
			(else            (rename 'eqv?)))))
	  (%cond (rename 'cond))
	  (%or (rename 'or))
	  (%value (rename 'x)))
      `(,(rename 'let) ((,%value ,(cadr exp)))
        (,%cond . ,(let loop ((clauses (cddr exp)) (result '()))
		     (cond ((null? clauses)
			    (reverse result))
			   ((compare (caar clauses) 'else)
			    (reverse `((,(rename 'else) . ,(cdar clauses))
				       . ,result)))
			   (else
			    (let ((values (caar clauses))
				  (body (cdar clauses)))
			      (loop (cdr clauses)
				    `(((,%or . ,(map (lambda (value)
						       `(,(pred value) ,%value ,value))
						     values))
				       . ,body)
				      . ,result))))))))))
  (else))
))