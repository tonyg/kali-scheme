; Copyright (c) 1993 by Richard Kelsey and Jonathan Rees.  See file COPYING.


; Stuff moved from segment.scm  6/5/93


; Debug-data records are for communicating information from the
; compiler to various debugging tools.

; Entries in an environment-maps list have the form
;   #(parent-uid pc-in-parent (env-map ...))

(define-record-type debug-data debug-data-type
  (make-debug-data uid name parent pc-in-parent env-maps source)
  debug-data?
  (uid      debug-data-uid)
  (name	    debug-data-name)
  (parent   debug-data-parent)
  (pc-in-parent debug-data-pc-in-parent)
  (env-maps debug-data-env-maps set-debug-data-env-maps!)
  (source   debug-data-source set-debug-data-source!))

(define (new-debug-data name parent pc-in-parent)
  (make-debug-data (new-template-uid) name parent pc-in-parent '() '()))

(define-record-discloser debug-data-type
  (lambda (dd)
    (list 'debug-data (debug-data-uid dd) (debug-data-name dd))))


; "Info" means either a debug data record or an integer index into a
; table of same.  An "info" is stored in a reserved place in every
; template.

(define (debug-data->info debug-data)
  (if (interesting-debug-data? debug-data)
      (if (tabulate-debug-data?)
	  (begin (note-debug-data! debug-data)
		 (debug-data-uid debug-data))
	  debug-data)
      (debug-data-uid debug-data)))	;+++

(define (get-debug-data info)		;info->debug-data
  (cond ((debug-data? info) info)
	((integer? info)
	 (table-ref (debug-data-table) info))
	(else #f)))

(define (note-debug-data! dd)
  (table-set! (debug-data-table) (debug-data-uid dd) dd))

(define (interesting-debug-data? debug-data)
  (and (debug-data? debug-data)
       (or (debug-data-name debug-data)
	   (interesting-debug-data? (debug-data-parent debug-data))
	   (not (null? (debug-data-env-maps debug-data)))
	   (not (null? (debug-data-source debug-data))))))

; We can follow parent links to get a full description of procedure
; nesting: "foo in bar in unnamed in baz"

(define (debug-data-names info)
  (let ((dd (get-debug-data info)))
    (if dd
	(cons (debug-data-name dd)
	      (debug-data-names (debug-data-parent dd)))
	'())))


; Associating names with templates

(define (template-debug-data tem)
  (get-debug-data (template-info tem)))

(define (template-id tem)
  (let ((info (template-info tem)))
    (if (debug-data? info)
	(debug-data-uid info)
	info)))

(define (template-name tem)
  (let ((probe (template-debug-data tem)))
    (if probe
	(debug-data-name probe)
	#f)))

(define (template-names tem)
  (debug-data-names (template-info tem)))
