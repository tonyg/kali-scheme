; no copyright notice

; ,open signals escapes

; Changes by jar:
;   Added Uses of Scheme 48's WITH-CONTINUATION primitive, so that unreachable
;   continuations can be reclaimed by the GC.
;
;   Renamed reset-thunk -> *reset
;	    call/ct -> *shift
;
; Note: the meta-continuation ought to be thread-specific.
; Alternatively, the threads package could be defined in terms of
; shift and reset.  This would have the advantage of making the threads
; package itself re-entrant.  It would be nice to rehabilitate the
; runnable-threads queue, currently a piece of global state, as local
; to a particular invocation of WITH-MULTITASKING.

;Date: Wed, 29 Dec 1993 13:54:52 +0100
;From: Olivier Danvy <danvy@daimi.aau.dk>
;To: jar@martigny.ai.mit.edu
;Subject: little Christmas gift
;Reply-To: danvy@daimi.aau.dk
;
;Hi again:
;
;Here is a contribution for the Scheme48 library: the shift and reset
;operators from "Abstracting Control" (LFP90) and "Representing Control"
;(MSCS92).  In his POPL94 paper, Andrzej Filinski observed that since the
;meta-continuation is single-threaded, it can be globalized in a
;register.  Andrzej has programmed this both in SML and in Scheme.  I
;only have prettified the Scheme definition a wee bit.

(define-syntax reset
  (syntax-rules ()
    ((_ ?e) (*reset (lambda () ?e)))))

(define-syntax shift
  (syntax-rules ()
    ((_ ?k ?e) (*shift (lambda (?k) ?e)))))

(define *meta-continuation*
  (lambda (v)
    (error "You forgot the top-level reset...")))

(define *abort
  (lambda (thunk)
    (with-continuation null-continuation ;JAR hack
      (lambda ()
	(let ((val (thunk)))
	  (*meta-continuation* val))))))

(define null-continuation #f)

(define *reset
  (lambda (thunk)
    (let ((mc *meta-continuation*))
      (call-with-current-continuation
        (lambda (k)
	  (begin
	    (set! *meta-continuation*
		  (lambda (v)
		    (set! *meta-continuation* mc)
		    (k v)))
	    (*abort thunk)))))))

(define *shift
  (lambda (f)
    (call-with-current-continuation
      (lambda (k)
	(*abort (lambda ()
		  (f (lambda (v)
		       (reset (k v))))))))))

;----------
;
;Reminder: reset specifies a control delimiter.  shift grabs the current
;continuation up to the current control delimiter, and reifies it as a
;composable procedure.  If the procedure is not used, shift has the
;effect of aborting up to the current control delimiter.
;
;Examples:
;
;(+ 10 (reset (+ 2 3)))
;-->
;15
;
;(+ 10 (reset (+ 2 (shift k 3))))
;-->
;13
;
;(+ 10 (reset (+ 2 (shift k (k 3)))))
;-->
;15
;
;(+ 10 (reset (+ 2 (shift k (+ 100 (k 3))))))
;-->
;115
;
;(+ 10 (reset (+ 2 (shift k (+ 100 (k (k 3)))))))
;-->
;117
;
;
;Other reminder: shift and reset are weaker than Matthias's control and
;prompt, in that they can be CPS-transformed.
;
;Have a happy holiday,
;
;-- Olivier
;
;PS: This definition is not unlike David Espinoza's implementation of monadic
;effects, ie, it has no interpretive or translation overhead.



; JAR's notes:
;
;  ; CWCC defined in terms of SHIFT
;
;  (define cwcc
;    (lambda (p)
;      (shift k (k (p (lambda (x)
;                       (shift k1 (k x))))))))
;
;  ; Monads from shift and reset (from Filinski, POPL '94)
;
;  (define (reflect meaning)
;    (shift k (extend k meaning)))
;
;  (define (reify thunk)
;    (reset (eta (thunk))))
;
;  Example: nondeterminism monad.
;
;  > (define (eta x) (list x))
;  > (define (extend f l) (apply append (map f l)))
;  > 
;  > (define-syntax amb
;      (syntax-rules () ((amb ?x ?y) (*amb (lambda () ?x) (lambda () ?y)))))
;
;  > (define (*amb t1 t2)
;      (reflect (append (reify t1) (reify t2))))
;  > 
;  > (reify (lambda () (amb 1 2)))
;  '(1 2)
;  > (reify (lambda () (+ (amb 1 2) 3)))
;  '(4 5)
;  > 
;  > (define cwcc call-with-current-continuation)
;  > (reify (lambda ()
;             (+ 1 (cwcc (lambda (k)
;                          (* 10 (amb 3 (k 4))))))))
;  '(31 51)
;  > 
