;; Rendezvous combinators

;; There's considerable potential for more abstraction in the protocol
;; for primitive rendezvous. --Mike

(define-record-type prim-rv :prim-rv
  (really-make-prim-rv wrap-proc poll-thunk)
  prim-rv?
  (wrap-proc prim-rv-wrap-proc)
  ;; This is a thunk which checks whether the rendezvous is currently
  ;; enabled or not; if it is, the thunk must return an ENABLED
  ;; record, if it isn't, a BLOCKED record.  It must perform only
  ;; provisional operations.
  (poll-thunk prim-rv-poll-thunk))

(define (make-prim-rv poll-thunk)
  (really-make-prim-rv identity poll-thunk))

(define-record-type prim-rv :enabled
  (make-enabled priority do-proc)
  enabled?
  (priority enabled-priority)
  ;; DO-PROC takes as its argument a queue of thread cells where it's
  ;; supposed to enqueue threads to be woken up upon commit.  It must
  ;; perform only provisional operations.
  (do-proc enabled-do-proc))

(define-record-type blocked :blocked
  (make-blocked proc)
  blocked?
  ;; PROC is a procedure with three arguments:
  ;; a TRANS-ID, a CLEANUP-PROC, and a WRAP-PROC.
  
  ;; TRANS-ID is the transaction ID of the blocked thread.  The
  ;; TRANS-ID should be fed, when it's woken up, a pair consisting of
  ;; a return value and a wrap-proc procedure.
  
  ;; CLEANUP-PROC is a procedure which takes a thread queue as an
  ;; argument; it can enqueue thread cells whose threads it wants
  ;; woken up.  This procedure is called from an operation which
  ;; enables this rendezvous.  It must perform only provisional state
  ;; mutations.
  
  ;; WRAP-PROC is the complete, composed-together chain of WRAP
  ;; procedures of the event.
  (proc blocked-proc))

(define-record-type base :base
  (really-make-base prim-rvs)
  base?
  (prim-rvs base-prim-rvs))

(define (make-base poll-thunk)
  (really-make-base (list (make-prim-rv poll-thunk))))

(define-record-type choose :choose
  (make-choose rvs)
  choose?
  (rvs choose-rvs))

(define-record-type guard :guard
  (make-guard thunk)
  guard?
  (thunk guard-thunk))

(define-record-type with-nack :with-nack
  (make-nack proc)
  nack?
  (proc nack-proc))

;; Condition variables for internal use

;; These are essentially for placeholder-like rendezvous, only they
;; don't communicate a value.

(define-synchronized-record-type cvar :cvar
  (really-make-cvar state)
  cvar?
  ;; this can be one of the two below:
  (state cvar-state set-cvar-state!))

(define-synchronized-record-type cvar-unset-state :cvar-unset-state
  (make-cvar-unset-state blocked)
  cvar-unset-state?
  ;; this is a list of :CVAR-ITEM
  (blocked cvar-unset-state-blocked set-cvar-unset-state-blocked!))

(define-record-type cvar-item :cvar-item
  (make-cvar-item trans-id cleanup-proc wrap-proc)
  cvar-item?
  (trans-id cvar-item-trans-id)
  (cleanup-proc cvar-item-cleanup-proc)
  (wrap-proc cvar-item-wrap-proc))

(define-synchronized-record-type cvar-set-state :cvar-set-state
  (make-cvar-set-state priority)
  cvar-set-state?
  (priority cvar-set-state-priority set-cvar-set-state-priority!))

