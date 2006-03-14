; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.


; Messages
;==========================================================================

(define (bug format-string . args)
  (apply error (list "~?~%  (compiler error)" format-string args)))

(define (user-error format-string . args)
  (apply error (list "~?~%  (source error)" format-string args)))

(define (user-warning format-string . args)
  (apply error (list "~?~%  (source error)" format-string args)))

; Little utilities.
;========================================================================

(define (true  x) #t)
(define (false x) #f)

(define (or-map proc list)
  (do ((list list (cdr list))
       (r '#f (or (proc (car list)) r)))
      ((null? list) r)))

(define (remove-similar-elts pred list)
  (do ((list list (cdr list))
       (res  '()  (if (mem? pred (car list) res)
                      res
                      (cons (car list) res))))
      ((null-list? list)
       res)))

(define (select-from-table pred table)
  (let ((res '()))
    (table-walk (lambda (key entry)
                  (if (pred key entry)
                      (set! res `((,key . ,entry) . ,res))))
                table)
    res))

(define (table->list table)
  (select-from-table (lambda (x y) #t) table))

(define (table->entry-list table)
  (let ((res '()))
    (table-walk (lambda  (key entry)
                  (set! res (cons entry res)))
                table)
    res))

(define (table-push table key val)
  (let ((old (table-ref table key)))
    (table-set! table key (if old
			      (cons val old)
			      (list val)))
    val))

(define (table-pop table key)
  (let ((old (table-ref table key)))
    (table-set! table key (cdr old))
    (car old)))

(define (merge-lists x y)
  (cond ((null? y) x)
        (else (do ((z x (cdr z))
                   (u y (let ((w (car z)))
                          (if (memq? w u) u (cons w u)))))
                  ((null? z) u)))))

(define (vector-every? pred vec)
  (let loop ((i (- (vector-length vec) '1)))
    (cond ((< i '0)
	   '#t)
          ((not (pred (vector-ref vec i)))
	   '#f)
          (else
	   (loop (- i '1))))))

(define (make-ignorable vars)
  (let loop ((vars vars) (res '()))
    (cond ((null? vars)
           `(ignorable . ,res))
          ((atom? vars)
           `(ignorable ,vars . ,res))
          (else
           (loop (cdr vars) (cons (car vars) res))))))

(define (sub-vector->list vec start)
  (do ((i (- (vector-length vec) '1) (- i '1))
       (r '() (cons (vector-ref vec i) r)))
      ((< i start)
       r)))

; A version of ASSQ that works on lists containing atoms.

(define (flag-assq obj list)
  (let loop ((l list))
    (if (null-list? l) 
        #f
        (let ((z (car l)))
          (if (and (pair? z)
                   (eq? obj (car z)))
              z
              (loop (cdr l)))))))

; T system stuff

(define (enforce predicate value)
  (if (predicate value)
      value
      (error "enforce ~A failed on ~A" predicate value)))

(define (writec port char)
  (write-char char port))

(define (mem? predicate object list)
  (let loop ((list list))
    (cond ((null? list)
	   #f)
	  ((predicate object (car list))
	   #t)
	  (else
	   (loop (cdr list))))))

(define (walk-vector procedure vector)
  (do ((i 0 (+ i 1)))
      ((>= i (vector-length vector))
       (values))
    (procedure (vector-ref vector i))))

(define (vector-replace to from count)
  (do ((i 0 (+ i 1)))
      ((>= i count))
    (vector-set! to i (vector-ref from i)))
  (values))

(define (copy-vector vector)
  (let ((new (make-vector (vector-length vector))))
    (do ((i 0 (+ i 1)))
	((= i (vector-length vector)))
      (vector-set! new i (vector-ref vector i)))
    new))

(define (copy-list list)
  (if (null-list? list)
      list
      (let ((copy (cons (car list) '())))
	(let loop ((list (cdr list)) (result copy))
	  (cond ((not (null-list? list))
		 (let ((next (cons (car list) '())))
		   (set-cdr! result next)
		   (loop (cdr list) next)))))
	copy)))

(define (symbol-hash symbol)
  (string-hash (symbol->string symbol)))

(define string-hash (structure-ref features string-hash))

(define (object-hash x)
  0)           ; wah!

(define (union l1 l2)
  (if (null? l2)
      l1
      (do ((l1 l1 (cdr l1))
	   (res l2 (if (memq? (car l1) res) res (cons (car l1) res))))
	  ((null? l1)
	   res))))

(define (intersection l1 l2)
  (if (null? l2)
      '()
      (do ((l1 l1 (cdr l1))
	   (res '() (if (memq? (car l1) l2) (cons (car l1) res) res)))
	  ((null? l1)
	   res))))

(define (set-difference l1 l2)
  (if (null? l2)
      l1
      (do ((l1 l1 (cdr l1))
	   (res '() (if (memq? (car l1) l2) res (cons (car l1) res))))
	  ((null? l1)
	   res))))
