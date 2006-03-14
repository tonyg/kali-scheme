; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Character maps, ASCII-only version

; Enable us to change the semantics afterwards (see the bottom of this file)
(define (char-whitespace? c)
  (char-whitespace?-proc c))
(define (char-whitespace?-proc c)
  (if (memq (char->ascii c) ascii-whitespaces) #t #f))

(define (char-lower-case? c)
  (char-lower-case?-proc c))
(define (char-lower-case?-proc c)
  (and (char>=? c #\a)
       (char<=? c #\z)))


(define (char-upper-case? c)
  (char-upper-case?-proc c))
(define (char-upper-case?-proc c)
  (and (char>=? c #\A)
       (char<=? c #\Z)))

(define (char-numeric? c)
  (char-numeric?-proc c))
(define (char-numeric?-proc c)
  (and (char>=? c #\0)
       (char<=? c #\9)))

(define (char-alphabetic? c)
  (char-alphabetic?-proc c))
(define (char-alphabetic?-proc c)
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
  (char-upcase-proc c))
(define (char-upcase-proc c)
  (string-ref upcase-map (char->ascii c)))

(define downcase-map
  (make-character-map
   (lambda (c)
     (if (char-upper-case? c)
	 (ascii->char (+ (char->ascii c) char-case-delta))
	 c))))

(define (char-downcase c)
  (char-downcase-proc c))
(define (char-downcase-proc c)
  (string-ref downcase-map (char->ascii c)))

; helper for defining the -ci procedures
; This is relevant for Unicode, where FOLDCASE != DOWNCASE
(define (char-foldcase c)
  (char-foldcase-proc c))
(define char-foldcase-proc char-downcase-proc)

(define (char-ci-compare pred)
  (lambda (c1 c2) (pred (char-foldcase c1) (char-foldcase c2))))
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
				  downcase
				  foldcase)
  (set! char-alphabetic?-proc alphabetic?)
  (set! char-numeric?-proc numeric?)
  (set! char-whitespace?-proc whitespace?)
  (set! char-upper-case?-proc upper-case?)
  (set! char-lower-case?-proc lower-case?)
  (set! char-upcase-proc upcase)
  (set! char-downcase-proc downcase)
  (set! char-foldcase-proc foldcase))

