; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Test various of the byte-codes

;(let ((system (make-system '("~/s48/x48/boot/byte-code-test.scm") 'resume #f)))
;  (write-system system "~/s48/x48/boot/byte-code-test.image"))

(define *tests* '())
(define *output-channel* #f)

(define (make-test . args)
  (set! *tests* (cons args *tests*)))

(define (run-test string compare result proc)
  (write-string string *output-channel*)
  (write-string "..." *output-channel*)
  (write-string (if (compare (proc) result) "OK" "failed") *output-channel*)
  (newline *output-channel*))

(make-test "testing test mechanism" (lambda (x y) (eq? x y)) 0 (lambda () 0))
(make-test "primitive catch and throw" (lambda (x y) (eq? x y)) 10
	   (lambda ()
	     (* 10 (primitive-catch (lambda (k)
				      (my-primitive-throw k 1)
				      (message "after throw???")
				      2)))))


(define (my-primitive-throw cont value)
  (with-continuation cont (lambda () value)))

(define (message string)
  (write-string string *output-channel*)
  (newline channel))

(define (write-string string channel)
  (channel-write string
		 0
		 (string-length string)
		 channel))

(define (newline channel)
  (write-string "
" channel))

(define (resume arg in in-encoding out out-encoding error error-encoding)
  (set! *output-channel* out)
  (do ((tests (do ((tests *tests* (cdr tests))
		   (r '() (cons (car tests) r)))
		  ((eq? '() tests) r))
	      (cdr tests)))
      ((eq? '() tests))
    (apply run-test (car tests)))
  (write-string "done" *output-channel*)
  (newline *output-channel*)
  (halt 0))

(define *initial-bindings* '())

(define (initial-env name)
  (let ((probe (assq name *initial-bindings*)))
    (if probe (cdr probe) (error "unbound" name))))

(define (define-initial name val)
  (let* ((probe (assq name *initial-bindings*))
	 (loc (if probe
		  (cdr probe)
		  (let ((loc (make-undefined-location name)))
		    (set! *initial-bindings*
			  (cons (cons name loc) *initial-bindings*))
		    loc))))
    ;; (set-location-defined?! loc #t)  - obsolescent?
    (set-contents! loc val)))

(for-each (lambda (name val)
	    (define-initial name val))
          '(    cons car cdr + - * < = > list map append reverse)
	  (list cons car cdr + - * < = > list map append reverse))

(make-test "little env-lookup test" eq? car
	   (lambda ()
	     (contents (initial-env 'car))))

(define (error string . stuff) (message string))
