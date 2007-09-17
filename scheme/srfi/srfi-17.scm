
(define-syntax set!
  (syntax-rules ()
    ((set! (?e0 ?e1 ...) ?v)
     ((setter ?e0) ?e1 ... ?v))
    ((set! ?i ?v)
     (scheme-set! ?i ?v))))

(define (setter proc)
  (let ((probe (assv proc setters)))
    (if probe
	(cdr probe)
	(assertion-violation 'setter "No setter found" proc))))

(define (set-setter! proc setter)
  (let ((probe (assv proc setters)))
    (if probe
	(set-cdr! probe setter)
	(scheme-set! setters
		     (cons (cons proc setter)
			   setters)))
    (unspecific)))

(define (car-setter proc)
  (lambda (p v)
    (set-car! (proc p) v)))

(define (cdr-setter proc)
  (lambda (p v)
    (set-cdr! (proc p) v)))

(define setters
  (list (cons setter     set-setter!)
	(cons vector-ref vector-set!)
	(cons string-ref string-set!)
	(cons car        set-car!)
	(cons cdr        set-cdr!)

	(cons caar   (car-setter car))
	(cons cdar   (cdr-setter car))
	(cons cadr   (car-setter cdr))
	(cons cddr   (cdr-setter cdr))

	(cons caaar  (car-setter caar))
	(cons cdaar  (cdr-setter caar))
	(cons cadar  (car-setter cdar))
	(cons cddar  (cdr-setter cdar))
	(cons caadr  (car-setter cadr))
	(cons cdadr  (cdr-setter cadr))
	(cons caddr  (car-setter cddr))
	(cons cdddr  (cdr-setter cddr))

	(cons caaaar (car-setter caaar))
	(cons cdaaar (cdr-setter caaar))
	(cons cadaar (car-setter cdaar))
	(cons cddaar (cdr-setter cdaar))
	(cons caadar (car-setter cadar))
	(cons cdadar (cdr-setter cadar))
	(cons caddar (car-setter cddar))
	(cons cdddar (cdr-setter cddar))
	(cons caaadr (car-setter caadr))
	(cons cdaadr (cdr-setter caadr))
	(cons cadadr (car-setter cdadr))
	(cons cddadr (cdr-setter cdadr))
	(cons caaddr (car-setter caddr))
	(cons cdaddr (cdr-setter caddr))
	(cons cadddr (car-setter cdddr))
	(cons cddddr (cdr-setter cdddr))))

