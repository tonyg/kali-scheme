;; This replaces trans-id REF in Reppy's code

(define-synchronized-record-type trans-id :trans-id
  (really-make-trans-id thread-cell value)
  (value)
  trans-id?
  (thread-cell trans-id-thread-cell)
  (value trans-id-value set-trans-id-value!))
   
(define (make-trans-id)
  (really-make-trans-id (make-cell (current-thread))
			#f))

(define (maybe-commit-and-trans-id-value trans-id)
  (cond
   ((trans-id-value trans-id)
    => (lambda (value)
	 (and (maybe-commit)
	      value)))
   (else
    (and (maybe-commit-and-block (trans-id-thread-cell trans-id))
	 (trans-id-value trans-id)))))

(define (trans-id-set-value! trans-id value)
  (cond
   ((not value) (error "trans-id value can't be #f" trans-id value))
   ((trans-id-value trans-id)
    (error "trans-id is already assigned"))
   (else
    (set-trans-id-value! trans-id value))))

(define (trans-id-cancelled? trans-id)
  (and (trans-id-value trans-id) #t))
