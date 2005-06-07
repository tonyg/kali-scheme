;; jas-match.scm  --  Jens Axel Søgaard  -- 13/14 dec 2003

;;; NOTE

;  Uncomment match-define-values and match-define-values-helper,
;  if your Scheme doesn't support define-values.
;  A definition of define-values is on the way ...

;;; HISTORY

; 13/14-dec-2003
;     - first version
;     - added cons* as synonym for list-rest (suggested by Riastradh)
; 17-dec-2003
;     - added match-defined-values
;     - improved match-compound
;       ( the cons and vector patterns no longer duplicate failure )

;;; PURPOSE

; This is a very naïve implementation of a subset of
; the pattern matcher plt-match.ss, which builds upon
; Wright's pattern matcher, but uses constructor notation
; in the patterns. 

; The idea was too see, how far I could get using nothing
; but syntax-rules. 

;;; INSTRUCTIONS OF USE

; The user macros are
;    (match expr (pattern <guard> expr) ...)    , <guard> can be omitted
;    (match-lambda (pattern expr ...) ...)
;    (match-let ((pattern expr) ...) expr ...)
;    (match-let* ((pattern expr) ...) expr ...)

; The syntax of patterns are a subset of the one in:
;     <http://download.plt-scheme.org/scheme/plt-clean-cvs/collects/mzlib/plt-match.ss>

; The semantics of the match functions are explained in
;     <http://download.plt-scheme.org/scheme/docs/html/mzlib/mzlib-Z-H-22.html#node_chap_22>

; Notably features missing:
;   - quasi-patterns
;   - set! and get!
;   - match-define 
;   - match-letrec
;   - the ooo and ook extension in list and vector patterns
;   - structures (easily added but they are non portable)

;;; IMPLEMENTATION

; The implementation is divided into layers, each layer
; handles one aspect of the pattern matching process.

; The main macro from the user perspective is the match macro.
;   (match expr ((pattern expr ...) ...])
; which binds the value to be matched to a variable, and leaves
; the real work to guarded match. Match also handles the case
; of multple patterns.

; The macro guarded-match 
;    (guarded-match var pattern success failure)
; expands to success if the value bound to var matches the pattern,
; otherwise it expands to failure.
; Guarded-match takes care of guards and then macro calls logical-match.

; The macro logical-match 
;    (logical-match var pattern success failure)
; expands to success if the value bound to var matches the pattern,
; otherwise it expands to failure.
; Logical-match takes care of patterns of the form
;   (and pattern ...)
;   (or  pattern ...)
;   (not pattern pattern ...)
;   (?   expr pattern ...)
; and then macro calls compound-match.

; The macro compound-match 
;    (compound-match var pattern success failure)
; expands to success if the value bound to var matches the pattern,
; otherwise it expands to failure.
; Compound-match takes care of patterns of the form
;   (cons pattern pattern)
;   (list pattern ...)
;   (list-rest pattern ... pattern)
;   (vector pattern pattern ...)
;   (app expr pattern)
; and then macro calls simple-match.

; The macro simple-match 
;    (simple-match var pattern success failure)
; expands to success if the value bound to var matches the pattern,
; otherwise it expands to failure.
; Simple-match takes care of patterns of the form
;   (quote symbol)
;   (quote datum)
;   pattern-var
;   literal
; and possible macro calls literal-match.

; The macro literal-match 
;    (literal-match var pattern success failure)
; expands to success if the value bound to var matches the pattern,
; otherwise it expands to failure.
; Literal-match takes care of patterns of atoms of the form
;   the empty list
;   booleans
;   strings
;   numbers
;   characters
; and compound literals.

