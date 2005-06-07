
; Comment out region

(defun comment-out-region (arg)
  "Insert comment string at beginning of each line in the region."
  (interactive "P")
  (let (start end)
    (if (< (point) (mark))
        (setq start (point) end (mark-marker))
        (setq start (mark) end (point-marker)))
    (save-excursion
      (untabify start (marker-position end))
      (goto-char start)
      (if (not (bolp))
	  (progn (end-of-line) (forward-char)))
      (while (< (point) (marker-position end))
	(if (eq arg '-)
	    (if (looking-at comment-start)
		(delete-char (length comment-start)))
	    (insert comment-start))
	(end-of-line)
	(forward-char)))))

;(defun uncomment-out-region (arg)
;  (interactive nil)
;  (comment-out-region '-))


; Mini-Find Tag

(defvar last-mini-tag "" "Last tag sought by mini-find-tag.")

(defun mini-find-tag (tagname &optional next)
  "Search for a definition of TAGNAME in current buffer.
 If TAGNAME is a null string, the expression in the buffer
around or before point is used as the tag name.
 If second arg NEXT is non-nil (interactively, with prefix arg),
searches for the next definition in the buffer
that matches the tag name used in the previous mini-find-tag."

  (interactive (if current-prefix-arg
		   '(nil t)
		 (list (read-string "Mini-find tag: "))))
  (if (equal tagname "")             ;See definition of find-tag.
      (setq tagname (save-excursion
		      (buffer-substring
		       (progn (backward-sexp 1) (point))
		       (progn (forward-sexp 1) (point))))))
  (let ((pt (save-excursion
	      (if (not next)
		  (goto-char (point-min))
		(setq tagname last-mini-tag))
	      (setq last-mini-tag tagname)
	      (if (re-search-forward
		     (concat "^(def.*" tagname)
		     nil t)
		  (point)
		nil))))
    (if pt
	(progn (set-mark-command nil)
	       (goto-char pt))
      (signal 'search-failed '()))))

; indent-differently

(defun indent-differently ()
  "Make the current line indent like the body of a special form by
changing the operator's scheme-indent-hook appropriately."
  (interactive nil)
  (let ((here (point)))
    (save-excursion
      (back-to-indentation)
      (backward-up-list 1)
      (forward-char 1)
      (let ((i -1)
	    (function nil)
	    (p (point)))
	(while (<= (point) here)
	  (setq i (+ i 1))
	  (forward-sexp 1)
	  (if (= i 0)
	      (setq function (buffer-substring p (point)))))
	(setq i (- i 1))
	(let ((name (intern (downcase function))))
	  (cond ((equal (get name 'scheme-indent-hook) i)
		 (message "Indent %s nil" name)
		 (put name 'scheme-indent-hook nil))
		(t
		 (message "Indent %s %d" name i)
		 (put name 'scheme-indent-hook i))))))
    (scheme-indent-line)))
