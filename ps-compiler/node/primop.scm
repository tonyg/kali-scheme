
; Copyright (c) 1993-1999 by Richard Kelsey.  See file COPYING.

;   The information about a primitive operation.

(define-record-type primop
  (id             ; Symbol identifying this primop

   trivial?       ; #t if this primop has does not require a continuation
   side-effects   ; side-effects of this primop

   simplify-call-proc ; Simplify method
   primop-cost-proc   ; Cost of executing this operation
                      ; (in some undisclosed metric)
   return-type-proc   ; Give the return type (for trivial primops only)
   proc-data      ; Record containing more data for the procedure primops
   cond-data      ; Record containing more data for conditional primops
   )
  (code-data      ; Code generation data
   ))

(define-record-discloser type/primop
  (lambda (primop)
    (list 'primop (object-hash primop) (primop-id primop))))

(define all-primops (make-vector primop-count))

(define (make-primop id trivial? side-effects simplify cost type)
  (let ((enum (name->enumerand id primop))
	(primop (primop-maker id trivial? side-effects simplify cost type #f #f)))
    (if enum
	(vector-set! all-primops enum primop))
    primop))

(define (get-primop enum)
  (vector-ref all-primops enum))

(define-local-syntax (define-primop-method id args)
  `(define (,id  . ,args)
     ((,(concatenate-symbol 'primop- id '- 'proc) (call-primop ,(car args)))
      . ,args)))

(define-primop-method primop-cost (call))
(define-primop-method simplify-call (call))

(define (trivial-call-return-type call)
  ((primop-return-type-proc (call-primop call)) call))

;-------------------------------------------------------------------------------
; procedure primops

(define-subrecord primop primop-proc-data primop-proc-data
  (call-index              ; index of argument being called
   )
  ())

(define (primop-procedure? primop)
  (if (primop-proc-data primop) #t #f))

; (call <cont> <proc-var> . <args>)
; (tail-call <cont-var> <proc-var> . <args>)
; (return <proc-var> . <args>)
; (jump   <proc-var> . <args>)
; (throw  <proc-var> . <args>)
;
; (unknown-call <cont> <proc-var> . <args>)
; (unknown-tail-call <cont-var> <proc-var> . <args>)
; (unknown-return <proc-var> . <args>)

(define (make-proc-primop id side-effects simplify cost index)
  (let* ((enum (name->enumerand id primop))
	 (data (primop-proc-data-maker index))
	 (primop (primop-maker id #f side-effects simplify cost #f data #f)))
    (vector-set! all-primops enum primop)
    primop))

;-------------------------------------------------------------------------------
; conditional primops

(define-subrecord primop primop-cond-data primop-cond-data
  (expand-to-conditional-proc     ; Expand this call to a conditional
   simplify-conditional?-proc     ; Can this conditional be simplified
   )
  ())

(define-primop-method expand-to-conditional (call))
(define-primop-method simplify-conditional? (call index value))

(define (primop-conditional? primop)
  (if (primop-cond-data primop) #t #f))

(define (make-conditional-primop id side-effects simplify cost expand simplify?)
  (let* ((enum (name->enumerand id primop))
	 (data (primop-cond-data-maker expand simplify?))
	 (primop (primop-maker id #f side-effects simplify cost #f #f data)))
    (if enum (vector-set! all-primops enum primop))
    primop))

;-------------------------------------------------------------------------------
; Random constants for location calls:

;  ($CONTENTS     <thing> <type> <offset> <rep>)
;  ($SET-CONTENTS <cont> <thing> <type> <offset> <rep> <value>)
;                    0      1       2       3      4 

(define loc/owner    0)
(define loc/type     1)
(define loc/rep      2)

(define set/owner    1)
(define set/type     2)
(define set/rep      3)
(define set/value    4)

; For slots that do not contain code pointers:
;  ($CLOSURE        <cont> <env> <slot>)
;  ($SET-CLOSURE    <cont> <env> <slot> <value>)
; For slots that do contain code pointers:
;  ($MAKE-PROCEDURE <cont> <env> <slot>)
;  ($SET-CODE       <cont> <env> <slot> <value>)
; For known calls to slots that contain code pointers:
;  ($ENV-ADJUST     <cont> <env> <slot>)
;                     0      1      2

(define env/owner    0)
(define env/offset   1)
(define env/value    2)

