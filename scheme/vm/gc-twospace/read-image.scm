; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; Image reader that reads images into the two-space GC

(define (really-read-image format reverse-byte-order? port)
  (enum-case image-format format
   ((two-space)
    (really-read-image-two-space-native reverse-byte-order? port))
   ((bibop)
    (really-read-image-portable format reverse-byte-order? port))
   (else
    (read-lost "unknown image format" port)
    -1)))

(define (really-read-image-two-space-native reverse-byte-order? port)
  (let* ((delta (address-difference (get-new-heap-start-addr) 
				    (get-img-start-addr)))
	 (new-hp (address+ (get-img-end-addr) delta)))

    ;; Allocate the needed whole space at once 
    (alloc-space (cells->a-units (get-img-heap-size)))
    
    ;; Copy the objects in the allocated space
    (receive (okay? string)
	(image-read-block port (get-new-heap-start-addr) 
			  (cells->a-units (get-img-heap-size)))
      (receive (ch eof? status)
	  (read-char port)
	(cond ((not okay?)
	       (read-lost string port))
	      ((error? status)
	       (read-lost "Error reading from image file" port))
	      ((not eof?)
	       (read-lost "Image file has extraneous data after image" port))
	      ((error? (close-input-port port))
	       (read-lost "Error closing image file" port))
	      (else
	       (if reverse-byte-order?
		   (reverse-byte-order! (get-new-heap-start-addr) new-hp))
	       (if (not (= delta 0))
		   (begin
		     (set-startup-procedure! (adjust (get-startup-procedure) delta))
		     (set-symbols!           (adjust (get-symbols) delta))
		     (set-imported-bindings! (adjust (get-imported-bindings) delta))
		     (set-exported-bindings! (adjust (get-exported-bindings) delta))
		     (set-resumer-records!   (adjust (get-resumer-records) delta))
				   
		     (relocate-symbol-table-two-space! (get-symbols) delta)
		     (relocate-binding-table-two-space! (get-imported-bindings) delta)
		     (relocate-binding-table-two-space! (get-exported-bindings) delta)
		     (relocate-image delta (get-new-heap-start-addr) new-hp)))
	       0))))))

(define (initialize-image-areas!)
  (unspecific))