(define-syntax symbol??
  ;; From Oleg's "How to write symbol? with syntax-rules.
  ;; <http://okmij.org/ftp/Scheme/macro-symbol-p.txt>
  (syntax-rules ()
    ((symbol?? (x . y) kt kf) kf)	; It's a pair, not a symbol
    ((symbol?? #(x ...) kt kf) kf)	; It's a vector, not a symbol
    ((symbol?? maybe-symbol kt kf)
     (let-syntax
         ((test
	   (syntax-rules ()
	     ((test maybe-symbol t f) t)
	     ((test x t f) f))))
       (test abracadabra kt kf)))))

(define (literal? datum)
  (or (string? datum)
      (number? datum)
      (char? datum)
      (null? datum)
      (boolean? datum)))


(define-syntax literal-match
  (syntax-rules ()
    ((_ var () success failure)        (if (null? var)  success failure))
    ((_ var #t success failure)        (if (eq? var #t) success failure))
    ((_ var #f success failure)        (if (eq? var #f) success failure))
    ((_ var literal success failure)   (if (and (literal? var)
                                               (equal? var literal))
                                          success
                                          failure))))

(define-syntax simple-match
  ; (simple-match var pattern success failure)
  ;     If the value bound to var matches pattern then the 
  ;     expression expands into a let binding the pattern variables
  ;     in the pattern to the matched (sub)values, success becomes the
  ;     body of the let. Otherwise the macro call expands to failure.
  (syntax-rules (quote)
    ((_ var (quote symbol/datum)     success failure)  (if ((symbol?? symbol/datum eq? equal?) var 'symbol/datum)
                                                          success 
                                                          failure))
    ((_ var name/literal             success failure)  (symbol?? name/literal
                                                                ; pattern variable
                                                                (let ((name/literal var))
                                                                  success)
                                                                ; literal
                                                                (literal-match var name/literal success failure)))))

(define-syntax compound-match 
  (syntax-rules (cons list list-rest app vector)
    ((_ var (cons p1 p2)            success failure)   (let ((failure-thunk (lambda () failure)))
                                                         ; Note: Converting failure to a failure thunk
                                                         ;       considerably reduces the size if the 
                                                         ;       exapnded code
                                                         ;      (at the cost of generating closures at runtime)
                                                         (if (pair? var)
                                                             (match (car var)
                                                               (p1 (match (cdr var) 
                                                                     (p2 success) 
                                                                     (_ (failure-thunk))))
                                                               (_  (failure-thunk)))
                                                             (failure-thunk))))
    
    ((_ var (list)                   success failure)  (compound-match var () success failure))
    ((_ var (list p1)                success failure)  (compound-match var (cons p1 ()) success failure))
    ((_ var (list p1 p2 ...)         success failure)  (compound-match var (cons p1 (list p2 ...)) success failure))
    
    ((_ var (vector p1 ...)          success failure)  (let ((vector-var (if (vector? var)
                                                                             (vector->list var)
                                                                             'failed-vector-match)))
                                                         (compound-match vector-var (list p1 ...) success failure)))
    
    ((_ var (list-rest p1 p2)        success failure)  (compound-match var (cons p1 p2) success failure))
    ((_ var (list-rest p1 p2 p3 ...) success failure)  (compound-match var (cons p1 (list-rest p2 p3 ...)) 
                                                                       success failure))
    
    ((_ var (app expr p1)            success failure)  (let ((new-var (expr var)))
                                                        (match new-var p1 success failure)))
    ((_ var pattern          success failure)  (simple-match var pattern success failure))))


(define-syntax logical-match 
  (syntax-rules (and or not ?)
    ((_ var (and)            success failure)  success)
    ((_ var (and p1)         success failure)  (compound-match var p1 success failure))
    ((_ var (and p1 p2 ...)  success failure)  (compound-match var p1 
                                                              (logical-match var (and p2 ...) success failure)
                                                              failure))

    ((_ var (or p1)          success failure)  (compound-match var p1 success failure))
    ((_ var (or p1 p2 ...)   success failure)  (compound-match var p1 success 
                                                              (logical-match var (or p2 ...) success failure)))
    
    ((_ var (not p)          success failure)  (logical-match var p failure success))
    ((_ var (not p1 p2 ...)  success failure)  (logical-match var (and (not p1) (not p2) ...) failure success))
    
    ((_ var (? expr p ...)   success failure)  (if expr
                                                  (logical-match var (and p ...) success failure)
                                                  failure))

    ((_ var pattern          success failure)  (compound-match var pattern success failure))))


(define-syntax guarded-match
  (syntax-rules ()
    ((_ var pattern success failure)          (logical-match var pattern success failure))
    ((_ var pattern guard success failure)    (guarded-match var pattern (if guard success failure) failure))))


(define-syntax match
  (syntax-rules ()
    ((_ expr)                                  (let ((v expr))
                                                 'no-match))
    ((_ expr (pattern template)
             clauses ...)                      (let ((v expr))
                                                 (guarded-match v pattern
                                                                template
                                                                (match v clauses ...))))
    ((_ expr (pattern guard template)
             clauses ...)                      (let ((v expr))
                                                 (guarded-match v pattern guard
                                                                template
                                                                (match v clauses ...))))))

(define-syntax match-lambda
  (syntax-rules ()
    ((_ (pat expr ...) ...)         (lambda (x) (match x (pat expr ...) ...)))))

(define-syntax match-lambda*
  (syntax-rules ()
    ((_ (pat expr ...) ...)         (lambda x   (match x (pat expr ...) ...)))))

(define-syntax match-let*
  (syntax-rules ()
    ((_ () body ...)                                (let () body ...))
    ((_ ((pat expr)) body ...)                      ((match-lambda (pat body ...)) expr))
    ((_ ((pat expr) (pat2 expr2) ...) body ...)     (match-let* ((pat expr))
                                                      (match-let* 
                                                          ((pat2 expr2) ...) 
                                                        body ...)))))

(define-syntax match-let 
  (syntax-rules ()
    ((_ () body ...)               (let () body ...))
    ((_ ((pat expr) ...) body ...) (match-let* (((list pat ...) (list expr ...))) body ...))))


(define-syntax match-define-values-helper
  (syntax-rules ()
    ((_ (id ...) (pat) (expr))                   (match expr
                                                   (pat  (values id ...))))
    ((_ (id ...) (pat . pats) (expr . exprs))    (match expr
                                                   (pat  (values id ...))
                                                   (else (match-define-values-helper (id ...) pats exprs))))))

(define-syntax match-define-values
  (syntax-rules ()
    ((_ (id ...) (pat expr))                   (define-values (id ...)
                                                 (match-define-values-helper (id ...) (pat) (expr))))
    ((_ (id ...) (pat expr) ...)               (define-values (id ...)
                                                 (match-define-values-helper (id ...) (pat ...) (expr ...))))))

;; Test

(define-syntax test-simple 
  (syntax-rules ()
    ((_ value pattern success failure) (let ((test-simple-var value))
                                        (simple-match test-simple-var pattern success failure)))))
