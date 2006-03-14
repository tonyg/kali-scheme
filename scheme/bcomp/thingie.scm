; Copyright (c) 1993-2006 by Richard Kelsey and Jonathan Rees. See file COPYING.

; A thingie (placecard?) is used to hold a spot for a location that is to be
; found later.  The compiler sticks them in templates and the module system
; later replaces them with locations.
;
; We can't use (BEGIN ...) for this trivial package because it is loaded
; by flatload, which can't handle them.

(define-record-type thingie :thingie
  (make-thingie binding name want-type)
  thingie?
  (binding thingie-binding)
  (name thingie-name)
  (want-type thingie-want-type))

