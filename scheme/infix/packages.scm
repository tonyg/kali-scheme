; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Infix stuff

(define-structure tokenizer (export make-tokenizer-table
				    set-up-usual-tokenization!
				    set-char-tokenization!
				    tokenize)
  (open scheme records signals defpackage ascii)
  (access primitives)
  (files tokenize))

(define-structure pratt (export toplevel-parse
				parse
				make-operator
				make-lexer-table set-char-tokenization!
				lexer-ttab define-keyword define-punctuation
				prsmatch comma-operator delim-error erb-error
				if-operator
				then-operator else-operator parse-prefix
				parse-nary parse-infix
				parse-matchfix end-of-input-operator
				port->stream)
  (open scheme records signals tokenizer tables)
  (files pratt))

(define-structure sgol (export sgol-read sgol-repl)
  (open scheme signals pratt)
  (files sgol))
