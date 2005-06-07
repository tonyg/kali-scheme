(define-syntax define-values
  (lambda (form r compare)
    (define (gensym i)
      (r (string->symbol (string-append "x" (number->string i)))))
    (destructure (((define-values values expr) form))
      (let ((tmp-vars (map gensym (iota (length values)))))
        `(,(r 'begin)
          ,@(map (lambda (name) `(,(r 'define) ,name (,(r 'unspecific)))) values)
          (,(r 'receive) ,tmp-vars ,expr
           ,@(map (lambda (var tmp) `(,(r 'set!) ,var ,tmp)) values tmp-vars)))))))
