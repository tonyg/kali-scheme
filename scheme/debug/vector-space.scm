; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.



; ,open architecture primitives low-level locations debug-data syntactic

; July 5th
;total number of 3-vectors: 10896
;probably table entries: 10381
;symbol keys: 7363
;integer keys: 3018
;symbol values: 3793
;location values: 2062
;pair values: 1723
;operator values: 989
;debug-data values: 1208
;transform values: 510
;            pair   4039   48468
;          symbol   1067    8536
;          vector   4477  124132
;         closure   1541   18492
;        location    807    9684
;            port      2      40
;           ratio      0       0
;          record    579   16732
;    continuation      6     136
; extended-number      0       0
;        template    985   23916
;    weak-pointer     33     264
;        external      0       0
;unused-d-header1      0       0
;unused-d-header2      0       0
;          string   1207   19338
;     code-vector    986   51097
;          double      0       0
;          bignum      0       0
;           total  15729  320835


(define (analyze-3-vectors)
  (collect)
  (let ((vs (find-all (enum stob vector)))
	(total 0)
	(table-entries 0)
	(symbol-keys 0)
	(int-keys 0)
	(symbols 0)
	(locations 0)
	(debug-datas 0)
	(pairs 0)
	(operators 0))
    (set! *foo* '())
    (vector-for-each
     (lambda (v)
       (if (= (vector-length v) 3)
	   (let ((x (vector-ref v 2)))
	     (set! total (+ total 1))
	     (cond ((or (vector? x) (eq? x #f))
		    (set! table-entries (+ table-entries 1))
		    (let ((key (vector-ref v 0)))
		      (cond ((symbol? key)
			     (set! symbol-keys (+ symbol-keys 1)))
			    ((integer? key)
			     (set! int-keys (+ int-keys 1)))))
		    (let ((val (vector-ref v 1)))
		      (cond ((symbol? val)
			     (set! symbols (+ symbols 1)))
			    ((location? val)
			     (set! locations (+ locations 1)))
			    ((pair? val)
			     (set! pairs (+ pairs 1)))
			    ((transform? val)
			     (set! operators (+ operators 1)))
			    ((debug-data? val)
			     (set! debug-datas (+ debug-datas 1)))
			    (else (set! *foo* (cons v *foo*))))))))))
     vs)
    (display "total number of 3-vectors: ") (write total) (newline)
    (display "probably table entries: ") (write table-entries) (newline)
    (display "symbol keys: ") (write symbol-keys) (newline)
    (display "integer keys: ") (write int-keys) (newline)
    (display "symbol values: ") (write symbols) (newline)
    (display "location values: ") (write locations) (newline)
    (display "pair values: ") (write pairs) (newline)
    (display "transform values: ") (write operators) (newline)
    (display "debug-data values: ") (write debug-datas) (newline)))

(define *foo* '())

(define (bar)
  (collect)
  (vector-size-histogram (find-all (enum stob vector))))

(define (vector-size-histogram vs)
  (write (vector-length vs)) (display " vectors") (newline)
  (let ((n 0))
    (vector-for-each (lambda (v)
		       (if (eq? v vs) 'foo
			   (if (> (vector-length v) n)
			       (set! n (vector-length v)))))
		     vs)
    (display "longest: ") (write n) (newline)
    (let ((hist (make-vector (+ n 1) 0)))
      (vector-for-each (lambda (v)
			 (let ((l (vector-length v)))
			   (vector-set! hist l (+ (vector-ref hist l) 1))))
		       vs)
      (let loop ((i 0))
	(if (< i n)
	    (let ((m (vector-ref hist i)))
	      (if (> m 0)
		  (begin (write-padded i 6)
			 (write-padded m 7)
			 (write-padded (* (+ (* i m) 1) 4) 7)
			 (newline)))
	      (loop (+ i 1))))))))

(define (write-padded x pad)
  (let ((s (if (symbol? x)
	       (symbol->string x)
	       (number->string x))))
    (do ((i (- pad (string-length s)) (- i 1)))
	((<= i 0) (display s))
      (write-char #\space))))


(define (vector-for-each proc v)
  (let ((z (vector-length v)))
    (do ((i (- z 1) (- i 1)))
	((< i 0) #f)
      (if (not (vector-unassigned? v i))
	  (proc (vector-ref v i))))))
