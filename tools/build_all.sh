#!/bin/sh
# Build every executable on the disc from this project and assemble a full disc image from them.
# Usage: tools/build_all.sh [--skip-level01]
set -e
cd "$(dirname "$0")/.."
TARGETS="boot shell movie level02 level03 level04 level05 level06 level07 level08 level09 level10 level11"
[ "$1" = "--skip-level01" ] || TARGETS="level01 $TARGETS"

REPLACE=""
for t in $TARGETS; do
    make --no-print-directory LEVEL=$t -j8 check
    case $t in
        boot) disc=SLUS_012.72 ;;
        shell) disc=SHELL.EXE ;;
        movie) disc=MOVIE.EXE ;;
        level*) disc=$(echo "$t" | tr a-z A-Z).EXE ;;
    esac
    REPLACE="$REPLACE $disc=build/$t/$t.exe"
done

mkdir -p build/disc
.venv/bin/python tools/make_disc.py build/disc/twine_all.xml --replace $REPLACE
tools/mkpsxiso/build/mkpsxiso -y -q -o build/disc/twine_all.bin -c build/disc/twine_all.cue build/disc/twine_all.xml
sha256sum build/disc/twine_all.bin
