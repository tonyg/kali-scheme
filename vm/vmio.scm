; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

; This is file vmio.scm.

;;;; I/O primitives

; Port fields:
;  port-mode         1 = input
;                    2 = output
;                    (this field doesn't ever change)
;  port-index        index into open-ports vector
;                    0 = initial input port
;                    1 = initial output port
;                    -1 = not open
;  peeked-char       char or #f
;  port-id           for debugging
;
; Questions:
;  What to do if an error occurs?
;  How to deal with transcript-on and transcript-off ?
;  How to deal with uninterrubtibly opening a port and pushing it onto
;    an outstanding-ports list?

; *open-vm-ports* is a vector of descriptors for open ports.
; *open-ports is a vector of open ports of the underlying Scheme implementation.

(define *number-of-ports* 100)

(define *open-ports*    (unassigned))
(define *open-vm-ports* (unassigned))

(define for-input  1)
(define for-output 2)

(define (vm-input-port? obj)
  (and (port? obj)
       (= (port-mode obj) (enter-fixnum for-input))))

(define (vm-output-port? obj)
  (and (port? obj)
       (= (port-mode obj) (enter-fixnum for-output))))

(define iip-index 0)  ;index of initial input port
(define iop-index 1)  ;[out/in]ditto

(define (initialize-i/o-system)
  (set! *open-ports*     (make-vector *number-of-ports*))
  (set! *open-vm-ports*  (make-vector *number-of-ports*))
  (if (or (null-pointer? *open-ports*)
	  (null-pointer? *open-vm-ports*))
      (error "out of memory, unable to continue"))
  (vector+length-fill! *open-vm-ports* *number-of-ports* false)
  (vector-set! *open-ports* iip-index (current-input-port))
  (vector-set! *open-ports* iop-index (current-output-port))
  (let ((iip (make-port (enter-fixnum for-input)
                        (enter-fixnum iip-index)
                        false
                        (enter-string "si")
			universal-key))
        (iop (make-port (enter-fixnum for-output)
                        (enter-fixnum iop-index)
                        false
                        (enter-string "so")
			universal-key)))
    (vector-set! *open-vm-ports* iip-index iip)
    (vector-set! *open-vm-ports* iop-index iop)
    unspecific))

(define (allocate-another-port-descriptor got-one-cont none-left-cont)
  (let* ((new-count (+ *number-of-ports* 8))
	 (old-count *number-of-ports*)
	 (new-ports (make-vector new-count))
	 (new-vm-ports (make-vector new-count)))
    (cond ((or (null-pointer? new-ports)
	       (null-pointer? new-vm-ports))
	   (none-left-cont))
	  (else
	   (vector+length-fill! new-vm-ports new-count false)
	   (do ((i 0 (+ i 1)))
	       ((>= i *number-of-ports*))
	     (vector-set! new-ports i (vector-ref *open-ports* i))
	     (vector-set! new-vm-ports i (vector-ref *open-vm-ports* i)))
	   (deallocate *open-ports*)
	   (deallocate *open-vm-ports*)
	   (set! *open-ports* new-ports)
	   (set! *open-vm-ports* new-vm-ports)
	   (set! *number-of-ports* new-count)
	   (got-one-cont old-count)))))  ; new and unallocated port index

(define initial-i/o-heap-space (* 2 port-size))

(define (initial-input-port)
  (vector-ref *open-vm-ports* iip-index))

(define (initial-output-port)
  (vector-ref *open-vm-ports* iop-index))

; Auxiliaries for I/O primitives

(define (extract-port port)
  (vector-ref *open-ports* (extract-fixnum (port-index port))))

(define (find-port-index)
  (let loop ((i 0))
    (cond ((>= i *number-of-ports*)
	   -1)
	  ((false? (vector-ref *open-vm-ports* i))
	   i)
	  (else (loop (+ i 1))))))

(define (use-port-index! index vm-port port)
  (vector-set! *open-ports* index port)
  (vector-set! *open-vm-ports* index vm-port))

; [An open can fail for several reasons:
;    - The OS couldn't open it
;    - There are no empty slots in *open-ports* and we can't get more memory
; ]

(define (open-port filename mode key collect-saving-temp
		   succeeded os-failed vm-failed)
  (let loop ((index (find-port-index)) (filename filename))
    (if (>= index 0)
	(really-open-port filename index mode key succeeded os-failed)
	(let ((filename (collect-saving-temp filename)))
	  (let ((index (find-port-index)))
	    (if (>= index 0)
		(loop index filename)
		(allocate-another-port-descriptor
		 (lambda (index)
		   (loop index filename))
		 (lambda ()
		   (vm-failed filename)))))))))

(define (really-open-port filename index mode key succeeded os-failed)
  (let ((port (cond ((= mode for-output)
		     (open-output-file (extract-string filename)))
		    (else        ;(= mode for-input)
		     (open-input-file (extract-string filename))))))
    (if (null-port? port)
	(os-failed filename)
	(let ((vm-port (make-port (enter-fixnum mode)
				  (enter-fixnum index)
				  false
				  filename
				  key)))
	  (use-port-index! index vm-port port)
	  (succeeded vm-port)))))

(define (open? port)
  (>= (port-index port) (enter-fixnum 0)))

(define (close-port vm-port)
  (if (open? vm-port)
      (let ((index (extract-fixnum (port-index vm-port))))
        (if (not (or (= index iip-index)
                     (= index iop-index)))
            (let ((port (extract-port vm-port))
                  (mode (extract-fixnum (port-mode vm-port))))
              (cond ((= mode for-input)
                     (close-input-port port))
                    ((= mode for-output)
                     (close-output-port port))
                    (else
                     (error "this shouldn't happen when closing a port")))
              (set-port-mode! vm-port (enter-fixnum 0))
              (set-port-index! vm-port (enter-fixnum -1))
;              (vector-set! *open-ports* index 0)  ; type error
              (vector-set! *open-vm-ports*  index false))))))

; The following is called after the GC finishes.

(define (close-untraced-ports!)
  (do ((i 2 (+ i 1)))   ; Skip over initial input and output ports
      ((= i *number-of-ports*) #f)
    (let ((port (vector-ref *open-vm-ports* i)))
      (if (not (false? port))
	  (let* ((header (stob-header port))
		 (new (cond ((stob? header)  ; port was copied
			     header)
			    (else            ; port was not copied
			     (close-port-noisily port)
			     false))))
	    (vector-set! *open-vm-ports* i new))))))

(define (close-port-noisily port)
  (let* ((header (stob-header (port-id port)))
	 (name (if (stob? header)
		   header              ; name was copied
		   (port-id port))))   ; name was not copied
    (close-port port)
    (write-string "Port closed: " (current-output-port))
    (write-vm-string (port-id port) (current-output-port))
    (newline (current-output-port))))

