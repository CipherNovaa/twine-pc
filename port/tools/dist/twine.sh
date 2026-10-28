#!/bin/sh
# TWINE launcher (the `twine` file of the Linux download). Run it from a terminal, a file manager or Steam ("Add a
# non-Steam game"). It starts the game the best way this system allows:
#   - the system's own 32-bit libraries when it has them (Steam Deck, and any PC with Steam installed): the graphics
#     card's 32-bit driver then draws;
#   - otherwise the 32-bit runtime in runtime/ (glibc, X11, Wayland, PulseAudio client), so nothing needs installing.
# TWINE_RUNTIME=system or =bundled picks one. Arguments go to the game (--setup: the disc setup screen again).
# The log goes to ~/.local/state/twine/twine.log (TWINE_LOG); after a crash the game says where it is.

here=$(cd "$(dirname "$0")" && pwd -P) || exit 1
game=${TWINE_EXE:-$here/bin/twine} # (TWINE_EXE: another build or a link to it, for tests)
runtime="$here/runtime"

say() { # a message the player sees: in a dialog when there is a desktop, and on the terminal
    printf 'TWINE: %s\n' "$1" >&2
    if [ -n "${DISPLAY:-}${WAYLAND_DISPLAY:-}" ] && [ -z "${TWINE_HEADLESS:-}" ]; then
        if command -v zenity >/dev/null 2>&1; then
            zenity --error --title=TWINE --text="$1" >/dev/null 2>&1 && return
        fi
        if command -v kdialog >/dev/null 2>&1; then
            kdialog --title TWINE --error "$1" >/dev/null 2>&1 && return
        fi
        command -v notify-send >/dev/null 2>&1 && notify-send TWINE "$1" >/dev/null 2>&1
    fi
}

case "$(uname -m)" in
x86_64 | i?86) ;;
*)
    say "TWINE runs on x86 PCs (Intel or AMD); this one is $(uname -m)."
    exit 1
    ;;
esac
if [ ! -x "$game" ]; then
    say "The game program is missing from $here/bin. Unpack the whole download again."
    exit 1
fi

# a 32-bit library of the system's, by name
has32() {
    if ldconfig -p 2>/dev/null | grep -q "[[:space:]]$1 (libc6)"; then
        return 0
    fi
    for d in /usr/lib32 /usr/lib/i386-linux-gnu /lib/i386-linux-gnu /usr/lib /lib; do
        # (an ELF file whose class byte says 32-bit)
        if [ -f "$d/$1" ] && [ "$(od -An -tx1 -j4 -N1 "$d/$1" 2>/dev/null | tr -d ' ')" = 01 ]; then
            return 0
        fi
    done
    return 1
}

# the system's C library, new enough for the game (2.34)
glibc_ok() {
    v=$(getconf GNU_LIBC_VERSION 2>/dev/null | awk '{print $2}')
    major=${v%%.*}
    minor=${v#*.}
    minor=${minor%%.*}
    [ -n "$v" ] && { [ "$major" -gt 2 ] || { [ "$major" -eq 2 ] && [ "$minor" -ge 34 ]; }; }
}

mode=${TWINE_RUNTIME:-auto}
if [ "$mode" = auto ]; then
    mode=bundled
    if [ -e /lib/ld-linux.so.2 ] && glibc_ok && has32 libc.so.6 && has32 libX11.so.6; then
        mode=system
    fi
fi

if [ "$mode" = system ]; then
    echo "TWINE: running on the system's 32-bit libraries" >&2
else
    echo "TWINE: running on the bundled 32-bit runtime" >&2
fi

state=${XDG_STATE_HOME:-$HOME/.local/state}/twine
if [ -z "${TWINE_LOG:-}" ] && mkdir -p "$state" 2>/dev/null; then
    TWINE_LOG="$state/twine.log"
    [ -f "$TWINE_LOG" ] && mv -f "$TWINE_LOG" "$TWINE_LOG.old" 2>/dev/null
    export TWINE_LOG
fi
TWINE_HOME=$here # (where the game looks for disc/ and bios/ folders the player may add)
export TWINE_HOME

run() {
    if [ "$mode" = system ]; then
        "$game" "$@"
    else
        # the bundled loader and libraries; X11 first, as there are no 32-bit OpenGL/Vulkan drivers in this mode for
        # SDL's Wayland window (its software picture works through X11, which every desktop has, XWayland too)
        SDL_VIDEO_DRIVER=${SDL_VIDEO_DRIVER:-x11,wayland} "$runtime/ld-linux.so.2" --library-path "$here/bin/lib:$runtime" \
            --argv0 "$game" "$game" "$@"
    fi
}

err=$(mktemp "${TMPDIR:-/tmp}/twine.XXXXXX" 2>/dev/null) || err=/dev/null
run "$@" 2>"$err"
code=$?
[ "$err" != /dev/null ] && { cat "$err" >&2; }
case $code in
0 | 1 | 130 | 143) ;; # (1: the game has said why itself; 130, 143: stopped with Ctrl+C or by the system)
126 | 127)
    if grep -q "Exec format error\|cannot execute binary" "$err" 2>/dev/null; then
        say "This Linux system can't run 32-bit programs (its kernel has IA32 emulation turned off)."
    else
        say "TWINE couldn't start: $(head -c 400 "$err" 2>/dev/null). Try TWINE_RUNTIME=bundled $0"
    fi
    ;;
*)
    if [ "$code" -gt 128 ]; then
        run --crash-notice "${TWINE_LOG:-port.log}" "signal $((code - 128))" 2>/dev/null ||
            say "TWINE stopped unexpectedly (signal $((code - 128))). The log is at ${TWINE_LOG:-port.log}"
    fi
    ;;
esac
[ "$err" != /dev/null ] && rm -f "$err"
exit $code
