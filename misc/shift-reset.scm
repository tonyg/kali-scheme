; no copyright notice

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
    ((_ ?e) (reset-thunk (lambda () ?e)))))

(define-syntax shift
  (syntax-rules ()
    ((_ ?k ?e) (call/ct (lambda (?k) ?e)))))

(define *meta-continuation*
  (lambda (v)
    (error "You forgot the top-level reset...")))

(define abort
  (lambda (v)
    (*meta-continuation* v)))

(define reset-thunk
  (lambda (t)
    (let ((mc *meta-continuation*))
      (call-with-current-continuation
        (lambda (k)
	  (begin
	    (set! *meta-continuation* (lambda (v)
					(begin
					  (set! *meta-continuation* mc)
					  (k v))))
	    (abort (t))))))))

(define call/ct
  (lambda (f)
    (call-with-current-continuation
      (lambda (k)
	(abort (f (lambda (v)
		    (reset (k v)))))))))

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
