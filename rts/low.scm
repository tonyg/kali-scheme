; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Low-level things that rely on the fact that we're running under the
; Scheme 48 VM.

; Needs LET macro.


; Characters are not represented in ASCII.  Using a different encoding
; helps to catch portability problems.

(define (char->integer c) (+ (char->ascii c) 1000))
(define (integer->char n) (ascii->char (- n 1000)))

(define ascii-limit 256)		;for reader
(define ascii-whitespaces '(32 10 9 12 13)) ;space linefeed tab page return


; Procedures and closures are two different abstractions.  Procedures
; are created by LAMBDA and invoked with procedure call; those are
; their only defined operations.  Closures are made with MAKE-CLOSURE,
; accessed using CLOSURE-TEMPLATE and CLOSURE-ENV, and invoked by
; INVOKE-CLOSURE, which starts the virtual machine going.

; In a running Scheme 48 system, the two happen to be implemented
; using the same data type.  The following is the only part of the
; system that should know this fact.

(define procedure? closure?)

(define (invoke-closure closure . args)
  (apply (loophole :procedure closure)
	 args))


; Similarly, there are escapes and there are VM continuations.
; Escapes are obtained with PRIMITIVE-CWCC and invoked with
; WITH-CONTINUATION.  VM continuations are obtained with
; PRIMITIVE-CATCH and inspected using CONTINUATION-REF and friends.
; (This is not such a hot naming strategy; it would perhaps be better
; to use the terms "continuation" and "frame".)

; In a running Scheme 48 system, the two happen to be implemented
; using the same data type.  The following is the only part of the
; system that should know this fact.

(define (primitive-cwcc p)
  (primitive-catch (lambda (cont)
		     (p (loophole :escape cont))))) ;?

; (define (invoke-continuation cont thunk)
;   (with-continuation (loophole :escape cont) thunk))


; These two procedures are part of the location abstraction.

(define (make-undefined-location id)
  (let ((loc (make-location #f id)))
    (set-location-defined?! loc #f)
    loc))

(define (vector-unassigned? v i)
  (eq? (vector-ref v i) (unassigned)))


; STRING-COPY is here because it's needed by STRING->SYMBOL.

(define (string-copy s)
  (let ((z (string-length s)))
    (let ((copy (make-string z #\space)))
      (let loop ((i 0))
	(cond ((= i z) copy)
	      (else
	       (string-set! copy i (string-ref s i))
	       (loop (+ i 1))))))))


; The symbol table

(define (string->symbol string)
  (really-string->symbol (if (immutable? string)
			     string	;+++
			     (string-copy string))))

(define (really-string->symbol string)
  (if (eq? *the-symbol-table* #f)
      (restore-the-symbol-table!))
  (make-immutable! string)
  (let ((sym (intern string *the-symbol-table*)))
    (make-immutable! sym)
    sym))

(define *the-symbol-table* #f)

(define (flush-the-symbol-table!)
  (set! *the-symbol-table* #f))

(define (restore-the-symbol-table!)
  (set! *the-symbol-table* (make-vector 1024 '()))
  (find-all-symbols *the-symbol-table*))

(restore-the-symbol-table!)


; I/O

(define (maybe-open-input-file string)
  (open-port string 1))			;(define for-input 1)

(define (maybe-open-output-file string)
  (open-port string 2))			;(define for-output 2)

(define (open-input-file string)
  (or (maybe-open-input-file string)
      (error "can't open for input" string)))

(define (open-output-file string)
  (or (maybe-open-output-file string)
      (error "can't open for output" string)))

(define close-input-port  close-port)
(define close-output-port close-port)
