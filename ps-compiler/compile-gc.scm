; Copyright (c) 1993-2001 by Richard Kelsey.  See file COPYING.

(config '(load "../scheme/vm/macro-package-defs.scm"))
(load-package 'vm-architecture)
(in 'forms '(run (set! *duplicate-lambda-size* 30)))
(in 'simplify-let '(run (set! *duplicate-lambda-size* 15)))

(in 'prescheme-compiler
    '(run (prescheme-compiler
           '(allocation heap heap-init gc)
           '("../scheme/vm/interfaces.scm"
	     "../scheme/vm/shared-interfaces.scm"
             "../scheme/vm/ps-package-defs.scm"
             "../scheme/vm/package-defs.scm"
	     "../scheme/vm/alt-vm-package-defs.scm")
           's48-heap-init
           "../scheme/vm/scheme48heap.c"
           '(header "#include \"scheme48vm.h\"")
           ;'(copy (heap walk-over-type-in-area))
	   '(no-copy (gc s48-trace-locations!))
           '(integrate (real-copy-object s48-trace-locations!)))))

(in 'prescheme-compiler
    '(run (prescheme-compiler
           '(read-image)
           '("../scheme/vm/interfaces.scm"
	     "../scheme/vm/shared-interfaces.scm"
             "../scheme/vm/ps-package-defs.scm"
             "../scheme/vm/package-defs.scm"
             "../scheme/vm/alt-gc-package-defs.scm"
             "../scheme/vm/alt-vm-package-defs.scm")
           's48-read-image-init
           "../scheme/vm/scheme48read-image.c"
           '(header "#include \"scheme48vm.h\"")
           '(header "#include \"scheme48heap.h\""))))

(in 'prescheme-compiler
    '(run (prescheme-compiler
	   '(write-image)
	   '("../scheme/vm/interfaces.scm"
	     "../scheme/vm/shared-interfaces.scm"
	     "../scheme/vm/ps-package-defs.scm"
             "../scheme/vm/package-defs.scm"
	     "../scheme/vm/alt-vm-package-defs.scm")
	   's48-write-image-init
	   "../scheme/vm/scheme48write-image.c"
	   '(header "#include \"scheme48vm.h\"")
	   '(header "#include \"scheme48heap.h\"")
	   '(no-copy (write-image trace-image-value)))))
