; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

;; Image reader that reads images into the BIBOP GC

(define (really-read-image format reverse-byte-order? port)
  (enum-case image-format format
   ((two-space)
    (really-read-image-portable format reverse-byte-order? port))
   ((bibop)
    (really-read-image-bibop-native reverse-byte-order? port))
   (else
    (read-lost "invalid image format" port))))

(define (really-read-image-bibop-native reverse-byte-order? port)
  ;; debugging
  ;;(describe-small)
  ;;(describe-large)
  ;;(describe-weaks)
  
  (let* ((small-delta (address-difference (s48-get-new-small-start-addr) 
					  (get-small-img-start-addr)))
	 (small-new-hp (address+ (get-small-img-hp-addr) small-delta))
	 (large-delta (address-difference (s48-get-new-large-start-addr) 
					  (get-large-img-start-addr)))
	 (large-new-hp (address+ (get-large-img-hp-addr) large-delta))
	 (weaks-delta (address-difference (s48-get-new-weaks-start-addr) 
					  (get-weaks-img-start-addr)))
	 (weaks-new-hp (address+ (get-weaks-img-hp-addr) weaks-delta)))

    (cond
     ((got-error? (really-read-weaks-image port))
      (read-lost "error reading weaks area from image" port))
     ((got-error? (really-read-large-image port))
      (read-lost "error reading large area from image" port))
     ((got-error? (really-read-small-image port))
      (read-lost "error reading small area from image" port))
     (else
      (receive (ch eof? status)
	  (read-char port)
	(cond ((error? status)
	       (read-lost "Error reading from image file" port))
	      ((not eof?)
	       (read-lost "Image file has extraneous data after image" port))
	      ((error? (close-input-port port))
	       (read-lost "Error closing image file" port))
	      (else
	       (if reverse-byte-order?
		   (begin
		     (reverse-byte-order! (s48-get-new-small-start-addr) small-new-hp)
		     (reverse-byte-order! (s48-get-new-large-start-addr) large-new-hp)
		     (reverse-byte-order! (s48-get-new-weaks-start-addr) weaks-new-hp)))
		    
	       ;; here we have to adjust along the new sizes of the areas

	       ;; TODO: the adjustment must depend on where the
	       ;; addresses point to (small, large, weaks). If the
	       ;; delta's differ, it won't work like it is, so for now
	       ;; we make sure they are equal
	       (if (not (and (= small-delta large-delta)
			     (= large-delta weaks-delta)))
		   (error "Bug: Cannot load image, because the deltas of all parts aren't equal. Notify the authors."))
	       (if (not (= small-delta 0))
		   (begin

		     (set-startup-procedure! (adjust (get-startup-procedure) small-delta))
		     (set-symbols!           (adjust (get-symbols) small-delta))
		     (set-imported-bindings! (adjust (get-imported-bindings) small-delta))
		     (set-exported-bindings! (adjust (get-exported-bindings) small-delta))
		     (set-resumer-records!   (adjust (get-resumer-records) small-delta))
			  
		     (relocate-symbol-table-two-space! (get-symbols) small-delta)
		     (relocate-binding-table-two-space! (get-imported-bindings) small-delta)
		     (relocate-binding-table-two-space! (get-exported-bindings) small-delta)
			  
		     (relocate-image small-delta (s48-get-new-small-start-addr) small-new-hp)))

	       (if (not (= large-delta 0))
		   (begin
			  
		     ;; debugging
		     ;;(notify-large-delta)

		     (relocate-image large-delta (s48-get-new-large-start-addr) large-new-hp)
		     (unspecific)))
		    
	       (if (not (= weaks-delta 0))
		   (begin

		     ;; debugging
		     ;;(notify-weaks-delta)

		     (relocate-image weaks-delta (s48-get-new-weaks-start-addr) weaks-new-hp)
		     (unspecific)))
	       0)))))))

(define s48-get-new-small-end-addr
  (external "s48_get_new_small_end_addr" (=> () address)))

(define s48-get-new-large-end-addr
  (external "s48_get_new_large_end_addr" (=> () address)))

(define s48-get-new-weaks-end-addr
  (external "s48_get_new_weaks_end_addr" (=> () address)))

(define s48-get-new-small-size
  (external "s48_get_new_small_size" (=> () integer)))

(define s48-get-new-large-size
  (external "s48_get_new_large_size" (=> () integer)))

(define s48-get-new-weaks-size
  (external "s48_get_new_weaks_size" (=> () integer)))

(define (get-small-img-whole-size)
  (address-difference (get-small-img-end-addr)
		      (get-small-img-start-addr)))

(define (get-large-img-whole-size)
  (address-difference (get-large-img-end-addr)
		      (get-large-img-start-addr)))

(define (get-weaks-img-whole-size)
  (address-difference (get-weaks-img-end-addr)
		      (get-weaks-img-start-addr)))

(define (initialize-image-areas!)
  (s48-initialize-image-areas (get-small-img-whole-size)
			      (address-difference (get-small-img-hp-addr)
						  (get-small-img-start-addr))
			      (get-large-img-whole-size)
			      (address-difference (get-large-img-hp-addr)
						  (get-large-img-start-addr))
			      (get-weaks-img-whole-size)
			      (address-difference (get-weaks-img-hp-addr)
						  (get-weaks-img-start-addr))))

; debugging

