; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 2005 by Andreas Rottmann. See file COPYING.

(define-record-type keyword :keyword
  (make-keyword symbol)
  keyword?
  (symbol keyword->symbol))

(define *keywords* (make-table))

(define (symbol->keyword symbol)
  (or (table-ref *keywords* symbol)
      (let ((keyword (make-keyword symbol)))
        (table-set! *keywords* symbol keyword)
        keyword)))
