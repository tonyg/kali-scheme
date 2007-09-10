; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.

; 5.1 Directories

(import-dynamic-externals "=scheme48external/posix")

; A record for directory streams.  It just has the name and a byte vector
; containing the C directory object.  The name is used only for printing.

(define-record-type directory :directory
  (make-directory-box name c-dir)
  directory-stream?
  (name directory-name)
  (c-dir directory-c-dir set-directory-c-dir!))	;set when the directory is closed

(define-record-discloser :directory
  (lambda (dir)
    (list 'dir (directory-name dir))))

; Directory streams are meaningless in a resumed image.

(define-record-resumer :directory #f)

; Opening, reading, and closing directories.

(define (open-directory-stream name)
  (let ((dir (make-directory-box name
				 (call-imported-binding posix-opendir
							(os-string->byte-vector (x->os-string name))))))
    (add-finalizer! dir close-directory-stream)
    dir))

(define (read-directory-stream directory)
  (cond
   ((call-imported-binding posix-readdir (directory-c-dir directory))
    => x->os-string)
   (else #f)))

(define (close-directory-stream directory)
  (let ((c-dir (directory-c-dir directory)))
    (if c-dir
	(begin
	  (call-imported-binding posix-closedir c-dir)
	  (set-directory-c-dir! directory #f)))))

; The C calls we use.

(import-definition posix-opendir)
(import-definition posix-closedir)
(import-definition posix-readdir)

; The obvious utility.  This returns a list of the names in a directory.

(define (list-directory name)
  (let ((directory (open-directory-stream name)))
    (let loop ((names '()))
      (let ((next (read-directory-stream directory)))
	(if next
	    (loop (cons next names))
	    (begin
	      (close-directory-stream directory)
	      (reverse names)))))))

;----------------
; 5.2 Working Directory

(define (working-directory)
  (x->os-string
   (call-imported-binding posix-working-directory #f)))

(define (set-working-directory! name)
  (call-imported-binding posix-working-directory 
			 (os-string->byte-vector (x->os-string name))))

(import-definition posix-working-directory)

;----------------
; 5.3 File Creation
;
; int open(char *path, int oflag)
; int open(char *path, int oflag, mode_t mode)
;
; The modes are required if the O_CREAT is in oflag, and are only used if
; the file doesn't already exist.

(define (open-file path options . mode)
  (let* ((input? (file-options-on? options (file-options read-only)))
	 (channel (call-imported-binding posix-open
					 (os-string->byte-vector
					  (x->os-string path))
					 options
					 (if (null? mode)
					     #f
					     (car mode))
					 input?)))
    (if input?
	(input-channel->port channel)
	(output-channel->port channel))))

(import-definition posix-open)

; int creat(char *path, int oflag, mode_t mode)  ; redundant with open()
;
; mode_t umask(mode_t cmask)
; Sets the file-mode creation mask, returning the old value.

(define (set-file-creation-mask! new-mask)
  (file-stuff 0 new-mask #f))

; int link(char *existing, char *new)
; Makes `new' be a link to `existing'.

(define (link existing new)
  (file-stuff 1
	      (os-string->byte-vector (x->os-string existing))
	      (os-string->byte-vector (x->os-string new))))

(import-lambda-definition file-stuff (op arg1 arg2) "posix_file_stuff")

;----------------
; 5.4 Special File Creation
;
; int mkdir(char path, mode_t mode)
; int mkfifo(char path, mode_t mode)

(define (make-directory path mode)
  (file-stuff 2 (os-string->byte-vector (x->os-string path)) mode))

(define (make-fifo path mode)
  (file-stuff 3 (os-string->byte-vector (x->os-string path)) mode))

;----------------
; 5.5 File Removal
;
; int unlink(char *path)

(define (unlink path)
  (file-stuff 4 (os-string->byte-vector (x->os-string path)) #f))

; int rmdir(char *path)

(define (remove-directory path)
  (file-stuff 5 (os-string->byte-vector (x->os-string path)) #f))

; int rename(char *old, char *new)

(define (rename old new)
  (file-stuff 6
	      (os-string->byte-vector (x->os-string old))
	      (os-string->byte-vector (x->os-string new))))

;----------------
; The C function posix_file_info() knows the offsets of these fields.

(define-record-type file-info :file-info
  (really-do-not-make-file-info)		; these are made from C
  file-info?
  (name       file-info-name)                   ; for printing
  (type       file-info-type)
  (device     file-info-device)
  (inode      file-info-inode)
  (mode       file-info-mode)
  (link-count file-info-link-count)
  (owner      file-info-owner)
  (group      file-info-group)
  (size       file-info-size)
  (last-access        file-info-last-access)
  (last-modification  file-info-last-modification)
  (last-status-change file-info-last-status-change))

; These are made in C.
(define-exported-binding "posix-file-info-type" :file-info)

; The order of these is known to the C code.
(define-enumerated-type file-type :file-type
  file-type?
  file-types
  file-type-name
  file-type-index
  (regular
   directory
   character-device
   block-device
   fifo
   symbolic-link
   socket
   other))

;----------------
; 5.6 File Characteristics

(import-definition posix-file-info)

; The following are stat(), lstat(), and fstat().

(define (get-file-info name)
  (call-imported-binding posix-file-info
			 (os-string->byte-vector (x->os-string name))
			 #t file-types))

(define (get-file/link-info name)
  (call-imported-binding posix-file-info
			 (os-string->byte-vector (x->os-string name))
			 #f file-types))

(define (get-port-info port)
  (let ((channel (port->channel port)))
    (if channel
	(call-imported-binding posix-file-info channel #f file-types)
	(call-error get-port-info (list port)))))

;----------------
; Modes

(define-record-type file-mode :file-mode
  (really-make-file-mode value)
  file-mode?
  (value file-mode->integer))

(define-record-discloser :file-mode
  (lambda (file-mode)
    (list 'file-mode
	  (string-append "0"
			 (number->string (file-mode->integer file-mode)
					 8)))))

; These are made in C.
(define-exported-binding "posix-file-mode-type" :file-mode)

; STUFF can be a number (#o644), a string ("rwxr--r--"), or ???
; Or should there be another macro?
;
; For now it has to be a number

(define (integer->file-mode stuff)
  (cond ((and (integer? stuff)
	      (<= 0 stuff)
	      (<= stuff #o7777))
	 (really-make-file-mode stuff))
	(else
	 (call-error "argument type error" integer->file-mode stuff))))

; Arithmetic

(define (file-mode+ . modes)
  (do ((i 0 (bitwise-ior i (file-mode->integer (car modes))))
       (modes modes (cdr modes)))
      ((null? modes)
       (integer->file-mode i))))

(define (file-mode- mode1 mode2)
  (integer->file-mode (bitwise-and (file-mode->integer mode1)
				   (bitwise-not (file-mode->integer mode2)))))

; Comparisons

(define (file-mode=? mode1 mode2)
  (= (file-mode->integer mode1)
     (file-mode->integer mode2)))

(define (file-mode<=? mode1 mode2)
  (= 0 (bitwise-and (file-mode->integer mode1)
		    (bitwise-not (file-mode->integer mode2)))))

(define (file-mode>=? mode1 mode2)
  (file-mode<=? mode2 mode1))

; Names for various permissions

(define-syntax file-mode
  (lambda (e r c)
    (let* ((names '((set-uid     . #o4000)
		    (set-gid     . #o2000)

		    (owner-read  . #o0400)
		    (owner-write . #o0200)
		    (owner-exec  . #o0100)
		    (owner       . #o0700)

		    (group-read  . #o0040)
		    (group-write . #o0020)
		    (group-exec  . #o0010)
		    (group       . #o0070)

		    (other-read  . #o0004)
		    (other-write . #o0002)
		    (other-exec  . #o0001)
		    (other       . #o0007)

		    (read        . #o0444)
		    (write       . #o0222)
		    (exec        . #o0111)
		    (all	 . #o0777)))
	   (lookup (lambda (name)
		     (let loop ((names names))
		       (cond ((null? names)
			      #f)
			     ((c name (caar names))
			      (cdar names))
			     (else
			      (loop (cdr names))))))))
      (if (or (null? (cdr e))
	      (not (pair? (cdr e))))
	  e
	  (let loop ((todo (cdr e)) (mask 0))
	    (cond ((null? todo)
		   `(,(r 'integer->file-mode) ,mask))
		  ((and (pair? todo)
			(lookup (car todo)))
		   => (lambda (i)
			(loop (cdr todo) (bitwise-ior i mask))))
		  (else
		   e)))))))

;----------------
; Time - seconds since the epoch.

(define-record-type time :time
  (make-time seconds)
  time?
  (seconds time-seconds))

(define-record-discloser :time
  (lambda (time)
    (let ((string (time->string time)))
      (list 'time (substring string 0 (- (string-length string) 1))))))

; We need to make these in the outside world.
(define-exported-binding "posix-time-type" :time)

(define (time=? time1 time2)
  (= (time-seconds time1)
     (time-seconds time2)))

(define (time<? time1 time2)
  (< (time-seconds time1)
     (time-seconds time2)))

(define (time<=? time1 time2)
  (not (time<? time2 time1)))
      
(define (time>? time1 time2)
  (time<? time2 time1))

(define (time>=? time1 time2)
  (not (time<? time1 time2)))

(import-lambda-definition current-time () "posix_time")
(import-lambda-definition time->string (time) "posix_ctime")

;----------------
; Dates - what a mess.
;
; This is not yet working.

(define-record-type date :date
  (make-date second minute hour month-day month year time-zone)
  date?
  (second    date-second set-date-second!)  ; to preserve mutability
  (minute    date-minute)
  (hour      date-hour)
  (week-day  date-week-day)
  (month-day date-month-day)
  (year-day  date-year-day)
  (month     date-month)
  (year      date-year)		; Since 1900 (why?)
  (time-zone date-time-zone))   ; possibly #f

(define-record-type time-zone :time-zone
  (make-time-zone name offset daylight-savings?)
  time-zone?
  (name   time-zone-name)	; string
  (offset time-zone-offset)     ; seconds
  (daylight-savings? date-daylight-savings?))   ; yes, no, don't know

;(import-definition posix-date->string     "posix_asctime")
;(import-definition posix-time->utc-date   "posix_gmtime")
;(import-definition posix-time->local-date "posix_localtime")
;(import-definition posix-date->time       "posix_mktime")

;----------------
; Users

(define-record-type user-id :user-id
  (integer->user-id uid)
  user-id?
  (uid user-id->integer))

(define-record-discloser :user-id
  (lambda (user-id)
    (list 'user-id (user-id->integer user-id))))

(define (user-id=? u1 u2)
  (= (user-id->integer u1)
     (user-id->integer u2)))

; We need to make these in the outside world.
(define-exported-binding "posix-user-id-type" :user-id)

(define-record-type user-info :user-info
  (really-make-user-info name uid group home-directory shell)
  user-info?
  (name user-info-name)
  (uid user-info-id)
  ;; this is misnamed: it should be called group-id
  (group user-info-group)
  (home-directory user-info-home-directory)
  (shell user-info-shell))

(define (make-user-info name uid gid home-directory shell)
  (really-make-user-info (x->os-string name)
			 uid gid
			 (x->os-string home-directory)
			 (x->os-string shell)))
     
(define-record-discloser :user-info
  (lambda (user-info)
    (list 'user-info (user-info-name user-info))))

(define (user-id->user-info user-id)
  (apply make-user-info
	 (external-user-id->user-info user-id)))

(define (name->user-info name)
  (apply make-user-info
	 (external-name->user-info
	  (os-string->byte-vector
	   (x->os-string name)))))

(import-lambda-definition external-user-id->user-info (user-id) "posix_getpwuid")
(import-lambda-definition external-name->user-info (name) "posix_getpwnam")

;----------------
; Groups

(define-record-type group-id :group-id
  (integer->group-id gid)
  group-id?
  (gid group-id->integer))

(define-record-discloser :group-id
  (lambda (group-id)
    (list 'group-id (group-id->integer group-id))))

(define-exported-binding "posix-group-id-type" :group-id)

(define (group-id=? g1 g2)
  (= (group-id->integer g1)
     (group-id->integer g2)))

(define-record-type group-info :group-info
  (really-make-group-info name uid members)
  group-info?
  (name group-info-name)
  (uid group-info-id)
  (members group-info-members))

(define (make-group-info name uid members)
  (really-make-group-info (x->os-string name)
			  uid
			  ;; #### this is in conflict with the docs,
			  ;; which say we have uids here
			  (map x->os-string (vector->list members))))

(define-record-discloser :group-info
  (lambda (group-info)
    (list 'group-info (group-info-name group-info))))

(define (group-id->group-info group-id)
  (apply make-group-info
	 (external-group-id->group-info group-id)))

(define (name->group-info name)
  (apply make-group-info
	 (external-name->group-info
	  (os-string->byte-vector
	   (x->os-string name)))))

(import-lambda-definition external-group-id->group-info (group-id) "posix_getgrgid")
(import-lambda-definition external-name->group-info (name) "posix_getgrnam")

;----------------
; Rest of 5.6
;
; int access(char *path, int amode)
;
; (accessible? "foo/bar/baz" (access-mode read))

; The masks are known to the C code.

(define-finite-type access-mode :access-mode
  (mask)
  access-mode?
  access-modes
  access-mode-name
  access-mode-index
  (mask access-mode-mask)
  ((read	#o0001)
   (write	#o0002)
   (execute	#o0004)
   (exists	#o0010)))

(define (accessible? path mode0 . more-modes)
  (file-stuff 7
	      (os-string->byte-vector (x->os-string path))
	      (if (null? more-modes)
		  (access-mode-mask mode0)
		  (apply + (map access-mode-mask
				(cons mode0 more-modes))))))

; int chmod(char *path, mode_t mode)
; int fchmod(int fd, mode_t mode)

; int chown(char *path, uid_t owner, gid_t group)
; int utime(char *path, struct utimbuf * times)

; int ftruncate(int fd, off_t length)

;----------------
; 5.7 Configurable Pathname Variables
;
; long pathconf(char *path, int name)
; long fpathconf(int fd, int name)


