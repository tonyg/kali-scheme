; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


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
 (cons 'byte-vector
       (let ((z (code-vector-length obj)))
         (do ((i (- z 1) (- i 1))
              (l '() (cons (code-vector-ref obj i) l)))
             ((< i 0) l))))
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
	  (channel-id obj)
	  (channel-os-index obj))))

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
