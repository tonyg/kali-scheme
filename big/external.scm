; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; Code for keeping external pointers in a table similar to the symbol table.
;
; The entry points for this code are:
;
; (GET-EXTERNAL string)    returns an external pointer
; (LOOKUP-ALL-EXTERNALS)   looks up new values for all external pointers;
;                          ideally this should be called automatically
;                          on startup


(define *the-external-table* #f)

(define (flush-the-external-table!)
  (set! *the-external-table* #f))

(define (restore-the-external-table!)
  (set! *the-external-table* (make-table))
  (vector-for-each (lambda (external)
		     (table-set! *the-external-table*
				 (external-name external)
				 external))
		   (find-all-xs (enum stob external))))

(define (gc-externals)
  (flush-the-external-table!)
  (collect)
  (restore-the-external-table!))

(define (vector-for-each proc vector)
  (do ((i 0 (+ i 1)))
      ((>= i (vector-length vector))
       (unspecific))
    (proc (vector-ref vector i))))

(restore-the-external-table!)

;------------------------------------------------------------

(define (get-external name)
  (cond ((table-ref *the-external-table* name)
	 => (lambda (x) x))
	(else
	 (let ((new (maybe-external-lookup
		     (make-external name (make-code-vector 4 0)))))
	   (if new
	       (table-set! *the-external-table* name new)
	       (warn "External not found" name))
	   new))))

(define (maybe-external-lookup external)
  (call-with-current-continuation
    (lambda (lose)
      (with-handler
	  (lambda (c punt)
	    (cond ((or (not (exception? c))
		       (not (= op/external-lookup (exception-opcode c))))
		   (punt))
		  (else
		   (lose #f))))
	(lambda ()
	  (external-lookup external)
	  external)))))

(define op/external-lookup (enum op external-lookup))

(define (null-terminate str)
  ;; No longer necessary
  (string-append str (string (ascii->char 0))))

;------------------------------------------------------------

(define (lookup-all-externals)
  (cond ((try-to-lookup-all-externals)
	 #t)
	(else
	 (display "GCing to try to remove unbound externals")
	 (newline)
	 (gc-externals)
	 (really-lookup-all-externals))))

; Quietly look up all externals, returning #F if unsuccessful

(define (try-to-lookup-all-externals)
  (call-with-current-continuation
    (lambda (k)
      (lookup-all-externals-with-handler
       (lambda (external)
	 (k #f)))
      #t)))

; Look up all externals, printing out the names of those that cannot
; be found.

(define (really-lookup-all-externals)
  (let ((okay? #t))
    (lookup-all-externals-with-handler
     (lambda (external)
       (cond (okay?
	      (display "Remaining unbound external(s):")
	      (newline)
	      (set! okay? #f)))
       (display " ")
       (display (external-name external))
       (newline)))
    okay?))

; Look up all externals, calling PROC on any that cannot be found.
; This assumes that not finding a value for the name is the only reason why
; op/external-lookup would fail, which isn't quite true.  Other possible
; reasons are that the name is not a string, or the value is not a
; code vector.

(define (lookup-all-externals-with-handler proc)
  (with-handler
   (lambda (c punt)
     (if (or (not (exception? c))
	     (not (= op/external-lookup (exception-opcode c))))
	 (punt)
	 (proc (car (exception-arguments c)))))
   (lambda ()
     (table-walk (lambda (name external)
		   (external-lookup external))
		 *the-external-table*))))


