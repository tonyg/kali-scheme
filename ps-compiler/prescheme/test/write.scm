; Copyright (c) 1993-2005 by Richard Kelsey.  See file COPYING.


(define (test)
  (let* ((b (allocate-memory 16))
	 (res (read-block (current-input-port)
			  b
			  16
			  (lambda (okay? eof? got)
			    (if (or (not okay?)
				    eof?)
				-1
				(write-block (current-output-port)
					     b
					     got
					     (lambda (okay? sent)
					       (if okay? sent -1))))))))
    (deallocate-memory b)
    res))
