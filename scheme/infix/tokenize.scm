; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.


; A tokenizer.

; Nonstandard things needed:
;  record package
;  char->ascii
;  peek-char
;  reverse-list->string
;  error

(define (reverse-list->string l n)
  (list->string (reverse l)))

; Tokenizer tables

(define-record-type tokenizer-table :tokenizer-table
  (really-make-tokenizer-table translation dispatch-vector terminating?-vector)
  tokenizer-table?
  (translation ttab-translation
	       set-tokenizer-table-translator!)
  (dispatch-vector ttab-dispatch-vector)
  (terminating?-vector ttab-terminating?-vector))

(define (make-tokenizer-table)
  (really-make-tokenizer-table
   (if (char=? (string-ref (symbol->string 't) 0) #\T)
       char-upcase
       char-downcase)
   (make-vector 256 (lambda (c port)
		      (error "illegal character read" c)))
   (make-vector 256 #t)))

(define (set-char-tokenization! ttab char reader term?)
  (vector-set! (ttab-dispatch-vector ttab) (char->ascii char) reader)
  (vector-set! (ttab-terminating?-vector ttab) (char->ascii char) term?))

; Main dispatch

(define (tokenize ttab port)
  (let ((c (read-char port)))
    (if (eof-object? c)
        c
        ((vector-ref (ttab-dispatch-vector ttab) (char->ascii c))
         c port))))

; Atoms (symbols and numbers)

(define (scan-atom c ttab port)
  (let ((translate (ttab-translation ttab)))
    (let loop ((l (list (translate c))) (n 1))
      (let ((c (peek-char port)))
	(cond ((or (eof-object? c)
		   (vector-ref (ttab-terminating?-vector ttab)
			       (char->ascii c)))
	       (reverse-list->string l n))
	      (else
	       (loop (cons (translate (read-char port)) l)
		     (+ n 1))))))))

; Allow ->foo, -v-, etc.

(define (parse-atom string)
  (let ((c (string-ref string 0)))
    (cond ((char=? c #\+)
	   (parse-possible-number string))
          ((char=? c #\-)
	   (parse-possible-number string))
          ((char=? c #\.)
	   (parse-possible-number string))
          (else
           (if (char-numeric? c)
               (parse-number string)
               (string->symbol string))))))

; First char is + - .

(define (parse-possible-number string)
  (if (and (> (string-length string) 1)
	   (char-numeric? (string-ref string 1)))
      (parse-number string)
      (string->symbol string)))

(define (parse-number string)
  (or (string->number string 'e 'd)
      (error "unsupported number syntax" string)))


; Usual stuff (what you'd expect to be common to Scheme and ML syntax)

(define (set-up-usual-tokenization! ttab)

  (define (tokenize-whitespace c port)     c ;ignored
    (tokenize ttab port))

  (define (tokenize-constituent c port)
    (parse-atom (scan-atom c ttab port)))

  (for-each (lambda (c)
	      (set-char-tokenization! ttab (ascii->char c)
				      tokenize-whitespace #t))
	    ascii-whitespaces)

  (for-each (lambda (c)
	      (set-char-tokenization! ttab c tokenize-constituent #f))
	    (string->list
	     (string-append ".0123456789"
			    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			    "abcdefghijklmnopqrstuvwxyz")))
  
  (set-char-tokenization! ttab #\" tokenize-string #t)

  )

(define (make-constituent! c ttab)
  (set-char-tokenization! ttab c
			  (lambda (c port)
			    (parse-atom (scan-atom c ttab port)))
			  #f))

(define (tokenize-string c port)      c ;ignored
  (let loop ((l '()) (i 0))
    (let ((c (read-char port)))
      (cond ((eof-object? c)
	     (error "end of file within a string"))
	    ((char=? c #\\)
	     (let ((c (read-char port)))
	       (if (or (char=? c #\\) (char=? c #\"))
		   (loop (cons c l) (+ i 1))
		   (error "invalid escaped character in string" c))))
	    ((char=? c #\") (reverse-list->string l i))
	    (else (loop (cons c l) (+ i 1)))))))

; Auxiliary for parse-atom and tokenize-string

;(define (reverse-list->string l n)   ;In microcode?
;  (let ((s (make-string n)))
;    (do ((l l (cdr l))
;        (i (- n 1) (- i 1)))
;       ((< i 0) s)
;      (string-set! s i (car l)))))
