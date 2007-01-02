; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Get NormalizationTest.txt from http://www.unicode.org/

; ,config ,load =scheme48/debug/test.scm
; ,exec (define normalization-tests-filename ".../NormalizationTest.txt")
; ,exec ,load =scheme48/debug/check-normalization.scm
; ,exec (done)

(load-package 'testing)

(config '(run 
(define-structure check-normalizations (export check-all)
  (open scheme testing
	(subset srfi-13 (string-skip))
	(subset srfi-14 (char-set:hex-digit))
	unicode
	unicode-normalizations)
  (begin

(define (read-line port)
  (let loop ((l '()))
    (let ((c (read-char port)))
      (if (eof-object? c)
          c
          (if (char=? c #\newline)
              (list->string (reverse l))
              (loop (cons c l)))))))

(define (parse-scalar-values s)
  (let ((size (string-length s)))
    (let column-loop ((start 0) (count 0) (rev-columns '()))
      (if (= count 5)
	  (apply values (reverse rev-columns))
	  (let sv-loop ((start start) (rev-svs '()))
	    (let* ((i (string-skip s char-set:hex-digit start))
		   (n (string->number (substring s start i) 16)))
	      (if (char=? #\space (string-ref s i))
		  (sv-loop (+ 1 i) (cons n rev-svs))
		  (column-loop (+ 1 i) (+ 1 count)
			       (cons (list->string (map scalar-value->char (reverse (cons n rev-svs))))
				     rev-columns)))))))))

(define (check-line s)
  (call-with-values
      (lambda ()
	(parse-scalar-values s))
    (lambda (c1 c2 c3 c4 c5)
      (test s equal? #t #t)
      (check-one c1 c2 c3 c4 c5))))

(define (check-one c1 c2 c3 c4 c5)
  (test "c2 == NFC(c1)" string=? c2 (string-normalize-nfc c1))
  (test "c2 == NFC(c2)" string=? c2 (string-normalize-nfc c2))
  (test "c2 == NFC(c3)" string=? c2 (string-normalize-nfc c3))
  (test "c4 == NFC(c4)" string=? c4 (string-normalize-nfc c4))
  (test "c4 == NFC(c5)" string=? c4 (string-normalize-nfc c5))
  
  (test "c3 == NFD(c1)" string=? c3 (string-normalize-nfd c1))
  (test "c3 == NFD(c2)" string=? c3 (string-normalize-nfd c2))
  (test "c3 == NFD(c3)" string=? c3 (string-normalize-nfd c3))
  (test "c5 == NFD(c4)" string=? c5 (string-normalize-nfd c4))
  (test "c5 == NFD(c5)" string=? c5 (string-normalize-nfd c5))

  (test "c4 == NFKC(c1)" string=? c4 (string-normalize-nfkc c1))
  (test "c4 == NFKC(c2)" string=? c4 (string-normalize-nfkc c2))
  (test "c4 == NFKC(c3)" string=? c4 (string-normalize-nfkc c3))
  (test "c4 == NFKC(c4)" string=? c4 (string-normalize-nfkc c4))
  (test "c4 == NFKC(c5)" string=? c4 (string-normalize-nfkc c5))

  (test "c5 == NFKD(c1)" string=? c5 (string-normalize-nfkd c1))
  (test "c5 == NFKD(c2)" string=? c5 (string-normalize-nfkd c2))
  (test "c5 == NFKD(c3)" string=? c5 (string-normalize-nfkd c3))
  (test "c5 == NFKD(c4)" string=? c5 (string-normalize-nfkd c4))
  (test "c5 == NFKD(c5)" string=? c5 (string-normalize-nfkd c5)))

(define (check-all filename)
  (call-with-input-file filename
    (lambda (port)
      (let loop ()
	(let ((thing (read-line port)))
	  (if (string? thing)
	      (begin
		(if (and (not (string=? "" thing))
			 (not (char=? (string-ref thing 0) #\#))
			 (not (char=? (string-ref thing 0) #\@)))
		    (check-line thing))
		(loop))))))))
	      
))
))

(open 'check-normalizations)
(check-all normalization-tests-filename)

(if (in 'testing '(run (lost?)))
    (display "Some tests failed.")
    (display "All tests succeeded."))
(newline)

(define (done)
  (exit (if (in 'testing '(run (lost?))) 1 0)))
