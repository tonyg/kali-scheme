(config '(load "../scheme/vm/macro-package-defs.scm"))
(load-package 'vm-architecture)
(in 'forms '(run (set! *duplicate-lambda-size* 30)))
(in 'simplify-let '(run (set! *duplicate-lambda-size* 15)))
(in 'prescheme-compiler
    '(run (prescheme-compiler
	   '(allocation heap heap-init)
	   '("../scheme/vm/interfaces.scm"
	     "../scheme/vm/ps-package-defs.scm"
	     "../scheme/vm/package-defs.scm")
	   's48-heap-init
	   "../scheme/vm/scheme48heap.c"
	   '(header "#include \"scheme48vm.h\"")
	   '(copy (heap walk-over-type-in-area))
	   '(integrate (real-copy-object trace-locations!)))))
