; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Internal variable representing the failure stack.

(define (fail) (*fail*))

(define *fail* (lambda () (error "You didn't do (init).")))


; For the alternation operator, Icon's a | b or McCarthy's (amb a b),
; we write (either a b).

(define-syntax either
  (syntax-rules ()
    ((either) (fail))
    ((either x) x)
    ((either x y ...)
     (%either (lambda () x) (lambda () (either y ...))))))

(define (%either thunk1 thunk2)		;Macro auxiliary
  (saving-failure-state
   (lambda (restore)
     ((call-with-current-continuation
	(lambda (k)
	  (set! *fail*
		(lambda ()
		  (restore)
		  (k thunk2)))
	  thunk1))))))

(define (saving-failure-state proc)
  (let ((save *fail*))
    (proc (lambda () (set! *fail* save)))))


; (one-value x) is Prolog's CUT operator

(define-syntax one-value
  (syntax-rules ()
    ((one-value x) (%one-value (lambda () x)))))

(define (%one-value thunk)
  (saving-failure-state
   (lambda (restore)
     (let ((value (thunk)))
       (restore)
       value))))


; (all-values a) returns a list of all the possible values of the
; expression a.  Prolog calls this "bagof"; I forget what Icon calls it.

(define-syntax all-values
  (syntax-rules ()
    ((all-values x) (%all-values (lambda () x)))))

(define (%all-values thunk)
  (let ((results '()))
    (either (let ((new-result (thunk)))
	      (set! results (cons new-result results))
	      (fail))
	    (reverse results))))


; Generate all the members of list l.  E.g.
;   (all-values (+ (member-of '(10 20 30)) (member-of '(1 2 3))))
;     => '(11 12 13 21 22 23 31 32 33)

(define (member-of l)
  (if (null? l)
      (fail)
      (either (car l) (member-of (cdr l)))))


; Crufty initialization hack that allows you to type failing
; expressions at the R-E-P loop (if there is an R-E-P loop).  E.g. try
; evaluating the sequence
;  (either 1 2)
;  (fail)
;  (+ (fail) 10)

(define (init)
  (set! *fail* #f)			;for GC purposes
  (either 'initialized
	  (let loop ()
	    (either 'failed (loop)))))

(display "Type (init) at the read-eval-print loop.")
(newline)
