; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; define-enumeration macro

(define-syntax define-enumeration
  (lambda (form rename compare)
    (let ((name (cadr form))
          (components (list->vector (caddr form)))
          (conc (lambda things
                  (string->symbol (apply string-append
                                         (map (lambda (thing)
                                                (if (symbol? thing)
                                                    (symbol->string thing)
                                                    thing))
                                              things)))))
          (%define (rename 'define))
          (%define-syntax (rename 'define-syntax))
          (%begin (rename 'begin))
          (%quote (rename 'quote)))
      (let ((e-name (conc name '- 'enumeration))
            (count (vector-length components)))
        `(,%begin (,%define-syntax ,name
                    (cons (let ((components ',components))
                            (lambda (e r c)
                              (let ((key (cadr e)))
                                (cond ((c key 'components)
                                       (r ',e-name))
                                      ((c key 'enum)
                                       (let ((which (caddr e)))
                                         (let loop ((i 0)) ;vector-posq
                                           (if (< i ,count)
                                               (if (c which (vector-ref components i))
                                                   i
                                                   (loop (+ i 1)))
                                               ;; (syntax-error "unknown enumerand name"
                                               ;;               `(,(cadr e) ,(car e) ,(caddr e)))
                                               e))))
                                      (else e)))))
                          '(,e-name)))  ;Auxiliary binding
                  (,%define ,e-name ',components)
                  (,%define ,(conc name '- 'count) ,count)))))
  (begin define define-syntax quote))


(define-syntax components
  (cons (lambda (e r c) `(,(cadr e) components))
        '()))

(define-syntax enum
  (cons (lambda (e r c) `(,(cadr e) enum ,(caddr e)))
        '()))


(define-syntax enumerand->name
  (syntax-rules ()
    ((enumerand->name ?enumerand ?type)
     (vector-ref (components ?type) ?enumerand))))

(define-syntax name->enumerand
  (syntax-rules ()
    ((name->enumerand ?name ?type)
     (lookup-enumerand (components ?type) ?name))))

(define (lookup-enumerand components name)
  (let ((len (vector-length components)))
    (let loop ((i 0))                   ;vector-posq
      (if (>= i len)
          #f
          (if (eq? name (vector-ref components i))
              i
              (loop (+ i 1)))))))
