; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file transport.scm.

; Transporters to and from simulated heap.
; This is intended for debugging only.

; Scheme value -> simulated value
;  This is a dangerous thing to call.  It's essential that there be
;  enough space for this routine to allocate whatever storage it
;  needs, without any need for GC, since there isn't any
;  opportunity to do one.

(define (enter obj)
  (cond ((integer? obj) (enter-fixnum obj))
        ((char? obj) (enter-char obj))
        ;; The relative ordering of the next two clauses is
        ;; important when we boot the system from a scheme that
        ;; doesn't distinguish #f from ().
        ((eq? obj '()) null)
        ((eq? obj #f) false)
        ((eq? obj #t) true)
        ((eof-object? obj) eof-object)
	((input-port? obj) (initial-input-port))
	((output-port? obj) (initial-output-port))
        ((pair? obj)
         (vm-cons (enter (car obj)) (enter (cdr obj)) universal-key))
        ((symbol? obj)
         (intern (enter (symbol->string obj))
		 (get-symbol-table)
		 universal-key))
        ((code-vector? obj)  ;should precede string case
         (let ((v (vm-make-code-vector (code-vector-length obj) universal-key)))
           (do ((i 0 (+ i 1)))
               ((>= i (code-vector-length obj)) v)
             (vm-code-vector-set! v i (code-vector-ref obj i)))))
        ((string? obj)
	 (enter-string obj))
        ((location? obj)
         (lookup (get-system-environment)
		 (enter (location-id obj))
		 universal-key))
        ;; Vector case must be last, for simulation
        ((vector? obj)
         (let ((v (vm-make-vector (vector-length obj) universal-key)))
           (do ((i 0 (+ i 1)))
               ((>= i (vector-length obj)) v)
             (vm-vector-set! v i (enter (vector-ref obj i))))))
        (else
         (error "unenterable object" obj))))

; Simulated value -> scheme value

(define *depth* 30)

(define (extract obj)
  (sub-extract obj '() 0))

(define (sub-extract obj a level)
  (cond ((memv obj a)   ; a = trail
         (newline)
         (display "(Cycle encountered.) ")
         '<cycle>)
        ((> level *depth*)
         (display "(Depth limit exceeded.) ")
         '<depth>)
        (else
      (let ((a (cons obj a))
            (level (+ level 1)))
        (cond ((fixnum? obj) (extract-fixnum obj))
              ((vm-char? obj) (extract-char obj))
              ((vm-eq? obj null) '())
              ((vm-eq? obj false) #f)
              ((vm-eq? obj true) #t)
              ((vm-eq? obj unspecific) '<unspecific>)
              ((vm-eq? obj unbound-marker) '<unbound>)
              ((vm-eq? obj unassigned-marker) '<unassigned>)
              ((vm-pair? obj)
               (cons (sub-extract (vm-car obj) a level)
                     (sub-extract (vm-cdr obj) a level)))
              ((vm-symbol? obj)
               (string->symbol (extract-string (vm-symbol->string obj))))
              ((vm-location? obj)
               (make-undefined-location (sub-extract (vm-location-id obj) a level)))
              ((vm-closure? obj)
               (make-closure (sub-extract (vm-closure-template obj) a level)
                             (sub-extract (vm-closure-env obj) a level)))
              ((vm-string? obj)
               (extract-string obj))
              ((vm-code-vector? obj)
               (let ((z (vm-code-vector-length obj)))
                 (let ((v (make-code-vector z 0)))
                   (do ((i 0 (+ i 1)))
                       ((>= i z) v)
                     (code-vector-set! v i (vm-code-vector-ref obj i))))))
              ((vm-vector? obj)
               (let ((z (vm-vector-length obj)))
                 (let ((v (make-vector z)))
                   (do ((i 0 (+ i 1)))
                       ((>= i z) v)
                     (vector-set! v i (sub-extract (vm-vector-ref obj i) a level))))))
              ((record? obj)
               (let ((z (record-length obj)))
                 (let ((v (make-vector (+ 1 z))))
		   (vector-set! v 0 'record)
                   (do ((i 0 (+ i 1)))
                       ((>= i z) v)
                     (vector-set! v (+ i 1)
				  (sub-extract (record-ref obj i)
					       a level))))))
              ((port? obj) (extract-port obj))
              (else `(<random> ,obj)))))))


(import vm-environment
	universal-key
	null false true eof-object
	unspecific unbound-marker unassigned-marker
	vm-eq?
	initial-input-port initial-output-port
	vm-char? enter-char extract-char
	fixnum? enter-fixnum extract-fixnum
	vm-pair? vm-cons vm-car vm-cdr
	(vm-code-vector?       code-vector?)
	(vm-make-code-vector   make-code-vector)
	(vm-code-vector-length code-vector-length)
	(vm-code-vector-ref    code-vector-ref)
	(vm-code-vector-set!   code-vector-set!)
	vm-vector? vm-make-vector vm-vector-length
	vm-vector-ref vm-vector-set!
	vm-string? extract-string enter-string
	vm-symbol? vm-symbol->string intern
	record? record-ref
	(vm-closure?         closure?)
	(vm-closure-template closure-template)
	(vm-closure-env      closure-env)
	lookup
	(vm-location? location?)
	(vm-location-id location-id)
	port? extract-port
	)
