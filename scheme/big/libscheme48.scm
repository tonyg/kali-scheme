; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; To use libscheme48, do this from a vanilla command prompt:

; ,open libscheme48
; <load anything else you want to access from C>
; (dump-libscheme48-image "scheme48-command.image")

; Now you can link a C application against libscheme48, and do
; something like this:
;
; #include <stdio.h>
; #include "scheme48.h"
; 
; int main(int argc, char ** argv)
; {
;   char* a[] = {"scheme48", "-i", "scheme48-command.image"};
; 
;   s48_main(3, a);
; 
;   s48_call_scheme(...);
; 
;   return 0;
; }

(define (start-libscheme48 args)
  (return-from-vm 0))

(define null-continuation #f)

(define (return-from-vm n)
  (with-continuation null-continuation (lambda () n)))

(define (dump-libscheme48-image filename)
  (build-image start-libscheme48 filename))
