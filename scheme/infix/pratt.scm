; -*- Mode: Scheme; -*-
;
; A simple Pratt-Parser for SIOD: 2-FEB-90, George Carrette, GJC@PARADIGM.COM
; Siod may be obtained by anonymous FTP to world.std.com:pub/gjc.
;
; *                      COPYRIGHT (c) 1988-1994 BY                          *
; *        PARADIGM ASSOCIATES INCORPORATED, CAMBRIDGE, MASSACHUSETTS.       *
; *                         ALL RIGHTS RESERVED                              *
;
;Permission to use, copy, modify, distribute and sell this software
;and its documentation for any purpose and without fee is hereby
;granted, provided that the above copyright notice appear in all copies
;and that both that copyright notice and this permission notice appear
;in supporting documentation, and that the name of Paradigm Associates
;Inc not be used in advertising or publicity pertaining to distribution
;of the software without specific, written prior permission.
;
;PARADIGM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
;ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
;PARADIGM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
;ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
;WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
;ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
;SOFTWARE.
;
; Based on a theory of parsing presented in:                       
;                                                                      
;  Pratt, Vaughan R., ``Top Down Operator Precedence,''         
;  ACM Symposium on Principles of Programming Languages         
;  Boston, MA; October, 1973.                                   
;                                                                      

; The following terms may be useful in deciphering this code:

; NUD -- NUll left Denotation (op has nothing to its left (prefix))
; LED -- LEft Denotation      (op has something to left (postfix or infix))

; LBP -- Left Binding Power  (the stickiness to the left)
; RBP -- Right Binding Power (the stickiness to the right)
;

; Mods for Scheme 48 by J Rees 6-14-90

; From: <gjc@mitech.com>
;
; Now a neat thing that CGOL had was a way of packaging and scoping
; different parsing contexts. The maclisp implementation was simple,
; instead of just NUD and LED and other properties there was a list
; of property indicators. And a lookup operation.
;
; One use of the local-context thing, in parsing the C language
; you can use a different binding-power for ":" depending on
; what kind of statement you are parsing, a general statement
; context where ":" means a label, a "switch" or the "if for value
; " construct of (a > b) > c : d;


