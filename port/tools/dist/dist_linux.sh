#!/bin/bash
# make -C port dist-linux: the Linux download, a folder and its .tar.gz, for any 64-bit x86 Linux from Ubuntu 22.04 and
# Debian 12 on (Fedora, Arch, SteamOS ...), with or without the system's 32-bit libraries:
#
#   twine/twine              the launcher (tools/dist/twine.sh): run this
#   twine/bin/twine          the game, as `make -C port` built it (debug information stripped)
#   twine/bin/lib/           SDL3, built for glibc 2.31 (tools/dist/sdl3_build.sh), found through the game's RUNPATH
#   twine/runtime/           32-bit glibc 2.35 and the libraries SDL3 opens (tools/dist/runtime_harvest.sh), for
#                            systems without 32-bit libraries of their own
#   twine/licenses/          the licences of everything bundled; THIRD-PARTY.txt lists it with versions and sources
#
# SDL3 and the runtime are built in containers (podman, else docker; rootless podman or the docker group, no sudo) and
# kept in build/dist-linux/cache until their inputs change. Nothing of the game's data, the BIOS, a disc image or film
# clips is ever put in the folder: the player supplies those.
#
# Usage: dist_linux.sh <the built game> <output folder>
set -euo pipefail
GAME=$1
OUT=$2
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../../.." && pwd)
CACHE=$ROOT/build/dist-linux/cache
SDL_VERSION=3.4.18
SDL_SHA256=9c75cf16330322c217dedd2e0609f1124f1b54b8633e763467b4684d0f4334a3
GLIBC_MAX=2.35 # the oldest glibc the game is for (Ubuntu 22.04)
DIST=$OUT/twine-linux
TARBALL=$OUT/twine-linux-x86.tar.gz

say() { printf 'dist-linux: %s\n' "$*"; }
die() { printf 'dist-linux: %s\n' "$*" >&2; exit 1; }

if command -v podman >/dev/null 2>&1; then
    CT=podman
elif command -v docker >/dev/null 2>&1; then
    CT=docker
else
    die "needs podman or docker to build SDL3 and the runtime"
fi

# --- the game: what it asks of the system
[ -x "$GAME" ] || die "$GAME is not built (make -C port)"
newest=$(objdump -T "$GAME" | grep -o 'GLIBC_[0-9.]*' | sed 's/GLIBC_//' | sort -uV | tail -1)
if [ "$(printf '%s\n%s\n' "$newest" "$GLIBC_MAX" | sort -V | tail -1)" != "$GLIBC_MAX" ]; then
    die "the game needs glibc $newest, newer than $GLIBC_MAX: $(objdump -T "$GAME" | grep "GLIBC_$newest" | awk '{print $NF}' | tr '\n' ' ')(see port/src/glibc_compat.h)"
fi
needed=$(objdump -p "$GAME" | awk '/NEEDED/ {print $2}' | sort | tr '\n' ' ')
for lib in $needed; do
    case $lib in libSDL3.so.0 | libc.so.6 | libm.so.6 | libpthread.so.0 | libdl.so.2 | librt.so.1) ;;
    *) die "the game needs $lib, which the download doesn't carry" ;;
    esac
done
say "the game needs glibc $newest at most; libraries: $needed"

# --- SDL3, from its release source
mkdir -p "$CACHE/sdl-src" "$CACHE/out"
src=$CACHE/sdl-src/SDL3-$SDL_VERSION.tar.gz
if [ ! -f "$src" ]; then
    say "downloading SDL3 $SDL_VERSION"
    curl -sSLf -o "$src.part" "https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VERSION/SDL3-$SDL_VERSION.tar.gz"
    mv "$src.part" "$src"
fi
echo "$SDL_SHA256  $src" | sha256sum -c --quiet - || die "SDL3's source doesn't match its checksum"
stamp="$SDL_VERSION $(sha256sum "$HERE/sdl3_build.sh" | cut -c1-16)"
if [ "$(cat "$CACHE/out/sdl.stamp" 2>/dev/null)" != "$stamp" ] || [ ! -f "$CACHE/out/sdl/lib/libSDL3.so.0" ]; then
    say "building SDL3 $SDL_VERSION for 32-bit x86 in a Debian 11 container"
    $CT run --rm --platform linux/386 -e SDL_VERSION=$SDL_VERSION -e OWNER="$(id -u):$(id -g)" \
        -v "$CACHE/sdl-src:/src:ro" -v "$CACHE/out:/out" -v "$HERE:/scripts:ro" \
        docker.io/i386/debian:bullseye sh /scripts/sdl3_build.sh
    echo "$stamp" >"$CACHE/out/sdl.stamp"