(define (make-cvar)
  (really-make-cvar (make-cvar-unset-state '())))

;; Note that this happens during synchronization which is why the
;; QUEUE argument is there.

(define (cvar-set! cvar queue)
  (let ((state (cvar-state cvar)))
    (cond
     ((cvar-unset-state? state)
      (for-each (lambda (cvar-item)
		  (let ((trans-id (cvar-item-trans-id cvar-item)))
		    (if (not (trans-id-cancelled? trans-id))
			(begin
			  ((cvar-item-cleanup-proc cvar-item) queue)
			  (trans-id-set-value! trans-id
					       (cons (unspecific)
						     (cvar-item-wrap-proc cvar-item)))
			  (enqueue! queue (trans-id-thread-cell trans-id))))))
		(cvar-unset-state-blocked state))
      (set-cvar-state! cvar (make-cvar-set-state 1)))
     (else
      (error "cvar already set")))))

(define (cvar-get-rv cvar)
  (make-base
   (lambda ()
     (let ((state (cvar-state cvar)))
       (cond
	((cvar-set-state? state)
	 (let ((priority (cvar-set-state-priority state)))
	   (set-cvar-set-state-priority! state (+ 1 priority))
	   (make-enabled priority
			 (lambda (queue)
			   (set-cvar-set-state-priority! state 1)
			   (unspecific)))))
	(else
	 (make-blocked
	  (lambda (trans-id cleanup-proc wrap-proc)
	    (set-cvar-unset-state-blocked!
	     state
	     (cons (make-cvar-item trans-id cleanup-proc wrap-proc)
		   (cvar-unset-state-blocked state)))))))))))

(define (always-rv value)
  (make-base
   (lambda ()
     (make-enabled -1
		   (lambda (queue)
		     value)))))

(define never-rv (really-make-base '()))

(define (guard thunk)
  (make-guard thunk))

(define (with-nack proc)
  (make-nack proc))

(define (gather-prim-rvs rev-rvs prim-rvs)
  (cond
   ((null? rev-rvs) (really-make-base prim-rvs))
   ((not (base? (car rev-rvs)))
    (if (null? prim-rvs)
	(gather rev-rvs '())
	(gather rev-rvs (list (really-make-base prim-rvs)))))
   ;; (car rev-rvs) is base
   (else
    (gather-prim-rvs (cdr rev-rvs)
			(append (base-prim-rvs (car rev-rvs))
				prim-rvs)))))

(define (gather rev-rvs rvs)
  (cond
   ((not (null? rev-rvs))
    (let ((rv (car rev-rvs)))
      (cond
       ((choose? rv)
	(gather (cdr rev-rvs) (append (choose-rvs rv) rvs)))
       ((and (base? rv)
	     (not (null? rvs))
	     (base? (car rvs)))
	(gather (cdr rev-rvs)
		(cons (really-make-base (append (base-prim-rvs rv)
						(base-prim-rvs (car rvs))))
		      (cdr rvs))))
       (else
	(gather (cdr rev-rvs) (cons rv rvs))))))
   ((null? (cdr rvs)) (car rvs))
   (else (make-choose rvs))))

(define (choose . rvs)
  (gather-prim-rvs (reverse rvs)  '()))

(define (compose f g)
  (lambda (x)
    (f (g x))))

(define (wrap-prim-rv prim-rv wrap-proc)
  (really-make-prim-rv (compose wrap-proc 
				(prim-rv-wrap-proc prim-rv))
		       (prim-rv-poll-thunk prim-rv)))

(define (wrap rv wrap-proc)
  (cond
   ((base? rv)
    (really-make-base (map (lambda (prim-rv)
			     (wrap-prim-rv prim-rv wrap-proc))
			   (base-prim-rvs rv))))
   ((choose? rv)
    (make-choose (map (lambda (rv)
			(wrap rv wrap-proc))
		      (choose-rvs rv))))
   ((guard? rv)
    (make-guard (lambda ()
		  (wrap ((guard-thunk rv)) wrap-proc))))
   ((nack? rv)
    (make-nack (lambda (nack-rv)
		 (wrap ((nack-proc rv) nack-rv) wrap-proc))))))

(define-record-type base-group :base-group
  (really-make-base-group prim-rvs)
  base-group?
  (prim-rvs base-group-prim-rvs))

(define-record-discloser :base-group
  (lambda (base-group)
    (cons 'base-group 
	  (base-group-prim-rvs base-group))))

(define-record-type choose-group :choose-group
  (make-choose-group groups)
  choose-group?
  (groups choose-group-groups))

(define-record-discloser :choose-group
  (lambda (choose-group)
    (cons 'choose-group 
	  (choose-group-groups choose-group))))

(define-record-type nack-group :nack-group
  (make-nack-group cvar group)
  nack-group?
  (cvar nack-group-cvar)
  (group nack-group-group))

(define-record-discloser :nack-group
  (lambda (nack-group)
    (list 'nack-group
	  (nack-group-group nack-group))))

(define (force-rv rv)
  (cond
   ((base? rv) 
    (really-make-base-group (base-prim-rvs rv)))
   (else
    (really-force-rv rv))))

(define (force-prim-rvs rvs prim-rvs)
  (if (null? rvs)
      (really-make-base-group prim-rvs)
      (let* ((rv (car rvs))
	     (group (really-force-rv rv)))
	(cond
	 ((base-group? group)
	  (force-prim-rvs (cdr rvs)
			  (append (base-group-prim-rvs group)
				  prim-rvs)))
	 ((choose-group? group)
	  (force-rvs (cdr rvs)
		     (append (choose-group-groups group)
			     (list (really-make-base-group prim-rvs)))))
	 (else
	  (force-rvs (cdr rvs)
		     (list group (really-make-base-group prim-rvs))))))))

(define (force-rvs rvs groups)
  (cond
   ((not (null? rvs))
    (let* ((rv (car rvs))
	   (group (really-force-rv rv)))
      (cond
       ((and (base-group? group)
	     (not (null? groups))
	     (base-group? (car groups)))
	(force-rvs (cdr rvs)
		   (cons (really-make-base-group
			  (append (base-group-prim-rvs group)
				  (base-group-prim-rvs (car groups))))
			 (cdr groups))))
       ((choose-group? group)
	(force-rvs (cdr rvs)
		   (append (choose-group-groups group)
			   groups)))
       (else
	(force-rvs (cdr rvs) (cons group groups))))))
   ((null? (cdr groups))
    (car groups))
   (else
    (make-choose-group groups))))

;; this corresponds to force' in Reppy's implementation

(define (really-force-rv rv)
  (cond
   ((guard? rv)
    (really-force-rv ((guard-thunk rv))))
   ((nack? rv)
    (let ((cvar (make-cvar)))
      (make-nack-group cvar
		       (really-force-rv
			((nack-proc rv)
			 (cvar-get-rv cvar))))))
   ((base? rv)
    (really-make-base-group (base-prim-rvs rv)))
   ((choose? rv)
    (force-prim-rvs (choose-rvs rv) '()))))

(define (sync-prim-rv prim-rv)
  (let ((poll-thunk (prim-rv-poll-thunk prim-rv))
	(wrap-proc (prim-rv-wrap-proc prim-rv)))
    (with-new-proposal (lose)
      (let ((status ((prim-rv-poll-thunk prim-rv))))
	(cond
	 ((enabled? status)
	  (let* ((queue (make-queue))
		 (value ((enabled-do-proc status) queue)))
	    (if (maybe-commit-and-make-ready queue)
		(wrap-proc value)
		(lose))))
	 ((blocked? status)
	  (let ((trans-id (make-trans-id)))
	    ((blocked-proc status) trans-id values wrap-proc)
	    (cond
	     ((maybe-commit-and-trans-id-value trans-id)
	      => (lambda (pair)
		   ((cdr pair) (car pair))))
	     (else
	      (lose))))))))))

(define (select-do-proc priority+do-list n)
  (cond
   ((null? (cdr priority+do-list))
    (cdar priority+do-list))
   (else
    (let ((priority
	   (lambda (p)
	     (if (= p -1)
		 n
		 p))))
      (let max ((rest priority+do-list)
		(maximum 0)
		(k 0) ; (length do-procs)
		(do-list '())) ; #### list of pairs do-proc * wrap-proc
	(cond
	 ((not (null? rest))
	  (let* ((pair (car rest))
		 (p (priority (car pair)))
		 (stuff (cdr pair)))
	    (cond 
	     ((> p maximum)
	      (max (cdr rest) p 1 (list stuff)))
	     ((= p maximum)
	      (max (cdr rest) maximum (+ 1 k) (cons stuff do-list)))
	     (else
	      (max (cdr rest) maximum k do-list)))))
	 ((null? (cdr do-list))
	  (car do-list))
	 (else
	  ;; List.nth(doFns, random k)
	  (car do-list))))))))

(define (block)
  (with-new-proposal (lose)
    (maybe-commit-and-block (make-cell (current-thread)))))

(define (sync-prim-rvs prim-rvs)
  (cond
   ((null? prim-rvs)
    (block))
   ((null? (cdr prim-rvs)) (sync-prim-rv (car prim-rvs)))
   (else
    (with-new-proposal (lose)
      (let ()
	(define (find-enabled prim-rvs block-procs wrap-procs)
	  (if (null? prim-rvs)
	      (let ((trans-id (make-trans-id)))
		(for-each (lambda (block-proc wrap-proc)
			    (block-proc trans-id values wrap-proc))
			  block-procs wrap-procs)
		(cond
		 ((maybe-commit-and-trans-id-value trans-id)
		  => (lambda (pair)
		       ((cdr pair) (car pair))))
		 (else
		  (lose))))
	      (let* ((prim-rv (car prim-rvs))
		     (poll-thunk (prim-rv-poll-thunk prim-rv))
		     (wrap-proc (prim-rv-wrap-proc prim-rv))
		     (status (poll-thunk)))
		(cond
		 ((enabled? status)
		  (handle-enabled (cdr prim-rvs)
				  (list
				   (cons (enabled-priority status)
					 (cons (enabled-do-proc status)
					       wrap-proc)))
				  1))
		 ((blocked? status)
		  (find-enabled (cdr prim-rvs)
				(cons (blocked-proc status)
				      block-procs)
				(cons wrap-proc wrap-procs)))))))

	(define (handle-enabled prim-rvs priority+do-list priority)
	  (if (null? prim-rvs)
	      (let* ((stuff (select-do-proc priority+do-list priority))
		     (do-proc (car stuff))
		     (wrap-proc (cdr stuff))
		     (queue (make-queue))
		     (value (do-proc queue)))
		(if (maybe-commit-and-make-ready queue)
		    (wrap-proc value)
		    (lose)))
	      (let* ((prim-rv (car prim-rvs))
		     (poll-thunk (prim-rv-poll-thunk prim-rv))
		     (wrap-proc (prim-rv-wrap-proc prim-rv))
		     (status (poll-thunk)))
		(cond
		 ((enabled? status)
		  (handle-enabled (cdr prim-rvs)
				  (cons (cons (enabled-priority status)
					      (cons (enabled-do-proc status)
						    wrap-proc))
					priority+do-list)
				  (+ 1 priority)))
		 (else
		  (handle-enabled (cdr prim-rvs)
				  priority+do-list
				  priority))))))

	(find-enabled prim-rvs '() '()))))))

(define (sync rv)
  (let ((group (force-rv rv)))
    (cond
     ((base-group? group)
      (sync-prim-rvs (base-group-prim-rvs group)))
     (else
      (sync-group group)))))

(define-record-type ack-flag :ack-flag
  (really-make-ack-flag acked?)
  ack-flag?
  (acked? flag-acked? set-flag-acked?!))

(define (make-ack-flag)
  (really-make-ack-flag #f))

(define (ack-flag! ack-flag)
  (set-flag-acked?! ack-flag #t))

(define-record-type flag-set :flag-set
  (make-flag-set cvar ack-flags)
  flag-set?
  (cvar flag-set-cvar)
  (ack-flags flag-set-ack-flags))

(define (check-cvars! flag-sets queue)
  (for-each (lambda (flag-set)
	      (check-cvar! flag-set queue))
	    flag-sets))

(define (check-cvar! flag-set queue)
  (let loop ((ack-flags (flag-set-ack-flags flag-set)))
    (cond
     ((null? ack-flags)
      (cvar-set! (flag-set-cvar flag-set) queue))
     ((flag-acked? (car ack-flags))
      (values))
     (else
      (loop (cdr ack-flags))))))

;; this corresponds to syncOnGrp from Reppy's code
(define (sync-group group)
  (call-with-values
   (lambda () (collect-group group))
   (lambda (prim-rv+ack-flag-list flag-sets)
     (if (null? (cdr prim-rv+ack-flag-list))
	 (sync-prim-rv (caar prim-rv+ack-flag-list))
	 (really-sync-group prim-rv+ack-flag-list flag-sets)))))

;; This is analogous to SYNC-PRIM-RVS

(define (really-sync-group prim-rv+ack-flag-list flag-sets)

  (with-new-proposal (lose)
    (let ()
      (define (find-enabled prim-rv+ack-flag-list
			    block-proc+ack-flag-list
			    wrap-procs)
	(if (null? prim-rv+ack-flag-list)
	    (let ((trans-id (make-trans-id)))
	      (for-each (lambda (block-proc+ack-flag wrap-proc)
			  (let ((block-proc (car block-proc+ack-flag))
				(ack-flag (cdr block-proc+ack-flag)))
			    (block-proc trans-id
					(lambda (queue)
					  (ack-flag! ack-flag)
					  (check-cvars! flag-sets queue))
					wrap-proc)))
			block-proc+ack-flag-list wrap-procs)
	      (cond
	       ((maybe-commit-and-trans-id-value trans-id)
		=> (lambda (pair)
		     ((cdr pair) (car pair))))
	       (else
		(lose))))
	    (let* ((prim-rv (caar prim-rv+ack-flag-list))
		   (ack-flag (cdar prim-rv+ack-flag-list))
		   (poll-thunk (prim-rv-poll-thunk prim-rv))
		   (wrap-proc (prim-rv-wrap-proc prim-rv))
		   (status (poll-thunk)))
	      (cond
	       ((enabled? status)
		(handle-enabled (cdr prim-rv+ack-flag-list)
				(list
				 (cons (enabled-priority status)
				       (cons (cons (enabled-do-proc status) ack-flag)
					     wrap-proc)))
				1))
	       ((blocked? status)
		(find-enabled (cdr prim-rv+ack-flag-list)
			      (cons (cons (blocked-proc status) ack-flag)
				    block-proc+ack-flag-list)
			      (cons wrap-proc wrap-procs)))))))

      (define (handle-enabled prim-rv+ack-flag-list priority+do-list priority)
	(if (null? prim-rv+ack-flag-list)
	    (let* ((stuff (select-do-proc priority+do-list priority))
		   (more-stuff (car stuff))
		   (do-proc (car more-stuff))
		   (ack-flag (cdr more-stuff))
		   (wrap-proc (cdr stuff))
		   (queue (make-queue)))
	      (ack-flag! ack-flag)
	      (check-cvars! flag-sets queue)
	      (let ((value (do-proc queue)))
		(if (maybe-commit-and-make-ready queue)
		    (wrap-proc value)
		    (lose))))
	    (let* ((prim-rv+ack-flag (car prim-rv+ack-flag-list))
		   (prim-rv (car prim-rv+ack-flag))
		   (ack-flag (cdr prim-rv+ack-flag))
		   (poll-thunk (prim-rv-poll-thunk prim-rv))
		   (wrap-proc (prim-rv-wrap-proc prim-rv))
		   (status (poll-thunk)))
	      (cond
	       ((enabled? status)
		(handle-enabled (cdr prim-rv+ack-flag-list)
				(cons (cons (enabled-priority status)
					    (cons (cons (enabled-do-proc status) ack-flag)
						  wrap-proc))
				      priority+do-list)
				(+ 1 priority)))
	       (else
		(handle-enabled (cdr prim-rv+ack-flag-list)
				priority+do-list
				priority))))))

      (find-enabled prim-rv+ack-flag-list '() '()))))

(define (collect-group group)
  (cond
   ((choose-group? group)
    (gather-choose-group group))
   (else
    (gather-wrapped group '() '()))))

(define (gather-choose-group group)
  (let ((ack-flag (make-ack-flag)))
    (let gather ((group group)
		 (prim-rv+ack-flag-list '())
		 (flag-sets '()))
      (cond
       ((base-group? group)
	(let append ((prim-rvs (base-group-prim-rvs group))
		     (prim-rv+ack-flag-list prim-rv+ack-flag-list))
	  (if (null? prim-rvs)
	      (values prim-rv+ack-flag-list flag-sets)
	      (append (cdr prim-rvs)
		      (cons (cons (car prim-rvs) ack-flag)
			    prim-rv+ack-flag-list)))))
       ((choose-group? group)
	;; fold-left
	(let loop ((groups (choose-group-groups group))
		   (prim-rv+ack-flag-list prim-rv+ack-flag-list)
		   (flag-sets flag-sets))
	  (if (null? groups)
	      (values prim-rv+ack-flag-list flag-sets)
	      (call-with-values
	       (lambda ()
		 (gather (car groups)
			 prim-rv+ack-flag-list
			 flag-sets))
	       (lambda (prim-rv+ack-flag-list flag-sets)
		 (loop (cdr groups)
		       prim-rv+ack-flag-list
		       flag-sets))))))
       ((nack-group? group)
	(gather-wrapped group prim-rv+ack-flag-list flag-sets))))))

(define (gather-wrapped group prim-rv+ack-flag-list flag-sets)
  (call-with-values
   (lambda ()
     (let gather ((group group)
		  (prim-rv+ack-flag-list prim-rv+ack-flag-list)
		  (all-flags '())
		  (flag-sets flag-sets))
       (cond
	((base-group? group)
	 (let append ((prim-rvs (base-group-prim-rvs group))
		      (prim-rv+ack-flag-list prim-rv+ack-flag-list)
		      (all-flags all-flags))
	   (if (null? prim-rvs)
	       (values prim-rv+ack-flag-list
		       all-flags
		       flag-sets)
	       (let ((ack-flag (make-ack-flag)))
		 (append (cdr prim-rvs)
			 (cons (cons (car prim-rvs) ack-flag)
			       prim-rv+ack-flag-list)
			 (cons ack-flag all-flags))))))
	((choose-group? group)
	 ;; fold-left
	 (let loop ((groups (choose-group-groups group))
		    (prim-rv+ack-flag-list prim-rv+ack-flag-list)
		    (all-flags all-flags)
		    (flag-sets flag-sets))
	   (if (null? groups)
	       (values prim-rv+ack-flag-list
		       all-flags
		       flag-sets)
	       (call-with-values
		(lambda ()
		  (gather (car groups)
			  prim-rv+ack-flag-list
			  all-flags
			  flag-sets))
		(lambda (prim-rv+ack-flag-list all-flags flag-sets)
		  (loop (cdr groups)
			prim-rv+ack-flag-list all-flags flag-sets))))))
	((nack-group? group)
	 (call-with-values
	  (lambda ()
	    (gather (nack-group-group group)
		    prim-rv+ack-flag-list
		    '()
		    flag-sets))
	  (lambda (prim-rv+ack-flag-list all-flags-new flag-sets)
	    (values prim-rv+ack-flag-list
		    (append all-flags-new all-flags)
		    (cons (make-flag-set (nack-group-cvar group)
					 all-flags-new)
			  flag-sets))))))))
   (lambda (prim-rv+ack-flag-list all-flags flag-sets)
     (values prim-rv+ack-flag-list flag-sets))))


(define (select . rvs)
  (sync (apply choose rvs)))
