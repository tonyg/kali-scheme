; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Locations

(define location-rtd
  (make-record-type 'location '(id contents assigned? defined?)))

(define make-undefined-location
  (let ((make (record-constructor location-rtd
				  '(id contents assigned? defined?))))
    (lambda (id)
      (make id '*empty* #f #f))))

(define location? (record-predicate location-rtd))
(define location-id        (record-accessor location-rtd 'id))
(define location-defined?  (record-accessor location-rtd 'defined?))
(define location-assigned? (record-accessor location-rtd 'assigned?))
(define contents  (record-accessor location-rtd 'contents))

(define set-location-assigned?! (record-modifier location-rtd 'assigned?))
(define set-location-defined?!  (record-modifier location-rtd 'defined?))
(define set-contents!		(record-modifier location-rtd 'contents))
