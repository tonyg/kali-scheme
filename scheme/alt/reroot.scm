; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


; A state space is a tree with the state at the root.  Each node other
; than the root is a triple <before, after, parent>, represented in
; this implementation as a structure ((before . after) . parent).
; Moving from one state to another means re-rooting the tree by pointer
; reversal. 

(define *here* (list #f))

(define original-cwcc call-with-current-continuation)

(define (call-with-current-continuation proc)
  (let ((here *here*))
    (original-cwcc (lambda (cont)
		     (proc (lambda results
			     (reroot! here)
			     (apply cont results)))))))

(define (dynamic-wind before during after)
  (let ((here *here*))
    (reroot! (cons (cons before after) here))
    (call-with-values during
      (lambda results
	(reroot! here)
	(apply values results)))))

(define (reroot! there)
  (if (not (eq? *here* there))
      (begin (reroot! (cdr there))
	     (let ((before (caar there))
		   (after (cdar there)))
	       (set-car! *here* (cons after before))
	       (set-cdr! *here* there)
	       (set-car! there #f)
	       (set-cdr! there '())
	       (set! *here* there)
	       (before)))))

; -----
;
;(define r #f) (define s #f) (define (p x) (write x) (newline))
;(define (tst)
;  (set! r *here*)
;  (set! s (cons (cons (lambda () (p 'in)) (lambda () (p 'out))) *here*))
;  (reroot! s))
;
;
;(define (check)                         ;Algorithm invariants
;  (if (not (null? (cdr *here*)))
;      (error "confusion #1"))
;  (if (car *here*)
;      (error "confusion #2")))
