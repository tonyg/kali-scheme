
; Stuff in Pre-Scheme that is not in Scheme.

(define (ps-read-char port char-cont eof-cont)
  (let ((next (read-char port)))
    (if (eof-object? next)
	(eof-cont)
	(char-cont next))))

(define ashl arithmetic-shift)

(define (ashr i n)
  (arithmetic-shift i (- 0 n)))


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