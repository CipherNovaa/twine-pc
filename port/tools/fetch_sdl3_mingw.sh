#!/bin/sh
# The Windows build's SDL3 (port/Makefile, make windows): the SDL project's own MinGW build of the release the Linux
# build uses, from its GitHub releases, checked against its SHA-256 and unpacked into DIR (build/win-deps).
# SDL3 is under the zlib licence (its LICENSE.txt goes into the game's folder).
#   fetch_sdl3_mingw.sh VERSION DIR
set -e
VERSION=$1
DIR=$2
case $VERSION in
3.4.18) SHA=049b9dd711e3fb3f8d84feb998dfec54a75e17cf539c4a80ff4d058d839771cf ;;
*) echo "fetch_sdl3_mingw.sh: no checksum known for SDL3 $VERSION (add it here)" >&2; exit 1 ;;
esac
TAR=SDL3-devel-$VERSION-mingw.tar.gz
mkdir -p "$DIR"
if [ ! -f "$DIR/$TAR" ]; then
    curl -fL -o "$DIR/$TAR.part" "https://github.com/libsdl-org/SDL/releases/download/release-$VERSION/$TAR"
    mv "$DIR/$TAR.part" "$DIR/$TAR"
fi
echo "$SHA  $DIR/$TAR" | sha256sum -c -
tar xzf "$DIR/$TAR" -C "$DIR"
touch "$DIR/SDL3-$VERSION/i686-w64-mingw32/lib/libSDL3.dll.a"
