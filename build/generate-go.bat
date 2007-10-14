@echo off

set GO=%1
set VM=%2
set IMAGE=%3
echo @%VM% -i %IMAGE% %%* > %GO%
