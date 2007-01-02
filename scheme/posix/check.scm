; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; A test suite for the POSIX interface.

; ,config ,load =scheme48/debug/test.scm
; ,exec ,load =scheme48/posix/check.scm
; ,exec (done)

(load-package 'testing)

(config '(run 
(define-structure posix-test (export)
  (open scheme testing sort threads
	util		; every
	posix-files
	posix-time
	posix-users
	posix-i/o
	posix-process-data
	posix-processes
	os-strings)
  (begin

(define-syntax xtest
  (syntax-rules ()
    ((xtest stuff ...) #f)))

; 1. get the process ID
; 2. make a /tmp/s48-posix-test-<pid> directory
; 3. go there and make files, etc.

(define initial-wd (working-directory))

(define directory-name
  (string-append "/tmp/s48-posix-test-"
		 (number->string (process-id->integer (get-process-id)))))

(test "file-mode-predicates" equal? '((#t #f)
				      (#t #f)
				      (#t #f #t)
				      (#t #t #f)
				      (#t #t #t #t))
      (let ((mode0 (file-mode set-uid owner-read group-write other-exec))
	    (mode1 (file-mode set-uid))
	    (mode2 (file-mode owner-read group-write))
	    (mode3 (file-mode set-uid other-exec)))
	(list (list (file-mode? mode0)
		    (file-mode? 'mode0))
	      (list (file-mode=? mode0 mode0)
		    (file-mode=? mode0 mode1))
	      (list (file-mode<=? mode0 mode0)
		    (file-mode<=? mode0 mode1)
		    (file-mode<=? mode1 mode0))
	      (list (file-mode>=? mode0 mode0)
		    (file-mode>=? mode0 mode1)
		    (file-mode>=? mode1 mode0))
	      (map (lambda (x)
		     (file-mode=? x
				  (integer->file-mode
				   (file-mode->integer x))))
		   (list mode0 mode1 mode2 mode3)))))
	      
(test "file-modes" equal? '((#o4421 #o4000)
			    (#o0000 #o4000 #o4421)
			    (#o0420)
			    (1 2 4 8 16 32 64 128 256 1024 2048)
			    (7 56 448 73 146 292 511))
      (let ((mode0 (file-mode set-uid owner-read group-write other-exec))
	    (mode1 (file-mode set-uid))
	    (mode2 (file-mode owner-read group-write))
	    (mode3 (file-mode set-uid other-exec)))
	(map (lambda (l)
	       (map file-mode->integer l))
	     (list (list mode0 mode1)
		   (list (file-mode+)
			 (file-mode+ mode1)
			 (file-mode+ mode1 mode2 mode3))
		   (list (file-mode- mode0 mode3))
		   (list (file-mode other-exec)
			 (file-mode other-write)
			 (file-mode other-read)
			 (file-mode group-exec)
			 (file-mode group-write)
			 (file-mode group-read)
			 (file-mode owner-exec)
			 (file-mode owner-write)
			 (file-mode owner-read)
			 (file-mode set-gid)
			 (file-mode set-uid))
		   (list (file-mode other)
			 (file-mode group)
			 (file-mode owner)
			 (file-mode exec)
			 (file-mode write)
			 (file-mode read)
			 (file-mode all))))))

(test "make-directory" eq? (file-type directory)
      (begin
	(make-directory directory-name (integer->file-mode #o700))
	(file-info-type (get-file-info directory-name))))

(test "time" equal? '((#t #t #f #f)
		      (#t #f #f #t)  ; =
		      (#f #f #t #f)  ; <
		      (#f #t #f #f)  ; >
		      (#t #f #t #t)  ; <=
		      (#t #t #f #t)  ; >=
		      (#t #f)
		      (#t #f #f #t)  ; =
		      #t)
      (begin
	(sleep 3000)			; three seconds
	(let ((now (current-time))
	      (dir-time (file-info-last-modification
			   (get-file-info directory-name))))
	  (let ((times1 (list now now dir-time dir-time))
		(times2 (list now dir-time now dir-time)))
	    (list (list (time? now)
			(time? dir-time)
			(time? 'now)
			(time? 20))
		  (map time=? times1 times2)
		  (map time<? times1 times2)
		  (map time>? times1 times2)
		  (map time<=? times1 times2)
		  (map time>=? times1 times2)
		  (list (= (time-seconds now)
			   (time-seconds now))
			(= (time-seconds now)
			   (time-seconds dir-time)))
		  (map time=?
		       times1
		       (map (lambda (t)
			      (make-time (time-seconds t)))
			    times2))
		  (string? (time->string now)))))))

(test "set-working-directory!" string=? directory-name
      (begin
	(set-working-directory! directory-name)
	(os-string->string (working-directory))))

(test "i/o-flags" equal? '(#f #f #t #f #f #f #f #t)
      (let* ((out (open-file "file0"
			     (file-options create write-only)
			     (integer->file-mode #o700)))
	     (flags (i/o-flags out)))
	(display "123456" out)
	(newline out)
	(close-output-port out)
	(list (file-options-on? flags (file-options append))
	      (file-options-on? flags (file-options synchronized-data))
	      (file-options-on? flags (file-options nonblocking))
	      (file-options-on? flags (file-options synchronized-read))
	      (file-options-on? flags (file-options synchronized))
	      (file-options-on? flags (file-options read-only))
	      (file-options-on? flags (file-options read-write))
	      (file-options-on? flags (file-options write-only)))))

(test "append mode" equal? '(7 14)
      (let* ((old-size (file-info-size (get-file-info "file0")))
	     (out (open-file "file0"
			     (file-options append write-only))))
	(display "123456" out)
	(newline out)
	(close-output-port out)
	(list old-size
	      (file-info-size (get-file-info "file0")))))

(test "file times" equal? '(#t #f #t)
      (let ((old-info (get-file-info "file0")))
	(sleep 3000)			; three seconds
	(let ((in (open-file "file0"
			     (file-options read-only))))
	  (read-char in)
	  (close-input-port in))
	(let ((new-info (get-file-info "file0")))
	  (list (time=? (file-info-last-modification old-info)
			(file-info-last-modification new-info))
		(time=? (file-info-last-access old-info)
			(file-info-last-access new-info))
		(time<? (file-info-last-access old-info)
			(file-info-last-access new-info))))))

(test "link" equal? '(1 2)
      (let ((old-link-count (file-info-link-count (get-file-info "file0"))))
	(link "file0" "link-to-file0")
	(list old-link-count
	      (file-info-link-count (get-file-info "file0")))))


(test "rename" eq? #t
      (let ((inode (file-info-inode (get-file-info "file0"))))
	(rename "file0" "file1")
	(= inode
	   (file-info-inode (get-file-info "file1")))))

(test "listings0" equal? '("file1" "link-to-file0")
      (let ((directory (open-directory-stream directory-name)))
	(let loop ((names '()))
	  (let ((next (read-directory-stream directory)))
	    (if next
		(loop (cons next names))
		(begin
		  (close-directory-stream directory)
		  (sort-list (map os-string->string names) string<=?)))))))
 
(test "listings1" equal? '("file1" "link-to-file0")
      (sort-list (map os-string->string (list-directory ".")) string<=?))

(test "unlink" = 1
      (begin
	(unlink "link-to-file0")
	(file-info-link-count (get-file-info "file1"))))

(test "umask" equal? '(#o012 #o765)
      (let* ((old-mask (set-file-creation-mask! (integer->file-mode #o012)))
	     (out (open-file "umask-file"
			     (file-options create write-only)
			     (integer->file-mode #o777))))
	(display "123456" out)
	(newline out)
	(close-output-port out)
	(let* ((my-mask (set-file-creation-mask! old-mask))
	       (file-mode (file-info-mode (get-file-info "umask-file"))))
	  (list (file-mode->integer my-mask)
		(file-mode->integer file-mode)))))

; This assumes that we are not running as root and that / is owned by root.

(test "users & groups" equal? '(#t #f #t #f "root")
      (let ((my-info (get-file-info directory-name))
	    (root-info (get-file-info "/")))
	(let ((my-user (user-id->user-info (file-info-owner my-info)))
	      (root-user (user-id->user-info (file-info-owner root-info)))
	      (my-group (group-id->group-info (file-info-group my-info)))
	      (root-group (group-id->group-info (file-info-group root-info))))
	  (let ((my-other-user (name->user-info (user-info-name my-user)))
		(my-other-group (name->group-info (group-info-name my-group))))
	    (list (user-id=? (file-info-owner my-info)
			     (user-info-id my-user))
		  (user-id=? (file-info-owner root-info)
			     (user-info-id my-user))
		  (group-id=? (file-info-group my-info)
			      (group-info-id my-group))
		  (group-id=? (file-info-group root-info)
			      (group-info-id my-group))
		  (os-string->string (user-info-name root-user)))))))

(test "environment" equal? '(#t #t #f)
      (let ((env (reverse (environment-alist))))
	(list (if (null? env)
		  #t
		  (string=? (os-string->string (cdar env))
			    (lookup-environment-variable->string (caar env))))
	      (every (lambda (x)
		       (and (pair? x)
			    (os-string? (car x))
			    (os-string? (cdr x))))
		     env)
	      (lookup-environment-variable->string "="))))

; This should be last, because it removes the directory.

(test "rmdir" equal? '(#t #f)
      (let ((before (accessible? directory-name (access-mode exists))))
	(for-each unlink (list-directory "."))
	(set-working-directory! initial-wd)
	(remove-directory directory-name)
	(list before
	      (accessible? directory-name (access-mode exists)))))
))))

(load-package 'posix-test)

(config '(run 
(define-structure regexp-test (export)
  (open scheme testing
	regexps)
  (begin

(test "any-match?" equal? '(#t #f #t)
      (list
       (any-match? (text "abc") "abc")
       (any-match? (text "abc") "abx")
       (any-match? (text "abc") "xxabcxx")))
       
(test "exact-match" equal? '(#t #f #f)
      (list
       (exact-match? (text "abc") "abc")
       (exact-match? (text "abc") "abx")
       (exact-match? (text "abc") "xxabcxx")))

(define (pair-match exp string)
  (let ((res (match exp string))) 
    (and res
	 (cons (list (match-start res)
		     (match-end res))
	       (map (lambda (p)
		      (cons (car p)
			    (list (match-start (cdr p))
				  (match-end (cdr p)))))
		    (match-submatches res))))))

(test "match" equal? '(((0 3))
		       #f
		       ((2 5))
		       ((3 9) (foo 5 7))
		       ((3 6) (bar 4 6)))
      (list
       (pair-match (text "abc") "abc")
       (pair-match (text "abc") "abx")
       (pair-match (text "abc") "xxabcxx")
       (pair-match (sequence (text "ab")
			     (submatch 'foo (text "cd"))
			     (text "ef"))
		   "xxxabcdefxx")
       (pair-match (sequence (set "a")
			     (one-of (submatch 'foo (text "bc"))
				     (submatch 'bar (text "BC"))))
		   "xxxaBCd")))
))))

(load-package 'regexp-test)

; All done.

(if (in 'testing '(run (lost?)))
    (display "Some tests failed.")
    (display "All tests succeeded."))
(newline)

(define (done)
  (exit (if (in 'testing '(run (lost?))) 1 0)))