fi

# --- the 32-bit runtime
stamp="$(sha256sum "$HERE/runtime_harvest.sh" | cut -c1-16)"
if [ "$(cat "$CACHE/out/runtime.stamp" 2>/dev/null)" != "$stamp" ] || [ ! -f "$CACHE/out/runtime/ld-linux.so.2" ]; then
    say "collecting the 32-bit runtime in an Ubuntu 22.04 container"
    $CT run --rm -e OWNER="$(id -u):$(id -g)" -v "$CACHE/out/sdl:/sdl:ro" -v "$CACHE/out:/out" -v "$HERE:/scripts:ro" \
        docker.io/library/ubuntu:22.04 sh /scripts/runtime_harvest.sh
    echo "$stamp" >"$CACHE/out/runtime.stamp"
fi

# --- the folder
rm -rf "$DIST"
mkdir -p "$DIST/bin/lib" "$DIST/runtime" "$DIST/licenses"
install -m 755 "$HERE/twine.sh" "$DIST/twine"
objcopy --strip-debug "$GAME" "$DIST/bin/twine"
chmod 755 "$DIST/bin/twine"
cp -L "$CACHE/out/sdl/lib/libSDL3.so.0" "$DIST/bin/lib/libSDL3.so.0"
cp -a "$CACHE/out/runtime/." "$DIST/runtime/"
cp "$CACHE/out/licenses/"* "$DIST/licenses/"
cp "$CACHE/out/sdl/LICENSE.txt" "$DIST/licenses/SDL3.LICENSE.txt"
cp "$HERE/README-LINUX.txt" "$DIST/README.txt"
{
    cat <<EOF
Third-party software in this download
=====================================

bin/lib/libSDL3.so.0
    SDL $SDL_VERSION (https://libsdl.org), zlib licence: licenses/SDL3.LICENSE.txt.
    Built unmodified from https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VERSION/SDL3-$SDL_VERSION.tar.gz
    (sha256 $SDL_SHA256).

bin/twine also contains:
    pl_mpeg (https://github.com/phoboslab/pl_mpeg), MIT licence, by Dominic Szablewski.
    Glyphs of the title card from the MGS1 Codec font, (c) 2025 Andrew Gleeson, CC BY-SA 4.0
    (https://creativecommons.org/licenses/by-sa/4.0/), as distance fields adapted from it.

runtime/ (used only on systems without 32-bit libraries of their own)
    Unmodified 32-bit (i386) libraries from Ubuntu 22.04 LTS packages, each under its own licence (MIT, BSD, zlib,
    LGPL-2.1 or later and similar; D-Bus under the Academic Free License 2.1). Each package's copyright file is in
    licenses/<package>.copyright. The source code of every one of them, at exactly these versions, is available from
    Ubuntu (https://launchpad.net/ubuntu/+source/<source package>/<version>, or apt-get source); on request we
    will provide it too. The libraries are loaded at run time and can be replaced with other builds of the same
    names (put them in runtime/).

EOF
    sed 's/^/    /' "$CACHE/out/packages.txt"
} >"$DIST/licenses/THIRD-PARTY.txt"

# nothing of the game's own in it
if find "$DIST" -iname '*.bin' -o -iname '*.cue' -o -iname '*.iso' -o -iname '*.chd' -o -iname '*.mcd' -o -iname '*.mpg' \
    -o -iname 'SCPH*' | grep -q .; then
    die "the folder holds game data or a BIOS: $(find "$DIST" -iname '*.bin' -o -iname '*.cue' -o -iname 'SCPH*' | head -3)"
fi

tar -C "$OUT" --owner=0 --group=0 --numeric-owner -czf "$TARBALL" "$(basename "$DIST")"
say "$DIST ($(du -sh "$DIST" | cut -f1)), $TARBALL ($(du -h "$TARBALL" | cut -f1))"
