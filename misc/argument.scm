; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; For dealing with -a command line arguments.



(define (parse-resume-argument string)
  (define limit (string-length string))
  (define (find pred i)
    (cond ((>= i limit) #f)
          ((pred (string-ref string i)) i)
          (else (find pred (+ i 1)))))
  (define (not-char-whitespace? c)
    (not (char-whitespace? c)))
  (if (= limit 0)
      '()
      (let recur ((i 0))
        (let ((i (find not-char-whitespace? i)))
          (if i
              (let ((j (find char-whitespace? i)))
                (if j
                    (cons (substring string i j)
                          (recur j))
                    (list (substring string i limit))))
              '())))))

(define (find-argument all-args string)
  (let loop ((args all-args) (left '()))
    (cond ((null? args)
	   (values #f all-args))
	  ((and (car args)
		(string=? (car args) string))
	   (values #t (append (reverse left) (list #f) (cdr args))))
	  (else
	   (loop (cdr args) (cons (car args) left))))))
  
(define (find-argument-with-value all-args string)
  (let loop ((args all-args) (left '()))
    (cond ((null? args)
	   (values #f all-args))
	  ((and (car args)
		(string=? (car args) string))
	   (if (null? (cdr args))
	       (values #f (reverse left))
	       (values (cadr args)
		       (append (reverse left) (list #f) (cddr args)))))
	  (else
	   (loop (cdr args) (cons (car args) left))))))
