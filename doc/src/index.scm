; Program to process index entries.

; Script for running this in Scheme 48
;
;  ,open tables sort define-record-types util big-util
;  (load "index.scm")
;  (process-index "manual.idx" "index.tex")

(define (process-index infile outfile)
  (let ((entries (call-with-input-file infile read-entries)))
    (call-with-output-file outfile
      (lambda (out)
	(write-index (sort-list entries entry<?)
		     out)))))

; Entry records.

(define-record-type entry :entry
  (make-entry key font main? page)
  entry?
  (key   entry-key)
  (font  entry-font)
  (main? entry-main?)
  (page  entry-page))

(define (entry<? x y)
  (let ((key1 (entry-key x))
	(key2 (entry-key y)))
    (or (string<? key1 key2)
	(and (string=? key1 key2)
	     (or (and (entry-main? x)
		      (not (entry-main? y)))
		 (and (eq? (entry-main? x)
			   (entry-main? y))
		      (< (entry-page x)
			 (entry-page y))))))))

;----------------------------------------------------------------
; Read a file of index entries.

(define (read-entries in)
  (let loop ((entries '()))
    (let ((next (read-char in)))
      (cond ((eof-object? next)
	     (reverse entries))
	    ((char=? next #\\)
	     (loop (cons (read-entry in)
			 entries)))
	    (else
	     (error "missing initial \\ in index entry" in))))))
    
; An *.idx entry looks like
; \indexentry{define-structure!tt!main}{12}
; We need to watch for {...} in the name, as they may be used to escape !'s.
; The caller has eaten the initial \.

(define (read-entry in)
  (skip-past #\{ in)
  (let* ((key (read-field in))
	 (font (read-field in))
	 (main? (string=? (read-field in) "main")))
    (read-char in)	; skip #\{
    (let ((page (string->number (read-field in))))
      (skip-past #\newline in)
      (make-entry key font main? page))))

; Read from IN until you see CHAR.

(define (skip-past char in)
  (let ((ch (read-char in)))
    (cond ((eof-object? ch)
	   (error "premature eof when looking for" char))
	  ((not (char=? ch char))
	   (skip-past char in)))))

; Gather chars until an unnested ! or #\} is seen.

(define (read-field in)
  (let loop ((chars '()) (depth 0))
    (let ((next (read-char in)))
      (if (eof-object? next)
	  (error "premature eof when reading field"
		 (list->string (reverse chars)))
	  (let ((next (char-downcase next)))
	    (case next
	      ((#\!)
	       (if (= depth 0)
		   (list->string (reverse chars))
		   (loop (cons next chars) depth)))
	      ((#\})
	       (if (= depth 0)
		   (list->string (reverse chars))
		   (loop chars (- depth 1))))
	      ((#\{)
	       (loop chars (+ depth 1)))
	      (else
	       (loop (cons next chars) depth))))))))

;----------------------------------------------------------------
; Writing the entries back out again.

(define (write-index entries out)
  (let loop ((entries entries))
    (if (not (null? entries))
	(mvlet (((key-entries entries)
		   (collect-key-entries entries)))
	  (process-key key-entries out)
	  (loop entries)))))

(define (collect-key-entries entries)
  (let ((key (entry-key (car entries))))
    (let loop ((entries (cdr entries))
	       (res (list (car entries))))
      (if (or (null? entries)
	      (not (string=? key
			     (entry-key (car entries)))))
	  (values (reverse res) entries)
	  (loop (cdr entries)
		(cons (car entries) res))))))

(define (process-key entries out)
  (if (not (consistent? entries))
      (error "inconsistent entries" entries))
  (let ((entry (car entries)))
    (let ((key (entry-key entry))
	  (font (entry-font entry))
	  (pages (remove-duplicates (map entry-page entries))))
      (if (entry-main? entry)
	  (write-entries key font (car pages) (cdr pages) out)
	  (write-entries key font #f pages out)))))

(define (consistent? entries)
  (let ((x (car entries)))
    (let ((key (entry-key x))
          (font (entry-font x)))
      (every (lambda (x)
	       (and (string=? key (entry-key x))
		    (or (string=? font (entry-font x))
			;; different entries for t and #t aren't inconsistent
			(string=? font "sharp")
			(string=? (entry-font x) "sharp"))
		    ;(eq? aux (entry-main/aux x))
		    ))
	     (cdr entries)))))

(define last-key "%")
(define s1 (string-append "\\item{" (list->string '(#\\))))
(define s2 "{")
(define s3 "}}{\\hskip .75em}")
(define semi "; ")
(define comma ", ")

(define (write-entries key font main pages p)
  (if (and (char-alphabetic? (string-ref key 0))
           (not (char=? (string-ref last-key 0)
                        (string-ref key 0))))
      (begin (display "\\indexspace" p)
             (newline p)))
  (set! last-key key)
  (display (string-append s1 font s2 key s3) p)
  (if main
      (begin (write main p)
             (if (not (null? pages))
                 (display semi p))))
  (if (not (null? pages))
      (begin (write (car pages) p)
             (for-each (lambda (page)
                         (display comma p)
                         (write page p))
                       (cdr pages))))
  (newline p))
