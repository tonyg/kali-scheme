(define-record-type ffi-obj :ffi-obj
  (make-ffi-obj handle lib name)
  ffi-obj?
  (handle ffi-obj-handle)
  (lib    ffi-obj-lib)
  (name   ffi-obj-name))

(define-record-discloser :ffi-obj
  (lambda (obj)
    (list 'ffi-obj (ffi-obj-lib obj) (ffi-obj-name obj)
          (pointer-value (ffi-obj-handle obj)))))

(define (ffi-obj name lib-or-libname)
  (let ((lib (if (string? lib-or-libname)
                 (ffi-lib lib-or-libname)
                 lib-or-libname)))
    (let ((obj (table-ref (ffi-lib-obj-hash lib) name)))
      (or obj
          (let ((obj (make-ffi-obj
                      (call-imported-binding ffi-make-obj
                                             (ffi-lib-handle lib) name) lib name)))
            (table-set! (ffi-lib-obj-hash lib) name obj)
            obj)))))

;; These internal functions provide the functionality to be used by
;; get-ffi-obj, set-ffi-obj! and define-c below
(define (ffi-get ffi-obj type)
  ;;(debug-pointer `("primitive" ,type) (ctype-get-primitive type))
  (c->scheme (call-imported-binding ffi-ptr-ref
                                    (ffi-obj-handle ffi-obj)
                                    (ctype-get-primitive type))
             type))

(define (object->ffi-obj-name obj)
  (cond ((symbol? obj) (object->ffi-obj-name (symbol->string obj)))
        ((string? obj) obj)
        (else
         (error "object name of invalid type" obj))))

(define get-ffi-obj
  (case-lambda
    ((name lib type) (get-ffi-obj name lib type #f))
    ((name lib type failure)
     (let ((name (object->ffi-obj-name name))
           (lib (ffi-lib lib)))
       (let-values (((obj error?)
                     (call-with-current-continuation
                      (lambda (k)
                        (with-handler
                            (lambda (c punt)
                              (if failure (k (failure) #t) (punt c)))
                          (lambda ()
                            (k (ffi-obj name lib) #f)))))))
         (for-each display `("ffi-obj: " ,obj #\newline))
         (if error? obj (ffi-get obj type)))))))

(import-definition ffi-make-obj)
