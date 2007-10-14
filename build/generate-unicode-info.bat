@echo off

set runnable="%~1"

echo ,bench > build\generate-unicode-info.input
echo ,batch >> build\generate-unicode-info.input
echo ,config ,load scheme/link/unicode-data-packages.scm >> build\generate-unicode-info.input
echo ,in unicode-data (create-unicode-tables "build/UnicodeData.txt" "build/PropList.txt" "build/SpecialCasing.txt" "build/CaseFolding.txt" "build/CompositionExclusions.txt" "scheme/env/unicode-info.scm" "scheme/rts/syntax-info.scm" "scheme/big/unicode-normalization-info.scm" "scheme/srfi/srfi-14-base-char-sets.scm") >> build\generate-unicode-info.input

%runnable% -h 6000000 < build\generate-unicode-info.input
