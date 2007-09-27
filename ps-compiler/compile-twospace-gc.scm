; Copyright (c) 1993-2007 by Richard Kelsey.  See file COPYING.

(config '(load "../scheme/vm/macro-package-defs.scm"))
(load-package 'vm-architecture)
(in 'forms '(run (set! *duplicate-lambda-size* 30)))
(in 'simplify-let '(run (set! *duplicate-lambda-size* 15)))

(in 'prescheme-compiler
    '(run (prescheme-compiler
           '(allocation heap heap-two-space-native-code gc read-image write-image)
	   '("../scheme/platform-interfaces.scm"
	     "../scheme/ps-platform-packages.scm"
	     "../scheme/vm/interfaces.scm"
	     "../scheme/vm/shared-interfaces.scm"
             "../scheme/vm/ps-package-defs.scm"
	     "../scheme/vm/package-defs.scm"
	     "../scheme/vm/twospace-gc-package-defs.scm"
	     "../scheme/vm/alt-vm-package-defs.scm")
           's48-heap-init
           "../scheme/vm/scheme48-twospace-heap.c"
           '(header "#include \"scheme48vm.h\"")
           '(header "#include \"scheme48heap.h\"")
           ;'(copy (heap walk-over-type-in-area))
	   '(no-copy (gc s48-trace-locations!))
	   '(no-copy (write-image trace-image-value))
	   '(copy (string-tables relocate-table))
           '(integrate (real-copy-object s48-trace-locations!)))))

