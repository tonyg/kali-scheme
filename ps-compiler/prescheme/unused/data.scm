; Copyright (c) 1994 by Richard Kelsey.  See file COPYING.


; Records & (tagged) Unions
;
; Can we arrange to pass records as a separate type from the including union?
; Probably not.  This introduces type relations that have to be satisified.
; No coercions are needed so it would be much simpler than before.  Unification
; would have to find meets.  I guess that it would be doable, but not for now.
;
; The only records that have tags are those that are part of a multi-record
; union.  If a record is not part of a union it does not need a tag field.
;
; The simplest thing is probably to only have tagged unions, with stand-alone
; records being a degenerate case (to be added later).

; Data types

; Representation of union types.

(define-record-type union-type
  (name       ; name of the data type
   variants   ; what this can be
   )
  ())

(define-record-discloser type/union-type
  (lambda (union)
    (list 'union-type (union-type-name dtype))))

; Record variants of unions

(define-record-type record-type
  (index                ; index of this record in parent union
   predicate            ; name of the predicate   ; only at top-level
   constructor          ; name of the constructor
   )
  (fields               ; filled in later because of circularity
   constructor-args))   ; fields passed to the constructor 

(define-record-discloser type/record-type
  (lambda (rtype)
    (list 'record-type (record-type-constructor rtype))))

; Fields of record types.

(define-record-type record-field
  (record-type
   name
   type
   )
  ())

; Global table of record types.  Since we compile to a single C file the
; record types used within a single computation must have distinct names.
; (This should really be a fluid.)
; Or we could make up names as necessary.

(define *record-type-table* (make-symbol-table))

(define (reset-record-data!)
  (set! *record-type-table* (make-symbol-table)))

(define (get-record-type id)
  (cond ((table-ref *record-type-table* id)
	 => identity)
	(else
	 (error "no record type ~S" id))))

(define (all-record-types)
  (table->entry-list *record-type-table*))

(define (lookup-union-type id)
  (table-ref *record-type-table* id))

; Construction a record type.  This gets the name, the list of fields whose
; initial values are passed to the constructor, and the field specifications.
; Each field specification consists of a name and a type.

(define (make-record-type id constructor-args specs)
  (let ((rt (record-type-maker id)))
    (if (table-ref *record-type-table* id)
	(user-error "multiple definitions of record type ~S" id))
    (table-set! *record-type-table* id rt)
    (set-record-type-fields! rt (map (lambda (spec)
				       (record-field-maker
					  rt
					  (car spec)
					  (expand-type-spec (cadr spec))))
				     specs))
    (set-record-type-constructor-args! rt
				       (map (lambda (name)
					      (get-record-type-field id name))
					    constructor-args))
    rt))

; Return the field record for FIELD-ID in record-type TYPE-ID.

(define (get-record-type-field type-id field-id)
  (let ((rtype (get-record-type type-id)))
    (cond ((any (lambda (field)
		  (eq? field-id (record-field-name field)))
		(record-type-fields rtype))
	   => identity)
	  (else
	   (user-error "~S is not a field of ~S" field-id rtype)))))

; The macro expander for DEFINE-RECORD-TYPE.
;
; (define-record-type <id> <type-id>
;   (<constructor> . <field-names>)
;   (<field-name> <type> <accessor-name> [<modifier-name>]) ...)
; 
; The <type-id> is used only by Pre-Scheme-in-Scheme.

(define (expand-define-record-type exp r c)
  (let ((id (cadr exp))
	(maker (cadddr exp))
	(fields (cddddr exp)))
    (let ((rt (make-record-type id (cdr maker) fields)))
      `(,(r 'begin)
	(,(r 'define) ,maker
	   (,(r 'let) ((,(r id) (,(r 'make-record) ',id)))
	     (,(r 'if) (,(r 'not) (,(r 'null-pointer?) ,(r id)))
	       (,(r 'begin)
		 . ,(map (lambda (name)
			   `(,(r 'record-set!) ,(r id) ,name ',id ',name))
			 (cdr maker))))
	     ,(r id)))
	,@(map (lambda (field)
		 `(,(r 'define) (,(caddr field) ,(r id))
				(,(r 'record-ref) ,(r id) ',id ',(car field))))
	       fields)
	,@(map (lambda (field)
		 `(,(r 'define) (,(cadddr field) ,(r id) ,(r 'x))
			(,(r 'record-set!)
			 ,(r id) ,(r 'x)',id ',(car field))))
	       (filter (lambda (spec)
			 (not (null? (cdddr spec))))
		       fields))))))


; In C.
;
; All unions are cast to a single type.  All records that are part of unions
; have an initial tag field.
;
;struct datatypeX { int tagX; };
;
;#define NULL_TAGX 0
;#define PAIR_TAGX 1
;
;struct pair {
;  int tagX;
;  long car;
;  struct datatypeX *cdr;
;};
;
;struct datatypeX nullX = { NULL_TAGX }, *null = &nullX;
;
;struct datatypeX *cons(unsigned char, struct datatypeX*);
;struct datatypeX *reverseB(struct datatypeX*);
;char memberP(struct datatypeX*, long);
;void init(void);
;
;struct datatypeX *cons(unsigned char car_0X, struct datatypeX *cdr_1X)
;{
;  struct pair *pair_2X;
; {pair_2X = (struct pair *)malloc(sizeof(struct pair));
;  if ((NULL == pair_2X)) {
;    return (struct datatypeX *)pair_2X;}
;  else {
;    pair_2X->tagX = PAIR_TAGX;
;    pair_2X->car = (((unsigned long) car_0X));
;    pair_2X->cdr = cdr_1X;
;    return (struct datatypeX *)pair_2X;}}}
;
;char memberP(struct datatypeX *list_7X, long x_8X)
;{
;  struct datatypeX *arg0K0;
;  struct datatypeX *list_9X;
; {arg0K0 = list_7X;
;  goto L169;}
; L169: {
;  list_9X = arg0K0;
;  if ((list_9X->tagX == NULL_TAGX)) {
;    return 0;}
;  else {
;    if ((x_8X == ((struct pair *)list_9X)->car)) {
;      return 1;}
;    else {
;      arg0K0 = ((struct pair *)list_9X)->cdr;
;      goto L169;}}}}
