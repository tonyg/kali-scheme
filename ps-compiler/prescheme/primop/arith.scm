; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.


(define (put-literal-first call)
  (if (and (not (literal-node? (call-arg call 0)))
	   (literal-node? (call-arg call 1)))
      (let ((arg1 (detach (call-arg call 0)))
	    (arg0 (detach (call-arg call 1))))
	(attach call 0 arg0)
	(attach call 1 arg1))))

(define (simplify-add call)
  (simplify-args call 0)
  (put-literal-first call)
  ((pattern-simplifier
    ((+ '0 x) x)
    ((+ 'a 'b) '(+ a b))
    ((+ 'a (+ 'b x)) (+ x '(+ a b)))
    ((+ 'a (- x 'b)) (+ x '(- a b)))  ; no overflow in Scheme, but what
    ((+ 'a (- 'b x)) (- '(+ a b) x))  ; about PreScheme?  Could check the
    ((+ (+ 'a x) (+ 'b y)) (+ '(+ a b) (+ x y)))
    ((+ x (+ 'a y)) (+ 'a (+ x y))))
   call))                             ; result of the literal.  Maybe these
                                      ; should be left out. 

(define-scheme-primop + #f type/integer simplify-add)

(define (simplify-subtract call)
  (simplify-args call 0)
  ((pattern-simplifier
    ((- 'a 'b) '(- a b))
    ((- x 'a) (+ '(- 0 a) x))
    ((- 'a (+ 'b x)) (- '(- a b) x))   ; more overflow problems
    ((- 'a (- 'b x)) (+ x '(- a b)))
    ((- x (+ 'a y)) (+ '(- 0 a) (- x y)))
;    ((- (+ 'a x) y) (+ 'a (- x y)))  hmm - need to come up with a normal form
    ((- (+ 'a x) (+ 'b y)) (- (+ '(- a b) x) y)))
   call))

(define-scheme-primop - #f type/integer simplify-subtract)

; This should check for multiply by powers of 2 (other constants can be
; done later).

(define (simplify-multiply call)
  (simplify-args call 0)
  (put-literal-first call)
  (cond ((power-of-two-literal (call-arg call 0))
	 => (lambda (i)
	      (set-call-primop! call (get-prescheme-primop 'ashl))
	      (replace (call-arg call 0) (detach (call-arg call 1)))
	      (attach call 1 (make-literal-node i type/unknown))))
	(else
	 ((pattern-simplifier
	   ((* '0 x) '0)
	   ((* '1 x) x)
	   ((* 'a 'b) '(* a b))
	   ((* 'a (* x 'b)) (* x '(* a b)))
	   ((* 'a (* 'b x)) (* x '(* a b))))
	  call))))

(define-scheme-primop *      #f type/integer simplify-multiply)
(define-scheme-primop small* #f type/integer simplify-multiply)

(define (simplify-quotient call)
  (simplify-args call 0)
  (cond ;((power-of-two-literal (call-arg call 1))
	; => (lambda (i)
	;      (set-call-primop! call (get-prescheme-primop 'ashr))
	;      (replace (call-arg call 1) (make-literal-node i type/unknown))))
	(else
	 ((pattern-simplifier
	   ((quotient x '0) '((lambda ()
				(error "program divides by zero"))))
	   ((quotient x '1) x)
	   ((quotient '0 x) '0)
	   ((quotient 'a 'b) '(quotient a b)))
	  call))))

(define (power-of-two-literal node)
  (if (not (literal-node? node))
      #f
      (let ((value (literal-value node)))
	(if (not (and (integer? value)
		      (<= 1 value)))
	    #f
	    (do ((v value (arithmetic-shift v -1))
		 (i 0 (+ i 1)))
		((odd? v)
		 (if (= v 1) i #f)))))))

(define-scheme-primop quotient  exception type/integer simplify-quotient)
(define-scheme-primop remainder exception type/integer)

(define (simplify-ashl call)
  (simplify-args call 0)
  ((pattern-simplifier
    ((ashl '0 x) '0)
    ((ashl x '0) x)
    ((ashl 'a 'b) '(arithmetic-shift a b))
    ((ashl (ashl x 'a) 'b) (ashl x '(+ a b)))
    ((ashl (ashr x 'a) 'b)
     (<= a b)    ; condition
     (ashl (bitwise-and x '(bitwise-not (- (expt 2 a) 1))) '(- b a)))
    ((ashl (ashr x 'a) 'b)
     (>= a b)    ; condition
     (bitwise-and (ashr x '(- a b)) '(bitwise-not (- (expt 2 b) 1))))
    ((ashl (+ 'a x) 'b) (+ (ashl x 'b) '(arithmetic-shift a b))))
   call))

(define (simplify-ashr call)
  (simplify-args call 0)
  ((pattern-simplifier
    ((ashr '0 x) '0)
    ((ashr x '0) x)
    ((ashr 'a 'b) '(arithmetic-shift a (- b)))
    ((ashr (ashr x 'a) 'b) (ashr x '(+ a b))))
   call))

(define (simplify-lshr call)
  (simplify-args call 0)
  ((pattern-simplifier
    ((lshr '0 x) '0)
    ((lshr x '0) x)
    ((lshr 'a 'b) '(lshr a (- b)))
    ((lshr (lshr x 'a) 'b) (lshr x '(+ a b)))
    ((ashr (lshr x 'a) 'b) (lshr x '(+ a b)))) ; depends on shifts by zero
                                               ; having been constant folded
   call))

(define-scheme-primop ashl #f type/integer simplify-ashl)
(define-scheme-primop ashr #f type/integer simplify-ashr)
(define-scheme-primop lshr #f type/integer simplify-lshr)

(define (simplify-bitwise-and call)
  (simplify-args call 0)
  (put-literal-first call)
  ((pattern-simplifier
    ((bitwise-and '0 x) '0)
    ((bitwise-and '-1 x) x)
    ((bitwise-and 'a 'b) '(bitwise-and a b)))
   call))

(define (simplify-bitwise-ior call)
  (simplify-args call 0)
  (put-literal-first call)
  ((pattern-simplifier
    ((bitwise-ior '0 x) x)
    ((bitwise-ior '-1 x) '-1)
    ((bitwise-ior 'a 'b) '(bitwise-ior a b)))
   call))

(define (simplify-bitwise-xor call)
  (simplify-args call 0)
  (put-literal-first call)
  ((pattern-simplifier
    ((bitwise-xor '0 x) x)
    ((bitwise-xor 'a 'b) '(bitwise-xor a b)))
   call))

(define (simplify-bitwise-not call)
  (simplify-args call 0)
  ((pattern-simplifier
    ((bitwise-not 'a) '(bitwise-not a)))
   call))

(define-scheme-primop bitwise-and #f type/integer simplify-bitwise-and)
(define-scheme-primop bitwise-ior #f type/integer simplify-bitwise-ior)
(define-scheme-primop bitwise-xor #f type/integer simplify-bitwise-xor)
(define-scheme-primop bitwise-not #f type/integer simplify-bitwise-not)

(define (simplify-= call)
  (simplify-args call 0)
  (put-literal-first call)
  ((pattern-simplifier
    ((= 'a 'b) '(= a b))
    ((= 'a (+ 'b c)) (= '(- a b) c))   ; will these ever be used?
    ((= 'a (- 'b c)) (= '(- b a) c)))
   call))

(define (simplify-< call)
  (simplify-args call 0)
  ((pattern-simplifier
    ((< 'a 'b) '(< a b))
    ((< 'a (+ 'b c)) (< '(- a b) c))   ; will these ever be used?
    ((< (+ 'b c) 'a) (< c '(- a b)))
    ((< 'a (- 'b c)) (< c '(- b a)))
    ((< (- 'b c) 'a) (< '(- b a) c)))
   call))

(define (simplify-char=? call)
  (simplify-args call 0)
  (put-literal-first call)
  ((pattern-simplifier
    ((char=? 'a 'b) '(char=? a b))
    ((char=? 'a (+ 'b c)) (char=? '(- a b) c))
    ((char=? 'a (- 'b c)) (char=? '(- b a) c)))
   call))

(define (simplify-char<? call)
  (simplify-args call 0)
  ((pattern-simplifier
    ((char<? 'a 'b) '(char<? a b))
    ((char<? 'a (+ 'b c)) (char<? '(- a b) c))
    ((char<? (+ 'b c) 'a) (char<? c '(- a b)))
    ((char<? 'a (- 'b c)) (char<? c '(- b a)))
    ((char<? (- 'b c) 'a) (char<? '(- b a) c)))
   call))

(define bool-type
  (lambda (call)
    type/boolean))

(define-scheme-primop =      #f bool-type simplify-=)
(define-scheme-primop <      #f bool-type simplify-<)
(define-scheme-primop char=? #f bool-type simplify-char=?)
(define-scheme-primop char<? #f bool-type simplify-char<?)

(define (simplify-char->ascii call)
  (simplify-args call 0)
  (let ((arg (call-arg call 0)))
    (if (literal-node? arg)
	(let ((value (literal-value arg)))
	  (if (char? value)
	      (replace call (make-literal-node (char->ascii value) #f))
	      (breakpoint "char->ascii is applied to a non-character literal ~S"
			  value))))))
			  
(define (simplify-ascii->char call)
  (simplify-args call 0)
  (let ((arg (call-arg call 0)))
    (if (literal-node? arg)
	(let ((value (literal-value arg)))
	  (if (integer? value)
	      (replace call (make-literal-node (ascii->char value) #f))
	      (breakpoint "ascii->char is applied to a non-integer literal ~S"
			  value))))))

(define-scheme-primop char->ascii #f type/integer simplify-char->ascii)
(define-scheme-primop ascii->char #f type/integer simplify-ascii->char)

;(define (simplify-sign-extend call)
;  (simplify-args call 0)
;  (let ((value (call-arg call 0)))
;    (cond ((literal-node? value)
;           (set-literal-type! value type/integer)
;           (replace call (detach value))))))
;
;(define-scheme-primop sign-extend #f type/integer simplify-sign-extend)
;(define-scheme-primop zero-extend #f type/integer simplify-sign-extend)
