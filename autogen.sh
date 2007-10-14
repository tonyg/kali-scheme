#! /bin/sh 

echo "This script requires an installed scheme48 executable in the path."
echo "The executable should be Scheme 48 1.6 or later." &&
sleep 3 &&
autoheader &&
autoconf &&
./configure &&
rm -rf autom4te.cache &&
rm -f scheme48.image build/initial.image c/scheme48.h &&
make build/filenames.make &&
make i-know-what-i-am-doing &&
make c/scheme48.h &&
make build/initial.image-32 build/initial.image-64 &&
make distclean
