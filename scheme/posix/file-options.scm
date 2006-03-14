; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Options for open() and fcntl()
 
(define-enumerated-type file-option :file-option
  file-option?				; predicate
  the-file-options		      ; vector containing all elements
  file-option-name			; name accessor
  file-option-index			; index accessor
  ;; the order of these is known to the C code
  ( ;; Options for open()
   create
   exclusive
   no-controlling-tty
   truncate

   ;; Options for open(), read and written by fcntl()
   append
   synchronized-data	      ; New in POSIX 2nd edition, not in Linux
   nonblocking
   synchronized-read	      ; New in POSIX 2nd edition, not in Linux
   synchronized

   ;; Modes for open(), read by fcntl()
   read-only
   read-write
   write-only))

(define open-options-mask  #o0777)
(define fcntl-options-mask #o0760)
(define mode-mask          #o7000)

(define-enum-set-type file-options :file-options
  file-options?
  make-file-options

  file-option file-option?
  the-file-options
  file-option-index)

(define-exported-binding "posix-file-options-enum-set-type" :file-options)

(define (file-options-on? options0 options1)
  (enum-set=? (enum-set-intersection options0 options1)
	      options1))

(define (file-options-union options0 options1)
  (enum-set-union options0 options1))


