; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; This is file t-features.scm.
; Synchronize any changes with all the other *-features.scm files.

; This hasn't been tested in a long time.


(define (get-from-t name)
  (*value t-implementation-env name))

; (define error (get-from-t 'error))   - already present
; (define warn (get-from-t 'warn))   - already present?

(define (interaction-environment)
  scheme-user-env)			;Foo
(define scheme-report-environment
  (let ((env (interaction-environment))) ;Isn't there a scheme-env?
    (lambda (n) env)))

(define (ignore-errors thunk)
  '(error "ignore-errors isn't implemented"))

(define force-output (get-from-t 'force-output))

(define char->ascii char->integer)
(define ascii->char integer->char)

(define (string-hash s)
  (let ((n (string-length s)))
    (do ((i 0 (+ i 1))
         (h 0 (+ h (char->ascii (string-ref s i)))))
        ((>= i n) h))))

;==============================================================================
; Bitwise logical operations on integers

; T's ASH doesn't work on negative numbers

(define arithmetic-shift
  (let ((fx-ashl (get-from-t 'fx-ashl))
	(fx-ashr (get-from-t 'fx-ashr)))
    (lambda (integer count)
      (if (>= count 0)
	  (fx-ashl integer count)
	  (fx-ashr integer (- 0 count))))))

; This is from Olin Shivers:
;   (define (correct-ash n m)
;     (cond ((or (= m 0) (= n 0)) n)
;           ((> n 0) (ash n m))
;           ;; shifting a negative number.
;           ((> m 0)                     ; left shift
;            (- (ash (- n) m)))
;           (else                                ; right shift
;            (lognot (ash (lognot n) m)))))

(define bitwise-and (get-from-t 'fx-and))
(define bitwise-ior (get-from-t 'fx-ior))

;==============================================================================
; Code vectors

(define make-bytev         (get-from-t 'make-bytev))
(define code-vector?       (get-from-t 'bytev?))
(define code-vector-length (get-from-t 'bytev-length))
(define code-vector-ref    (get-from-t 'bref-8))
(define code-vector-set! ((get-from-t 'setter) code-vector-ref))

(define (make-code-vector size . init)
  (let ((vec (make-bytev size)))
    (if (not (null? init))
	(code-vector-fill! vec (car init)))
    vec))

(define (code-vector-fill! cv x)
  (do ((i 0 (+ i 1)))
      ((>= i (code-vector-length cv)))
    (code-vector-set! cv i x)))

;==============================================================================
; Bug fixes and modernizations
; I think syntax-rules will be needed, as well.

; Simulate a modernized DEFINE-SYNTAX.

(#[syntax define-syntax] (define-syntax name xformer)
  `(#[syntax define-syntax] (,name . %tail%)
       (,xformer (cons ',name %tail%)
		 (lambda (x) x)		;rename
		 eq?)))			;compare

; T's MAKE-VECTOR and MAKE-STRING ignore their init argument.

(define make-vector
  (let ((broken-make-vector (get-from-t 'make-vector)))
    (lambda (size . init)
      (let ((vec (broken-make-vector size)))
	(if (not (null? init))
	    (vector-fill! vec (car init)))
	vec))))

(define make-string
  (let ((make-string (get-from-t 'make-string))
	(string-fill (get-from-t 'string-fill)))
    (lambda (size . init-option)
      (if (null? init-option) 
	  (make-string size)
	  (string-fill (make-string size) (car init-option))))))

; Dynamic-wind.

(define (dynamic-wind before during after)
  (before)
  (let ((result (during)))
    (after)
    result))
