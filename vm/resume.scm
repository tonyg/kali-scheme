; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

; This is file resume.scm.

; All sizes are in cells

(define (required-init-space startup-vector startup-vector-length)
  (+ (+ (do ((i 0 (+ i 1))
	     (s 0 (+ s (vm-string-size
			(string-length
			 (vector-ref startup-vector i))))))
	    ((>= i startup-vector-length)
	     s))
	(code-vector-size 2))
     (+ initial-interpreter-heap-space
	(+ initial-stack-heap-space
	   initial-i/o-heap-space))))

(define (initialize-vm memory-begin memory-size stack-begin stack-size)
  (initialize-heap memory-begin memory-size)
  (initialize-i/o-system)
  (initialize-stack stack-begin stack-size)
  (initialize-interpreter))

(define (call-startup-procedure startup-proc
				startup-vector
				startup-vector-length)
  (let ((vector (vm-make-vector startup-vector-length universal-key)))
    (do ((i 0 (+ i 1)))
	((>= i startup-vector-length))
      (vm-vector-set! vector i (enter-string (vector-ref startup-vector i))))
    (clear-registers)
    (push vector)
    (push (initial-input-port))
    (push (initial-output-port))
    (let ((code (make-code-vector 2 universal-key)))
      (code-vector-set! code 0 (enum op call))
      (code-vector-set! code 1 3)         ; nargs    
      (set! *code-pointer* (address-after-header code)))
    (restart startup-proc)))

(define (restart value)
  (set! *val* value)
  (let loop ()
    (let ((option (interpret)))
      (cond ((= option (enum return-option exit))
	     *val*)
	    ((= option (enum return-option external-call))
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

