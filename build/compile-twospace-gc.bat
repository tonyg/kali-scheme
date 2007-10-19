@echo off

set runnable="%~1"

cd ps-compiler
echo ,batch > compile-twospace-gc.input
echo ,config ,load ../scheme/platform-interfaces.scm >> compile-twospace-gc.input
echo ,config ,load ../scheme/rts-packages-32.scm >> compile-twospace-gc.input
echo ,config ,load ../scheme/prescheme/interface.scm >> compile-twospace-gc.input
echo ,config ,load ../scheme/prescheme/package-defs.scm >> compile-twospace-gc.input
echo ,exec ,load load-ps-compiler.scm >> compile-twospace-gc.input
echo ,exec ,load compile-twospace-gc-32.scm >> compile-twospace-gc.input
echo ,exit >> compile-twospace-gc.input

call %runnable% -h 8000000 < compile-twospace-gc.input
cd ..
move /Y scheme\vm\scheme48-twospace-heap-32.c c

