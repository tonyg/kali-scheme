; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Utility for tracking down storage leaks.
;
; Just do (traverse-depth-first obj1) or (traverse-breadth-first obj1),
; and then (trail obj2) to find out via what path obj1 points to obj2.
;
; Breadth first traversal needs misc/queue.scm.


(define *mark-table* #f)
(define *interesting-table* #f)

(define *traverse-count* 0)

(define (start-over)		
  (set! *mark-table* (make-table hash))
  (set! *interesting-table* (make-table))
  (set! *traverse-count* 0))

(define (traverse-depth-first obj)
  (start-over)
  (let recur ((obj obj) (parent (list 'root)) (parent-tag 'root))
    (if (stored? obj)
	(if (not (table-ref *mark-table* obj))
	    (let ((tag (visit obj parent parent-tag)))
	      (for-each-subobject (lambda (child)
				    (recur child obj tag))
				  obj))))))

(define (traverse-breadth-first obj)
  (start-over)
  (let ((queue (make-queue)))
    (define (deal-with obj parent parent-tag)
      (if (stored? obj)
	  (if (not (table-ref *mark-table* obj))
	      (enqueue! queue
			(cons obj
			      (visit obj parent parent-tag))))))
    (deal-with obj (list 'root) 'root)
    (let loop ()
      (if (not (queue-empty? queue))
	  (let* ((parent+tag (dequeue! queue))
		 (parent (car parent+tag))
		 (parent-tag (cdr parent+tag)))
	    (for-each-subobject (lambda (obj)
				  (deal-with obj parent parent-tag))
				parent)
	    (loop))))))

(define (visit obj parent parent-tag)
  (table-set! *mark-table* obj parent)
  (if (interesting? obj)
      (let ((tag *traverse-count*))
	(table-set! *interesting-table* tag obj)
	(set! *traverse-count* (+ *traverse-count* 1))
	(write tag) (display " ")
	(write (list parent-tag))
	(display ": ") (write obj) (newline)
	tag)
      parent-tag))

(define (trail obj)
  (let loop ((obj (if (integer? obj)
		      (table-ref *interesting-table* obj)
		      obj)))
    (let ((probe (table-ref *mark-table* obj)))
      (if probe
	  (loop probe))
      (if (not (vector? obj))
	  (begin (write obj)
		 (newline))))))

(define (interesting? obj)
  (and (closure? obj)
       (let ((info (template-info (closure-template obj))))
	 (if (integer? info)
	     (> info first-interesting-template-info)
	     #t))))

(define (template-info tem) (template-ref tem 1))

(define first-interesting-template-info
  (template-info
   (closure-template 
    (loophole :closure read)))) ;foo
  
;(define (interesting? obj)
;  (if (pair? obj)
;      #f
;      (if (vector? obj)
;          #f
;          #t)))
	  
(define (for-each-subobject proc obj)
  (cond ((pair? obj)
	 (proc (car obj))
	 (proc (cdr obj)))
	((symbol? obj)
	 (proc (symbol->string obj)))
	((vector? obj)
	 (vector-for-each proc obj))
	((closure? obj)
	 (proc (closure-template obj))
	 (proc (closure-env obj)))
	((location? obj)
	 (proc (location-id obj))
	 (if (location-defined? obj)
	     (proc (contents obj))))
	((record? obj)
	 (cond ((eq? obj *mark-table*)	;or (debug-data-table)
		(display "skipping mark table") (newline))
	       ((eq? obj *interesting-table*)
		(display "skipping interesting table") (newline))
	       (else
		(record-for-each proc obj))))
	((continuation? obj)
	 (continuation-for-each proc obj))
	((template? obj)
	 (template-for-each proc obj))
	((extended-number? obj)
	 (extended-number-for-each proc obj))))


(define (vector-for-each proc v)
  (let ((z (vector-length v)))
    (do ((i (- z 1) (- i 1)))
	((< i 0) #f)
      (if (not (vector-unassigned? v i))
	  (proc (vector-ref v i))))))

(define-syntax define-for-each
  (syntax-rules ()
    ((define-for-each foo-for-each foo-length foo-ref)
     (define (foo-for-each proc v)
       (let ((z (foo-length v)))
	 (do ((i (- z 1) (- i 1)))
	     ((< i 0) #f)
	   (proc (foo-ref v i))))))))

(define-for-each record-for-each
  record-length record-ref)
(define-for-each continuation-for-each
  continuation-length continuation-ref)
(define-for-each template-for-each
  template-length template-ref)
(define-for-each extended-number-for-each
  extended-number-length extended-number-ref)


(define (quick-hash obj n)
  (cond ((symbol? obj) (string-hash (symbol->string obj)))
	((location? obj) (+ 3 (quick-hash (location-id obj) n)))
	((string? obj) (+ 33 (string-hash obj)))
	((integer? obj) (if (and (>= obj 0)
				 (< obj hash-mask))
			    obj
			    (modulo obj hash-mask)))
	((char? obj) (+ 333 (char->integer obj)))
	((eq? obj #f) 3001)
	((eq? obj #t) 3003)
	((null? obj) 3005)
	((pair? obj) (if (= n 0)
			 30007
			 (+ (quick-hash (car obj) (- n 1))
			    (quick-hash (cdr obj) (- n 1)))))
	((vector? obj) (if (= n 0)
			   30009
			   (if (> (vector-length obj) 1)
			       (+ 30011 (quick-hash (vector-ref obj 1)
						    (- n 1)))
			       30017)))
	((number? obj) 4000)
	((closure? obj) 4004)
	((template? obj) (if (= n 0)
			     300013
			     (+ 30027 (quick-hash (template-ref obj 1)
						  (- n 1)))))
	((output-port? obj) 4006)
	((input-port? obj) 4007)
	((record? obj) 4008)
	((continuation? obj) 4009)
	((number? obj) 40010)
	((string? obj) 40011)
	((code-vector? obj) 40012)
	((eq? obj (unspecific)) 40013)
	(else 50007)))

(define hash-mask (- (arithmetic-shift 1 26) 1))

(define (hash obj) (quick-hash obj 1))

(define (leaf? obj)
  (or (and (number? obj)
	   (not (extended-number? obj)))
      ;; (symbol? obj)
      (string? obj)
      (code-vector? obj)
      (char? obj)
      (eq? obj #f)
      (eq? obj #t)
      (eq? obj '())
      (eq? obj (unspecific))))

(define usual-leaf-predicate leaf?)

(define (set-leaf-predicate! proc) (set! leaf? proc))

(define (stored? obj) (not (leaf? obj)))

(define least-fixnum (arithmetic-shift -1 29))
(define greatest-fixnum (- -1 least-fixnum))

