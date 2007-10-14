; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; BIBOP-specific part of the dumper

(define small-image-start-address 0)
(define large-image-start-address 0)
(define weaks-image-start-address 0)

(define (write-header resumer-records resume-proc image-descriptor port)
  ;; debugging
  ;;(display-small-image)
  ;;(display-large-image)
  ;;(display-weaks-image)

  (write-check (write-char #\newline port))
  (write-page)
  (write-check (write-char #\newline port))
  (write-check (write-string architecture-version port))
  (write-check (write-char #\newline port))
  (write-check (write-integer (enum image-format bibop) port))
  (write-check (write-char #\newline port))
  (write-header-integer bytes-per-cell)

  ;; small area addresses
  (write-header-integer (a-units->cells (address->integer *small-image-begin*)))
  (write-header-integer (a-units->cells (address->integer *small-image-hp*)))
;;  (write-header-integer (a-units->cells (address->integer *small-image-end*)))

  ;; large area addresses
  (write-header-integer (a-units->cells (address->integer *large-image-begin*)))
  (write-header-integer (a-units->cells (address->integer *large-image-hp*)))
;;  (write-header-integer (a-units->cells (address->integer *large-image-end*)))

  ;; weaks area addresses
  (write-header-integer (a-units->cells (address->integer *weaks-image-begin*)))
  (write-header-integer (a-units->cells (address->integer *weaks-image-hp*)))
  (write-header-integer (a-units->cells (address->integer *weaks-image-end*)))

  (write-header-integer (image-descriptor (s48-symbol-table)))
  (write-header-integer (image-descriptor (s48-imported-bindings)))
  (write-header-integer (image-descriptor (s48-exported-bindings)))
  (write-header-integer resumer-records)
  (write-header-integer resume-proc)
  (write-page))

(define (begin-making-image/gc-specific)
;  (let ((cand-addr (get-candidate-address)))
;    (if (not (null-address? cand-addr))
;	(set! small-image-start-address (address->integer cand-addr))))
  ;; There seems to be no problem with a possible null-address - David
  (set! small-image-start-address (address->integer (get-candidate-address)))

  (set! *small-image-begin* (integer->address small-image-start-address))
  (set! *small-image-hp* (integer->address small-image-start-address))
  (set! *small-image-end* null-address)

  (set! *large-image-begin* (integer->address large-image-start-address))
  (set! *large-image-hp* (integer->address large-image-start-address))
  (set! *large-image-end* null-address)

  (set! *weaks-image-begin* (integer->address weaks-image-start-address))
  (set! *weaks-image-hp* (integer->address weaks-image-start-address))
  (set! *weaks-image-end* null-address)
  
  ;; Initialize the areas
  (set! *bibop-areas* (make-bibop-areas))
  (set! *traced-last-stob?* #f))

(define (note-traced-last-stob!)      
  (set! *traced-last-stob?* #t))

(define (allocate-new-image-object stob)
  (let ((new-descriptor (image-alloc
			 (s48-area-type-size stob)
			 (header-length-in-a-units 
			  (stob-header stob)))))
    (values new-descriptor 
	    (make-image-location new-descriptor))))

(define (finalize-new-image-object stob)
  (set! *stob-table-obj-nr* (+ *stob-table-obj-nr* 1))
  (update-bibop-areas! (s48-area-type-size stob) stob))  

(define (small-image-size)
  (address-difference *small-image-hp* *small-image-begin*))

(define (large-image-size)
  (address-difference *large-image-hp* *large-image-begin*))

(define (weaks-image-size)
  (address-difference *weaks-image-hp* *weaks-image-begin*))

(define (get-candidate-address)
  (s48-get-new-small-start-addr))

(define *small-image-begin*)	; Starting address (small part image)
(define *small-image-hp*)       ; Current ending address (small part image)
(define *small-image-end*)      ; Ending address of the small area (BIBOP)

(define *large-image-begin*)	; Starting address (large part image)
(define *large-image-hp*)       ; Current ending address (large part image)
(define *large-image-end*)      ; Ending address of the large area (BIBOP)

(define *weaks-image-begin*)	; Starting address (weaks part image)
(define *weaks-image-hp*)       ; Current ending address (weaks part image)
(define *weaks-image-end*)      ; Ending address of the weaks area (BIBOP)

(define *bibop-areas*)         ; Record to keep the stobs in 3 areas (small, large, weaks)
(define *traced-last-stob?* #f) ; Flag to mark the last traced stob in bibop-areas

; Each STOB's new-descriptor becomes a descriptor-address from its own
; image part (small, large, weaks). Each stob will be written in
; *bibop-areas* container for the last scan to write the image
; properly.

(define s48-area-type-size
  (external "s48_area_type_size" (=> (integer) integer)))

(define (write-image-areas first-stob stob-table write-stob)
  (write-weaks-area write-stob)
  (write-large-area write-stob)
  (write-small-area write-stob)
  (unspecific))

;; --------------------------------------------------------------------
;; Bibop Areas
;; --------------------------------------------------------------------
;; In this container are collected the stobs - before they 'll
;; inserted into the *stob-table* - categorized among the heap area
;; they belong to: small, large and weaks.

;; - During 'make-image', this container will be scanned to adjust the
;; descriptors in *stob-table*

;; - During 'write-image', this container will be scanned to write the
;; stobs from *stob-table* for each area separate
;; --------------------------------------------------------------------

(define-record-type bibop-areas :bibop-areas
  (really-make-bibop-areas small small-index 
			    large large-index 
			    weaks weaks-index)
  (small         (^ integer) bibop-areas-small         set-bibop-areas-small!)
  (small-index   integer     bibop-areas-small-index   set-bibop-areas-small-index!)

  (large         (^ integer) bibop-areas-large         set-bibop-areas-large!)
  (large-index   integer     bibop-areas-large-index   set-bibop-areas-large-index!)

  (weaks         (^ integer) bibop-areas-weaks         set-bibop-areas-weaks!)
  (weaks-index   integer     bibop-areas-weaks-index   set-bibop-areas-weaks-index!))

;; How big are the vectors (each area) wich hold the stobs
(define *initial-stob-obj-nr* (shift-left 1 20))

(define (make-bibop-areas)
  (let ((small (make-vector *initial-stob-obj-nr* 0))
	(large (make-vector *initial-stob-obj-nr* 0))
	(weaks (make-vector *initial-stob-obj-nr* 0)))
    (really-make-bibop-areas small 0
			      large 0
			      weaks 0)))

(define (deallocate-bibop-areas)
  (deallocate (bibop-areas-small (get-bibop-areas)))
  (deallocate (bibop-areas-large (get-bibop-areas)))
  (deallocate (bibop-areas-weaks (get-bibop-areas)))
  (deallocate (get-bibop-areas)))

(define (get-bibop-areas)
  *bibop-areas*)

(define (update-bibop-areas! type-size stob)
  (enum-case area-type-size type-size
    ((small)
     (insert-small-area! stob))
    ((large)
     (insert-large-area! stob))
    ((weaks)
     (insert-weaks-area! stob))
    (else (error "Unexpected area type size!"))))

(define (insert-small-area! stob)
  (if *traced-last-stob?*
      ;; Mark this index as the last traced (means not to be written)
      ;; But adjustment must be done to all of them
      (let ((i (bibop-areas-small-index (get-bibop-areas))))
	(assert (not (= i *initial-stob-obj-nr*)))
	(vector-set! (bibop-areas-small (get-bibop-areas)) i false)
	(set-bibop-areas-small-index! 
	 (get-bibop-areas) 
	 (+ (bibop-areas-small-index (get-bibop-areas)) 1))))

  ;; Now insert the stob
  (let ((i (bibop-areas-small-index (get-bibop-areas))))
    (assert (not (= i *initial-stob-obj-nr*)))
    (vector-set! (bibop-areas-small (get-bibop-areas)) i stob)
    (set-bibop-areas-small-index! 
     (get-bibop-areas) 
     (+ (bibop-areas-small-index (get-bibop-areas)) 1))))

(define (insert-large-area! stob)
  (if *traced-last-stob?*
      ;; Mark this index as the last traced (means not to be written)
      ;; But adjustment must be done to all of them
      (let ((i (bibop-areas-large-index (get-bibop-areas))))
	(assert (not (= i *initial-stob-obj-nr*)))
	(vector-set! (bibop-areas-large (get-bibop-areas)) i false)
	(set-bibop-areas-large-index!
	 (get-bibop-areas) 
	 (+ (bibop-areas-large-index (get-bibop-areas)) 1))))
  
  ;; Now insert the stob
  (let ((i (bibop-areas-large-index (get-bibop-areas))))
    (assert (not (= i *initial-stob-obj-nr*)))
    (vector-set! (bibop-areas-large (get-bibop-areas)) i stob)
    (set-bibop-areas-large-index!
     (get-bibop-areas)
     (+ (bibop-areas-large-index (get-bibop-areas)) 1))))

(define (insert-weaks-area! stob)
  (if *traced-last-stob?*
      ;; Mark this index as the last traced (means not to be written)
      ;; But adjustment must be done to all of them
      (let ((i (bibop-areas-weaks-index (get-bibop-areas))))
	(assert (not (= i *initial-stob-obj-nr*)))
	(vector-set! (bibop-areas-weaks (get-bibop-areas)) i false)
	(set-bibop-areas-weaks-index!
	 (get-bibop-areas) 
	 (+ (bibop-areas-weaks-index (get-bibop-areas)) 1))))
  
  ;; Now insert the stob
  (let ((i (bibop-areas-weaks-index (get-bibop-areas))))
    (assert (not (= i *initial-stob-obj-nr*)))
    (vector-set! (bibop-areas-weaks (get-bibop-areas)) i stob)
    (set-bibop-areas-weaks-index! 
     (get-bibop-areas) 
     (+ (bibop-areas-weaks-index (get-bibop-areas)) 1))))

; - The descriptors of the small objects have already been adjusted
;   by small-image-start-address 
; - The descriptors of the large and weaks objects will be adjusted
;   by the last address of the future bibop area (rounded pages)

(define (adjust-descriptors! stob-table)
  (calculate-bibop-small-image-end!)
  (calculate-bibop-large-image-end!)
  (calculate-bibop-weaks-image-end!)
  (adjust-large-area-descriptors! stob-table)
  (adjust-weaks-area-descriptors! stob-table))

(define log-bytes-per-page 12)

(define bytes-per-page (shift-left 1 log-bytes-per-page))

(define (bytes->pages n)
  (arithmetic-shift-right (+ n (- bytes-per-page 1)) log-bytes-per-page))

(define (pages->bytes n)
  (shift-left n log-bytes-per-page))

(define (calculate-bibop-small-image-end!)
  (set! *small-image-end*
	(address+ *small-image-begin*
		  (pages->bytes (bytes->pages (if (= 0 (small-image-size))
						  1
						  (small-image-size)))))))

(define (calculate-bibop-large-image-end!)
  (set! *large-image-end*
	(address+ *large-image-begin*
		  (pages->bytes (bytes->pages (if (= 0 (large-image-size))
						  1
						  (large-image-size)))))))

(define (calculate-bibop-weaks-image-end!)
  (set! *weaks-image-end*
	(address+ *weaks-image-begin*
		  (pages->bytes (bytes->pages (if (= 0 (weaks-image-size))
						  1
						  (weaks-image-size)))))))

(define (adjust-large-area-descriptors! stob-table)
  (do ((i 0 (+ i 1)))
      ((= i (bibop-areas-large-index (get-bibop-areas))))
    (let ((stob (vector-ref (bibop-areas-large (get-bibop-areas)) i)))
      ;; If this is 'false'(= the marked index as last stob) jump it 
      (if (and (stob? stob)
	       (not (= stob false)))
	  (let ((image-location (table-ref stob-table stob)))
	    (set-image-location-new-descriptor!
	     image-location
	     (address->stob-descriptor
	      (address+ *small-image-end*
			(address->integer 
			 (address-after-header 
			  (image-location-new-descriptor image-location))))))))))

  ;; Move the pointers of the large area
  (set! *large-image-begin* (address+ *small-image-end*
				      (address->integer *large-image-begin*)))
  (set! *large-image-hp* (address+ *small-image-end*
				   (address->integer *large-image-hp*)))
  (set! *large-image-end* (address+ *small-image-end*
				    (address->integer *large-image-end*))))

(define (adjust-weaks-area-descriptors! stob-table)
  (do ((i 0 (+ i 1)))
      ((= i (bibop-areas-weaks-index (get-bibop-areas))))
    (let ((stob (vector-ref (bibop-areas-weaks (get-bibop-areas)) i)))
      ;; If this is 'false'(= the marked index as last stob) jump it 
      (if (and (stob? stob)
	       (not (= stob false)))
	  (let ((image-location (table-ref stob-table stob)))
	    (set-image-location-new-descriptor! 
	     image-location
	     (address->stob-descriptor
	      (address+ *large-image-end*
			(address->integer 
			 (address-after-header 
			  (image-location-new-descriptor image-location))))))))))
  
  ;; Move the pointers of the weaks area
  (set! *weaks-image-begin* (address+ *large-image-end*
				      (address->integer *weaks-image-begin*)))
  (set! *weaks-image-hp* (address+ *large-image-end*
				   (address->integer *weaks-image-hp*)))
  (set! *weaks-image-end* (address+ *large-image-end*
				    (address->integer *weaks-image-end*))))


;; Write everything till last traced stob (marked index 'false')
(define (write-small-area write-stob)
    (do ((i 0 (+ i 1)))
	((or (= i (bibop-areas-small-index (get-bibop-areas)))
	     (= false (vector-ref (bibop-areas-small (get-bibop-areas)) i))))
      (write-stob (vector-ref (bibop-areas-small (get-bibop-areas)) i))))

;; Write everything till last traced stob (marked index 'false')
(define (write-large-area write-stob)
    (do ((i 0 (+ i 1)))
	((or (= i (bibop-areas-large-index (get-bibop-areas)))
	     (= false (vector-ref (bibop-areas-large (get-bibop-areas)) i))))
      (write-stob (vector-ref (bibop-areas-large (get-bibop-areas)) i))))

;; Write everything till last traced stob (marked index 'false')
(define (write-weaks-area write-stob)
    (do ((i 0 (+ i 1)))
	((or (= i (bibop-areas-weaks-index (get-bibop-areas)))
	     (= false (vector-ref (bibop-areas-weaks (get-bibop-areas)) i))))
      (write-stob (vector-ref (bibop-areas-weaks (get-bibop-areas)) i))))

(define (image-alloc type-size length-in-a-units)
  (let ((image-hp
	 (enum-case area-type-size type-size
           ((small) *small-image-hp*)
	   ((large) *large-image-hp*)
	   ((weaks) *weaks-image-hp*)
	   (else
	    (error "invalid area tag")
	    *weaks-image-hp*)))) ; don't confuse the PreScheme compiler
    (let ((data-addr (address+ image-hp (cells->a-units stob-overhead))))
      (enum-case area-type-size type-size
        ((small)
	 (set! *small-image-hp* (address+ data-addr length-in-a-units)))
	((large)
	 (set! *large-image-hp* (address+ data-addr length-in-a-units)))
	((weaks)
	 (set! *weaks-image-hp* (address+ data-addr length-in-a-units))))
      (address->stob-descriptor data-addr))))

(define (deallocate-areas)
  (unspecific))

;; Debugging stuff
(define *stob-table-obj-nr* 0)

(define (*bibop-areas-obj-nr*)
  (+ (bibop-areas-small-index (get-bibop-areas))
     (bibop-areas-large-index (get-bibop-areas))
     (bibop-areas-weaks-index (get-bibop-areas))))

(define (compare-stobs-nr)
  (write-out-newline)
  (write-out-string "STOB-TABLE has : ")
  (write-out-integer *stob-table-obj-nr*)
  (write-out-newline)
  (write-out-string "SMALL IMAGE-SIZE : ")
  (write-out-integer (small-image-size))
  (write-out-newline)
  (write-out-string "LARGE IMAGE-SIZE : ")
  (write-out-integer (large-image-size))
  (write-out-newline)
  (write-out-string "WEAKS IMAGE-SIZE : ")
  (write-out-integer (weaks-image-size))
  (write-out-newline)
  (write-out-newline)
  (write-out-string "BIBOP-AREAS has : ")
  (write-out-integer (*bibop-areas-obj-nr*))
  (write-out-newline))

(define (show-descriptors stob-table)
  (show-small-descriptors stob-table)
  (show-large-descriptors stob-table)
  (show-weaks-descriptors stob-table))

(define (show-small-descriptors stob-table)
  (do-show-descriptors "SMALL" 
		       (bibop-areas-small-index (get-bibop-areas))
		       (bibop-areas-small (get-bibop-areas))
		       stob-table))

(define (show-large-descriptors stob-table)
  (do-show-descriptors "LARGE"
		       (bibop-areas-large-index (get-bibop-areas))
		       (bibop-areas-large (get-bibop-areas))
		       stob-table))
		       
(define (show-weaks-descriptors stob-table)
  (do-show-descriptors "WEAKS" 
		       (bibop-areas-weaks-index (get-bibop-areas))
		       (bibop-areas-weaks (get-bibop-areas))
		       stob-table))


(define (do-show-descriptors area-size-str area-index area stob-table)
  (write-out-newline)
  (write-out-string area-size-str)
  (write-out-newline)
  (do ((i 0 (+ i 1)))
      ((= i area-index))
    (let* ((stob (vector-ref area i))
	   (image-location (table-ref stob-table stob)))
      (write-out-newline)
      (write-out-integer stob)
      (write-out-string " : ")
      (write-out-integer (image-location-new-descriptor image-location))
      (write-out-newline))))

(define (display-small-image)
  (write-out-newline)
  (write-out-string "SMALL-IMG-DATA")
  (write-out-newline)
  (write-out-string "*small-image-begin* : ")
  (write-out-integer (address->integer *small-image-begin*))
  (write-out-newline)
  (write-out-string "*small-image-hp* : ")
  (write-out-integer (address->integer *small-image-hp*))
  (write-out-newline)
  (write-out-string "*small-image-end* : ")
  (write-out-integer (address->integer *small-image-end*))
  (write-out-newline)
  (write-out-string "PAGES : ")
  (write-out-integer (quotient (address-difference *small-image-end* 
						   *small-image-begin*)
			       4096))
  (write-out-newline))

(define (display-large-image)
  (write-out-newline)
  (write-out-string "LARGE-IMG-DATA")
  (write-out-newline)
  (write-out-string "*large-image-begin* : ")
  (write-out-integer (address->integer *large-image-begin*))
  (write-out-newline)
  (write-out-string "*large-image-hp* : ")
  (write-out-integer (address->integer *large-image-hp*))
  (write-out-newline)
  (write-out-string "*large-image-end* : ")
  (write-out-integer (address->integer *large-image-end*))
  (write-out-newline)
  (write-out-string "PAGES : ")
  (write-out-integer (quotient (address-difference *large-image-end* 
						   *large-image-begin*)
			       4096))
  (write-out-newline))

(define (display-weaks-image)
  (write-out-newline)
  (write-out-string "WEAKS-IMG-DATA")
  (write-out-newline)
  (write-out-string "*weaks-image-begin* : ")
  (write-out-integer (address->integer *weaks-image-begin*))
  (write-out-newline)
  (write-out-string "*weaks-image-hp* : ")
  (write-out-integer (address->integer *weaks-image-hp*))
  (write-out-newline)
  (write-out-string "*weaks-image-end* : ")
  (write-out-integer (address->integer *weaks-image-end*))
  (write-out-newline)
  (write-out-string "PAGES : ")
  (write-out-integer (quotient (address-difference *weaks-image-end* 
						   *weaks-image-begin*)
			       4096))
  (write-out-newline))
