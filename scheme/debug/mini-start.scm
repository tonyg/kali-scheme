; Copyright (c) 1993-2001 by Richard Kelsey and Jonathan Rees. See file COPYING.

; Start up a system that has reified packages.
; COMMAND-PROCESSOR might be either the miniature one or the real one.

(define (start structs-thunk)
  (usual-resumer
   (lambda (arg)
     (initialize-interaction-environment! (structs-thunk))
     (command-processor #f arg))))

(define (initialize-interaction-environment! structs)
  (let ((scheme (cdr (assq 'scheme structs))))
    (let ((tower (delay (cons eval (scheme-report-environment 5)))))
      (set-interaction-environment!
         (make-simple-package (map cdr structs) #t tower 'interaction))
      (set-scheme-report-environment!
         5
	 (make-simple-package (list scheme) #t tower 'r5rs)))))
