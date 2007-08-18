;; ,config ,load kali/scheme/misc/packages.scm
;; ,open primitives kali address-spaces messages threads signals srfi-66 srfi-78 srfi-9 reset/shift

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
	      (cond ((not (null? bad-count-proxies))
		     (for-each return-counts (adjust-proxy-counts! bad-count-proxies)))
		     ;(error "endecode-id: bad count proxies! That can't be true"))
		     
		    ((not (null? missing-uids))
		     (error "endecode-id: missing uids! That can't be true"))
		    (else (car val)))))))))

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

(define (encode-decode-test)
  (if (not (server-running?))
      (begin 
	(display "starting server now...")
	(newline)
	(spawn start-server 'test-server)
	(sleep 2000)))
  (let ((local-aspace (local-address-space)))

    (check-ec (endecode-id #t local-aspace) => #t)

    (check-ec (endecode-id #f local-aspace) => #f)

    (check-ec (endecode-id '() local-aspace) => '())

    (check-ec (endecode-id 1 local-aspace) => 1)

    (check-ec (endecode-id 2 local-aspace) => 2)

    (check-ec (endecode-id 300 local-aspace) => 300)

    (check-ec (endecode-id 12334 local-aspace) => 12334)

    (check-ec (endecode-id 912189349087623783450986709823476534098234 
			   local-aspace) 
	      => 912189349087623783450986709823476534098234)

    (check-ec (endecode-id 1.4554645 local-aspace) => 1.4554645)

    (check-ec (endecode-id 3453454.45645656 local-aspace) => 3453454.45645656)

    (check-ec (endecode-id 5/7 local-aspace) => 5/7)

    (check-ec (endecode-id 199/232432298723432423423 
			   local-aspace) 
	      => 199/232432298723432423423)

    (check-ec (endecode-id 132442896789623489756347899/178634908034981298721787623  
			   local-aspace) 
	      => 132442896789623489756347899/178634908034981298721787623)

    (check-ec (endecode-id (vector 1 2 3)  
			   local-aspace) 
	      =>  (vector 1 2 3))

    (check-ec (endecode-id (vector 'a #f '() (vector 1 2 3) "str")  
			   local-aspace) 
	      => (vector 'a #f '() (vector 1 2 3) "str"))

    (check-ec (endecode-id (u8vector 0 123 233 10 23 34 55 56 76 123 103) 
			   local-aspace) 
	      (=> u8vector=?) (u8vector 0 123 233 10 23 34 55 56 76 123 103))

    (check-ec (endecode-id (list->u8vector (make-list 255)) 
			   local-aspace) 
	      (=> u8vector=?) (list->u8vector (make-list 255)))

    (check-ec (endecode-id (cons 1 2)  local-aspace) 
	      => (cons 1 2))

    (check-ec (endecode-id (cons 'symbol "string")  
			   local-aspace) 
	      => (cons 'symbol "string"))

    (check-ec (endecode-id '(hallo liste bla bla bla)  
			   local-aspace) 
	      => '(hallo liste bla bla bla))
    
    (check-ec (endecode-id (make-list 1400)  local-aspace) 
	      => (make-list 1400))

    (check-ec (endecode-id  #\a local-aspace) => #\a)
    
    (check-ec (endecode-id  #\~ local-aspace) => #\~)

    (check-ec (endecode-id 'symbol  local-aspace) => 'symbol)
    
    (check-ec (endecode-id 'a-longer-symbol-than-the-one-before  
			   local-aspace) 
	      => 'a-longer-symbol-than-the-one-before)

    (check-ec (endecode-id "a little string"  local-aspace) 
	      => "a little string")
    
    (check-ec (endecode-id "a specila string \\\"%!§$%&/()=~~#'^"  
			   local-aspace) 
	      => "a specila string \\\"%!§$%&/()=~~#'^")
    
    (check-ec (endecode-id " a quiet long string:
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

    (check-ec (endecode-id rec-1 local-aspace) 
	      (=> rec-equal?) rec-1)
    
    (check-ec (endecode-id rec-2 local-aspace) 
	      (=> rec-equal?) rec-2)

    (check-ec ((endecode-id + local-aspace) 1 2 3) 
	      => 6)
    
    (check-ec ((endecode-id list local-aspace) 1 2 3) 
	      => '(1 2 3))
    
    (check-ec ((endecode-id make-list local-aspace) 6) 
	      => '(0 1 2 3 4 5 6))
    
    (check-ec ((endecode-id rec-equal? local-aspace) rec-2 rec-2) 
	      => #t)

    (check-ec ((endecode-id fak-c local-aspace) 6) 
	      => 720)))
