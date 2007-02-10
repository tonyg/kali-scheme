(define-test-suite regexp-tests)

(define-test-case any-match? regexp-tests
  (check (any-match? (text "abc") "abc"))
  (check (not (any-match? (text "abc") "abx")))
  (check (any-match? (text "abc") "xxabcxx")))
       
(define-test-case exact-match regexp-tests
  (check (exact-match? (text "abc") "abc"))
  (check (not (exact-match? (text "abc") "abx")))
  (check (not (exact-match? (text "abc") "xxabcxx"))))

(define (pair-match exp string)
  (let ((res (match exp string))) 
    (and res
	 (cons (list (match-start res)
		     (match-end res))
	       (map (lambda (p)
		      (cons (car p)
			    (list (match-start (cdr p))
				  (match-end (cdr p)))))
		    (match-submatches res))))))

(define-test-case match regexp-tests
  (check (pair-match (text "abc") "abc")
	 => '((0 3)))
  (check (not (pair-match (text "abc") "abx")))
  (check (pair-match (text "abc") "xxabcxx")
	 => '((2 5)))
  (check (pair-match (sequence (text "ab")
			       (submatch 'foo (text "cd"))
			       (text "ef"))
		     "xxxabcdefxx")
	 => '((3 9) (foo 5 7)))
  (check (pair-match (sequence (set "a")
			       (one-of (submatch 'foo (text "bc"))
				       (submatch 'bar (text "BC"))))
		     "xxxaBCd")
	 => '((3 6) (bar 4 6))))
