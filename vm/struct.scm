; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-

; This is file struct.scm.

; This file defines a level of abstraction for storage somewhat higher
; than that of d-vectors and b-vectors: pairs, symbols, and other datatypes.

; An ugly and unsafe macro for defining VM data structures.
;
; (DEFINE-PRIMITIVE-DATA-TYPE <name> <type> <immutable?> <constructor-name>
;                             <slot>*)
; <slot> ::= (<accessor-name>) | (<accessor-name> <modifier-name>)
;
; (define-primitive-data-type pair N #f cons (car set-car!) (cdr))
; =>
; (begin
;  (define (cons a b) (d-vector ...))
;  (define pair? (stob-predicate ...))
;  (define pair-size 3)
;  (define (car x) (d-vector-ref x 0))
;  (define (set-car! x val) (d-vector-set! x 0 val))
;  (define (cdr x) (d-vector-ref x 1))

(define-syntax define-primitive-data-type
  (lambda (exp rename compare)
    (destructure (((d-p-d-t name type immutable? make . body) exp))
      (define (concatenate-symbol . syms)
	(string->symbol (apply string-append (map symbol->string syms))))
      (let* ((pred (concatenate-symbol name '?))
	     (size (concatenate-symbol name '- 'size))
	     (shorten (lambda (l1 l2) (map (lambda (x1 x2) x2 x1) l1 l2)))
	     (vars (shorten `(a b c d e f g) body)))
	`(begin (define ,make
		  (let ((type (enum stob ,type)))
		    (lambda (,@vars key)
		      ,(if immutable?
			   `(immutable-d-vector type key ,@vars)
			   `(d-vector type key ,@vars)))))
		(define ,pred (stob-predicate (enum stob ,type)))
		(define ,size (+ ,(length body) 1))
		,@(do ((s body (cdr s))
		       (i 0 (+ i 1))
		       (d '() (let* ((slot (car s))
				     (d (cons `(define (,(car slot) x)
						 (d-vector-ref x ,i))
					      d)))
				(if (null? (cdr slot))
				    d
				    (cons `(define (,(cadr slot) x val)
					     (d-vector-set! x ,i val))
					  d)))))
		      ((null? s) (reverse d))))))))

; This is a front for DEFINE-PRIMITIVE-DATA-TYPE that gets the names from
; STOB-DATA (which is defined in arch.scm).  This ensures that the run-time
; code, the VM, and the linker agree on what these structures look like.
;
; SCHEME? is #T if the data structure is a Scheme structure, in which case
; the names defined by the form all have VM- prepended.

(define-syntax define-shared-primitive-data-type
  (lambda (exp rename compare)
    (let ((name (cadr exp))
	  (scheme? (if (null? (cddr exp)) #f (car (cddr exp))))
	  (immutable? (if (or (null? (cddr exp))
			      (null? (cdddr exp)))
			  #f
			  (cadr (cddr exp)))))
      (define (concatenate-symbol . syms)
	(string->symbol (apply string-append (map symbol->string syms))))
      (let ((data (cddr (assq name stob-data)))
	    (fixup (lambda (n)
		     (if scheme? (concatenate-symbol 'vm- n) n))))
	`(define-primitive-data-type
	   ,(fixup name)
	   ,name
	   ,immutable?
	   ,(fixup (car data))
	   . ,(map (lambda (p)
		     (cons (fixup (car p))
			   (if (not (cadr p))
			       '()
			       (list (fixup (cadr p))))))
		   (cdr data)))))))

; A d-vector macro version of the VECTOR procedure.
; This is only used in the expansion of DEFINE-PRIMITIVE-DATA-TYPE.

(define-syntax d-vector
  (lambda (exp rename compare)
    (destructure (((d-v type key . args) exp))
      `(let ((v (make-d-vector ,type ,(length args) key)))
	 ,@(do ((a args (cdr a))
		(i 0 (+ i 1))
		(z '() (cons `(d-vector-set! v ,i ,(car a)) z)))
	       ((null? a) (reverse z)))
	 v))))

(define-syntax immutable-d-vector
  (syntax-rules ()
   ((immutable-d-vector stuff ...)
    (let ((vec (d-vector stuff ...)))
      (make-immutable! vec)
      vec))))

; A simpler macro for defining types of vectors.  Again SCHEME? being #T
; causes VM- to be prepended to the defined names.

(define-syntax define-vector-data-type
  (lambda (exp rename compare)
    (let ((name (cadr exp))
	  (scheme? (cddr exp)))
      (define (concatenate-symbol . syms)
	(string->symbol (apply string-append (map symbol->string syms))))
      (let* ((type `(enum stob ,name))
	     (fix (if (not (null? scheme?))
		      'vm-
		      (string->symbol "")))
	     (pred (concatenate-symbol fix name '?))
	     (make (concatenate-symbol fix 'make- name))
	     (size (concatenate-symbol fix name '- 'size))
	     (length (concatenate-symbol fix name '- 'length))
	     (ref (concatenate-symbol fix name '- 'ref))
	     (set (concatenate-symbol fix name '- 'set!)))
	`(begin (define ,make (stob-maker ,type make-d-vector))
		(define ,pred (stob-predicate ,type))
		(define (,size len) (+ len 1))
		(define ,length d-vector-length)
		(define ,ref d-vector-ref)
		(define ,set d-vector-set!))))))

(define (stob-maker type maker)
  (lambda (length key)
    (maker type length key)))

(define (stob-predicate type)
  (lambda (obj) (stob-of-type? obj type)))

; data for these comes from STOB-DATA in arch.scm

(define-shared-primitive-data-type pair #t)
(define-shared-primitive-data-type symbol #t #t)
(define-shared-primitive-data-type closure #f #t)
(define-shared-primitive-data-type location)
(define-shared-primitive-data-type weak-pointer)
(define-shared-primitive-data-type external)

; The one currently unshared data structure.

(define-primitive-data-type port port #f make-port
  (port-mode set-port-mode!)
  (port-index set-port-index!)
  (peeked-char set-peeked-char!)
  (port-id set-port-id!))  ; setter needed by the post-GC code

; Vectors and so on

(define-vector-data-type vector #t)
(define-vector-data-type record)
(define-vector-data-type extended-number)
(define-vector-data-type continuation)
(define-vector-data-type template)

(define (vm-vector-fill! v val)
  (do ((i 0 (+ i 1)))
      ((= i (vm-vector-length v)) v)
    (vm-vector-set! v i val)))

(define (continuation-cont     c) (continuation-ref c 0))
(define (continuation-pc       c) (continuation-ref c 1))
(define (continuation-template c) (continuation-ref c 2))
(define (continuation-env      c) (continuation-ref c 3))

(define (set-continuation-cont!     c val) (continuation-set! c 0 val))
(define (set-continuation-pc!       c val) (continuation-set! c 1 val))
(define (set-continuation-template! c val) (continuation-set! c 2 val))
(define (set-continuation-env!      c val) (continuation-set! c 3 val))

(define continuation-cells 4)

(define (template-code tem) (template-ref tem 0))
(define (template-name tem) (template-ref tem 1))

; The VM needs a few templates for various obscure purposes.

(define (make-template-containing-ops op1 op2)
  (let ((temp (make-template 2 universal-key))
        (code (make-code-vector 2 universal-key)))
    (template-set! temp 0 code)
    (code-vector-set! code 0 op1)
    (code-vector-set! code 1 op2)
    temp))

(define (op-template-size op-count)
  (+ (template-size 2) (code-vector-size op-count)))

; Code vectors

(define make-code-vector   (stob-maker (enum stob code-vector) make-b-vector))
(define code-vector?       (stob-predicate (enum stob code-vector)))
(define code-vector-length b-vector-length)
(define code-vector-ref    b-vector-ref)
(define code-vector-set!   b-vector-set!)

(define (code-vector-size len)
  (+ 1 (bytes->cells len)))

; Strings are presented as being one character shorter than they really
; are to hide the null character at the end.

(define vm-make-string   (stob-maker (enum stob string)
				     (lambda (type len key)
				       (make-b-vector type (+ len 1) key))))
(define vm-string?       (stob-predicate (enum stob string)))
(define vm-string-length (lambda (x) (- (b-vector-length x) 1)))
(define vm-string-ref    (lambda (s i) (ascii->char (b-vector-ref s i))))
(define vm-string-set!   (lambda (s i c) (b-vector-set! s i (char->ascii c))))

(define (vm-string-size length)
  (+ 1 (bytes->cells (+ 1 length))))

(define (enter-string string)           ; used by VMIO on startup
  (let ((z (string-length string)))
    (let ((v (vm-make-string z universal-key)))
      (do ((i 0 (+ i 1)))
          ((>= i z) v)
        (vm-string-set! v i (string-ref string i))))))

(define (extract-string string)         ; used by OPEN, WRITE-STRING, SUSPEND
  (let ((z (vm-string-length string)))
    (let ((v (make-string z)))
      (do ((i 0 (+ i 1)))
          ((>= i z) v)
        (string-set! v i (vm-string-ref string i))))))

(define (write-vm-string string port)
  (write-bytes (address-after-header string)
	       (vm-string-length string)
	       port))

(define vm-string=? stob-equal?)

; Hashing

; The hash function used here is to take the sum of the ascii values
; of the characters in the string, modulo the symbol table size.
;
; This hash function was also compared against some others, e.g.
; adding in the length as well, and taking only the odd or only the
; even characters.  It fared about the same as adding the length, and
; much better than examining only every other character.
;
; Perhaps a hash function that is sensitive to the positions of the
; characters should be tried?  (Consider CADDR, CDADR, CDDAR.)
;
; Of course, if we switched to rehashing, a prime modulus would be
; important.

(define (vm-string-hash s)
  (let ((n (vm-string-length s)))
    (do ((i 0 (+ i 1))
         (h 0 (+ h (char->ascii (vm-string-ref s i)))))
        ((>= i n) h))))

; Symbol table and environment lookup

(define (table-searcher hash match? make-new)
  ;; In FX terms, this procedure has type
  ;;          (poly (t1 t2 t3)
  ;;            (proc ((proc (t1) int)     ;hash
  ;;                   (proc (t1 t2) bool) ;match?
  ;;                   (proc (t1) t2))     ;make-new
  ;;              (proc (t1 (vector-of (list-of t2)))
  ;;                t2)))
  ;; For the symbol table, t1 = string, t2 = t3 = symbol.
  (lambda (obj table key)
    (let* ((index (bitwise-and (hash obj) (- (vm-vector-length table) 1)))
           (bucket (vm-vector-ref table index)))
      (let loop ((b bucket))
        (cond ((vm-eq? b null)
               (let ((new (make-new obj key)))
                 (vm-vector-set! table index (vm-cons new bucket key))
                 new))
              ((match? obj (vm-car b)) (vm-car b))
              (else (loop (vm-cdr b))))))))

(define intern
  (table-searcher vm-string-hash
                  (lambda (string sym)
                    (vm-string=? string (vm-symbol->string sym)))
		  (lambda (string key)
		    (let ((sym (vm-make-symbol string key)))
		      (make-immutable! sym)
		      sym))))

(define add-to-symbol-table
  (table-searcher (lambda (sym) (vm-string-hash (vm-symbol->string sym)))
		  vm-eq?
		  (lambda (sym key) sym)))

