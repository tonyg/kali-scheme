; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.



; Package mutation tests

"
,translate =scheme48/ ./
,open packages compiler built-in-structures handle condition
,open interfaces table defpackage package-mutation
"

(define (try exp env . should-return-option)
  (let ((val (ignore-errors (lambda () (eval exp env)))))
    (if (if (null? should-return-option)
	    (error? val)
	    (not (if (eq? (car should-return-option) 'error)
		     (error? val)
		     (eq? val (car should-return-option)))))
	(begin (write `(lost: ,exp => ,val))
	       (newline)))))

(define p1 (make-simple-package (list scheme) eval #f 'p1))

(try 'a p1 'error)

(try '(define a 'aa) p1)
(try 'a p1 'aa)

(try '(define (foo) b) p1)
(try '(foo) p1 'error)

(try '(define b 'bb) p1)
(try 'b p1 'bb)
(try '(foo) p1 'bb)


(define s1-sig (make-simple-interface 's1-sig '(((a b c d e f) value))))
(define s1 (make-structure p1 (lambda () s1-sig) 's1))

(define p2 (make-simple-package (list s1 scheme) eval #f 'p2))

(try 'b p2 'bb)
(try 'c p2 'error)
(try 'z p2 'error)

(try '(define (bar) c) p2)
(try '(bar) p2 'error)
(try '(define c 'cc) p1)
(try 'c p2 'cc)
(try '(bar) p2 'cc)

(try '(define (baz1) d) p1)
(try '(define (baz2) d) p2)
(try '(baz1) p1 'error)
(try '(baz2) p2 'error)
(try '(define d 'dd) p1)
(try '(baz1) p1 'dd)
(try '(baz2) p2 'dd)

; Shadow
(try '(define d 'shadowed) p2)
(try '(baz1) p1 'dd)
(try '(baz2) p2 'shadowed)

; Shadow undefined
(try '(define (moo1) f) p1)
(try '(define (moo2) f) p2)
(try '(define f 'ff) p2)
(try '(moo1) p1 'error)
(try '(moo2) p2 'ff)


(try '(define (quux1) e) p1)
(try '(define (quux2) e) p2)
(try '(define (quux3 x) (set! e x)) p1)
(try '(define (quux4 x) (set! e x)) p2)
;
(try '(quux1) p1 'error)
(try '(quux2) p2 'error)
(try '(quux3 'q3) p1 'error)
(try '(quux4 'q4) p2 'error)
;
(try '(define e 'ee) p1)
(try '(quux1) p1 'ee)
(try '(quux2) p2 'ee)
(try '(quux3 'q3) p1)
(try '(quux1) p1 'q3)
(try '(quux2) p2 'q3)
(try '(quux4 'q4) p2 'error)
;
(try '(define e 'ee2) p2)
(try '(quux1) p1 'q3)
(try '(quux2) p2 'ee2)
(try '(quux3 'qq3) p1)
(try '(quux4 'qq4) p2)
(try '(quux1) p1 'qq3)
(try '(quux2) p2 'qq4)


; (set-verify-later! really-verify-later!)

(define-interface s3-sig (export a b x y z))

(define s3
  (make-structure p1 (lambda () s3-sig) 's3))

(define p4 (make-simple-package (list s3 scheme) eval #f 'p4))

(try '(define (fuu1) a) p4)
(try '(define (fuu2) d) p4)
(try '(fuu1) p4 'aa)
(try '(fuu2) p4 'error)

; Remove a, add d
(define-interface s3-sig (export b d x y z))
;(package-system-sentinel)

(try 'a p4 'error)
(try 'd p4 'dd)
(try '(fuu2) p4 'dd)
(try '(fuu1) p4 'error)    ; Foo.



(define (table->alist t)
  (let ((l '()))
    (table-walk (lambda (key val) (set! l (cons (cons key val) l))) t)
    l))

