(define (cpointer? thing)
  (call-imported-binding ffi-is-ptr thing))

(define (malloc count-or-type . args)
  (let* ((type (or (and (ctype? count-or-type) count-or-type)
                   (and (pair? args) (ctype? (car args)) (car args))))
         (count (or (and (integer? count-or-type) count-or-type)
                    (and (pair? args) (integer? (car args)) (car args))))
         (rest-args (if (and type count) (cdr args) args))
         (cptr (and (pair? rest-args) (cpointer? (car rest-args)) (car rest-args)))
         (raw? (and (pair? rest-args)
                    (let ((sym (or (symbol? (car rest-args))
                                   (and (pair? (cdr rest-args))
                                        (symbol? (cadr rest-args))
                                        (cadr rest-args)))))
                      (eq? sym 'raw)))))
    (for-each display `("type: " ,(and type (ctype-get-primitive type)) ", "
                        "count: " ,count ", cptr: " ,cptr ", raw: " ,raw?
                        #\newline))
    (call-imported-binding ffi-malloc
                           (and type (ctype-get-primitive type))
                           (or count 1) cptr raw?)))
    
(define (ptr-ref cptr type)
  (call-imported-binding ffi-ptr-ref cptr (ctype-get-primitive type)))

(define (ptr-set! cptr type value)
  (call-imported-binding ffi-ptr-set cptr (ctype-get-primitive type)
                         (scheme->c value type)))

(define (cpointer->string cptr)
  (call-imported-binding ffi-cpointer-to-string cptr))

;;; call-out and call-back

(define (ffi-call obj itypes otype)
  ;; TODO: finalizer for cif
  (let ((cif (call-imported-binding ffi-make-cif
                                    (map ctype-get-primitive itypes)
                                    (ctype-get-primitive otype))))
    (lambda args
      (let ((prim-args (let loop ((prim-args '()) (args args) (itypes itypes))
                         (cond
                          ((null? args)
                           (if (not (null? itypes))
                               (error "too few arguments" obj args))
                           (reverse prim-args))
                          ((null? itypes)
                           (error "too many arguments" obj args))
                          (else
                           (loop (cons (scheme->c (car args) (car itypes))
                                       prim-args)
                                 (cdr args)
                                 (cdr itypes)))))))
        (c->scheme (call-imported-binding ffi-primitive-call obj cif prim-args)
                   otype)))))

(define (ffi-callback obj itypes otype)
  (error "implement ffi-callback!"))

(define (_cprocedure itypes otype . wrapper)
  (let ((wrapper (and (pair? wrapper) (car wrapper))))
    (if wrapper
        (make-ctype _fpointer
          (lambda (x) (ffi-callback (wrapper x) itypes otype))
          (lambda (x)
            (debug-pointer "x" x)
            (wrapper (ffi-call x itypes otype))))
        (make-ctype _fpointer
          (lambda (x) (ffi-callback x itypes otype))
          (lambda (x) (debug-pointer "x" x)
                  (ffi-call x itypes otype))))))

;; ----------------------------------------------------------------------------
;; Safe raw vectors
(define-record-type cvector :cvector
  (really-make-cvector ptr type length)
  cvector?
  (ptr cvector-ptr)
  (type cvector-type)
  (length cvector-length))

(define (make-vector type len)
  (let ((cblock (malloc len type)))
    (really-make-cvector cblock type len)))

(define (cvector type . args)
  (list->cvector args type))

(import-definition ffi-malloc)
(import-definition ffi-free)
(import-definition ffi-is-ptr)
(import-definition ffi-ptr-ref)
(import-definition ffi-ptr-set)
(import-definition ffi-make-cif)
(import-definition ffi-primitive-call)
(import-definition ffi-cpointer-to-string)
