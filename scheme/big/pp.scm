; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

;;;; A pretty-printer

; This isn't exactly in the spirit of the rest of the Scheme 48
; system.  It's too hairy, and it has unexploited internal generality.
; It really ought to be rewritten.  In addition, it seems to be buggy
; -- it sometimes prints unnecessarily wide lines.  Usually it's
; better than no pretty printer at all, so we tolerate it.

; From: ramsdell@linus.mitre.org
; Date:  Wed, 12 Sep 1990 05:14:49 PDT
;
; As you noted in your comments, pp.scm is not a straight forward
; program.  You could add some comments that would greatly ease the task
; of figuring out what his going on.  In particular, you should describe
; the interface of various objects---most importantly the interface of a
; formatter.  You might also add some description as to what protocol
; they are to follow.

; Other things to implement some day:
;  - LET, LET*, LETREC binding lists should be printed vertically if longer
;    than about 30 characters
;  - COND clauses should all be printed vertically if the COND is vertical
;  - Add an option to lowercase or uppercase symbols and named characters.
;  - Parameters controlling behavior of printer should be passed around
;  - Do something about choosing between #f and ()
;  - Insert line breaks intelligently following head of symbol-headed list,
;    when necessary
;  - Some equivalents of *print-level*, *print-length*, *print-circle*.

; Possible strategies:
;   (foo x y z)     Horizontal = infinity sticky 
;   (foo x y        One sticky + one + body (e.g. named LET)
;     z
;     w)
;   (foo x          One + body
;     y
;     z)
;   (foo x          Two + body
;        y
;     z)
;   (foo x          Big ell = infinity + body (combination)
;	 y
;	 z)
;   (foo            Little ell, zero + body (combination)
;     x
;     y)
;   (foo            Vertical
;    x
;    y)
;
; Available height/width tradeoffs:
;   Combination:
;     Horizontal, big ell, or little ell.
;   Special form:
;     Horizontal, or M sticky + N + body.
;   Random (e.g. vector, improper list, non-symbol-headed list):
;     Horizontal, or vertical.  (Never zero plus body.)

(define (p x . port-option)
  (let ((port (if (pair? port-option) (car port-option)
		  (current-output-port))))
    (pretty-print x port 0)
    (newline port)))

(define *line-width* 80)

(define *single-line-special-form-limit* 30)

; Stream primitives

(define head car)
(define (tail s) (force (cdr s)))

(define (map-stream proc stream)
  (cons (proc (head stream))
	(delay (map-stream proc (tail stream)))))

(define (stream-ref stream n)
  (if (= n 0)
      (head stream)
      (stream-ref (tail stream) (- n 1))))

; Printer

(define (pretty-print obj port pos)
  (let ((node (pp-prescan obj 0)))
;    (if (> (column-of (node-dimensions node)) *line-width*)
;        ;; Eventually add a pass to change format of selected combinations
;        ;; from big-ell to little-ell.
;        (begin (display ";** too wide - ")
;               (write (node-dimensions node))
;               (newline)))
    (print-node node port pos)))

(define make-node list)

(define (node-dimensions node)
  ((car node)))

(define (node-pass-2 node pos)
  ((cadr node) pos))

(define (print-node node port pos)
  ((caddr node) port pos))

