@echo off

set runnable="%~1"

cd ps-compiler
echo ,batch > compile-twospace-gc.input
echo ,config ,load ../scheme/prescheme/interface.scm >> compile-twospace-gc.input
echo ,config ,load ../scheme/prescheme/package-defs.scm >> compile-twospace-gc.input
echo ,exec ,load load-ps-compiler.scm >> compile-twospace-gc.input
echo ,exec ,load compile-twospace-gc.scm >> compile-twospace-gc.input
echo ,exit >> compile-twospace-gc.input

%runnable% -h 8000000 < compile-twospace-gc.input
cd ..
move /Y scheme\vm\scheme48-twospace-heap.c c
move /Y scheme\vm\scheme48-twospace-write-image.c c
move /Y scheme\vm\scheme48-twospace-read-image.c c
