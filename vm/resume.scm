; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

; This is file resume.scm.

; RESUME is the main entry point to the entire system

(define (resume filename startup-vector startup-vector-length
		initial-memory-size initial-stack-size)
  (create-memory (a-units->cells initial-memory-size) 0)
  (initialize-heap (memory-begin)
		   (- (memory-size) (a-units->cells initial-stack-size)))
  (let* ((resume-space (+ (do ((i 0 (+ i 1))
			       (s 0 (+ s (vm-string-size
					  (string-length
					   (vector-ref startup-vector i))))))
			      ((>= i startup-vector-length)
			       s))
			  (code-vector-size 2)))
	 (startup-proc (read-image filename
				   (+ initial-heap-space resume-space))))
    (initialize-i/o-system)
    (initialize-stack (+ (memory-begin)
			 (- (cells->a-units (memory-size)) initial-stack-size))
		      (a-units->cells initial-stack-size))
    (initialize-interpreter)
    (call-startup-procedure startup-proc startup-vector startup-vector-length)))

(define initial-heap-space
  (+ initial-interpreter-heap-space
     (+ initial-stack-heap-space
	initial-i/o-heap-space)))

(define (call-startup-procedure startup-proc startup-vector startup-vector-length)
  (let ((vector (vm-make-vector startup-vector-length universal-key)))
    (do ((i 0 (+ i 1)))
	((>= i startup-vector-length))
      (vm-vector-set! vector i (enter-string (vector-ref startup-vector i))))
    (clear-registers)
    (push vector)
    (push (initial-input-port))
    (push (initial-output-port))
    (let ((code (make-code-vector 2 universal-key)))
      (code-vector-set! code 0 op/call)
      (code-vector-set! code 1 3)         ; nargs    
      (set! *code-pointer* (address-after-header code)))
    (restart startup-proc)))

(define op/call (enum op call))

(define (restart value)
  (set! *val* value)
  (let loop ()
    (let ((option (interpret)))
      (cond ((= option return-option/exit)
	     *val*)
	    ((= option return-option/external-call)
	     (set! *val* (call-external-value             ; type inference hack
			  (fetch (address-after-header (external-value *val*)))
			  *nargs*
			  (pointer-to-stack-arguments)))
	     (remove-stack-arguments (+ *nargs* 1))  ; remove proc and args
	     (loop))
	    (else
             (error "unkown VM return option" option)
	     -1)))))

(define-enumeration return-option
  (exit
   external-call
   native-call
   native-return
   ))

;----------------------------------------------------------------
; Entry point for debugging

; Used by RUN

(define (start-vm thunk)
  (set! *val* thunk)
  (set! *nargs* 0)
  (set! *pending-interrupts* 0)
  (perform-application 0)
  *val*)

; (define (run x) (start-vm (make-closure (compile-form `(halt ,x) e) 'run)))
; (define write-instruction (access-scheme48 'write-instruction))

