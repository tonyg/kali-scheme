@echo off

set srcdir=%1
set srcdir_cooked=%~f1
set srcdir_cooked=%srcdir_cooked:\=\\%
set lib=%~f2
set lib=%lib:\=\\%
set image=%~3
set vm=%4
set initial=%5
set builddate=%~t5

echo ,load "%srcdir_cooked%scheme/env/init-defpackage.scm" > %srcdir%\build\build-usual-image.input
echo ((*structure-ref filenames 'set-translation!) >> %srcdir%\build\build-usual-image.input
echo  "=scheme48/" "%srcdir_cooked%scheme/") >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/packages.scm >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/more-interfaces.scm >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/link-packages.scm >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/env-packages.scm >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/sort/interfaces.scm >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/sort/packages.scm >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/more-packages.scm >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/posix/packages.scm >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/cml/interfaces.scm >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/cml/packages.scm >> %srcdir%\build\build-usual-image.input
echo ,load =scheme48/srfi/packages.scm >> %srcdir%\build\build-usual-image.input
echo (ensure-loaded command-processor) >> %srcdir%\build\build-usual-image.input
echo (ensure-loaded usual-commands) >> %srcdir%\build\build-usual-image.input
echo ,go ((*structure-ref command 'command-processor) >> %srcdir%\build\build-usual-image.input
echo      (structure-package usual-commands) >> %srcdir%\build\build-usual-image.input
echo      (list ((*structure-ref os-strings 'string-^>os-string) "batch"))) >> %srcdir%\build\build-usual-image.input
echo (ensure-loaded usual-features) >> %srcdir%\build\build-usual-image.input
echo ,structure more-structures more-structures-interface >> %srcdir%\build\build-usual-image.input
echo ,in debuginfo (read-debug-info "%srcdir_cooked%build/initial.debug") >> %srcdir%\build\build-usual-image.input
echo ,keep maps source files >> %srcdir%\build\build-usual-image.input
echo ,translate =scheme48/ "%lib%/" >> %srcdir%\build\build-usual-image.input
echo ,build ((*structure-ref package-commands-internal >> %srcdir%\build\build-usual-image.input
echo                         'new-command-processor) >> %srcdir%\build\build-usual-image.input
echo         "(made by %USERNAME% on %builddate%)" >> %srcdir%\build\build-usual-image.input
echo         usual-commands >> %srcdir%\build\build-usual-image.input
echo         built-in-structures more-structures) "%image:\=\\%" >> %srcdir%\build\build-usual-image.input

%vm% -i %initial% -a batch < %srcdir%\build\build-usual-image.input
