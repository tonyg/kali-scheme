rem @echo off

set runnable=%~1

cd ps-compiler
echo ,batch > i-know-what-i-am-doing.input
echo ,config ,load ../scheme/prescheme/interface.scm >> i-know-what-i-am-doing.input
echo ,config ,load ../scheme/prescheme/package-defs.scm >> i-know-what-i-am-doing.input
echo ,exec ,load load-ps-compiler.scm >> i-know-what-i-am-doing.input
echo ,exec ,load compile-vm-no-gc.scm >> i-know-what-i-am-doing.input
echo ,exec ,load compile-gc.scm>> i-know-what-i-am-doing.input
echo ,exit >> i-know-what-i-am-doing.input

%runnable% -h 8000000 < i-know-what-i-am-doing.input
cd ..
move /Y scheme\vm\scheme48vm.c c
move /Y scheme\vm\scheme48heap.c c
move /Y scheme\vm\scheme48write-image.c c
move /Y scheme\vm\scheme48read-image.c c
