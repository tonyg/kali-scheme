; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1994 Richard Kelsey.  See file COPYING.


; Scheme48 reader modified to annotate pairs with source line and column
; data.

(define-record-type annotation
  (
   file
   row
   column
   )
  (form
   ))

(define $current-annotating-port (make-fluid #f))

(define-syntax define-lap
  (lambda (exp rename compare)
    (let ((spec (cadr exp))
	  (stuff (cddr exp)))
      `(define ,(car spec) (lap ,spec . ,stuff)))))

(define-lap (annotated-cons car cdr annotation)
  (check-nargs= 3)
  (pop)
  (make-stored-object 3 pair)
  (return))

(define (file-annotated-cons car cdr row column)
  (let* ((note (annotation-maker (fluid $current-annotating-port)
				 row
				 column))
	 (pair (annotated-cons car cdr note)))
    (set-annotation-form! note pair)
    (make-immutable! note)
    (make-immutable! pair)
    pair))

(define-lap (pair-annotation pair)
  (check-nargs= 1)
  (make-env 1)
  (local pair)
  (stored-object-length pair)
  (push)
  (literal 3)
  (=)
  (jump-if-false lose)
  (local pair)
  (stored-object-ref pair 2)  ; this is the reason for the LAP
  (return)
 lose
  (false)
  (return))

; A little Scheme reader.

; Nonstandard things needed:
;  char->ascii, ascii->char (for dispatch table)
;  make-fluid, fluid (used only to implement ##)
;  reverse-list->string (ok to define as list->string of reverse)
;  really-string->symbol (ok to define this to be string->symbol)

(define preferred-case
  (if (char=? (string-ref (symbol->string 't) 0) #\T)
      char-upcase
      char-downcase))

(define close-paren (list 'close-paren))
(define dot         (string->symbol "."))

(define (read-and-annotate port id)
  (let-fluid $current-annotating-port id
    (lambda ()
      (let loop ()
	(let ((form (sub-read port)))
	  (cond ((eq? form dot)
		 (error "\" . \" in illegal context"))
		((eq? form close-paren)
		 ;; Too many right parens.
		 (loop))
		(else form)))))))

(define (sub-read-carefully port)
  (let ((form (sub-read port)))
    (cond ((eof-object? form)
           (error "unexpected end of file"))
          ((eq? form close-paren)
           (error "unexpected right parenthesis"))
          ((eq? form dot)
           (error "unexpected \" . \""))
          (else form))))

; Main dispatch

(define (sub-read port)
  (let ((c (read-char port)))
    (if (eof-object? c)
        c
        ((vector-ref read-dispatch-vector (char->ascii c))
         c port))))

(define read-dispatch-vector
  (make-vector 256
               (lambda (c port)
                 (error "illegal character read" c))))

(define read-terminating?-vector
  (make-vector 256 #t))

(define (set-standard-syntax! char terminating? reader)
  (vector-set! read-dispatch-vector     (char->ascii char) reader)
  (vector-set! read-terminating?-vector (char->ascii char) terminating?))

(let ((sub-read-whitespace
       (lambda (c port)
         c                              ;ignored
         (sub-read port))))
  (do ((i 0 (+ i 1)))
      ((>= i (vector-length read-dispatch-vector)))
    (if (char-whitespace? (ascii->char i))
	(vector-set! read-dispatch-vector i sub-read-whitespace))))

(let ((sub-read-constituent
       (lambda (c port)
	 (parse-token (sub-read-token c port)))))
  (for-each (lambda (c)
              (set-standard-syntax! c #f sub-read-constituent))
            (string->list
             (string-append "!$%&*+-./0123456789:<=>?@^_~ABCDEFGHIJKLM"
                            "NOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"))))

; Usual read macros

(define (set-standard-read-macro! c terminating? proc)
  (set-standard-syntax! c terminating? proc))

(define (sub-read-list c port)
  (let* ((row (current-row port))
	 (column (- (current-column port) 1))  ; to get the location of "("
	 (form (sub-read port)))
    (cond ((eof-object? form)
           (error "end of file inside list -- unbalanced parentheses"))
          ((eq? form close-paren) '())
          ((eq? form dot)
           (let* ((last-form (sub-read-carefully port))
                  (another-form (sub-read port)))
             (cond ((eq? another-form close-paren)
		    last-form)
                   (else
                    (error "randomness after form after dot" another-form)))))
          (else
	   (file-annotated-cons form (sub-read-list c port) row column)))))

(set-standard-read-macro! #\( #t sub-read-list)

(set-standard-read-macro! #\) #t
  (lambda (c port)
    c port
    close-paren))

(set-standard-read-macro! #\' #t
  (lambda (c port)
    c
    (list 'quote (sub-read-carefully port))))

(set-standard-read-macro! #\` #t
  (lambda (c port)
    c
    (list 'quasiquote (sub-read-carefully port))))

(set-standard-read-macro! #\, #t
  (lambda (c port)
    c
    ;; Do not beta-reduce this!
    (let* ((next (peek-char port))
	   (marker (cond ((eof-object? next)
			  (error "end of file after ,"))
			 ((char=? next #\@)
			  (read-char port)
			  'unquote-splicing)
			 (else 'unquote))))
      (list marker
            (sub-read-carefully port)))))

(set-standard-read-macro! #\" #t
  (lambda (c port)
    c ;ignored
    (let loop ((l '()) (i 0))
      (let ((c (read-char port)))
        (cond ((eof-object? c)
               (error "end of file within a string"))
              ((char=? c #\\)
               (let ((c (read-char port)))
		 (cond ((eof-object? c)
			(error "end of file within a string"))
		       ((or (char=? c #\\) (char=? c #\"))
			(loop (cons c l) (+ i 1)))
		       (else
			(error "invalid escaped character in string" c)))))
              ((char=? c #\")
	       (reverse-list->string l i))
              (else
	       (loop (cons c l) (+ i 1))))))))

(set-standard-read-macro! #\; #t
  (lambda (c port)
    c ;ignored
    (gobble-line port)
    (sub-read port)))

(define (gobble-line port)
  (let loop ()
    (let ((c (read-char port)))
      (cond ((eof-object? c) c)
	    ((char=? c #\newline) #f)
	    (else (loop))))))

(set-standard-read-macro! #\# #f
  (lambda (c port)
    c ;ignored
    (let* ((c (peek-char port))
	   (c (if (eof-object? c)
		  (error "end of file after #")
		  (char-downcase c))))
      (cond ((char=? c #\f) (read-char port) #f)
            ((char=? c #\t) (read-char port) #t)
            ((char=? c #\\)
	     (read-char port)
             (let ((c (peek-char port)))
               (cond ((eof-object? c)
		      (error "end of file after #\\"))
		     ((char-alphabetic? c)
		      (let ((name (sub-read-carefully port)))
			(cond ((= (string-length (symbol->string name)) 1)
			       c)
			      ((assq name '((space   #\space)
					    (newline #\newline)))
			       => cadr)
			      (else
			       (error "unknown #\\ name" name)))))
		     (else
		      (read-char port)))))
            ((char=? c #\()
	     (read-char port)
             (list->vector (sub-read-list c port)))
            ;; ## should evaluate to the last REP-loop result.
            ((memq c '(#\b #\o #\d #\x #\i #\e))
	     (let ((string (sub-read-token #\# port)))
	       (or (string->number string)
		   (error "unsupported number syntax" string))))
;            ((char=? c #\#)   ; don't worry about this
;	     (read-char port)
;	     (make-last-value-expression))
            (else
             (error "unknown # syntax" c))))))


; Tokens

(define (sub-read-token c port)
  (let loop ((l (list (preferred-case c))) (n 1))
    (let ((c (peek-char port)))
      (cond ((or (eof-object? c)
                 (vector-ref read-terminating?-vector (char->ascii c)))
             (reverse-list->string l n))
            (else
             (loop (cons (preferred-case (read-char port)) l)
                   (+ n 1)))))))

(define (parse-token string)
  (if (let ((c (string-ref string 0)))
	(or (char-numeric? c) (char=? c #\+) (char=? c #\-) (char=? c #\.)))
      (cond ((string->number string))
	    ((member string strange-symbol-names)
	     (string->symbol string))
	    ((string=? string ".")
	     dot)
	    (else
	     (error "unsupported number syntax" string)))
      (string->symbol string)))

(define strange-symbol-names
  '("+" "-" "..." "1+" "-1+"))  ;The latter two only for S&ICP support
