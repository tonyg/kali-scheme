; Copyright (c) 1993-2004 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Dump and restore

; Unix has special meanings for
;  ETX, FS, DEL, ETB, NAK, DC2, EOT, EM (or SUB?), DC3, DC1, SI, SYN,
;  3    28  127  23   21   18   4    25     26     19   17   15  22
; so avoid using them.

(define type/null        #\n)
(define type/true	 #\t)
(define type/false	 #\f)
(define type/unspecific #\u)
(define type/pair        #\p)  ;obj1 obj2
(define type/string	 #\s)  ;length chars
(define type/number	 #\i)  ;#chars rep
(define type/symbol	 #\y)  ;length chars
(define type/char	 #\c)  ;char
(define type/vector      #\v)  ;length objects
(define type/template    #\a)  ;length objects
(define type/code-vector #\k)  ;length bytes (each byte is 2 hex digits?)
(define type/location	 #\l)  ;uid
(define type/closure	 #\q)  ;template-info
(define type/ellipsis	 #\e)
(define type/random	 #\r)


; Recursive entry

(define (dump obj write-char depth)
  (cond ((null? obj) (dump-type type/null write-char))
	((eq? obj #t) (dump-type type/true write-char))
	((eq? obj #f) (dump-type type/false write-char))
	((pair? obj) (dump-pair obj write-char depth))
	;; Template case needs to precede vector case
	((template? obj) (dump-template obj write-char depth))
	((vector? obj) (dump-vector obj write-char depth))
	((symbol? obj) (dump-symbol obj write-char))
	((number? obj) (dump-number obj write-char))
	((string? obj) (dump-string obj write-char))
	((char? obj) (dump-char-literal obj write-char))
	((code-vector? obj) (dump-code-vector obj write-char))
	((location? obj) (dump-location obj write-char))
	((unspecific? obj) (dump-type type/unspecific write-char))
	((closure? obj) (dump-closure obj write-char))
	(else (dump-random obj write-char depth))))

(define (restore read-char)
  (let ((type (restore-type read-char)))
    ((vector-ref restorers (char->ascii type)) type read-char)))
    
(define restorers
  (make-vector 256 (lambda (type read-char)
		     ;; Invalid type
		     (error "invalid type code" type))))

(define (define-restorer! type proc)
  (vector-set! restorers (char->ascii type) proc))


; Particular dumpers & restorers

(define-restorer! type/null  (lambda (c read-char) '()))
(define-restorer! type/false (lambda (c read-char) #f))
(define-restorer! type/true  (lambda (c read-char) #t))
(define-restorer! type/unspecific (lambda (c read-char) (if #f #f)))

; Pairs

(define (dump-pair obj write-char depth)
  (if (= depth 0)
      (dump-ellipsis obj write-char)
      (let ((depth (- depth 1)))
	(dump-type type/pair write-char)
	(dump (car obj) write-char depth)
	(dump (cdr obj) write-char depth))))

(define-restorer! type/pair
  (lambda (c write-char)
    c ;ignored
    (let ((the-car (restore write-char)))
      (cons the-car (restore write-char)))))

; Symbols

(define (dump-symbol obj write-char)
  (dump-type type/symbol write-char)
  (dump-a-string (symbol-case-converter (symbol->string obj)) write-char))

(define-restorer! type/symbol
  (lambda (c read-char)
    c ;ignored
    (string->symbol (symbol-case-converter (restore-a-string read-char)))))


; Numbers
;   <space> ... _   represent  0 ... 63,
;  {<space> ... {_  represent  64 ... 127,     -- { is ascii 123 
;  |<space> ... |_  represent  128 ... 191,    -- | is ascii 124
;  }<space> ... }_  represent  192 ... 256.    -- } is ascii 125

(define (dump-number n write-char)
  (if (not (communicable-number? n))
      (error "can't dump this number" n))
  (if (and (integer? n)
	   (>= n 0)
	   (< n 256))
      (dump-byte n write-char)
      (begin (dump-type type/number write-char)
	     ;; Note logarithmic recursion
	     (dump-a-string (number->string n comm-radix) write-char))))

(define (communicable-number? n) #t)  ;this gets redefined in client

(define (dump-byte n write-char)  ;Dump a number between 0 and 255
  (if (< n 64)
      (write-char (ascii->char (+ n ascii-space)))
      (begin (write-char (ascii->char (+ (arithmetic-shift n -6)
					 122)))
	     (write-char (ascii->char (+ (bitwise-and n 63)
					 ascii-space))))))

(define ascii-space (char->ascii #\space)) ;32

(define (restore-small-integer c read-char)
  (- (char->ascii c) ascii-space))

(do ((i (+ ascii-space 63) (- i 1)))
    ((< i ascii-space))
  (define-restorer! (ascii->char i) restore-small-integer))

(define (restore-medium-integer c read-char)
  (+ (arithmetic-shift (- (char->ascii c) 122) 6)
     (- (char->ascii (read-char)) ascii-space)))

(do ((i 123 (+ i 1)))
    ((> i 125))
  (define-restorer! (ascii->char i) restore-medium-integer))

(define (restore-number read-char)
  (let ((c (read-char)))
    (if (char=? c type/number)
	(string->number (restore-a-string read-char) comm-radix)
	(let ((n (char->ascii c)))
	  (if (> n 122)
	      (restore-medium-integer c read-char)
	      (- n ascii-space))))))

(define-restorer! type/number
  (lambda (c read-char)
    c ;ignored
    (string->number (restore-a-string read-char) comm-radix)))

(define comm-radix 16)




; String literals

(define (dump-string obj write-char)
  (dump-type type/string write-char)
  (dump-a-string obj write-char))

(define-restorer! type/string
  (lambda (c read-char)
    c ;ignored
    (restore-a-string read-char)))

; Characters

(define (dump-char-literal obj write-char)
  (dump-type type/char write-char)
  (dump-a-char obj write-char))

(define-restorer! type/char
  (lambda (c read-char)
    c ;ignored
    (restore-a-char read-char)))

; Vectors

(define (dump-vector obj write-char depth)
  (dump-vector-like obj write-char depth
		    type/vector vector-length vector-ref))

(define (dump-template obj write-char depth)
  (dump-vector-like obj write-char depth
		    type/template template-length template-ref))

(define (dump-vector-like obj write-char depth type vector-length vector-ref)
  (if (= depth 0)
      (dump-ellipsis obj write-char)
      (let ((depth (- depth 1))
	    (len (vector-length obj)))
	(dump-type type write-char)
	(dump-length len write-char)
	(do ((i 0 (+ i 1)))
	    ((= i len) 'done)
	  (dump (vector-ref obj i) write-char depth)))))

(define (restore-vector-like make-vector vector-set!)
  (lambda (c read-char)
    c ;ignored
    (let* ((len (restore-length read-char))
	   (v (make-vector len #\?)))
      (do ((i 0 (+ i 1)))
	  ((= i len) v)
	(vector-set! v i (restore read-char))))))

(define-restorer! type/vector
  (restore-vector-like make-vector vector-set!))

(define-restorer! type/template
  (restore-vector-like make-template template-set!))
  
; Code vectors

(define (dump-code-vector obj write-char)
  (dump-type type/code-vector write-char)
  (let ((len (code-vector-length obj)))
    (dump-length len write-char)
    (do ((i 0 (+ i 1)))
	((= i len) 'done)
      (dump-byte (code-vector-ref obj i) write-char))))

(define-restorer! type/code-vector
  (lambda (c read-char)
    c					;ignored
    (let* ((len (restore-length read-char))
	   (cv (make-code-vector len 0)))
      (do ((i 0 (+ i 1)))
	  ((= i len) cv)
	(code-vector-set! cv i
			  (restore-number read-char))))))

; Locations

(define (dump-location obj write-char)
  (dump-type type/location write-char)
  (dump-number (location->uid obj) write-char))

(define (location->uid obj)
  (or ((fluid $dump-index) obj)
      (location-id obj)))

(define-restorer! type/location
  (lambda (c read-char)
    c ;ignored
    (uid->location (restore-number read-char))))

(define (uid->location uid)
  (or ((fluid $restore-index) uid)
      (table-ref uid->location-table uid)
      (let ((loc (make-undefined-location uid)))
	(note-location! loc)
	loc)))
(define $restore-index (make-fluid (lambda (uid) #f)))

(define uid->location-table (make-table))

(define (note-location! den)
  (table-set! uid->location-table
	      (location-id den)
	      den))

(define $dump-index (make-fluid (lambda (loc) #f)))

; For simulation purposes, it's better for location uid's not to
; conflict with any that might be in the base Scheme 48 system.  (In the
; real server system there isn't any base Scheme 48 system, so there's
; no danger of conflict.)

; (define location-uid-origin 5000)


; Closure

(define (dump-closure obj write-char)
  (dump-type type/closure write-char)
  (let ((id (template-info (closure-template obj))))
    (dump-number (if (integer? id) id 0) write-char)))

(define-restorer! type/closure
  (lambda (c read-char)
    c ;ignored
    (make-random (list 'closure (restore-number read-char)))))


; Random

(define random-type (make-record-type 'random '(disclosure)))
(define make-random (record-constructor random-type '(disclosure)))
(define-record-discloser random-type
  (let ((d (record-accessor random-type 'disclosure)))
    (lambda (r) (cons "Remote" (d r)))))

(define (dump-random obj write-char depth)
  (dump-type type/random write-char)
  (dump (or (disclose obj) (list '?))
	    write-char
	    depth))

(define-restorer! type/random
  (lambda (c read-char)
    (make-random (restore read-char))))



; Ellipsis

(define (dump-ellipsis obj write-char)
  (dump-type type/ellipsis write-char))

(define-restorer! type/ellipsis
  (lambda (c read-char) (make-random (list (string->symbol "---")))))




; Auxiliaries:

; Strings (not necessarily preceded by type code)

(define (dump-a-string obj write-char)
  (let ((len (string-length obj)))
    (dump-length len write-char)
    (do ((i 0 (+ i 1)))
	((= i len) 'done)
      (dump-a-char (string-ref obj i) write-char))))

(define (restore-a-string read-char)
  (let* ((len (restore-length read-char))
	 (str (make-string len #\?)))
    (do ((i 0 (+ i 1)))
	((= i len) str)
      (string-set! str i (restore-a-char read-char)))))

(define (dump-a-char c write-char)
  (write-char c))

(define (restore-a-char read-char)
  (read-char))


; Type characters

(define (dump-type c write-char)
  (write-char c))
(define (restore-type read-char)
  (read-char))

(define dump-length dump-number)
(define restore-length restore-number)

;(define char->ascii char->integer)  -- defined in p-features.scm
;(define ascii->char integer->char)  -- ditto


; Miscellaneous support

(define (unspecific? obj)
  (eq? obj *unspecific*))

(define *unspecific* (if #f #f)) ;foo

;(define (integer->digit-char n)
;  (ascii->char (+ n (if (< n 10) ascii-zero a-minus-ten))))
;
;(define (digit-char->integer c)
;  (cond ((char-numeric? c)
;         (- (char->ascii c) ascii-zero))
;        ((char=? c #\#) 0)
;        (else
;         (- (char->ascii (char-downcase c)) a-minus-ten))))
;
;(define ascii-zero (char->ascii #\0))
;
;(define a-minus-ten (- (char->integer #\a) 10))

; These modified from s48/boot/transport.scm

(define (string-case-converter string)
  (let ((new (make-string (string-length string) #\?)))
    (do ((i 0 (+ i 1)))
	((>= i (string-length new)) new)
      (string-set! new i (invert-case (string-ref string i))))))

(define (invert-case c)
  (cond ((char-upper-case? c) (char-downcase c))
	((char-lower-case? c) (char-upcase c))
	(else c)))

(define symbol-case-converter
  (if (char=? (string-ref (symbol->string 't) 0) #\t)
      (lambda (string) string)
      string-case-converter))


; ASCII
;   !"#$%&'()*+,-./0123456789:;<=>?
;  @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_
;  `abcdefghijklmnopqrstuvwxyz{|}~

;(define (tst x)
;  (let ((l '()))
;    (dump x (lambda (c) (set! l (cons c l))) -1)
;    (let ((l (reverse l)))
;      (restore (lambda ()
;                 (let ((c (car l)))
;                   (set! l (cdr l))
;                   c))))))

;(define cwcc call-with-current-continuation)
;
;(define (tst x)
;  (letrec ((write-cont (lambda (ignore)
;                         (dump x
;                               (lambda (c)
;                                 (cwcc (lambda (k)
;                                         (set! write-cont k)
;                                         (read-cont c))))
;                               -1)))
;           (read-cont #f))
;    (restore (lambda ()
;               (cwcc (lambda (k)
;                       (set! read-cont k)
;                       (write-cont 'ignore)))))))
