; Copyright (c) 1993-1999 by Richard Kelsey and Jonathan Rees. See file COPYING.




; Stuff for debugging new images:

(define (ev form package)
  (invoke-template (compile-form form package)
		   (package-uid package)))

; If desired, this definition of invoke-template can be replaced by
; something that starts up a different virtual machine.

(define (invoke-template template env . args)
  (apply (make-closure template env)
	 args))

; Utility for tracking down uses of variables

(define (who-uses name proc)
  (let recur ((tem (closure-template proc))
              (path '()))
    (let loop ((i 0))
      (if (< i (template-length tem))
          (let ((thing (template-ref tem i))
                (down (lambda (tem)
                        (recur tem (cons (or (template-ref tem 1) i) path)))))
            (cond ((template? thing)
                   (down thing))
                  ((location? thing)
                   (if (eq? (location-name thing) name)
                       (begin (write path) (newline))))
                  ((closure? thing)
                   (down (closure-template thing))))
            (loop (+ i 1)))))))
