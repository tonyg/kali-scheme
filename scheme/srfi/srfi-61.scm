; SRFI 61 reference implementation

; Copyright (C) 2004 Taylor Campbell.  All rights reserved.
; 
; This document and translations of it may be copied and furnished to others, and
; derivative works that comment on or otherwise explain it or assist in its
; implementation may be prepared, copied, published and distributed, in whole or
; in part, without restriction of any kind, provided that the above copyright notice
; and this paragraph are included on all such copies and derivative works.  However,
; this document itself may not be modified in any way, such as by removing this
; copyright notice or references to the Scheme Request for Implementation process or
; editors, except as needed for the purpose of developing SRFIs in which case the
; procedures for copyrights defined in the SRFI process must be followed, or as
; required to translate it into languages other than English.
; 
; The limited permissions granted above are perpetual and will not be revoked by the
; authors or their successors or assigns.
; 
; This document and the information contained herein is provided on an "AS IS" basis
; and THE AUTHORS AND THE SRFI EDITORS DISCLAIM ALL WARRANTIES, EXPRESS OR IMPLIED,
; INCLUDING BUT NOT LIMITED TO ANY WARRANTY THAT THE USE OF THE INFORMATION HEREIN
; WILL NOT INFRINGE ON ANY RIGHTS OR ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR
; FITNESS FOR A PARTICULAR PURPOSE.

(define-syntax cond
  (syntax-rules (=> ELSE)

    ((COND (ELSE else1 else2 ...))
     ;; The (IF #T (BEGIN ...)) wrapper ensures that there may be no
     ;; internal definitions in the body of the clause.  R5RS mandates
     ;; this in text (by referring to each subform of the clauses as
     ;; <expression>) but not in its reference implementation of COND,
     ;; which just expands to (BEGIN ...) with no (IF #T ...) wrapper.
     (IF #T (BEGIN else1 else2 ...)))

    ((COND (test => receiver) more-clause ...)
     (LET ((T test))
       (COND/MAYBE-MORE T
                        (receiver T)
                        more-clause ...)))

    ((COND (generator guard => receiver) more-clause ...)
     (CALL-WITH-VALUES (LAMBDA () generator)
       (LAMBDA T
         (COND/MAYBE-MORE (APPLY guard    T)
                          (APPLY receiver T)
                          more-clause ...))))

    ((COND (test) more-clause ...)
     (LET ((T test))
       (COND/MAYBE-MORE T T more-clause ...)))

    ((COND (test body1 body2 ...) more-clause ...)
     (COND/MAYBE-MORE test
                      (BEGIN body1 body2 ...)
                      more-clause ...))))

(define-syntax cond/maybe-more
  (syntax-rules ()
    ((COND/MAYBE-MORE test consequent)
     (IF test
         consequent))
    ((COND/MAYBE-MORE test consequent clause ...)
     (IF test
         consequent
         (COND clause ...)))))
