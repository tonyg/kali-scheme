; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; --------------------
; DISCLOSE methods

(define-method &disclose ((obj :closure))
  (let ((id (template-id (closure-template obj)))
        (name (template-print-name (closure-template obj))))
    (if name
        (list 'procedure
              id
              ;; A heuristic that sometimes loses.
;                 (if (and (pair? name)
;                          (eq? (car name) '#t) ;Curried
;                          (vector? (closure-env obj)))
;                     (error-form
;                       (if (null? (cdddr name))
;                           (caddr name)
;                           (cdddr name))
;                       (reverse (cdr (vector->list (closure-env obj)))))
;                     name)
              name)
        (list 'procedure id))))

(define-method &disclose ((obj :template))
  (let ((id (template-id obj))
        (name (template-print-name obj)))
    (if name
        (list 'template id name)
        (list 'template id))))

(define-method &disclose ((obj :location))
  (cons 'location
        (cons (location-id obj)
              (let ((name (location-name obj)))
                (if (and name (not (eq? name (location-id obj))))
                    (list name (location-package-name obj))
                    '())))))

(define-method &disclose ((obj :cell))
  (if (cell-unassigned? obj)
      (list 'uninitialized-cell)
      (list 'cell)))

;; this overwrites the method defined in rts/continuation.scm

(define-method &disclose ((obj :continuation))
  (list (if (vm-exception-continuation? obj)
	    'vm-exception-continuation
	    'continuation)
	(list 'pc (continuation-pc obj))
	(let ((tem (continuation-template obj)))
	  (if tem
	      (or (template-print-name tem) ; <-- the original method doesn't have this
		  (template-id tem))
	      '?))))
  
(define-method &disclose ((obj :code-vector))
  (list 'byte-vector (code-vector-length obj))
; (cons 'code-vector
;       (let ((z (code-vector-length obj)))
;         (do ((i (- z 1) (- i 1))
;              (l '() (cons (code-vector-ref obj i) l)))
;             ((< i 0) l))))
  )

(define-method &disclose ((obj :channel))
  (let ((status (channel-status obj)))
    (list (cond ((= status (enum channel-status-option closed))
		 'closed-channel)
		((or (= status (enum channel-status-option input))
		     (= status (enum channel-status-option special-input)))
		 'input-channel)
		((or (= status (enum channel-status-option output))
		     (= status (enum channel-status-option special-output)))
		 'output-channel)
		(else	; shouldn't happen unless we get out of sync
		 'unknown-channel))
	  (channel-id obj))))

(define-method &disclose ((obj :port))
  (disclose-port obj))

(define (template-print-name tem)
  (make-print-name (template-names tem)))

(define (make-print-name names)
  (if (null? names)
      #f
      (let ((name (car names))
            (parent-name (make-print-name (cdr names))))
        (cond (parent-name
               `(,(if name name 'unnamed)
                 in
                 ,@(if (pair? parent-name) parent-name (list parent-name))))
              ((string? name) #f)  ;File name
              (else name)))))

(define (template-file-name tem)
  (let loop ((names (template-names tem)))
    (if (null? names)
        #f
        (if (string? (car names))
            (car names)
            (loop (cdr names))))))

; --------------------
; Location names

(define (location-info loc)
  (let ((id (location-id loc)))
    (if (integer? id)
        (table-ref location-info-table id)
        #f)))

(define (location-name loc)
  (let ((probe (location-info loc)))
    (if probe
        (car probe)
        #f)))

(define (location-package-name loc)
  (let ((probe (location-info loc)))
    (if probe
        (table-ref package-name-table (cdr probe))
        #f)))


; --------------------
; Condition disclosers

(define *condition-disclosers* '())

(define (define-condition-discloser pred proc)
  (set! *condition-disclosers*
        (cons (cons pred proc) *condition-disclosers*)))

; Simple conditions

(define-method &disclose-condition ((c :pair))
  (let loop ((l *condition-disclosers*))
    (if (null? l)
        (cons (cond ((error? c) 'error)
                    ((warning? c) 'warning)
                    (else (car c)))
              (condition-stuff c))
        (if ((caar l) c)
            ((cdar l) c)
            (loop (cdr l))))))

(define-condition-discloser interrupt?
  (lambda (c)
    (list 'interrupt (enumerand->name (cadr c) interrupt))))
        

; Make prettier error messages for VM exceptions

(define-condition-discloser vm-exception?
  (lambda (c)
    (disclose-vm-condition (vm-exception-opcode c)
			   (vm-exception-reason c)
			   (vm-exception-arguments c))))

(define vm-exception-disclosers
  (make-vector op-count
               (lambda (opcode reason args)
                 (list 'error
                       "vm-exception"
		       reason
                       (let ((name (enumerand->name opcode op)))
                         (if (>= opcode (enum op eq?))
                             (error-form name args)
                             (cons name args)))))))

(define (define-vm-exception-discloser opcode discloser)
  (vector-set! vm-exception-disclosers opcode discloser))

(define (disclose-vm-condition opcode reason args)
  ((vector-ref vm-exception-disclosers opcode)
   opcode
   reason
   args))

(let ((disc (lambda (opcode reason args)
              (let ((loc (car args)))
                (cons 'error
                      (cons (if (location-defined? loc)
                                "unassigned variable"
                                "undefined variable")
                            (cons (or (location-name loc) loc)
                                  (let ((pack
                                         (location-package-name loc)))
                                    (if pack
                                        (list (list 'package pack))
                                        '())))))))))
  (define-vm-exception-discloser (enum op global) disc)
  (define-vm-exception-discloser (enum op set-global!) disc))

(define-vm-exception-discloser (enum op unassigned-check)
  (lambda (opcode reason args)
    (list 'error
	  "LETREC variable used before its value has been produced")))

(let ((disc (lambda (opcode reason args)
	      (list 'error
		    (case reason
		      ((bad-procedure)
		       "attempt to call a non-procedure")
		      ((wrong-number-of-arguments)
		       "wrong number of arguments")
		      ((wrong-type-argument)
		       "wrong type argument")
		      (else
		       (symbol->string reason)))
		    (map value->expression (cons (car args) (cadr args)))))))
  (define-vm-exception-discloser (enum op call) disc)
  (define-vm-exception-discloser (enum op tail-call) disc)
  (define-vm-exception-discloser (enum op big-call) disc))

(let ((disc (lambda (opcode reason args)
	      (list 'error
		    (case reason
		      ((bad-procedure)
		       "with-continuation passed a non-procedure")
		      ((wrong-number-of-arguments)
		       "with-continuation passed a non-nullary procedure")
		      (else
		       (string-append "with-continuation: "
				      (symbol->string reason))))
		    (value->expression (car args))))))
  (define-vm-exception-discloser (enum op with-continuation) disc))

(let ((disc (lambda (opcode reason args)
	      (list 'error
		    (symbol->string reason)
		    (cons 'apply (map value->expression args))))))
  (define-vm-exception-discloser (enum op apply) disc)
  (define-vm-exception-discloser (enum op closed-apply) disc))

(let ((disc (lambda (opcode reason args)
	      (let ((proc (car args))
		    (args (cadr args)))
		(cond (proc
		       (list 'error
			     "returning wrong number of values"
			     (cons proc args)))
		      ((null? args)
		       (list 'error
			     "returning zero values when one is expected"
			     '(values)))
		      (else
		       (list 'error
			     "returning wrong number of values"
			     (error-form 'values args))))))))
  (define-vm-exception-discloser (enum op return) disc)
  (define-vm-exception-discloser (enum op values) disc)
  (define-vm-exception-discloser (enum op closed-values) disc))

(let ((disc (lambda (opcode reason args)
              (let ((thing     (car args))
                    (type-byte (cadr args))
                    (offset    (caddr args))
                    (rest      (cdddr args)))
                (let ((data (assq (enumerand->name type-byte stob)
                                  stob-data)))
                  (list 'error
                        "vm-exception"
                        (error-form ((if (= opcode
					    (enum op stored-object-ref))
                                         car
                                         cadr)
                                     (list-ref data (+ offset 3)))
                                    (cons thing rest))))))))
  (define-vm-exception-discloser (enum op stored-object-ref) disc)
  (define-vm-exception-discloser (enum op stored-object-set!) disc))

(let ((disc (lambda (opcode reason args)
              (let ((type (enumerand->name (car args) stob)))
                (list 'error
                      "vm-exception"
                      (error-form (string->symbol
                                   ;; Don't simplify this to "make-"  --JAR
                                   (string-append (symbol->string 'make-)
                                                  (symbol->string type)))
                                  (cdr args)))))))
  (define-vm-exception-discloser (enum op make-vector-object) disc))

(define (vector-vm-exception-discloser suffix)
  (lambda (opcode reason args)
    (let ((type (enumerand->name (cadr args) stob)))
      (list 'error
            "vm-exception"
            (error-form (string->symbol
                         (string-append (symbol->string type)
                                        "-"
                                        (symbol->string suffix)))
                        (cons (car args) (cddr args)))))))

(define-vm-exception-discloser (enum op stored-object-length)
  (vector-vm-exception-discloser 'length))

(define-vm-exception-discloser (enum op stored-object-indexed-ref)
  (vector-vm-exception-discloser 'ref))

(define-vm-exception-discloser (enum op stored-object-indexed-set!)
  (vector-vm-exception-discloser 'set!))

;(define-vm-exception-discloser (enum op get-cont-from-heap)
;  (lambda (opcode reason args)
;    (let ((value (car args))
;          (continuation (cadr args)))
;      (if (not continuation)
;          (list 'error
;                "exit status is not a small integer"
;                (value->expression value))
;          (list 'error
;                "returning to a non-continuation"
;                (value->expression continuation))))))

(define-vm-exception-discloser (enum op scalar-value->char)
  (lambda (opcode reason args)
    (let ((value (car args)))
      `(error
	"vm-exception"
	"wrong-type-argument"
	(scalar-value->char ,(value->expression value))))))

; Call-errors should print as (proc 'arg1 'arg2 ...)

(define-condition-discloser call-error?
  (lambda (c)
    (list 'error (cadr c) (error-form (caddr c) (cdddr c)))))

; --------------------
; Associating names with templates

(define (template-debug-data tem)
  (get-debug-data (template-info tem)))

(define (template-id tem)
  (let ((info (template-info tem)))
    (if (debug-data? info)
	(debug-data-uid info)
	info)))

(define (template-name tem)
  (let ((probe (template-debug-data tem)))
    (if probe
	(debug-data-name probe)
	'?)))

(define (template-names tem)
  (debug-data-names (template-info tem)))

; We can follow parent links to get a full description of procedure
; nesting: "foo in bar in unnamed in baz"

(define (debug-data-names info)
  (let ((dd (get-debug-data info)))
    (if (debug-data? dd)     ;paranoid
	(cons (debug-data-name dd)
	      (debug-data-names (debug-data-parent dd)))
	'())))

; --------------------
; Utilities

(define (error-form proc args)
  (cons proc (map value->expression args)))

(define (value->expression obj)
  (if (or (number? obj) (char? obj) (string? obj) (boolean? obj))
      obj
      `',obj))
