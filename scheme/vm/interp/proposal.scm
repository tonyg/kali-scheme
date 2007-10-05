; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Proposals are four-element vectors.  There doesn't seem to be much point
; in making a separate primitive data type for them.  The four values are:
;   in-use?	- boolean value, true if the proposal is the current proposal
;		  for some thread
;   d-log	- a vector of log entries for descriptor reads and writes
;   b-log	- a vector of log entries for single byte reads and writes
;   copy-list	- a list of blocks of bytes to be copied if the proposal
;		  succeeds
;
; Each log is also a vector.  Every log entry has four values:
;   stob	- the STored OBject which is being read and/or written
;   index	- the index within STOB
;   verify	- the value that the proposal assumes will be at INDEX in STOB;
;		  a special marker value is used if the location is written
;		  before it is read
;   value	- the value that should be put at INDEX in STOB when the
;		  proposal succeeds
; Logs may have unused slots after the final entry.  The final entry in a log
; is followed by #f.

; Proposal offsets and accessors.

(define proposal-size 4)
(define proposal-in-use?-index 0)
(define proposal-d-log-index 1)
(define proposal-b-log-index 2)
(define proposal-copy-list-index 3)

(define (proposal-in-use? proposal)
  (vm-vector-ref proposal proposal-in-use?-index))

(define (set-proposal-in-use?! proposal value)
  (vm-vector-set! proposal proposal-in-use?-index value))

(define (proposal-d-log proposal)
  (vm-vector-ref proposal proposal-d-log-index))

(define (proposal-b-log proposal)
  (vm-vector-ref proposal proposal-b-log-index))

(define (proposal-copies proposal)
  (vm-vector-ref proposal proposal-copy-list-index))

(define (set-proposal-copies! proposal value)
  (vm-vector-set! proposal proposal-copy-list-index value))

; Log offsets and accessors.

(define log-entry-size 4)
(define (log-entry-stob   log i) (vm-vector-ref log i))
(define (log-entry-index  log i) (vm-vector-ref log (+ i 1)))
(define (log-entry-value  log i) (vm-vector-ref log (+ i 2)))
(define (log-entry-verify log i) (vm-vector-ref log (+ i 3)))

(define (set-log-entry-value! log i value)
  (vm-vector-set! log (+ i 2) value))

; Block copy accessors.

(define (copy-from copy) (vm-vector-ref copy 0))
(define (copy-to   copy) (vm-vector-ref copy 2))
(define (copy-next copy) (vm-vector-ref copy 5))

(define (copy-from-index copy) (extract-fixnum (vm-vector-ref copy 1)))
(define (copy-to-index   copy) (extract-fixnum (vm-vector-ref copy 3)))
(define (copy-count      copy) (extract-fixnum (vm-vector-ref copy 4)))

;----------------
; A length-one vector containing #f.  This is used when clearing a protocols
; logs after the protocol has committed.

(define *empty-log*)

; Initialize the above registers and trace them when a GC occurs.

(define (initialize-proposals!+gc)
  (set! *empty-log* (vm-make-vector+gc 1))
  (vm-vector-set! *empty-log* 0 false))

(add-gc-root!
  (lambda ()
    (set! *empty-log* (s48-trace-value *empty-log*))))

; Just return the current proposal.

(define-primitive current-proposal ()
  (lambda ()
    (current-proposal))
  return)

; Install a new proposol.  We do a little error checking and set and clear
; the in-use? flags of the incoming and outgoing proposals.

(define-primitive set-current-proposal! (any->)
  (lambda (proposal)
    (cond ((or (false? proposal)
	       (and (vm-vector? proposal)
		    (= proposal-size (vm-vector-length proposal))
		    (false? (proposal-in-use? proposal))))
	   (let ((old (current-proposal)))
	     (if (not (false? old))
		 (set-proposal-in-use?! old false)))
	   (if (not (false? proposal))
	       (set-proposal-in-use?! proposal true))
	   (set-current-proposal! proposal)
	   (goto return unspecific-value))
	  ((vm-eq? proposal (current-proposal))
	   (goto return unspecific-value))
	  (else
	   (raise-exception wrong-type-argument 0 proposal)))))

