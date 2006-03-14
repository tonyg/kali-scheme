; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-string/bytes-type file-name :file-name
  file-name?
  
  ;; #### We somehow need to arrange that this becomes
  ;; locale-sensitive on Unix and locale-insensitive on Windows
  ;; (which uses a UTF-16-based binary encoding)
  string-encoding-length encode-string
  ;; #### need to become locale-sensitive here
  string-decoding-length decode-string

  thing->file-name
  string->file-name
  byte-vector->file-name
  
  file-name->string
  file-name->byte-vector file-name->byte-string)

; common utility
(define (thing->file-name-byte-string thing)
  (file-name->byte-string
   (thing->file-name thing)))
