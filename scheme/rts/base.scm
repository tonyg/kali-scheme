; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file base.scm.

;;;; Fundamental definitions

; Order of appearance is approximately that of the Revised^4 Report.

; Booleans

(define (not x) (if x #f #t))
(define (boolean? x) (or (eq? x #t) (eq? x #f)))

; Equality

(define (eqv? x y)
  (or (eq? x y)
      (and (number? x)
           (number? y)
           (eq? (exact? x) (exact? y))
           (= x y))))

(define (equal? obj1 obj2)
  (cond ((eqv? obj1 obj2) #t)
        ((pair? obj1)
         (and (pair? obj2)
              (equal? (car obj1) (car obj2))
              (equal? (cdr obj1) (cdr obj2))))
        ((string? obj1)
         (and (string? obj2)
              (string=? obj1 obj2)))
        ((vector? obj1)
         (and (vector? obj2)
              (let ((z (vector-length obj1)))
                (and (= z (vector-length obj2))
                     (let loop ((i 0))
                       (cond ((= i z) #t)
                             ((equal? (vector-ref obj1 i) (vector-ref obj2 i))
                              (loop (+ i 1)))
                             (else #f)))))))
        (else #f)))

; Messy because of inexact contagion.

(define (max first . rest)
  (max-or-min first rest #t))

(define (min first . rest)
  (max-or-min first rest #f))

(define (max-or-min first rest max?)
  (let loop ((result first) (rest rest) (lose? (inexact? first)))
    (if (null? rest)
	(if (and lose? (exact? result))
	    (exact->inexact result)
	    result)
	(let ((next (car rest)))
	  (loop (if (if max?
			(< result next)
			(> result next))
		    next
		    result)
		(cdr rest)
		(or lose? (inexact? next)))))))

(define (abs n) (if (< n 0) (- 0 n) n))

(define (zero? x) (= x 0))
(define (positive? x) (< 0 x))
(define (negative? x) (< x 0))

(define (even? n) (= 0 (remainder n 2)))
(define (odd? n) (not (even? n)))

; Lists

(define (caar   x) (car (car x)))
(define (cadr   x) (car (cdr x)))
(define (cdar   x) (cdr (car x)))
(define (cddr   x) (cdr (cdr x)))

(define (caaar  x) (caar (car x)))
(define (caadr  x) (caar (cdr x)))
(define (cadar  x) (cadr (car x)))
(define (caddr  x) (cadr (cdr x)))
(define (cdaar  x) (cdar (car x)))
(define (cdadr  x) (cdar (cdr x)))
(define (cddar  x) (cddr (car x)))
(define (cdddr  x) (cddr (cdr x)))

(define (caaaar x) (caaar (car x)))
(define (caaadr x) (caaar (cdr x)))
(define (caadar x) (caadr (car x)))
(define (caaddr x) (caadr (cdr x)))
(define (cadaar x) (cadar (car x)))
(define (cadadr x) (cadar (cdr x)))
(define (caddar x) (caddr (car x)))
(define (cadddr x) (caddr (cdr x)))
(define (cdaaar x) (cdaar (car x)))
(define (cdaadr x) (cdaar (cdr x)))
(define (cdadar x) (cdadr (car x)))
(define (cdaddr x) (cdadr (cdr x)))
(define (cddaar x) (cddar (car x)))
(define (cddadr x) (cddar (cdr x)))
(define (cdddar x) (cdddr (car x)))
(define (cddddr x) (cdddr (cdr x)))

(define (null? x) (eq? x '()))

(define (list . l) l)

;(define (length l)
;  (reduce (lambda (ignore n) (+ n 1)) 0 l))

; Bummed version.  Pretend that you didn't see this.

(define (length l)
  (real-length l 0))

(define (real-length l r)
  (if (null? l)
      r
      (real-length (cdr l) (+ r 1))))

(define (append . lists)
  (if (null? lists)
      '()
      (let recur ((lists lists))
	(if (null? (cdr lists))
	    (car lists)
	    (reduce cons (recur (cdr lists)) (car lists))))))

(define (reverse list)
  (append-reverse list '()))

(define (append-reverse list seed)
  (if (null? list)
      seed
      (append-reverse (cdr list) (cons (car list) seed))))

(define (list-tail l i)
  (cond ((= i 0) l)
	(else (list-tail (cdr l) (- i 1)))))

(define (list-ref l k)
  (car (list-tail l k)))

(define (mem pred)
  (lambda (obj l)
    (let loop ((l l))
      (cond ((null? l) #f)
	    ((pred obj (car l)) l)
	    (else (loop (cdr l)))))))

(define memq   (mem eq?))
(define memv   (mem eqv?))
(define member (mem equal?))

(define (ass pred)
  (lambda (obj l)
    (let loop ((l l))
      (cond ((null? l) #f)
            ((pred obj (caar l)) (car l))
            (else (loop (cdr l)))))))

;(define assq  (ass eq?))  ; done by VM for speed
(define assv  (ass eqv?))
(define assoc (ass equal?))

(define (list? l)			;New in R4RS
  (let recur ((l l) (lag l))		;Cycle detection
    (or (null? l)
	(and (pair? l)
	     (or (null? (cdr l))
		 (and (pair? (cdr l))
		      (not (eq? (cdr l) lag))
		      (recur (cddr l) (cdr lag))))))))

; Characters

(define (char>? x y) (char<? y x))
(define (char>=? x y) (not (char<? x y)))
(define (char<=? x y) (not (char>? x y)))


; Strings

(define (string . rest)
  (list->string rest))

(define (substring s start end)
  (let ((new-string (make-string (- end start) #\space)))
    (do ((i start (+ i 1))
         (j 0 (+ j 1)))
        ((= i end) new-string)
      (string-set! new-string j (string-ref s i)))))

(define (string-append  . strings)
  (let ((len (reduce (lambda (s n) (+ (string-length s) n)) 0 strings)))
    (let ((new-string (make-string len #\space)))
      (let loop ((s strings)
		 (i 0))
	(if (null? s)
	    new-string
	    (let* ((string (car s))
		   (l (string-length string)))
	      (do ((j 0 (+ j 1))
		   (i i (+ i 1)))
		  ((= j l) (loop (cdr s) i))
		(string-set! new-string i (string-ref string j)))))))))

(define (string->list v)
  (let ((z (string-length v)))
    (do ((i (- z 1) (- i 1))
         (l '() (cons (string-ref v i) l)))
        ((< i 0) l))))

(define (list->string l)
  (let ((v (make-string (length l) #\space)))
    (do ((i 0 (+ i 1))
         (l l (cdr l)))
        ((null? l) v)
      (string-set! v i (car l)))))

; comes from low-level package ...
;(define (string-copy s)
;  (let ((z (string-length s)))
;    (let ((copy (make-string z #\space)))
;      (let loop ((i 0))
;        (cond ((= i z) copy)
;              (else
;               (string-set! copy i (string-ref s i))
;               (loop (+ i 1))))))))

(define (string-fill! v x)
  (let ((z (string-length v)))
    (do ((i 0 (+ i 1)))
        ((= i z) (unspecific))
      (string-set! v i x))))

(define (make-string=? char=?)
  (lambda (s1 s2)
    (let ((z (string-length s1)))
      (and (= z (string-length s2))
	   (let loop ((i 0))
	     (cond ((= i z) #t)
		   ((char=? (string-ref s1 i) (string-ref s2 i))
		    (loop (+ i 1)))
		   (else #f)))))))

;(define string=?    (make-string=? char=?))  -- VM implements this
(define string-ci=?-proc (make-string=? char-ci=?))

(define (string-ci=? s1 s2)
  (string-ci=?-proc s1 s2))

(define (make-string<? char<? char=?)
  (lambda (s1 s2)
    (let ((z1 (string-length s1))
	  (z2 (string-length s2)))
      (let ((z (min z1 z2)))
	(let loop ((i 0))
	  (if (= i z)
	      (< z1 z2)
	      (let ((c1 (string-ref s1 i))
		    (c2 (string-ref s2 i)))
		(or (char<? c1 c2)
		    (and (char=? c1 c2)
			 (loop (+ i 1)))))))))))

(define string<?    (make-string<? char<? char=?))

(define string-ci<?-proc (make-string<? char-ci<? char-ci=?))

(define (string-ci<? s1 s2)
  (string-ci<?-proc s1 s2))

(define (string>? s1 s2) (string<? s2 s1))
(define (string<=? s1 s2) (not (string>? s1 s2)))
(define (string>=? s1 s2) (not (string<? s1 s2)))

(define (string-ci>? s1 s2) (string-ci<? s2 s1))
(define (string-ci<=? s1 s2) (not (string-ci>? s1 s2)))
(define (string-ci>=? s1 s2) (not (string-ci<? s1 s2)))

(define (set-string-ci-procedures! ci=? ci<?)
  (set! string-ci=?-proc ci=?)
  (set! string-ci<?-proc ci<?))

; Vectors

;(define (vector . l)   ; now an opcode for efficiency
;  (list->vector l))

(define (vector->list v)
  (do ((i (- (vector-length v) 1) (- i 1))
       (l '() (cons (vector-ref v i) l)))
      ((< i 0) l)))

(define (list->vector l)
  (let ((v (make-vector (length l) #f)))
    (do ((i 0 (+ i 1))
         (l l (cdr l)))
        ((null? l) v)
      (vector-set! v i (car l)))))

(define (vector-fill! v x)
  (let ((z (vector-length v)))
    (do ((i 0 (+ i 1)))
        ((= i z) (unspecific))
      (vector-set! v i x))))

; Control features

(define (map proc first . rest)
  (if (null? rest)
      (map1 proc first)
      (map2+ proc first rest)))

(define (map1 proc l)
  ;; (reduce (lambda (x l) (cons (proc x) l)) '() l)
  (if (null? l)
      '()
      (cons (proc (car l)) (map1 proc (cdr l)))))

(define (map2+ proc first rest)
  (if (or (null? first)
	  (any null? rest))
      '()
      (cons (apply proc (cons (car first) (map1 car rest)))
	    (map2+ proc (cdr first) (map1 cdr rest)))))

(define (for-each proc first . rest)
  (if (null? rest)
      (for-each1 proc first)
      (for-each2+ proc first rest)))

(define (for-each1 proc first)
  (let loop ((first first))
    (if (null? first)
	(unspecific)
	(begin (proc (car first))
	       (loop (cdr first))))))

(define (for-each2+ proc first rest)
  (let loop ((first first) (rest rest))
    (if (or (null? first)
	    (any null? rest))
	(unspecific)
	(begin (apply proc (cons (car first) (map car rest)))
	       (loop (cdr first) (map cdr rest))))))


; Promises, promises.

(define-syntax delay
  (syntax-rules ()
    ((delay ?exp) (make-promise (lambda () ?exp)))))

; A slightly modified copy of the code from R4RS; the modification ensures
; that the thunk is GC'ed after the promise is evaluted.
; JAR writes: "It is not for us to judge the wisdom of the new definition."

(define (make-promise thunk-then-result)
  (let ((already-run? #f))
    (lambda ()
      (if already-run?                         ; can't be interrupted from now
	  thunk-then-result
	  (let ((result (thunk-then-result)))  ; until after this call
	    (cond ((not already-run?)
		   (set! already-run? #t)
		   (set! thunk-then-result result)))
	    thunk-then-result)))))

(define (force promise)
  (promise))
