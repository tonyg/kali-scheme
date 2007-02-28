#!/bin/sh
# Build file list for the Windows installer WiX script
# Mike doesn't know how to do this using only Windows batch files.
# Run this in the source directory, under Cygwin or something.

runnable=$1

(
echo ',batch';
echo ',open srfi-1 srfi-13 srfi-14';
echo ',load build/windows-installer.scm';
# generate enough uuids and quote them
echo '(define uuids (list ';
uuidgen -n100 | sed 's/^.*$/"&"/';
echo '))';
echo '(define files (list';
for f in scheme/*.scm \
      scheme/env/*.scm \
      scheme/big/*.scm \
      scheme/misc/*.scm \
      scheme/cml/*.scm \
      scheme/srfi/*.scm \
      scheme/rts/*.scm \
      scheme/sort/*.scm \
      scheme/posix/*.scm \
      doc/*.ps doc/*.pdf doc/*.txt doc/html/*; do
  echo "\"$f\"";
done;
echo '))';
echo "(write-file-elements-include-file files uuids \"build/scheme48-files.wxi\")"
) > scheme48-files.input

# For some reason, directly piping into $runnable doesn't work under Mike's Cygwin
# installation---the second 4k block gets read a second time.

$runnable < scheme48-files.input
