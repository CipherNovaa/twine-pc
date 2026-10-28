#!/bin/bash
# The Windows build's guard (workbench/research/NOTES_windows.md): code outside the platform layer may use POSIX only
# as far as port/src/win + port/src/platform_win.c give it on Windows. Flags:
#   - #include of a system header MinGW lacks that port/src/win doesn't supply;
#   - calls the Windows platform layer has no version of (processes, dynamic loading, users, terminals, sockets...);
#   - Linux-only addresses and files (/proc other than /proc/self/exe, 0x0804xxxx program addresses).
# Exit 1 when something is found. (The full check is `make -C port windows` itself: an unsupported call doesn't
# compile or link there.)
#   tools/check_posix.sh
cd "$(dirname "$0")/.." || exit 2
MINGW=/usr/i686-w64-mingw32/include
GCCINC=$(i686-w64-mingw32-gcc -print-file-name=include 2>/dev/null)
FILES=$(ls port/src/*.c port/src/*.h port/mods/*.c port/fixes/*.c src/mods/*.c 2>/dev/null | grep -v 'platform_win\.c$')
bad=0

for f in $FILES; do
    grep -no '#include <[^>]*>' "$f" | while IFS=: read -r line inc; do
        h=${inc#\#include <}
        h=${h%>}
        case $h in SDL3/*) continue ;; esac
        if [ ! -e "port/src/win/$h" ] && [ ! -e "$MINGW/$h" ] && [ ! -e "$GCCINC/$h" ]; then
            echo "$f:$line: <$h>: no such header on Windows (add it to port/src/win, or use what is there)"
        fi
    done
done | tee /dev/stderr | grep -q . && bad=1

CALLS='fork|vfork|execl|execlp|execle|execv|execvp|execve|popen|pclose|waitpid|wait|dlopen|dlsym|dlclose|getuid|geteuid|getpwnam|getpwuid|kill|alarm|pause|sigsuspend|sigwait|ioctl|tcgetattr|tcsetattr|socket|connect|bind|listen|accept|select|poll|epoll_create|inotify_init|symlink|link|chown|fchmod|mkfifo|mlock|madvise|shm_open|sem_open|posix_spawn|ptrace|prctl|sched_setaffinity|syscall'
grep -nwE "($CALLS)[[:space:]]*\(" $FILES | grep -vE '^[^:]+:[0-9]+:[[:space:]]*(/?\*|//)' |
    sed 's/$/   <- not in the Windows platform layer/' | tee /dev/stderr | grep -q . && bad=1

# (the Linux side of an #ifdef _WIN32 / #ifndef _WIN32 is left out)
windows_side() {
    awk '{ if ($0 ~ /^#if(ndef|def) _WIN32/) { d = d + 1; lin[d] = ($0 ~ /ifndef/); nest[d] = 0; print ""; next }
           if (d && $0 ~ /^#if/) nest[d]++
           if (d && $0 ~ /^#endif/) { if (nest[d]) nest[d]--; else { d = d - 1; print ""; next } }
           if (d && !nest[d] && $0 ~ /^#else/) { lin[d] = !lin[d]; print ""; next }
           skip = 0; for (i = 1; i <= d; i++) if (lin[i]) skip = 1
           print (skip ? "" : $0) }' "$1"
}
for f in $FILES; do
    windows_side "$f" | grep -nE '"/proc/|0x0804[0-9A-Fa-f]{4}' | grep -v '/proc/self/exe' | sed "s|^|$f:|; s/\$/   <- Linux-only path or address/"
done | tee /dev/stderr | grep -q . && bad=1

[ $bad = 0 ] && echo "check_posix: nothing outside the platform layer"
exit $bad
