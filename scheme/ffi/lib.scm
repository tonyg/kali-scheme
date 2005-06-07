(define-exported-binding "string->symbol" string->symbol)

(define-record-type ffi-lib :ffi-lib
  (make-ffi-lib handle name obj-hash)
  ffi-lib?
  (handle    ffi-lib-handle)
  (name      ffi-lib-name)
  (obj-hash  ffi-lib-obj-hash))

(define-record-discloser :ffi-lib
  (lambda (lib)
    (list 'ffi-lib (ffi-lib-name lib))))

(define *lib-suffix* ".so")
(define *ffi-libs* (make-string-table))

(define (cached-ffi-lib name no-error?)
  (let ((lib (table-ref *ffi-libs* name)))
    (or lib
        (let ((handle (call-imported-binding ffi-make-lib name no-error?)))
          (if handle
              (let ((lib (make-ffi-lib handle name (make-string-table))))
                ;;(add-finalizer! lib (lambda (lib) (ffi-free-lib (ffi-lib-handle lib))))
                (table-set! *ffi-libs* name lib)
                lib)
              #f)))))

(define ffi-lib
  (case-lambda
    ((name) (ffi-lib name ""))
    ((name version)
     (cond
      ((ffi-lib? name)
       name)
      ((not name)
       (cached-ffi-lib #f #f)) ;; #f -> NULL -> open this executable
      (else
       (let* ((version (if (pair? version)
                           (string-append "." (car version))
                           ""))
              (name+version (if (string-suffix? *lib-suffix* name)
                                (string-append name version)
                                (string-append name *lib-suffix* version))))
         (or (cached-ffi-lib name+version #t) ; try extended name first
             ;; fallback on original
             (cached-ffi-lib name #f))))))))

(import-definition ffi-make-lib)
(import-definition ffi-free-lib)