(define (pp-prescan obj hang)
  (cond ((symbol? obj)
         (make-leaf (string-length (symbol->string obj))
                    obj hang))
        ((number? obj)
         (make-leaf (string-length (number->string obj))
                    obj hang))
        ((boolean? obj)
         (make-leaf 2 obj hang))
        ((string? obj)
         ;;++ Should count number of backslashes and quotes
         (make-leaf (+ (string-length obj) 2) obj hang))
        ((char? obj)
         (make-leaf (case obj
                      ((#\space) 7)
                      ((#\newline) 9)
                      (else 3))
                    obj hang))
        ((pair? obj)
         (pp-prescan-pair obj hang))
        ((vector? obj)
         (pp-prescan-vector obj hang))
	(else
	 (pp-prescan-random obj hang))))

(define (make-leaf width obj hang)
  (let ((width (+ width hang)))
    (make-node (lambda () width)
	       (lambda (pos)
		 (+ pos width))
	       (lambda (port pos)
		 (write obj port)
		 (do ((i 0 (+ i 1)))
		     ((>= i hang) (+ pos width))
		   (write-char #\) port))))))

(define (make-prefix-node string node)
  (let ((len (string-length string)))
    (make-node (lambda ()
		 (+ (node-dimensions node) len))
	       (lambda (pos)
		 (node-pass-2 node (+ pos len)))
	       (lambda (port pos)
		 (display string port)
		 (print-node node port (+ pos len))))))

(define (pp-prescan-vector obj hang)
  (if (= (vector-length obj) 0)
      (make-leaf 3 obj hang)
      (make-prefix-node "#" (pp-prescan-list (vector->list obj) #t hang))))

; Improve later.

(define (pp-prescan-random obj hang)
  (let ((l (disclose obj)))
    (if (list? l)
	(make-prefix-node "#." (pp-prescan-list l #t hang))
	(make-leaf 25 obj hang))))  ;Very random number

(define (pp-prescan-pair obj hang)
  (cond ((read-macro-inverse obj)
         =>
         (lambda (inverse)
	   (make-prefix-node inverse (pp-prescan (cadr obj) hang))))
        (else
         (pp-prescan-list obj #f hang))))

(define (pp-prescan-list obj random? hang)
  (let loop ((l obj) (z '()))
    (if (pair? (cdr l))
	(loop (cdr l)
	      (cons (pp-prescan (car l) 0) z))
	(make-list-node
	  (reverse
	    (if (null? (cdr l))
		(cons (pp-prescan (car l) (+ hang 1)) z)
		(cons (make-prefix-node ". " (pp-prescan (cdr l) (+ hang 1)))
		      (cons (pp-prescan (car l) 0) z))))
	  obj
	  (or random? (not (null? (cdr l))))))))

; Is it sufficient to tell parent node:
;  At a cost of X line breaks, I can make myself narrower by Y columns. ?
; Then how do we decide whether we narrow ourselves or some of our children?

(define (make-list-node node-list obj random?)
  (let* ((random? (or random?
		      ;; Heuristic for things like do, cond, let, ...
		      (not (symbol? (car obj)))
		      (eq? (car obj) 'else)))
	 (probe (if (not random?)
		    (indentation-for (car obj))
		    #f))
	 (format horizontal-format)
	 (dimensions (compute-dimensions node-list format))
	 (go-non-horizontal
	  (lambda (col)
	    (set! format
		  (cond (random? vertical-format)
			(probe (probe obj))
			(else big-ell-format)))
	    (let* ((start-col (+ col 1))
		   (col (node-pass-2 (car node-list) start-col))
		   (final-col
		       (format (cdr node-list) 
			       (lambda (node col target-col)
				 (node-pass-2 node target-col))
			       start-col
			       (+ col 1)
			       col)))
	      (set! dimensions (compute-dimensions node-list format))
	      final-col))))
    (if (> dimensions
	   (if probe
	       *single-line-special-form-limit*
	       *line-width*))
	(go-non-horizontal 0))
    (make-node (lambda () dimensions)
	       (lambda (col) ;Pass 2: if necessary, go non-horizontal
		 (let ((defacto (+ col (column-of dimensions))))
		   (if (> defacto *line-width*)
		       (go-non-horizontal col)
		       defacto)))
	       (lambda (port pos)
		 (write-char #\( port)
		 (let* ((pos (+ pos 1))
			(start-col (column-of pos))
			(pos (print-node (car node-list) port pos)))
		   (format (cdr node-list) 
			   (lambda (node pos target-col)
			     (let ((pos (go-to-column target-col
						      port pos)))
			       (print-node node port pos)))
			   start-col
			   (+ (column-of pos) 1)
			   pos))))))

(define (compute-dimensions node-list format)
  (let* ((start-col 1)			;open paren
	 (pos (+ (make-position start-col 0)
		 (node-dimensions (car node-list)))))
    (format (cdr node-list)
	    (lambda (node pos target-col)
	      (let* ((dims (node-dimensions node))
		     (lines (+ (line-of pos) (line-of dims)))
		     (width (+ target-col (column-of dims))))
		(if (>= (column-of pos) target-col)
		    ;; Line break required
		    (make-position
		     (max (column-of pos) width)
		     (+ lines 1))
		    (make-position width lines))))
	    start-col
	    (+ (column-of pos) 1)	;first-col
	    pos)))

; Three positions are significant
;   (foo baz ...)
;    ^   ^  ^
;    |   |  +--- (column-of pos)
;    |   +------ first-col
;    +---------- start-col

; Separators

(define on-same-line
  (lambda (start-col first-col pos)
    start-col first-col ;ignored
    (+ (column-of pos) 1)))

(define indent-under-first
  (lambda (start-col first-col pos)
    start-col ;ignored
    first-col))

(define indent-for-body
  (lambda (start-col first-col pos)
    first-col ;ignored
    (+ start-col 1)))

(define indent-under-head
  (lambda (start-col first-col pos)
    first-col ;ignored
    start-col))

; Format constructors

(define (once separator format)
  (lambda (tail proc start-col first-col pos)
    (if (null? tail)
	pos
	(let ((target-col (separator start-col first-col pos)))
	  (format (cdr tail)
		  proc
		  start-col
		  first-col
		  (proc (car tail) pos target-col))))))

(define (indefinitely separator)
  (letrec ((self (once separator	;eta
		       (lambda (tail proc start-col first-col pos)
			 (self tail proc start-col first-col pos)))))
    self))

(define (repeatedly separator count format)
  (do ((i 0 (+ i 1))
       (format format
	       (once separator format)))
      ((>= i count) format)))

; Particular formats

(define vertical-format
  (indefinitely indent-under-head))

(define horizontal-format
  (indefinitely on-same-line))

(define big-ell-format
  (indefinitely indent-under-first))

(define little-ell-format
  (indefinitely indent-for-body))

(define format-for-named-let
  (repeatedly on-same-line 2 (indefinitely indent-for-body)))

(define hook-formats
  (letrec ((stream (cons little-ell-format
			 (delay (map-stream (lambda (format)
					      (once indent-under-first format))
					    stream)))))
    stream))

; Hooks for special forms.
; A hook maps an expression to a format.

(define (compute-let-indentation exp)
  (if (and (not (null? (cdr exp)))
	   (symbol? (cadr exp)))
      format-for-named-let
      (stream-ref hook-formats 1)))

(define hook
  (let ((hooks (map-stream (lambda (format)
			     (lambda (exp) exp ;ignored
			       format))
			   hook-formats)))
    (lambda (n)
      (stream-ref hooks n))))


; Table of indent hooks.

(define indentations (make-table))

(define (indentation-for name)
  (table-ref indentations name))

(define (define-indentation name n)
  (table-set! indentations
	      name
	      (if (integer? n) (hook n) n)))

; Indent hooks for Revised^n Scheme.

(for-each (lambda (name)
	    (define-indentation name 1))
	  '(lambda define define-syntax let* letrec let-syntax letrec-syntax
	    case call-with-values call-with-input-file
	    call-with-output-file with-input-from-file
	    with-output-to-file syntax-rules))

(define-indentation 'do            2)
(define-indentation 'call-with-current-continuation 0)

(define-indentation 'let           compute-let-indentation)

; Kludge to force vertical printing (do AND and OR as well?)
(define-indentation 'if            (lambda (exp) big-ell-format))
(define-indentation 'cond          (lambda (exp) big-ell-format))


; Other auxiliaries

(define (go-to-column target-col port pos) ;=> pos
  ;; Writes at least one space or newline
  (let* ((column (column-of pos))
	 (line (if (>= column target-col)
		   (+ (line-of pos) 1)
		   (line-of pos))))
    (do ((column (if (>= column target-col)
		     (begin (newline port) 0)
		     column)
		 (+ column 1)))
	((>= column target-col)
	 (make-position column line))
      (write-char #\space port))))

(define (make-position column line)
  (+ column (* line 1000)))

(define (column-of pos)
  (remainder pos 1000))

(define (line-of pos)
  (quotient pos 1000))

(define (read-macro-inverse x)
  (cond ((and (pair? x)
              (pair? (cdr x))
              (null? (cddr x)))
         (case (car x)
           ((quote)            "'")
           ((quasiquote)       "`")
           ((unquote)          ",")
           ((unquote-splicing) ",@")
           (else #f)))
        (else #f)))

; For the command processor:

;(define-command 'p "<exp>" "pretty-print" '(expression)
;  (p (eval expression (user-package)) (command-output)))
