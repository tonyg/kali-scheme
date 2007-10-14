; Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees. See file COPYING.


; Platform-specific constants

(define-interface platform-interface
  (export bytes-per-cell
	  log-bytes-per-cell
	  bits-per-byte
	  bits-per-cell
	  addressing-units-per-cell
	  c-useful-bits-per-word
	  s48-useful-bits-per-word
	  unused-field-width
	  tag-field-width
	  data-field-width
	  immediate-type-field-width
	  pre-scheme-integer-size))
