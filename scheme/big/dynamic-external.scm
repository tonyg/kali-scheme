; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; More high-level interface to dynamic loading:

;; This automatically initializes an externals shared object, keeps
;; track of which shared objects are loaded, and prevents them from
;; being removed automatically by the GC.

;; The shared object must define two functions
;; void s48_on_load(void);
;; and
;; void s48_on_reload(void);
;; (which typically do the same thing) that LOAD-DYNAMIC-EXTERNALS
;; calls, depending on whether the object is being loaded for the
;; first time or not.

(define-record-type dynamic-externals :dynamic-externals
  (make-dynamic-externals shared-object
			  reload-on-repeat?
			  reload-on-resume?)
  dynamic-externals?
  (shared-object dynamic-externals-shared-object
		 set-dynamic-externals-shared-object!)
  (reload-on-repeat? dynamic-externals-reload-on-repeat?)
  (reload-on-resume? dynamic-externals-reload-on-resume?))

(define *the-dynamic-externals-table* '())

(define (find-dynamic-externals name)
  (any (lambda (dynamic-externals)
	 (string=? name
		   (shared-object-name
		    (dynamic-externals-shared-object
		     dynamic-externals))))
       *the-dynamic-externals-table*))

;; returns the DYNAMIC-EXTERNALS object
(define (load-dynamic-externals name reload-on-repeat? reload-on-resume?)
  (cond
   ((find-dynamic-externals name)
    => (lambda (dynamic-externals)
	 ;; Should we respect the original settings for
	 ;; RELOAD-ON-REPEAT? and RELOAD-ON-RESUME? or the new ones?
	 ;; We assume they're always the same.  We should probably
	 ;; verify.
	 (if reload-on-repeat?
	     (reload-dynamic-externals-internal dynamic-externals "s48_on_reload"))
	 dynamic-externals))
   (else
    (let* ((shared-object (open-shared-object name))
	   (dynamic-externals (make-dynamic-externals shared-object
						      reload-on-repeat?
						      reload-on-resume?)))
      (set! *the-dynamic-externals-table*
	    (cons dynamic-externals
		  *the-dynamic-externals-table*))
      (call-shared-object-address
       (shared-object-address shared-object "s48_on_load"))
      dynamic-externals))))

(define (reload-dynamic-externals-internal dynamic-externals close-first? init-name)
  (let* ((old-shared-object (dynamic-externals-shared-object dynamic-externals))
	 (name (shared-object-name old-shared-object)))
    (if close-first?
	(close-shared-object old-shared-object))
    (let ((shared-object (open-shared-object name)))
      (set-dynamic-externals-shared-object! dynamic-externals
					    shared-object)
      (call-shared-object-address
       (shared-object-address shared-object init-name)))))

;; for interactive usage
(define (reload-dynamic-externals name)
  (cond
   ((find-dynamic-externals name) =>
    (lambda (dynamic-externals)
      (reload-dynamic-externals-internal dynamic-externals #t "s48_on_reload")))
   (else
    (error "trying to load dynamic externals that were never loaded" name))))

;; most common usage, when a Scheme package requires C externals to work
(define (import-dynamic-externals name)
  (load-dynamic-externals name #f #t))

(define-reinitializer dynamic-externals-reinitializer
  (lambda ()
    (set! *the-dynamic-externals-table*
	  (delete (lambda (dynamic-externals)
		    (not (dynamic-externals-reload-on-resume? dynamic-externals)))
		  *the-dynamic-externals-table*))
    (for-each (lambda (dynamic-externals)
		(reload-dynamic-externals-internal dynamic-externals #f "s48_on_load"))
	      *the-dynamic-externals-table*)))

;; note this leaves the shared bindings in place.
(define (unload-dynamic-externals dynamic-externals)
  (set! *the-dynamic-externals-table*
	(delq dynamic-externals *the-dynamic-externals-table*))
  (close-shared-object (dynamic-externals-shared-object dynamic-externals)))
