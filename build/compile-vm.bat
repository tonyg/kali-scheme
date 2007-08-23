@echo off

set runnable="%~1"

cd ps-compiler
echo ,batch > compile-vm.input
echo ,config ,load ../scheme/prescheme/interface.scm >> compile-vm.input
echo ,config ,load ../scheme/prescheme/package-defs.scm >> compile-vm.input
echo ,exec ,load load-ps-compiler.scm >> compile-vm.input
echo ,exec ,load compile-vm-no-gc.scm >> compile-vm.input
echo ,exit >> compile-vm.input

call %runnable% -h 12000000 < compile-vm.input
cd ..
move /Y scheme\vm\scheme48vm.c c
