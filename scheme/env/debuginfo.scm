; Copyright (c) 1993-2000 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Reading/writing debugging info

(define (write-debug-info location-info file)
  (call-with-output-file file
    (lambda (port)

      (display "Writing ") (display file) (newline)
      (let ((write-table
	     (lambda (table comment)
	       (display "; " port) (display comment port) (newline port)
	       (table-walk (lambda (key datum)
			     (write (list key datum) port)
			     (newline port))
			   table)
	       (write '- port) (newline port))))
	(write-table package-name-table "Package uid -> name")
	(write-table location-info "Location uid -> (name . package-uid)"))

      (display "; Template uid -> name, parent, pc in parent, env maps" port)
      (newline port)
      (table-walk (lambda (id data)
		    ;; Fields: (uid name parent pc-in-parent
		    ;;	        env-maps source)
		    (write (list id
				 (let ((name (debug-data-name data)))
				   (if (generated? name)
				       (name->symbol name)
				       name))
				 (let ((p (debug-data-parent data)))
				   ;; we'd like to (note-debug-data! p)
				   (if (debug-data? p)
				       (debug-data-uid p)
				       p))
				 (debug-data-pc-in-parent data)
				 (debug-data-env-maps data)
				 ;; Don't retain source code, right?
				 )
			   port)
		    (newline port))
		  (debug-data-table))
      (write '- port) (newline port))))

(define (read-debug-info file)
  (call-with-input-file file
    (lambda (port)

      (display "Reading ") (display file) (newline)

      (let ((read-table
	     (lambda (table)
	       (let loop ()
		 (let ((z (read port)))
		   (if (pair? z)
		       (begin (table-set! table
					  (car z)
					  (make-immutable! (cadr z)))
			      ;; (set! *location-uid*
			      ;;       (max *location-uid* (+ (car z) 1)))
			      (loop))))))))
	(read-table package-name-table)
	(read-table location-info-table))

      (let loop ()
	(let ((z (read port)))
	  (if (pair? z)
	      (begin ;; (set! *template-uid*
		     ;;	      (max *template-uid* (+ (car z) 1)))
		     (table-set! (debug-data-table)
				 (car z)
				 (make-immutable!
				  (apply make-debug-data
					 (append z '(())))))
		     (loop))))))))