;----------------
; Add an entry to a current log, increasing the size of the log if necessary.
; VERIFY? is true if we need to verify that VALUE is in STOB at INDEX when
; trying to commit to the proposal.  VERIFY? is false if a write occurs before
; any reads.

(define (add-log-entry+gc proposal-index i stob index value verify?)
  (let* ((proposal (current-proposal))
	 (log-size (vm-vector-length
		    (vm-vector-ref proposal proposal-index))))
    (receive (proposal stob value)
	(if (= i (- log-size 1))
	    (begin
	      (save-temp0! stob)
	      (save-temp1! value)
	      (extend-log!+gc proposal-index log-size)
	      (values (current-proposal)
		      (recover-temp0!)
		      (recover-temp1!)))
	    (values proposal stob value))
      (let ((log (vm-vector-ref proposal proposal-index)))
	(vm-vector-set! log i       stob)
	(vm-vector-set! log (+ i 1) index)
	(vm-vector-set! log (+ i 2) value)
	(vm-vector-set! log (+ i 3) (if verify? value unreleased-value))
	(vm-vector-set! log (+ i 4) false)	; end marker
	value))))

; The new log has twice the number of entries as the old one unless the old
; one has no entries, in which case we make room for four.
; We clear the currently unused portion of the log to avoid GC problems.

(define (extend-log!+gc proposal-index log-size)
  (let* ((new-size (if (= log-size 1)
		       (+ 1 (* log-entry-size 4))
		       (- (* log-size 2) 1)))
	 (new (vm-make-vector+gc new-size))
	 (proposal (current-proposal)))
    (if (< 1 log-size)
	(copy-memory! (address-after-header (proposal-d-log proposal))
		      (address-after-header new)
		      (cells->a-units (- log-size 1))))
    (do ((i (+ log-size log-entry-size) (+ i 1)))
	((= i new-size))
      (vm-vector-set! new i (enter-fixnum 0)))
    (vm-vector-set! proposal proposal-index new)))

; Add a byte-copy-block record to the current proposal.

(define (add-block-copy!+gc from from-index to to-index count)
  (save-temp0! from)
  (save-temp1! to)
  (let* ((entry (vm-make-vector+gc 6))
	 (proposal (current-proposal)))
    (vm-vector-set! entry 0 (recover-temp0!))
    (vm-vector-set! entry 1 from-index)
    (vm-vector-set! entry 2 (recover-temp1!))
    (vm-vector-set! entry 3 to-index)
    (vm-vector-set! entry 4 count)
    (vm-vector-set! entry 5 (proposal-copies proposal))
    (set-proposal-copies! proposal entry)))

;----------------
; Accessors and setters for stored objects.  There are three kinds, with an
; accessor and setter for each.  They are:
;   fixed-sized stored objects (pairs, closures, ...); no range check is done
;    for these
;   variable-sized stored-objects (vectors, records, ...)
;   records with types; these take an additional argument, the record type,
;    which is checked against the value in the record's first slot
;
; For efficency reasons there are two versions of stored-object-ref, one which
; logs the read in the current proposal and one which doesn't.  For all of the
; others the logging/not-logging is determined by a byte in the instruction
; stream.
;
; All of these are mostly argument checks.

(define-primitive stored-object-ref
  (any->)
  (lambda (stob)
    (let ((type (code-byte 0))
	  (offset (code-byte 1)))
      (if (stob-of-type? stob type)
	  (goto continue-with-value
		(d-vector-ref stob offset)
		2)
	  (raise-exception wrong-type-argument 2
			   stob
			   (enter-fixnum type)
			   (enter-fixnum offset))))))

(define-primitive stored-object-logging-ref
  (any->)
  (lambda (stob)
    (let ((type (code-byte 0))
	  (offset (code-byte 1)))
      (if (stob-of-type? stob type)
	  (goto continue-with-value
		(if (false? (current-proposal))
		    (d-vector-ref stob offset)
		    (proposal-d-read stob (enter-fixnum offset)))
		2)
	  (raise-exception wrong-type-argument 2
			   stob
			   (enter-fixnum type)
			   (enter-fixnum offset))))))

