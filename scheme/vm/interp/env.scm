; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


(define *env*)

(define (current-env) *env*)
(define (set-current-env! env) (set! *env* env))

; Access to environment slots

(define env-ref  vm-vector-ref)
(define env-set! vm-vector-set!)
(define (env-parent env) (env-ref env 0))
(define (set-env-parent! env x) (env-set! env 0 x))

(define (env-back env back)  ;Resembles NTHCDR
  (do ((env env (env-parent env))
       (i back (- i 1)))
      ((= i 0) env)))

; Making new environments

(define (pop-args-into-env count)
  (push *env*)
  (push (make-header (enum stob vector) (cells->bytes (+ count 1))))
  (add-env-stats count)
  (set! *env* (address->stob-descriptor (address1+ *stack*))))

(define (stack-loc s)
  (- (address->integer *stack-end*) s))

; Alternative method for making environments - put the values into the heap.

(define (heap-env-space count)
  (+ stob-overhead (+ count 1)))  ; includes superior environment

(define (pop-args-into-heap-env count key)
  (let ((stob (make-d-vector (enum stob vector) (+ count 1) key)))
    (copy-memory! *stack*
		  (address+ (address-after-header stob)
			    (cells->a-units 1))
		  (cells->bytes count))
    (add-cells-to-stack! (- count))
    (vm-vector-set! stob 0 *env*)
    (set! *env* stob)))

; Migrate the current environment to the heap.  Used when creating a closure.
; CURRENT-ENV-SIZE size is conservative.

(define (current-env-size)
  (if (within-stack? *env*)
      (stack-size)
      0))

; This is what the interpreter calls when it needs to put the current
; environment in a closure.

(define (preserve-current-env key)
  (preserve-current-env-with-reason key (enum copy closure)))

(define (preserve-current-env-with-reason key reason)
  (if (within-stack? *env*)
      (set! *env* (save-env-in-heap *env* *cont* key reason)))
  *env*)

; 1) Copy ENV and its ancestors into heap, adding forwarding pointers
; 2) Go down the continuation chain updating the env pointers
;
; This code depends on continuation-cont pointers not crossing environment
; parent pointers on the stack.

(define (save-env-in-heap env cont key reason)
  (let ((top (copy-env env key reason)))
    (let loop ((env top))
      (cond ((within-stack? (env-parent env))
             (let ((new (copy-env (env-parent env) key reason)))
               (set-env-parent! env new)
               (loop new)))))
    (let loop ((cont cont))
      (let ((env (stack-cont-env cont)))
        (cond ((and (stob? env)
                    (stob? (stob-header env)))
               (set-stack-cont-env! cont (stob-header env))
               (loop (integer->address (stack-cont-cont cont)))))))
    top))

; ARGUMENTS-ON-STACK needs to walk down the stack and find the end of the
; current arguments.  It looks for headers, which we clobber with forwarding
; pointers, so we put a marker in the first slot of the environment and
; ARGUMENTS-ON-STACK knows to back up one if it finds the marker.
; (Putting the forwarding pointer in the first slot doesn't work, because
;  we can't distinguish between it and a normal first slot.)

(define (copy-env env key reason)
  (let ((new (header+contents->stob (stob-header env)
				    (address-after-header env)
				    key)))
    (add-copy-env-stats env reason)
    (vm-vector-set! env 0 argument-limit-marker)
    (stob-header-set! env new)
    new))
