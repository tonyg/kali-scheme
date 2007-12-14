; ,config ,load scheme/misc/packages.scm
; ,open i/o primitives kali address-spaces proxy-count-requests messages threads signals srfi-42 srfi-66 srfi-78 srfi-9 reset/shift
;(spawn-server)
;(define local-aspace (local-address-space))
;(load "scheme/kali/tests/encode-decode.scm")

(define (endecode-id val aspace)
  (call-with-values
      (lambda ()
	(encode (cons val '())
		aspace))
    (lambda (bytes need-counts)
      (if (not (null? need-counts))
	  (error "endecode-id: Need proxy counts! That can't be true")
	  (call-with-values
	      (lambda ()
		(decode (u8cdr bytes) aspace #f)) ;; no reverse! 
	    (lambda (val missing-uids bad-count-proxies)
	      (let loop ((treated-bad-count-proxies #f))
		(cond ((and (not treated-bad-count-proxies)
			    (not (null? bad-count-proxies)))
		       (display "going to return counts" (current-error-port))
		       (newline (current-error-port))		       
		       (for-each return-counts (adjust-proxy-counts! bad-count-proxies))
		       (loop #t))
		      ((not (null? missing-uids))
		       (display missing-uids)
		       (display "endecode-id: missing uids! That can't be true")
		       (newline))
		      (else (car val))))))))))

(define (u8cdr bv)
  (list->u8vector 
   (cddddr 
    (u8vector->list bv))))


;;-------------------------------------------------------------------------

(define (make-list n)
  (let lp ((m 0))
    (if (> m n)
	'()
	(cons m 
	      (lp (+ m 1))))))

(define (make-string n)
  (if (zero? n)
      ""
      (string-append "*"
		     (make-string (- n 1)))))

(define (make-symbol n)
  (string->symbol (string-append "number-"
				 (number->string n))))

(define (make-symbol-list n)
  (map make-symbol
       (make-list n)))

;;-------------------------------------------------------------------------

(define-record-type :rec
  (make-rec a b)
  rec?
  (a rec-a)
  (b rec-b))

(define (rec-equal? r1 r2)
  (and (rec? r1)
       (rec? r2)
       (let ((a1 (rec-a r1))
	     (a2 (rec-a r2))
	     (b1 (rec-b r1))
	     (b2 (rec-b r2)))
	 ((if (rec? a1)
	      rec-equal?
	      equal?) 
	  a1 a2)
	 ((if (rec? b1)
	      rec-equal?
	      equal?) 
	  b1 b2))))

(define rec-1 (make-rec 'symbol "string"))
(define rec-2 (make-rec + rec-1))

;;-------------------------------------------------------------------------

(define (fak n) 
  (if (zero? n)
      1
      (* n (fak (- n 1)))))

(define fak-c)

(reset (fak (shift k (begin (set! fak-c k) 6))))

;;-------------------------------------------------------------------------

(define (make-adder a)
  (lambda (b)
    (+ a b)))

(define add3 (make-adder 3))
(define add19 (make-adder 19))

;;-------------------------------------------------------------------------

(define (encode-decode-test)
  (if (not (server-running?))
      (begin 
	(display "starting server now...")
	(newline)
	(spawn-server)))
  (let ((local-aspace (local-address-space)))

    (report)

    (check (endecode-id #t local-aspace) => #t)
    (report)

    (check (endecode-id #f local-aspace) => #f)
    (report)

    (check (endecode-id '() local-aspace) => '())

    (report)

    (check-ec (:range i 0 100) 
	      ((lambda (x) 
		 (endecode-id x local-aspace)) i)
	      => i)

    (report)

    (check-ec (:range i 
		      100000000000000000000000000000000000000 
		      100000000000000000000000000000000001000) 
	      ((lambda (x) 
		 (endecode-id x local-aspace)) i)
	      => i)

    (report)

    (check-ec (:real-range i 0.000000001 10.0 0.01) 
	      ((lambda (x) 
		 (endecode-id x local-aspace)) i)
	      => i)

    (report)

    (check-ec (:real-range i 
			   10000000.01 
			   10000010.01 
			   0.01) 
	      ((lambda (x) 
		 (endecode-id x local-aspace)) i)
	      => i)

    (report)

    (check-ec (:real-range i 
			   100000000000000000000000000000000000.01 
			   100000000000000000000000000000010000.01 
			   0.01) 
	      ((lambda (x) 
		 (endecode-id x local-aspace)) i)
	      => i)

    (report)

    (check-ec (:range i 100 120) 
	      ((lambda (x) 
		 (endecode-id (/ 3 x) local-aspace)) i)
	      => (/ 3 i))

    (report)

    (check-ec (:range i 100 120) 
	      ((lambda (x) 
		 (endecode-id (/ x 3) local-aspace)) i)
	      => (/ i 3))

    (report)

    (check-ec (:range i 100 120) 
	      ((lambda (x) 
		 (endecode-id (/ 1999999999999999999999 x) local-aspace)) i)
	      => (/ 1999999999999999999999 i))

    (report)

    (check-ec (:range i 100 120) 
	      ((lambda (x) 
		 (endecode-id (/ x 19999999999999999999999) local-aspace)) i)
	      => (/ i 19999999999999999999999))

    (report)

    (check-ec (:range i 
		      1000000000000000000000000000 
		      1000000000000000000000000020) 
	      ((lambda (x) 
		 (endecode-id (/ x 19999999999999999999999) local-aspace)) i)
	      => (/ i 19999999999999999999999))

    (report)

    (check-ec (:range i 
		      1000000000000000000000000000 
		      1000000000000000000000000020) 
	      ((lambda (x) 
		 (endecode-id (/ 19999999999999999999999 x) local-aspace)) i)
	      => (/ 19999999999999999999999 i))

    (report)

    (check (endecode-id (vector 1 2 3)  
			local-aspace) 
	   =>  (vector 1 2 3))

    (report)

    (check (endecode-id (vector 'a #f '() (vector 1 2 3) "str")  
			local-aspace) 
	   => (vector 'a #f '() (vector 1 2 3) "str"))

    (report)

    (check (endecode-id (u8vector 0 123 233 10 23 34 55 56 76 123 103) 
			local-aspace) 
	   (=> u8vector=?) (u8vector 0 123 233 10 23 34 55 56 76 123 103))

    (report)

    (check (endecode-id (list->u8vector (make-list 255)) 
			local-aspace) 
	   (=> u8vector=?) (list->u8vector (make-list 255)))

    (report)

    (check (endecode-id (cons 1 2)  local-aspace) 
	   => (cons 1 2))

    (report)

    (check (endecode-id (cons 'symbol "string")  
			local-aspace) 
	   => (cons 'symbol "string"))

    (report)

    (check (endecode-id '(hallo liste bla bla bla)  
			local-aspace) 
	   => '(hallo liste bla bla bla))
 
    (report)

    (check-ec (:range i 1000 3000 500)
	      ((lambda (x)
		 (endecode-id (make-list x) local-aspace))
	       i)
	      => (make-list i))

    (report)

    (check-ec (:char-range c #\space #\~)
	      ((lambda (ch)
		 (endecode-id  ch local-aspace))
	       c)
	      => c)

    (report)

    (check (endecode-id 'symbol  local-aspace) => 'symbol)
 
    (report)

    (check (endecode-id 'a-longer-symbol-than-the-one-before  
			local-aspace) 
	   => 'a-longer-symbol-than-the-one-before)

    (report)

    (check (endecode-id (make-symbol-list 400) 
			local-aspace)
	   => (make-symbol-list 400))

    (report)

    (check (endecode-id "a little string"  local-aspace) 
	   => "a little string")

    (report)

	
    (check (endecode-id "a specila string \\\"%!§$%&/()=~~#'^"  
			local-aspace) 
	   => "a specila string \\\"%!§$%&/()=~~#'^")

    (report)

    (check (endecode-id " a quiet long string:
can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
..."  local-aspace) 
	   => " a quiet long string:
can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
...")

    (report)

    (check (endecode-id rec-1 local-aspace) 
	   (=> rec-equal?) rec-1)
  
    (report)

    (check (endecode-id rec-2 local-aspace) 
	   (=> rec-equal?) rec-2)

    (report)

    (check ((endecode-id + local-aspace) 1 2 3) 
	   => 6)
 
    (report)

    (check ((endecode-id list local-aspace) 1 2 3) 
	   => '(1 2 3))
 
    (report)

    (check ((endecode-id make-list local-aspace) 6) 
	   => '(0 1 2 3 4 5 6))
 
    (report)

    (check ((endecode-id rec-equal? local-aspace) rec-2 rec-2) 
	   => #t)

    (report)

    (check ((endecode-id add3 local-aspace) 5) 
	   => 8)

    (report)

    (check ((endecode-id add19 local-aspace) 23)
	   => 42)
    
    (report)

    (check ((endecode-id fak-c local-aspace) 6) 
	   => 720)))

(define (conti-test)
  (let ((local-aspace (local-address-space)))
    (check ((endecode-id fak-c local-aspace) 6) 
	   => 720)))

(define (report) (values))
(define _report
  (let ((i 0))
    (lambda ()
      (set! i (+ i 1))
      (display (if (< i 10) 
		   "******************"
		   "*******************")(current-error-port))
      (newline (current-error-port))
      (display "******* " (current-error-port))
      (display i (current-error-port))
      (display " ********" (current-error-port))
      (newline (current-error-port))
      (display (if (< i 10) 
		   "******************"
		   "*******************")(current-error-port))
      (newline (current-error-port))
      (sleep 1000))))
;;
(define (debug-message str)
  (display str)
  (newline))



;; =================================================

(define str " a quiet long string:
can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
that i can't stop writing, that i can't stop writing,
...")

(define str2 "here comes
a new line, and
a new line, and
a new line, and
a new")

(define (string-test las)

    (check (endecode-id "a little string"  las) 
	 => "a little string")
 
    (check (endecode-id "a specila string \\\"%!§$%&/()=~~#'^"  
		      las) 
	 => "a specila string \\\"%!§$%&/()=~~#'^")

    (check (endecode-id str2 las) 
	 => str2)
)
;    (check (endecode-id str las) 
;	 => str))


(define big-c #f)

(define (make-deep-c n)
  (let loop ((n n))
    (if (zero? n)
	(call-with-current-continuation
	 (lambda (cc)
	   (set! big-c cc)
	   (display "setting!")(newline)
	   n))
	(begin 
	  (display (loop (- n 1)))
	  n))))

;(make-deep-c 100)

(define (display* . args)
  (for-each display args)
  (newline))

