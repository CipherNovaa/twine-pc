#!/bin/bash
# release.sh VERSION: the two downloads of a release in build/release/
#
#   twine-pc-vVERSION-windows-x86.zip   build/dist-win/twine (make -C port windows), portable, with README.txt
#   twine-pc-vVERSION-linux-x86.tar.gz  build/dist/twine-linux (make -C port dist-linux)
#
# Each gets the project's LICENSE, NOTICE.md and CHANGELOG.md, and tools/hdmovies.py. Nothing of the game's data, a
# BIOS, a disc image or film clips goes in: the script stops if a file of those kinds is in either folder.
set -euo pipefail
VERSION=${1:?usage: release.sh VERSION}
ROOT=$(cd "$(dirname "$0")/../../.." && pwd)
OUT=$ROOT/build/release
say() { printf 'release: %s\n' "$*"; }
die() { printf 'release: %s\n' "$*" >&2; exit 1; }

make -C "$ROOT/port" -j8 >/dev/null
make -C "$ROOT/port" dist-linux >/dev/null
make -C "$ROOT/port" windows -j8 >/dev/null

add_docs() { # (the folder, the line ending for the text files: crlf on Windows)
    mkdir -p "$1/tools"
    cp "$ROOT/LICENSE" "$ROOT/NOTICE.md" "$ROOT/CHANGELOG.md" "$1/"
    cp "$ROOT/port/tools/hdmovies.py" "$1/tools/"
    if [ "$2" = crlf ]; then
        for f in "$1/LICENSE" "$1/NOTICE.md" "$1/CHANGELOG.md"; do sed -i 's/\r\?$/\r/' "$f"; done
    fi
}
check() { # (no game data, BIOS, disc images or films)
    local bad
    bad=$(find "$1" -type f \( -iname '*.bin' -o -iname '*.cue' -o -iname '*.iso' -o -iname '*.img' -o -iname '*.chd' \
        -o -iname '*.mcd' -o -iname '*.str' -o -iname '*.xa' -o -iname '*.mp4' -o -iname '*.mkv' -o -iname '*.webm' \
        -o -iname 'scph*' -o -iname '*.exe' ! -iname 'twine.exe' \) | grep -v '/runtime/' || true)
    [ -z "$bad" ] || die "not for a download: $bad"
}

rm -rf "$OUT" && mkdir -p "$OUT"

W=$OUT/twine
cp -r "$ROOT/build/dist-win/twine" "$W"
sed 's/\r\?$/\r/' "$ROOT/port/tools/dist/README-WINDOWS.txt" > "$W/README.txt"
printf 'This file keeps the game portable: its settings, saves, cards and logs stay in the twine folder here.\r\n' \
    > "$W/portable"
add_docs "$W" crlf
check "$W"
(cd "$OUT" && python3 -m zipfile -c "twine-pc-v$VERSION-windows-x86.zip" twine)

L=$OUT/twine-linux
cp -r "$ROOT/build/dist/twine-linux" "$L"
add_docs "$L" lf
check "$L"
tar -C "$OUT" --owner=0 --group=0 --numeric-owner -czf "$OUT/twine-pc-v$VERSION-linux-x86.tar.gz" twine-linux

(cd "$OUT" && sha256sum "twine-pc-v$VERSION-windows-x86.zip" "twine-pc-v$VERSION-linux-x86.tar.gz" > SHA256SUMS.txt)
say "$(cd "$OUT" && ls -l --block-size=M ./*.zip ./*.tar.gz | awk '{print $5, $NF}' | tr '\n' ' ')"
