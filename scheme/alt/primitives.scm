; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Alternate implementation of PRIMITIVES module.

(define underlying-error error)

(define (unspecific) (if #f #f))


; Records

(define-record-type new-record :new-record
  (make-new-record fields)
  record?
  (fields new-record-fields))

(define (make-record size init)
  (make-new-record (make-vector size init)))

(define (record-ref r i)
  (vector-ref (new-record-fields r) i))

(define (record-set! r i value)
  (vector-set! (new-record-fields r) i value))

(define (record-length r)
  (vector-length (new-record-fields r)))


; Extended numbers

(define-record-type new-extended-number :new-extended-number
  (make-new-extended-number fields)
  extended-number?
  (fields new-extended-number-fields))

(define-record-discloser :new-extended-number
  (lambda (n) `(extended-number ,(new-extended-number-fields n))))

(define (make-extended-number size init)
  (make-new-extended-number (make-vector size init)))

(define (extended-number-ref n i)
  (vector-ref (new-extended-number-fields n) i))

(define (extended-number-set! n i value)
  (vector-set! (new-extended-number-fields n) i value))

(define (extended-number-length n)
  (vector-length (new-extended-number-fields n)))


; Current thread

(define *current-thread* 'uninitialized-current-thread)
(define (current-thread) *current-thread*)
(define (set-current-thread! thread)
  (if (not (and (record? thread)
		(list? (record-ref thread 1))))
      (underlying-error "invalid current thread" thread))
  (set! *current-thread* thread))

; Etc.

(define (close-port port)
  ((if (input-port? port) close-input-port close-output-port)
   port))

(define (write-string s port)
  (display s port))

(define (write-string-tail s start port)
  (display (substring s start (port))

(define (schedule-interrupt interval)
  (if (not (= interval 0))
      (warn "ignoring schedule-interrupt" interval)))

(define *pseudo-enabled-interrupts* 0)

(define (set-enabled-interrupts! ei)
  (let ((previous *pseudo-enabled-interrupts*))
    (set! *pseudo-enabled-interrupts* ei)
    ;; (if (bitwise-and *pseudo-pending-interrupts* ei) ...)
    previous))

(define *pseudo-pending-interrupts* 0)

(define *pseudo-exception-handlers* #f)
(define (set-exception-handlers! h)
  (let ((old *pseudo-exception-handlers*))
    (set! *pseudo-exception-handlers* h)
    old))

(define *pseudo-interrupt-handlers* #f)
(define (set-interrupt-handlers! v)
  (let ((old *pseudo-interrupt-handlers*))
    (set! *pseudo-interrupt-handlers* v)
    old))

(define (unimplemented name)
  (lambda args (underlying-error "unimplemented primitive" name args)))
(define collect (unimplemented 'collect))
(define external-call (unimplemented 'external-call)) 
(define external-lookup (unimplemented 'external-lookup))
(define external-name (unimplemented 'external-name))
(define external-value (unimplemented 'external-value))
(define (external? x) #f)
(define find-all (unimplemented 'find-all))
(define make-external (unimplemented 'make-external))
(define vm-extension (unimplemented 'vm-extension))

(define (memory-status which arg)
  (case which
    ((2) 100)
    ((3) (display "(Ignoring set-minimum-recovered-space!)") (newline))
    (else (underlying-error "unimplemented memory-status" which arg))))

(define (time which arg)
  (case which
    ((0) 1000)
    (else (underlying-error "unimplemented time" which arg))))


; end of definitions implementing PRIMITIVES structure


; --------------------

; Auxiliary crud.

(define (maybe-handle-interrupt which)
  ;; Should actually do (get-highest-priority-interrupt!) ...
  (let ((bit (arithmetic-shift 1 which)))
    (cond ((= (bitwise-and *pseudo-enabled-interrupts* bit) 0)
	   (set! *pseudo-pending-interrupts*
		 (bitwise-ior *pseudo-pending-interrupts* bit))
	   (display "(Interrupt deferred)")
	   (newline)
	   #f)
	  (else
	   (set! *pseudo-pending-interrupts*
		 (bitwise-and *pseudo-pending-interrupts*
			      (bitwise-not bit)))
	   (display "(Handling interrupt)")
	   (newline)
	   ((vector-ref *pseudo-interrupt-handlers* which)
	    (set-enabled-interrupts! 0))
	   #t))))

(define (raise-exception opcode exception arguments)
  (apply (vector-ref (get-exception-handlers) opcode)
	 (cons exception arguments)))

(define (get-exception-handlers)
  *pseudo-exception-handlers*)


(define (clear-registers!)
  (set! *current-thread* 'uninitialized-current-thread)
  (set! *pseudo-enabled-interrupts* 0)
  (set! *pseudo-interrupt-handlers* #f)
  (set! *pseudo-exception-handlers* #f))

(define *vm-return* #f)

(define (vm-return . rest)
  (if *vm-return*
      (apply *vm-return* rest)
      (underlying-error "vm-return" rest)))


(define (?start entry-point arg) ;E.g. (?start (usual-resumer bare) 0)
  (clear-registers!)
  (call-with-current-continuation
    (lambda (k)
      (set! *vm-return* k)
      (entry-point arg
		   (current-input-port)
		   (current-output-port)))))
