; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Alternate implementation of PRIMITIVES module.

; Based on
;(define-interface primitives-interface
;  (export close-port                    ;extended-ports
;          collect                       ; ,collect command
;          continuation-length
;          continuation-ref
;          continuation-set!
;          continuation?
;          extended-number-length
;          extended-number-ref
;          extended-number-set!
;          extended-number?
;          external-call
;          external-lookup
;          external-name
;          external-value
;          external?
;          find-all-xs                   ; externals.scm
;          force-output                  ;ports re-exports this.
;          get-dynamic-state             ;fluids
;          make-continuation
;          make-extended-number
;          make-external
;          make-record
;          make-template
;          make-weak-pointer
;          memory-status                 ;interrupts
;          record-length
;          record-ref
;          record-set!
;          record?
;          schedule-interrupt            ;interrupts re-exports
;          set-dynamic-state!            ;fluids
;          set-enabled-interrupts!       ;interrupts
;          set-exception-handler!
;          set-interrupt-handlers!       ;interrupts
;          template-length
;          template-ref
;          template-set!
;          template?
;          time                          ;interrupts
;          unspecific                    ;record
;          vm-extension
;          vm-return
;          weak-pointer-ref
;          weak-pointer?
;          write-string))


(define underlying-error error)

(define (unspecific) (if #f #f))

; Shadow record primitives so that the existing inspector and printer
; don't get confused.

(define pseudo-record-type
  (make-record-type 'new-record '(fields)))
(define make-record
  (let ((make (record-constructor pseudo-record-type
				  '(fields))))
    (lambda (size init)
      (make (make-vector size init)))))
(define pseudo-record-fields
  (record-accessor pseudo-record-type 'fields))
(define (record-ref record i)
  (vector-ref (pseudo-record-fields record) i))
(define (record-set! record i value)
  (vector-set! (pseudo-record-fields record) i value))
(define record? (record-predicate pseudo-record-type))
(define (record-length record)
  (vector-length (pseudo-record-fields record)))


; Similarly for extended numbers.

(define pseudo-extended-number-type
  (make-record-type 'new-extended-number '(fields)))
(define make-extended-number
  (let ((make (record-constructor pseudo-extended-number-type
				  '(fields))))
    (lambda (size init)
      (make (make-vector size init)))))
(define pseudo-extended-number-fields
  (record-accessor pseudo-extended-number-type 'fields))
(define (extended-number-ref extended-number i)
  (vector-ref (pseudo-extended-number-fields extended-number) i))
(define (extended-number-set! extended-number i value)
  (vector-set! (pseudo-extended-number-fields extended-number) i value))
(define extended-number?
  (record-predicate pseudo-extended-number-type))
(define (extended-number-length extended-number)
  (vector-length (pseudo-extended-number-fields extended-number)))

; Dynamic state (= current thread)

(define *dynamic-state* 'uninitialized-dynamic-state)
(define (get-dynamic-state) *dynamic-state*)
(define (set-dynamic-state! state)
  (if (not (and (record? state)
		(list? (record-ref state 1))))
      (underlying-error "invalid dynamic state" state))
  (set! *dynamic-state* state))

; Etc.

(define (close-port port)
  ((if (input-port? port) close-input-port close-output-port)
   port))

(define (write-string s port)
  (display s port))

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

(define *pseudo-exception-handler* #f)
(define (set-exception-handler! h)
  (set! *pseudo-exception-handler* h))

(define *pseudo-interrupt-handlers* #f)
(define (set-interrupt-handlers! v)
  (set! *pseudo-interrupt-handlers* v))

(define (unimplemented name)
  (lambda args (underlying-error "unimplemented primitive" name args)))
(define collect (unimplemented 'collect))
(define external-call (unimplemented 'external-call)) 
(define external-lookup (unimplemented 'external-lookup))
(define external-name (unimplemented 'external-name))
(define external-value (unimplemented 'external-value))
(define (external? x) #f)
(define find-all-xs (unimplemented 'find-all-xs))
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


; force-output, continuations, etc. are inherited

; templates

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

(define (raise-exception opcode arguments)
  (apply (get-exception-handler)
	 opcode
	 arguments))

(define (get-exception-handler)
  *pseudo-exception-handler*)


(define (clear-registers!)
  (set! *dynamic-state* 'uninitialized-dynamic-state)
  (set! *pseudo-enabled-interrupts* 0)
  (set! *pseudo-interrupt-handlers* #f)
  (set! *pseudo-exception-handler* #f))

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
