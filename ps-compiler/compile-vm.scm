; Copyright (c) 1993-2006 by Richard Kelsey.  See file COPYING.

(config '(load "../scheme/vm/macro-package-defs.scm"))
(load-package 'vm-architecture)
(in 'forms '(run (set! *duplicate-lambda-size* 30)))
(in 'simplify-let '(run (set! *duplicate-lambda-size* 15)))
(in 'prescheme-compiler
    '(run (prescheme-compiler
	   '(interpreter heap-init)
	   '("../scheme/vm/interfaces.scm"
	     "../scheme/vm/shared-interfaces.scm"
	     "../scheme/vm/ps-package-defs.scm"
	     "../scheme/vm/package-defs.scm")
	   'scheme48-init
	   "../scheme/vm/scheme48vm.c"
	   '(header "#include \"scheme48vm-prelude.h\"")
	   '(copy (heap walk-over-type-in-area)
		  (fixnum-arithmetic quotient-carefully))
	   '(no-copy (interpreter interpret
				  application-exception
				  handle-interrupt
				  uuo
				  collect-saving-temp
				  do-gc))
	   '(integrate (copy-next do-gc)
		       (copy-object do-gc))
	   '(shadow ((interpreter do-gc) (heap *hp*))
		    ((interpreter restart)
		     (interpreter *val* *code-pointer*)
		     (stack *stack* *env*))))))
