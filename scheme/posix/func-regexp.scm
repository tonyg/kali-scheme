; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; All regular expressions are records of the following type.
;
; type     - a symbol indicating which type of regular expression this is.
; compiled - cache for the compiled Posix regular expression; initially #F
; field0   - data fields containing
; field1   -   type-specific
; field2   -     information
;
; Regular expressions are not modified, except for adding cached Posix values.
;
; The different types of regular expressions are:
;   Set
;     use-case   - character bitmask for case-sensitive matching
;     no-case    - character bitmask for case-insensitive matching
;     string     - cached Posix regular expression for this set
;   String-Start
;   String-End
;   Submatch
;     subexp     - the subexpression whose match should be reported
;     id	 - EQ? tag used to identify the substring that matched
;   Sequence
;     subexps    - list subexpressions to be matched in order
;   One-Of
;     subexps    - list of subexpressions, any one of which is to match
;   Repeat
;     low        - minimum count
;     high       - maximum count, or #f if unbounded
;     subexp     - subexpression to be repeated

(define-record-type regexp :regexp
  (make-regexp type compiled field0 field1 field2)
  regexp?
  (type regexp-type)
  (compiled regexp-compiled set-regexp-compiled!)
  (field0 regexp-field0)
  (field1 regexp-field1)
  (field2 regexp-field2 set-regexp-field2!))

