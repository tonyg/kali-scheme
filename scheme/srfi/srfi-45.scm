;;; SRFI 45, which this code is part of, bears the following
;;; copyright/license notice:

;;; Copyright (C) André van Tonder (2003). All Rights Reserved.

;;; This document and translations of it may be copied and furnished to
;;; others, and derivative works that comment on or otherwise explain it
;;; or assist in its implementation may be prepared, copied, published and
;;; distributed, in whole or in part, without restriction of any kind,
;;; provided that the above copyright notice and this paragraph are
;;; included on all such copies and derivative works. However, this
;;; document itself may not be modified in any way, such as by removing
;;; the copyright notice or references to the Scheme Request For
;;; Implementation process or editors, except as needed for the purpose of
;;; developing SRFIs in which case the procedures for copyrights defined
;;; in the SRFI process must be followed, or as required to translate it
;;; into languages other than English.

;;; The limited permissions granted above are perpetual and will not be
;;; revoked by the authors or their successors or assigns.

;;; This document and the information contained herein is provided on an
;;; "AS IS" basis and THE AUTHOR AND THE SRFI EDITORS DISCLAIM ALL
;;; WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY
;;; WARRANTY THAT THE USE OF THE INFORMATION HEREIN WILL NOT INFRINGE ANY
;;; RIGHTS OR ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A
;;; PARTICULAR PURPOSE.

(define-record-type promise :promise
  (make-promise pair)
  promise?
  ;; This pair has as a car either 'EAGER (in which case the cdr is
  ;; the value) or 'LAZY (in which case the cdr is a thunk).
  (pair promise-ref set-promise!))

(define-record-discloser :promise
  (lambda (r)
    (case (car (promise-ref r))
      ((eager)
       (list 'promise 'eager (cdr (promise-ref r))))
      ((lazy)
       (list 'promise 'lazy)))))

;;;=========================================================================
;;; Primitives for lazy evaluation:

(define-syntax lazy
  (syntax-rules ()
    ((lazy exp)
     (make-promise (cons 'lazy (lambda () exp))))))

(define (eager x)
  (make-promise (cons 'eager x)))

(define-syntax delay
  (syntax-rules ()
    ((delay exp) (lazy (eager exp)))))

(define (force promise)
  (let ((content (promise-ref promise)))
    (case (car content)
      ((eager) (cdr content))
      ((lazy)  (let* ((promise* ((cdr content)))        
                      (content  (promise-ref promise)))                ; * 
                 (if (not (eqv? (car content) 'eager))                 ; *
                     (begin (set-car! content (car (promise-ref promise*)))
                            (set-cdr! content (cdr (promise-ref promise*)))
                            (set-promise! promise* content)))
                 (force promise))))))

; (*) These two lines re-fetch and check the original promise in case 
;     the first line of the let* caused it to be forced.  For an example  
;     where this happens, see reentrancy test 3 below.

