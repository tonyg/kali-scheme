; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Character maps, ASCII-only version

(define (char-whitespace? c)
  (if (memq (char->ascii c) ascii-whitespaces) #t #f))

(define (char-lower-case? c)
  (and (char>=? c #\a)
       (char<=? c #\z)))

(define (char-upper-case? c)
  (and (char>=? c #\A)
       (char<=? c #\Z)))

(define (char-numeric? c)
  (and (char>=? c #\0)
       (char<=? c #\9)))

(define (char-alphabetic? c)
  (or (char-upper-case? c)
      (char-lower-case? c)))

(define char-case-delta 
  (- (char->ascii #\a) (char->ascii #\A)))

(define (make-character-map f)
  (let ((s (make-string ascii-limit #\0)))
    (do ((i 0 (+ i 1)))
	((>= i ascii-limit))
      (string-set! s i (f (ascii->char i))))
    s))

(define upcase-map
  (make-character-map
   (lambda (c)
     (if (char-lower-case? c)
	 (ascii->char (- (char->ascii c) char-case-delta))
	 c))))

(define (char-upcase c)
  (string-ref upcase-map (char->ascii c)))

(define downcase-map
  (make-character-map
   (lambda (c)
     (if (char-upper-case? c)
	 (ascii->char (+ (char->ascii c) char-case-delta))
	 c))))

(define (char-downcase c)
  (string-ref downcase-map (char->ascii c)))

(define (char-ci-compare pred)
  (lambda (c1 c2) (pred (char-upcase c1) (char-upcase c2))))
(define char-ci=? (char-ci-compare char=?))
(define char-ci<? (char-ci-compare char<?))
(define char-ci<=? (char-ci-compare char<=?))
(define char-ci>? (char-ci-compare char>?))
(define char-ci>=? (char-ci-compare char>=?))

; Later, we replace these by the Unicode versions.  We don't want them
; in the initial image because they use a lot more memory.

(define (set-char-map-procedures! alphabetic?
				  numeric?
				  whitespace?
				  upper-case?
				  lower-case?
				  upcase
				  downcase)
  (set! char-alphabetic? alphabetic?)
  (set! char-numeric? numeric?)
  (set! char-whitespace? whitespace?)
  (set! char-upper-case? upper-case?)
  (set! char-lower-case? lower-case?)
  (set! char-upcase upcase)
  (set! char-downcase downcase))