(define-record-discloser :regexp
  (lambda (regexp)
    (list 'regexp (regexp-type regexp))))

; A goofy macro to make the expression-type definitions more readable.

(define-syntax define-regexp-type
  (syntax-rules ()
    ((define-regexp-type name maker predicate)
     (begin
       (define (maker) (make-regexp 'name #f #f #f #f))
       (define (predicate regexp)
	 (eq? (regexp-type regexp) 'name))))
    ((define-regexp-type name maker predicate slot0)
     (begin
       (define (maker x)
	 (make-regexp 'name #f x #f #f))
       (define (predicate regexp)
	 (eq? (regexp-type regexp) 'name))
       (define slot0 regexp-field0)))
    ((define-regexp-type name maker predicate slot0 slot1)
     (begin
       (define (maker x y) (make-regexp 'name #f x y #f))
       (define (predicate regexp) (eq? (regexp-type regexp) 'name))
       (define slot0 regexp-field0)
       (define slot1 regexp-field1)))
    ((define-regexp-type name maker predicate slot0 slot1 slot2)
     (begin
       (define (maker x y z) (make-regexp 'name #f x y z))
       (define (predicate regexp) (eq? (regexp-type regexp) 'name))
       (define slot0 regexp-field0)
       (define slot1 regexp-field1)
       (define slot2 regexp-field2)))))
    
; Character sets
;
; Each has two bitsets, one which is the case-sensitive version of the set and
; the other the case-insensitive.
;
; There are two cached values, the POSIX regexp string for the set and the
; actual regexp object.  The second is set only if this set is the entire
; expression being matched.

(define-regexp-type set really-make-set set?
  set-use-case
  set-no-case
  set-string)

(define set-set-string! set-regexp-field2!)

(define (make-set case no-case)
  (really-make-set case no-case #f))

(define the-empty-set
  (really-make-set 0 0 #f))

(define (empty-set? set)
  (and (set? set)
       (= 0 (set-use-case set))
       (= 0 (set-no-case set))))

(define (char->mask char)
  (arithmetic-shift 1 (char->ascii char)))

(define char-limit 256)		; allow eight-bit characters

; A vector mapping ASCII values to case-insensitive bitsets.

(define no-case-char-masks
  (reduce ((count* i 0 char-limit))
	  ((masks '()))
    (cons (let ((ch (ascii->char i)))
	    (bitwise-ior (arithmetic-shift 1 i)
			 (cond ((char-upper-case? ch)
				(char->mask (char-downcase ch)))
			       ((char-lower-case? ch)
				(char->mask (char-upcase ch)))
			       (else
				0))))
	  masks)
    (list->vector (reverse masks))))

; A vector of the single-character sets.

(define singleton-sets
  (reduce ((count* i 0 char-limit))	; allow eight-bit characters
	  ((sets '()))
    (cons (make-set (arithmetic-shift 1 i)
		    (vector-ref no-case-char-masks i))
	  sets)
    (list->vector (reverse sets))))

(define (char->set char)
  (vector-ref singleton-sets (char->ascii char)))

; Arguments can be strings or single characters.  We walk down all of the
; characters, or-ing their masks together.

(define (set . all-args)
  (if (and (= 1 (length all-args))
	   (char? (car all-args)))
      (char->set (car all-args))
      (reduce ((list* arg all-args))
	      ((case 0)
	       (no-case 0))
	(cond ((char? arg)
	       (add-char-masks arg case no-case))
	      ((string? arg)
	       (add-string-masks arg case no-case))
	      (else
	       (apply call-error "invalid argument" set all-args)))
	(make-set case no-case))))
	      
(define (add-char-masks char case no-case)
  (values (bitwise-ior case (char->mask char))
	  (bitwise-ior no-case
		       (vector-ref no-case-char-masks
				   (char->ascii char)))))
  
(define (add-string-masks string case no-case)
  (reduce ((string* char string))
	  ((case case)
	   (no-case no-case))
    (add-char-masks char case no-case)))

; Ranges.  Again, we loop through the ranges building up two masks.

(define (range low high)
  (or (real-ranges `(,low ,high) char->integer integer->ascii)
      (call-error "invalid argument" range low high)))

(define (ranges . limits)
  (or (real-ranges limits char->integer integer->ascii)
      (apply call-error "invalid argument" ranges limits)))

(define (ascii-range low high)
  (or (real-ranges `(,low ,high) char->ascii identity)
      (call-error "invalid argument" ascii-range low high)))

(define (ascii-ranges . limits)
  (or (real-ranges limits char->ascii identity)
      (apply call-error "invalid argument" ascii-ranges limits)))

(define (integer->ascii i)
  (char->ascii (integer->char i)))

(define (identity i)
  i)

; LIMITS is a list of lists (<start-char> <end-char>), CHAR->INT returns an
; integer given a character and INT->ASCII translates that integer to the
; corresponding ASCII value.

(define (real-ranges limits char->int int->ascii)
  (if (every char? limits)
      (let loop ((to-do limits) (case 0) (no-case 0))
	(cond ((null? to-do)
	       (make-set case no-case))
	      ((null? (cdr to-do))
	       #f)
	      (else
	       (let ((start (char->int (car to-do)))
		     (end (char->int (cadr to-do))))
		 (if (< end start)
		     #f
		     (reduce ((count* i start (+ end 1)))
			     ((case case)
			      (no-case no-case))
			(let ((ascii (int->ascii i)))
			  (values (bitwise-ior case
					       (arithmetic-shift 1 ascii))
				  (bitwise-ior no-case
					       (vector-ref no-case-char-masks
							   ascii))))
			(loop (cddr to-do) case no-case)))))))
      #f))
	       
    
; Set operations.

(define (negate set)
  (make-set (bitwise-not (set-use-case set))
	    (bitwise-not (set-no-case set))))

(define (set-binop op)
  (lambda (set1 set2)
    (make-set (op (set-use-case set1)
		  (set-use-case set2))
	      (op (set-no-case set1)
		  (set-no-case set2)))))

(define intersection (set-binop bitwise-and))

(define union (set-binop bitwise-ior))

(define subtract
  (set-binop (lambda (x y)
	       (bitwise-xor x
			    (bitwise-and x y)))))

; Predefined sets.  These are from the LC_CTYPE category in the POSIX
; Locale, which is the nearest POSIX gets to defining character classes.

(define lower-case   (range #\a #\z))
(define upper-case   (range #\A #\Z))
(define alphabetic   (union lower-case upper-case))
(define numeric      (range #\0 #\9))
(define alphanumeric (union alphabetic numeric))
(define punctuation  (set "!\"#$%&'()*+,-./:;<=>?@ [\\]^_`{|}~"))
(define blank        (set #\space (ascii->char 9)))	;#\tab
(define graphic      (union alphanumeric punctuation))
(define whitespace   (apply set (map ascii->char
				     '(32	;space
				       9	;tab
				       10	;newline
				       11	;vertical tab
				       12	;form feed
				       13))))	;return
(define printing     (union graphic (set #\space)))
(define control      (negate printing))
(define hexdigit     (union numeric (ranges #\a #\f #\A #\F)))

;----------------
; String beginning and end anchors.

(define-regexp-type string-start make-string-start string-start?)
(define-regexp-type string-end make-string-end string-end?)

(define the-string-start (make-string-start))
(define the-string-end   (make-string-end))

(define (string-start)
  the-string-start)

(define (string-end)
  the-string-end)

;----------------
; Expression types.
;
; There are four records for expressions: submatches, sequences, one-ofs,
; and repeats.  The first and last have a single subexpression, the middle
; two each have a list.
;
; Each record contains a slot for caching a POSIX regexp object.  This is
; set only if that record is the entire expression being matched.

; A submatch is just a marker wrapped around another expression, with a tag
; used to identify the matched substring.

(define-regexp-type submatch make-submatch submatch?
  submatch-exp
  submatch-id)

(define (submatch id exp)
  (cond ((not (regexp? exp))
	 (call-error "invalid argument" submatch exp))
	((empty-set? exp)
	 the-empty-set)
	(else
	 (make-submatch exp id))))

; Sequences.

(define-regexp-type sequence make-sequence sequence?
  sequence-exps)

; Epsilon is encoded as an empty sequence.  

(define epsilon (make-sequence '()))

(define (epsilon? x)
  (and (sequence? x)
       (null? (sequence-exps x))))

; We splice out any subordinate sequences.  An empty-set means that the sequence
; itself can never match and is thus the empty-set as well.
;
; We can't use REDUCE because we add on to the EXPS list.

(define (sequence . all-exps)
  (let loop ((exps all-exps) (res '()))
    (if (null? exps)
	(cond ((null? res)
	       epsilon)
	      ((null? (cdr res))
	       (car res))
	      (else
	       (make-sequence (reverse res))))
	(let ((exp (car exps)))
	  (cond ((not (regexp? exp))
		 (apply call-error "invalid argument" sequence all-exps))
		((empty-set? exp)
		 the-empty-set)
		((sequence? exp)
		 (loop (append (sequence-exps exp) (cdr exps)) res))
		(else
		 (loop (cdr exps) (cons exp res))))))))

; one-ofs

(define-regexp-type one-of make-one-of one-of?
  one-of-exps)

; We splice out any subordinate one-ofs and merge all sets into a single set.
; An empty one-of is the empty set.  Any subordinate empty-sets can be dropped.

(define (one-of . all-exps)
  (let loop ((exps all-exps) (res '()) (set the-empty-set))
    (if (null? exps)
	(cond ((null? res)
	       set)
	      ((and (null? (cdr res))
		    (empty-set? set))
	       (car res))
	      (else
	       (make-one-of (if (empty-set? set)
				(reverse res)
				(cons set (reverse res))))))
	(let ((exp (car exps)))
	  (cond ((not (regexp? exp))
		 (apply call-error "invalid argument" sequence all-exps))
		((empty-set? exp)
		 (loop (cdr exps) res set))
		((set? exp)
		 (loop (cdr exps) res (union exp set)))
		((one-of? exp)
		 (loop (append (one-of-exps exp) (cdr exps)) res))
		(else
		 (loop (cdr exps) (cons exp res) set)))))))

; (TEXT <string>) matches the <string>.

(define (text string)
  (if (string? string)
      (apply sequence (map char->set (string->list string)))
      (call-error "invalid argument" text string)))

; Repetitions
;  LOW is an integer >= 0.
;  HIGH is either an integer >= LOW or #F, indicating that there is no limit.
;  EXP is the expression to be repeated.

(define-regexp-type repeat really-make-repeat repeat?
  repeat-low
  repeat-high
  repeat-exp)

; Optimizations:
;  - if both high and low are 0 this is the empty set
;  - if both high and low are 1 this is just the expression
;  - the empty set is epsilon if zero repetitions are allowed, or itself
;    if at least one repetition is required.

(define (make-repeat low high exp)
  (cond ((not (and (integer? low)
		   (<= 0 low)
		   (regexp? exp)
		   (or (not high)
		       (and (integer? high)
			    (<= low high)))))
	 (call-error "invalid argument" make-repeat low high exp))
	((or (epsilon? exp)
	     (and high
		  (= low high 0)))
	 epsilon)
	((and high
	      (= low high 1))
	 exp)
	((empty-set? exp)
	 (if (and (= low 0)
		  (or (not high)
		      (= high 0)))
	     epsilon
	     the-empty-set))
	(else
	 (really-make-repeat low high exp))))

; One argument is an expression that can appear any number of times.
; Two arguments are LOW and an expression.
; Three arguments are LOW, HIGH, and an expression.

(define (repeat . stuff)
  (case (length stuff)
    ((0)
     (call-error "invalid argument" repeat))
    ((1)
     (make-repeat 0 #f (car stuff)))
    ((2)
     (make-repeat (car stuff) (car stuff) (cadr stuff)))
    ((3)
     (apply make-repeat stuff))
    (else
     (apply call-error "invalid argument" repeat stuff))))

;----------------
; Three functions that transform EXP instead of having their own record type.
; They are idempotent: (eq? (f x) (f (f x))) => #t.

(define (ignore-case exp)
  (start-expression-map ignore-case 'no-case exp))

(define (use-case exp)
  (start-expression-map use-case 'use-case exp))

(define (no-submatches exp)
  (start-expression-map no-submatches 'no-submatch exp))

; FUNCTION is one of:
;  no-case        - replace use-case with no-case in all sets
;  use-case       - replace no-case with use-case in all sets
;  no-submatch    - remove all submatches
; This reuses as much of EXP as possible.

(define (start-expression-map proc function exp)
  (if (regexp? exp)
      (or (expression-map function exp)
	  exp)
      (call-error "invalid argument" proc exp)))

; This returns #F if FUNCTION does not modify EXP.

(define (expression-map function exp)
  (let recur ((exp exp))
    (cond ((set? exp)
	   (if (or (eq? function 'no-submatch)
		   (= (set-use-case exp)
		      (set-no-case exp)))
	       #f
	       (let ((chars (if (eq? function 'no-case)
				(set-no-case exp)
				(set-use-case exp))))
		 (make-set chars chars))))
	  ((submatch? exp)
	   (let ((sub (submatch-exp exp)))
	     (if (eq? function 'no-submatch)
		 (or (recur sub)
		     sub)
		 (let ((new (recur sub)))
		   (if new
		       (submatch (submatch-id exp) new)
		       #f)))))
	  ((sequence? exp)
	   (let ((exps (expression-list-map function (sequence-exps exp))))
	     (if exps
		 (make-sequence exps)
		 #f)))
	  ((one-of? exp)
	   (let ((exps (expression-list-map function (one-of-exps exp))))
	     (if exps
		 (make-one-of exps)
		 #f)))
	  ((repeat? exp)
	   (let ((new (recur (repeat-exp exp))))
	     (if new
		 (make-repeat (repeat-low exp)
			      (repeat-high exp)
			      new)
		 #f)))
	  ((or (string-start? exp)
	       (string-end? exp))
	   exp)
	  (else
	   (error "expression-map got a non-expression" exp)))))

; As above, this returns #F if FUNCTION leaves EXPS unchanged.

(define (expression-list-map function exps)
  (let recur ((exps exps))
    (if (null? exps)
	#f
	(let ((new (expression-map function (car exps)))
	      (more (recur (cdr exps))))
	  (if (or new more)
	      (cons (or new (car exps))
		    (or more (cdr exps)))
	      #f)))))

;----------------
; Translation to a POSIX regexp string.
;
; We get the string from EXP->STRINGS as a list of substrings to concatenate
; together.  In this list (<id>) represents a #\( that begins a submatch with
; the given id and #F represents a #\( that does not begin a submatch.  We walk
; down the list replacing these with #\( and collecting the ids and #f's into a
; list.  When matching this list will be used to discard the match records that
; correspond to parens used for grouping while retaining those that correspond
; to submatches.
;
; This used to signal an error if a beginning-of-line or end-of-line occured
; in an unmatchable position.  This is a questionable notion and the checking
; was not completely accurate, so I removed it.

(define (exp->posix-string exp)
  (cond ((not (regexp? exp))
	 (call-error "invalid argument" exp->posix-string exp))
	((empty-set? exp)
	 (call-error "no Posix string for the empty set" exp->posix-string exp))
	(else
	 (reduce ((list* elt (exp->strings exp '())))
		 ((strings '())
		  (parens '()))
	   (cond ((not elt)
		  (values (cons "(" strings) (cons #f parens)))
		 ((pair? elt)
		  (values (cons "(" strings) (cons elt parens)))
		 (else
		  (values (cons elt strings) parens)))
	   (values (apply string-append (reverse strings))
		   (reverse parens))))))

; submatch -> (...), no need to parenthesize the subexp
; one-of -> ... | ... | ..., no need to parenthesize the subexps
; seq -> concatenation, need to parenthesize one-ofs
; repeat -> ...{x,y}, parenthesize sequences and one-ofs
;
; TAIL is a list of strings to come after those for EXP.

(define (exp->strings exp tail)
  (cond ((set? exp)
	 (cons (set->posix-string exp) tail))
	((submatch? exp)
	 `((,(submatch-id exp))
	   ,@(exp->strings (submatch-exp exp) '())
	   ")"
	   . ,tail))
	((one-of? exp)
	 (one-of-strings (one-of-exps exp) tail))
	((sequence? exp)
	 (sequence-strings (sequence-exps exp) tail))
	((repeat? exp)
	 (repetition-strings (repeat-low exp)
			     (repeat-high exp)
			     (repeat-exp exp)
			     tail))
	((string-start? exp)
	 (cons "^" tail))
	((string-end? exp)
	 (cons "$" tail))
	(else
	 (error "bad expression" exp))))

; Add parentheses around the strings for EXP, encoding "(" as #F because it
; does not begin a submatch.

(define (exp->parenthesized-strings exp tail)
  (cons #f (exp->strings exp (cons ")" tail))))

; Convert EXPS to strings, adding "|" between them.  This depends on there
; being at least two EXPS.

(define (one-of-strings exps tail)
  (let ((exps (reverse exps)))
    (reduce ((list* exp (cdr exps)))
	    ((res (exp->strings (car exps) tail)))
      (exp->strings exp (cons "|" res)))))

; Convert EXPS to strings, adding parentheses for any that are ONE-OFs.

(define (sequence-strings exps tail)
  (if (null? exps)
      `(#f ")" . ,tail)
      (reduce ((list* exp (reverse exps)))
	      ((strings tail))
	(if (one-of? exp)
	    (exp->parenthesized-strings exp strings)
	    (exp->strings exp strings)))))

; e{x,}    X or more Es
; e{x}     exactly X Es
; e{x,y}   between X and Y Es, inclusive

(define (repetition-strings low high subexp tail)
  (let ((tail `("{"
		,(number->string low)
		,@(cond ((not high)
			 (list ","))
			((= low high)
			 '())
			(else
			 (list "," (number->string high))))
		"}"
		. ,tail)))
    (if (or (one-of? subexp)
	    (sequence? subexp))
	(exp->parenthesized-strings subexp tail)
	(exp->strings subexp tail))))

;----------------
; Converting a set into POSIX regexp [...] sets.  We have to avoid various
; pitfalls of the notation, which does not use a simple escape mechanism.
; The operator characters lose their meaning when in particular positions:
;   ^ when first indicates negation, anywhere else it's normal
;   ] is normal when first or second after ^
;   - is normal as the first or last character
;   [ is normal except when followed by any one of three characters: .=:
;
; 1. If it is everything, use `.'
; 2. If it a singleton just use the character, escaped if necessary.
; 3. If the set is -^ then use "[-^]".
; 4. If - is in the set and not inside a range, then put it last.
; 5. If ^ is "                                , then put it before ^^^.
; 6. If [ is "                                , then put it before ^^^.
; 7. If ] is "                                , then put it first.
;
; The code would be simpler if we didn't preserve ranges across funny
; characters.
;
; The computation is expensive enough that we cache the result.  This is a
; win for the named sets (alphabetic etc.) and will also help when people use
; subexpressions multiple times.

(define (set->posix-string set)
  (or (set-string set)
      (let ((string (bit-set->posix-string (set-use-case set))))
	(set-set-string! set string)
	string)))

; Pick off empty sets and full sets, then negate if necessary and build the
; string.  MAYBE-BIT-SET->STRING returns a character if the set is a singleton.

(define (bit-set->posix-string bit-set)
  (cond ((= bit-set 0)
	 (error "trying to convert the empty set"))
	((= (bitwise-and bit-set all-chars)
	    all-chars)
	 ".")
	(else
	 (let* ((string (maybe-bit-set->string bit-set)))
	   (if (string? string)
	       (string-append "["
			      (if (char? string)
				  (list->string (list string))
				  string)
			      "]")
	       (char->posix-string string))))))

; The mask with all ones.

(define all-chars (- (arithmetic-shift 1 char-limit) 1))

; The general rule does not work for "[-^]" (it would come out as "[^-]").

(define dash-hat (bitwise-ior (char->mask #\-)
			      (char->mask #\^)))

; This is a list of three element lists:
;  - a bitmask for a character that has special meaning
;  - the character itself
;  - a mask with the character and two or three adjacent characters
; If the character is present but not all of the bigger mask, then the character
; will not be inside a range and so must be removed and placed at the end of the
; string.  This is complicated by #\] and #\^ being adjacent ASCII characters.
; If one is present then the four characters "\]^_" must be present to make sure
; the funny character is inside a range.
;
; The bitmask comes first so that we can use ASSOC to search the list.

(define funny-char-list
  (map (lambda (char)
	 (let ((mask (char->mask char)))
	   (list mask
		 char
		 (bitwise-ior mask
			      (arithmetic-shift mask -1)
			      (arithmetic-shift mask 1)
			      (cond ((eq? char #\^)
				     (char->mask #\\))
				    ((eq? char #\])
				     (char->mask #\_))
				    (else
				     0))))))
       '(#\[ #\^ #\- #\])))

; Pick off "-^" and any funny characters on their own.  Otherwise we remove
; any funny characters that will not be internal to a range and then add them
; back in at the end.  The order of the funny character data makes it so that
; any subset of them is an unambiguous end for the set (except for #\] which
; goes at the beginning).

(define (maybe-bit-set->string bit-set)
  (cond ((= bit-set dash-hat)
	 "-^")
	((assoc bit-set funny-char-list)
	 => cadr)
	(else
	 (reduce ((list* info funny-char-list))
		 ((bit-set bit-set)
		  (funny-chars '()))
	   (if (or (= 0
		      (bitwise-and bit-set
				   (car info)))
		   (= (caddr info)
		      (bitwise-and bit-set
				   (caddr info))))
	       (values bit-set
		       funny-chars)
	       (values (bitwise-xor bit-set (car info))
		       (cons (cadr info) funny-chars)))
	   (let ((chars (bit-set->chars bit-set)))
	     (char-list->string (if (and (not (null? funny-chars))
					 (eq? (car funny-chars) #\]))
				    (append (cdr funny-chars)
					    (reverse (cons #\] chars)))
				    (append funny-chars
					    (reverse chars)))))))))

; Returns a list of pairs (char0 . char1), each pair representing an inclusive
; range of characters in the bitset.  We first make a list of possibly adjacent
; ranges and then merge the adjacent ones.  Believe it or not, the code is
; much simpler this way.

(define (bit-set->chars bit-set)
  (reduce ((bits* chunk bit-set 16)
	   (count* i 0 -1 16))
	  ((ranges '()))
    (if (= 0 chunk)
	ranges
        (small-bit-set->ranges chunk i ranges))
    (reduce ((list* range (cdr ranges)))
	    ((done (list (car ranges))))
      (if (= (+ (cdr range) 1)
	     (caar done))
	  (cons (cons (car range) (cdar done))
		(cdr done))
	  (cons range done)))))

; Here we loop through the bits creating ranges.  REDUCE doesn't work because
; we need both the next bit and the remaining bit-set.

(define (small-bit-set->ranges bit-set i result)
  (let loop ((bit-set bit-set) (i i))
    (cond ((= 0 bit-set)
	   result)
	  ((odd? bit-set)
	   (let range-loop ((bit-set (shift-down bit-set)) (j (+ i 1)))
	     (if (odd? bit-set)
		 (range-loop (shift-down bit-set) (+ j 1))
		 (small-bit-set->ranges (shift-down bit-set)
					(+ j 1) 
					(cons (cons i (- j 1))
					      result)))))
	  (else
	   (loop (shift-down bit-set) (+ i 1))))))

(define (shift-down n)
  (arithmetic-shift n -1))

; Turn RANGES, which is list of pairs and (<ascii start> . <ascii end>) ranges
; into a string, where the ranges become <start char>-<end char>.  Any
; characters in RANGES are put directly into the result.

(define (char-list->string ranges)
  (if (and (null? (cdr ranges))
	   (= (caar ranges)
	      (cdar ranges)))
      (reduce ((list* range ranges))
	      ((res '()))
	(if (char? range)
	    (cons range res)
	    (let ((first (ascii->char (car range)))
		  (last (ascii->char (cdr range))))
	      (case (- (cdr range) (car range))
		((0)
		 (cons first res))
		((1)
		 (cons first (cons last res)))
		(else
		 `(,first #\- ,last . ,res)))))
	(list->string res))))

; These are the characters that need to be escaped when appearing in an
; expression (but not necessarily when in a character set).

(define special-char-set
  (set-use-case (set "[.*?()|\\$^+")))

; Add a \ in front of CHAR if it is an operator.
;
; To avoid sequences like "{3", which look like parts of ranges, we always put
; #\{ characters in ranges.

(define (char->posix-string char)
  (cond ((char=? char #\{)
	 "[{]")
	((= 0 (bitwise-and (char->mask char)
			   special-char-set))
	 (string char))
	(else
	 (string #\\ char))))

;----------------
; Matching
;
; A compiled regexp is actually a pair of a POSIX regexp object and the list
; of submatch indexes.  

(define (any-match? exp string)
  (if (and (regexp? exp)
	   (string? string))
      (if (empty-set? exp)
	  #f
	  (regexp-match (car (compile-exp exp)) string #f #t #t))
      (call-error "invalid argument" any-match? exp string)))
  
(define (exact-match? exp string)
  (if (and (regexp? exp)
	   (string? string))
      (if (empty-set? exp)
	  #f
	  (let ((matches (regexp-match (car (compile-exp exp)) string #t #t #t)))
	    (and matches
		 (= 0 (match-start (car matches)))
		 (= (string-length string) (match-end (car matches))))))
      (call-error "invalid argument" exact-match? exp string)))

; Do the match and select out the match records that correspond to submatches,
; making them into an alist.  The first match record, which is for the entire
; expression is remade with the alist as an additional field.

(define (match exp string)
  (if (and (regexp? exp)
	   (string? string))
      (let* ((pair (compile-exp exp))
	     (regexp (car pair))
	     (match-flags (cdr pair))
	     (matches (regexp-match regexp string #t #t #t)))
	(if matches
	    (reduce ((list% match (cdr matches))
		     (list% flag match-flags))
		    ((submatches '()))
	      (if (and flag match)
		  (cons (cons (car flag)
			      match)
			submatches)
		  submatches)
	      (make-match (match-start (car matches))
			  (match-end (car matches))
			  (reverse submatches)))
	    #f))
      (call-error "invalid argument" match exp string)))

; Compile the expression if this hasn't already been done.  The compiled version
; is the POSIX regexp object and the list of submatch indexes.

(define (compile-exp exp)
  (or (regexp-compiled exp)
      (mvlet* (((string match-flags)
		(exp->posix-string exp))
	       (regexp (if (null? match-flags)
			   (make-posix-regexp string (regexp-option extended))
			   (make-posix-regexp string
					      (regexp-option extended)
					      (regexp-option submatches)))))
	(set-regexp-compiled! exp (cons regexp match-flags))
	(cons regexp match-flags))))

;----------------
; A handy debugging function.  In theory this returns as S-expression that
; when EVAL'ed would give back the original expression.  It does not produce
; a particularly compact s-expression.

(define (regexp->s-exp x)
  (cond ((not (regexp? x))
	 (call-error "invalid argument" regexp->s-exp x))	 
	((set? x)
	 (list 'set
	       (let ((chars (set->chars x)))
		 (if (= 1 (length chars))
		     (car chars)
		     (list->string chars)))))
	((submatch? x)
	 (list 'submatch (regexp->s-exp (submatch-exp x))))
	((sequence? x)
	 (cons 'sequence (map regexp->s-exp (sequence-exps x))))
	((one-of? x)
	 (cons 'one-of (map regexp->s-exp (one-of-exps x))))
	((repeat? x)
	 (list 'repeat
	       (repeat-low x)
	       (repeat-high x)
	       (regexp->s-exp (repeat-exp x))))
	((string-start? x)
	 '(string-start))
	((string-end? x)
	 '(string-end))
	(else
	 (error "unknown type of regular-expression" x))))

; Used by EXP->S-EXP.  Returns a list of the characters in SET (using the
; case-sensitive set).

(define (set->chars set)
  (iterate loop ((count* i 0 -1 16))
	        ((bits (set-use-case set))
		 (chars '()))
    (if (= 0 bits)
	(reverse chars)
	(loop (arithmetic-shift bits -16)
	      (iterate loop ((count* i i))
		            ((bits (bitwise-and bits #xffff))
			     (chars chars))
		(if (= 0 bits)
		    chars
		    (loop (arithmetic-shift bits -1)
			  (if (odd? bits)
			      (cons (ascii->char i)
				    chars)
			      chars))))))))

