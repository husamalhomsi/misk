#! /bin/sh

# build.sh - build programs and put programs and libraries in /usr/local/

# This script requires permission to write and run /usr/local/bin/ and
# /usr/local/include/, which likely requires being the superuser.
#
# All programs depend on the C compiler cc, which may be a link to GCC or Clang.
# Some programs that write PNG files depend on the C libraries libpng and zlib.
#
# To install the dependencies on Debian or a derivative of it, run
#
#   apt install clang libpng16-16 libpng-dev zlib1g zlib1g-dev

set -e

cd ../src/

BIN="/usr/local/bin/"
INC="/usr/local/include/"

echo "Copying libraries"

sudo cp --preserve=ownership *.h $INC

echo "Compiling programs"

OPT="-march=native -O3"

cc $OPT -o ctime     ctime.c
cc $OPT -o eca-png   eca-png.c -lpng
cc $OPT -o eca-txt   eca-txt.c
cc $OPT -o eca-wav   eca-wav.c
cc $OPT -o getlocale getlocale.c
cc $OPT -o limits    limits.c
cc $OPT -o midi      midi.c -lm
cc $OPT -o morse     morse.c
cc $OPT -o pq-fb     pq-fb.c
cc $OPT -o pq-png    pq-png.c -lpng
cc $OPT -o pq-seq    pq-seq.c
cc $OPT -o printable printable.c
cc $OPT -o rainbow   rainbow.c -lm
cc $OPT -o sumber    sumber.c -lm
cc $OPT -o ttt       ttt.c
cc $OPT -o xy-png    xy-png.c -lpng

echo "Moving programs"

sudo mv ctime eca-png eca-txt eca-wav getlocale limits midi morse pq-fb pq-png pq-seq printable rainbow sumber ttt xy-png $BIN

echo "Done"
