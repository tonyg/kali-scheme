;----------------
; SRFI packages

; SRFI-0 - Doesn't work with the module system.

; Olin's list library.

(define-interface srfi-1-interface
  (export map for-each member assoc	; redefined from R5RS
	  xcons make-list list-tabulate cons* list-copy 
	  proper-list? circular-list? dotted-list? not-pair? null-list? list=
	  circular-list length+
	  iota
	  first second third fourth fifth sixth seventh eighth ninth tenth
	  car+cdr
	  take       drop       
	  take-right drop-right 
	  take!      drop-right!
	  split-at   split-at!
	  last last-pair
	  zip unzip1 unzip2 unzip3 unzip4 unzip5
	  count
	  append! append-reverse append-reverse! concatenate concatenate! 
	  unfold       fold       pair-fold       reduce
	  unfold-right fold-right pair-fold-right reduce-right
	  append-map append-map! map! pair-for-each filter-map map-in-order
	  filter  partition  remove
	  filter! partition! remove! 
	  find find-tail any every list-index
	  take-while drop-while take-while!
	  span break span! break!
	  delete delete!
	  alist-cons alist-copy
	  delete-duplicates delete-duplicates!
	  alist-delete alist-delete!
	  reverse! 
	  lset<= lset= lset-adjoin  
	  lset-union  lset-intersection  lset-difference  lset-xor
	  lset-diff+intersection
	  lset-union! lset-intersection! lset-difference! lset-xor!
	  lset-diff+intersection!))

(define-structure srfi-1 srfi-1-interface
  (open (modify scheme-level-2 (hide map for-each member assoc)) ; redefined
	receiving
	(subset signals (error)))
  (files srfi-1))

; AND-LET*: an AND with local bindings, a guarded LET* special form

(define-structure srfi-2 (export (and-let* :syntax))
  (open scheme-level-2
	signals)		; error
  (files srfi-2))

; SRFI-3 - withdrawn
; SRFI-4 - needs hacks to the reader

; A compatible let form with signatures and rest arguments

(define-structure srfi-5 (export (let :syntax))
  (open (modify scheme-level-2 (rename (let standard-let))))
  (files srfi-5))

; Basic String Ports

(define-structure srfi-6 (export open-input-string
				 open-output-string
				 get-output-string)
  (open (modify extended-ports
		(rename (make-string-input-port    open-input-string)
			(make-string-output-port   open-output-string)
			(string-output-port-output get-output-string)))))

; Configuration language

(define-structure srfi-7 (export program)	; also defines a command
  (open scheme

	; for parsing programs
	receiving
	nondeterminism
	(subset signals (error))

	(subset evaluation			(eval-from-file))

	(subset package-commands-internal	(config-package))
	ensures-loaded
	(subset packages			(note-structure-name!))

	; for defining the command
	(subset command-processor		(define-user-command-syntax
						 user-command-environment))
	(subset environments			(environment-define! environment-ref))
        
        ; for opening needed packages during evaluation
        (subset package-mutation                (package-open!))
        (subset packages                        (make-modified-structure)))

  (begin
    (define available-srfis
      '(srfi-1 srfi-2 srfi-5 srfi-6 srfi-7 srfi-8 srfi-9
	srfi-11 srfi-13 srfi-14 srfi-16 srfi-17
	srfi-23 srfi-25 srfi-26 srfi-27 srfi-28
	srfi-31 srfi-34 srfi-35 srfi-36 srfi-37
	srfi-42 srfi-45))

    ; Some SRFI's redefine Scheme variables.
    (define shadowed
      '((srfi-1 map for-each member assoc)
	(srfi-5 let)
	(srfi-13 string->list string-copy string-fill!)
	(srfi-17 set!)
	(srfi-45 force delay)))
    )

  (files srfi-7))