(define-primitive stored-object-set!
  (any-> any->)
  (lambda (stob value)
    (let ((type (code-byte 0))
	  (offset (code-byte 1)))
      (cond ((not (stob-of-type? stob type))
	     (raise-exception wrong-type-argument 3
			      stob
			      (enter-fixnum type)
			      (enter-fixnum offset)
			      value))
	    ((immutable? stob)
	     (raise-exception immutable-argument 3
			      stob
			      (enter-fixnum type)
			      (enter-fixnum offset)
			      value))
	    (else
	     (if (or (= 0 (code-byte 2))
		     (false? (current-proposal)))
		 (d-vector-set! stob offset value)
		 (proposal-d-write stob (enter-fixnum offset) value))
	     (goto continue-with-value
		   unspecific-value
		   3))))))

; Indexed objects

(define-primitive stored-object-indexed-ref (any-> any->)
  (lambda (stob index)
    (let ((type (code-byte 0)))
      (cond ((or (not (fixnum? index))
	         (not (stob-of-type? stob type)))
	     (raise-exception wrong-type-argument 2
			      stob (enter-fixnum type) index))
	    ((valid-index? (extract-fixnum index) (d-vector-length stob))
	     (goto continue-with-value
		   (if (or (= 0 (code-byte 1))
			   (false? (current-proposal)))
		       (d-vector-ref stob (extract-fixnum index))
		       (proposal-d-read stob index))
		   2))
	    (else
	     (raise-exception index-out-of-range 2
			      stob (enter-fixnum type) index))))))

(define-primitive stored-object-indexed-set! (any-> any-> any->)
  (lambda (stob index value)
    (let ((type (code-byte 0)))
      (cond ((or (not (fixnum? index))
		 (not (stob-of-type? stob type)))
	     (raise-exception wrong-type-argument 2
			      stob (enter-fixnum type) index value))
	    ((immutable? stob)
	     (raise-exception immutable-argument 2
			      stob (enter-fixnum type) index value))
	    ((valid-index? (extract-fixnum index)
			   (d-vector-length stob))
	     (if (or (= 0 (code-byte 1))
		     (false? (current-proposal)))
		 (d-vector-set! stob (extract-fixnum index) value)
		 (proposal-d-write stob index value))		 
	     (goto continue-with-value unspecific-value 2))
	    (else
	     (raise-exception index-out-of-range 2
			      stob (enter-fixnum type) index value))))))

; Byte vectors

(define-primitive byte-vector-logging-ref (code-vector-> fixnum->)
  (lambda (vector index)
    (if (valid-index? index (code-vector-length vector))
	(goto continue-with-value
	      (if (false? (current-proposal))
		  (enter-fixnum (code-vector-ref vector index))
		  (proposal-b-read vector (enter-fixnum index)))
	      0)
	(raise-exception index-out-of-range 0 vector (enter-fixnum index)))))

(define-primitive byte-vector-logging-set! (code-vector-> fixnum-> any->)
  (lambda (vector index byte)
    (cond ((immutable? vector)
	   (raise-exception immutable-argument 0
			    vector (enter-fixnum index) byte))
	  ((not (fixnum? byte))
	   (raise-exception wrong-type-argument 0
			    vector (enter-fixnum index) byte))
	  ((valid-index? index (code-vector-length vector))
	   (if (false? (current-proposal))
	       (code-vector-set! vector index (extract-fixnum byte))
	       (proposal-b-write vector (enter-fixnum index) byte))
	   (goto continue-with-value unspecific-value 0))
	  (else
	   (raise-exception index-out-of-range 0
			    vector (enter-fixnum index) byte)))))

; Hacko record handlers done for speed.

(define-primitive checked-record-ref
  (any-> any-> fixnum->)
  (lambda (record type index)
    (cond ((not (and (stob-of-type? record (enum stob record))
		     (vm-eq? type (record-ref record 0))))
	   (raise-exception wrong-type-argument 1
			    record type (enter-fixnum index)))
	  ((valid-index? index (record-length record))
	   (goto continue-with-value
		 (if (or (= 0 (code-byte 0))
			 (false? (current-proposal)))
		     (record-ref record index)
		     (proposal-d-read record (enter-fixnum index)))
		 1))
	  (else
	   (raise-exception index-out-of-range 1
			    record type (enter-fixnum index))))))

