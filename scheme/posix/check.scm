; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; A test suite for the POSIX interface.

(define-test-suite posix-core-tests)

; 1. get the process ID
; 2. make a /tmp/s48-posix-test-<pid> directory
; 3. go there and make files, etc.

(define initial-wd (working-directory))

; doesn't work on Mac OS X

(define directory-name
  (string-append "/tmp/s48-posix-test-"
		 (number->string (process-id->integer (get-process-id)))))

(define-test-case file-mode-predicates posix-core-tests
  (let ((mode0 (file-mode set-uid owner-read group-write other-exec))
	(mode1 (file-mode set-uid))
	(mode2 (file-mode owner-read group-write))
	(mode3 (file-mode set-uid other-exec)))
    
    (check (file-mode? mode0))
    (check (not (file-mode? 'mode0)))

    (check (file-mode=? mode0 mode0))
    (check (not (file-mode=? mode0 mode1)))

    (check (file-mode<=? mode0 mode0))
    (check (not (file-mode<=? mode0 mode1)))
    (check (file-mode<=? mode1 mode0))
    
    (check (file-mode>=? mode0 mode0))
    (check (file-mode>=? mode0 mode1))
    (check (not (file-mode>=? mode1 mode0)))

    (for-each (lambda (x)
		(check (file-mode=? x
				    (integer->file-mode
				     (file-mode->integer x)))))
	      (list mode0 mode1 mode2 mode3))))
	      
(define-test-case file-modes posix-core-tests
  (let ((mode0 (file-mode set-uid owner-read group-write other-exec))
	(mode1 (file-mode set-uid))
	(mode2 (file-mode owner-read group-write))
	(mode3 (file-mode set-uid other-exec)))

    (check (file-mode->integer mode0) => #o4421)
    (check (file-mode->integer mode1) => #o4000)

    (check (file-mode->integer (file-mode+)) => #o0000)
    (check (file-mode->integer (file-mode+ mode1)) => #o4000)
    (check (file-mode->integer (file-mode+ mode1 mode2 mode3)) => #o4421)
     
    (check (file-mode->integer (file-mode- mode0 mode3)) => #o0420)
     
    (check (file-mode->integer (file-mode other-exec)) => 1)
    (check (file-mode->integer (file-mode other-write)) => 2)
    (check (file-mode->integer (file-mode other-read)) => 4)
    (check (file-mode->integer (file-mode group-exec)) => 8)
    (check (file-mode->integer (file-mode group-write)) => 16)
    (check (file-mode->integer (file-mode group-read)) => 32)
    (check (file-mode->integer (file-mode owner-exec)) => 64)
    (check (file-mode->integer (file-mode owner-write)) => 128)
    (check (file-mode->integer (file-mode owner-read)) => 256)
    (check (file-mode->integer (file-mode set-gid)) => 1024)
    (check (file-mode->integer (file-mode set-uid)) => 2048)

    (check (file-mode->integer (file-mode other)) => 7)
    (check (file-mode->integer (file-mode group)) => 56)
    (check (file-mode->integer (file-mode owner)) => 448)
    (check (file-mode->integer (file-mode exec)) => 73)
    (check (file-mode->integer (file-mode write)) => 146)
    (check (file-mode->integer (file-mode read)) => 292)
    (check (file-mode->integer (file-mode all)) => 511)))

(define-test-case make-directory posix-core-tests
  (check (begin
	   (make-directory directory-name (integer->file-mode #o700))
	   (file-info-type (get-file-info directory-name)))
	 => (file-type directory)))

(define-test-case time posix-core-tests
  (sleep 3000)				; three seconds
  (let ((now (current-time))
	(dir-time (file-info-last-modification
		   (get-file-info directory-name))))
    (check (time? now))
    (check (time? dir-time))
    (check (not (time? 'now)))
    (check (not (time? 20)))

    (check (time=? now now))
    (check (not (time=? now dir-time)))
    (check (not (time=? dir-time now)))
    (check (time=? dir-time dir-time))

    (check (not (time<? now now)))
    (check (not (time<? now dir-time)))
    (check (time<? dir-time now))
    (check (not (time<? dir-time dir-time)))

    (check (not (time>? now now)))
    (check (time>? now dir-time))
    (check (not (time>? dir-time now)))
    (check (not (time>? dir-time dir-time)))

    (check (time<=? now now))
    (check (not (time<=? now dir-time)))
    (check (time<=? dir-time now))
    (check (time<=? dir-time dir-time))

    (check (time>=? now now))
    (check (time>=? now dir-time))
    (check (not (time>=? dir-time now)))
    (check (time>=? dir-time dir-time))

    (check (time-seconds now) => (time-seconds now))
    (check (not (= (time-seconds now) (time-seconds dir-time))))
      
    (check (time=? now (make-time (time-seconds now))))
    (check (not (time=? now (make-time (time-seconds dir-time)))))
    (check (not (time=? dir-time (make-time (time-seconds now)))))
    (check (time=? dir-time (make-time (time-seconds dir-time))))

    (check (string? (time->string now)))))

(define-test-case set-working-directory! posix-core-tests
  (set-working-directory! directory-name)
  ;; On Mac OS X, /tmp is soft-linked to /private/tmp
  (let ((normalized-wd (os-string->string (working-directory))))
    (set-working-directory! normalized-wd)
    (check (os-string->string (working-directory)) => normalized-wd)))

(define-test-case i/o-flags posix-core-tests
  (let* ((out (open-file "file0"
			 (file-options create write-only)
			 (integer->file-mode #o700)))
	 (flags (i/o-flags out)))
    (display "123456" out)
    (newline out)
    (close-output-port out)
    (check (not (file-options-on? flags (file-options append))))
    (check (not (file-options-on? flags (file-options synchronized-data))))
    (check (file-options-on? flags (file-options nonblocking)))
    (check (not (file-options-on? flags (file-options synchronized-read))))
    (check (not (file-options-on? flags (file-options synchronized))))
    (check (not (file-options-on? flags (file-options read-only))))
    (check (not (file-options-on? flags (file-options read-write))))
    (check (file-options-on? flags (file-options write-only)))))

(define-test-case append-mode posix-core-tests
  (let* ((old-size (file-info-size (get-file-info "file0")))
	 (out (open-file "file0"
			 (file-options append write-only))))
    (display "123456" out)
    (newline out)
    (close-output-port out)
    (check old-size => 7)
    (check (file-info-size (get-file-info "file0")) => 14)))

(define-test-case file-times posix-core-tests
  (let ((old-info (get-file-info "file0")))
    (sleep 3000)			; three seconds
    (let ((in (open-file "file0"
			 (file-options read-only))))
      (read-char in)
      (close-input-port in))
    (let ((new-info (get-file-info "file0")))
      (check (time=? (file-info-last-modification old-info)
		     (file-info-last-modification new-info)))
      (check (not (time=? (file-info-last-access old-info)
			  (file-info-last-access new-info))))
      (check (time<? (file-info-last-access old-info)
		     (file-info-last-access new-info))))))

(define-test-case link posix-core-tests
  (let ((old-link-count (file-info-link-count (get-file-info "file0"))))
    (link "file0" "link-to-file0")
    (check old-link-count => 1) 
    (check (file-info-link-count (get-file-info "file0")) => 2)))

(define-test-case rename posix-core-tests
  (let ((inode (file-info-inode (get-file-info "file0"))))
    (rename "file0" "file1")
    (check (file-info-inode (get-file-info "file1"))
	   => inode)))

(define-test-case listings0 posix-core-tests
  (let ((directory (open-directory-stream directory-name)))
    (let loop ((names '()))
      (let ((next (read-directory-stream directory)))
	(if next
	    (loop (cons next names))
	    (begin
	      (close-directory-stream directory)
	      (check
	       (sort-list (map os-string->string names) string<=?)
	       => '("file1" "link-to-file0"))))))))
 
(define-test-case listings1 posix-core-tests
  (check (sort-list (map os-string->string (list-directory ".")) string<=?)
	 => '("file1" "link-to-file0")))

(define-test-case unlink posix-core-tests
  (unlink "link-to-file0")
  (check (file-info-link-count (get-file-info "file1")) => 1))

(define-test-case umask posix-core-tests
  (let* ((old-mask (set-file-creation-mask! (integer->file-mode #o012)))
	 (out (open-file "umask-file"
			 (file-options create write-only)
			 (integer->file-mode #o777))))
    (display "123456" out)
    (newline out)
    (close-output-port out)
    (let* ((my-mask (set-file-creation-mask! old-mask))
	   (file-mode (file-info-mode (get-file-info "umask-file"))))
      (check (file-mode->integer my-mask) => #o012)
      (check (file-mode->integer file-mode) => #o765))))

; This assumes that we are not running as root and that / is owned by root.

(define-test-case users&groups posix-core-tests
  (let ((my-info (get-file-info directory-name))
	(root-info (get-file-info "/")))
    (let ((my-user (user-id->user-info (file-info-owner my-info)))
	  (root-user (user-id->user-info (file-info-owner root-info)))
	  (my-group (group-id->group-info (file-info-group my-info)))
	  (root-group (group-id->group-info (file-info-group root-info))))
      (let ((my-other-user (name->user-info (user-info-name my-user)))
	    (my-other-group (name->group-info (group-info-name my-group))))
	(check (user-id=? (file-info-owner my-info)
			  (user-info-id my-user)))
	(check (not (user-id=? (file-info-owner root-info)
			       (user-info-id my-user))))
	(check (group-id=? (file-info-group my-info)
			   (group-info-id my-group)))
	;; doesn't work reliably
	;; (specifically, if the user is member of wheel)
	;; (check (not (group-id=? (file-info-group root-info)
	;;		(group-info-id my-group))))
	(check (os-string->string (user-info-name root-user))
	       => "root")))))

(define-test-case environment posix-core-tests
  (let ((env (reverse (environment-alist))))
    (check (if (null? env)
	       #t
	       (string=? (os-string->string (cdar env))
			 (lookup-environment-variable->string (caar env)))))
    (for-each (lambda (x)
		(check
		 (and (pair? x)
		      (os-string? (car x))
		      (os-string? (cdr x)))))
	      env))
  (check (not (lookup-environment-variable->string "="))))

; This should be last, because it removes the directory.

(define-test-case rmdir posix-core-tests
  (let ((before (accessible? directory-name (access-mode exists))))
    (for-each unlink (list-directory "."))
    (set-working-directory! initial-wd)
    (remove-directory directory-name)
    (check before)
    (check (not (accessible? directory-name (access-mode exists))))))
