; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.



;(define-syntax assert
;  (lambda ignore
;    ''assert))

(define debugging? #t)

; ,bench
; ,load rts/defenum.scm
; ,for-syntax ,load my-vm/for-syntax.scm
; ,load my-vm/s48-prescheme.scm my-vm/util.scm my-vm/memory.scm
; ,load my-vm/arch.scm my-vm/data.scm my-vm/struct.scm
; ,load link/s48-features.scm link/read-image.scm
; ,load-into extended-numbers misc/bigbit.scm

(define (resume filename arg)
  (call-startup-procedure (extract (read-image filename)) arg))

(define (call-startup-procedure proc arg)
  (proc arg (current-input-port) (current-output-port)))

(define level 14)

(define (read-image filename)
  (call-with-input-file filename
    (lambda (port)
      (read-page port) ; read past any user cruft at the beginning of the file
      (let* ((old-level          (read-number port))
             (old-bytes-per-cell (read-number port))
             (old-begin (cells->a-units (read-number port)))
             (old-hp    (cells->a-units (read-number port)))
             (startup-proc       (read-number port)))
        (read-page port)
        (if (not (= old-level level))
            (error "format of image is incompatible with this version of system"
                   old-level level))
        (if (not (= old-bytes-per-cell bytes-per-cell))
            (error "incompatible bytes-per-cell"
                   old-bytes-per-cell bytes-per-cell))

	;; ***CHANGED***
	(create-memory (a-units->cells (- (addr1+ old-hp) old-begin))
		       quiescent)
	(set! *hp* 0)

        (let* ((delta (- *hp* old-begin))
               (new-hp (+ old-hp delta)))
	  (let ((reverse? (check-image-byte-order port)))
	    (read-block port *memory* *hp* (- old-hp old-begin))
	    (if reverse?
		(reverse-byte-order new-hp))
	    (if (= delta 0)
		(set! *hp* new-hp)
		(relocate-image delta new-hp))
	    (set! *extracted* (make-vector (a-units->cells *memory-end*) #f))
	    (adjust startup-proc delta)))))))

(define (check-image-byte-order port)
  (read-block port *memory* *hp* (cells->a-units 1))
  (cond ((= (fetch *hp*) 1)
	 #f)
	(else
	 (reverse-descriptor-byte-order! *hp*)
	 (if (= (fetch *hp*) 1)
	     #t
	     (begin (error "unable to correct byte order" (fetch *hp*))
		    #f)))))

(define *hp* 0)

(define *extracted* #f)

(define (extract obj)
  (cond ((vm-fixnum? obj) (extract-vm-fixnum obj))
	((stob? obj)
	 (let ((index (a-units->cells (address-after-header obj))))
	   (or (vector-ref *extracted* index)
	       (extract-stored-object obj
				      (lambda (new)
					(vector-set! *extracted* index new)
					new)))))
	((vm-char? obj) (extract-char obj))
	((vm-eq? obj null) '())
	((vm-eq? obj false) #f)
	((vm-eq? obj true) #t)
	((vm-eq? obj vm-unspecific) (if #f 0))
	((vm-eq? obj unbound-marker) '<unbound>)
	((vm-eq? obj unassigned-marker) '<unassigned>)
	(else (error "random descriptor" obj))))

(define (extract-stored-object old store-new!)
  ((vector-ref stored-object-extractors (header-type (stob-header old)))
   old store-new!))

(define stored-object-extractors
  (make-vector stob-count (lambda rest (apply error "no extractor" rest))))

(define (define-extractor which proc)
  (vector-set! stored-object-extractors which proc))

(define-extractor stob/pair
  (lambda (old store-new!)
    (let ((new (cons #f #f)))
      (store-new! new)
      (set-car! new (extract (vm-car old)))
      (set-cdr! new (extract (vm-cdr old)))
      new)))

(define-extractor stob/vm-closure
  (lambda (old store-new!)
    (store-new! (make-closure (extract (vm-closure-template old))
			      (extract (vm-closure-env old))))))

(define-extractor stob/symbol
  (lambda (obj store-new!)
    (store-new! (string->symbol (extract (vm-symbol->string obj))))))

(define-extractor stob/vm-location
  (lambda (obj store-new!)
    (let ((new (store-new! (make-undefined-location
			        (+ 10000
				   (extract (vm-location-id obj))))))
	  (val (vm-contents obj)))
      (if (not (vm-eq? val unbound-marker))
	  (begin (set-location-defined?! new #t)
		 (if (not (vm-eq? val unassigned-marker))
		     (set-contents! new (extract val)))))
      new)))

(define-extractor stob/string
  (lambda (obj store-new!)
    (store-new! (extract-string obj))))

(define-extractor stob/vm-code-vector
  (lambda (obj store-new!)
    (store-new! (extract-code-vector obj))))

(define-extractor stob/vector
  (lambda (obj store-new!)
    (let* ((z (vm-vector-length obj))
	   (v (make-vector z)))
      (store-new! v)
      (do ((i 0 (+ i 1)))
	  ((= i z) v)
	(vector-set! v i (extract (vm-vector-ref obj i)))))))

;(define-extractor stob/record
;  (lambda (obj store-new!)
;    (let* ((z (vm-record-length obj))
;           (v (make-record z)))
;      (store-new! v)
;      (do ((i 0 (+ i 1)))
;          ((= i z) v)
;        (record-set! v i (extract (vm-record-ref obj i)))))))

(define-extractor stob/port
  (lambda (obj store-new!)
    (store-new!
     (case (extract-vm-fixnum (port-index obj))
       ((0) (current-input-port))
       ((1) (current-output-port))
       (else (error "unextractable port" obj))))))



(define (extract-code-vector x)
  (let ((z (vm-code-vector-length x)))
    (let ((v (make-code-vector z 0)))
      (do ((i 0 (+ i 1)))
          ((>= i z) v)
        (code-vector-set! v i (vm-code-vector-ref x i))))))



; Various things copied from vm/gc.scm

(define (store-next! descriptor)
  (store! *hp* descriptor)
  (set! *hp* (addr1+ *hp*)))

(define (reverse-descriptor-byte-order! addr)
  (let ((x (fetch-byte addr)))
    (store-byte! addr (fetch-byte (addr+ addr 3)))
    (store-byte! (addr+ addr 3) x))
  (let ((x (fetch-byte (addr+ addr 1))))
    (store-byte! (addr+ addr 1) (fetch-byte (addr+ addr 2)))
    (store-byte! (addr+ addr 2) x)))

(define (reverse-byte-order end)
  (write-string "Correcting byte order of resumed image."
		 (current-output-port))
  (newline (current-output-port))
  (let loop ((ptr *hp*))
    (reverse-descriptor-byte-order! ptr)
    (let ((value (fetch ptr)))
      (if (addr< ptr end)
	  (loop (if (b-vector-header? value)
		    (addr+ (addr1+ ptr) (header-a-units value))
		    (addr1+ ptr)))))))

(define (adjust descriptor delta)
  (if (stob? descriptor)
      (make-stob-descriptor (addr+ (address-after-header descriptor) delta))
      descriptor))

(define (relocate-image delta new-hp)
  (let loop ()
    (cond ((addr< *hp* new-hp)
	   (let ((d (adjust (fetch *hp*) delta)))
	     (store-next! d)
	     (cond ;;((eq? d the-primitive-header)
	       ;; Read symbolic label name.
	       ;;(store-next!
	       ;;  (label->fixnum (name->label (read port)))))
	       ((b-vector-header? d)
		(set! *hp* (addr+ *hp*
				  (cells->bytes
				   (bytes->cells
				    (header-length-in-bytes d)))))))
	     (loop))))))
