#! /bin/sh

# build.sh - build some programs and get executables and headers to /usr/local/

set -e

BIN="/usr/local/bin/"
INC="/usr/local/include/"

# Python

echo "Copying Python programs"

OPT="--preserve=ownership"

sudo cp $OPT eca.py        $BIN/eca
sudo cp $OPT hjjs.py       $BIN/hjjs
sudo cp $OPT k2cfr.py      $BIN/k2cfr
sudo cp $OPT mts.py        $BIN/mts
sudo cp $OPT printable.py  $BIN/printable
sudo cp $OPT projectile.py $BIN/projectile

# C

echo "Copying C headers"

sudo cp $OPT *.h $INC

echo "Compiling C programs"

OPT="-std=c11 -march=native -O3"

cc $OPT -fsyntax-only test.c

cc $OPT -o aeca       aeca.c
cc $OPT -o amorse     amorse.c
cc $OPT -o ctime      ctime.c
cc $OPT -o getendian  getendian.c
cc $OPT -o getlocale  getlocale.c
cc $OPT -o hacc       hacc.c
cc $OPT -o hj2fb      hj2fb.c
cc $OPT -o hj2png     hj2png.c    -lpng -lz
cc $OPT -o limits     limits.c
cc $OPT -o morse      morse.c
cc $OPT -o pp         pp.c
cc $OPT -o waves      waves.c     -lFLAC

echo "Moving C programs"

sudo mv aeca amorse ctime getendian getlocale hacc hj2fb hj2png limits morse pp waves $BIN

# Haskell

echo "Compiling Haskell program"

ghc -no-keep-hi-files -no-keep-o-files -v0 -O2 -o qda qda.hs

echo "Moving Haskell program"

sudo mv qda $BIN

echo "Done"
