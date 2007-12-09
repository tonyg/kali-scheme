@echo off

set runnable="%~1"

cd ps-compiler
echo ,batch > compile-bibop-gc.input
echo ,config ,load ../scheme/platform-interfaces.scm >> compile-bibop-gc.input
echo ,config ,load ../scheme/rts-packages-32.scm >> compile-bibop-gc.input
echo ,config ,load ../scheme/prescheme/interface.scm >> compile-bibop-gc.input
echo ,config ,load ../scheme/prescheme/package-defs.scm >> compile-bibop-gc.input
echo ,exec ,load load-ps-compiler.scm >> compile-bibop-gc.input
echo ,exec ,load compile-bibop-gc-32.scm >> compile-bibop-gc.input
echo ,exit >> compile-bibop-gc.input

call %runnable% -h 12000000 < compile-bibop-gc.input
cd ..
move /Y scheme\vm\scheme48-bibop-heap-32.c c
