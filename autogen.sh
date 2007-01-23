#! /bin/sh 

echo "This script requires an installed scheme48 executable in the path"
echo "The executable should be Scheme 48 1.3 or 1.4 (but not 1.5!)" &&
sleep 3 &&
autoheader &&
autoconf &&
./configure &&
rm -rf autom4te.cache &&
rm -f scheme48.image build/initial.image c/scheme48.h &&
make build/filenames.make &&
make i-know-what-i-am-doing &&
make c/scheme48.h &&
make build/initial.image &&
make distclean
