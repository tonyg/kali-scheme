; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.



; Rudimentary type reconstruction, hardly worthy of the name.

; Currently, NODE-TYPE is called in two places.  One is to determine
; the type of the right-hand side of a DEFINE for a variable that is
; never assigned, so uses of the variable can be checked later.  The
; other is when compiling a call, to check types of arguments and
; produce warning messages.

; This is heuristic, to say the least.  It's not clear what the right
; interface or formalism is for Scheme; I'm still experimenting.

; Obviously we can't do Hindley-Milner inference.  Not only does
; Scheme have subtyping, but it also has dependent types up the wazoo.
; For example, the following is perfectly correct Scheme:
;
;   (define (foo x y) (if (even? x) (car y) (vector-ref y 3)))


(define (node-type node env)
  ;; Ignore env, since we don't ever call CLASSIFY or LOOKUP.
  (reconstruct node 'fast any-values-type))

(define (reconstruct-type node env)
  (reconstruct node '() any-values-type))

(define (reconstruct node constrained want-type)
  (cond ((node? node)
         ((operator-table-ref reconstructors (node-operator-id node))
          node constrained want-type))
        ((pair? node) any-values-type)
        ((name? node) value-type)
        (else (constant-type node))))

(define reconstructors
  (make-operator-table (lambda (node constrained want-type)
                         (reconstruct-call node constrained want-type))))

(define (define-reconstructor name type proc)
  (operator-define! reconstructors name type proc))


(define-reconstructor 'lambda syntax-type
  (lambda (node constrained want-type)
    (if (eq? constrained 'fast)
        any-procedure-type
        (let ((form (node-form node))
              (var-nodes (node-ref node 'var-nodes))
              (want-result (careful-codomain want-type)))
          (let ((formals (cadr form)))
            (if var-nodes
                (let* ((alist (map (lambda (node) (cons node value-type))
                                   var-nodes))
                       ;; We can't do (append alist constrained) because the
                       ;; lambda might not be called...
                       (cod (reconstruct-body (cddr form)
                                              alist
                                              want-result)))
                  (procedure-type (if (n-ary? formals)
                                      any-values-type ;lose
                                      (make-some-values-type (map cdr alist)))
                                  cod
                                  #t))
                (procedure-type
                 (if (n-ary? formals)
                     any-values-type    ;lose
                     (make-some-values-type (map (lambda (f) value-type)
                                                 formals)))
                 (reconstruct-body (cddr form) constrained want-result)
                 #t)))))))

(define (careful-codomain proc-type)
  (if (procedure-type? proc-type)
      (procedure-type-codomain proc-type)
      any-values-type))

(define (reconstruct-body body constrained want-type)
  (if (null? (cdr body))
      (reconstruct (car body) constrained want-type)
      any-values-type))

(define operator/name (get-operator 'name))

(define-reconstructor 'name 'leaf
  (lambda (node constrained want-type)
    (if (eq? constrained 'fast)
        (reconstruct-name node)
        (let ((z (assq node constrained)))
          (if z
              (let ((type (meet-type (cdr z) want-type)))
                (begin (set-cdr! z type)
                       type))
              (reconstruct-name node))))))

(define (reconstruct-name node)
  (let ((probe (node-ref node 'binding)))
    (if (binding? probe)
        (let ((t (binding-type probe)))
          (cond ((variable-type? t) (variable-value-type t))
                ((subtype? t value-type) t)
                (else value-type)))
        value-type)))

(define (reconstruct-call node constrained want-type)
  (let* ((form (node-form node))
         (op-type (reconstruct (car form)
                               constrained
                               (procedure-type any-arguments-type
                                               want-type
                                               #f)))
         (args (cdr form))
         (lose (lambda ()
                 (for-each (lambda (arg)
                             (examine arg constrained value-type))
                           args))))
    (if (procedure-type? op-type)
        (begin (if (restrictive? op-type)
                   (let loop ((args args)
                              (dom (procedure-type-domain op-type)))
                     (if (not (or (null? args)
                                  (empty-rail-type? dom)))
                         (begin (examine (car args)
                                         constrained
                                         (head-type dom))
                                (loop (cdr args) (tail-type dom)))))
                   (lose))
               (procedure-type-codomain op-type))
        (begin (lose)
               any-values-type))))

(define-reconstructor 'literal 'leaf
  (lambda (node constrained want-type)
    (constant-type (node-form node))))

(define-reconstructor 'quote syntax-type
  (lambda (node constrained want-type)
    (constant-type (cadr (node-form node)))))

(define-reconstructor 'if syntax-type
  (lambda (node constrained want-type)
    (let ((form (node-form node)))
      (examine (cadr form) constrained value-type)
      ;; Fork off two different constrain sets
      (let ((con-alist (fork-constraints constrained))
            (alt-alist (fork-constraints constrained)))
        (let ((con-type (reconstruct (caddr form) con-alist want-type))
              (alt-type (reconstruct (cadddr form) alt-alist want-type)))
          (if (pair? constrained)
              (for-each (lambda (c1 c2 c)
                          (set-cdr! c (join-type (cdr c1) (cdr c2))))
                        con-alist
                        alt-alist
                        constrained))
          (join-type con-type alt-type))))))


(define (fork-constraints constrained)
  (if (pair? constrained)
      (map (lambda (x) (cons (car x) (cdr x)))
           constrained)
      constrained))

(define-reconstructor 'begin syntax-type
  (lambda (node constrained want-type)
    ;; This is unsound - there might be a throw out of some subform
    ;; other than the final one.
    (do ((forms (cdr (node-form node)) (cdr forms)))
        ((null? (cdr forms))
         (reconstruct (car forms) constrained want-type))
      (examine (car forms) constrained any-values-type))))

(define (examine node constrained want-type)
  (if (pair? constrained)
      (reconstruct node constrained want-type)
      want-type))

(define-reconstructor 'set! syntax-type
  (lambda (node constrained want-type)
    (examine (caddr (node-form node)) constrained value-type)
    unspecific-type))

(define-reconstructor 'letrec syntax-type
  (lambda (node constrained want-type)
    (let ((form (node-form node)))
      (if (eq? constrained 'fast)
          (reconstruct (last form) 'fast want-type)
          (let ((types (map (lambda (spec)
                              (reconstruct (cadr spec) constrained value-type))
                            (cadr form))))
            (reconstruct (last form)
                         (let ((nodes (node-ref node 'var-nodes)))
                           (if nodes
                               (append (map cons nodes types)
                                       constrained)
                               constrained))
                         want-type))))))

(define-reconstructor 'primitive-procedure syntax-type
  (lambda (node constrained want-type)
    (operator-type (get-operator (cadr (node-form node))))))    ;mumble

(define-reconstructor 'loophole syntax-type
  (lambda (node constrained want-type)
    (let ((args (cdr (node-form node))))
      (examine (cadr args) constrained any-values-type)
      (sexp->type (schemify (car args)) #t))))  ;Foo

(define (node->type node)
  (if (node? node)
      (let ((form (node-form node)))
        (if (pair? form)
            (map node->type form)
            (desyntaxify form)))
      (desyntaxify node)))

(define-reconstructor 'define syntax-type
  (lambda (node constrained want-type)
    ':definition))

(define-reconstructor 'define-syntax syntax-type
  (lambda (node constrained want-type)
    ':definition))


(define call-node? (node-predicate 'call))
(define name-node? (node-predicate 'name))
(define begin-node? (node-predicate 'begin))





; --------------------
; Primitive procedures:

(define-reconstructor 'values any-procedure-type
  (lambda (node constrained want-type)
    (make-some-values-type (map (lambda (node)
                                  (meet-type
                                   (reconstruct node constrained value-type)
                                   value-type))
                                (cdr (node-form node))))))

(define-reconstructor 'call-with-values
                      (proc ((proc () any-values-type #f)
                             any-procedure-type)
                            any-values-type)
  (lambda (node constrained want-type)
    (let* ((args (cdr (node-form node)))
           (thunk-type (reconstruct (car args)
                                    constrained
                                    (procedure-type empty-rail-type
                                                    any-values-type
                                                    #f))))
      (careful-codomain
            (reconstruct (cadr args)
                         constrained
                         (procedure-type (careful-codomain thunk-type)
                                         any-values-type
                                         #f))))))

(define (reconstruct-apply node constrained want-type)
  (let* ((args (cdr (node-form node)))
         (proc-type (reconstruct (car args)
                                 constrained
                                 any-procedure-type)))
    (for-each (lambda (arg) (examine arg constrained value-type))
              (cdr args))
    (careful-codomain proc-type)))

(define-reconstructor 'apply
    (proc (any-procedure-type &rest value-type) any-values-type)
  reconstruct-apply)

(define-reconstructor 'primitive-catch
                      (proc ((proc (escape-type) any-values-type #f))
                            any-values-type)
  reconstruct-apply)


; --------------------
; Types of simple primitives.

(define (declare-operator-type ops type)
  (if (list? ops)
      (for-each (lambda (op) (get-operator op type))
                ops)
      (get-operator ops type)))

(declare-operator-type 'with-continuation
                       (proc (escape-type (proc () any-values-type #f))
                             any-arguments-type))
 
(declare-operator-type 'eq?
                       (proc (value-type value-type) boolean-type))

(declare-operator-type '(number? integer? rational? real? complex?
                                 char? eof-object? input-port? output-port?)
                       (proc (value-type) boolean-type))

(declare-operator-type 'exact?
                       (proc (number-type) boolean-type))

(declare-operator-type 'exact->inexact (proc (exact-type) inexact-type))
(declare-operator-type 'inexact->exact (proc (inexact-type) exact-type))

(declare-operator-type '(exp log sin cos tan asin acos sqrt)
                       (proc (number-type) number-type))

(declare-operator-type '(atan)
                       (proc (number-type number-type) number-type))

(declare-operator-type '(floor)
                       (proc (real-type) integer-type))

(declare-operator-type '(real-part imag-part angle magnitude)
                       (proc (complex-type) real-type))

(declare-operator-type '(numerator denominator)
                       (proc (rational-type) integer-type))

(declare-operator-type '(+ * - /)
                       (proc (number-type number-type) number-type))

(declare-operator-type '(= <)
                       (proc (real-type real-type) boolean-type))

(declare-operator-type '(make-polar make-rectangular)
                       (proc (real-type real-type) complex-type))

(declare-operator-type '(quotient remainder)
                       (proc (integer-type integer-type) integer-type))

(declare-operator-type '(bitwise-not)
                       (proc (exact-integer-type) exact-integer-type))

(declare-operator-type '(bitwise-and bitwise-ior bitwise-xor
                         arithmetic-shift)
                       (proc (exact-integer-type exact-integer-type)
                             exact-integer-type))

(declare-operator-type '(char=? char<?)
                       (proc (char-type char-type) boolean-type))

(declare-operator-type 'char->ascii
                       (proc (char-type) exact-integer-type))

(declare-operator-type 'ascii->char
                       (proc (exact-integer-type) char-type))

(declare-operator-type 'string=?
                       (proc (string-type string-type) boolean-type))

(declare-operator-type 'open-port
                       ;; Can return #f
                       (proc (string-type exact-integer-type) value-type))

(declare-operator-type 'cons (proc (value-type value-type) pair-type))

(declare-operator-type 'intern (proc (string-type vector-type) symbol-type))

; Can't do I/O until the meta-types interface exports input-port-type and
; output-port-type.

(define (constant-type x)
  (cond ((number? x)
         (meet-type (if (exact? x) exact-type inexact-type)
                    (cond ((integer? x) integer-type)
                          ((rational? x) rational-type)
                          ((real? x) real-type)
                          ((complex? x) complex-type)
                          (else number-type))))
        ((boolean? x) boolean-type)
        ((pair? x) pair-type)
        ((string? x) string-type)
        ((char? x) char-type)
        ((null? x) null-type)
        ((symbol? x) symbol-type)
        ((vector? x) vector-type)
        (else value-type)))
