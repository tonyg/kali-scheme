; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; This is file write.scm.

;;;; WRITE

; To use this with some Scheme other than Scheme 48, do the following:
;  1. Copy the definition of output-port-option from port.scm
;  2. Define write-string as appropriate (as a write-char loop)
;  3. (define (disclose x) #f)

(define (write obj . port-option)
  (let ((port (output-port-option port-option)))
    (if (and (output-port? port)
	     (open-output-port? port))
	(let recur ((obj obj))
	  (recurring-write obj port recur))
	(call-error "invalid port argument" write port))))

(define (recurring-write obj port recur)
  (cond ((null? obj) (write-string "()" port))
        ((pair? obj) (write-list obj port recur))
        ((eq? obj #t) (write-boolean 't port))
        ((eq? obj #f) (write-boolean 'f port))
        ((symbol? obj) (write-string (symbol->string obj) port))
        ((number? obj) (write-number obj port))
        ((string? obj) (write-string-literal obj port))
        ((char? obj) (write-char-literal obj port))
	(else (write-other obj port recur))))

(define (write-boolean mumble port)
  (write-char #\# port)
  (write mumble port))

(define (write-number n port)
  (write-string (number->string n 10) port))

(define (write-char-literal obj port)
  (let ((probe (character-name obj)))
    (write-string "#\\" port)
    (if probe
	(write probe port)
	(write-char obj port))))

; These are from Matthew Flatt's Unicode proposal for R6RS
; See read.scm.

; Don't use non-R5RS char literals to avoid bootstrap circularities

(define *nul* (scalar-value->char 0))
(define *alarm* (scalar-value->char 7))
(define *backspace* (scalar-value->char 8))
(define *tab* (scalar-value->char 9))
(define *linefeed* (scalar-value->char 10))
(define *vtab* (scalar-value->char 11))
(define *page* (scalar-value->char 12))
(define *return* (scalar-value->char 13))
(define *escape* (scalar-value->char 27))
(define *rubout* (scalar-value->char 127))

(define *char-name-table*
  (list
   (cons #\space 'space)
   (cons #\newline 'newline)
   (cons *nul* 'nul)
   (cons *alarm* 'alarm)
   (cons *backspace* 'backspace)
   (cons *tab* 'tab)
   (cons *linefeed* 'linefeed)
   (cons *vtab* 'vtab)
   (cons *page* 'page)
   (cons *return* 'return)
   (cons *escape* 'escape)
   (cons *rubout* 'rubout)))

(define (character-name char)
  (cond ((assq char *char-name-table*)
	 => cdr)
	(else #f)))

(define (write-string-literal obj port)
  (write-char #\" port)
  (let ((len (string-length obj)))
    (do ((i 0 (+ i 1)))
	((= i len) (write-char #\" port))
      (let ((c (string-ref obj i)))
	(cond
	 ((or (char=? c #\\) (char=? c #\")
	      (char=? c #\') (char=? c #\newline)) ; proposed for R6RS
	  (write-char #\\ port)
	  (write-char c port))
	 ;; the following were all proposed for R6RS
	 ((char=? c *alarm*) (write-string "\\a" port))
	 ((char=? c *backspace*) (write-string "\\b" port))
	 ((char=? c *tab*) (write-string "\\t" port))
	 ((char=? c *linefeed*) (write-string "\\n" port))
	 ((char=? c *vtab*) (write-string "\\v" port))
	 ((char=? c *page*) (write-string "\\f" port))
	 ((char=? c *return*) (write-string "\\r" port))
	 ((char=? c *escape*)  (write-string "\\e" port))
	 (else
	  (write-char c port)))))))

(define (write-list obj port recur)
  (cond ((quotation? obj)
         (write-char #\' port)
         (recur (cadr obj)))
        (else
         (write-char #\( port)
         (recur (car obj))
         (let loop ((l (cdr obj))
                    (n 1))
              (cond ((not (pair? l))
                     (cond ((not (null? l))
                            (write-string " . " port)
                            (recur l))))
                    (else
                      (write-char #\space port)
                      (recur (car l))
                      (loop (cdr l) (+ n 1)))))
         (write-char #\) port))))

(define (quotation? obj)
  (and (pair? obj)
       (eq? (car obj) 'quote)
       (pair? (cdr obj))
       (null? (cddr obj))))

(define (write-vector obj port recur)
   (write-string "#(" port)
   (let ((z (vector-length obj)))
     (cond ((> z 0)
            (recur (vector-ref obj 0))
            (let loop ((i 1))
              (cond ((>= i z))
                    (else
                     (write-char #\space port)
                     (recur (vector-ref obj i))
                     (loop (+ i 1))))))))
   (write-char #\) port))

; The vector case goes last just so that this version of WRITE can be
; used in Scheme implementations in which records, ports, or
; procedures are represented as vectors.  (Scheme 48 doesn't have this
; property.)

(define (write-other obj port recur)
  (cond ((disclose obj)
	 => (lambda (l)
	      (write-string "#{" port)
	      (display-type-name (car l) port)
	      (for-each (lambda (x)
			  (write-char #\space port)
			  (recur x))
			(cdr l))
	      (write-string "}" port)))
	((eof-object? obj) (write-string "#{End-of-file}" port))
	((vector? obj) (write-vector obj port recur))
	((procedure? obj) (write-string "#{Procedure}" port))
	((code-vector? obj)
	 (write-string "#{Byte-vector}" port))
	((channel? obj)
	 (write-string "#{Channel " port)
	 (display (channel-id obj) port)
	 (write-string "}" port))
	((eq? obj (if #f #f)) (write-string "#{Unspecific}" port))
	(else
	 (write-string "#{Random object}" port))))

; Display the symbol WHO-CARES as Who-cares.

(define (display-type-name name port)
  (if (symbol? name)
      (let* ((s (symbol->string name))
	     (len (string-length s)))
	(if (and (> len 0)
		 (char-alphabetic? (string-ref s 0)))
	    (begin (write-char (char-upcase (string-ref s 0)) port)
		   (do ((i 1 (+ i 1)))
		       ((>= i len))
		     (write-char (char-downcase (string-ref s i)) port)))
	    (display name port)))
      (display name port)))

;(define (write-string s port)
;  (do ((i 0 (+ i 1)))
;      ((= i (string-length s)))
;    (write-char (string-ref s i) port)))



; DISPLAY

(define (display obj . port-option)
  (let ((port (output-port-option port-option)))
    (if (and (output-port? port)
	     (open-output-port? port))
	(let recur ((obj obj))
	  (cond ((string? obj) (write-string obj port))
		((char? obj) (write-char obj port))
		(else
		 (recurring-write obj port recur))))
	(call-error "invalid port argument" display port))))

