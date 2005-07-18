; Copyright (c) 1993-2005 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Encoders/decoders from text to bytes and vice versa, for use by the
; the ports subsystem.

; Note that encoders and decoders must operate on buffers only
; provisionally.

(define-record-type text-codec :text-codec
  (really-make-text-codec names
			  builtin-code
			  encode-char-proc
			  decode-char-proc)
  text-codec?
  (names text-codec-names)
  ;; either #f or an integer from enum TEXT-ENCODING-OPTION
  ;; for encodings built into the VM
  (builtin-code text-codec-builtin-code)
  ;; (char buffer start count) -> (ok? #f or #bytes consumed or #bytes needed)
  (encode-char-proc text-codec-encode-char-proc)
  ;; (buffer start count) -> (char #bytes consumed)
  ;;                      or (#f #total bytes needed at least)
  ;;                      or (#f #f) (failure)
  (decode-char-proc text-codec-decode-char-proc))

(define (make-builtin-text-codec names code)
  (really-make-text-codec names
			  code
			  (lambda (char buffer start count)
			    (encode-char code char buffer start count))
			  (lambda (buffer start count)
			    (decode-char code buffer start count))))

(define (make-text-codec names encode-char-proc decode-char-proc)
  (really-make-text-codec names #f encode-char-proc decode-char-proc))

(define-record-discloser :text-codec
  (lambda (r)
    (cons 'text-codec (text-codec-names r))))

(define *builtin-text-codecs*
  (make-vector (+ (max (enum text-encoding-option us-ascii)
		       (enum text-encoding-option utf-8)
		       (enum text-encoding-option utf-16le)
		       (enum text-encoding-option utf-16be)
		       (enum text-encoding-option utf-32le)
		       (enum text-encoding-option utf-32be))
		  1)))

(define (port-text-codec p)
  (let ((spec (port-text-codec-spec p)))
    (if (text-codec? spec)
	spec
	(vector-ref *builtin-text-codecs* spec))))

(define (set-port-text-codec! p codec)
  (cond
   ((text-codec-builtin-code codec)
    => (lambda (code)
	 (set-port-text-codec-spec! p code)))
   (else
    (set-port-text-codec-spec! p codec))))

(define *text-codecs* '())

(define (register-text-codec! codec)
  (set! *text-codecs* (cons codec *text-codecs*)))

(define (find-text-codec name)
  (let loop ((codecs *text-codecs*))
    (cond
     ((null? codecs) #f)
     ((member name (text-codec-names (car codecs)))
      (car codecs))
     (else (loop (cdr codecs))))))

(define-syntax define-text-codec
  (syntax-rules ()
    ((define-text-codec ?id (?name ...) ?encode-proc ?decode-proc)
     (begin
       (define ?id (make-text-codec '(?name ...) ?encode-proc ?decode-proc))
       (register-text-codec! ?id)))
    ((define-text-codec ?id ?name ?encode-proc ?decode-proc)
     (define-text-codec ?id (?name) ?encode-proc ?decode-proc))))

(define-syntax define-builtin-text-codec
  (syntax-rules ()
    ((define-builtin-text-codec ?id (?name ...) ?enumerand)
     (begin
       (define ?id (make-builtin-text-codec '(?name ...) (enum text-encoding-option ?enumerand)))
       (register-text-codec! ?id)
       (vector-set! *builtin-text-codecs* (enum text-encoding-option ?enumerand)
		    ?id)))
    ((define-builtin-text-codec ?id ?name ?enumerand)
     (define-builtin-text-codec ?id (?name) ?enumerand))))
       
(define-text-codec null-text-codec "null"
  (lambda (char buffer start count)
    #f)
  (lambda (buffer start count)
    (values #f #f)))

(define-builtin-text-codec us-ascii-codec "US-ASCII" us-ascii)
(define-builtin-text-codec latin-1-codec "ISO8859-1" latin-1)
(define-builtin-text-codec utf-8-codec "UTF-8" utf-8)
(define-builtin-text-codec utf-16le-codec "UTF-16LE" utf-16le)
(define-builtin-text-codec utf-16be-codec "UTF-16BE" utf-16be)
(define-builtin-text-codec utf-32le-codec "UTF-32LE" utf-32le)
(define-builtin-text-codec utf-32be-codec "UTF-32BE" utf-32be)
