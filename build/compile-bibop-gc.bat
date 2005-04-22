@echo off

set runnable=%~1

cd ps-compiler
echo ,batch > compile-bibop-gc.input
echo ,config ,load ../scheme/prescheme/interface.scm >> compile-bibop-gc.input
echo ,config ,load ../scheme/prescheme/package-defs.scm >> compile-bibop-gc.input
echo ,exec ,load load-ps-compiler.scm >> compile-bibop-gc.input
echo ,exec ,load compile-bibop-gc.scm >> compile-bibop-gc.input
echo ,exit >> compile-bibop-gc.input

%runnable% -h 10000000 < compile-bibop-gc.input
cd ..
move /Y scheme\vm\scheme48-bibop-heap.c c
