; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.


(define-interface utilities-interface
  (export bug
          user-error
          user-warning
          true false
          or-map
          remove-similar-elts
          select-from-table
          table->list table->entry-list
          table-push table-pop
          merge-lists
          vector-every?
          make-ignorable
          sub-vector->list
          flag-assq

          enforce
          writec
          mem?
          walk-vector
          vector-replace
          copy-list
          copy-vector
          symbol-hash
          string-hash
          char->ascii
          object-hash
          union intersection set-difference

          make-xvector xvector-length xvector-ref xvector-set! xvector->vector

          (define-subrecord :syntax)
          ;(define-simple-record-type :syntax)
          (define-local-syntax :syntax)
          ))

(define-interface primop-interface
  (export primop? make-primop make-proc-primop make-conditional-primop
          all-primops get-primop

          primop-id primop-trivial? primop-side-effects
          primop-cost
          simplify-call
          primop-procedure? primop-call-index
          primop-conditional?
          expand-to-conditional
          simplify-conditional?
          primop-code-data set-primop-code-data!
	  trivial-call-return-type

	  (primop :syntax)
          ))

(define-interface variable-interface
  (export variable? make-variable
          global-variable? make-global-variable
          variable-name set-variable-name!
          variable-id
          variable-type   set-variable-type!
          variable-binder set-variable-binder!
          variable-refs   set-variable-refs!
          variable-flag   set-variable-flag!
          variable-flags  set-variable-flags!
	  variable-generate set-variable-generate!
          erase-variable
          variable-index copy-variable used? unused?
          variable-known-value
	  add-variable-known-value!
	  remove-variable-known-value!
          variable-simplifier
	  add-variable-simplifier!
	  remove-variable-simplifier!
	  note-known-global-lambda!
          ))

(define-interface node-interface
  (compound-interface
   primop-interface
   variable-interface
   (export reset-node-id node-hash node-unhash

           node? node-variant
           node-parent set-node-parent!
           node-index set-node-index!
           node-simplified? set-node-simplified?!
           node-flag set-node-flag!

           empty empty? proclaim-empty

           erase

           detach detach-body
           attach attach-body
           move move-body
           insert-body
           replace replace-body
	   connect-sequence

           mark-changed

           leaf-node?
          
           literal-node? make-literal-node
           literal-value set-literal-value!
           literal-type  set-literal-type!
           copy-literal-node

           reference-node? make-reference-node
           reference-variable set-reference-variable!

           call-node? make-call-node
           call-primop set-call-primop!
           call-args set-call-args!
           call-exits set-call-exits!
           call-source set-call-source!
           call-arg call-arg-count

           lambda-node? make-lambda-node
           lambda-body set-lambda-body!
           lambda-variables set-lambda-variables!
           lambda-name set-lambda-name!
           lambda-id
           lambda-type
           lambda-block set-lambda-block!
	   lambda-env set-lambda-env!
           lambda-protocol set-lambda-protocol!
           lambda-source set-lambda-source!
           lambda-variable-count
           calls-known? set-calls-known?!
	   proc-lambda?

           initialize-lambdas add-lambda add-lambdas
           change-lambda-type
           walk-lambdas make-lambda-list
          
           loc/owner loc/type loc/rep
           set/owner set/type set/rep set/value

           node-base containing-procedure
           trivial? nontrivial?
           nontrivial-ancestor
           calls-this-primop?
           bound-to-variable
           walk-refs-safely
           small-node?
           side-effects?
           called-node? called-node
           called-lambda
           get-lambda-value
           ;set-reference?

           attach-call-args remove-call-args replace-call-args
           remove-null-arguments
           shorten-call-args insert-call-arg remove-call-arg
           append-call-arg

           remove-body

	   put-in-letrec make-letrec

           remove-lambda-variable remove-variable remove-unused-variables

           substitute substitute-vars-in-node-tree
           replace-call-with-value

           copy-node-tree

           mark-ancestors marked-ancestor? unmarked-ancestor?
           node-ancestor? marked-ancestor least-common-ancestor
           proc-ancestor

           hoistable-node?

           find-scoping

           ((let-nodes new-lambda new-call) :syntax)

           node-equal?

           no-free-references?

	   find-calls

	   node-type

           the-undefined-value
           undefined-value?
           undefined-value-node?
           make-undefined-literal
           )))

(define-interface simplify-internal-interface
  (export simplify-node
          default-simplifier
	  simplify-arg
          simplify-args
          simplify-lambda-body
	  simplify-known-lambda
          
          (pattern-simplifier :syntax)

          simplify-allocation
          simplify-known-call
          simplify-known-tail-call
          simplify-unknown-call
          simplify-return
          simplify-jump
          ; simplify-undefined-value
          simplify-test expand-test simplify-test?
          ))

(define-interface front-debug-interface
  (export debug-breakpoint
          add-checks add-check clear-checks clear-check
          add-procs add-proc clear-procs clear-proc))

(define-interface front-interface
  (export simplify-all
          integrate-jump-procs!  ; for debugging
          ))

(define-interface annotated-read-interface
  (export read-and-annotate
          pair-annotation
          annotated-cons
          annotation?
          annotation-file
          annotation-form
          annotation-row
          annotation-column
          ))

(define-interface compiler-byte-vector-interface
  (export make-byte-vector byte-vector? byte-vector-length
          byte-vector-ref  byte-vector-word-ref  byte-vector-half-word-ref
          byte-vector-set! byte-vector-word-set! byte-vector-half-word-set!
          byte-vector-endianess set-byte-vector-endianess!
          ))

(define-interface parameter-interface
  (export lookup-primop
	  lookup-imported-variable
          
          type/unknown
	  type-eq?

          lambda-node-type

          true-value
          false-value

	  determine-lambda-protocol
	  determine-continuation-protocol
          ))


