; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Handy things for debugging the run-time system, byte code compiler,
; and linker.


; Alternative command processor.  Handy for debugging the bigger one.

(define (make-mini-command scheme)
  (define-structure mini-command (export command-processor)
    (open scheme
	  simple-signals simple-conditions handle
	  display-conditions
	  i/o)   ; current-error-port
    (files (debug mini-command)))
  mini-command)

; Miniature EVAL, for debugging runtime system sans package system.

(define-structures ((mini-eval evaluation-interface)
		    (mini-environments
		     (export interaction-environment
			     scheme-report-environment
			     set-interaction-environment!
			     set-scheme-report-environment!)))
  (open scheme-level-2
	simple-signals)		;error
  (files (debug mini-eval)))

(define (make-scheme environments evaluation) ;cf. initial-packages.scm
  (define-structure scheme scheme-interface
    (open scheme-level-2
	  environments
	  evaluation))
  scheme)

; Stand-alone system that doesn't contain a byte-code compiler.
; This is useful for various testing purposes.

(define mini-scheme (make-scheme mini-environments mini-eval))

(define mini-command (make-mini-command mini-scheme))

(define-structure little-system (export start)
  (open scheme-level-1
	mini-command
	usual-resumer)
  (begin (define start
	   (usual-resumer
	    (lambda (args) (command-processor #f args))))))

(define (link-little-system)
  (link-simple-system '(scheme/debug little)
		      'start
		      little-system))



; --------------------
; Hack: smallest possible reified system.

(define-structures ((mini-for-reification for-reification-interface)
		    (mini-packages (export make-simple-package)))
  (open scheme-level-2
	features		;contents
	locations
	simple-signals)		;error
  (files (debug mini-package)))

(define-structure mini-system (export start)
  (open mini-scheme
	mini-command
	mini-for-reification
	mini-packages
	mini-environments		;set-interaction-environment!
        usual-resumer
	simple-conditions handle	;error? with-handler
	simple-signals)			;error
  (files (debug mini-start)))

(define (link-mini-system)
  (link-reified-system (list (cons 'scheme mini-scheme)
			     (cons 'write-images write-images)
			     (cons 'primitives primitives) ;just for fun
			     (cons 'usual-resumer usual-resumer)
			     (cons 'command mini-command))
		       '(scheme/debug mini)
		       'start
		       mini-system mini-for-reification))



; --------------------
; S-expression (nodes, really) interpreter

(define-structure run evaluation-interface
  (open scheme-level-2
	tables
	packages        	;package-uid package->environment link!
	compiler-envs		;bind-source-filename
	reading-forms		;read-forms $note-file-package
	syntactic		;scan-forms expand-forms
	locations
	nodes
	bindings
	meta-types
	mini-environments
	simple-signals
	fluids)
  (files (debug run)))


; Hack: an interpreter-based system.

(define (link-medium-system)		;cf. initial.scm

  (def medium-scheme (make-scheme environments run))

  (let ()

    (def command (make-mini-command medium-scheme))

    (let ()

      (def medium-system
	;; Cf. initial-packages.scm
	(make-initial-system medium-scheme command))

      (let ((structs (list (cons 'scheme medium-scheme)
			   (cons 'primitives primitives) ;just for fun
			   (cons 'usual-resumer usual-resumer)
			   (cons 'command command))))

	(link-reified-system structs
			     '(scheme/debug medium)
			     `(start ',(map car structs))
			     medium-system for-reification)))))

;;; load this into a Scheme implementation you trust, call TEST-ALL
;;; and (print-results "t1"). Repeate the same for the untrusted
;;; Scheme with a different filename and compare the files using diff.
(define-structure test-bignum (export test-all print-results)
  (open scheme
	i/o
	bitwise)
  (begin 

    (define *tests* '())
    (define (add-test! test) (set! *tests* (cons test *tests*)))
    (define (test-all) (for-each (lambda (t) (t)) *tests*))

    (define *results* '())
    (define (print-results fname)
      (with-output-to-file fname 
	(lambda () 
	  (for-each (lambda (x) (display x)(newline)) *results*))))

    (define (add! e) (set! *results* (cons e *results*)))

    
    (define (square-map f l1 l2)
      (if (null? l1)
	  '()
	  (letrec ((one-map (lambda (e1)
			      (map (lambda (e2)
				     (add! (f e1 e2))) 
				   l2))))
	    (cons (one-map (car l1))
		  (square-map f (cdr l1) l2)))))

    (define (printing-map f l)
      (for-each add!
		(map f l)))

    (define small-args '(-1234 -23 -2 -1 1 2 23 1234))
    (define fixnum-args (append (list -536870912 -536870911 536870911)
				small-args))
    (define usual-args 
      (append (list -12345678901234567890 -1234567890 -536870913 536870912 
		    536870913 1234567890 12345678901234567890)
	      fixnum-args))
	      
    (define small-args/0 (cons 0 small-args))
    (define fixnum-args/0 (cons 0 fixnum-args))
    (define usual-args/0 (cons 0 usual-args))
    
    
    (add-test! (lambda () (square-map + usual-args/0 usual-args/0)))
    (add-test! (lambda () (square-map - usual-args/0 usual-args/0)))
    (add-test! (lambda () (square-map * usual-args/0 usual-args/0)))
    
    (add-test! (lambda () (square-map /         usual-args/0 usual-args)))
    (add-test! (lambda () (square-map quotient  usual-args/0 usual-args)))
    (add-test! (lambda () (square-map remainder usual-args/0 usual-args)))
    
    (add-test! (lambda () (square-map arithmetic-shift usual-args/0 small-args)))

    (add-test! (lambda () (square-map bitwise-and usual-args/0 usual-args/0)))
    (add-test! (lambda () (square-map bitwise-ior usual-args/0 usual-args/0)))
    (add-test! (lambda () (square-map bitwise-xor usual-args/0 usual-args/0)))

    (add-test! (lambda () (printing-map bitwise-not usual-args/0)))
;    (add-test! (lambda () (printing-map bit-count usual-args/0)))

    (add-test! (lambda () (square-map <  usual-args/0 usual-args/0)))
    (add-test! (lambda () (square-map >  usual-args/0 usual-args/0)))
    (add-test! (lambda () (square-map <= usual-args/0 usual-args/0)))
    (add-test! (lambda () (square-map >= usual-args/0 usual-args/0)))
    (add-test! (lambda () (square-map =  usual-args/0 usual-args/0)))

    (add-test! (lambda () (printing-map abs usual-args/0)))
;    (add-test! (lambda () (printing-map (lambda (x) (angle (abs x))) usual-args/0)))

    (add-test! 
     (lambda () 
       (map (lambda (unary)
	      (printing-map unary usual-args/0))
	    (list integer? rational? real? complex? exact? real-part 
		  imag-part floor numerator denominator))))
    ))
    

		 

    
    
