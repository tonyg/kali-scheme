; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Code for keeping external pointers in a table similar to the symbol table.
;
; The entry points for this code are:
;
; (GET-EXTERNAL string)    returns an external pointer
; (LOOKUP-ALL-EXTERNALS)   looks up new values for all external pointers;
;                          ideally this should be called automatically
;                          on startup
;
; (DYNAMIC-LOAD filename)  loads the file into the current image
;
; The following used to be in the PRIMITIVES interface when externals were
; a primitive data type.
;
; external?
; external-name
; external-value
; (external-lookup external)
; (call-external external . args)

; New, non-primitive data type.

(define-record-type external :external
  (make-external name value)
  external?
  (name external-name)
  (value external-value))

; Table of externals.

(define *the-external-table* #f)

(define (flush-the-external-table!)
  (set! *the-external-table* #f))

(define (restore-the-external-table!)
  (set! *the-external-table* (make-string-table))
  (vector-for-each (lambda (external)
		     (table-set! *the-external-table*
				 (external-name external)
				 external))
		   (find-all-records :external)))

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

;----------------
; Making new externals.

(define-string/bytes-type dynamic-external-name :dynamic-external-name
  dynamic-external-name?
  
  string-encoding-length encode-string
  string-decoding-length decode-string

  thing->dynamic-external-name
  string->dynamic-external-name
  byte-vector->dynamic-external-name
  
  dynamic-external-name->string
  dynamic-external-name->byte-vector dynamic-external-name->byte-string)

(define (get-external name)
  (cond ((table-ref *the-external-table* name)
	 => (lambda (x) x))
	(else
	 (let* ((value (make-code-vector 4 0))
		(new (make-external name value)))
	   (table-set! *the-external-table* name new)
	   (if (not (external-lookup (dynamic-external-name->byte-string
				      (thing->dynamic-external-name name))
				     value))
	       (warn "External not found" name))
	   new))))

(import-lambda-definition external-lookup (name value) "s48_external_lookup")

; No longer necessary, as all strings now end with nulls.

(define (null-terminate str)
  str)

;----------------
; Re-lookup all externals.  This needs to be done whenever we resume an image.

(define (lookup-all-externals)
  (cond ((null? (really-lookup-all-externals))
	 #t)
	(else
	 (display "GCing to try to remove unbound externals")
	 (newline)
	 (gc-externals)
	 (let ((losers (really-lookup-all-externals)))
	   (if (null? losers)
	       #t
	       (let ((out (current-error-port)))
		 (display "Unbound external(s):" out)
		 (for-each (lambda (name)
			     (write-char #\space out)
			     (display name out))
			   losers)
		 #f))))))

; Re-lookup one external.

(define (lookup-external external)
  (external-lookup (external-name external)
		   (external-value external)))

; Quietly look up all externals, returning #F if unsuccessful

(define (try-to-lookup-all-externals)
  (null? (really-lookup-all-externals)))

; Look up all externals, returning a list of the names of those that cannot
; be found.

(define (really-lookup-all-externals)
  (let ((losers '()))
    (table-walk (lambda (name external)
		  (if (not (external-lookup name (external-value external)))
		      (set! losers (cons name losers))))
		*the-external-table*)
    losers))

;----------------

(define (call-external external . args)
  (apply call-external-value (external-value external)
	                     (external-name external)
			     args))

;----------------

(define (dynamic-load name)
  (external-dynamic-load (thing->file-name-byte-string name)))

(import-lambda-definition external-dynamic-load (filename) "s48_dynamic_load")
