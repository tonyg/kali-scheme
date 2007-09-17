; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; More high-level interface to dynamic loading:

;; This automatically initializes an externals shared object, keeps
;; track of which shared objects are loaded, and prevents them from
;; being removed automatically by the GC.

;; The shared object must define a function
;; void s48_on_load(void);
;; It can also define functiosn:
;; void s48_on_unload(void);
;;   which is called just before unloading, and
;; void s48_on_reload(void);
;;   which is called after reloading.
;; (which typically do the same thing) that LOAD-DYNAMIC-EXTERNALS
;; calls, depending on whether the object is being loaded for the
;; first time or not.

(define-record-type dynamic-externals :dynamic-externals
  (make-dynamic-externals shared-object
			  complete-name?
			  reload-on-repeat?
			  reload-on-resume?)
  dynamic-externals?
  (shared-object dynamic-externals-shared-object
		 set-dynamic-externals-shared-object!)
  (complete-name? dynamic-externals-complete-name?)
  (reload-on-repeat? dynamic-externals-reload-on-repeat?)
  (reload-on-resume? dynamic-externals-reload-on-resume?))

(define *the-dynamic-externals-table* '())

(define (find-dynamic-externals name)
  (let ((real-name (translate name)))
    (any (lambda (dynamic-externals)
	   (string=? real-name
		     (shared-object-name
		      (dynamic-externals-shared-object
		       dynamic-externals))))
	 *the-dynamic-externals-table*)))

;; returns the DYNAMIC-EXTERNALS object
(define (load-dynamic-externals name complete-name?
				reload-on-repeat? reload-on-resume?)
  (cond
   ((find-dynamic-externals name)
    => (lambda (dynamic-externals)
	 ;; Should we respect the original settings for
	 ;; RELOAD-ON-REPEAT? and RELOAD-ON-RESUME? or the new ones?
	 ;; We assume they're always the same.  We should probably
	 ;; verify.
	 (if reload-on-repeat?
	     (reload-dynamic-externals-internal dynamic-externals #t))
	 dynamic-externals))
   (else
    (let* ((shared-object (open-shared-object (translate name) complete-name?))
	   (dynamic-externals (make-dynamic-externals shared-object
						      complete-name?
						      reload-on-repeat?
						      reload-on-resume?)))
      (set! *the-dynamic-externals-table*
	    (cons dynamic-externals
		  *the-dynamic-externals-table*))
      (call-shared-object-address
       (shared-object-address shared-object "s48_on_load"))
      dynamic-externals))))

(define (reload-dynamic-externals-internal dynamic-externals reload?)
  (let* ((old-shared-object (dynamic-externals-shared-object dynamic-externals))
	 (name (shared-object-name old-shared-object)))
    (if reload?
	(unload-shared-object dynamic-externals)) 
    (let ((shared-object
	   (open-shared-object (translate name)
			       (dynamic-externals-complete-name? dynamic-externals))))
      (set-dynamic-externals-shared-object! dynamic-externals shared-object)
      (cond
       ((not reload?)
	(shared-object-address shared-object "s48_on_load"))
       ((shared-object-address shared-object "s48_on_reload")
	=> call-shared-object-address)
       (else (shared-object-address shared-object "s48_on_load"))))))

;; for interactive usage
(define (reload-dynamic-externals name)
  (cond
   ((find-dynamic-externals name) =>
    (lambda (dynamic-externals)
      (reload-dynamic-externals-internal dynamic-externals #t)))
   (else
    (assertion-violation 'name
			 "trying to reload dynamic externals that were never loaded"
			 name))))

;; most common usage, when a Scheme package requires C externals to work
(define (import-dynamic-externals name)
  (load-dynamic-externals name #t #f #t))

;; We can't do this via a reinitializer, because the reinitializer
;; will typically call external C code, which is typically in a shared
;; library.  So we need to load the shared libraries before we run any
;; reinitializers.

(add-initialization-thunk!
 (lambda ()
   (set! *the-dynamic-externals-table*
	 (delete (lambda (dynamic-externals)
		   (not (dynamic-externals-reload-on-resume? dynamic-externals)))
		 *the-dynamic-externals-table*))
   (for-each (lambda (dynamic-externals)
	       (reload-dynamic-externals-internal dynamic-externals #f))
	     *the-dynamic-externals-table*)))

;; note this leaves the shared bindings in place.
(define (unload-dynamic-externals dynamic-externals)
  (set! *the-dynamic-externals-table*
	(delq dynamic-externals *the-dynamic-externals-table*))
  (unload-shared-object dynamic-externals))

(define (unload-shared-object dynamic-externals)
  (let ((shared-object (dynamic-externals-shared-object dynamic-externals)))
    (cond
     ((shared-object-address-or-false shared-object "s48_on_unload")
      => call-shared-object-address))
    (close-shared-object shared-object)))
  
			      