; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Access to Posix process environment

; We multiplex a bunch of these to save typing.

;----------------
; 4.1 Process Identification

(define (get-process-id)
  (call-imported-binding posix-get-pid #f))

(define (get-parent-process-id)
  (call-imported-binding posix-get-pid #t))

;----------------
; 4.2 User Identification and 4.3 Process Groups

(define (get-user-id)
  (call-imported-binding posix-get-id #t #t))

(define (get-effective-user-id)
  (call-imported-binding posix-get-id #t #f))

(define (get-group-id)
  (call-imported-binding posix-get-id #f #t))

(define (get-effective-group-id)
  (call-imported-binding posix-get-id #f #f))

(define (set-user-id! user-id)
  (call-imported-binding posix-set-id! #t user-id))

(define (set-group-id! group-id)
  (call-imported-binding posix-set-id! #f group-id))

(import-definition posix-get-pid)
(import-definition posix-get-id)
(import-definition posix-set-id! "posix_set_id")

(define (get-groups)
  (call-imported-binding posix-get-groups))

(define (get-login-name)
  (call-imported-binding posix-get-login))

(import-definition posix-get-groups)
(import-definition posix-get-login)

(import-lambda-definition posix-set-sid ())

;----------------
; 4.4 System Identification
;
; The five values returned by uname().

(import-lambda-definition posix-sys-name (which))

(define (os-name)         (posix-sys-name 0))
(define (os-node-name)    (posix-sys-name 1))
(define (os-release-name) (posix-sys-name 2))
(define (os-version-name) (posix-sys-name 3))
(define (machine-name)    (posix-sys-name 4))

;----------------
; 4.5 Get Process Times
;
;----------------
; 4.6 Environment Variables

; We cheat here by using one type for both the variable names and
; their values.  The rules are the same for both, after all.

(define (lookup-environment-variable name)
  (cond
   ((external-lookup-environment-variable
     (os-string->byte-vector
      (x->os-string name)))
    => x->os-string)
   (else #f)))

(define (lookup-environment-variable->string name)
  (cond
   ((lookup-environment-variable name)
    => os-string->string)
   (else #f)))
   
(define (environment-alist)
  (map (lambda (pair)
	 (cons (x->os-string (car pair))
	       (x->os-string (cdr pair))))
       (external-environment-alist)))

(define (environment-alist-as-strings)
  (map (lambda (pair)
	 (cons (os-string->string (car pair))
	       (os-string->string (cdr pair))))
       (environment-alist)))

(import-lambda-definition external-lookup-environment-variable (name) "posix_get_env")
(import-lambda-definition external-environment-alist () "posix_get_env_alist")

;----------------
; 4.7 Terminal Identification

; See io.scm.

;----------------
; 4.8 Configurable System Variables
