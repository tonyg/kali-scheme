
; Stuff in Pre-Scheme that is not in Scheme.

(define (ps-read-char port char-cont eof-cont)
  (let ((next (read-char port)))
    (if (eof-object? next)
	(eof-cont)
	(char-cont next))))

(define ashl arithmetic-shift)

(define (ashr i n)
  (arithmetic-shift i (- 0 n)))

(define small* *) ; could do a range check

; What we will get in C.
(define pre-scheme-integer-size 32)

(define int-mask (- (arithmetic-shift 1 pre-scheme-integer-size) 1))

(define (lshr i n)
  (if (>= i 0)
      (arithmetic-shift i (- 0 n))
      (arithmetic-shift (bitwise-and i int-mask) (- 0 n))))

(define (deallocate x) #f)

(define unassigned unspecific)

(define (write-string str port)
  (do ((i 0 (+ i 1)))
      ((>= i (string-length str)))
    (write-char (string-ref str i) port)))

(define-syntax goto
  (lambda (exp rename compare)
    (cdr exp)))

(define (null-port? x)
  #f) ; no null-ports in Scheme

(define-syntax external
  (lambda (exp rename compare)
    (error "(EXTERNAL ...) can only be compiled" exp)))