#!/usr/bin/env python3
"""The USB test kit's results (port/usbkit, from a Windows PC) against the same runs on Linux.

Usage: tools/usbkit_compare.py RESULTS_DIR [LINUX_BIN]
  RESULTS_DIR  one results\\<time> folder of the kit (summary.txt, system\\, runs\\<name>\\)
  LINUX_BIN    the Linux build to run (default build/port/twine)

Runs port/usbkit/runs.txt with the Linux build, one at a time, as test.ps1 runs them on Windows (each in
build/usbkit_linux/<name>, headless, dummy sound, no cards, TWINE_TURBO=1 unless the run says 0, under setarch -R as
tools/endless_check.py does), checks them as test.ps1 does, and prints for each run both results and how their logs
compare: equal, or the first line that differs (times of day and code addresses left out). Turbo runs on the same
build version should give the same log on both systems; paced runs (TWINE_TURBO=0) differ in timing by nature.
The Windows system report is printed first. Nothing here holds game data: the disc comes from TWINE_DISC or
workbench/disc/.
"""
import glob
import os
import re
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def runs():
    out = []
    for line in open(os.path.join(ROOT, "port", "usbkit", "runs.txt")):
        line = line.rstrip("\r\n")
        if not line or line.startswith("#"):
            continue
        name, secs, must, rest = line.split("|", 3)
        out.append((name, float(secs), must, dict(re.findall(r'"([^"=]+)=([^"]*)"', rest))))
    return out


def judge(log, must, stuck=False):
    over = bool(re.search(r"test: the run's \d+ vblanks are over", log))
    crash = bool(re.search(r"crash:|fatal:|watchdog: no frame", log))
    has = bool(re.search(must, log))
    return over and has and not crash and not stuck, "ended %s, crash %s, %r %s" % (over, crash, must,
                                                                                   "seen" if has else "not seen")


def normal(log):
    """lines to compare: no times of day, no code addresses, no host paths"""
    out = []
    for l in log.splitlines():
        # (what is the host's by nature: the program's own memory a save state holds and its build hash, the GPU)
        if re.match(r"savestate: \d+ bytes of the program's memory|gpu_hw: |video: display|setup: disc ", l):
            continue
        l = re.sub(r"^endless: \d\d:\d\d:\d\d\.\d+ ", "endless: ", l)
        l = re.sub(r"\b(from|at) [0-9A-F]{8}\b", r"\1 X", l)
        l = re.sub(r"wrote \S+", "wrote X", l)
        l = re.sub(r"\b0[0-9A-F]{7}\b", "HOSTPTR", l)  # (the host's own pointers: the game's are 80xxxxxx)
        if l.startswith(("savestate: saved", "savestate: loaded", "savestate: ", "video: display")):
            l = re.sub(r" /\S+| [A-Z]:\S+", " PATH", l)
        out.append(l)
    return out


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    res = sys.argv[1]
    linux = sys.argv[2] if len(sys.argv) > 2 else os.path.join(ROOT, "build", "port", "twine")
    disc = os.environ.get("TWINE_DISC") or (sorted(glob.glob(os.path.join(ROOT, "workbench", "disc", "*.bin"))) or [""])[0]
    if not disc:
        sys.exit("usbkit_compare: no disc (set TWINE_DISC)")
    for f in ("system/system.txt", "system/sysinfo.txt"):
        p = os.path.join(res, f)
        if os.path.exists(p):
            print("---- Windows: %s" % f)
            print(open(p, errors="replace").read().rstrip())
    summary = open(os.path.join(res, "summary.txt"), errors="replace").read() if os.path.exists(
        os.path.join(res, "summary.txt")) else ""
    print("---- Windows summary\n" + summary.rstrip())
    work = os.path.join(ROOT, "build", "usbkit_linux")
    setarch = ["setarch", "-R"] if shutil.which("setarch") else []
    print("---- the runs, Windows | Linux | logs")
    same_all = True
    for name, secs, must, env_add in runs():
        d = os.path.join(work, name)
        shutil.rmtree(d, ignore_errors=True)
        for s in ("conf/twine", "data/twine", "state"):
            os.makedirs(os.path.join(d, s))
        env = {k: v for k, v in os.environ.items() if not k.startswith(("TWINE_", "XDG_", "SDL_AUDIO"))}
        env.update({"XDG_CONFIG_HOME": os.path.join(d, "conf"), "XDG_DATA_HOME": os.path.join(d, "data"),
                    "XDG_STATE_HOME": os.path.join(d, "state"), "TWINE_HEADLESS": "1", "TWINE_MOUSE": "0",
                    "TWINE_NO_INTRO": "1", "TWINE_CARD1": "none", "TWINE_CARD2": "none",
                    "SDL_AUDIO_DRIVER": "dummy", "SDL_AUDIODRIVER": "dummy", "TWINE_DISC": disc, "TWINE_TURBO": "1",
                    "TWINE_TEST_VBLANKS": str(int(round(secs * 59.94)))})
        env.update({k: os.path.splitext(disc)[0] + ".cue" if v == "@cue" else v for k, v in env_add.items()})
        exe = os.path.join(d, "twinechk")
        os.symlink(os.path.abspath(linux), exe)
        stuck = False
        with open(os.path.join(d, "out.txt"), "w") as out:
            p = subprocess.Popen(["nice"] + setarch + [exe], cwd=d, env=env, stdout=out, stderr=subprocess.STDOUT)
            try:
                p.wait(timeout=secs * 2 + 60)
            except subprocess.TimeoutExpired:
                stuck = True
                p.kill()
                p.wait()
        llog = open(os.path.join(d, "port.log"), errors="replace").read() if os.path.exists(
            os.path.join(d, "port.log")) else ""
        lok, ldetail = judge(llog, must, stuck)
        wpath = os.path.join(res, "runs", name, "port.log")
        wlog = open(wpath, errors="replace").read() if os.path.exists(wpath) else ""
        wok, wdetail = judge(wlog, must)
        a, b = normal(wlog), normal(llog)
        if not wlog:
            cmp = "no Windows log"
        elif a == b:
            cmp = "logs equal"
        else:
            i = next((k for k in range(min(len(a), len(b))) if a[k] != b[k]), min(len(a), len(b)))
            cmp = "logs differ from line %d: Windows %r | Linux %r" % (
                i + 1, a[i][:100] if i < len(a) else "(end)", b[i][:100] if i < len(b) else "(end)")
            if env_add.get("TWINE_TURBO") != "0":
                same_all = False
        print("%-18s Windows %s | Linux %s | %s" % (name, "PASS" if wok else "FAIL", "PASS" if lok else "FAIL", cmp))
        if not wok:
            print("    Windows: " + wdetail)
        if not lok:
            print("    Linux: " + ldetail)
    print("usbkit_compare: turbo runs' logs %s" % ("all equal" if same_all else "differ (see above)"))


if __name__ == "__main__":
    main()
