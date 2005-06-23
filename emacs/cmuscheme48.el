;;; cmuscheme48.el -- Scheme process in a buffer.  Adapted from cmuscheme.el.

(provide 'cmuscheme48)
(require 'cmuscheme)

(define-key scheme-mode-map "\M-\C-x" 'scheme48-send-definition);gnu convention
(define-key scheme-mode-map "\C-x\C-e" 'scheme48-send-last-sexp);gnu convention
(define-key scheme-mode-map "\C-ce"    'scheme48-send-definition)
(define-key scheme-mode-map "\C-c\C-e" 'scheme48-send-definition-and-go)
(define-key scheme-mode-map "\C-cr"    'scheme48-send-region)
(define-key scheme-mode-map "\C-c\C-r" 'scheme48-send-region-and-go)
(define-key scheme-mode-map "\C-cl"    'scheme48-load-file)

(defun scheme48-send-region (start end)
  "Send the current region to the inferior Scheme process."
  (interactive "r")
  (comint-send-string (scheme-proc)
		      (concat ",from-file "
			      (enough-scheme-file-name
			       (buffer-file-name (current-buffer)))
			      "\n"))
  (comint-send-region (scheme-proc) start end)
  (comint-send-string (scheme-proc) " ,end\n"))

; This assumes that when you load things into Scheme 48, you type
; names of files in your home directory using the syntax "~/".
; Similarly for current directory.  Maybe we ought to send multiple
; file names to Scheme and let it look at all of them.

(defun enough-scheme-file-name (file)
  (let* ((scheme-dir
	  (save-excursion
	    (set-buffer scheme-buffer)
	    (expand-file-name default-directory)))
	 (len (length scheme-dir))
	 (raw
	  (cond
	   ((and (> (length file) len)
		 (string-equal scheme-dir (substring file 0 len)))
	    (substring file len))
	   (*scheme48-home-directory-kludge*
	    (let* ((home-dir (expand-file-name "~/"))
		   (len (length home-dir)))
	      (if (and (> (length file) len)
		       (string-equal home-dir (substring file 0 len)))
		  (concat "~/" (substring file len))
		file)))
	   (t file))))
    (subst-char-in-string ?\\ ?/ raw)))

(defvar *scheme48-home-directory-kludge* t)

(defun scheme48-send-definition (losep)
  "Send the current definition to the inferior Scheme48 process."
  (interactive "P")
  (save-excursion
   (end-of-defun)
   (let ((end (point)))
     (beginning-of-defun)
     (if losep
	 (let ((loser "/tmp/s48lose.tmp"))
	   (write-region (point) end loser)
	   (scheme48-load-file loser))
	 (scheme48-send-region (point) end)))))

(defun scheme48-send-last-sexp ()
  "Send the previous sexp to the inferior Scheme process."
  (interactive)
  (scheme48-send-region (save-excursion (backward-sexp) (point)) (point)))

(defun scheme48-send-region-and-go (start end)
  "Send the current region to the inferior Scheme48 process,
and switch to the process buffer."
  (interactive "r")
  (scheme48-send-region start end)
  (switch-to-scheme t))

(defun scheme48-send-definition-and-go (losep)
  "Send the current definition to the inferior Scheme48,
and switch to the process buffer."
  (interactive "P")
  (scheme48-send-definition losep)
  (switch-to-scheme t))

(defun scheme48-load-file (file-name)
  "Load a Scheme file into the inferior Scheme48 process."
  (interactive (comint-get-source "Load Scheme48 file: "
				  scheme-prev-l/c-dir/file
				  scheme-source-modes t)) ; T because LOAD 
                                                          ; needs an exact name
  (comint-check-source file-name) ; Check to see if buffer needs saved.
  (setq scheme-prev-l/c-dir/file (cons (file-name-directory    file-name)
				       (file-name-nondirectory file-name)))
  (comint-send-string (scheme-proc)
		      (concat ",load "
			      (enough-scheme-file-name file-name)
			      "\n")))


; For Pertti Kellom\"aki's debugger.
; Cf. misc/psd-s48.scm.
(defvar psd-using-slib nil "Scheme 48, not SLIB.")
