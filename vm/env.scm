
(define *env* (unassigned))

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

; How much heap space we will need.
(define (stack-env-space count)
  (+ 2 count))

(define stob/vector (enum stob vector))

(define (pop-args-into-env count key)
  (check-stack-cons (stack-env-space count) key)
  (push *env*)
  (push (make-header stob/vector (cells->bytes (+ count 1))))
  (add-env-stats count)
  (set! *env* (address->stob-descriptor (addr1+ (addr1+ *stack*)))))

; Alternative method for making environments - put the values into the heap.

(define (heap-env-space count)
  (+ count 2))  ; header + superior environment

(define (pop-args-into-heap-env count key)
  (let ((stob (make-d-vector stob/vector (+ count 1) key)))
    (copy-cells! (addr1+ *stack*)
		 (addr+ (cells->a-units 1)
			(address-after-header stob))
		 count)
    (stack-add (- 0 count))
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
  (preserve-current-env-with-reason key copy/closure))

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
      (let ((env (continuation-env cont)))
        (cond ((and (stob? env)
                    (stob? (stob-header env)))
               (set-continuation-env! cont (stob-header env))
               (loop (continuation-cont cont))))))
    top))

(define (copy-env env key reason)
  (let ((new (copy-stob env key)))
    (add-copy-env-stats env reason)
    (stob-header-set! env new)
    new))
