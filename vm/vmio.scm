; -*- Mode: Scheme; Syntax: Scheme; Package: Scheme; -*-
; Copyright (c) 1993, 1994 Richard Kelsey and Jonathan Rees.  See file COPYING.


; This is file vmio.scm.

; *** INCOMPLETE MODULARIZATION -- some of the stuff in prim.scm ought
; *** to be here, and vice versa.

;;;; I/O primitives

; Port fields:
;  port-mode         1 = input
;                    2 = output
;                    (this field doesn't ever change)
;  port-index        index into open-ports vector
;                    0 = initial input port
;                    1 = initial output port
;                    -1 = not open
;  peeked-char       char or #f
;  port-id           for debugging
;
; Questions:
;  What to do if an error occurs?
;  How to deal with transcript-on and transcript-off ?
;  How to deal with uninterrubtibly opening a port and pushing it onto
;    an outstanding-ports list?

; *open-vm-ports* is a vector of descriptors for open ports.
; *open-ports is a vector of open ports of the underlying Scheme implementation.

(define number-of-ports 100)

(define *open-ports*    (unassigned))
(define *open-vm-ports* (unassigned))

(define for-input  1)
(define for-output 2)

(define iip-index 0)  ;index of initial input port
(define iop-index 1)  ;[out/in]ditto

(define (initialize-i/o-system)
  (set! *open-ports*     (make-vector number-of-ports))
  (set! *open-vm-ports*  (make-vector number-of-ports))
  (vector+length-fill! *open-vm-ports* number-of-ports false)
  (vector-set! *open-ports* iip-index (current-input-port))
  (vector-set! *open-ports* iop-index (current-output-port))
  (let ((iip (make-port (enter-fixnum for-input)
                        (enter-fixnum iip-index)
                        false
                        (enter-string "si")
			universal-key))
        (iop (make-port (enter-fixnum for-output)
                        (enter-fixnum iop-index)
                        false
                        (enter-string "so")
			universal-key)))
    (vector-set! *open-vm-ports* iip-index iip)
    (vector-set! *open-vm-ports* iop-index iop)
    unspecific))

(define initial-i/o-heap-space (* 2 port-size))

(define (initial-input-port)
  (vector-ref *open-vm-ports* iip-index))

(define (initial-output-port)
  (vector-ref *open-vm-ports* iop-index))

; Auxiliaries for I/O primitives

(define (extract-port port)
  (vector-ref *open-ports* (extract-fixnum (port-index port))))

(define (find-port-index)
  (let loop ((i 0))
    (cond ((>= i number-of-ports)
	   -1)
	  ((false? (vector-ref *open-vm-ports* i))
	   i)
	  (else (loop (+ i 1))))))

(define (use-port-index! index vm-port port)
  (vector-set! *open-ports* index port)
  (vector-set! *open-vm-ports* index vm-port))

; [An open can fail for several reasons:
;    - No space to cons new port,
;    - No more slots in *open-vm-ports* vector,
;    - File not found, directory not found, bad filespec, protection, etc.
; ]

(define (open? port)
  (>= (port-index port) (enter-fixnum 0)))

(define (close-port vm-port)
  (if (open? vm-port)
      (let ((index (extract-fixnum (port-index vm-port))))
        (if (not (or (= index iip-index)
                     (= index iop-index)))
            (let ((port (extract-port vm-port))
                  (mode (extract-fixnum (port-mode vm-port))))
              (cond ((= mode for-input)
                     (close-input-port port))
                    ((= mode for-output)
                     (close-output-port port))
                    (else
                     (error "this shouldn't happen when closing a port")))
              (set-port-mode! vm-port (enter-fixnum 0))
              (set-port-index! vm-port (enter-fixnum -1))
;              (vector-set! *open-ports* index 0)  ; type error
              (vector-set! *open-vm-ports*  index false))))))

; The following are auxiliaries for GC and SUSPEND.

(define (close-port-noisily port)
  (close-port port)
  (write-string "Port closed: " (current-output-port))
  (write-vm-string (port-id port) (current-output-port))
  (newline (current-output-port)))

(define (map-over-open-ports! proc)  ;For suspend and GC
  (do ((i 2 (+ i 1)))   ; Skip over initial input and output ports
      ((= i number-of-ports) #f)
    (let ((port (vector-ref *open-vm-ports* i)))
      (if (not (false? port))
          ;; Update pointer after GC
          (vector-set! *open-vm-ports* i (proc port))))))
