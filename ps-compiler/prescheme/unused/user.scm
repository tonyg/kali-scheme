; Copyright (c) 1994 Richard Kelsey.  See file COPYING.


(define *break-on-user-errors?* #f)

(define (user-error node string . args)
  (format #t "~%PreScheme error: ")
  (apply format #t string args)
  (newline)
  (if node (display-source node))
  (if *break-on-user-errors?*
      (breakpoint "")))

(define (display-source node)
  (let ((source-node (find-some-source node)))
    (if source-node
	(display-annotation (node-source source-node)
			    (eq? source-node node)
			    node)
	(pp-cps (top-call node)))))

(define (top-call node)
  (let loop ((node node))
    (if (or (not (node? (node-parent node)))
	    (and (call-node? node)
		 (not (call-node? (node-parent node)))))
	node
	(loop (node-parent node)))))

(define (node-source node)
  (cond ((call-node? node)
	 (call-source node))
	((lambda-node? node)
	 (lambda-source node))
	(else
	 #f)))

(define (find-some-source node)
  (let loop ((node node))
    (cond ((not (node? node))
	   #f)
	  ((annotation? (node-source node))
	   node)
	  (else (loop (node-parent node))))))

(define (display-annotation annotation exact? node)
  (let ((file   (annotation-file annotation))
	(form   (annotation-form annotation))
	(row    (annotation-row annotation))
	(column (annotation-column annotation)))
    (newline)
    (cond (exact?
	   (display "  "))
	  (else
	   (write-one-line (current-output-port)
			   60
			   (lambda (port)
			     (show-user-node node port)))
	   (display "  within ")))
    (newline)
    (write-one-line (current-output-port)
		    70
		    (lambda (port) (display form port)))
    (format #t "~%    on line ~D of ~A~%" column file)))

(define (show-user-node node port)
  (cond ((literal-node? node)
	 (format port "'~S" (literal-value node)))
	((reference-node? node)
	 (format port "~S" (variable-name (reference-variable node))))
	((lambda-node? node)
	 (format port "(lambda ~A ...)"
		 (map variable-name
		      (if (proc-lambda? node)
			  (cdr (lambda-variables node))
			  (lambda-variables node))))
	 (show-user-node (lambda-body node) port))
	((call-node? node)
	 (show-user-call-node node port))))

(define (show-user-call-node call port)
  (let ((exits (call-exits call))
	(id (primop-id (call-primop call))))
    (case id
      ((let)
       (display "(let ..." port)	; wimpy
       )
      ((call unknown-call)
       (display "(" port)
       (show-user-node (call-arg call 1) port)
       (show-user-call-args call (if (eq? 'call id) 2 3) port)
       (display ")" port))
      (else				; wimpy
       (format port "(~S" id)
       (show-user-call-args call exits port)
       (format port ")")))))

(define (show-user-call-args call start port)
  (do ((i start (+ i 1)))
      ((>= i (call-arg-count call)))
    (display " " port)
    (show-user-node (call-arg call i) port)))
       
      