(define (peek-token stream)
  (stream 'peek #f))

(define (read-token stream)
  (stream 'get #f))
   
(define (toplevel-parse stream)
  (if (eq? end-of-input-operator (peek-token stream))
      (read-token stream)
      (parse -1 stream)))


; A token is either an operator or atomic (number, identifier, etc.)

(define-record-type operator :operator
  (really-make-operator name lbp rbp nud led)
  operator?
  (name operator-name)
  (lbp operator-lbp)
  (rbp operator-rbp)
  (nud operator-nud)
  (led operator-led))

(define (make-operator name lbp rbp nud led)
  (really-make-operator name
			(or lbp default-lbp)
			(or rbp default-rbp)
			(or nud default-nud)
			(or led default-led)))

(define (default-nud operator stream)
  (if (eq? (operator-led operator) default-led)
      operator
      (error 'not-a-prefix-operator operator)))

(define (nudcall token stream)
  (if (operator? token)
      ((operator-nud token) token stream)
      token))

(define default-led #f)

;+++ To do: fix this to make juxtaposition work   (f x+y)

(define (ledcall token left stream)
  ((or (and (operator? token)
	    (operator-led token))
       (error 'not-an-infix-operator token))
   token
   left
   stream))

(define default-lbp 200)

(define (lbp token)
  (if (operator? token)
      (operator-lbp token)
      default-lbp))

(define default-rbp 200)

(define (rbp token)
  (if (operator? token)
      (operator-rbp token)
      default-rbp))

(define-record-discloser :operator
  (lambda (obj)
    (list 'operator (operator-name obj))))

; Mumble

(define (delim-error token stream)
  (error 'invalid-use-of-delimiter token))

(define (erb-error token left stream)
  (error 'too-many-right-parentheses token))

(define (premterm-err token stream)
  (error 'premature-termination-of-input token))

; Parse

(define *parse-debug* #f)

(define (parse rbp-level stream)
  (if *parse-debug* (print `(parse ,rbp-level)))
  (let parse-loop ((translation (nudcall (read-token stream) stream)))
    (if (< rbp-level (lbp (peek-token stream)))
	(parse-loop (ledcall (read-token stream) translation stream))
      (begin (if *parse-debug* (print translation))
	     translation))))

(define (print s) (write s) (newline))

(define (parse-prefix operator stream)
  (list (operator-name operator)
	(parse (rbp operator) stream)))

(define (parse-infix operator left stream)
  (list (operator-name operator)
	left
	(parse (rbp operator) stream)))

(define (parse-nary operator left stream)
  (cons (operator-name operator) (cons left (prsnary operator stream))))

(define (prsnary operator stream)
  (define (loop l)
    (if (eq? operator (peek-token stream))
	(begin (read-token stream)
	       (loop (cons (parse (rbp operator) stream) l)))
      (reverse l)))
  (loop (list (parse (rbp operator) stream))))

; Parenthesis matching, with internal commas.
; Kind of a kludge if you ask me.

(define (parse-matchfix operator stream) ; |x|
  (cons (operator-name operator)
	(prsmatch operator stream)))

(define (prsmatch close-op stream)
  (if (eq? (peek-token stream) close-op)
      (begin (read-token stream)
	     '())
      (let loop ((l (list (parse 10 stream))))
	(if (eq? (peek-token stream) close-op)
	    (begin (read-token stream)
		   (reverse l))
	    (if (eq? (peek-token stream) comma-operator)
		(begin (read-token stream)
		       (loop (cons (parse 10 stream) l)))
		(error 'comma-or-match-not-found (read-token stream)))))))

(define comma-operator (make-operator 'comma 10 #f delim-error #f))

; if A then B [else C]

(define (if-nud token stream)
  (let* ((pred (parse (rbp token) stream))
	 (then (if (eq? (peek-token stream) then-operator)
		   (parse (rbp (read-token stream)) stream)
		   (error 'missing-then pred))))
    (if (eq? (peek-token stream) else-operator)
	`(if ,pred ,then ,(parse (rbp (read-token stream)) stream))
	`(if ,pred ,then))))

(define if-operator (make-operator 'if #f 45 if-nud #f))
(define then-operator (make-operator 'then 5 25 delim-error #f))
(define else-operator (make-operator 'else 5 25 delim-error #f))

; Lexer support:

(define-record-type lexer :lexer
  (make-lexer ttab punctab keytab)
  lexer?
  (ttab lexer-ttab)
  (punctab lexer-punctab)
  (keytab lexer-keytab))

(define (make-lexer-table)
  (let ((ttab (make-tokenizer-table)))
    (set-up-usual-tokenization! ttab)
    (make-lexer ttab (make-table) (make-table))))

(define (lex ltab port)
  (let ((thing (tokenize (lexer-ttab ltab) port)))
    (cond ((eof-object? thing)
	   end-of-input-operator)
	  ((symbol? thing)
	   (or (table-ref (lexer-keytab ltab) thing)
	       thing))
	  (else thing))))

; Keywords

(define (define-keyword ltab name op)
  (table-set! (lexer-keytab ltab) name op))

; Punctuation

; lexnode = (* operator (table-of char (+ lexnode #f)))  -- discrimination tree

(define (define-punctuation ltab string op)
  (let ((end (- (string-length string) 1)))
    (let loop ((i 0)
	       (table (lexer-punctab ltab)))
      (let* ((c (string-ref string i))
	     (lexnode
	      (or (table-ref table c)
		  (let ((lexnode
			 (cons (error-operator (substring string 0 (+ i 1)))
			       (make-table))))
		    (table-set! table c lexnode)
		    (if (= i 0)
			(set-char-tokenization! (lexer-ttab ltab)
						c
						(operator-reader lexnode)
						#t))
		    lexnode))))
	(if (>= i end)
	    (set-car! lexnode op)
	    (loop (+ i 1) (cdr lexnode)))))))

(define (operator-reader lexnode)
  (lambda (c port)
    (let loop ((lexnode lexnode))
      (let ((nextc (peek-char port)))
	(let ((nextnode (table-ref (cdr lexnode) nextc)))
	  (if nextnode
	      (begin (read-char port)
		     (loop nextnode))
	      (car lexnode)))))))

(define (error-operator string)
  (make-operator 'invalid-operator #f #f
		 (lambda rest (error "invalid operator" string))
		 #f))

; Mumble

(define end-of-input-operator
  (make-operator "end of input" -1 #f premterm-err #f))

(define (port->stream port ltab)
  (define (really-get)
    (lex ltab port))
  (define peeked? #f)
  (define peek #f)
  (define (stream op arg)
    (case op
      ((get) (if peeked?
		 (begin (set! peeked? #f) peek)
		 (really-get)))
      ((peek) (if peeked?
		  peek
		  (begin (set! peeked? #t)
			 (set! peek (really-get))
			 peek)))))
  stream)
