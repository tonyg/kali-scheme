; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

(define-structure unicode-data (export create-unicode-tables)
  (open scheme
	finite-types destructuring define-record-types bitwise tables
	(subset srfi-13 (string-index string-prefix? string-suffix? string-contains string-skip))
	(subset srfi-14 (char-set:hex-digit char-set:whitespace))
	(subset srfi-1 (iota find filter-map))
	srfi-23 ; ERROR
	sort
	)
  (files "../big/compact-table.scm" ; so we run on older Scheme 48
	 "../env/unicode-category.scm"
	 unicode-data))
