; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file macros.scm.

; Declarations aren't currently used for anything.  There was once
; some idea of making a Prescheme compiler pay attention to them.

(define-vm-syntax (declare . stuff)
  ''0)

; Enumerations

; Formerly in struct.scm

(define-vm-syntax (d-vector type key . args)
  `(let ((v (make-d-vector ,type ,(length args) key)))
     ,@(do ((a args (cdr a))
            (i 0 (+ i 1))
            (z '() (cons `(d-vector-set! v ,i ,(car a)) z)))
           ((null? a) (reverse z)))
     v))

(define-vm-syntax (define-shared-primitive-data-type name . scheme?)
  (let ((data (cddr (assq name stob-data)))
	(type (concatenate-symbol 'stob/ name))
	(fixup (lambda (n)
		 (if (not (null? scheme?))
		     (concatenate-symbol 'vm- n)
		     n))))
    `(define-primitive-data-type
       ,type
       ,(fixup name)
       ,(fixup (car data))
       . ,(map (lambda (p)
		 (cons (fixup (car p))
		       (if (not (cadr p))
			   '()
			   (list (fixup (cadr p))))))
	       (cdr data)))))

(define-vm-syntax (define-primitive-data-type num type make . body)
  (let* ((pred (concatenate-symbol type '?))
         (size (concatenate-symbol type '- 'size))
         (shorten (lambda (l1 l2) (map (lambda (x1 x2) x2 x1) l1 l2)))
         (vars (shorten `(a b c d e f g) body)))
    `(begin (define (,make ,@vars key)
              (d-vector ,num key ,@vars))
            (define ,pred (stob-predicate ,num))
            (define ,size ,(+ (length body) 1))
            ,@(do ((s body (cdr s))
                   (i 0 (+ i 1))
                   (d '() (let* ((slot (car s))
                                 (d (cons `(define (,(car slot) x)
                                             (d-vector-ref x ,i))
                                          d)))
                            (if (null? (cdr slot))
                                d
                                (cons `(define (,(cadr slot) x val)
                                         (d-vector-set! x ,i val))
                                      d)))))
                  ((null? s) (reverse d))))))

(define-vm-syntax (define-primitive opcode input-types action . returner-option)
  `(define-opcode ,opcode
     (lambda ()
       (primitive-procedure-body
        ,opcode ,input-types ,action . ,returner-option))))

(define-vm-syntax (define-consing-primitive opcode
                   input-types space-proc action . returner-option)
  (let* ((shorten (lambda (l1 l2) (map (lambda (x1 x2) x2 x1) l1 l2)))
         (args (shorten '(arg1 arg2 arg3 arg4) input-types))
	 (action `(lambda ,args (,action ,@args key))))
    `(define-opcode ,opcode
       (lambda ()
         (let ((key (ensure-space (,space-proc *val*))))
           (primitive-procedure-body
             ,opcode ,input-types ,action . ,returner-option))))))

(define-vm-syntax (primitive-procedure-body opcode input-types action
                                         . returner-option)
  (let* ((shorten (lambda (l1 l2) (map (lambda (x1 x2) x2 x1) l1 l2)))
	 (places (reverse (shorten '(*val* arg2 arg3 arg4) input-types)))
	 (nargs (length input-types)))
    `(let* (
	    ,@(if (>= nargs 2) `((arg2 (pop))) `())
	    ,@(if (>= nargs 3) `((arg3 (pop))) `())
	    ,@(if (>= nargs 4) `((arg4 (pop))) `())
	    )
       (if (and ,@(map (lambda (in place)
                         `((input-type-predicate ,in) ,place))
                       input-types
                       places))
           ,(let ((yow `(,action
                         ,@(map (lambda (in place)
                                  `((input-type-coercion ,in) ,place))
                                input-types
                                places))))
              (if (null? returner-option)
                  yow
                  `(goto ,(car returner-option) ,yow)))
           (goto ,(case nargs
		    ((0) 'raise-exception)
		    ((1) 'raise-exception1)
		    ((2) 'raise-exception2)
		    ((3) 'raise-exception3)
		    ((4) 'raise-exception4))
		 0 . ,places)))))

; This is no longer used

;(define-vm-syntax (define-primitive-structure-type in-scheme? type make . body)
;  (let* ((id->vm (lambda (s) (if in-scheme? (concatenate-symbol 'vm- s) s)))
;         (type->           (concatenate-symbol           type '- '>))
;         (immutable-type-> (concatenate-symbol 'mutable- type '- '>))
;         (type?    (concatenate-symbol type '?))
;         (size     (concatenate-symbol type '- 'size))
;         (op/    (lambda (name) (concatenate-symbol 'op/ name)))
;         (shorten (lambda (l1 l2) (map (lambda (x1 x2) x2 x1) l1 l2)))
;         (vars (shorten `(a b c d e f g) body)))
;    `(let ((,type-> (input-type ,(id->vm type?) no-coercion))
;           (,immutable-type-> (mutable-input-type ,(id->vm type?))))
;       (define-primitive ,(op/ type?) (any->) ,(id->vm type?) return-boolean)
;       ,@(if make
;             `((define-consing-primitive ,(op/ make)
;                 ,(map (lambda (var) var `any->) vars)
;                 (lambda (ignore) ,(id->vm size))
;                 (lambda (,@vars key) (return (,(id->vm make) ,@vars key)))))
;             '())
;       ,@(apply append
;                (map (lambda (slot)
;                       (let ((get (car slot)))
;                         `((define-primitive ,(op/ get) (,type->)
;                             ,(id->vm get) return)
;                           ,@(if (null? (cdr slot))
;                                 `()
;                                 (let ((set (cadr slot)))
;                                   `((define-primitive ,(op/ set)
;                                       (,immutable-type-> any->)
;                                       ,(id->vm set)
;                                       return-unspecific)))))))
;                     body)))))

(define-vm-syntax (define-vector-type type elt-type in-scheme?)
  (let* ((id->vm (lambda (s) (if in-scheme? (concatenate-symbol 'vm- s) s)))
	 (type->           (concatenate-symbol           type '- '>))
	 (immutable-type-> (concatenate-symbol 'mutable- type '- '>))
         (type?      (concatenate-symbol type '?))
         (size       (concatenate-symbol type '- 'size))
         (make       (concatenate-symbol 'make- type))
         (length     (concatenate-symbol type '- 'length))
         (ref        (concatenate-symbol type '- 'ref))
         (set        (concatenate-symbol type '- 'set!))
         (elt->      (concatenate-symbol elt-type '- '>))
         (return-elt (concatenate-symbol 'return- elt-type))
         (op/ (lambda (name) (concatenate-symbol 'op/ name))))
    `(let ((,type-> (input-type ,(id->vm type?) no-coercion))
	   (,mutable-type-> (mutable-input-type ,(id->vm type?) no-coercion)))
       (define-primitive ,(op/ type?) (any->) ,(id->vm type?) return-boolean)
       (define-primitive ,(op/ length) (,type->) ,(id->vm length) return-fixnum)
       (define-primitive ,(op/ make)
         (fixnum-> ,elt->)
         (vector-maker ,(id->vm size) ,(id->vm make) ,(id->vm set)))
       (define-primitive ,(op/ ref) (,type-> fixnum->)
         (vector-referencer ,(id->vm length) ,(id->vm ref) ,return-elt))
       (define-primitive ,(op/ set) (,mutable-type-> fixnum-> ,elt->)
         (vector-setter ,(id->vm length) ,(id->vm set)))
       )))