(define-primitive checked-record-set! (any-> any-> fixnum-> any->)
  (lambda (record type index value)
    (cond ((or (not (stob-of-type? record (enum stob record)))
	       (not (vm-eq? type (record-ref record 0))))
	   (raise-exception wrong-type-argument 1
			    record type (enter-fixnum index) value))
	  ((immutable? record)
	   (raise-exception immutable-argument 1
			    record type (enter-fixnum index) value))
	  ((valid-index? index (record-length record))
	   (if (or (= 0 (code-byte 0))
		   (false? (current-proposal)))
	       (record-set! record index value)
	       (proposal-d-write record (enter-fixnum index) value))
	   (goto continue-with-value unspecific-value 1))
	  (else
	   (raise-exception index-out-of-range 1
			    record type (enter-fixnum index) value)))))

; Get the right log out of the current proposal and look for an entry for
; STOB at INDEX.  If it is there we return the value, otherwise we add a
; new entry with the current value.

(define (proposal-reader proposal-index accessor)
  (lambda (stob index)
    (let ((log (vm-vector-ref (current-proposal) proposal-index)))
      (let loop ((i 0))
	(let ((next-stob (log-entry-stob log i)))
	  (cond ((false? next-stob)
		 (add-log-entry+gc proposal-index
				   i  
				   stob
				   index
				   (accessor stob (extract-fixnum index))
				   #t))
		((and (eq? stob next-stob)
		      (= index (log-entry-index log i)))
		 (log-entry-value log i))
		(else
		 (loop (+ i log-entry-size)))))))))
  
(define proposal-d-read
  (proposal-reader proposal-d-log-index d-vector-ref))

(define proposal-b-read
  (proposal-reader proposal-b-log-index
		   (lambda (b-vector index)
		     (enter-fixnum (b-vector-ref b-vector index)))))

; Make the value at INDEX in STOB be VALUE in the current log.

(define (proposal-writer proposal-index)
  (lambda (stob index value)
    (let ((log (vm-vector-ref (current-proposal) proposal-index)))
      (let loop ((i 0))	     
	(let ((next-stob (log-entry-stob log i)))
	  (cond ((false? next-stob)
		 (add-log-entry+gc proposal-index i stob index value #f)
		 (unspecific))
		((and (eq? stob next-stob)
		      (= index (log-entry-index log i)))
		 (set-log-entry-value! log i value))
		(else
		 (loop (+ i log-entry-size)))))))))

(define proposal-d-write
  (proposal-writer proposal-d-log-index))

(define proposal-b-write
  (proposal-writer proposal-b-log-index))

;----------------
; Mostly error-checking as usual.

(define-primitive copy-bytes! (code-vector-> fixnum-> code-vector-> fixnum-> fixnum->)
  (lambda (from from-index to to-index count)
    (let ((lose (lambda ()
		  (raise-exception wrong-type-argument 1
				   from (enter-fixnum from-index)
				   to (enter-fixnum to-index)
				   (enter-fixnum count))))
	  (no-log? (= 0 (code-byte 0))))
      (cond ((not (and (okay-copy-code-vector? from from-index count)
		       (okay-copy-code-vector? to   to-index   count)
		       (<= 0 count)))
	     (lose))
	    ((immutable? to)
	     (raise-exception immutable-argument 1
			      from (enter-fixnum from-index)
			      to (enter-fixnum to-index)
			      (enter-fixnum count)))
	    ((or no-log?
		 (false? (current-proposal)))
	     (copy-memory! (address+ (address-after-header from) from-index)
			   (address+ (address-after-header to) to-index)
			   count)
	     (goto continue-with-value unspecific-value 1))
	    ((<= count (remaining-block-copying))
	     (add-block-copy!+gc from
				 (enter-fixnum from-index)
				 to
				 (enter-fixnum to-index)
				 (enter-fixnum count))
	     (goto continue-with-value unspecific-value 1))
	    (else
	     (lose))))))

(define maximum-proposal-block-copies 4096)

; Return the number of bytes we have remaining in our block-copy quota.

(define (remaining-block-copying)
  (let loop ((left maximum-proposal-block-copies)
	     (copies (proposal-copies (current-proposal))))
    (if (false? copies)
	left
	(loop (- left (copy-count copies))
	      (copy-next copies)))))

(define (okay-copy-code-vector? c index count)
  (and (<= 0 index)
       (<= (+ index count)
	   (code-vector-length c))))

;----------------
; Committing a proposal.
;
; If the two logs are up-to-date we do all writes and copies and then empty
; the logs.  We return #t if the proposal worked and #f if it didn't.

(define-primitive maybe-commit ()
  (lambda ()
    (let ((proposal (current-proposal)))
      (if (false? proposal)
	  (raise-exception no-current-proposal 0)
	  (begin
	    (get-proposal-lock!)
	    (cond ((and (d-log-in-sync? (proposal-d-log proposal))
			(b-log-in-sync? (proposal-b-log proposal))
			(copies-in-sync? (proposal-copies proposal)))
		   (commit-d-log! (proposal-d-log proposal))
		   (commit-b-log! (proposal-b-log proposal))
		   (do-copies! (proposal-copies proposal))
		   (vm-vector-set! proposal proposal-d-log-index *empty-log*)
		   (vm-vector-set! proposal proposal-b-log-index *empty-log*)
		   (vm-vector-set! proposal proposal-copy-list-index false)
		   (release-proposal-lock!)
		   (set-current-proposal! false)
		   (goto return true))
		  (else
		   (release-proposal-lock!)
		   (set-current-proposal! false)
		   (goto return false))))))))

; Go down LOG checking that each entry is consistent with the current state.
; Write entries require a check that the stob is still mutable.  Read entries
; require a check that the originally seen value is still there.  Read+write
; entries to both.

(define (log-synchronizer accessor)
  (lambda (log)
    (let loop ((i 0))
      (let ((stob (log-entry-stob log i)))
	(if (false? stob)
	    #t
	    (let ((value (log-entry-value log i))
		  (verify (log-entry-verify log i)))
	      (and (if (vm-eq? verify unreleased-value)
		       (not (immutable? stob))
		       (and (vm-eq? verify
				    (accessor stob
					      (extract-fixnum
					       (log-entry-index log i))))
			    (or (vm-eq? verify value)
				(not (immutable? stob)))))
		   (loop (+ i log-entry-size)))))))))

(define d-log-in-sync? (log-synchronizer d-vector-ref))

(define b-log-in-sync?
  (log-synchronizer (lambda (stob index)
		      (enter-fixnum (b-vector-ref stob index)))))

; The only thing to check is that the TO object is still mutable.

(define (copies-in-sync? copies)
  (let loop ((copies copies))
    (cond ((false? copies)
	   #t)
	  ((immutable? (copy-to copies))
	   #f)
	  (else
	   (loop (copy-next copies))))))

; Actually do all writes listed in LOG.

(define (log-committer setter)
  (lambda (log)
    (let loop ((i 0))
      (let ((stob (log-entry-stob log i)))
	(if (not (false? stob))
	    (begin
	      (let ((value (log-entry-value log i))
		    (verify (log-entry-verify log i)))
		(if (not (vm-eq? verify value))
		    (setter stob 
			    (extract-fixnum (log-entry-index log i))
			    value)))
	      (loop (+ i log-entry-size))))))))

(define commit-d-log! (log-committer d-vector-set!))
(define commit-b-log!
  (log-committer (lambda (stob index value)
		   (b-vector-set! stob index (extract-fixnum value)))))

(define (do-copies! copies)
  (let loop ((copies copies))
    (if (not (false? copies))
	(begin
	  (copy-memory! (address+ (address-after-header (copy-from copies))
				  (copy-from-index copies))
			(address+ (address-after-header (copy-to copies))
				  (copy-to-index copies))
			(copy-count copies))
	  (loop (copy-next copies))))))
