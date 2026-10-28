#!/bin/sh
# Runs inside an Ubuntu 22.04 container (dist_linux.sh): the 32-bit libraries the port needs on a system that has
# none (no i386 multiarch, no lib32): glibc 2.35 and its loader, and what SDL3 opens when it runs to show a window
# (X11, Wayland, xkbcommon), play sound (the PulseAudio client, which PipeWire systems serve too), open the file
# chooser (D-Bus) and find gamepads (udev). Each library comes with its package's copyright file. Graphics drivers
# (Mesa, NVIDIA) are not among them: they have to match the system's own, so without its 32-bit ones the picture is
# drawn and scaled on the CPU (SDL's software renderer). In: /sdl (SDL3's prefix). Out: /out/runtime,
# /out/licenses, /out/packages.txt, owned by $OWNER.
set -eu
export DEBIAN_FRONTEND=noninteractive
dpkg --add-architecture i386
apt-get update -qq
PKGS="libc6 libx11-6 libx11-xcb1 libxext6 libxrandr2 libxrender1 libxcursor1 libxi6 libxfixes3 libxss1 libxtst6
      libxkbcommon0 libwayland-client0 libwayland-cursor0 libwayland-egl1 libpulse0 libdbus-1-3 libudev1 libasound2"
apt-get install -y -qq --no-install-recommends $(for p in $PKGS; do printf '%s:i386 ' "$p"; done) >/dev/null
L=/usr/lib/i386-linux-gnu
# the libraries SDL3 opens by name (SDL_LoadObject), and SDL3 itself: the closure of their dependencies
ROOTS="/sdl/lib/libSDL3.so.0 $L/libX11.so.6 $L/libX11-xcb.so.1 $L/libXext.so.6 $L/libXrandr.so.2 $L/libXrender.so.1
       $L/libXcursor.so.1 $L/libXi.so.6 $L/libXfixes.so.3 $L/libXss.so.1 $L/libXtst.so.6 $L/libxkbcommon.so.0
       $L/libwayland-client.so.0 $L/libwayland-cursor.so.0 $L/libwayland-egl.so.1 $L/libpulse.so.0
       $L/libdbus-1.so.3 $L/libudev.so.1 $L/libasound.so.2 /lib/i386-linux-gnu/libc.so.6 /lib/i386-linux-gnu/libm.so.6
       /lib/i386-linux-gnu/libpthread.so.0 /lib/i386-linux-gnu/libdl.so.2 /lib/i386-linux-gnu/librt.so.1
       /lib/i386-linux-gnu/libresolv.so.2"
rm -rf /out/runtime /out/licenses
mkdir -p /out/runtime /out/licenses
todo="$ROOTS"
seen=" "
while [ -n "$(echo $todo)" ]; do
    next=""
    for f in $todo; do
        case "$seen" in *" $f "*) continue ;; esac
        seen="$seen$f "
        for dep in $(ldd "$f" 2>/dev/null | awk '/=> \// {print $3}'); do
            next="$next $dep"
        done
    done
    todo="$next"
done
: >/tmp/pkgs
for f in $seen; do
    case "$f" in /sdl/*) continue ;; esac
    name=$(basename "$f")
    cp -L "$f" "/out/runtime/$name"
    real=$(readlink -f "$f")
    pkg=$(dpkg -S "$f" 2>/dev/null || dpkg -S "$real" 2>/dev/null || dpkg -S "/usr$f" 2>/dev/null || true)
    pkg=${pkg%%:*}
    [ -n "$pkg" ] && echo "$pkg" >>/tmp/pkgs
done
cp -L /lib/i386-linux-gnu/ld-linux.so.2 /out/runtime/ld-linux.so.2
echo libc6 >>/tmp/pkgs
: >/out/packages.txt
for p in $(sort -u /tmp/pkgs); do
    dpkg-query -W -f '${Package} ${Version} (source: ${source:Package} ${source:Version})\n' "$p:i386" >>/out/packages.txt
    cp "/usr/share/doc/$p/copyright" "/out/licenses/$p.copyright" 2>/dev/null ||
        cp "/usr/share/doc/$(dpkg-query -W -f '${source:Package}' "$p:i386")/copyright" "/out/licenses/$p.copyright"
done
chown -R "$OWNER" /out/runtime /out/licenses /out/packages.txt
