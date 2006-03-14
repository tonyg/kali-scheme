; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.


; The intermediate language (node tree)
; The structures VARIABLE and PRIMOP are contained in NODE.  They are used
; in client language code where the NODE- names conflict.

(define-structures ((node     node-interface)
		    (variable variable-interface)
		    (primop   primop-interface))
  (open scheme big-scheme comp-util arch parameters
	defrecord)
  (for-syntax (open scheme big-scheme let-nodes))
  (begin 
    (define-syntax let-nodes
      (lambda (form rename compare)
	(expand-let-nodes form rename compare)))
    (define-syntax new-lambda
      (lambda (form rename compare)
	(expand-new-lambda form rename compare)))
    (define-syntax new-call
      (lambda (form rename compare)
	(expand-new-call form rename compare))))
  (files (node node)         ; variable and node data structures
	 (node primop)       ; primop data structure
	 (node node-util)    ; various small utilities
	 (node node-equal))) ; node equality

;(define node
;  (let ()
;    (define-structure let-nodes (export expand-let-nodes)
;      (open scheme big-scheme arch)
;      (files (node let-nodes)))
;    (define-structures ((node     node-interface)
;                        (variable variable-interface)
;                        (primop   primop-interface))
;      (open scheme big-scheme comp-util arch parameters)
;      (for-syntax (open scheme big-scheme let-nodes))
;      (begin 
;        (define-syntax let-nodes
;          (lambda (form rename compare)
;            (expand-let-nodes form rename compare))))
;      (files (node node)         ; variable and node data structures
;             (node primop)       ; primop data structure
;             (node node-util)    ; various small utilities
;             (node node-equal)   ; node equality
;             (node leftovers)))  ; more node utilities
;    node))

; Pretty printer

(define-structure pp-cps (export pp-cps)
  (open scheme big-scheme comp-util node structure-refs)
  (access i/o)  ; force-output
  (files (node pp-cps)))

; Expander for LET-NODES, a macro for creating interconnected nodes.
	 
(define-structure let-nodes (export expand-let-nodes
				    expand-new-lambda
				    expand-new-call)
  (open scheme big-scheme arch)
  (files (node let-nodes)))

; Compiler Parameters
; This allows client languages to supply parameters to the compiler
; without introducing circular module dependencies.

(define-structures ((parameters parameter-interface)
		    (set-parameters (export set-compiler-parameter!)))
  (open scheme big-scheme)
  (files param))

; An enumerated type defining the standard primops.

(define-structure arch (export (primop :syntax) primop-count)
  (open scheme enumerated)
  (files (node arch)))

; linearizing node trees for later reuse

(define-structure node-vector (export node->vector
				      vector->node
				      vector->leaf-node)
  (open scheme big-scheme comp-util node parameters
	defrecord)
  (files (node vector)))

; Translating the input forms into simplified node trees

(define-structures ((front       front-interface)
		    (front-debug front-debug-interface))
  (open scheme big-scheme comp-util node simplify parameters jump
	remove-cells flow-values)
  (files (front top)))       ; main entry points and debugging utilities

(define-structure cps-util (export cps-call cps-sequence)
  (open scheme big-scheme comp-util node
	define-record-types)
  (files (front cps)))

; Converting tail-recursive calls to jumps

(define-structure jump (export integrate-jump-procs!
			       find-jump-procs
			       procs->jumps)
  (open scheme big-scheme comp-util node parameters ssa
	define-record-types)
  (files (front jump)))

; Program simplification and partial evaluation

(define-structures ((simplify (export simplify-node))
		    (simplify-internal simplify-internal-interface))
  (open scheme big-scheme comp-util node parameters node-vector)
  (for-syntax (open scheme big-scheme simp-patterns))
  (begin
    (define-syntax pattern-simplifier
      (lambda (form rename compare)
	(make-pattern-simplifier (cdr form) rename compare))))
  (files (simp simplify)  ; main entry point and driver
	 (simp call)))    ; simplifiers for some of the standard primops

; Simplifying calls to lambda nodes

(define-structure simplify-let (export simplify-let)
  (open scheme big-scheme comp-util node parameters
	simplify-join simplify-internal)
  (files (simp let)))

; Substituting lambda nodes that are bound by calls to lambda nodes,
; trying to maximize the further simplification opportunites while
; minimizing code expansion.

(define-structure simplify-join (export substitute-join-arguments)
  (open scheme big-scheme comp-util node)
  (files (simp join)))

; The expander for PATTERN-SIMPLIFIER, a macro for writing algebraic
; transformations.

(define-structure simp-patterns (export make-pattern-simplifier)
  (open scheme big-scheme defrecord fluids)
  (files (simp pattern)))

; Replacing cells with values passed as parameters, currently empty
; and unused (the code has not been made compatible with the current
; version of the compiler).

(define-structure remove-cells (export remove-cells-from-tree)
  (open scheme big-scheme)
  (begin
    (define (remove-cells-from-tree . stuff)
      (error "REMOVE-CELLS-FROM-TREE is undefined"))))

; Flow analysis, also currently empty and unused for the same reason.

(define-structure flow-values (export flow-values)
  (open scheme big-scheme)
  (begin
    (define (flow-values . stuff)
      (error "FLOW-VALUES is undefined"))))

; A random collection of utilities.

(define-structure comp-util utilities-interface
  (open scheme big-scheme defrecord structure-refs expanding-vectors)
  (for-syntax (open scheme big-scheme))
  (access primitives features)
  (files (util syntax)        ; macro for defining subrecords
	 (util util)))        ; random utilities

(define-structure expanding-vectors (export make-xvector
					    xvector-length
					    xvector-ref
					    xvector-set!
					    xvector-length
					    xvector->vector)
  (open scheme define-record-types)
  (files (util expand-vec)))

(define-interface transitive-interface
  (export make-graph-from-predecessors
	  make-graph-from-successors
	  transitive-or! transitive-or-with-kill! transitive-or-with-pass!
	  transitive-and! transitive-and-with-kill! transitive-and-with-pass!))

(define-structure transitive transitive-interface
  (open scheme big-scheme integer-sets defrecord)
  (optimize auto-integrate)
  (files (util transitive)))

(define-interface integer-set-interface
  (export make-empty-integer-set
	  add-to-integer-set
	  integer-set-not
	  integer-set-ior
	  integer-set-and
	  integer-set-subtract
	  integer-set-equal?
	  map-over-integer-set))

(define-structure integer-sets integer-set-interface
  (open scheme bitwise bigbit)
  (optimize auto-integrate)
  (files (util z-set)))

(define-structure strongly-connected (export strongly-connected-components)
  (open scheme big-scheme defrecord)
  (optimize auto-integrate)
  (files (util strong)))

(define-structure dominators (export find-dominators!)
  (open scheme big-scheme comp-util
	define-record-types)
  (optimize auto-integrate)
  (files (util dominators)))

(define-structure ssa (export graph->ssa-graph! find-joins)
  (open scheme big-scheme dominators
	define-record-types)
  (optimize auto-integrate)
  (files (util ssa)))

; Vectors of bytes, a renaming of Scheme 48's code vectors.

(define-structure compiler-byte-vectors compiler-byte-vector-interface
  (open scheme byte-vectors bitwise signals)
  (optimize auto-integrate)
  (files (util byte-vector)))

; A version of READ that annotates pairs with source file, line, and
; column information.

(define-structure annotated-read annotated-read-interface
  ; this is correct for linking, but doesn't work when loading
  ;(open defrecord extended-ports primitives scheme assembler)
  (open scheme big-scheme primitives fluids assembler)
  (files (prescheme track-read)))




