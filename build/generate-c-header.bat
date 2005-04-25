@echo off

set runnable="%~1"

echo ,bench > generate-c-header.input
echo ,batch >> generate-c-header.input
echo ,load-package big-scheme >> generate-c-header.input
echo ,open big-scheme >> generate-c-header.input
echo ,load scheme/link/generate-c-header.scm >> generate-c-header.input
echo (make-c-header-file "c/scheme48.h"	>> generate-c-header.input
echo			 "c/scheme48.h.in" >> generate-c-header.input
echo			 "scheme/vm/interp/arch.scm" >> generate-c-header.input
echo			 "scheme/vm/data/data.scm" >> generate-c-header.input
echo			 "scheme/rts/record.scm") >> generate-c-header.input

%runnable% < generate-c-header.input
