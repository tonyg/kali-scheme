; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; --------------------
; DISCLOSE methods

(define-method &disclose ((obj :closure))
  (let ((id (template-id (closure-template obj)))
        (name (template-print-name (closure-template obj))))
    (if name
        (list 'procedure
              id
              ;; A heuristic that sometimes loses.
;                 (if (and (pair? name)
;                          (eq? (car name) '#t) ;Curried
;                          (vector? (closure-env obj)))
;                     (error-form
;                       (if (null? (cdddr name))
;                           (caddr name)
;                           (cdddr name))
;                       (reverse (cdr (vector->list (closure-env obj)))))
;                     name)
              name)
        (list 'procedure id))))

(define-method &disclose ((obj :template))
  (let ((id (template-id obj))
        (name (template-print-name obj)))
    (if name
        (list 'template id name)
        (list 'template id))))

(define-method &disclose ((obj :location))
  (cons 'location
        (cons (location-id obj)
              (let ((name (location-name obj)))
                (if (and name (not (eq? name (location-id obj))))
                    (list name (location-package-name obj))
                    '())))))

(define-method &disclose ((obj :continuation))
  (list 'continuation
        (list 'pc (continuation-pc obj))
        (let ((tem (continuation-template obj)))
          (or (template-print-name tem) (template-id tem)))))

(define-method &disclose ((obj :code-vector))
  (list 'code-vector (code-vector-length obj))
; (cons 'code-vector
;       (let ((z (code-vector-length obj)))
;         (do ((i (- z 1) (- i 1))
;              (l '() (cons (code-vector-ref obj i) l)))
;             ((< i 0) l))))
  )


(define (template-print-name tem)
  (make-print-name (template-names tem)))

(define (make-print-name names)
  (if (null? names)
      #f
      (let ((name (car names))
            (parent-name (make-print-name (cdr names))))
        (cond (parent-name
               `(,(if name name 'unnamed)
                 in
                 ,@(if (pair? parent-name) parent-name (list parent-name))))
              ((string? name) #f)  ;File name
              (else name)))))

(define (template-file-name tem)
  (let loop ((names (template-names tem)))
    (if (null? names)
        #f
        (if (string? (car names))
            (car names)
            (loop (cdr names))))))

; --------------------
; Location names

(define (location-info loc)
  (let ((id (location-id loc)))
    (if (integer? id)
        (table-ref location-info-table id)
        #f)))

(define (location-name loc)
  (let ((probe (location-info loc)))
    (if probe
        (car probe)
        #f)))

(define (location-package-name loc)
  (let ((probe (location-info loc)))
    (if probe
        (table-ref package-name-table (cdr probe))
        #f)))


; --------------------
; Condition disclosers

(define *condition-disclosers* '())

(define (define-condition-discloser pred proc)
  (set! *condition-disclosers*
        (cons (cons pred proc) *condition-disclosers*)))

(define-method &disclose-condition ((c :pair))
  (let loop ((l *condition-disclosers*))
    (if (null? l)
        (cons (cond ((error? c) 'error)
                    ((warning? c) 'warning)
                    (else (car c)))
              (condition-stuff c))
        (if ((caar l) c)
            ((cdar l) c)
            (loop (cdr l))))))

(define-condition-discloser interrupt?
  (lambda (c)
    (list 'interrupt (enumerand->name (cadr c) interrupt))))
        

; Make prettier error messages for exceptions

(define-condition-discloser exception?
  (lambda (c)
    (let ((opcode (exception-opcode c))
          (args   (exception-arguments c)))
      ((vector-ref exception-disclosers opcode)
       opcode
       args))))

(define exception-disclosers
  (make-vector op-count
               (lambda (opcode args)
                 (list 'error
                       "exception"
                       (let ((name (enumerand->name opcode op)))
                         (if (>= opcode (enum op eq?))
                             (error-form name args)
                             (cons name args)))))))

(define (define-exception-discloser opcode discloser)
  (vector-set! exception-disclosers opcode discloser))

(let ((disc (lambda (opcode args)
              (let ((loc (car args)))
                (cons 'error
                      (cons (if (location-defined? loc)
                                "unassigned variable"
                                "undefined variable")
                            (cons (or (location-name loc) loc)
                                  (let ((pack
                                         (location-package-name loc)))
                                    (if pack
                                        (list (list 'package pack))
                                        '())))))))))
  (define-exception-discloser (enum op global) disc)
  (define-exception-discloser (enum op set-global!) disc))

(let ((disc (lambda (opcode args)
              (let ((proc (car args))
                    (as (cadr args)))
                (list 'error
                      "wrong number of arguments"
                      (error-form (or (if (closure? proc)
                                          (or (template-print-name
                                               (closure-template proc))
                                              proc)
                                          proc)
                                      proc)
                                  as))))))
  (define-exception-discloser (enum op check-nargs=) disc)
  (define-exception-discloser (enum op check-nargs>=) disc))

(define-exception-discloser (enum op call)
  (lambda (opcode args)
    (list 'error
          "attempt to call a non-procedure"
          (map value->expression (cons (car args) (cadr args))))))

(define-exception-discloser (enum op values)
  (lambda (opcode args)
    (if (null? (car args))
        (list 'error
              "returning zero values when one is expected"
              '(values))
        (list 'error
              "returning several values when only one is expected"
              (error-form 'values (car args))))))

(let ((disc (lambda (opcode args)
              (let ((thing     (car args))
                    (type-byte (cadr args))
                    (offset    (caddr args))
                    (rest      (cdddr args)))
                (let ((data (assq (enumerand->name type-byte stob)
                                  stob-data)))
                  (list 'error
                        "exception"
                        (error-form ((if (= opcode op/stored-object-ref)
                                         car
                                         cadr)
                                     (list-ref data (+ offset 3)))
                                    (cons thing rest))))))))
  (define-exception-discloser (enum op stored-object-ref) disc)
  (define-exception-discloser (enum op stored-object-set!) disc))

(define op/stored-object-ref (enum op stored-object-ref))

(let ((disc (lambda (opcode args)
              (let ((type (enumerand->name (car args) stob)))
                (list 'error
                      "exception"
                      (error-form (string->symbol
                                   ;; Don't simplify this to "make-"  --JAR
                                   (string-append (symbol->string 'make-)
                                                  (symbol->string type)))
                                  (cdr args)))))))
  (define-exception-discloser (enum op make-vector-object) disc))

(define (vector-exception-discloser suffix)
  (lambda (opcode args)
    (let ((type (enumerand->name (cadr args) stob)))
      (list 'error
            "exception"
            (error-form (string->symbol
                         (string-append (symbol->string type)
                                        "-"
                                        (symbol->string suffix)))
                        (cons (car args) (cddr args)))))))

(define-exception-discloser (enum op stored-object-length)
  (vector-exception-discloser 'length))

(define-exception-discloser (enum op stored-object-indexed-ref)
  (vector-exception-discloser 'ref))

(define-exception-discloser (enum op stored-object-indexed-set!)
  (vector-exception-discloser 'set!))

; Call-errors should print as (proc 'arg1 'arg2 ...)

(define-condition-discloser call-error?
  (lambda (c)
    (list 'error (cadr c) (error-form (caddr c) (cdddr c)))))

; --------------------
; Utilities

(define (error-form proc args)
  (cons proc (map value->expression args)))

(define (value->expression obj)
  (if (or (number? obj) (char? obj) (string? obj) (boolean? obj))
      obj
      `',obj))