(define (describe-small)
  (write-out-newline)
  (write-out-string "-----------")
  (write-out-newline)
  (write-out-string "SMALL DATA ")
  (write-out-newline)
  (write-out-string "-----------")
  (write-out-newline)
  (write-out-string "(get-small-img-start-addr): ")
  (write-out-integer (address->integer (get-small-img-start-addr)))
  (write-out-newline)
  (write-out-string "(get-small-img-hp-addr): ")
  (write-out-integer (address->integer (get-small-img-hp-addr)))
  (write-out-newline)
  (write-out-string "(get-small-img-end-addr): ")
  (write-out-integer (address->integer (get-small-img-end-addr)))
  (write-out-newline)
  (write-out-string "PAGES: ")
  (write-out-integer (quotient (address-difference (get-small-img-end-addr)
						   (get-small-img-start-addr))
			       4096))
  (write-out-newline)
  (write-out-string "(get-new-small-start-addr): ")
  (write-out-integer (address->integer (s48-get-new-small-start-addr)))
  (write-out-newline)
  (write-out-string "(get-new-small-end-addr): ")
  (write-out-integer (address->integer (s48-get-new-small-end-addr)))
  (write-out-newline)
  (write-out-string "PAGES: : ")
  (write-out-integer (quotient (address-difference (s48-get-new-small-end-addr)
						   (s48-get-new-small-start-addr))
			       4096))
  (write-out-newline)
  (write-out-string "(get-new-small-size): ")
  (write-out-integer (s48-get-new-small-size))
  (write-out-newline)
  (write-out-string "(get-small-img-heap-size): ")
  (write-out-integer (cells->a-units (get-small-img-heap-size)))
  (write-out-newline)
)



(define (describe-large)
  (write-out-newline)
  (write-out-string "-----------")
  (write-out-newline)
  (write-out-string "LARGE DATA ")
  (write-out-newline)
  (write-out-string "-----------")
  (write-out-newline)
  (write-out-newline)
  (write-out-string "(get-large-img-start-addr): ")
  (write-out-integer (address->integer (get-large-img-start-addr)))
  (write-out-newline)
  (write-out-string "(get-large-img-end-addr): ")
  (write-out-integer (address->integer (get-large-img-end-addr)))
  (write-out-newline)
  (write-out-string "PAGES: ")
  (write-out-integer (quotient (address-difference (get-large-img-end-addr)
					    (get-large-img-start-addr))
			4096))
  (write-out-newline)
  (write-out-string "(get-new-large-start-addr): ")
  (write-out-integer (address->integer (s48-get-new-large-start-addr)))
  (write-out-newline)
  (write-out-string "(get-new-large-end-addr): ")
  (write-out-integer (address->integer (s48-get-new-large-end-addr)))
  (write-out-newline)
  (write-out-string "PAGES: : ")
  (write-out-integer (quotient (s48-get-new-large-size) 4096))
  (write-out-newline)
  (write-out-string "(get-new-large-size): ")
  (write-out-integer (s48-get-new-large-size))
  (write-out-newline)
  (write-out-string "(get-large-img-heap-size): ")
  (write-out-integer (cells->a-units (get-large-img-heap-size)))
  (write-out-newline))

(define (describe-weaks)
  (write-out-newline)
  (write-out-string "-----------")
  (write-out-newline)
  (write-out-string "WEAKS DATA ")
  (write-out-newline)
  (write-out-string "-----------")
  (write-out-newline)
  (write-out-newline)
  (write-out-string "(get-weaks-img-start-addr): ")
  (write-out-integer (address->integer (get-weaks-img-start-addr)))
  (write-out-newline)
  (write-out-string "(get-weaks-img-end-addr): ")
  (write-out-integer (address->integer (get-weaks-img-end-addr)))
  (write-out-newline)
  (write-out-string "PAGES: ")
  (write-out-integer (quotient (address-difference (get-weaks-img-end-addr)
					    (get-weaks-img-start-addr))
			4096))
  (write-out-newline)
  (write-out-string "(get-new-weaks-start-addr): ")
  (write-out-integer (address->integer (s48-get-new-weaks-start-addr)))
  (write-out-newline)
  (write-out-string "(get-new-weaks-end-addr): ")
  (write-out-integer (address->integer (s48-get-new-weaks-end-addr)))
  (write-out-newline)
  (write-out-string "PAGES: : ")
  (write-out-integer (quotient (s48-get-new-weaks-size) 4096))
  (write-out-newline)
  (write-out-string "(get-new-weaks-size): ")
  (write-out-integer (s48-get-new-weaks-size))
  (write-out-newline)
  (write-out-string "(get-weaks-img-heap-size): ")
  (write-out-integer (cells->a-units (get-weaks-img-heap-size)))
  (write-out-newline))

(define (notify-small-delta)
  (break "ADJUST SMALL DELTA"))

(define (notify-large-delta)
  (break "ADJUST LARGE DELTA"))

(define (notify-weaks-delta)
  (break "ADJUST WEAKS DELTA"))

(define (break str)
  (write-out-newline)
  (write-out-string str)
  (write-out-newline))

; Utilities for portable reader

(define (read-image-area new-start-addr img-heap-size port)
  (receive (okay? string)
      (image-read-block port 
			new-start-addr
			(cells->a-units img-heap-size))
    (cond ((not okay?)
	   (read-lost string port))
	  (else 0))))

(define (really-read-small-image port)
  (read-image-area (s48-get-new-small-start-addr)
		   (get-small-img-heap-size)
		   port))
(define (really-read-large-image port)
  (read-image-area (s48-get-new-large-start-addr)
		   (get-large-img-heap-size)
		   port))
(define (really-read-weaks-image port)
  (read-image-area (s48-get-new-weaks-start-addr)
		   (get-weaks-img-heap-size)
		   port))
