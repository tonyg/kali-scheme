; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.

(config '(load "../scheme/vm/macro-package-defs.scm"))
(load-package 'vm-architecture)
(in 'forms '(run (set! *duplicate-lambda-size* 30)))
(in 'simplify-let '(run (set! *duplicate-lambda-size* 15)))
(in 'prescheme-compiler
    '(run (prescheme-compiler
	   '(vm external-gc-roots interpreter-gc)
	   '("../scheme/vm/interfaces.scm"
	     "../scheme/vm/shared-interfaces.scm"
	     "../scheme/vm/ps-package-defs.scm"
	     "../scheme/vm/package-defs.scm"
	     "../scheme/vm/alt-gc-package-defs.scm" ;; For the type-checker
	     "../scheme/vm/alt-image-package-defs.scm")
	   's48-init
	   "../scheme/vm/scheme48vm.c"
	   '(header "#include \"scheme48vm-prelude.h\"")
	   '(copy (interpreter pop-continuation-from-stack
			       env-and-template-setup)
		  (text-encodings encode-scalar-value decode-scalar-value))
	   '(no-copy (interpreter interpret
				  application-exception
				  handle-interrupt
				  real-protocol-match
				  raise
				  uuo)
		     (vm s48-restart)))))
