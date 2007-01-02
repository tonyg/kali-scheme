; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define (concatenate-symbol . stuff)
  (string->symbol
   (apply string-append
	  (map (lambda (x)
		 (cond ((string? x) x)
		       ((symbol? x) (symbol->string x))
		       ((number? x) (number->string x))
		       (else
			(error "cannot coerce ~S to a string" x))))
	       stuff))))

(define (error format-string . args)
  (if #t       ; work around a bug in the type system
      (rts-error (apply format (cons #f (cons format-string args))))))

(define (breakpoint format-string . args)
  (rts-breakpoint (apply format (cons #f (cons format-string args)))))

(define (atom? x)
  (not (pair? x)))

(define (neq? x y)
  (not (eq? x y)))

(define (n= x y)
  (not (= x y)))

(define (identity x) x)

(define (no-op x) x)    ; guaranteed not to be in-lined

(define (null-list? x)
  (cond ((null? x) #t)
	((pair? x) #f)
	(else
	 (error "null-list? got a non-list" x))))

(define (reverse! l)
  (cond ((or (null? l)
	     (null? (cdr l)))
	 l)
	(else
	 (let ((rest (cdr l)))
	   (set-cdr! l '())
	   (let loop ((l1 l) (l2 rest))
	     (cond ((null? l2)
		    l1)
		   (else
		    (let ((rest (cdr l2)))
		      (set-cdr! l2 l1)
		      (loop l2 rest)))))))))

(define (memq? x l)
  (let loop ((l l))
    (cond ((null? l)       #f)
	  ((eq? x (car l)) #t)
	  (else            (loop (cdr l))))))

(define (first pred list)
  (let loop ((list list))
    (cond ((null? list)
	   #f)
          ((pred (car list))
	   (car list))
          (else
	   (loop (cdr list))))))

(define any first)  ; ANY? need not search in order, but it does anyway

(define (any? proc list)
  (let loop ((list list))
    (cond ((null? list)
	   #f)
          ((proc (car list))
	   #t)
          (else
	   (loop (cdr list))))))

(define (every? pred list)
  (let loop ((list list))
    (cond ((null? list)
	   #t)
          ((pred (car list))
	   (loop (cdr list)))
          (else
	   #f))))

(define (filter pred l)
  (let loop ((l l) (r '()))
    (cond ((null? l)
	   (reverse r))
          ((pred (car l))
	   (loop (cdr l) (cons (car l) r)))
          (else
	   (loop (cdr l) r)))))

(define (filter! pred list)
  (let filter! ((list list))
    (cond ((null-list? list)
	   '())
          ((pred (car list))
	   (set-cdr! list (filter! (cdr list))) list)
          (else
	   (filter! (cdr list))))))

(define (filter-map f l)
  (let loop ((l l) (r '()))
    (cond ((null? l)
	   (reverse r))
          ((f (car l))
           => (lambda (x)
                (loop (cdr l) (cons x r))))
          (else
	   (loop (cdr l) r)))))

(define (remove-duplicates list)
  (do ((list list (cdr list))
       (res  '()  (if (memq? (car list) res)
                      res
                      (cons (car list) res))))
      ((null-list? list)
       res)))

(define (partition-list pred l)
  (let loop ((l l) (yes '()) (no '()))
    (cond ((null? l)
           (values (reverse yes) (reverse no)))
          ((pred (car l))
           (loop (cdr l) (cons (car l) yes) no))
          (else
           (loop (cdr l) yes (cons (car l) no))))))

(define (partition-list! pred l)
  (let loop ((l l) (yes '()) (no '()))
    (cond ((null? l)
           (values (reverse! yes) (reverse! no)))
          ((pred (car l))
	   (let ((rest (cdr l)))
	     (set-cdr! l yes)
	     (loop rest l no)))
          (else
	   (let ((rest (cdr l)))
	     (set-cdr! l no)
	     (loop rest yes l))))))

(define (delq! object list)
  (let loop ((list list))
    (cond ((null? list)
	   '())
	  ((eq? object (car list))
	   (loop (cdr list)))
	  (else
	   (let loop ((next (cdr list)) (prev list))
	     (cond ((null? next)
		    list)
		   ((eq? (car next) object)
		    (set-cdr! prev (cdr next))
		    (loop (cdr next) prev))
		   (else
		    (loop (cdr next) next))))))))

(define (delq thing list)
  (delete (lambda (x) (eq? x thing)) list))

(define (delete pred in-list)
  (let loop ((list in-list) (res '()))
    (cond ((null? list)
	   in-list)
	  ((pred (car list))
	   (append-reverse! res (delete pred (cdr list))))
	  (else
	   (loop (cdr list) (cons (car list) res))))))

(define (append-reverse! l1 l2)
  (let loop ((list l1) (res l2))
    (cond ((null? list)
	   res)
	  (else
	   (let ((next (cdr list)))
	     (set-cdr! list res)
	     (loop next list))))))

; Copying strings.

(define (string->immutable-string string)
  (if (immutable? string)
      string
      (let ((copy (string-copy string)))
	(make-immutable! copy)
	copy)))



