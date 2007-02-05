@echo off

set runnable="%~1"

call build\filenames.bat

echo ,batch > build\build-initial-image.input
echo ,bench on >> build\build-initial-image.input
echo ,open signals handle features >> build\build-initial-image.input
echo ,open bitwise ascii code-vectors >> build\build-initial-image.input
echo ,open (subset i/o (write-byte)) >> build\build-initial-image.input
echo ,open cells record-types >> build\build-initial-image.input
echo ,load %linker-files% >> build\build-initial-image.input
echo ,load scheme/alt/init-defpackage.scm >> build\build-initial-image.input
echo (load-configuration "scheme/interfaces.scm") >> build\build-initial-image.input
echo (load-configuration "scheme/vm/shared-interfaces.scm") >> build\build-initial-image.input
echo (load-configuration "scheme/packages.scm") >> build\build-initial-image.input
echo (flatload initial-structures) >> build\build-initial-image.input
echo (load "build/initial.scm") >> build\build-initial-image.input
echo (link-initial-system) >> build\build-initial-image.input

%runnable% -h 8000000 < build\build-initial-image.input
