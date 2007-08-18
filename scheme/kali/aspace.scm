
; Aspace slots provided by the VM are:
;  uid              ; unique id of this aspace within the local aspace
;  proxy-vector     ; vector mapping uids to proxies
;  decode-vector    ; vector mapping uids to objects
;  data		    ; catch-all slot for our use
; The first three are the only slots the VM itself uses.

; The DATA slot contains an instance the following record type:

(define-record-type aspace-data :aspace-data
  (make-aspace-data ip-address socket pending-vector lock
		    in-channel out-channel placeholder)
  (ip-address aspace-data-ip-address)
  (socket aspace-data-socket)
  (pending-vector aspace-data-pending-vector set-aspace-data-pending-vector!)
  (lock aspace-data-lock)
  (placeholder aspace-data-placeholder set-aspace-data-placeholder!)
  (in-channel aspace-data-in-channel set-aspace-data-in-channel!)
  (out-channel aspace-data-out-channel set-aspace-data-out-channel!)
  )
  
; Make the aspace-data slots look like address-space slots.
;
; (define-address-space-slot x a-s-x [set-a-s-x!])
;  ->
; (begin (define (a-s-x a)
;          (address-space-data-x (address-space-data a)))
;       [(define (set-a-s-x! a v)
;          (set-address-space-data-x! (address-space-data a) v))]
;        )

(define-syntax define-address-space-slot
  (lambda (e r c)
    (let ((make-name (lambda stuff
		       (string->symbol
			(apply string-append
			       (map symbol->string stuff)))))
	  (slot-name (cadr e)))
      `(begin (define (,(caddr e) a)
		(,(make-name 'aspace-data- slot-name)
		   (address-space-data a)))
	      ,@(if (null? (cdddr e))
		    '()
		    `((define (,(cadddr e) a v)
			(,(make-name 'set-aspace-data- slot-name '!)
		           (address-space-data a)
			   v))))))))

(define-address-space-slot ip-address address-space-ip-address)  ; ip-address
(define-address-space-slot socket     address-space-socket)      ; socket number
(define-address-space-slot lock       address-space-lock)

; vector mapping uids to incomplete objects                
(define-address-space-slot pending-vector
  address-space-pending-vector
  set-address-space-pending-vector!)

(define-address-space-slot placeholder
  address-space-placeholder
  set-address-space-placeholder!)

(define-address-space-slot in-channel
  address-space-in-channel
  set-address-space-in-channel!)

(define-address-space-slot out-channel
  address-space-out-channel
  set-address-space-out-channel!)

;----------------
; Used to break symmetry when making connections.

(define (address-space< aspace1 aspace2)
  (if (= (address-space-socket aspace1)
	 (address-space-socket aspace2))
      (string<=? (address-space-ip-address aspace1)
		 (address-space-ip-address aspace2))
      (< (address-space-socket aspace1)
	 (address-space-socket aspace2))))

; Tables of address spaces - there is only one of these.

(define make-address-space-table
  (make-table-maker (lambda (p1 p2)
		      (and (string=? (car p1) (car p2))
			   (= (cdr p1) (cdr p2))))
		    (lambda (p)
		      (+ (apply * (car p)) (cdr p)))))

; All of the address spaces we know about.

(define *address-space-table* (make-address-space-table))

(define (add-to-address-space-table! aspace)
  (table-set! *address-space-table*
	      (cons (address-space-ip-address aspace)
		    (address-space-socket aspace))
	      aspace))