; receive: Binding to multiple values
; Taken directly from the SRFI document (or from `receiving', take your pick).

(define-structure srfi-8 (export (receive :syntax))
  (open scheme-level-2)
  (begin
    (define-syntax receive
      (syntax-rules ()
	((receive formals expression body ...)
	 (call-with-values (lambda () expression)
			   (lambda formals body ...)))))))

; Defining Record Types
; SRFI-9 is a slight modification of DEFINE-RECORD-TYPE.

(define-structure srfi-9 (export (define-record-type :syntax))
  (open scheme-level-2 
	(with-prefix define-record-types sys:))
  (begin
    (define-syntax define-record-type
      (syntax-rules ()
	((define-record-type type-name . stuff)
	 (sys:define-record-type type-name type-name . stuff))))))

; SRFI-10 - no stand-alone interface.

; Syntax for receiving multiple values

(define-structure srfi-11 (export (let-values :syntax)
				  (let*-values :syntax))
  (open scheme-level-2)
  (files srfi-11))

; SRFI-12 - withdrawn

; Two more encyclopedias from Olin.

; String Library

(define-interface srfi-13-interface
  (export string-map string-map!
	  string-fold       string-unfold
	  string-fold-right string-unfold-right 
	  string-tabulate string-for-each string-for-each-index
	  string-every string-any
	  string-hash string-hash-ci
	  string-compare string-compare-ci
	  string=    string<    string>    string<=    string>=    string<>
	  string-ci= string-ci< string-ci> string-ci<= string-ci>= string-ci<> 
	  string-downcase  string-upcase  string-titlecase  
	  string-downcase! string-upcase! string-titlecase! 
	  string-take string-take-right
	  string-drop string-drop-right
	  string-pad string-pad-right
	  string-trim string-trim-right string-trim-both
	  string-filter string-delete
	  string-index string-index-right 
	  string-skip  string-skip-right
	  string-count
	  string-prefix-length string-prefix-length-ci
	  string-suffix-length string-suffix-length-ci
	  string-prefix? string-prefix-ci?
	  string-suffix? string-suffix-ci?
	  string-contains string-contains-ci
	  string-copy! substring/shared
	  string-reverse string-reverse! reverse-list->string
	  string-concatenate
	  string-concatenate/shared
	  string-concatenate-reverse
	  string-concatenate-reverse/shared
	  string-append/shared
	  xsubstring string-xcopy!
	  string-null?
	  string-join
	  string-tokenize
	  string-replace

	  string->list string-copy string-fill! 
	  string? make-string string-length string-ref string-set! 
	  string string-append list->string))

(define-structure srfi-13 srfi-13-interface
  (open scheme-level-2
	bitwise
	srfi-8 srfi-14
	(subset signals		(error)))
  (files srfi-13))

; Character-Set Library

(define-interface srfi-14-interface
  (export char-set? char-set=
	  char-set<= char-set-hash char-set-cursor char-set-ref
	  char-set-cursor-next end-of-char-set? char-set-fold char-set-unfold
	  char-set-unfold!  char-set-for-each char-set-map char-set-copy
	  char-set

	  list->char-set  string->char-set 
	  list->char-set! string->char-set! 

	  char-set-filter  ucs-range->char-set

	  ; the SRFI defines ->CHAR-SET, but that isn't a legal identifier
	  x->char-set
	  
	  char-set-filter! ucs-range->char-set!

	  char-set->list char-set->string

	  char-set-size char-set-count char-set-contains?
	  char-set-every char-set-any

	  char-set-adjoin  char-set-delete 
	  char-set-adjoin! char-set-delete!
 

	  char-set-complement  char-set-union  char-set-intersection  
	  char-set-complement! char-set-union! char-set-intersection! 

	  char-set-difference  char-set-xor  char-set-diff+intersection
	  char-set-difference! char-set-xor! char-set-diff+intersection!

	  char-set:lower-case	char-set:upper-case	char-set:title-case
	  char-set:letter	char-set:digit		char-set:letter+digit
	  char-set:graphic	char-set:printing	char-set:whitespace
	  char-set:iso-control	char-set:punctuation	char-set:symbol
	  char-set:hex-digit	char-set:blank		char-set:ascii
	  char-set:empty	char-set:full

	  ))

(define-structure srfi-14 srfi-14-interface
  (open scheme-level-2
	bitwise
	srfi-9
	(modify ascii (rename (char->ascii %char->latin1)
			      (ascii->char %latin1->char)))
	(subset features (make-immutable!))
	(subset signals (error)))
  (files srfi-14))

; SRFI-15 - withdrawn

; Syntax for procedures of variable arity

(define-structure srfi-16 (export (case-lambda :syntax))
  (open scheme-level-2
	(subset signals (error)))
  (files srfi-16))

; Generalized set!

(define-structure srfi-17 (export (set! :syntax) setter)
  (open (modify scheme-level-2 (rename (set! scheme-set!)))
	(subset signals (error))
	(subset util (unspecific)))
  (files srfi-17))

; SRFI-18 - no implementation given
; SRFI-19 - implementation is specific to MzScheme
; SRFI-20 - withdrawn
; SRFI-21 - no implementation given
; SRFI-22 - needs internals hacking

; Error reporting mechanism

(define-structure srfi-23 (export error)
  (open (subset signals (error))))

; Multi-dimensional Array Primitives 

(define-interface srfi-25-interface
  (export array? make-array shape array array-rank
	  array-start array-end array-ref array-set! share-array))

(define-structure srfi-25 srfi-25-interface
  (open scheme
	define-record-types
	(subset signals (error)))
  (files srfi-25))

; Notation for Specializing Parameters without Currying

(define-interface srfi-26-interface
  (export ((cut cute) :syntax)))

(define-structure srfi-26 srfi-26-interface
  (open scheme)
  (files srfi-26))

; Sources of Random Bits

(define-interface srfi-27-interface
  (export random-integer
          random-real
          default-random-source
          make-random-source
          random-source?
          random-source-state-ref
          random-source-state-set!
          random-source-randomize!
          random-source-pseudo-randomize!
          random-source-make-integers
          random-source-make-reals))

(define-structure srfi-27 srfi-27-interface
  (open
   scheme-level-1
   floatnums
   external-calls
   (subset srfi-9 (define-record-type)) 
   (subset srfi-23 (error))
   (subset posix-time (current-time time-seconds)))
  (files srfi-27))

; Basic Format Strings

(define-interface srfi-28-interface
  (export format))

(define-structure srfi-28 srfi-28-interface
  (open scheme
	(subset signals (error))
	srfi-6				; string ports
	)
  (files srfi-28))

; A special form for recursive evaluation

(define-interface srfi-31-interface
  (export (rec :syntax)))

(define-structure srfi-31 srfi-31-interface
  (open scheme)
  (begin
    ; Taken directly from the SRFI document.
    
    (define-syntax rec
      (syntax-rules ()
	((rec (NAME . VARIABLES) . BODY)
	 (letrec ( (NAME (lambda VARIABLES . BODY)) ) NAME))
	((rec NAME EXPRESSION)
	 (letrec ( (NAME EXPRESSION) ) NAME))))))

; Exception Handling for Programs

(define-interface srfi-34-interface
  (export with-exception-handler
	  raise
	  (guard :syntax)))

(define-structure srfi-34 srfi-34-interface
  (open scheme
	exceptions))

; Conditions

(define-interface srfi-35-interface
  (export make-condition-type
	  condition-type?
	  make-condition
	  condition?
	  condition-has-type?
	  condition-ref
	  make-compound-condition
	  extract-condition

	  (define-condition-type :syntax)
	  (condition :syntax)

	  &condition
	  &message message-condition? condition-message
	  &serious serious-condition?
	  &error error?))

(define-structure srfi-35 srfi-35-interface
  (open scheme
	conditions))

; I/O Conditions

(define-interface srfi-36-interface
  (export &i/o-error i/o-error?
	  &i/o-port-error i/o-port-error?
	  i/o-error-port
	  &i/o-read-error i/o-read-error?
	  &i/o-write-error i/o-write-error?
	  &i/o-closed-error i/o-closed-error?
	  &i/o-filename-error i/o-filename-error?
	  i/o-error-filename
	  &i/o-malformed-filename-error i/o-malformed-filename-error?
	  &i/o-file-protection-error i/o-file-protection-error?
	  &i/o-file-is-read-only-error i/o-file-is-read-only-error?
	  &i/o-file-already-exists-error i/o-file-already-exists-error?
	  &i/o-no-such-file-error i/o-no-such-file-error?
	  &read-error read-error?
	  read-error-line read-error-column read-error-position read-error-span))

(define-structure srfi-36 srfi-36-interface
  (open scheme
	i/o-conditions))

; args-fold: a program argument processor

(define-interface srfi-37-interface
  (export args-fold
	  option-processor
	  option option-names option-required-arg? option-optional-arg?))

(define-structure srfi-37 srfi-37-interface
  (open scheme
	define-record-types
	srfi-11)
  (files srfi-37))

; Eager Comprehensions

(define-interface srfi-42-interface
  (export ((do-ec
	    list-ec append-ec
	    string-ec string-append-ec
	    vector-ec vector-of-length-ec
	    sum-ec product-ec
	    min-ec max-ec
	    any?-ec every?-ec
	    first-ec last-ec
	    fold-ec fold3-ec) :syntax)
	  ((:
	    :list :string :vector
	    :integers
	    :range :real-range :char-range
	    :port
	    :dispatched) :syntax)
	  ((:do :let :parallel :while :until) :syntax)
	  :-dispatch-ref :-dispatch-set! make-initial-:-dispatch 
	  (:generator-proc :syntax)))

(define-structure srfi-42 srfi-42-interface
  (open scheme
	srfi-23)
  (files srfi-42))


; SRFI-45: Primitives for Expressing Iterative Lazy Algorithms

(define-interface srfi-45-interface
  (export (lazy :syntax) force (delay :syntax)))

(define-structure srfi-45 srfi-45-interface
  (open scheme
	define-record-types)
  (files srfi-45))
