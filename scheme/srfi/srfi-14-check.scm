; SRFI 14 test suite

; ,config ,load =scheme48/debug/test.scm
; ,exec ,load =scheme48/srfi/srfi-14-check.scm
; ,exec (done)

(load-package 'testing)

(config '(run 
(define-structure srfi-14-test (export)

  (open scheme testing
	unicode
	srfi-14)

  (begin

;; adapted from Olin's test suite
(define (vowel? c)
  (member c '(#\a #\e #\i #\o #\u)))

(test "char-set?" equal?
      '(#f #t)
      (list
       (char-set? 5)
       (char-set? (char-set #\a #\e #\i #\o #\u))))

(test "char-set=" equal?
      '(#t #t #t #f)
      (list
       (char-set=)
       (char-set= (char-set))
       (char-set= (char-set #\a #\e #\i #\o #\u)
      	    (string->char-set "ioeauaiii"))
       (char-set= (char-set #\e #\i #\o #\u)
      	    (string->char-set "ioeauaiii"))))

(test "char-set<=" equal?
      '(#t #t #t #t)
      (list
       (char-set<=)
       (char-set<= (char-set))
       (char-set<= (char-set #\a #\e #\i #\o #\u)
		   (string->char-set "ioeauaiii"))
       (char-set<= (char-set #\e #\i #\o #\u)
      	     (string->char-set "ioeauaiii"))))

(test "char-set-hash" equal?
      #t
      (<= 0 (char-set-hash char-set:graphic 100) 99))

(test "char-set-fold" equal?
      4
      (char-set-fold (lambda (c i) (+ i 1)) 0
      	       (char-set #\e #\i #\o #\u #\e #\e)))


; The following test is ASCII/Latin-1 only, and fails with Unicode
; (char-set= (string->char-set "eiaou2468013579999")
;            (char-set-unfold null? car cdr '(#\a #\e #\i #\o #\u #\u #\u)
;                             char-set:digit))

(test "char-set-unfold" char-set=
      (string->char-set "eiaou246801357999")
      (char-set-unfold null? car cdr '(#\a #\e #\i #\o #\u)
      		 (string->char-set "0123456789")))

(define (not-char-set= . char-sets)
  (not (apply char-set= char-sets)))

(test "char-set-unfold!" not-char-set=
      (string->char-set "eiaou246801357")
      (char-set-unfold! null? car cdr '(#\a #\e #\i #\o #\u)
      		  (string->char-set "0123456789")))

(test "char-set-for-each" equal?
      '(#t #f)
      (list
       (let ((cs (string->char-set "0123456789")))
         (char-set-for-each (lambda (c) (set! cs (char-set-delete cs c)))
      		      (string->char-set "02468000"))
         (char-set= cs (string->char-set "97531")))

       (let ((cs (string->char-set "0123456789")))
         (char-set-for-each (lambda (c) (set! cs (char-set-delete cs c)))
      		      (string->char-set "02468"))
         (char-set= cs (string->char-set "7531")))))

(test "char-set-map" equal?
      '(#t #f)
      (list
       (char-set=
        (char-set-map char-upcase (string->char-set "aeiou"))
        (string->char-set "IOUAEEEE"))
       (char-set= (char-set-map char-upcase (string->char-set "aeiou"))
      	    (string->char-set "OUAEEEE"))))

(test "char-set-copy" char-set=
      (char-set-copy (string->char-set "aeiou"))
      (string->char-set "aeiou"))

(test "char-set" equal?
      '(#t #f)
      (list
       (char-set= (char-set #\x #\y) (string->char-set "xy"))
       (char-set= (char-set #\x #\y #\z) (string->char-set "xy"))))

(test "list->char-set" equal?
      '(#t #f #t #f)
      (list
       (char-set= (string->char-set "xy") (list->char-set '(#\x #\y)))
       (char-set= (string->char-set "axy") (list->char-set '(#\x #\y)))
       (char-set= (string->char-set "xy12345")
		  (list->char-set '(#\x #\y) (string->char-set "12345")))
       (char-set= (string->char-set "y12345")
		  (list->char-set '(#\x #\y) (string->char-set "12345")))))

(test "list->char-set!" equal?
      '(#t #f)
      (list
       (char-set= (string->char-set "xy12345")
      	    (list->char-set! '(#\x #\y) (string->char-set "12345")))
       (char-set= (string->char-set "y12345")
      	    (list->char-set! '(#\x #\y) (string->char-set "12345")))))

(test "char-set-filter" equal?
      '(#t #f)
      (list
       (char-set= (string->char-set "aeiou12345")
		  (char-set-filter vowel? char-set:ascii (string->char-set "12345")))
       (char-set= (string->char-set "aeou12345")
		  (char-set-filter vowel? char-set:ascii (string->char-set "12345")))))

(test "char-set-filter!" equal?
      '(#t #f)
      (list
       (char-set= (string->char-set "aeiou12345")
		  (char-set-filter! vowel? char-set:ascii (string->char-set "12345")))
       (char-set= (string->char-set "aeou12345")
		  (char-set-filter! vowel? char-set:ascii (string->char-set "12345")))))

(test "ucs-range->char-set" equal?
      '(#t #f)
      (list
       (char-set= (string->char-set "abcdef12345")
		  (ucs-range->char-set 97 103 #t (string->char-set "12345")))
       (char-set= (string->char-set "abcef12345")
		  (ucs-range->char-set 97 103 #t (string->char-set "12345")))))

(test "ucs-range_>char-set!" equal?
      '(#t #f)
      (list
       (char-set= (string->char-set "abcdef12345")
		  (ucs-range->char-set! 97 103 #t (string->char-set "12345")))
       (char-set= (string->char-set "abcef12345")
		  (ucs-range->char-set! 97 103 #t (string->char-set "12345")))))

(test "x->char-set" equal?
      '(#t #f)
      (list
       (char-set= (x->char-set #\x)
		  (x->char-set "x")
		  (x->char-set (char-set #\x)))
       (char-set= (x->char-set #\x)
		  (x->char-set "y")
		  (x->char-set (char-set #\x)))))

(test "char-set-size" equal?
      10
      (char-set-size (char-set-intersection char-set:ascii char-set:digit)))

(test "char-set-count" equal?
      5
      (char-set-count vowel? char-set:ascii))

(test "char-set->list" equal?
      '(#t #f)
      (list
       (equal? '(#\x) (char-set->list (char-set #\x)))
       (equal? '(#\X) (char-set->list (char-set #\x)))))

(test "char-set->string" equal?
      '(#t #f)
      (list
       (equal? "x" (char-set->string (char-set #\x)))
       (equal? "X" (char-set->string (char-set #\x)))))

(test "char-set-contains?" equal?
      '(#t #f)
      (list
       (char-set-contains? (x->char-set "xyz") #\x)
       (char-set-contains? (x->char-set "xyz") #\a)))

(test "char-set-every" equal?
      '(#t #f)
      (list
       (char-set-every char-lower-case? (x->char-set "abcd"))
       (char-set-every char-lower-case? (x->char-set "abcD"))))

(test "char-set-any" equal?
      '(#t #f)
      (list
       (char-set-any char-lower-case? (x->char-set "abcd"))
       (char-set-any char-lower-case? (x->char-set "ABCD"))))

(test "cursors" char-set=
      (x->char-set "ABCD")
      (let ((cs (x->char-set "abcd")))
	(let lp ((cur (char-set-cursor cs)) (ans '()))
	  (if (end-of-char-set? cur) (list->char-set ans)
	      (lp (char-set-cursor-next cs cur)
		  (cons (char-upcase (char-set-ref cs cur)) ans))))))

(test "char-set-adjoin" equal?
      '(#t #f)
      (list
       (char-set= (char-set-adjoin (x->char-set "123") #\x #\a)
		  (x->char-set "123xa"))
       (char-set= (char-set-adjoin (x->char-set "123") #\x #\a)
		  (x->char-set "123x"))))

(test "char-set-adjoin!" equal?
      '(#t #f)
      (list
       (char-set= (char-set-adjoin! (x->char-set "123") #\x #\a)
		  (x->char-set "123xa"))
       (char-set= (char-set-adjoin! (x->char-set "123") #\x #\a)
		  (x->char-set "123x"))))

(test "char-set-delete" equal?
      '(#t #f)
      (list
       (char-set= (char-set-delete (x->char-set "123") #\2 #\a #\2)
		  (x->char-set "13"))
       (char-set= (char-set-delete (x->char-set "123") #\2 #\a #\2)
		  (x->char-set "13a"))))

(test "char-set-delete!" equal?
      '(#t #f)
      (list
       (char-set= (char-set-delete! (x->char-set "123") #\2 #\a #\2)
		  (x->char-set "13"))
       (char-set= (char-set-delete! (x->char-set "123") #\2 #\a #\2)
		  (x->char-set "13a"))))

(test "char-set-intersection" char-set=
      (x->char-set "abcdefABCDEF")
      (char-set-intersection char-set:hex-digit (char-set-complement char-set:digit)))

(test "char-set-intersection!" char-set=
      (x->char-set "abcdefABCDEF")
      (char-set-intersection! (char-set-complement! (x->char-set "0123456789"))
				    char-set:hex-digit))

(test "char-set-union" char-set=
      (x->char-set "abcdefABCDEFghijkl0123456789")
      (char-set-union char-set:hex-digit
		      (x->char-set "abcdefghijkl")))

(test "char-set-union!" char-set=
      (x->char-set "abcdefABCDEFghijkl0123456789")
      (char-set-union! (x->char-set "abcdefghijkl")
		       char-set:hex-digit))

(test "char-set-difference" char-set=
      (x->char-set "ghijklmn")
      (char-set-difference (x->char-set "abcdefghijklmn")
			   char-set:hex-digit))

(test "char-set-difference!" char-set=
      (x->char-set "ghijklmn")
      (char-set-difference! (x->char-set "abcdefghijklmn")
			    char-set:hex-digit))

(test "char-set-xor" char-set=
      (x->char-set "abcdefABCDEF")
      (char-set-xor (x->char-set "0123456789")
		    char-set:hex-digit))

(test "char-set-xor!" char-set=
      (x->char-set "abcdefABCDEF")
      (char-set-xor! (x->char-set "0123456789")
		     char-set:hex-digit))

(test "char-set-diff+intersection" equal?
      #t
      (call-with-values (lambda ()
			  (char-set-diff+intersection char-set:hex-digit
						      char-set:letter))
	(lambda (d i)
	  (and (char-set= d (x->char-set "0123456789"))
	       (char-set= i (x->char-set "abcdefABCDEF"))))))

(test "char-set-diff+intersection!" equal?
      #t
      (call-with-values (lambda ()
			  (char-set-diff+intersection! (char-set-copy char-set:hex-digit)
						       (char-set-copy char-set:letter)))
	(lambda (d i)
	  (and (char-set= d (x->char-set "0123456789"))
	       (char-set= i (x->char-set "abcdefABCDEF"))))))

; The following stuff was adapted from the suite Matthew Flatt wrote
; for PLT Scheme

(test "char-set:lower-case" equal?
      '(#t #f #t #f #t)
      (list
       (char-set-contains? char-set:lower-case #\a)
       (char-set-contains? char-set:lower-case #\A)
       (char-set-contains? char-set:lower-case (scalar-value->char #x00E0))
       (char-set-contains? char-set:lower-case (scalar-value->char #x00C2))
       (char-set-contains? char-set:lower-case (scalar-value->char #x00B5))))

(test "char-set:upper-case" equal?
      '(#t #f #t #f)
      (list
       (char-set-contains? char-set:upper-case #\A)
       (char-set-contains? char-set:upper-case #\a)
       (char-set-contains? char-set:upper-case (scalar-value->char #x00C2))
       (char-set-contains? char-set:upper-case (scalar-value->char #x00E0))))

(test "char-set:title-case" equal?
      '(#t #t #f #f)
      (list
       (char-set-contains? char-set:title-case (scalar-value->char #x01C5))
       (char-set-contains? char-set:title-case (scalar-value->char #x1FA8))
       (char-set-contains? char-set:title-case #\a)
       (char-set-contains? char-set:title-case #\A)))

(test "char-set:letter" equal?
      '(#t #t #f #t #t)
      (list
       (char-set-contains? char-set:letter #\a)
       (char-set-contains? char-set:letter #\A)
       (char-set-contains? char-set:letter #\1)
       (char-set-contains? char-set:letter (scalar-value->char #x00AA))
       (char-set-contains? char-set:letter (scalar-value->char #x00BA))))

(test "char-set:lower-case / upper-case / letter" equal?
      '(#f #t #f #t)
      (list
       (char-set-every (lambda (c) (char-set-contains? char-set:lower-case c)) char-set:letter)
       (char-set-any (lambda (c) (char-set-contains? char-set:lower-case c)) char-set:letter)
       (char-set-every (lambda (c) (char-set-contains? char-set:upper-case c)) char-set:letter)
       (char-set-any (lambda (c) (char-set-contains? char-set:upper-case c)) char-set:letter)))

;; Not true?
;; (test #t char-set<= char-set:letter (char-set-union char-set:lower-case char-set:upper-case char-set:title-case))

(test "char-set:digit" equal?
      '(#t #f)
      (list
       (char-set-contains? char-set:digit #\1)
       (char-set-contains? char-set:digit #\a)))

(test "char-set:hex-digit" equal?
      '(#t #t #t #f)
      (list
       (char-set-contains? char-set:hex-digit #\1)
       (char-set-contains? char-set:hex-digit #\a)
       (char-set-contains? char-set:hex-digit #\A)
       (char-set-contains? char-set:hex-digit #\g)))

(test "char-set:letter+digit" equal?
      '(#t #t #t #t #t)
      (list
       (char-set-contains? char-set:letter+digit #\1)
       (char-set-contains? char-set:letter+digit #\a)
       (char-set-contains? char-set:letter+digit #\z)
       (char-set-contains? char-set:letter+digit #\A)
       (char-set-contains? char-set:letter+digit #\Z)))

(test "char-set:letter (size)" equal?
      9188
      (char-set-size char-set:letter))

(test "char-set:letter / letter+digit" char-set=
      char-set:letter+digit 
      (char-set-union char-set:letter char-set:digit))

;; Slow!:
;;  (test #t char-set-every (lambda (c) (char-set-contains? char-set:letter+digit c)) char-set:letter)
;; Slow!:
;;  (test #t char-set-every (lambda (c) (char-set-contains? char-set:letter+digit c)) char-set:digit)

(test "char-set:letter / digit / letter+digit" equal?
      '(#f #f #t)
      (list
       (char-set-every (lambda (c) (char-set-contains? char-set:letter c)) char-set:letter+digit)
       (char-set-every (lambda (c) (char-set-contains? char-set:digit c)) char-set:letter+digit)
       (char-set-any (lambda (c) (char-set-contains? char-set:letter c)) char-set:letter+digit)))

(define char-set:latin-1 (ucs-range->char-set 0 256))

(test "char-set:latin-1" char-set= 
      (char-set-intersection char-set:graphic char-set:latin-1)
      (char-set-intersection (char-set-union char-set:letter char-set:digit char-set:punctuation char-set:symbol)
			     char-set:latin-1))

(test "char-set:printing" char-set=
      char-set:printing
      (char-set-union char-set:graphic char-set:whitespace))

(test "char-set:whitespace" equal?
      '(#t #t #f)
      (list
       (char-set-contains? char-set:whitespace (scalar-value->char #x0009))
       (char-set-contains? char-set:whitespace (scalar-value->char #x000D))
       (char-set-contains? char-set:whitespace #\a)))

(test "char-set:iso-control" char-set=
     char-set:iso-control 
     (char-set-union (ucs-range->char-set #x0000 #x0020)
		     (ucs-range->char-set #x007F #x00A0)))

(test "char-set:punctuation" equal?
      '(#t #t #f)
      (list
       (char-set-contains? char-set:punctuation #\!)
       (char-set-contains? char-set:punctuation (scalar-value->char #x00A1))
       (char-set-contains? char-set:punctuation #\a)))

(test "char-set:symbol" equal?
      '(#t #t #f)
      (list
       (char-set-contains? char-set:symbol #\$)
       (char-set-contains? char-set:symbol (scalar-value->char #x00A2))
       (char-set-contains? char-set:symbol #\a)))

(test "char-set:blank" equal?
      '(#t #t #f)
      (list
       (char-set-contains?  char-set:blank #\space)
       (char-set-contains?  char-set:blank (scalar-value->char #x3000))
       (char-set-contains?  char-set:blank #\a)))

;; General procedures ----------------------------------------

(test "char-set= (2)" equal?
      '(#t #f #f #f)
      (list
       (char-set= char-set:letter char-set:letter char-set:letter)
       (char-set= char-set:letter char-set:digit)
       (char-set= char-set:letter char-set:letter char-set:digit)
       (char-set= char-set:letter char-set:digit char-set:letter)))

(test "char-set<= (2)" equal?
      '(#t #f #t #f)
      (list
       (char-set<= char-set:graphic char-set:printing)
       (char-set<= char-set:printing char-set:graphic)
       (char-set<= char-set:graphic char-set:printing char-set:full)
       (char-set<= char-set:graphic char-set:full char-set:printing)))

(test "char-set-hash (2)" equal? 
      (char-set-hash char-set:graphic)
      (char-set-hash char-set:graphic))

;; Iterating over character sets ----------------------------------------

;; The number 268 comes from "grep Nd UnicodeData.txt | wc -l"
(test "char-set-size (2)" equal?
      268
      (char-set-size char-set:digit))

(test "cursors (2)" char-set=
      char-set:digit
      (list->char-set
       (let loop ((c (char-set-cursor char-set:digit)) (l '()))
	 (if (end-of-char-set? c)
	     l
	     (loop (char-set-cursor-next char-set:digit c)
		   (cons (char-set-ref char-set:digit c)
			 l))))))

(define (add1 x) (+ 1 x))

(test "char-set-unfold (2)" equal?
      '(#t #t)
      (list
       (char-set=
	(ucs-range->char-set 10 20)
	(char-set-unfold (lambda (x) (= x 20)) scalar-value->char add1 10))
       (char-set=
	(ucs-range->char-set 10 21)
	(char-set-unfold (lambda (x) (= x 20)) scalar-value->char add1 10 (char-set (scalar-value->char #x14))))))

(test "char-set-unfold! (2)" char-set=
      (ucs-range->char-set 10 20)
      (char-set-unfold! (lambda (x) (= x 20)) scalar-value->char add1 10
			(char-set-copy char-set:empty)))

(test "char-set-for-each (2)" char-set=
      char-set:digit
      (let ((cs char-set:empty))
	(char-set-for-each 
	 (lambda (c) 
	   (set! cs (char-set-adjoin cs c)))
	 char-set:digit)
	cs))

(test "char-set-map (2)" equal?
      '(#t #t #t)
      (list
       (char-set= char-set:digit
		  (char-set-map
		   (lambda (c) c)
		   char-set:digit))
       (char-set= char-set:digit
		  (char-set-map
		   (lambda (c) c)
		   char-set:digit))
       (char-set= (char-set-adjoin char-set:digit #\A)
		  (char-set-union
		   (char-set-map
		    (lambda (c) c)
		    char-set:digit)
		   (char-set #\A)))))

;; Creating character sets ----------------------------------------

(test "char-set-copy (2)" char-set=
      char-set:digit
      (char-set-copy char-set:digit))

(let ((abc (char-set #\a #\b #\c)))
  (test "char-set (2)" char-set=
	abc
	(char-set #\c #\a #\b))
  (test "string->char-set (2)" equal?
	'(#t #t #t)
	(list
	 (char-set= abc (string->char-set "cba"))
	 (char-set= abc (string->char-set! "cba" (char-set-copy char-set:empty)))
	 (char-set= abc (string->char-set "cb" (char-set #\a)))))
  (test "char-set-filter (2)" equal?
	'(#t #t #t)
	(list
	 (char-set= (char-set #\b) (char-set-filter (lambda (c) (char=? c #\b)) abc))
	 (char-set= (char-set #\b) (char-set-filter (lambda (c) (char=? c #\b)) abc char-set:empty))
	 (char-set= (char-set #\b) (char-set-filter! (lambda (c) (char=? c #\b)) (char-set-copy abc) (char-set-copy char-set:empty)))))

  (test "x->char-set (2)" equal?
	'(#t #t #t)
	(list
	 (char-set= abc (x->char-set "abc"))
	 (char-set= abc (x->char-set abc))
	 (char-set= (char-set #\a) (x->char-set #\a)))))

(test "ucs-range->char (2)" equal?
      '(#t #t #t #t #t #t #t)
      (list
      (char-set= 
       (ucs-range->char-set 0 #x20000)
       (char-set-union (ucs-range->char-set 0 #xD800)
		       (ucs-range->char-set #xE000 #x20000)))
      (char-set=
       (ucs-range->char-set 0 #xD801)
       (ucs-range->char-set 0 #xD800))
      (char-set= 
       (ucs-range->char-set 0 #xDFFF)
       (ucs-range->char-set 0 #xD800))
      (char-set= 
       (ucs-range->char-set #xD800 #xD810)
       char-set:empty)
      (char-set= 
       (ucs-range->char-set #xD810 #xE000)
       char-set:empty)
      (char-set= 
       (ucs-range->char-set #xE000 #xE001)
       (ucs-range->char-set #xD810 #xE001))
      (char-set= 
       (ucs-range->char-set #xD7FF #xE001)
       (char-set (scalar-value->char #xD7FF) (scalar-value->char #xE000)))))

;; Querying character sets ------------------------------

(test "char-set-count (2)" equal?
      3
      (char-set-count (lambda (x)
			(and (char<=? #\0 x)
			     (char<=? x #\2)))
		      char-set:digit))

(test "char-set->list / list->char-set (2)" equal?
      '(#t #t #t #t)
      (list
       (char-set= char-set:digit (list->char-set (char-set->list char-set:digit)))
       (char-set= char-set:digit (list->char-set (char-set->list char-set:digit) char-set:empty))
       (char-set= char-set:digit (list->char-set! (char-set->list char-set:digit) (char-set-copy char-set:empty)))
       (char-set= char-set:digit (string->char-set (char-set->string char-set:digit)))))

))))

(if (in 'testing '(run (lost?)))
    (display "Some tests failed.")
    (display "All tests succeeded."))
(newline)

(define (done)
  (exit (if (in 'testing '(run (lost?))) 1 0)))

