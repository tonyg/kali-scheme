; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Compile-time environments
; These are functions
;  name -> node			; lexical variable
;          binding              ; package variable, any syntax
;          #f			; free
;
; Special names are used to retrieve various values from environments.

(define (lookup cenv name)
  (cenv name))

(define (bind1 name binding cenv)
  (lambda (a-name)
    (if (eq? a-name name)
	binding
	(lookup cenv a-name))))

(define (bind names bindings cenv)
  (cond ((null? names) cenv)
	(else
	 (bind1 (car names)
		(car bindings)
		(bind (cdr names) (cdr bindings) cenv)))))

(define (bindrec names cenv->bindings cenv)
  (set! cenv (bind names
		   (cenv->bindings (lambda (a-name) (cenv a-name)))
		   cenv))
  cenv)

; Making the initial environment.
;
;  lookup : name -> binding or (binding . path) or #f
;  define : name type [static] -> void
;  macro-eval : form -> delay that returns (<eval> . <env>) for evaluating
;   macro expanders

(define (make-compiler-env lookup define! macro-eval package)
  (lambda (name)
    (cond ((eq? name funny-name/macro-eval)
	   macro-eval)
	  ((eq? name funny-name/define!)
	   define!)
	  ((eq? name funny-name/source-file-name)
	   #f)
	  ((eq? name funny-name/package)
	   package)
	  (else
	   (lookup name)))))

; EVAL function for evaluating macro expanders.

(define funny-name/macro-eval (string->symbol "Eval function for macros"))

(define (environment-macro-eval cenv)
  (cenv funny-name/macro-eval))

; Function for adding definitions to the outer package.

(define funny-name/define! (string->symbol "Definition function"))

(define (environment-define! cenv name type . maybe-value)
  (apply (cenv funny-name/define!) name type maybe-value))

; The package on which the environment is based.  This is a temporary hack
; to keep the package-editing code working.

(define funny-name/package (string->symbol "Base package"))

(define (extract-package-from-environment cenv)
  (cenv funny-name/package))

; The name of the source file.
;   This is used by the %FILE-NAME% special form,
;   which is in turn used by the (MODULE ...) form to save the current file in
;    each package, 
;   which is (finally) used to look up filenames in the correct directory.

(define funny-name/source-file-name (string->symbol "Source file name"))

(define (bind-source-file-name filename env)
  (if filename
      (bind1 funny-name/source-file-name filename env)
      env))

(define (source-file-name cenv)
  (cenv funny-name/source-file-name))


