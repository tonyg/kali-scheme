; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.

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

(define (character-name char)
  (cond ((char=? char #\space) 'space)
        ((char=? char #\newline) 'newline)
	(else #f)))

(define (write-string-literal obj port)
  (write-char #\" port)
  (let ((len (string-length obj)))
    (do ((i 0 (+ i 1)))
	((= i len) (write-char #\" port))
      (let ((c (string-ref obj i)))
	(if (or (char=? c #\\) (char=? c #\"))
	    (write-char #\\ port))
	(write-char c port)))))

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

