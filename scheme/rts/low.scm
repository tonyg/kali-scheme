; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


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
  (let ((loc (make-location id #f)))
    (set-location-defined?! loc #f)
    loc))

(define (location-assigned? loc)
  (and (location-defined? loc)
       (if (eq? (contents loc)
		(unassigned))
	   #f
	   #t)))

; Used by the inspector.

(define (vector-unassigned? v i)
  (eq? (vector-ref v i) (unassigned)))


; STRING-COPY is here because it's needed by STRING->SYMBOL.

(define (string-copy s)
  (let ((z (string-length s)))
    (let ((copy (make-string z #\space)))
      (copy-bytes! s 0 copy 0 z)
      copy)))

; The symbol table

(define (string->symbol string)
  (intern (if (immutable? string)
	      string			;+++
	      (make-immutable! (string-copy string)))))

; The following magic bitmasks are derived from PORT-STATUS-OPTIONS in arch.scm.

(define (input-port? port)
  (and (port? port)
       (= 1 (bitwise-and 1 (port-status port)))))

(define (output-port? port)
  (and (port? port)
       (= 2 (bitwise-and 2 (port-status port)))))


; code-vectors == byte-vectors
; These are functions so that they will be inlined.

(define (make-code-vector length init) (make-byte-vector length init))
(define (code-vector? x) (byte-vector? x))
(define (code-vector-length bv) (byte-vector-length bv))
(define (code-vector-ref bv i) (byte-vector-ref bv i))
(define (code-vector-set! bv i x) (byte-vector-set! bv i x))

; Block reads and writes in terms of partial reads and writes.

; CHANNEL-READ returns the number of characters read or the EOF object.
; BUFFER is either a string or byte vector and START is the index at which
; to place the first character read.  NEEDED is one of
;  <integer> : the call returns when this many characters has been read or
;     an EOF is reached.
;  'IMMEDIATE : the call reads as many characters as are available and
;     returns immediately.
;  'ANY : the call returns as soon as at least one character has been read
;     or an EOF is reached.

(define (channel-read buffer start needed channel)
  (call-with-values
   (lambda ()
     (cond ((eq? needed 'immediate)
	    (values #f 0 (- (buffer-length buffer) start)))
	   ((eq? needed 'any)
	    (values #t 1 (- (buffer-length buffer) start)))
	   (else
	    (values #t needed needed))))
   (lambda (keep-trying? need max-chars)
     (let loop ((have 0))
       (let ((got (channel-maybe-read buffer
				      (+ start have)
				      (- max-chars have)
				      keep-trying?
				      channel)))
	 (if (eof-object? got)
	     (if (= have 0)
		 (eof-object)
		 have)
	     (let ((have (+ have got)))
	       (if (and keep-trying? (< have need))
		   (loop have)
		   have))))))))

(define (buffer-length buffer)	   
  (if (byte-vector? buffer)
      (byte-vector-length buffer)
      (string-length buffer)))

; Write COUNT characters from BUFFER, which is either a string or a byte-vector,
; to CHANNEL, beginning with the character at START.  No meaningful value is
; returned.

(define (channel-write buffer start count channel)
  (let loop ((sent 0))
    (if (< sent count)
	(loop (+ sent
		 (channel-maybe-write buffer
				      (+ start sent)
				      (- count sent)
				      channel))))))

; Shared bindings - six procedures from two primitives.  The lookup and
; undefine primitives take a flag which is true for imports and false for
; exports.

(define (lookup-imported-binding name)
  (lookup-shared-binding name #t))

(define (lookup-exported-binding name)
  (lookup-shared-binding name #f))

(define (define-imported-binding name value)
  (shared-binding-set! (lookup-shared-binding name #t)
		       value))
  
(define (define-exported-binding name value)
  (shared-binding-set! (lookup-shared-binding name #f)
		       value))

(define (undefine-imported-binding name)
  (undefine-shared-binding name #t))

(define (undefine-exported-binding name)
  (undefine-shared-binding name #f))

; Writing debugging messages.

(define (debug-message . stuff)
  (message stuff))