; Return a list of all `real' address spaces.  We ignore the shared address
; space.

(define (all-address-spaces)
  (with-interrupts-inhibited
   (lambda ()
     (let ((list '()))
       (table-walk (lambda (key value)
		     (if (not (eq? value *shared-address-space*))
			 (set! list (cons value list))))
		   *address-space-table*)
       list))))

(define (socket-id->address-space ip-address-name socket-number)
  (get-address-space ip-address-name socket-number #f))

; If the new address space is not the local one, then encode it so that
; it will have a uid.

(define (get-address-space machine-name socket-number local-aspace?)  
  (let* ((ip-address (if (= socket-number pseudo-aspace-socket-number)
			 machine-name
			 (machine-name->ip-address machine-name)))
	 (key (cons ip-address socket-number)))
    (or (table-ref *address-space-table* key)
	(let ((aspace (make-address-space (if local-aspace? 1 #f)  ; uid
					  (make-vector 2)       ; decode vector
					  (make-vector 1 1)     ; proxy vector
					  (make-aspace-data
					     ip-address
					     socket-number
					     (make-vector 2)    ; pending vector
					     (make-lock)
					     #f                 ; in-channel
					     #f                 ; out-channel
					     #f))))		; placeholder
	  (vector-set! (address-space-decode-vector aspace) 0 2) ; next uid
	  (vector-set! (address-space-decode-vector aspace) 1 aspace)
	  (table-set! *address-space-table* key aspace)
	  (if (not local-aspace?)
	      (encode aspace *local-address-space*))
	  aspace))))

; Our local address space.

(define *local-address-space* #f)

(define (local-address-space)
  (or *local-address-space*
      (error "no server has been started")))

; Initialize the server, using SOCKET-NUMBER as our socket.

(define (initialize-local-address-space! socket-number)
  (if *local-address-space*
      (error "server already started"))
  (let ((my-machine (get-host-name)))
    (set! *address-space-table* (make-address-space-table))
    (if *shared-address-space*
	(add-to-address-space-table! *shared-address-space*))
    (let ((local (get-address-space my-machine socket-number #t)))
      (set! *local-address-space* local)
      (set-address-space-decode-vector! local
					(vector 3
						local
						*shared-address-space*))
      (set-address-space-pending-vector! local (vector #f #f #f)))))


;; get date and time
(define (time-and-date-string)
  (time->string (current-time)))

; Returns the ip address of MACHINE-NAME as a string, for example "12.14.15.16".

;; chnx TODO
(define (machine-name->ip-address machine-name)
  (let ((ip-assoc-list (list (cons "chimaera" "127.0.0.1")
			     (cons "127.0.0.1" "127.0.0.1")
			     ;(cons "chimaera" "192.168.0.127")
			     (cons "192.168.0.127" "192.168.0.127"))))
    (cdr (assoc machine-name
		ip-assoc-list))))

;----------------
; A common, pseudo address space (it has no socket) whose uids can be shared
; between servers starting from the same image.  This avoids the need for
; every server to get identical copies of every template from every server it
; talks to.

(define *shared-address-space* #f)

(define (initialize-shared-address-space!)
  (set! *shared-address-space* #f)
  (collect)
  (let ((aspace (make-template&location-address-space
		 (string-append "system:"
				(machine-name->ip-address (get-host-name))
				":"
				(time-and-date-string))
		 shared-address-space-uid)))
    (set! *shared-address-space* aspace)))

; This must be > 0 for the address-space-table's hashing to work.
(define pseudo-aspace-socket-number 1)

(define shared-address-space-uid 2)

; Finds all of the templates, locations, and proxies in the heap and put them
; into a address space (that has no socket).  This address space is saved in
; the image and thus shared between all `real' address spaces, effectively
; giving all of these templates, locations, and proxies unique global ids.

(define (make-template&location-address-space name aspace-uid)
  (let ((proxy-datas (find-all (enum stob proxy-data)))
	(locations   (find-all (enum stob location)))
	(templates   (find-all (enum stob template))))
    (let ((decode-vector (make-vector (+ 2
					 (vector-length templates)
					 (vector-length locations))))
	  (proxy-vector (make-vector (+ 1 (vector-length proxy-datas)))))
      (let ((aspace (make-address-space aspace-uid
					decode-vector
					proxy-vector
					(make-aspace-data
					     name
					     pseudo-aspace-socket-number
					     '#()		; pending vector
					     (make-lock)
					     #f                 ; in-channel
					     #f                 ; out-channel
					     #f))))		; placeholder
	(vector-set! decode-vector 0 (vector-length decode-vector)) ; freelist
	(vector-set! decode-vector 1 aspace)	; magic self pointer
	(copy-uid-elements! locations
			    decode-vector
			    2
			    (lambda (location i)
			      (set-location-uid! location (cons aspace-uid i))))
	(copy-uid-elements! templates
			    decode-vector
			    (+ 2 (vector-length locations))
			    (lambda (template i)
			      (set-template-id! template (cons aspace-uid i))))
	(vector-set! proxy-vector 0 (vector-length proxy-vector)) ; freelist
	(copy-uid-elements! proxy-datas
			    proxy-vector
			    1
			    (lambda (proxy-data i)
			      (set-proxy-data-uid! proxy-data i)
			      (set-proxy-data-owner! proxy-data aspace)))
	(table-set! *address-space-table*
		    (cons name pseudo-aspace-socket-number)
		    aspace)
	aspace))))

(define (copy-uid-elements! from to start set-uid!)
  (do ((i 0 (+ i 1)))
      ((= i (vector-length from)))
    (let ((element (vector-ref from i))
	  (id (+ i start)))
      (vector-set! to id element)
      (set-uid! element id))))


