@echo off

set runnable=%~1

echo ,bench > build\generate-unicode-info.input
echo ,batch >> build\generate-unicode-info.input
echo ,open finite-types destructuring define-record-types bitwise >> build\generate-unicode-info.input
echo ,open (subset srfi-13 (string-index)) (subset srfi-1 (find)) srfi-23 >> build\generate-unicode-info.input
echo ,load scheme/big/compact-table.scm scheme/env/unicode-category.scm  >> build\generate-unicode-info.input
echo ,load scheme/link/unicode-data.scm >> build\generate-unicode-info.input
echo (create-unicode-tables "build/UnicodeData.txt" "scheme/env/unicode-info.scm" "scheme/srfi/srfi-14-base-char-sets.scm") >> build\generate-unicode-info.input

%runnable% -h 6000000 < build\generate-unicode-info.input
