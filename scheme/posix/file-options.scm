; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Options for open() and fcntl()
 
(define-finite-type file-option :file-option
  (mask)
  file-option?                     ; predicate
  the-file-options                 ; vector containing all elements
  file-option-name                 ; name accessor
  file-option-index                ; index accessor
  (mask   file-option-mask)
  (; Options for open()
   (create             #o0001)
   (exclusive          #o0002)
   (no-controlling-tty #o0004)
   (truncate           #o0010)

   ; Options for open(), read and written by fcntl()
   (append             #o0020)
   (synchronized-data  #o0040)  ; New in POSIX 2nd edition, not in Linux
   (nonblocking        #o0100)
   (synchronized-read  #o0200)  ; New in POSIX 2nd edition, not in Linux
   (synchronized       #o0400)

   ; Modes for open(), read by fcntl()
   (read-only          #o1000)
   (read-write         #o2000)
   (write-only         #o4000)))

(define open-options-mask  #o0777)
(define fcntl-options-mask #o0760)
(define mode-mask          #o7000)

(define-record-type file-options :file-options
  (make-file-options value)
  file-options?
  (value file-options-value))

(define-exported-binding "posix-file-options-type" :file-options)

(define (file-options-on? options0 options1)
  (= 0 (bitwise-and (file-options-value options1)
		    (bitwise-not (file-options-value options0)))))

; Simplifying syntax, e.g. (file-options create truncate write-only)

(define-syntax file-options
  (syntax-rules ()
    ((file-options name ...)
     (make-file-options
        (bitwise-ior (file-option-mask (file-option name)) ...)))))

