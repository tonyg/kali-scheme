; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Lexer for Infix Scheme (JAR's obscure syntax)
; Bears no relation to Pratt's CGOL

; To do: add ML-ish binding constructs.

;  (sgol-read)  reads an expression
;
;  semicolon    terminates input
;  comment character is # (comment goes to end of line)
;
;  f(x, y)  reads as  (f x y)
;
;  if x then y else z      reads as  (if x y z)
;  x and y, x or y, not x  do the obvious thing
;
;  x + y    reads as  (+ x y)     - similarly for - * / = < > <= >=
;
;  x::y     reads as  (cons x y)     - ML's syntax
;  x++y     reads as  (append x y)   - whose syntax?  Haskell's?
;  []            reads as  '()
;  [a, b, ...]   reads as  (list a b ...)
;
;  ()            reads as  the-unit
;  (x, y, ...)   reads as  (tuple x y ...)
;
;  a[i]		 reads as  (vector-ref a i)
;  a[i, j, ...]  reads as  (array-ref a i j ...)
;
;  x := y       reads as  (set! x y)
;  car(x) := y  reads as  (set-car! x y)       - similarly for cdr
;  x[y] := z    reads as  (vector-set! x y z)  - similarly for array-ref
;
;  'foo'   tries to read as 'foo  but usually loses


(define sgol-lexer-table (make-lexer-table))

(set-char-tokenization! (lexer-ttab sgol-lexer-table)
			#\#
			(lambda (c port)
			  c		;ignored
			  (gobble-line port)
			  (read port))
			#t)

(define (gobble-line port)
  (let loop ()
    (let ((c (read-char port)))
      (cond ((eof-object? c) c)
	    ((char=? c #\newline) #f)
	    (else (loop))))))

;

(define (define-sgol-keyword name op)
  (define-keyword sgol-lexer-table name op))

(define (define-sgol-punctuation string op)
  (define-punctuation sgol-lexer-table string op))

; Arguments to make-operator are: name lbp rbp nud led

(define (open-paren-nud token stream)
  (let ((right (prsmatch close-paren-operator stream)))
    (if (null? right)
	'the-unit			; ()
	(if (null? (cdr right))
	    (car right)			; (x)
	    (cons 'tuple right)))))	; (x, y, ..., z)

; f(x, y) reads as (f x y)
; f((x, y)) reads as (f (tuple x y))

(define (open-paren-led token left stream)
  (cons left (prsmatch close-paren-operator stream)))

(define-sgol-punctuation "("
  (make-operator 'open-paren 200 #f open-paren-nud open-paren-led))

(define-sgol-punctuation "," comma-operator)

(define close-paren-operator
  (make-operator 'close-paren 5 #f delim-error erb-error))
(define-sgol-punctuation ")" close-paren-operator)

; Boolean operators

(define-sgol-keyword 'true '#t)
(define-sgol-keyword 'false '#f)

(define-sgol-keyword 'if if-operator)
(define-sgol-keyword 'then then-operator)
(define-sgol-keyword 'else else-operator)

(define-sgol-keyword 'not (make-operator 'not 70 70 parse-prefix #f))
(define-sgol-keyword 'and (make-operator 'and 65 #f #f parse-nary))
(define-sgol-keyword 'or  (make-operator 'or  60 #f #f parse-nary))

; Lists

(define (open-bracket-nud token stream)
  (let ((elements (prsmatch close-bracket-operator stream)))
    (if (null? elements)
	`'()
	`(list ,@elements))))

(define (open-bracket-led token left stream)
  (let ((subscripts (prsmatch close-bracket-operator stream)))
    (if (and (not (null? subscripts))
	     (null? (cdr subscripts)))
	`(vector-ref ,left ,@subscripts)
	`(array-ref ,left ,@subscripts))))

(define-sgol-punctuation "["
  (make-operator 'open-bracket 200 #f open-bracket-nud open-bracket-led))

(define close-bracket-operator
  (make-operator 'close-bracket 5 #f delim-error erb-error))
(define-sgol-punctuation "]" close-bracket-operator)

(define-sgol-punctuation "::"
  (make-operator 'cons 75 74 #f parse-infix))

(define-sgol-punctuation "++"
  (make-operator 'append 75 74 #f parse-nary))

; Quotation

(define-sgol-punctuation "'"
  (make-operator 'quote 5 #f parse-matchfix #f)) ;This isn't right

; Arithmetic

(define-sgol-punctuation "+"
  (make-operator '+ 100 100 parse-prefix parse-infix))

(define-sgol-punctuation "-"
  (make-operator '- 100 100 parse-prefix parse-infix))

(define-sgol-punctuation "*"
  (make-operator '* 120 120 #f parse-infix))  ;should be parse-nary

(define-sgol-punctuation "/"
  (make-operator '/ 120 120 #f parse-infix))

(define-sgol-punctuation "="
  (make-operator '= 80 80 #f parse-infix))

(define-sgol-punctuation ">"
  (make-operator '> 80 80 #f parse-infix))

(define-sgol-punctuation "<"
  (make-operator '< 80 80 #f parse-infix))

(define-sgol-punctuation ">="
  (make-operator '>= 80 80 #f parse-infix))

(define-sgol-punctuation "<="
  (make-operator '<= 80 80 #f parse-infix))

(define-sgol-punctuation "!="
  (make-operator '!= 80 80 #f parse-infix))

; Side effects

(define (:=-led token left stream)
  (let* ((form (parse-infix token left stream))
	 (lhs (cadr form))
	 (rhs (caddr form)))
    (if (pair? lhs)
	(case (car lhs)
	  ((car) `(set-car! ,@(cdr lhs) ,rhs))
	  ((cdr) `(set-cdr! ,@(cdr lhs) ,rhs))
	  ((vector-ref) `(vector-set! ,@(cdr lhs) ,rhs))
	  ((array-ref) `(array-set! ,@(cdr lhs) ,rhs))
	  (else (error "invalid LHS for :=" form)))
	form)))

(define-sgol-punctuation ":="
  (make-operator 'set! 70 #f #f :=-led))

; End of input...

(define-sgol-punctuation ";" end-of-input-operator)

; Read using Pratt parser with SGOL tokenizer table

(define (sgol-read . port-option)
  (toplevel-parse (port->stream (if (null? port-option)
				    (current-input-port)
				    (car port-option))
				sgol-lexer-table)))

; Read/eval/print loop

(define (sgol-repl)
  (let ((thing (sgol-read)))
    (if (not (eq? thing end-of-input-operator))
	(begin (write thing)
	       (newline)
	       (rpl)))))
