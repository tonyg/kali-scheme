; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Compiler state, including flags controlling debug data retention.


; Package and location uids and the location name table should be here
; as well...

; Will the use of a fluid variable significantly degrade performance?

(define (new-template-uid)
  (let ((uid *template-uid*))
    (set! *template-uid* (+ *template-uid* 1))
    uid))

(define *template-uid* 5000)

(define (template-uid) *template-uid*)
(define (set-template-uid! uid) (set! *template-uid* uid))



; These variables really ought to be dynamically scoped, not global.
; Fix this some day.

(define debug-flag-names '(names maps files source tabulate table))

(define type/debug-flags
  (make-record-type 'debug-flags debug-flag-names))

(define make-debug-flags
  (record-constructor type/debug-flags debug-flag-names))

(define $debug-flags
  (make-fluid (make-debug-flags #t	;proc names
				#t	;env maps
				#f	;no file names
				#f	;no cont source
				#f	;no tabulate
				(make-table))))

(define (debug-flag-accessor name)
  (let ((access (record-accessor type/debug-flags name)))
    (lambda () (access (fluid $debug-flags)))))

(define (debug-flag-modifier name)
  (let ((modify (record-modifier type/debug-flags name)))
    (lambda (new) (modify (fluid $debug-flags) new))))

(define keep-source-code?      (debug-flag-accessor 'source))
(define keep-environment-maps? (debug-flag-accessor 'maps))
(define keep-procedure-names?  (debug-flag-accessor 'names))
(define keep-file-names?       (debug-flag-accessor 'files))
(define tabulate-debug-data?   (debug-flag-accessor 'tabulate))
(define debug-data-table       (debug-flag-accessor 'table))


; Kludge for static linker.

(define (with-fresh-compiler-state thunk)
  (let-fluid $debug-flags (make-debug-flags #t ;proc names
					    #t ;env maps
					    #f ;no file names
					    #f ;no cont source
					    #t ;tabulate       ***
					    (make-table))
    (lambda ()
      (saving-and-restoring (lambda () *template-uid*)
			    (lambda (s) (set! *template-uid* s))
			    0
			    thunk))))

(define (saving-and-restoring fetch store! other thunk)
  (let ((swap (lambda ()
		(let ((temp (fetch)))
		  (store! other)
		  (set! other temp)))))
    (dynamic-wind swap thunk swap)))
