; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Interfaces
;
; An interface has four fields:
;   - A procedure for looking up names in the interface.  The procedure
;     returns a base name and a type.  If the name is not exported the
;     base name is #F.
;   - A procedure for walking over the declarations in the interfaces.
;     The name, base-name, and type of each exported name are passed
;     to the action procedure.
;   - A population containing the structures that export this interface and
;     any compound or modified interfaces that build on it.  This population
;     is used for propogating changes when an interface or structure is
;     redefined.
;   - A name for debugging.

(define-record-type interface :interface
  (really-make-interface ref walk clients name)
  interface?
  (ref ref-method)
  (walk walk-method)
  (clients interface-clients)
  (name interface-name set-interface-name!))

(define-record-discloser :interface
  (lambda (int)
    (list 'interface (interface-name int))))

(define (make-interface ref walk name)
  (really-make-interface ref
			 walk
			 (make-population)
			 name))

; The generic lookup function, and a simplified version for use when the
; base name and type are not needed.

(define (interface-ref int name)
  ((ref-method int) name))

(define (interface-member? int name)
  (mvlet (((base-name type)
	     (interface-ref int name)))
    base-name))

; The generic walk function.

(define (for-each-declaration proc int)
  ((walk-method int) proc))

; Adding to the client population.

(define (note-reference-to-interface! int thing)
  (let ((pop (interface-clients int)))
    (if pop
	(add-to-population! thing pop))))

; Adding a late name.

(define (note-interface-name! int name)
  (if (and name (not (interface-name int)))
      (set-interface-name! int name)))

;----------------
; Simple interfaces.  ITEMS is a list of items of the form:
;  - <name>			; use the default type
;  - (<name> <type>)		; use <type>
;  - ((<name> ...) <type>)	; use <type> for each <name>
;
; We make a table of the names and use it appropriately.

(define (make-simple-interface name items)
  (let ((table (make-simple-interface-table items)))
    (make-interface (lambda (name)
		      (let ((type (table-ref table name)))
			(if type
			    (values name type)
			    (values #f #f))))
		    (lambda (proc)
		      (table-walk (lambda (name type)
				    (proc name name type))
				  table))
		    name)))

(define (make-simple-interface-table items)
  (let ((table (make-symbol-table)))
    (for-each (lambda (item)
		(if (pair? item)
		    (let ((name (car item))
			  (type (cadr item)))
		      (if (or (null? name)
			      (pair? name))
			  (for-each (lambda (name)
				      (table-set! table name type))
				    name)
			  (table-set! table name type)))
		    (table-set! table item undeclared-type)))
	      items)
    (make-table-immutable! table)
    table))

;----------------
; Compound interfaces
;
; A compound interface is the union of a set of existing interfaces.
; To do lookups or walks we walk down the list of included interfaces.

(define (make-compound-interface name . ints)
  (let ((int (make-interface (lambda (name)
			       (let loop ((ints ints))
				 (if (null? ints)
				     (values #f #f)
				     (mvlet (((new-name type)
					      (interface-ref (car ints) name)))
					    (if new-name
						(values new-name type)
						(loop (cdr ints)))))))
			     (lambda (proc)
			       (for-each (lambda (int)
					   (for-each-declaration proc int))
					 ints))
			     name)))
    (for-each (lambda (i)
		(note-reference-to-interface! i int))
	      ints)
    int))

;----------------
; Modified interfaces.
;
; We return a new interface that is INTERFACE modified by COMMANDS.  Commands
; are:
;  (prefix <symbol>)
;      Add <symbol> to the beginning of every name in INTERFACE.
;  (expose <symbol> ...)
;      Export only those names in INTERFACE that are listed.
;  (hide <symbol> ...)
;      Do not export any of the names listed.
;  (alias (<old> <new>) ...)
;      Make name <old> also visible as <new>.
;  (rename (<old> <new>) ...)
;      Make name <old> visible as <new> but not as <old>.
; The commands are interpreted last-to-first.  Thus
;  ((expose foo:bar) (prefix foo:))
; and
;  ((prefix foo:) (expose bar))
; both make BAR visible as FOO:BAR but
;  ((expose bar) (prefix foo:))
; does not allow any names to be seen.

(define (make-modified-interface interface commands)
  (if (and (proper-list? commands)
	   (every okay-command? commands))
      (mvlet (((alist hidden default)
	         (process-commands commands)))
	(let ((lookup (make-lookup alist hidden default interface))
	      (walker (if default
			  (make-default-walker alist hidden default interface)
			  (make-alist-walker alist interface))))
	  (let ((int (make-interface lookup walker #f)))
	    (note-reference-to-interface! interface int)
	    int)))
      (error "badly-formed structure modifiers" commands)))

; We process COMMANDS and compute three values:
;   - an alist mapping visible names to their real names in the package
;   - a list of names that are hidden (these may also appear in the alist;
;     the hiding overrides the alist).      
;   - a default, which applies to all other names:
;         = #f, there are no other visible names
;         = #t, all other names are visible
;         = <symbol>, names beginning with this prefix are visible
;
; We just loop over the commands, dispatching on the type of command.

(define (process-commands commands)
  (let loop ((alist '())
	     (hidden '())
	     (default #t)
	     (commands (reverse commands)))
    (if (null? commands)
	(values (filter (lambda (pair)
			  (not (memq (car pair) hidden)))
			alist)
		hidden
		default)
	(mvlet (((alist hidden default)
		   (let ((proc (case (caar commands)
				 ((prefix) process-prefix)
				 ((expose) process-expose)
				 ((hide)   process-hide)
				 ((alias)  process-alias)
				 ((rename) process-rename))))
		     (proc (cdar commands) alist hidden default))))
	  (loop alist hidden default (cdr commands))))))

; Checks that COMMAND is properly formed.

(define (okay-command? command)
  (and (proper-list? command)
       (pair? command)
       (symbol? (car command))
       (pair? (cdr command))
       (let ((args (cdr command)))
	 (case (car command)
	   ((prefix)
	    (and (symbol? (car args))
		 (null? (cdr args))))
	   ((expose hide)
	    (every symbol? args))
	   ((alias rename)
	    (every (lambda (spec)
		     (and (proper-list? spec)
			  (= 2 (length spec))
			  (symbol? (car spec))
			  (symbol? (cadr spec))))
		   args))
	   (else
	    #f)))))

; Checks that L is a proper list.

(define (proper-list? l)
  (cond ((null? l)
	 #t)
	((pair? l)
	 (proper-list? (cdr l)))
	(else
	 #f)))

; We add the prefix to the names in ALIST and HIDDEN.  If DEFAULT is already
; a prefix we add this one to it, otherwise the prefix is the new default.

(define (process-prefix args alist hidden default)	     
  (let ((prefix (car args)))
    (values (map (lambda (pair)
		   (cons (symbol-append prefix (car pair))
			 (cdr pair)))
		 alist)
	    (map (lambda (name)
		   (symbol-append prefix name))
		 hidden)
	    (cond ((symbol? default)
		   (symbol-append default prefix))
		  ((not default)
		   #f)
		  (else
		   prefix)))))

; We make a new ALIST with the exposed names and with package names are looked
; up in the current state.  Then we start again with no hidden names and no
; default.

(define (process-expose args alist hidden default)	     
  (values (let loop ((args args) (new-alist '()))
	    (if (null? args)
		(reverse new-alist)
		(let* ((name (car args))
		       (pname (interface-lookup name alist hidden default)))
		  (loop (cdr args)
			(if pname
			    (cons (cons name pname)
				  new-alist)
			    new-alist)))))
	  '()		    
	  #f))

; Just add the names to the hidden list.

(define (process-hide args alist hidden default)
  (values alist
	  (append args hidden)
	  default))

; Add the new aliases to ALIST.

(define (process-alias args alist hidden default)
  (values (append (map (lambda (spec)
			 (cons (cadr spec)
			       (car spec)))
		       args)
		  alist)
	  hidden
	  default))

; Add the new aliases to ALIST and add the old names to HIDDEN.

(define (process-rename args alist hidden default)
  (values (append (map (lambda (spec)
			 (cons (cadr spec)
			       (car spec)))
		       args)
		  alist)
	  (append (map car args) hidden)
	  default))

;----------------
; Look up a name, returning the name by which it is known in the base structure.
;   - If it is in HIDDEN then it is not exported.
;   - If there is an alias, then return the alias.
;   - If there is no default the name is not exported.
;   - A default of #T means every name is passed through.
;   - Otherwise, check that NAME begins with the default and return the
;     suffix after the default.

(define (interface-lookup name alist hidden default)
  (cond ((memq name hidden)
	 #f)
	((assq name alist)
	 => cdr)
	((not default)
	 #f)
	((eq? default #t)
	 name)
	((prefix-match? (symbol->string name)
			(symbol->string default))
	 (remove-prefix (symbol->string name)
			(symbol->string default)))
	(else
	 #f)))

; Curried version of MAKE-LOOKUP for making the INTERFACE-REF method for
; modified structures.

(define (make-lookup alist hidden default interface)
  (lambda (name)
    (let ((alias (interface-lookup name alist hidden default)))
      (if alias
	  (interface-ref interface alias)
	  (values #f #f)))))

; True if NAME begins with PREFIX (and is not just PREFIX).

(define (prefix-match? name prefix)
  (and (< (string-length prefix)
	  (string-length name))
       (let loop ((i 0))
	 (cond ((= i (string-length prefix))
		#t)
	       ((char=? (string-ref name i)
			(string-ref prefix i))
		(loop (+ i 1)))
	       (else
		#f)))))

; Return the portion of NAME that follows PREFIX.

(define (remove-prefix name prefix)
  (string->symbol (substring name
			     (string-length prefix)
			     (string-length name))))

;----------------
; Return a procedure for walking over the declarations in a modified interface.
; There are two versions, depending on whether names are passed on by default.
; If there is a default we need to walk over the declarations in the base
; interface and pass on the ones that are not hidden.

(define (make-default-walker alist hidden default interface)
  (lambda (proc)
    (for-each-declaration
      (lambda (name base-name type)
	(if (not (memq name hidden))
	    (proc (cond ((cdr-assq name alist)
			 => car)
			((symbol? default)
			 (symbol-append default name))
			(else
			 name))
		  base-name
		  type)))
      interface)))

; Same as ASSQ except we look for THING as the cdr instead of the car.

(define (cdr-assq thing alist)
  (let loop ((alist alist))
    (cond ((null? alist)
	   #f)
	  ((eq? thing (cdar alist))
	   (car alist))
	  (else
	   (loop (cdr alist))))))

; With no default, all of the names are in the ALIST and we do not need to
; walk over the declarations in the base interface.

(define (make-alist-walker alist interface)
  (lambda (proc)
    (for-each (lambda (pair)
		(mvlet (((base-name type)
			  (interface-ref interface (cdr pair))))
		  (if base-name
		      (proc (car pair)
			    base-name
			    type))))
	      alist)))
	 
;----------------
; Random utility.
			  
(define (symbol-append a b)
  (string->symbol (string-append (symbol->string a)
				 (symbol->string b))))

