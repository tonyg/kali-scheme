;; Copyright (C) Scott G. Miller (2002). All Rights Reserved. 

;; Revised uses of ERROR as compared to the reference implementation.

(define format
  (lambda (format-string . the-objects)
    (let ((buffer (open-output-string)))
      (let loop ((format-list (string->list format-string))
                 (objects the-objects))
        (cond ((null? format-list) (get-output-string buffer))
              ((char=? (car format-list) #\~)
               (if (null? (cdr format-list))
                   (assertion-violation 'format "Incomplete escape sequence" format-string)
                   (case (cadr format-list)
                     ((#\a)
                      (if (null? objects)
                          (apply assertion-violation 'format "No value for escape sequence"
				 format-string the-objects)
                          (begin
                            (display (car objects) buffer)
                            (loop (cddr format-list) (cdr objects)))))
	             ((#\s)
                      (if (null? objects)
                          (apply assertion-violation 'format "No value for escape sequence"
				 format-string the-objects)
                          (begin
                            (write (car objects) buffer)
                            (loop (cddr format-list) (cdr objects)))))
                     ((#\%)
                      (newline buffer)
                      (loop (cddr format-list) objects))
                     ((#\~)
                      (write-char #\~ buffer)
                      (loop (cddr format-list) objects))
                     (else
                      (assertion-violation 'format "Unrecognized escape sequence" format-string)))))
              (else (write-char (car format-list) buffer)
                    (loop (cdr format-list) objects)))))))
