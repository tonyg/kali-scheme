(define-syntax _fun
  (lambda (form r compare)
    (define (make-gensym prefix)
      (lambda (i)
        (r (string->symbol (string-append prefix (number->string i))))))
    (define (err msg . sub) (apply syntax-error '_fun msg form sub))
    (let ((inputs #f) (output #f) (bind '()) (pre '()) (post '())
          (input-names #f) (output-type #f) (output-expr #f)
          (first-arg #f) (prev-arg #f))
      (define (bind! x) (set! bind (append! bind (list x))))
      (define (pre! x) (set! pre (append! pre (list x))))
      (define (post! x) (set! post (append! post (list x))))
      (define (t-n-e clause)
        (lambda (type name expr)
          (let ((keys (custom-type->keys type err)))
            (define (getkey key) (cond ((assq key keys) => cdr) (else #f)))
            (define (arg x . no-expr?)
              (define use-expr?
                (and (list x) (= 3 (length x)) (eq? '=> (car x))))
              (if use-expr?
                  (set! x (add-renamer (caddr x) name (cadr x)))
                  (if (and (pair? no-expr?) (car no-expr?) expr)
                      (err "got an expression for a custom type that does not use it"
                           clause)
                      (set! expr (unspecific))))
              (cond ((getkey 'first) =>
                     (lambda (v)
                       (if first-arg
                           (set! x v)
                           (err "got a custom type that wants 1st arg too early"
                                clause)))))
              (cond ((getkey 'prev) =>
                     (lambda (v)
                       (if prev-arg
                           (set! x v)
                           (err "got a custom type that wants prev arg too early"
                                clause)))))
              x)
            (cond (keys
                   (set! type (getkey 'type))
                   (cond ((and (not expr) (getkey 'expr))
                          => (lambda (x) (set! expr x))))
                   (cond ((getkey 'bind)
                          => (lambda (x) (bind! `(,x ,name)))))
                   (cond ((getkey 'pre)
                          => (lambda (x) (pre! `(,name ,(arg x #t))))))
                   (cond ((getkey 'post)
                          => (lambda (x) (post! `(,name ,(arg x))))))))
            (cond (type
                   (if (not first-arg) (set! first-arg name))
                   (set! prev-arg name)))
            (list type name expr))))
      ;; parse "::"
      (let ((s (split-by ':: (cdr form))))
        (case (length s)
          ((0) (err "something bad happened (::)"))
          ((1) #f)
          ((2) (cond ((and (= 1 (length (car s))) (not (eq? '-> (caar s))))
                      (set! xs (cadr s))
                      (set! input-naes (caar s)))
                     (else
                      (err "bad wrapper formals"))))
          (else (err "saw two or more instances of `::'"))))
      ;; parse "->"
      (let ((s (split-by '-> (cdr form))))
        (case (length s)
          ((0) (err "something bad happened (->)"))
          ((1) (err "missing output type"))
          ((2 3) (set! inputs (car s))
           (case (length (cadr s))
             ((1) (set! output-type (caadr s)))
             ((0) (err "missing output type after `->'"))
             (else (err "extraneous output type" (cadadr s))))
           (if (not (null? (cddr s)))
               (case (length (caddr s))
                 ((1) (set! output-expr (caaddr s)))
                 ((0) (err "missing output expression after `->'"))
                 (else (err "extraneous output expression"
                            (cadr (caddr s)))))))
          (else (err "saw three or more instances of `->'"))))
      (set! inputs
            (map (lambda (sub temp)
                   (let ((t-n-e (t-n-e sub)))
                     (match sub 
                       ((list name ': type)        (t-n-e type name #f))
                       ((list type '= expr)        (t-n-e type temp expr))
                       ((name ': type '= expr)     (t-n-e type name expr))
                       (else                       (t-n-e sub temp #f)))))
                 inputs
                 (map (make-gensym "tmp") (iota (length inputs)))))
      ;; when processing the output type, only the post code matters
      (set! pre! (lambda (x) #f))
      (set! output
            (let ((temp ((make-gensym "ret") 0))
                  (t-n-e (t-n-e output-type)))
              (match output-type
                ((list name ': type) (t-n-e type name output-expr))
                ((list type '= expr) (if output-expr
                                         (err "extraneous output expression" expr)
                                         (t-n-e type temp expr)))
                ((list name ': type '= expr)
                 (if output-expr
                     (err "extraneous output expression" expr)
                     (t-n-e type name expr)))
                (else
                 (t-n-e output-type temp output-expr)))))
      (if (or (caddr output) input-names (any caddr inputs)
              (any (lambda (x) (not (car x))) inputs)
              (pair? bind) (pair? pre) (pair? post))
          (let* ((input-names (or input-names
                                  (filter-map (lambda (i)
                                                (and (not (caddr i)) (cadr i)))
                                              inputs)))
                 (output-expr (let ((o (caddr output)))
                                (or (and (not (void? o)) o)
                                    (cadr output))))
                 (args (filter-map (lambda (i) (and (caddr i)
                                                    (not (void? (caddr i)))
                                                    `(,(cadr i) ,(caddr i))))
                                   inputs))
                 (ffi-args (filter-map (lambda (x) (and (car x) (cadr x)))
                                       inputs)))
            (for-each display `("post: " ,post ", pre: " ,pre #\newline))
            `(,(r '_cprocedure) (list ,@(filter-map car inputs)) ,(car output)
              (,(r 'lambda) (,(r 'ffi))
                (,(r 'lambda) ,input-names
                 (,(r 'let*) (,@args
                              ,@bind
                              ,@pre
                              (,(cadr output) (,(r 'ffi) ,@ffi-args))
                              ,@post)
                  ,output-expr)))))
          `(,(r '_cprocedure) (list ,@(filter-map car inputs)) ,(car output))))))

(define-fun-syntax _ptr
  ((list 'i t) `(type: _pointer
                 pre: (x => (let ((p (malloc t))) (ptr-set! p ,t x) p))))
  ((list 'o t) `(type: _pointer
                 pre: (malloc ,t)
                 post: (x => (ptr-ref x ,t))))
  ((list 'io t) `(type: _pointer
                  pre: (x => (let ((p (malloc ,t))) (ptr-set! p ,t x) p))
                  post: (x => (ptr-ref x ,t)))))
