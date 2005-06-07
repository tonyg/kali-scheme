;;; Utilities for the _fun macro

(define (split-by key args)
  (let loop ((args args) (r (list '())))
    (cond ((null? args) (reverse! (map reverse! r)))
          ((eq? key (car args)) (loop (cdr args) (cons '() r)))
          (else (set-car! r (cons (car args) (car r)))
                (loop (cdr args) r)))))

(define (add-renamer body to from)
  (cond ((equal? body from)
         to)
        ((pair? body)
         (cons (add-renamer (car body) from to)
               (add-renamer (cdr body) from to)))
        (else
         body)))

(define *fun-syntax-expanders* '())

(define (expand-fun-syntax type)
  ;;(for-each display `("expanding fun: " ,type ", expanders: " ,*fun-syntax-expanders* #\newline))
  (cond ((and (pair? type) (assq (car type) *fun-syntax-expanders*))
         => (lambda (expander)
              ;;(for-each display `("expander: " ,expander #\newline))
              (if expander ((cdr expander) (cdr type)) type)))
        (else type)))

(define-syntax define-fun-syntax
  (lambda (form r compare)
    (destructure (((define-fun-syntax id . body) form))
      `(,(r 'set!) ,(r '*fun-syntax-expanders*)
        (,(r 'cons) (,(r 'cons) ',id (,(r 'lambda) (,(r 'form))
                                      (,(r 'match) ,(r 'form) 
                                       ,@body)))
         ,(r '*fun-syntax-expanders*))))))

(define (custom-type->keys type err)
  (define (with-arg x) (match x
                         ((list id '=> body) (list '=> id body))
                         (else x)))
  (let ((keys '()))
    (define (setkey! key val . id?)
      (cond
       ((assq key keys)
        (err "bad expansion of custom type (two `~a:'s)" key type))
       ((and (pair? id?) (car id?) (not (symbol? val)))
        (err "bad expansion of custom type (`~a:' exepects and identifier)"
             key type))
       (else
        (set! keys (cons (cons key val) keys)))))
    (let loop ((t (expand-fun-syntax type)))
      (define (next rest . args) (apply setkey! args) (loop rest))
      (match t
        ((list-rest 'type: t xs) (next xs 'type t))
        ((list-rest 'expr: e xs) (next xs 'expr e))
        ((list-rest 'bind: id xs) (next xs 'bind id #t))
        ((list-rest 'first-arg: id xs) (next xs 'first id #t))
        ((list-rest 'prev-arg: id xs) (next xs 'prev id #t))
        ((list-rest 'pre: p xs) (next xs 'pre (with-arg p)))
        ((list-rest 'post: p xs) (next xs 'post (with-arg p)))
        (`() (and (pair? keys) keys))
        (else #f)))))

(define (void? thing)
  (eq? thing (unspecific)))
