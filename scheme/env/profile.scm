; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Profiling

; NOTE: the sampling rate is set at the beginning of each run.  Different
; machines and loadings will produce different tick rates.

(define (profile command)
  (let ((thunk (if (eq? (car command) 'run)
		   (evaluate `(lambda () ,(cadr command))
			     (environment-for-commands))
		   (lambda () (execute-command command)))))
    (call-with-values
     (lambda () (run-with-profiling thunk (command-output)))
     (lambda (results hits running on-stack)
       (let ((port (command-output)))
	 (display hits port)
	 (display " samples" port)
	 (newline port)
	 (display "Running:" port)
	 (newline port)
	 (display-counts running hits - port)
	 (display "Waiting:" port)
	 (newline port)
	 (display-counts on-stack hits (lambda (total next) next) port)
	 (set-focus-values! results))))))

(define-command-syntax 'profile "<command>" "profile execution"
  '(command))

(define (display-counts counts hits combine port)
  (let ((limit (quotient hits 10)))
    (do ((counts counts (cdr counts))
	 (total hits (combine total (cdar counts))))
	((or (<= total limit)
	     (null? counts)
	     (<= (cdar counts) 1)))
      (display " " port)
      (display (cdar counts) port)
      (display " " port)
      (display (caar counts) port)
      (newline port))))

; Strategy:
; Request periodic interrupts.
; At each interrupt save the current (raw) continuation.
; Either at the end of the run or every so many interrupts, stop the
; timer interrupts and walk the continuations adding the templates to
; a table, with a count of how many times each has been seen.

(define (run-with-profiling thunk port)
  (calculate-tick-rate! port)
  (call-with-values
   (lambda ()
     (dynamic-wind
      (lambda ()
	(vector-set! interrupt-handlers
		     (enum interrupt alarm)
		     handle-timer-interrupt)
	(start-periodic-interrupts!))
      (lambda ()
	(primitive-cwcc
	 (lambda (top)
	   (set! *top-continuation* (continuation-parent top))
	   (set! *hits* 0)
	   (set! *conts* '())
	   (set! *templates* '())
	   (set! *template-counts* (make-template-table))
	   (set! *cont-counts* (make-template-table))
	   (set! *cont-count* cont-limit)
	   (thunk))))
      reset-timer-interrupts!))
   (lambda results
     (for-each add-cont-data! *conts*)
     (let ((templates (gather-template-table-data *template-counts*))
	   (conts (gather-template-table-data *cont-counts*)))
       (set! *top-continuation* #f)  ; drop pointer
       (set! *conts* '())
       (values results *hits* templates conts)))))

(define *quantum-mantissa* #f)
(define *quantum-exponent* #f)

; For checking how fast the machine is.

(define (fib x)
  (if (< x 2)
      1
      (+ (fib (- x 1)) (fib (- x 2)))))

(define (calculate-tick-rate! port)
  (let ((start-time (run-time)))
    (fib 17)  ; chosen more or less at random.
    (let ((end-time (run-time)))
      (set! *quantum-mantissa* (quotient (- end-time start-time) 4))
      (set! *quantum-exponent* (tick-exponent))
      (display (round (/ (* *quantum-mantissa* (expt 10 *quantum-exponent*)))) port)
      (display " ticks per second" port)
      (newline port))))

(define (start-periodic-interrupts!)
  (schedule-interrupt *quantum-mantissa* *quantum-exponent* #t))

(define (stop-periodic-interrupts!)
  (schedule-interrupt 0 0 #f))

(define cont-limit 100)

(define *cont-count* cont-limit)

(define (handle-timer-interrupt template ei)
  (set! *cont-count* (- *cont-count* 1))
  (if (= 0 *cont-count*)
      (begin
	(stop-periodic-interrupts!)
	(for-each add-template-data! *templates*)
	(for-each add-cont-data! *conts*)
	(set! *cont-count* cont-limit)
	(set! *templates* '())
	(set! *conts* '())
	(start-periodic-interrupts!)))
  (set! *templates* (cons template *templates*))
  (set! *hits* (+ *hits* 1))
  (primitive-cwcc (lambda (cont)
		    (set! *conts* (cons cont *conts*)))))

(define *top-continuation* #f)
(define *conts* '())
(define *templates* '())
(define *hits* 0)

(define make-template-table (make-table-maker eq? template-id))

(define *template-counts* (make-template-table))
(define *cont-counts* (make-template-table))

(define (okay-cont? cont)
  (and cont (not (eq? cont *top-continuation*))))

(define (add-template-data! template)
  (let ((p (table-ref *template-counts* template)))
    (if (not p)
	(table-set! *template-counts*
		    template
		    (cons 1 '()))
	(set-car! p (+ (car p) 1)))))

(define (add-cont-data! cont)
  (let loop ((cont (continuation-parent cont)))
    (if (and (okay-cont? cont)
	     (okay-cont? (continuation-parent cont)))
	(let* ((template (continuation-template cont))
	       (p (table-ref *cont-counts* template)))
	  (if (not p)
	      (table-set! *cont-counts*
			  template
			  (cons 1 '()))
	      (set-car! p (+ (car p) 1)))
	  (loop (continuation-parent cont))))))

(define (gather-template-table-data table)
  (let ((counts '()))
    (table-walk (lambda (template p)
		  (set! counts
			(cons (cons (debug-data-names
				     (template-debug-data template))
				    (car p))
			      counts)))
		table)
    (sort-list counts
	       (lambda (p1 p2)
		 (>= (cdr p1) (cdr p2))))))
