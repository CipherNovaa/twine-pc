"""Names the samples of a TWINE_PROFILE=1 run (profile.txt, one program counter per line) after the native
binary's functions, the recompiled ones by the PS1 function they come from.

Usage: profile.py BINARY [profile.txt] [N]   -> the N (30) functions with the most samples"""
import bisect
import subprocess
import sys
from collections import Counter


def main():
    binary = sys.argv[1]
    prof = sys.argv[2] if len(sys.argv) > 2 else "profile.txt"
    top = int(sys.argv[3]) if len(sys.argv) > 3 else 30
    syms = []
    for line in subprocess.run(["nm", "-n", "--defined-only", binary], capture_output=True, text=True).stdout.splitlines():
        f = line.split()
        if len(f) == 3 and f[1] in "tTwW":
            syms.append((int(f[0], 16), f[2]))
    addrs = [a for a, _ in syms]
    counts = Counter()
    total = 0
    for line in open(prof):
        pc = int(line, 16)
        i = bisect.bisect_right(addrs, pc) - 1
        counts[syms[i][1] if i >= 0 else "?"] += 1
        total += 1
    print(f"{total} samples")
    for name, n in counts.most_common(top):
        print(f"{100 * n / total:6.2f}%  {name}")


if __name__ == "__main__":
    main()
