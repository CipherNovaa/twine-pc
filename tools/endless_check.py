#!/usr/bin/env python3
"""Endless mode's regression suite: headless runs of the native port, checked from their logs and screenshots.

Usage: tools/endless_check.sh [-j JOBS] [-k CHECKS] [-r ROOMS] [-v] [-8] [-o] [-p]
  -j JOBS    runs at once (default 4; at most 12, paced at most 5). A turbo run takes a whole core while it runs: 8 suit
             the 16-core machine when it is otherwise quiet; a run waits to start while the load is over the core count
  -k CHECKS  the checks to run, letters from abcdefghijklmnopqr (default all):
             a guards fight, b nobody unkillable, c spawns valid (a, b and c share each pool room's run),
             d room rules, e menus, f restart (and a lent automatic's looping fire stopped through a door, a restart,
             the menu), g weapon carry, h card save, i campaign regression,
             j enemy AI (hunting: guards close on a still Bond, walking by the floor; no bunching),
             k lent weapons as at home (fire rate, damage, scope, sound; ammo from drops for every weapon carried),
             l room starts never run out of the menu texts' letters (20 starts; past them: cut, no crash),
             (i also: smooth motion's extra drawings never run the hit loop: per tick as without, campaign and a run)
             m a run started from the shell (main menu, mission select's ENDLESS, its briefing) with no weapons
               catalogue: the briefing seen, the survey then the first room; a survey stalled is given up after 10 s,
             n exits: every pool room's doors walked to and used once the exit is open (each opens, or, kept locked
               by the mission, leads out all the same; none just refuses), and the entry's way out
             o the early waves' ramp: Meltdown 4 (six guards of its own) as waves 1, 3 and 5 against no ramp: the
               damage Bond takes standing still, its bursts, the hunters at once
             p more than six people (8 MB of RAM, whatever -8 says: 12 slots): 12 alive at once, drawn, each fires
               and falls, real shots hit and kill people in the extra slots (hw renderer, smooth motion), a save
               state with 12 about, a long run of rooms
             q King's Ransom's bar (8:5), 2 MB and 8 MB: guards sent in every minute, by Bond's door and the far side
             r the weapon list: own and lent weapons each an entry with a checkmark; the weapon buttons (wheel, Z/X,
               d-pad) step through the checked ones, Q/L1 the one held before, number keys by group; checkmarks kept
               for the run; every lent weapon reached; the campaign's stepping the game's own; the HUD strip
  -r ROOMS   pool rooms for a-c, as level:point,... (default: every room in port/src/endless.c's table)
  -v         print each failing run's log tail
  -8         every run with 8 MB of RAM (the ram_8mb setting)
  -o         every run with the game's own enemy AI (enemy_ai = original; default: the port's, IMPROVED)
  -p         paced: every run in real time, as played (default: TWINE_TURBO, game time as fast as the machine
             runs it; the same frames, vblanks, controls and sound, irq.c). Either way each run lasts its seconds
             of game time (TWINE_TEST_VBLANKS); build/endless_check/times.txt has each run's time on the wall clock

Environment:
  TWINE_DISC         the disc image (default: the first .bin in workbench/disc/)
  TWINE_CHECK_CARD   a memory card image with a campaign save, for h (default ~/.local/share/twine/card1.mcd);
                     only a copy is ever used
  TWINE_CHECK_DIR    where the runs go (default build/endless_check/)
  TWINE_CHECK_ENV    NAME=VALUE;... added to every run (tests of the harness: TWINE_TURBO_LOOK_NS=0 makes LEVEL07's
                     busy wait stick)

Each run has its own folders (config, data, state, cards) and runs as "twinechk" (a link to build/port/twine), so
it never touches the user's settings or cards and isn't mistaken for the game. Runs are headless with SDL's dummy
sound driver (they never connect to the computer's sound server). A run is never cut short by the wall clock as
such: only when its game is stuck (the watchdog's "no frame shown", in turbo 6 s of the game's own CPU time, and 30 s
more) or past secs * 20 + 900 s; it is then run once more, and if stopped again its checks say STOP (the harness's
doing, the check's verdict unknown), never FAIL. A check over a run that asked for the hw renderer and drew in
software (no GPU) says ENV, not FAIL. Exit code 0 when every check passed, 1 if any failed, 2 if none failed but
some are STOP or ENV.
Nothing here holds game data: it all comes from the disc at run time.
"""
import concurrent.futures as cf
from collections import Counter
import glob
import hashlib
import math
import os
import re
import shutil
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.path.join(ROOT, "build", "port", "twine")


def disc():
    d = os.environ.get("TWINE_DISC")
    if d:
        return d
    found = sorted(glob.glob(os.path.join(ROOT, "workbench", "disc", "*.bin")))
    if not found:
        sys.exit("endless_check: no disc (set TWINE_DISC)")
    return found[0]


def pool_rooms():
    src = open(os.path.join(ROOT, "port", "src", "endless.c")).read()
    body = src[src.index("static Room rooms[] = {"):]
    body = body[:body.index("};")]
    rooms = [(int(a), int(b)) for a, b in re.findall(r"\{(-?\d+),\s*(-?\d+)\}", body)]
    return [r for r in rooms if r[0] > 0]


class Run:
    """one headless run: level, seconds of game time, extra environment; .log is its port.log afterwards.
    The run ends itself when the game's vblank count reaches the seconds' worth (TWINE_TEST_VBLANKS, 59.94 a second),
    paced or not; the seconds on the wall clock only bound a run that never gets there (stuck)."""

    def __init__(self, name, level, secs, env=None, card=None, config=None):
        self.name, self.level, self.secs, self.env, self.card, self.config = name, level, secs, env or {}, card, config
        self.dir = os.path.join(WORK, name)
        self.log = ""
        self.timed_out = False  # (stopped by the harness, never reaching its vblanks: .stopped says why)
        self.stopped = ""
        self.wall = 0.0

    def cost(self):
        """a guess at its time on the wall clock, for starting the longest first: smooth motion's extra drawings take
        host time whatever the clock (about 4 ms each, 10 a tick); otherwise a turbo run takes a fraction of its secs"""
        if not TURBO:
            return self.secs
        smooth = str(self.env.get("TWINE_SMOOTH", "0")) == "1"
        return self.secs * (1.0 if smooth else 0.15) + 3

    def go(self):
        if TURBO:  # (a turbo run takes a whole core: none started while the machine is busy, up to 10 min)
            for _ in range(300):
                if os.getloadavg()[0] < (os.cpu_count() or 4) - 1:
                    break
                time.sleep(2)
        shutil.rmtree(self.dir, ignore_errors=True)
        for d in ("conf/twine", "data/twine", "state"):
            os.makedirs(os.path.join(self.dir, d), exist_ok=True)
        if self.config:
            open(os.path.join(self.dir, "conf/twine/twine.ini"), "w").write(self.config)
        exe = os.path.join(self.dir, "twinechk")
        os.symlink(BIN, exe)
        env = dict(os.environ)
        for k in list(env):
            if k.startswith("TWINE_") and k != "TWINE_DISC":
                del env[k]
        env.update({"XDG_CONFIG_HOME": os.path.join(self.dir, "conf"), "XDG_DATA_HOME": os.path.join(self.dir, "data"),
                    "XDG_STATE_HOME": os.path.join(self.dir, "state"), "TWINE_HEADLESS": "1", "TWINE_MOUSE": "0",
                    "TWINE_NO_INTRO": "1", "TWINE_LEVEL": str(self.level), "TWINE_DISC": DISC,
                    "TWINE_CARD2": "none",
                    # (never the computer's sound server: many runs at once wedged it; the game's sound via TWINE_WAV)
                    "SDL_AUDIO_DRIVER": "dummy", "SDL_AUDIODRIVER": "dummy"})
        if self.card:
            shutil.copy(self.card, os.path.join(self.dir, "data/twine/card1.mcd"))
        else:
            env["TWINE_CARD1"] = "none"
        env.update({k: str(v) for k, v in self.env.items()})
        for kv in filter(None, os.environ.get("TWINE_CHECK_ENV", "").split(";")):  # (tests of the harness itself)
            env[kv.split("=", 1)[0]] = kv.split("=", 1)[1]
        env["TWINE_TEST_VBLANKS"] = str(int(round(self.secs * 59.94)))
        if TURBO:  # (game time: as fast as the machine runs it, the same frames, vblanks, controls and sound)
            env["TWINE_TURBO"] = "1"
        if RAM8:
            env["TWINE_RAM_8MB"] = "1"
        if AI_ORIGINAL and "TWINE_ENEMY_AI" not in self.env:
            env["TWINE_ENEMY_AI"] = "original"
        t0 = time.time()
        with open(os.path.join(self.dir, "out.txt"), "w") as out:
            # (setarch -R: the same addresses every run. Some of the game's code reads memory it never set (a blast's
            # debris in Fallen Angel at f1088 of k_gl_lent comes out in one of three places), finding there what the
            # host's address randomization left: with it, two runs of the same build can differ)
            child = subprocess.Popen(["nice"] + SETARCH + [exe], cwd=self.dir, env=env, stdout=out,
                                     stderr=subprocess.STDOUT)
            # (no bound on the wall clock that a loaded machine can reach: a run is stopped when its game is stuck, the
            # watchdog's "no frame shown" (in turbo: 6 s of the game thread's own CPU time) standing for 30 s, or
            # past secs * 20 + 900 s whatever it does; only this run's own process)
            self.timed_out, self.stopped = False, ""
            seen, stuck_since, log = 0, None, os.path.join(self.dir, "port.log")
            stuck = False
            while True:
                try:
                    child.wait(timeout=5)
                    break
                except subprocess.TimeoutExpired:
                    pass
                try:
                    with open(log, "rb") as f:
                        f.seek(seen)
                        new = f.read()
                    seen += len(new)
                    for m in re.finditer(rb"watchdog: (no frame shown|frames again)", new):
                        stuck = m.group(1) == b"no frame shown"
                except OSError:
                    pass
                now = time.time()
                stuck_since = (stuck_since or now) if stuck else None
                if stuck_since and now - stuck_since >= 30:
                    self.stopped = "stuck: no frame shown for 6 s and 30 s more"
                elif now - t0 > self.secs * 20 + 900:
                    self.stopped = "over %d s on the wall clock" % (self.secs * 20 + 900)
                if self.stopped:
                    self.timed_out = True
                    child.terminate()
                    try:
                        child.wait(timeout=5)
                    except subprocess.TimeoutExpired:
                        child.kill()
                        child.wait()
                    break
        self.wall = time.time() - t0
        try:
            self.log = open(os.path.join(self.dir, "port.log"), errors="replace").read()
        except OSError:
            self.log = ""
        return self

    def hw_fell_back(self):
        """the hardware renderer asked for and not had (no GPU for new programs, e.g. its driver updated under a running
        kernel): the run drew in software, its hw shots missing: the machine's problem, not the check's"""
        return str(self.env.get("TWINE_RENDERER", "")).lower() == "hw" and "no hardware renderer" in self.log

    def lines(self, pat):
        return [l for l in self.log.splitlines() if re.search(pat, l)]

    def crashed(self):
        return bool(re.search(r"SIGSEGV|segmentation|watchdog: no frame|fatal", self.log, re.I))

    def shot(self, what):
        """the shot the log says was taken of `what` (fixes/shell.c's test shots), as rows of RGB, or None"""
        lines = self.log.splitlines()
        for i, l in enumerate(lines):
            if "test: a shot of " + what in l:
                for m in lines[i:i + 40]:
                    g = re.search(r"wrote (shot_\d+\.ppm)", m)
                    if g:
                        return read_ppm(os.path.join(self.dir, g.group(1)))
        return None


def read_ppm(path):
    try:
        data = open(path, "rb").read()
    except OSError:
        return None
    parts = data.split(maxsplit=4)
    w, h = int(parts[1]), int(parts[2])
    px = parts[4] if len(parts) > 4 else b""
    return w, h, px


def bright(img, x0, y0, x1, y1, level=90):
    """the share of pixels in the box (fractions of the picture) brighter than level"""
    w, h, px = img
    n = hit = 0
    for y in range(int(y0 * h), int(y1 * h), 2):
        for x in range(int(x0 * w), int(x1 * w), 2):
            i = (y * w + x) * 3
            if i + 2 < len(px):
                n += 1
                hit += max(px[i], px[i + 1], px[i + 2]) > level
    return hit / n if n else 0.0


def motorola(img):
    """the share of the pause menu's watch frame (the Motorola's grey case: its left edge) found in a screen"""
    w, h, px = img
    n = hit = 0
    for y in range(int(0.2 * h), int(0.9 * h), 2):
        for x in range(0, int(0.03 * w), 2):
            i = (y * w + x) * 3
            if i + 2 < len(px):
                r, g, b = px[i], px[i + 1], px[i + 2]
                n += 1
                hit += min(r, g, b) > 150 and max(r, g, b) - min(r, g, b) < 25
    return hit / n if n else 0.0


def people(run):
    """the people lines: (frame, slot, type, how, health, ai, x, y, z, floor, flags, w4, +4C, +E)"""
    out = []
    for l in run.lines(r"people: \d+ type"):
        m = re.search(r" f(-?\d+) v\d+ people: (\d+) type (\d+) (.+?), health (-?\d+), ai (\d+) at (-?\d+) (-?\d+) (-?\d+) "
                      r"\(floor (-?\d+).*?flags ([0-9A-F]+) w4 ([0-9A-F]+).*?\+4C ([0-9A-F]+)(?: \+E ([0-9A-F]+))?", l)
        if m:
            g = m.groups()
            out.append(dict(f=int(g[0]), slot=int(g[1]), type=int(g[2]), how=g[3], health=int(g[4]), ai=int(g[5]),
                            x=int(g[6]), y=int(g[7]), z=int(g[8]), floor=int(g[9]), flags=int(g[10], 16),
                            w4=int(g[11], 16), c=int(g[12], 16), e=int(g[13] or "0", 16)))
    return out


RESULTS = []
SETARCH = ["setarch", "-R"] if shutil.which("setarch") else []
JOBS_MAX = 12
TURBO = True
RAM8 = False
AI_ORIGINAL = False


def checked(runs, fn, *args):
    """a check over these runs: what it would call failed is STOP when the harness had to stop one of them (stuck, or
    past its bound: never reaching its vblanks), the run's log cut short: the check's verdict is not known; ENV when
    it is a check of the hw renderer's ("hw" in its name) and one of the runs asked for it but drew in software"""
    n = len(RESULTS)
    fn(*args)
    stopped = [r for r in runs if r.timed_out]
    soft = [r for r in runs if r.hw_fell_back()]
    if stopped or soft:
        why = "; ".join(["%s stopped: %s" % (r.name, r.stopped) for r in stopped] +
                        ["%s: hw renderer asked, no GPU (software)" % r.name for r in soft])
        for i in range(n, len(RESULTS)):
            c, w, st, d = RESULTS[i]
            # (ENV only for a check that says it is the hw renderer's: the group's software runs still count)
            if st == "FAIL" and (stopped or re.search(r"\bhw\b", w)):
                RESULTS[i] = (c, w, "STOP" if stopped else "ENV", ("[%s] " % why) + d)


def result(check, what, ok, detail=""):
    RESULTS.append((check, what, "PASS" if ok is True else "SKIP" if ok is None else "FAIL", detail))


# ---- a, b, c: each pool room, Bond unhurt, a guard sent and Bond put by him ----

def room_run(room):
    l, p = room
    return Run("room_%d_%d" % (l, p if p >= 0 else 99), l, 72,
               {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "%d:%d" % room, "TWINE_ENDLESS_SEED": 4,
                "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_TEST_MEET": 1, "TWINE_ENDLESS_TEST_NOISE": 1,
                "TWINE_ENDLESS_TEST_SHOOT": 50})


def check_room(run, room, checks):
    name = "%d:%d" % room
    if run.crashed() or "room start" not in run.log:
        for c in checks:
            if c in "abc":
                result(c, name, False, "crashed or never started")
        return
    if "a" in checks:  # (damage dealt to Bond before the shooting test begins: by a sent guard Bond was put by, or
        # failing one sent in time, by the room's own)
        shoot = run.log.find("test shot:")
        shoot = shoot if shoot >= 0 else len(run.log)
        met = run.log.find("test: Bond put")
        if 0 <= met < shoot:
            n = len(re.findall(r"damage: \d+: health", run.log[met:shoot]))
            result("a", name, n > 0, "sent guard met: %d hits on Bond" % n)
        else:
            n = len(re.findall(r"damage: \d+: health", run.log[:shoot]))
            result("a", name, n > 0 or None, "no guard sent in time (slots full); the room's own: %d hits" % n)
    ppl = people(run)
    if "b" in checks:  # (from 50 s on everyone is shot every 2 s past the game's hit tests: each must fall)
        immune = sorted({l.split("test shot: ")[1] for l in run.lines(r"test shot: .* immune")})
        shot = run.lines(r"test shot: person \d+ type \d+ health")
        fell = [l for l in shot if re.search(r"-> (-\d+|0)$", l)]
        result("b", name, bool(not immune and (not shot or fell)),
               "; ".join(immune)[:160] or "%d shots, %d fell" % (len(shot), len(fell)))
    if "c" in checks:
        bad = []
        off = {}  # (three samples running with no floor under him: a moment's knock-back or a stair's edge is not it)
        for q in ppl:
            k = q["slot"]
            off[k] = off.get(k, 0) + 1 if q["health"] > 0 and q["floor"] == -32767 else 0
            if off[k] == 5:  # (a doorway's threshold has none under it either: a while, not a moment)
                bad.append("type %d on no floor at %d %d %d" % (q["type"], q["x"], q["y"], q["z"]))
        bad += ["removed while seen: " + l.split("removed: ")[1][:60] for l in run.lines(r"removed: .*Bond could see")
                if "no floor under him" not in l]  # (a guard just made inside a wall goes at once, seen or not)
        bad += ["copy posed (type %d)" % q["type"] for q in ppl if q["how"] == "sent" and q["flags"] & 0x400]
        result("c", name, not bad, "; ".join(sorted(set(bad)))[:200] or "%d spawns" % len(run.lines(r"spawn \(")))


# ---- d: the room's own guards and snipers last; civilians kept ----

def check_rules(runs):
    kr = runs["rules_kr"]
    ppl = people(kr)
    if not ppl:
        result("d", "8:4 fixed guards last", False, "no people logged")
    else:
        first = min(q["f"] for q in ppl)
        last = max(q["f"] for q in ppl)
        fixed = {(q["type"], q["x"] // 500, q["z"] // 500) for q in ppl if q["f"] == first and q["how"] == "placed" and 2 <= q["ai"] <= 5}
        alive = {(q["type"], q["x"] // 500, q["z"] // 500) for q in ppl if q["f"] == last and q["health"] > 0}
        gone = fixed - alive
        result("d", "8:4 fixed guards last %ds" % ((last - first) // 25), bool(fixed) and not gone,
               "%d fixed (ai 2-5), %d gone" % (len(fixed), len(gone)))
    r = runs["rules_hurry"]
    muted = r.lines(r"story line 101 muted")
    played = r.lines(r"sound: \d+ flags \w+ \(program 9 tone 7 note 80 voice \d+\): handle")
    result("d", "8:5 \"hurry James\" muted (hw renderer, smooth motion)", bool(muted) and not played,
           "muted %d, played %d" % (len(muted), len(played)))
    co = runs["rules_courier"]
    civ = [l for l in co.lines(r"made: person \d+ type 5 placed")]
    result("d", "2:-1 civilians kept", len(civ) > 0, "%d bank staff made" % len(civ))
    made60 = co.lines(r"made: person \d+ type 60 placed")
    her = co.lines(r"trace: L2 f\d+ person \d+ type 60: ")
    anims = Counter(re.search(r"anim (-?\d+/-?\d+)", l).group(1) for l in her)
    near = co.lines(r"test: Bond put \d+ from person \d+ \(type 60\)")
    last = max([int(f) for f in re.findall(r"endless: \S+ f(\d+) ", co.log)] or [0])
    result("d", "2:-1 receptionist seated, still (Bond armed before her; hw, smooth)",
           bool(made60) and bool(near) and bool(her) and set(anims) == {"60/60"} and last > 400,
           "made %d, Bond before her %d, her animations (asked/running) %s to frame %d" %
           (len(made60), len(near), dict(anims), last))
    result("d", "2:-1 no story people made", True, "story people left out: %d" % len(co.lines(r"story person")))


# ---- e: the menus after a run's stats, and the main menu ----

def menu_runs():
    runs = {"menu_main": Run("menu_main", 0, 40, {"TWINE_TEST_NO_FILMS": 1, "TWINE_TEST_SHELL_SHOTS": 1, "TWINE_SMOOTH": 0})}
    for name, pad in (("quick_x", "470:4000,473:0"), ("quick_start", "470:8,473:0"), ("late_start", "990:8,993:0"),
                      ("late_tri_start", "990:1000,993:0,1160:8,1163:0"), ("late_x", "990:4000,993:0")):
        runs["menu_" + name] = Run("menu_" + name, 7, 85 if name.startswith("late") else 60,
                                    {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "7:2", "TWINE_ENDLESS_TEST_DIE": 150,
                                     "TWINE_TEST_PAD": pad, "TWINE_TEST_SHELL_SHOTS": 1, "TWINE_SMOOTH": 0})
    card = os.environ.get("TWINE_CHECK_CARD", os.path.expanduser("~/.local/share/twine/card1.mcd"))
    if os.path.exists(card):  # (the main menu's cursor on CONTINUE needs the autoloaded save)
        pad = "400:40,403:0,460:4000,463:0,530:40,533:0,560:40,563:0,620:4000,623:0," + \
              ",".join("%d:40,%d:0" % (t, t + 3) for t in (750, 780, 810, 840, 870, 900)) + ",950:20,953:0"
        runs["menu_cheats"] = Run("menu_cheats", 0, 45, {"TWINE_TEST_NO_FILMS": 1, "TWINE_SMOOTH": 0, "TWINE_HD": 0,
                                                         "TWINE_TEST_PAD": pad}, card=card,
                                  config="infinite_grenades = 1\n")
    return runs


def check_menus(runs):
    if "menu_cheats" in runs:
        r = runs["menu_cheats"]
        ini = open(os.path.join(r.dir, "conf/twine/twine.ini")).read()
        result("e", "cheats: infinite grenades", bool(r.lines(r"infinite grenades off \(cheats\)")) and
               "infinite_grenades = 0" in ini, "toggled %d, ini %s" % (len(r.lines("infinite grenades")),
                                                                     "saved" if "= 0" in ini else "unchanged"))
    img = runs["menu_main"].shot("the main menu")
    if img is None:
        result("e", "main menu", False, "no shot")
    else:
        b = bright(img, 0.6, 0.25, 0.95, 0.8)
        result("e", "main menu items", b > 0.04, "items area %.0f%% bright" % (b * 100))
    for k, run in runs.items():
        if k in ("menu_main", "menu_cheats"):
            continue
        img = run.shot("mission select")
        if img is None:
            result("e", k[5:], False, "mission select never shown" + (" (crash)" if run.crashed() else ""))
            continue
        tiles = bright(img, 0.13, 0.25, 0.87, 0.72)
        name = bright(img, 0.2, 0.76, 0.8, 0.84, 120)
        result("e", k[5:], tiles > 0.06 and name > 0.18,  # (broken: tiles 0%, name 10%; whole: 15%, 26%)
               "tiles %.0f%%, name %.0f%% bright" % (tiles * 100, name * 100))


# ---- f: the watch's RESTART MISSION ----

def restart_runs():
    pad = "200:8,203:0," + ",".join("%d:40,%d:0" % (t, t + 3) for t in (280, 310, 340, 370, 400, 430)) + ",470:4000,473:0"
    env = {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "2:4"}
    return {"restart_yes": Run("restart_yes", 2, 45, dict(env, TWINE_TEST_PAD=pad + ",560:20,563:0,600:4000,603:0")),
            "restart_no": Run("restart_no", 2, 45, dict(env, TWINE_TEST_PAD=pad + ",600:4000,603:0"))}


def sound_stop_runs():
    """a lent automatic's looping fire (King's Ransom's Ingalls lent in City of Walkways) held as Bond goes
    through a door, as RESTART MISSION comes, as the menu opens (hw renderer, smooth motion, pad-held fire)"""
    env = {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_SEED": 5, "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_TEST_ARM": 1,
           "TWINE_ENDLESS_TEST_DRAW": "INGALLS", "TWINE_ENDLESS_ROOM": "8:-1", "TWINE_ENDLESS_NEXT": "5:-1,6:0",
           "TWINE_TRACE": "weapon,voices,sound", "TWINE_RENDERER": "hw", "TWINE_SMOOTH": 1}
    return {"snd_door": Run("snd_door", 8, 50, dict(env, TWINE_ENDLESS_TEST_EXIT="200,300", TWINE_TEST_PAD="600:4000,700:0")),
            "snd_restart": Run("snd_restart", 8, 50, dict(env, TWINE_ENDLESS_TEST_EXIT="200,5000",
                                                          TWINE_ENDLESS_TEST_RESTART=250, TWINE_TEST_PAD="540:4000,800:0")),
            "snd_menu": Run("snd_menu", 8, 45, dict(env, TWINE_ENDLESS_TEST_EXIT="200,5000",
                                                    TWINE_TEST_PAD="500:4000,600:4008,603:0,800:8,803:0"))}


def firing_lent(log, level):
    """the lent Ingalls firing (its looping sound keyed, state 9) at the last weapon frame traced in level"""
    w = re.findall(r"trace: L%d f\d+ weapon: slot \d+ \(.*?\) INGALLS TYPE 20, state (\d+)" % level, log)
    return bool(w) and w[-1] == "9" and bool(re.search(r"trace: L%d f\d+ sound: lent .* looping" % level, log))


def leftovers(log, level):
    """the voices lines of level's first room after the change: (lines, those with sounds from before it)"""
    after = log.split("room in level %d " % level, 1)[1] if "room in level %d " % level in log else ""
    lines = re.findall(r"voices: room frame \d+: .*keyed on before this executable: (\d+)(.*)", after)
    return lines, [l for l in lines if l[0] != "0"]


def check_sound_stop(runs):
    d, r, m = runs["snd_door"], runs["snd_restart"], runs["snd_menu"]
    lines, bad = leftovers(d.log, 6)
    pre = firing_lent(d.log.split("room in level 6 ", 1)[0], 5)
    result("f", "a lent automatic's fire held through a door: no sound left on (hw, smooth)",
           None if not pre else len(lines) >= 3 and not bad,
           ("firing as Bond left: %s; next room: %d checks, %d with a sound from before%s" %
            (pre, len(lines), len(bad), (" (" + bad[0][1][:80] + ")") if bad else "")))
    two = r.log.split("restart: RESTART MISSION: the run begins again", 1)
    m2 = re.search(r"restart: a fresh run from level (\d+)", r.log)
    lines, bad = leftovers(two[1], int(m2.group(1))) if len(two) > 1 and m2 else ([], [])
    pre = len(two) > 1 and firing_lent(two[0], 5)
    result("f", "a lent automatic's fire held as RESTART MISSION comes: no sound left on (hw, smooth)",
           None if not pre else len(lines) >= 3 and not bad,
           "firing as it came: %s; the fresh run's first room: %d checks, %d with a sound from before" %
           (pre, len(lines), len(bad)))
    menu = re.findall(r"trace: L5 f\d+ voices: the menu up \(call (\d+)\): .*the runtime's (\d+);", m.log)
    stopped = m.lines(r"L5 f\d+ sound: all voices stopped: \d+ of the runtime's own")
    later = [n for c, n in menu if int(c) > 1]
    result("f", "a lent automatic's fire held as the menu opens: stopped with the game's (hw, smooth)",
           None if not stopped and not (menu and menu[0][1] != "0") else len(later) >= 3 and set(later) == {"0"},
           "lent sounds stopped as it opened: %d; in the menu after: %s" % (len(stopped), ",".join(later[:8])))


def check_restart(runs):
    y, n = runs["restart_yes"], runs["restart_no"]
    result("f", "YES: a fresh run", bool(y.lines("watch's RESTART MISSION asked")) and bool(y.lines("a fresh run from")),
           "asked %d, fresh run %d" % (len(y.lines("RESTART MISSION asked")), len(y.lines("a fresh run from"))))
    result("f", "NO: back to the game", bool(n.lines("RESTART MISSION asked")) and not n.lines("a fresh run from|run end"),
           "run end %d" % len(n.lines("run end")))


# ---- g: the weapon in hand carried, King's Ransom stretches included ----

def carry_runs():
    return {"carry": Run("carry", 8, 110, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "8:-1", "TWINE_ENDLESS_SEED": 5,
                                         "TWINE_ENDLESS_TEST_ARM": 1, "TWINE_ENDLESS_TEST_DRAW": "FRINESI AUTO",
                                         "TWINE_ENDLESS_TEST_DOOR": 2, "TWINE_ENDLESS_TEST_EXIT": 150,
                                         "TWINE_ENDLESS_TEST_GOD": 1})}


def iris_runs():  # (the second room's mission: the carried PS100, lent, in hand as its opening iris opens)
    return {"carry_iris": Run("carry_iris", 10, 60, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "10:4",
                                                     "TWINE_ENDLESS_NEXT": "2:6", "TWINE_ENDLESS_SEED": 4,
                                                     "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_TEST_ARM": 1,
                                                     "TWINE_ENDLESS_TEST_DRAW": "BELGIQUE", "TWINE_ENDLESS_TEST_EXIT": 200,
                                                     "TWINE_TRACE": "weapon,change", "TWINE_RENDERER": "hw",
                                                     "TWINE_SMOOTH": 1})}


def check_iris(runs):
    r = runs["carry_iris"]
    two = r.log.split("room in level 2 ", 1)[1] if "room in level 2 " in r.log else ""
    first = re.search(r"trace: L2 f(\d+) weapon: slot (\d+) \(before -?\d+, to -?\d+\) ([A-Z0-9 ]+), state (\d+)", two)
    asked = re.search(r"trace: L2 f(\d+) weapon: model asked \(\w+\) for slot (\d+) \(BELGIQUE", two)
    ok = bool(first and asked) and first.group(3).startswith("BELGIQUE") and first.group(4) == "1" and \
        int(asked.group(1)) <= 2
    result("g", "a new mission: the carried weapon taken out in its opening iris (hw, smooth)", ok,
           "model asked at f%s; the controls' first frame f%s: %s, state %s" %
           (asked.group(1) if asked else "-", first.group(1) if first else "-", first.group(3) if first else "-",
            first.group(4) if first else "-"))


def lend_runs():
    pad = ",".join("%d:4000,%d:0" % (t, t + 3) for t in range(520, 900, 6))
    return {"lend": Run("lend", 8, 50, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "8:-1", "TWINE_ENDLESS_SEED": 5,
                                        "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_TEST_ARM": 1,
                                        "TWINE_ENDLESS_TEST_DRAW": "INGALLS", "TWINE_ENDLESS_TEST_EXIT": 500,
                                        "TWINE_ENDLESS_TEST_MEET": 1, "TWINE_TEST_PAD": pad}),
            "survey": Run("survey", 2, 60, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "2:4", "TWINE_ENDLESS_SURVEY": 1})}


def check_lend(runs):
    r = runs["lend"]
    two = r.log.split("room start: level 2 ", 1)[1] if "room start: level 2 " in r.log else ""
    lent = r.lines(r"weapons: INGALLS TYPE 20 lent from level 8")
    result("g", "lent: Ingalls in Courier", bool(lent) and "INGALLS TYPE 20 drawn" in two,
           (lent[0].split("weapons: ")[1][:70] if lent else "not lent"))
    kills = len(re.findall(r"kill: .* with INGALLS TYPE 20", two))
    result("g", "lent: it kills there", kills > 0, "%d kills with it in Courier" % kills)
    v = runs["survey"]
    m = v.lines(r"survey done \((\d+) weapons known\)")
    n = int(re.search(r"\((\d+) weapons", m[0]).group(1)) if m else 0
    result("g", "weapon survey", n >= 80 and not v.crashed(), "%d weapons known after the survey" % n)


def check_carry(runs):
    r = runs["carry"]
    left = r.lines(r"carry: Bond leaves with")
    drawn = r.lines(r"weapon: .* drawn")
    stretch_ok = len(left) >= 1 and "FRINESI AUTO" in left[0] and len(drawn) >= 2 and "FRINESI AUTO drawn" in drawn[1]
    result("g", "stretch keeps the weapon", stretch_ok, "; ".join(x.split("endless: ")[1][22:80] for x in (left[:1] + drawn[1:2])))
    other = [d for d in drawn[2:3] if "FRINESI" in d]
    result("g", "another mission: it or the P2K", bool(other),
           drawn[2].split("weapon: ")[1][:60] if len(drawn) > 2 else "fewer than 3 rooms")


# ---- k: a lent weapon as at home: its fire rate, its damage, its scope; ammo from drops for whatever Bond carries ----

def burst(pad_from, frames=40):
    return "%d:4000,%d:0" % (pad_from, pad_from + frames)


def lent_runs():
    base = {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_SEED": 5, "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_TEST_ARM": 1,
            "TWINE_ENDLESS_TEST_MEET": 1, "TWINE_ENDLESS_AMMO_LOG": 1,
            "TWINE_ENDLESS_TEST_NO_RAMP": 1}  # (the weapons measured against guards as many as before the ramp: o's)
    shots = ",".join(burst(t, 30) for t in range(260, 1000, 50))  # (the room's first: from frame 200 on)
    later = ",".join(burst(t, 30) for t in range(760, 1500, 50))  # (the room after it: from about frame 622)
    aim = "200:0800,250:4800,254:0800,300:4800,304:0800,350:4800,354:0800,400:0"
    aim2 = "700:0800,750:4800,754:0800,800:4800,804:0800,850:4800,854:0800,900:0"
    home, lent = dict(base, TWINE_ENDLESS_ROOM="8:-1"), dict(base, TWINE_ENDLESS_ROOM="8:-1", TWINE_ENDLESS_NEXT="2:4",
                                                         TWINE_ENDLESS_TEST_EXIT=500)
    long_lent = dict(lent, TWINE_ENDLESS_TEST_EXIT=1000)
    runs = {"k_home_ingalls": Run("k_home_ingalls", 8, 55, dict(home, TWINE_ENDLESS_TEST_DRAW="INGALLS", TWINE_TEST_PAD=shots)),
            "k_lent_ingalls": Run("k_lent_ingalls", 8, 85, dict(long_lent, TWINE_ENDLESS_TEST_DRAW="INGALLS", TWINE_TEST_PAD=later)),
            "k_home_ar36": Run("k_home_ar36", 8, 40, dict(home, TWINE_ENDLESS_TEST_DRAW="AR36", TWINE_TEST_PAD=aim,
                                                          TWINE_SHOT=240)),
            "k_lent_ar36": Run("k_lent_ar36", 8, 60, dict(lent, TWINE_ENDLESS_TEST_DRAW="AR36", TWINE_TEST_PAD=aim2,
                                                          TWINE_SHOT=740)),
            "k_home_ks5": Run("k_home_ks5", 2, 40, dict(base, TWINE_ENDLESS_ROOM="2:4", TWINE_ENDLESS_TEST_DRAW="KOFFLER",
                                                        TWINE_TEST_PAD=shots)),
            "k_lent_ks5": Run("k_lent_ks5", 2, 60, dict(base, TWINE_ENDLESS_ROOM="2:4", TWINE_ENDLESS_NEXT="8:0",
                                                        TWINE_ENDLESS_TEST_EXIT=500, TWINE_ENDLESS_TEST_DRAW="KOFFLER",
                                                        TWINE_TEST_PAD=later)),
            "k_ammo": Run("k_ammo", 8, 120, dict(base, TWINE_ENDLESS_ROOM="8:-1", TWINE_ENDLESS_NEXT="2:4",
                                                 TWINE_ENDLESS_TEST_EXIT=700, TWINE_ENDLESS_TEST_DRAW="INGALLS",
                                                 TWINE_ENDLESS_TEST_KILL=30, TWINE_ENDLESS_TEST_GRAB_BOX=15))}
    # (the next weapon button: Square; it steps through all Bond carries in the groups' order, the lent among his own)
    nxt = ",".join("%d:8000,%d:0" % (t, t + 3) for t in range(650, 1000, 20))
    runs["k_pool"] = Run("k_pool", 2, 70, dict(base, TWINE_ENDLESS_ROOM="2:4", TWINE_ENDLESS_NEXT="10:-1,8:0",
                                               TWINE_ENDLESS_TEST_EXIT=250, TWINE_ENDLESS_TEST_DRAW="KOFFLER",
                                               TWINE_TEST_PAD=nxt, TWINE_TRACE="weapon,change"))
    del runs["k_pool"].env["TWINE_ENDLESS_TEST_MEET"]
    # (as played: the hardware renderer, smooth motion; the pool stepped through, each weapon's model; the timing)
    runs["k_pool_hw"] = Run("k_pool_hw", 2, 80, dict(runs["k_pool"].env, TWINE_RENDERER="hw", TWINE_SMOOTH=1))
    # (a guard's grenade spares his friends from its blast, not from Bond: a blast of a guard's at the guard farthest
    # from Bond while Bond's Ingalls bursts hit another (TWINE_TRACE=hits); and Bond's blast there, which kills him)
    runs["k_blast"] = Run("k_blast", 8, 30, dict(home, TWINE_ENDLESS_TEST_DRAW="INGALLS", TWINE_TEST_PAD=shots,
                                                 TWINE_ENDLESS_TEST_GUARD_BLAST="298", TWINE_TRACE="hits"))
    runs["k_blast_bond"] = Run("k_blast_bond", 8, 30, dict(runs["k_blast"].env, TWINE_ENDLESS_TEST_GUARD_BLAST="298,bond"))
    runs["k_perf"] = Run("k_perf", 8, 70, dict(base, TWINE_ENDLESS_ROOM="8:-1", TWINE_ENDLESS_NEXT="2:4",
                                              TWINE_ENDLESS_TEST_EXIT=120, TWINE_ENDLESS_TEST_DRAW="AR36",
                                              TWINE_RENDERER="hw", TWINE_SMOOTH=1, TWINE_TIMING=1))
    # (the pause menu's INVENTORY in King's Ransom: down to its last rows, one toggled out of the round, chosen)
    inv = "800:0008,803:0,840:0040,843:0,870:4000,873:0," + ",".join("%d:0040,%d:0" % (t, t + 3) for t in range(900, 1051, 15))
    runs["k_inv"] = Run("k_inv", 2, 70, dict(runs["k_pool"].env, TWINE_TEST_PAD=inv + ",1080:8000,1083:0,1100:4000,1103:0"))
    # (the pause menu with a lent weapon (the PS100) in hand in the mission's watch slot, King's Ransom's 10: the watch raised, the
    # menu and its INVENTORY on its face, closed, the lent weapon back; as played: hw renderer, smooth motion)
    runs["k_watch"] = Run("k_watch", 5, 80, dict(base, TWINE_ENDLESS_ROOM="5:-1", TWINE_ENDLESS_NEXT="8:-1",
                                                 TWINE_ENDLESS_TEST_EXIT=500, TWINE_ENDLESS_TEST_DRAW="BELGIQUE",
                                                 TWINE_RENDERER="hw", TWINE_SMOOTH=1, TWINE_TRACE="weapon,change",
                                                 TWINE_TEST_PAD="850:0008,853:0,900:0040,903:0,920:4000,923:0,980:1000,"
                                                                "983:0,1010:4000,1013:0", TWINE_SHOT="840,900,960,1100"))
    del runs["k_watch"].env["TWINE_ENDLESS_TEST_MEET"]
    # (nine missions in a row, a lent AR36 drawn in each, a quick punch in the first: as played, hw renderer and smooth
    # motion; each mission's own copy of a mod's memory must be put back after the extra drawings, or its animations
    # race (the list of such memory was never emptied: after four missions the lent gun's and the fists' weren't))
    runs["k_many"] = Run("k_many", 8, 150, dict(base, TWINE_ENDLESS_ROOM="8:-1",
                                                TWINE_ENDLESS_NEXT="2:4,10:-1,9:0,5:-1,2:4,3:-1,5:-1,7:2",
                                                TWINE_ENDLESS_TEST_EXIT=200, TWINE_ENDLESS_TEST_DRAW="AR36",
                                                TWINE_RENDERER="hw", TWINE_SMOOTH=1, TWINE_TRACE="weapon,punch",
                                                TWINE_TEST_MELEE=",".join(str(t) for t in range(150, 5000, 130))))
    del runs["k_many"].env["TWINE_ENDLESS_TEST_MEET"]
    # (a lent weapon's fire sound, sample for sample as at home: the IAC Defender at home (Meltdown) and lent (King's
    # Ransom), each shot's first half second measured by spu.c (TWINE_TRACE=spu: energy, peak, checksum); and one of a
    # mission's own sounds played the game's way and the lent way in the same room (TWINE_TEST_TONE_AB))
    snd = dict(base, TWINE_TRACE="sound,spu", TWINE_TEST_NO_CD=1, TWINE_ENDLESS_TEST_DRAW="IAC",
               TWINE_TEST_PAD="500:4000,503:0,600:4000,603:0,700:4000,703:0", TWINE_ENDLESS_ROOM="10:4",
               TWINE_ENDLESS_TEST_EXIT="200,5000")
    runs["k_snd_home"] = Run("k_snd_home", 10, 40, dict(snd, TWINE_ENDLESS_NEXT="10:-1"))
    runs["k_snd_lent"] = Run("k_snd_lent", 10, 40, dict(snd, TWINE_ENDLESS_NEXT="8:-1"))
    runs["k_snd_ab"] = Run("k_snd_ab", 8, 25, {"TWINE_TEST_TONE_AB": 42, "TWINE_TEST_NO_CD": 1, "TWINE_TRACE": "sound,spu"})
    for k in ("k_snd_home", "k_snd_lent"):
        del runs[k].env["TWINE_ENDLESS_TEST_MEET"]
    # (home against lent as played, hw renderer + smooth motion, both a second room carried in: idle, fire held to
    # a reload, put away and drawn; tools/lent_matrix.py does every weapon this way)
    # (the launchers too, with Bond's projectiles traced (TWINE_TRACE=proj): the rocket lent to Courier, the grenade
    # launcher's round to Christmas (whose own subtype 4 is a grenade; 0x2C, the round's, is no mission's but Fallen Angel's))
    for key, w, first, home, host in (("sg", "FRINESI SHOTGUN", "2:5", "2:4", "8:-1"), ("tmp", "MEYER", "6:0", "6:9", "2:4"),
                                      ("rocket", "ROCKET LAUNCHER", "5:4", "5:-1", "2:4"),
                                      ("gl", "GRENADE LAUNCHER", "9:2", "9:0", "6:0")):
        for kind, nxt in (("home", home), ("lent", host)):
            runs["k_%s_%s" % (key, kind)] = Run("k_%s_%s" % (key, kind), int(first.split(":")[0]), 75, dict(
                base, TWINE_ENDLESS_TEST_SHOTGUN_VANILLA=1, TWINE_ENDLESS_ROOM=first, TWINE_ENDLESS_NEXT=nxt, TWINE_ENDLESS_TEST_EXIT="200,5000",
                TWINE_ENDLESS_TEST_DRAW=w, TWINE_TEST_PAD="600:4000,900:0", TWINE_TEST_WEAPON="1100:1,1250:-1",
                TWINE_TRACE="weapon,change" + (",proj" if key in ("rocket", "gl") else ""), TWINE_RENDERER="hw",
                TWINE_SMOOTH=1))
            del runs["k_%s_%s" % (key, kind)].env["TWINE_ENDLESS_TEST_MEET"]
    # (the flash bang at home (Courier) and lent (King's Ransom), as played: hw renderer, smooth motion; thrown every
    # 90 frames, a guard sent put by Bond (TEST_MEET); traced: state 15, the fade, the guards flagged and blinded, the
    # sounds; tools/flash_trace.py compares them)
    flashes = ",".join("%d:4000,%d:0" % (t, t + 3) for t in range(300, 2400, 90))
    fl = dict(base, TWINE_ENDLESS_ROOM="2:4", TWINE_ENDLESS_TEST_DRAW="FLASH", TWINE_TEST_PAD=flashes,
              TWINE_TRACE="flash,sound", TWINE_RENDERER="hw", TWINE_SMOOTH=1, TWINE_ENDLESS_TEST_FLASH_VANILLA=1)
    runs["k_flash_home"] = Run("k_flash_home", 2, 60, fl)
    runs["k_flash"] = Run("k_flash", 2, 70, dict(fl, TWINE_ENDLESS_NEXT="8:0", TWINE_ENDLESS_TEST_EXIT="300,5000"))
    # (and to Fallen Angel: which guards come near enough to be blinded varies with the runs' pace)
    runs["k_flash_b"] = Run("k_flash_b", 2, 70, dict(fl, TWINE_ENDLESS_NEXT="9:0", TWINE_ENDLESS_TEST_EXIT="300,5000"))
    # (a run's flash, twice as long: at home 40 s, then lent in King's Ransom)
    lf = dict(fl, TWINE_ENDLESS_NEXT="8:0", TWINE_ENDLESS_TEST_EXIT="800,5000")
    del lf["TWINE_ENDLESS_TEST_FLASH_VANILLA"]
    runs["k_flash_long"] = Run("k_flash_long", 2, 100, lf)
    # (Courier's double-barrelled shotgun, its damage twice in a run: guards met and shot)
    runs["k_sg_dmg"] = Run("k_sg_dmg", 2, 45, dict(base, TWINE_ENDLESS_ROOM="2:4", TWINE_ENDLESS_TEST_DRAW="FRINESI",
                                                   TWINE_TEST_PAD=",".join("%d:4000,%d:0" % (t, t + 3) for t in range(240, 900, 40))))
    # (a lent weapon's shots sound as at home, Bond's own sounds by state: the IAC Defender lent into City of Walkways,
    # whose slot 7 (its flare gun's) played the flare's reload with every shot (playtest 8); and lent into that slot,
    # to see the comparison catch it)
    taps = ",".join("%d:4000,%d:0" % (t, t + 3) for t in range(600, 1000, 30))
    for name, nxt, extra in (("k_iacs_home", "10:-1", {}), ("k_iacs_lent", "5:-1", {}),
                             ("k_iacs_slot7", "5:-1", {"TWINE_ENDLESS_TEST_LENDAT": 7})):
        runs[name] = Run(name, 10, 75, dict(base, TWINE_ENDLESS_ROOM="10:4", TWINE_ENDLESS_NEXT=nxt,
                                            TWINE_ENDLESS_TEST_EXIT="200,5000", TWINE_ENDLESS_TEST_DRAW="IAC",
                                            TWINE_TEST_PAD=taps, TWINE_TRACE="weapon,change,sound,proj",
                                            TWINE_RENDERER="hw", TWINE_SMOOTH=1, **extra))
        del runs[name].env["TWINE_ENDLESS_TEST_MEET"]
    # (the lent rocket's smoke trail in Courier (2:-1's hall, a long free flight): every puff drawn from a sprite entry
    # with a texture (the playtest's "large grey box": City of Walkways' trail entry empty elsewhere, its puffs drawn
    # from VRAM's corner), and without the fix the empty entry seen, to know the check sees it; hw renderer, smooth)
    for name, extra in (("k_trail", {}), ("k_trail_nofix", {"TWINE_ENDLESS_TEST_NO_TRAIL_FIX": 1})):
        runs[name] = Run(name, 5, 45, dict(base, TWINE_ENDLESS_ROOM="5:4", TWINE_ENDLESS_NEXT="2:-1",
                                           TWINE_ENDLESS_TEST_EXIT="200,5000", TWINE_ENDLESS_TEST_DRAW="ROCKET",
                                           TWINE_ENDLESS_TEST_PUT="-9399,-705,-11513,0", TWINE_TEST_PAD="600:4000,603:0",
                                           TWINE_TRACE="weapon,proj", TWINE_TRACE_PROJ_ALL=1, TWINE_RENDERER="hw",
                                           TWINE_SMOOTH=1, **extra))
        del runs[name].env["TWINE_ENDLESS_TEST_MEET"]
    # (lent satchels as at home: a charge thrown, the detonator in hand by itself; fired, the charges back; then the
    # weapon buttons step all round: the detonator never comes with no charge out. As played: hw renderer, smooth)
    runs["k_satchel"] = Run("k_satchel", 8, 70, dict(base, TWINE_ENDLESS_ROOM="8:-1", TWINE_ENDLESS_NEXT="9:0",
                                                    TWINE_ENDLESS_TEST_EXIT="300,800", TWINE_ENDLESS_TEST_DRAW="SATCHEL CHARGE",
                                                    TWINE_RENDERER="hw", TWINE_SMOOTH=1, TWINE_TRACE="weapon,change",
                                                    TWINE_TEST_PAD="500:4000,503:0,600:4000,603:0," + ",".join(
                                                        "%d:8000,%d:0" % (t, t + 3) for t in range(650, 1000, 35))))
    runs["k_shotgun"] = Run("k_shotgun", 8, 45, dict(base, TWINE_ENDLESS_ROOM="8:-1", TWINE_ENDLESS_NEXT="3:-1",
                                                    TWINE_ENDLESS_TEST_EXIT=300, TWINE_ENDLESS_TEST_DRAW="FRINESI",
                                                    TWINE_TEST_PAD="200:4000,240:0,520:4000,560:0"))
    for k in ("k_satchel", "k_shotgun"):
        del runs[k].env["TWINE_ENDLESS_TEST_MEET"]
    del runs["k_ammo"].env["TWINE_ENDLESS_TEST_MEET"]
    return runs


def check_flash(runs):
    """the flash bang lent (King's Ransom, Fallen Angel) as at home (Courier): tools/flash_trace.py on the runs' traces"""
    import flash_trace
    hp = os.path.join(runs["k_flash_home"].dir, "port.log")
    part = room_part(runs["k_flash"].log, 8)
    fired = re.findall(r"weapons: FLASH BANG fired \(state 15", part)
    result("k", "Flash Bang: lent, fires as at home (its state 15)", len(fired) >= 2 and not runs["k_flash"].crashed(),
           "%d fired in King's Ransom" % len(fired))
    try:
        h = flash_trace.parse(hp, 2)
        ls = [flash_trace.parse(os.path.join(runs[k].dir, "port.log"), lv) for k, lv in (("k_flash", 8), ("k_flash_b", 9))]
    except OSError as e:
        for c in ("state 15", "guards blinded", "no callout"):
            result("k", "Flash Bang lent: " + c, False, str(e))
        return
    l = {"flashes": [f for x in ls for f in x["flashes"]], "blinded": [b for x in ls for b in x["blinded"]],
         "callouts": sum(x["callouts"] for x in ls)}
    d = [x for y in ls for x in flash_trace.compare(h, y) if x != "no lent guard blinded"]
    if not l["blinded"]:
        d.append("no lent guard blinded")
    how = " (trace; hw renderer, smooth motion)"
    st = [x for x in d if "flash" in x and "guard" not in x]
    result("k", "Flash Bang lent: state 15 as at home (its frames, counter, fade and prims, Courier's sounds)" + how,
           bool(h["flashes"]) and bool(l["flashes"]) and not st,
           "%d flashes home, %d lent%s" % (len(h["flashes"]), len(l["flashes"]), "; " + "; ".join(st)[:300] if st else ""))
    gd = [x for x in d if "guard" in x or "my eyes" in x and "callouts" not in x]
    result("k", "Flash Bang lent: guards who see it blinded as at home (flag 0x40000, Courier's animations, rate, "
           "duration, a \"my eyes!\" each)" + how, not gd,
           "home: %s; lent: %s%s" % (", ".join("%s %d frames" % (flash_trace.kind(b), b["len"]) for b in h["blinded"][:4]),
                                     ", ".join("rate %d %d frames" % (b["rate"], b["len"]) for b in l["blinded"][:4]),
                                     "; " + "; ".join(gd)[:300] if gd else ""))
    result("k", "Flash Bang: no \"my eyes!\" callout of the port's on top of the game's line", not h["callouts"] and not l["callouts"],
           "home %d, lent %d" % (h["callouts"], l["callouts"]))
    lp = os.path.join(runs["k_flash_long"].dir, "port.log")
    try:
        rs = [flash_trace.parse(lp, 2), flash_trace.parse(lp, 8)]
    except OSError as e:
        rs, err = [], str(e)
    pr = [p for r in rs for p in flash_trace.long_problems(r)]
    fl = [f for r in rs for f in r["flashes"] if f["end"] is not None]
    bl = [b for r in rs for b in r["blinded"]]
    van = [flash_trace.fade_frames(f)[0] for f in h["flashes"] if f["end"] is not None]
    result("k", "Flash Bang (a run's): the white twice as long, at home and lent (Courier's: %s frames solid)" % sorted(set(van)),
           len({r["level"] for r in rs if r["flashes"]}) == 2 and not [p for p in pr if p.startswith("flash")] and set(van) == {4},
           "%d flashes, %s frames solid%s" % (len(fl), sorted(set(flash_trace.fade_frames(f)[0] for f in fl)),
                                              "; " + "; ".join(p for p in pr if p.startswith("flash"))[:200] if pr else ""))
    result("k", "Flash Bang (a run's): guards blinded twice as long (260 or 668 frames, Courier's 130 or 334), a \"my eyes!\" each",
           bool(bl) and not [p for p in pr if not p.startswith("flash")],
           "%s%s" % (", ".join("L%d %d" % (r["level"], b["len"]) for r in rs for b in r["blinded"]),
                     "; " + "; ".join(p for p in pr if not p.startswith("flash"))[:200] if pr else ""))
    sg = runs["k_sg_dmg"].log
    up = re.search(r"FRINESI SHOTGUN \(slot \d+\): (\d+) damage a pellet -> (\d+)", sg)
    hits = [(int(f), int(p)) for f, p in re.findall(r" f(\d+) v\d+ hit: person (\d+) by FRINESI SHOTGUN", sg)]
    kills = [int(f) for f in re.findall(r" f(\d+) v\d+ kill: guard .* with FRINESI SHOTGUN", sg)]
    one = [f for f, p in hits if f + 1 in kills or f in kills]
    result("k", "Frinesi Shotgun (a run's): twice the damage a pellet, a close hit downs a guard",
           bool(up) and int(up.group(2)) == 2 * int(up.group(1)) and bool(one),
           "%s; %d hits, %d downing at once" % ("%s -> %s a pellet" % up.groups() if up else "not raised", len(hits), len(one)))


def check_pool_models(run, how):
    """each weapon stepped to in King's Ransom's pool slot shows its own model, put away as itself, no flash idle"""
    import weapon_trace
    path = os.path.join(run.dir, "port.log")
    fr = [x for x in weapon_trace.frames(path, "", 8)]
    idle = {}
    for x in fr:
        if x["state"] == 1 and x["loaded"]:
            idle.setdefault(x["name"], set()).add(x["file"])
    files = [f for v in idle.values() for f in v]
    ok = len(idle) >= 3 and all(len(v) == 1 for v in idle.values()) and len(set(files)) == len(files)
    result("k", "pool: each weapon shows its own model" + how, ok,
           "; ".join("%s %s" % (n, "/".join(sorted(v))) for n, v in sorted(idle.items())) or "no idle frames")
    own = {n: next(iter(v)) for n, v in idle.items() if len(v) == 1}
    wrong = [x for x in fr if x["state"] in (3, 7) and x["loaded"] and x["name"] in own and x["file"] != own[x["name"]]]
    result("k", "pool: put away and drawn as itself" + how, bool(own) and not wrong,
           "%d frames of a weapon with another's model%s" % (len(wrong), " (f%d %s: %s)" % (wrong[0]["f"], wrong[0]["name"],
                                                                                      wrong[0]["file"]) if wrong else ""))
    flash = [x for x in fr if x["state"] == 1 and x["flash"] == "SHOWN"]
    result("k", "pool: no muzzle flash while idle" + how, bool(fr) and len(flash) <= 1,
           "%d idle frames with the flash shown" % len(flash))


def room_part(log, level):
    """the log of the run's first room in that level (to the next room's start)"""
    part = log.split("room start: level %d " % level, 1)
    return part[1].split("room start:", 1)[0] if len(part) > 1 else ""


def fire_rate(part, weapon):
    """the most shots in any 20 frames (the room's frames), from the clip's count going down (only down: a clip filled
    from a pickup of its ammo, as one is taken mid-burst, logs a change too, and was counted as a shot)"""
    t, last = [], None
    for m in re.finditer(r"ammo: %s[^:]*? clip (\d+) at frame (\d+)" % weapon, part):
        c, f = int(m.group(1)), int(m.group(2))
        if last is not None and c < last:
            t.append(f)
        last = c
    return max((sum(1 for u in t if v <= u < v + 20) for v in t), default=0) - (1 if t else 0)


def bullet_damage(part, weapon):
    """the damage of each of Bond's bullets that hit a person (endless.c: the bullet's own, +0x34, before where it hit
    multiplies it)"""
    return [int(x) for x in re.findall(r"bullet: %s[^']*'s hit person \d+: its damage (\d+)" % weapon, part)]


def damage(part, weapon):
    """the damage a bullet does (where it hits multiplies it: the hits' greatest common divisor), and the hits"""
    d = [int(x) for x in re.findall(r"hit: person \d+ by %s[^:]*: (\d+) damage" % weapon, part)]
    g = 0
    for x in d:
        g = math.gcd(g, x)
    return g, len(d)


def scope_picture(img):
    """the share of the sniper scope's picture found where it is drawn (a 512x240 frame's places, dark): its
    crosshair's lines and ring"""
    w, h, px = img
    sx, sy = w / 512.0, h / 240.0

    def dark(x, y):
        for dy in (-1, 0, 1):
            for dx in (-1, 0, 1):
                i = (int((y + dy) * sy) * w + int((x + dx) * sx)) * 3
                if i + 2 < len(px) and max(px[i], px[i + 1], px[i + 2]) < 48:
                    return True
        return False
    pts = [(255, y) for y in range(150, 195, 5)] + [(x, 117) for x in range(125, 205, 8)] + \
        [(x, 117) for x in range(306, 386, 8)] + [(214, 118), (297, 118), (256, 93), (256, 142)]
    return sum(dark(x, y) for x, y in pts) / len(pts)


def check_blast(runs):
    """a guard's grenade: his friends near it unhurt by it, Bond's bullets still hurting people meanwhile"""
    g, b = runs["k_blast"].log, runs["k_blast_bond"].log
    went = re.search(r"f(\d+) v\d+ ai: a guard's grenade goes off at .*? till frame (\d+) \(hit masks \+0x400.*?, (\d+) people within", g)
    back = re.search(r"masks put back: of the (\d+) near the blast .*?, (\d+) still up", g.split("test: a grenade's blast", 1)[-1])
    up = re.search(r"test: the blast's person \d+ a second on: (\w+)", g)
    t0, t1 = (int(went.group(1)), int(went.group(2))) if went else (0, 0)
    hits = [(int(f), d) for f, d in re.findall(r"trace: L8 f(\d+) hits: .*? hit by INGALLS TYPE 20 .*?: (damage \d+|NO DAMAGE)", g)]
    during = [d for f, d in hits if t0 <= f < t1]
    result("k", "a guard's grenade spares his friends near it (%s near)" % (back.group(1) if back else "?"),
           bool(went) and bool(back) and int(back.group(1)) >= 1 and back.group(1) == back.group(2) and bool(up) and
           up.group(1) == "up", "%s; %s" % (back.group(0) if back else "no blast", up.group(0) if up else "no target"))
    bu = re.search(r"test: the blast's person \d+ a second on: (\w+)", b)
    result("k", "... where Bond's blast kills (the same place, Bond's grenade)", bool(bu) and bu.group(1) == "down",
           bu.group(0) if bu else "no target")
    result("k", "... and Bond's bullets hurt people in the blast's second (Ingalls, TWINE_TRACE=hits)",
           bool(during) and all(d.startswith("damage") for d in during),
           "frames %d-%d: %s" % (t0, t1, ", ".join(during) or "no hits"))


def check_lent(runs):
    check_blast(runs)
    for w, lv_home, lv_lent, label in (("INGALLS", 8, 2, "Ingalls"), ("KOFFLER", 2, 8, "KS5")):
        h = room_part(runs["k_home_" + label.lower()].log, lv_home)
        l = room_part(runs["k_lent_" + label.lower()].log, lv_lent)
        lent = re.search(r"weapons: %s\S* .*lent from level \d+ into slot (\d+)" % w, runs["k_lent_" + label.lower()].log)
        rh, rl = fire_rate(h, w), fire_rate(l, w)
        result("k", "%s: lent, fires as at home" % label, bool(lent) and rh > 2 and abs(rh - rl) <= 1,
               "shots in 20 frames: home %d, lent %d%s" % (rh, rl, "" if lent else " (not lent)"))
        sh = re.search(r"sound: a lent weapon's own \(level %d" % lv_home, l)
        result("k", "%s: its own fire sound" % label, bool(sh), sh.group(0)[7:] if sh else "no lent sound played")
    h, l = room_part(runs["k_home_ingalls"].log, 8), room_part(runs["k_lent_ingalls"].log, 2)
    # (each bullet's own damage, as it hits: the health a hit takes is that times where it hit (500, 1000, 2500 seen
    # from 500 bullets), so a few hits' drops alone can't tell; their greatest common divisor is shown too)
    bh, bl = bullet_damage(h, "INGALLS"), bullet_damage(l, "INGALLS")
    (dh, nh), (dl, nl) = damage(h, "INGALLS"), damage(l, "INGALLS")
    result("k", "Ingalls: damage as at home", bool(bh) and bool(bl) and set(bh) == set(bl),
           "its bullets' damage: home %s (%d hit), lent %s (%d hit); the hits' common divisor home %d (%d), lent %d (%d)" %
           (sorted(set(bh)), len(bh), sorted(set(bl)), len(bl), dh, nh, dl, nl))
    h, l = room_part(runs["k_home_ar36"].log, 8), room_part(runs["k_lent_ar36"].log, 2)
    sc = lambda p: [int(x) for x in re.findall(r"weapon state (\d+) at", p) if int(x) in (33, 34, 35, 36)]
    result("k", "AR36: the scope's states as at home", set(sc(h)) == set(sc(l)) == {33, 34, 35, 36},
           "home %s, lent %s" % (sorted(set(sc(h))), sorted(set(sc(l)))))
    ph = read_ppm(os.path.join(runs["k_home_ar36"].dir, "shot_0240.ppm"))
    pl = read_ppm(os.path.join(runs["k_lent_ar36"].dir, "shot_0740.ppm"))
    qh, ql = scope_picture(ph) if ph else 0, scope_picture(pl) if pl else 0
    result("k", "AR36: the scope's picture as at home", qh >= 0.85 and ql >= 0.85,
           "its marks found: home %d%%, lent %d%%" % (qh * 100, ql * 100))
    check_flash(runs)
    part = room_part(runs["k_satchel"].log, 9)
    fr = lambda pat: [int(x) for x in re.findall(r" f(\d+) v\d+ weapons: " + pat, part)]
    thrown, det_in, boom, back = (fr(r"SATCHEL CHARGE fired"), fr(r"SATCHEL DETONATOR into slot \d+ \(a charge out"),
                                  fr(r"the detonator set off [1-9]"), fr(r"SATCHEL CHARGE into slot \d+ \(the charges set off"))
    ok = bool(thrown and det_in and boom and back) and 15 <= det_in[0] - thrown[0] <= 35 and 0 < back[0] - boom[0] <= 10
    result("k", "Satchels + detonator: lent, as at home (thrown: the detonator; fired: the charges)", ok,
           "thrown f%s, detonator in hand f%s, set off f%s, charges back f%s (home: +23, +7)" %
           tuple((x[0] if x else "-") for x in (thrown, det_in, boom, back)))
    import weapon_trace
    tr = [x for x in weapon_trace.frames(os.path.join(runs["k_satchel"].dir, "port.log"), "SATCHEL DETONATOR", 9)
          if x["f"] > (back[0] if back else 0) and x["state"] in (1, 7)]
    steps = re.findall(r"change: to \d+ \((.+?)\) from .* by the next-weapon button", part)
    pooled = re.findall(r"weapons: the pool's (.+?) into slot", part)
    result("k", "Satchels: no detonator in hand with no charge out (the weapon buttons stepped round)",
           bool(back) and "SATCHEL CHARGE" in steps and len(steps) + len(pooled) >= 8 and not tr,
           "%d steps round to the charges again (%d through the pool), %d frames of the detonator in hand after" %
           (len(steps) + len(pooled), len(pooled), len(tr)))
    h, l = room_part(runs["k_shotgun"].log, 8), room_part(runs["k_shotgun"].log, 3)
    rh, rl = fire_rate(h, "FRINESI"), fire_rate(l, "FRINESI")
    result("k", "Frinesi auto: lent, fires as at home", rh >= 2 and abs(rh - rl) <= 1,
           "shots in 20 frames: home %d, lent %d" % (rh, rl))
    p = room_part(runs["k_pool"].log, 8)
    before = runs["k_pool"].log.split("room start: level 8 ", 1)[0].rsplit("room start:", 1)[-1]
    waits = re.findall(r"weapons: (.+?) waits in the pool", before)  # (as King's Ransom's room starts)
    steps = re.findall(r"the pool's (.+?) into slot", p)
    result("k", "pool: more lent than slots, stepped through", len(waits) >= 2 and set(waits) <= set(steps),
           "waiting %s; stepped to %s" % (", ".join(waits) or "none", ", ".join(steps) or "none"))
    for k in ("k_pool", "k_pool_hw"):
        check_pool_models(runs[k], "" if k == "k_pool" else " (hw renderer, smooth motion)")
    p = room_part(runs["k_perf"].log, 2)
    t = [(float(a), float(b)) for a, b in re.findall(r"timing: .*?, lent swaps ([0-9.]+), .*?; lent reads ([0-9.]+)", p)][1:]
    swaps = max([a for a, _ in t] or [99])
    result("k", "lent weapon in hand: its VRAM swaps cheap (hw renderer, smooth motion)", bool(t) and swaps < 6 and
           all(b == 0 for _, b in t), "%d periods, swaps at most %.2f ms a tick, read back %s" %
           (len(t), swaps, "never" if all(b == 0 for _, b in t) else "again and again"))
    p = room_part(runs["k_inv"].log, 8)
    rows = [(int(a), int(b), int(n)) for a, b, n in re.findall(r"inventory: rows (\d+)-(\d+) of (\d+)", p)]
    pooled = re.findall(r"weapons: (.+?) waits in the pool", runs["k_inv"].log.split("room start: level 8 ", 1)[0].rsplit("room start:", 1)[-1])
    chosen = re.search(r"the pool's (.+?) into slot \d+ \(chosen in the inventory\)", p)
    drawn = chosen and re.search(r"ammo: %s clip" % re.escape(chosen.group(1)), p.split("chosen in the inventory", 1)[1])
    result("k", "inventory: the pool's weapons listed, scrolled", bool(rows) and bool(pooled) and
           rows[0][2] > 11 and any(r[0] > 0 for r in rows),
           "rows %s; %d waiting in the pool" % (", ".join("%d-%d of %d" % r for r in rows[:4]), len(pooled)))
    result("k", "inventory: a pool weapon chosen, in hand", bool(drawn),
           "%s" % (chosen.group(0) + (", in hand" if drawn else ", not in hand") if chosen else "none chosen"))
    import weapon_trace
    for key, w, home, host in (("sg", "FRINESI SHOTGUN", 2, 8), ("tmp", "MEYER", 6, 2)):
        hp, lp = (os.path.join(runs["k_%s_%s" % (key, k)].dir, "port.log") for k in ("home", "lent"))
        hs = weapon_trace.summary(weapon_trace.frames(hp, w, home), weapon_trace.changes(hp, home))
        ls = weapon_trace.summary(weapon_trace.frames(lp, w, host), weapon_trace.changes(lp, host))
        bad = weapon_trace.compare(hs, ls)
        lent = "lent from" in runs["k_%s_lent" % key].log and re.search(r"weapons: %s\S* .*lent from" % w, runs["k_%s_lent" % key].log)
        result("k", "%s lent: as at home (trace: idle, fire, reload, put away, drawn; hw, smooth motion)" % w.title(),
               bool(lent) and hs["frames"] > 100 and ls["frames"] > 100 and 11 in ls["states"] and not bad,
               "; ".join(bad[:3]) or "no differences (%d frames at home, %d lent)" % (hs["frames"], ls["frames"]))
    # (the launchers: their home missions' own weapon states (City of Walkways' 0x16, Fallen Angel's 0x26) run lent,
    # and what they fire as at home: the rocket (City of Walkways' bullet: its mask, blast, speed) and the round
    # (Fallen Angel's type 32: its drop, its blast where it hits); port/mods/endless.c THE LAUNCHERS)
    for key, w, home, host, state, what in (("rocket", "ROCKET LAUNCHER", 5, 2, 22, "its rocket and blast"),
                                            ("gl", "GRENADE LAUNCHER", 9, 6, 38, "its round, its drop and blast")):
        hp, lp = (os.path.join(runs["k_%s_%s" % (key, k)].dir, "port.log") for k in ("home", "lent"))
        hs = weapon_trace.summary(weapon_trace.frames(hp, w, home), weapon_trace.changes(hp, home))
        ls = weapon_trace.summary(weapon_trace.frames(lp, w, host), weapon_trace.changes(lp, host))
        ph, pl = weapon_trace.projectiles(hp, home), weapon_trace.projectiles(lp, host)
        bad = weapon_trace.compare(hs, ls) + weapon_trace.compare_projectiles(ph, pl)
        log = runs["k_%s_lent" % key].log
        lent = re.search(r"weapons: %s .*lent from" % w, log)
        blasts = [p for p in pl if p["blast"]]
        errors = re.findall(r"CT_LAUNCHED_GRENADE: MISSING|NOT found on the disc", log)
        result("k", "%s lent: as at home (trace: its own state, fire, %s; hw, smooth motion)" % (w.title(), what),
               bool(lent) and hs["frames"] > 100 and ls["frames"] > 100 and state in ls["states"] and len(pl) >= 2
               and len(blasts) >= 2 and not errors and not runs["k_%s_lent" % key].crashed() and not bad,
               "; ".join(bad[:3] + errors[:1]) or "no differences (%d frames at home, %d lent; %d fired, %d went off)" %
               (hs["frames"], ls["frames"], len(pl), len(blasts)))
    def puffs(r):  # (the sprites made from the lent rocket's launch in Courier on: (all, those with no texture))
        log = r.log.split("launch: rocket #", 1)[1] if "launch: rocket #" in r.log else ""
        made = re.findall(r"trace: L2 f\d+ proj: sprite \d+ made .*?; def ([0-9A-F]{40})", log)
        return made, [d for d in made if d[24:32] == "00000000"]
    made, bare = puffs(runs["k_trail"])
    made_o, bare_o = puffs(runs["k_trail_nofix"])
    result("k", "Rocket Launcher lent (Courier): its smoke trail's puffs drawn with a texture, no grey box (hw, smooth)",
           len(made) >= 4 and not bare and bool(bare_o),
           "%d puffs, %d without a texture (without the fix: %d of %d)" % (len(made), len(bare), len(bare_o), len(made_o)))
    def second(r):  # (the second room's part of a run's log, as a file: weapon_trace reads files)
        p = os.path.join(r.dir, "second.log")
        parts = r.log.split("room in level ")
        open(p, "w").write("room in level " + "room in level ".join(parts[2:]) if len(parts) > 2 else "")
        return p
    hp, lp, cp = (second(runs[k]) for k in ("k_iacs_home", "k_iacs_lent", "k_iacs_slot7"))
    sh, sl, sc = (weapon_trace.sounds(p, "IAC", lv) for p, lv in ((hp, 10), (lp, 5), (cp, 5)))
    bad, caught = weapon_trace.compare_sounds(sh, sl), weapon_trace.compare_sounds(sh, sc)
    slot = re.search(r"IAC DEFENDER lent from level \d+ into slot (\d+)", runs["k_iacs_lent"].log)
    shots = sl.get(8, [0])[0]
    result("k", "IAC Defender lent (City of Walkways): Bond's sounds by state as at home (hw, smooth; its flare gun's "
           "slot not used)", bool(slot) and slot.group(1) != "7" and shots >= 6 and not bad,
           "slot %s, %d shots; %s; lent into slot 7 the comparison sees: %s" %
           (slot.group(1) if slot else "-", shots, "; ".join(bad) or "no differences", "; ".join(caught) or "nothing"))
    result("k", "Bond's sounds by state compared: the flare gun slot's reload sound caught", bool(caught),
           "; ".join(caught) or "not caught")
    r = runs["k_many"]
    path = os.path.join(r.dir, "port.log")
    levels = [int(x) for x in re.findall(r"room start: level (\d+) ", r.log)]
    bad, seen = [], 0
    for lv in sorted(set(levels)):
        sm = weapon_trace.summary(weapon_trace.frames(path, "AR36", lv), [])
        if sm["frames"]:
            seen += 1
            bad += ["L%d %s" % (lv, b) for b in weapon_trace.jumpy(sm, "AR36")]
    result("k", "lent weapons: animations at the game's pace in every mission of a long run (hw, smooth motion)",
           len(set(levels)) >= 7 and seen >= 7 and not bad and not r.lines("smooth: no room to keep"),
           "%d missions, AR36 in hand in %d; %s" % (len(set(levels)), seen, "; ".join(bad[:3]) or "no playhead jumps"))
    jumps, frames, lv_seen = Counter(), Counter(), set()  # (the quick punch's own animation, frame by frame)
    prev = None
    for m in re.finditer(r"trace: L(\d+) f(\d+) punch: anim (-?\d+) left (\d+), track playhead (\d+)", r.log):
        lv, f, _, left, ph = map(int, m.groups())
        if prev and prev[0] == lv and prev[1] + 1 == f and left < prev[3]:
            frames[lv] += 1
            jumps[lv] += ph - prev[4] not in (0, 1, 2)
            lv_seen.add(lv)
        prev = (lv, f, 0, left, ph)
    sides = Counter(re.findall(r"punch: the (\w+) hand punches", r.log))
    result("k", "quick punch: every frame of its animation in every mission (hw, smooth motion), the free hand",
           len(lv_seen) >= 7 and not sum(jumps.values()) and set(sides) == {"left"},
           "%d missions, %d punch frames, playhead jumps %d; hands %s" % (len(lv_seen), sum(frames.values()),
                                                                         sum(jumps.values()), dict(sides)))
    r = runs["k_watch"]
    fr = weapon_trace.frames(os.path.join(r.dir, "port.log"), "", 8)
    menu = [x for x in fr if x["menu"] and x["state"] == 0x1F]  # (the watch held up)
    after = [x for x in fr if not x["menu"] and x["state"] == 1 and x["name"].startswith("BELGIQUE") and
             fr.index(x) > (fr.index(menu[-1]) if menu else 1 << 30)]
    shot = lambda f: read_ppm(os.path.join(r.dir, "screen_%04d_0.ppm" % f)) or read_ppm(os.path.join(r.dir, "screen_%04d.ppm" % f))
    m = [motorola(i) if i else 0 for i in (shot(840), shot(900), shot(960), shot(1100))]
    result("k", "pause: the watch raised, a lent weapon in the watch slot (hw renderer, smooth motion)",
           bool(menu) and all(x["file"].endswith("MOTO.HMD") for x in menu) and m[1] > 0.8 and m[2] > 0.8,
           "%d menu frames, models %s; the watch's frame on screen: menu %d%%, inventory %d%%" %
           (len(menu), "/".join(sorted({x["file"] for x in menu})) or "-", m[1] * 100, m[2] * 100))
    result("k", "pause: the lent weapon back after the menu", bool(after) and all(x["file"] == "L05/FNP90TEX.HMD" for x in after)
           and m[0] < 0.2 and m[3] < 0.2, "%d frames after, models %s; watch frame before %d%%, after %d%%" %
           (len(after), "/".join(sorted({x["file"] for x in after})) or "-", m[0] * 100, m[3] * 100))
    def measured(log, own, want=()):
        """each program 4 tone 13 (the IAC's shot) keyed on: its first half second as spu.c measured it (lent: of
        the next two lent voices measured, the one as at home if either is: a guard's callout, also a lent voice, may
        be measured in between)"""
        out, lines = [], log.splitlines()
        for i, x in enumerate(lines):
            m = re.search(r"sound: (\d+) flags \w+ \(program 4 tone 13 .*handle (\d+)", x) if own else \
                re.search(r"tone level \d+ program 4 tone 13 note \d+: a voice.*handle (\d+)", x)
            if not m:
                continue
            pat = r"spu: voice %s's first half second: (.*)" % m.group(2) if own else r"lent voice \d+'s first half second: (.*)"
            got = [k.group(1) for k in (re.search(pat, y) for y in lines[i:i + 600]) if k][:1 if own else 2]
            if got:
                out.append(next((g for g in got if g in want), got[0]))
        return out
    h = measured(runs["k_snd_home"].log, True)
    l = measured(runs["k_snd_lent"].log, False, set(h))
    lent_room = room_part(runs["k_snd_lent"].log, 8)
    result("k", "IAC Defender lent: its shot's sound sample for sample as at home", bool(h) and bool(l) and
           set(h) == set(l) and "lent from" in runs["k_snd_lent"].log,
           "home %s; lent %s" % (sorted(set(h))[:2] or "none", sorted(set(l))[:2] or "none"))
    ab = runs["k_snd_ab"].log
    m = re.search(r"f100 sound: 42 flags 0 .*handle (\d+)", ab)
    own = re.search(r"spu: voice %s's first half second: (.*)" % m.group(1), ab.split(m.group(0), 1)[1]) if m else None
    lent = re.search(r"lent voice \d+'s first half second: (.*)", ab)
    result("k", "a sound the lent way (a voice as the SPU's) as the game plays it", bool(own and lent) and
           own.group(1) == lent.group(1), "game %s; lent way %s" % (own.group(1) if own else "-", lent.group(1) if lent else "-"))
    a = room_part(runs["k_ammo"].log, 2)
    got = re.findall(r"pickup: (.+?)'s ammo (\d+) -> (\d+)( \(a lent weapon\))?", a)
    lent_got = [g for g in got if g[3] and int(g[2]) > int(g[1])]
    result("k", "ammo box feeds a lent weapon", bool(lent_got),
           "%d boxes for lent weapons taken%s" % (len(lent_got), " (%s %s -> %s)" % lent_got[0][:3] if lent_got else ""))
    start = re.search(r"weapons as the room starts: (.*)", a)
    carried = []
    for e in (start.group(1).split(";") if start else []):  # ("*3:WOLFRAM P2K 80", "4:FLASH BANG(-) 4": not his)
        m = re.match(r"\s*\*?\d+:(.*?)(\(-\))? \d+$", e)
        if m and not m.group(2):
            carried.append(m.group(1))
    full = set(m[:16] for m in re.findall(r"ammo: (.+?) reserve \d+ of \d+ \(full\)", a))
    guns = [c.strip() for c in carried if not re.match(r"UNARMED|SATCHEL|FRAG|FLASH|CELL|PALMTOP|BRIEFCASE|VLF|FINGER", c)
            and c.strip()[:16] not in full]
    fed = {g[0][:16] for g in got}
    missing = [g for g in guns if g[:16] not in fed]
    result("k", "drops feed every gun carried", bool(guns) and not missing,
           "guns %s; fed %s%s" % (", ".join(guns), len(fed), "; none for " + ", ".join(missing) if missing else ""))


# ---- h: the high scores on the card ----

NAV = ("400:40,403:0,460:4000,463:0,530:40,533:0,590:4000,593:0")  # main menu: OPTIONS, MEMORY CARD


def card_runs():
    card = os.environ.get("TWINE_CHECK_CARD", os.path.expanduser("~/.local/share/twine/card1.mcd"))
    if not os.path.exists(card):
        return {}
    base = {"TWINE_TEST_NO_FILMS": 1, "TWINE_HD": 0, "TWINE_SMOOTH": 0}
    save = NAV + ",750:4000,753:0,900:4000,903:0,1050:40,1053:0,1110:4000,1113:0"
    load = NAV + ",700:40,703:0,750:4000,753:0,900:4000,903:0,1050:40,1053:0,1110:4000,1113:0"
    return {"card_save": Run("card_save", 0, 70, dict(base, TWINE_TEST_PAD=save), card=card),
            "card_load": Run("card_load", 0, 70, dict(base, TWINE_TEST_PAD=load), card=card),
            "card_ranked": Run("card_ranked", 2, 45, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "2:4",
                                                     "TWINE_ENDLESS_TEST_DIE": 150, "TWINE_TEST_PAD": "490:8,493:0"},
                               card=card),
            # (from the shell, with the campaign autoloaded: ENDLESS (up from the cursor on Meltdown), a run in Meltdown,
            # mission index 10, Bond dies; its stats, then X: the game's "YOU WON THE GAME" and cheat unlocking must not
            # come, nor the campaign change)
            "card_won": Run("card_won", 0, 75, dict(base, TWINE_RENDERER="hw", TWINE_ENDLESS_ROOM="10:-1",
                                                    TWINE_ENDLESS_TEST_DIE=150, TWINE_SHOT=",".join(
                                                        str(f) for f in range(1000, 1600, 30)),
                                                    TWINE_TEST_PAD="460:4000,463:0,560:10,563:0,600:4000,603:0,"
                                                                   "740:4000,743:0,1500:4000,1503:0"), card=card)}


def stats_screen(img):
    """the share of the stats screen's cyan frames in a screen (none on the game's won-the-game screen)"""
    w, h, px = img
    n = hit = 0
    for y in range(0, h, 4):
        for x in range(0, w, 4):
            i = (y * w + x) * 3
            if i + 2 < len(px):
                n += 1
                hit += px[i + 2] > 150 and px[i + 1] > 120 and px[i] < 110
    return hit / n if n else 0.0


def card_files(path):
    try:
        d = open(path, "rb").read()
    except OSError:
        return {}
    out = {}
    for b in range(1, 16):
        e = d[b * 128:(b + 1) * 128]
        if e[0] == 0x51:
            out[e[10:30].split(b"\0")[0].decode(errors="replace")] = hashlib.md5(d[b * 8192:(b + 1) * 8192]).hexdigest()
    return out


def check_card(runs):
    if not runs:
        result("h", "card", None, "no card with a campaign save (TWINE_CHECK_CARD)")
        return
    card = runs["card_save"].card
    before = card_files(card)
    for k, why in (("card_save", "the game saved"), ("card_ranked", "a ranked run")):
        r = runs[k]
        after = card_files(os.path.join(r.dir, "data/twine/card1.mcd"))
        wrote = bool(r.lines("table written to the card in slot 1 \\(" + why))
        same = after.get("BASLUS-01272") == before.get("BASLUS-01272") or k == "card_save"
        result("h", k[5:], wrote and "BASLUS-01272ENDLESS" in after and same,
               "written %s, endless block %s, campaign block %s" % (wrote, "BASLUS-01272ENDLESS" in after,
                                                                    "kept" if same else "CHANGED"))
    r = runs["card_won"]
    op = re.search(r" v(\d+) the run's stats screen opens", r.log)
    shots = []
    if op:
        for f in range(1000, 1600, 30):
            if int(op.group(1)) + 30 <= f <= int(op.group(1)) + 280:
                img = read_ppm(os.path.join(r.dir, "screen_%04d.ppm" % f))
                shots.append(stats_screen(img) if img else 0)
    after = card_files(os.path.join(r.dir, "data/twine/card1.mcd"))
    result("h", "a run's end in Meltdown: its stats, not the campaign's won-the-game screen",
           bool(r.lines("the game's \"won the game\" check skipped")) and not r.lines("CHECK CHEAT") and
           bool(shots) and min(shots) > 0.008,
           "stats %s, %d screens after it opened, %d of them the stats; cheats unlocked: %d" %
           ("opened" if op else "never", len(shots), sum(x > 0.008 for x in shots), len(r.lines("CHECK CHEAT"))))
    result("h", "a run's end: the campaign's save kept", bool(r.lines("table written to the card")) and
           after.get("BASLUS-01272") == before.get("BASLUS-01272"),
           "campaign block %s" % ("kept" if after.get("BASLUS-01272") == before.get("BASLUS-01272") else "CHANGED"))
    r = runs["card_load"]
    result("h", "load", bool(r.lines(r"scores: .*the game loaded|card's table read|endless.txt's .* taken")),
           "; ".join(x.split("scores: ")[1][:50] for x in r.lines("scores:"))[:150])


# ---- j: the IMPROVED enemy AI against the game's own, a still Bond ----

AI_ROOMS = [(3, 11), (6, 13), (9, 2), (2, 6), (10, 4), (7, 6), (5, 4), (8, 3)]


GRENADE_ROOMS = [(8, 3), (3, 11), (6, 13), (10, 4)]


def ai_runs():
    runs = {}
    for l, p in GRENADE_ROOMS:  # (a guard met, and a grenade of Bond's lobbed at him: he and those near shout and run;
        # the IMPROVED AI's, so with it under -o too, as the other runs here say which they have)
        runs["aig_%d_%d" % (l, p)] = Run("aig_%d_%d" % (l, p), l, 40, {
            "TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "%d:%d" % (l, p), "TWINE_ENDLESS_SEED": 4,
            "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_TEST_MEET": 1, "TWINE_ENDLESS_TEST_MEET_GRENADE": 1,
            "TWINE_ENEMY_AI": "improved"})
    for l, p in AI_ROOMS:
        for m in ("original", "improved"):
            runs["ai_%d_%d_%s" % (l, p, m)] = Run("ai_%d_%d_%s" % (l, p, m), l, 75, {
                "TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "%d:%d" % (l, p), "TWINE_ENDLESS_SEED": 7,
                "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENEMY_AI": m, "TWINE_ENDLESS_PEOPLE_FAST": 1,
                # (the two AIs against the game's six slots under -8 too: with 12 the director's cap doubles and its
                # draws, not the AIs, decide how much each side sends; 12 people are check n's)
                "TWINE_PEOPLE_SLOTS": 6})
    return runs


def approach(run):
    """(the closest a live guard came to Bond's place, from 15 s on; the most samples two stood within 300 in a row)"""
    bond, best, pairs, run_len = None, 10 ** 9, {}, 0
    frames = {}
    for l in run.log.splitlines():
        m = re.search(r"room start: level \d+ section \d+ at (-?\d+) (-?\d+) (-?\d+)", l)
        if m:
            bond = (int(m.group(1)), int(m.group(3)))
        m = re.search(r" f(\d+) .*people: (\d+) type \d+ \S+, health (-?\d+).* at (-?\d+) (-?\d+) (-?\d+)", l)
        if m and bond and int(m.group(3)) > 0:
            f, x, z = int(m.group(1)), int(m.group(4)), int(m.group(6))
            frames.setdefault(f, []).append((x, z))
            if f >= 25 * 15:
                best = min(best, ((x - bond[0]) ** 2 + (z - bond[1]) ** 2) ** 0.5)
    stack = 0
    for f in sorted(frames):
        p = frames[f]
        close = any(((a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2) ** 0.5 < 300 for i, a in enumerate(p) for b in p[i + 1:])
        stack = stack + 1 if close else 0
        run_len = max(run_len, stack)
    return best, run_len


def check_ai(runs):
    covers = 0
    for l, p in AI_ROOMS:
        o, i = runs["ai_%d_%d_original" % (l, p)], runs["ai_%d_%d_improved" % (l, p)]
        bo, so = approach(o)
        bi, st = approach(i)
        walks = len(i.lines("ai: type \\d+ walked from"))
        hits_o, hits_i = len(o.lines(r"damage: \d+: health")), len(i.lines(r"damage: \d+: health"))
        dmg = lambda r: sum(int(x.split("damage: ")[1].split(":")[0]) for x in r.lines(r"damage: \d+: health"))
        dmg_o, dmg_i = dmg(o), dmg(i)  # (damage dealt in all, grenades and shots: the pressure on Bond)
        ok = not i.crashed() and (bi <= max(bo, 3000) or hits_i > 0) and dmg_i >= dmg_o // 2
        result("j", "%d:%d hunting" % (l, p), ok,
               "closest %s (game's AI %s), walks %d, hits on Bond %d for %d (game's %d for %d)" % (
                   int(bi) if bi < 10 ** 9 else "-", int(bo) if bo < 10 ** 9 else "-", walks, hits_i, dmg_i, hits_o, dmg_o))
        shots = [re.search(r"guards' shots (\d+), held (\d+)", x) for x in i.lines("guards' shots")]
        fired, held = (int(shots[-1].group(1)), int(shots[-1].group(2))) if shots and shots[-1] else (0, 0)
        result("j", "%d:%d fair shooting" % (l, p), hits_i == 0 or fired > 0,
               "guards' shots %d, held %d (no line of fire, or reacting)" % (fired, held))
        covers += len(i.lines("takes cover"))
        result("j", "%d:%d no bunching" % (l, p), st <= 12 or st <= so,
               "two within 300 for %d samples (0.5 s) in a row (game's AI %d)" % (st, so))
    result("j", "cover used", covers > 0, "%d times in the %d rooms" % (covers, len(AI_ROOMS)))
    thr = sum(len(runs["ai_%d_%d_improved" % r].lines("throws a grenade at Bond")) for r in AI_ROOMS)
    sht = sum(len(runs["ai_%d_%d_improved" % r].lines('shouts "grenade!" \\(as he throws')) for r in AI_ROOMS)
    result("j", "grenades thrown at Bond", thr > 0 and sht >= thr, "%d thrown, %d with a shout" % (thr, sht))
    said = [re.search(r'calls out "(.+?)" \(level (\d+)', x) for l, p in AI_ROOMS
            for x in runs["ai_%d_%d_improved" % (l, p)].lines("calls out")]
    texts, voices = {m.group(1) for m in said if m}, {m.group(2) for m in said if m}
    orig = sum(len(runs["ai_%d_%d_original" % r].lines("calls out")) for r in AI_ROOMS)
    result("j", "callouts: guards call out, in several voices", len(texts) >= 4 and len(voices) >= 3 and orig == 0,
           "%d calls: %s; voices of levels %s; with the game's own AI %d" % (
               len(said), ", ".join(sorted(texts)), ", ".join(sorted(voices)), orig))
    for r in GRENADE_ROOMS:
        g = runs["aig_%d_%d" % r]
        scat = len(g.lines("and runs from it"))
        result("j", "%d:%d scatter from Bond's grenade" % r, scat > 0 and not g.crashed(),
               "%d shouted and ran (grenade %s)" % (scat, "lobbed" if g.lines("grenade of Bond's lobbed") else "never lobbed"))


# ---- l: room starts never run out of the menu texts' letters (the game's 230: "inc MAX_TEXT_SPRITES!", a crash) ----

STRESS_ROOMS = [(7, 6)] * 8 + [(2, 6), (3, 11), (5, 4), (6, 13), (8, 3), (9, 2), (10, 4), (7, 6), (2, 4), (10, 2),
                               (8, 4), (6, 9)]


CHAINS = 2
CHAIN_ROOMS = ("7:2,7:6,2:6,7:6,10:4,7:2,8:3,7:6,3:11,7:2,7:6,9:2,7:6,6:13",
               "7:6,7:2,7:6,5:4,7:2,10:2,7:6,2:4,7:2,8:4,7:6,6:9,7:2,7:6")


def text_runs():
    runs = {}
    for k, (l, p) in enumerate(STRESS_ROOMS):  # (each its first start too: the catalogue's survey, then the room)
        runs["text_%d_%d_%d" % (l, p, k)] = Run("text_%d_%d_%d" % (l, p, k), l, 30, {
            "TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "%d:%d" % (l, p), "TWINE_ENDLESS_SEED": 7 + k % 3,
            "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_PEOPLE_FAST": 1})
    for k in range(CHAINS):  # (one run, room after room: missions started again and again, Masquerade's among them)
        runs["text_chain_%d" % k] = Run("text_chain_%d" % k, 7, 230, {
            "TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "7:6" if k % 2 == 0 else "7:2", "TWINE_ENDLESS_SEED": 11 + k,
            "TWINE_ENDLESS_NEXT": CHAIN_ROOMS[k % 2], "TWINE_ENDLESS_TEST_EXIT": "120,120", "TWINE_ENDLESS_TEST_GOD": 1})
    runs["text_forced"] = Run("text_forced", 7, 25, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "7:6",
                                                     "TWINE_ENDLESS_SEED": 7, "TWINE_ENDLESS_TEST_GOD": 1,
                                                     "TWINE_ENDLESS_TEST_TEXTS": 12})
    return runs


def check_texts(runs):
    singles = {k: r for k, r in runs.items() if k.startswith("text_") and k != "text_forced" and "chain" not in k}
    bad = [r.name for k, r in singles.items() if r.crashed() or "MAX_TEXT" in r.log or "room start:" not in r.log]
    cut = [r.name for k, r in singles.items() if r.lines(r"text: .* cut to")]
    result("l", "%d room starts: no crash, letters to spare" % len(singles), not bad and not cut,
           "crashed or not started: %s; letters cut: %s" % (", ".join(bad) or "none", ", ".join(cut) or "none"))
    for k in range(CHAINS):  # (room after room in one run: each a mission started afresh)
        r = runs["text_chain_%d" % k]
        starts = len(r.lines(r"room start: level"))
        worst = max([int(m) for l in r.lines(r"text: \d+ letters in") for m in re.findall(r"text: (\d+) letters", l)] or [0])
        ok = starts >= 12 and not r.crashed() and "MAX_TEXT" not in r.log and not r.lines(r"text: .* cut to")
        result("l", "room after room (%s...): %d room starts, no crash, letters to spare" %
               (CHAIN_ROOMS[k % 2][:11], starts), ok,
               "%d starts, crash %s, MAX_TEXT %s, cut %d, most letters logged %d%s" %
               (starts, r.crashed(), "MAX_TEXT" in r.log, len(r.lines(r"text: .* cut to")), worst,
                "; " + "; ".join(l.split("endless: ")[-1][:200] for l in r.lines(r"text \(last|the game ends with"))
                if "MAX_TEXT" in r.log else ""))
    f = runs["text_forced"]
    result("l", "past the letters: cut, no crash", bool(f.lines(r"text: .* cut to")) and not f.crashed() and
           "MAX_TEXT" not in f.log, "%d cut, crash %s" % (len(f.lines(r"text: .* cut to")), f.crashed()))


# ---- q: King's Ransom's bar (8:5): its kinds of guards have no slot of their own (its slots: its three posted guards',
# the bar's civilians', the story person's left out): a slot nobody needs made a kind's (endless.c kind_slots), the
# posted guards hunt when nobody comes; guards keep coming, in by both doorways, Bond's own among them ----

def bar_runs():
    env = {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "8:5", "TWINE_ENDLESS_SEED": 5, "TWINE_ENDLESS_TEST_GOD": 1,
           "TWINE_ENDLESS_TEST_SHOOT": 20, "TWINE_ENDLESS_TEST_MEET": 1}  # (everyone shot from 20 s on, every 2 s;
    # Bond put by each guard sent, so he's away from the door he came in by)
    return {"q_bar": Run("q_bar", 8, 150, dict(env)), "q_bar8": Run("q_bar8", 8, 150, dict(env, TWINE_RAM_8MB=1))}


def check_bar(runs):
    for k, label in (("q_bar", "2 MB (8 under -8)"), ("q_bar8", "8 MB")):
        r = runs[k]
        st = re.search(r"f(\d+) v\d+ room start: level 8 section 5 at (-?\d+) -?\d+ (-?\d+)", r.log)
        f0, ex, ez = (int(st.group(1)), int(st.group(2)), int(st.group(3))) if st else (0, 0, 0)
        sp = [(int(m.group(1)), m.group(2), int(m.group(3)), int(m.group(4))) for m in
              re.finditer(r" f(\d+) v\d+ spawn \([^)]*\): guard type \d+ (in at a doorway|on the floor) at (-?\d+) -?\d+ (-?\d+)",
                          r.log)]
        mins = [sum(1 for f, _, _, _ in sp if 1200 * m <= f - f0 < 1200 * (m + 1)) for m in range(3)]
        near = [s for s in sp if s[1] == "in at a doorway" and math.hypot(s[2] - ex, s[3] - ez) < 4500]
        far = [s for s in sp if s[1] == "in at a doorway" and math.hypot(s[2] - ex, s[3] - ez) > 8000]
        made = r.lines(r"room: slot \d+ \(nobody's now\) made a type")
        result("q", "%s: guards keep coming (each minute)" % label, bool(st) and not r.crashed() and all(mins[:2]),
               "spawns each minute %s, %d in all; %s" % (mins, len(sp), (made[0].split("room: ")[1][:70] if made else
                                                                          "no slot made a kind's")))
        result("q", "%s: in by both doorways (Bond's own too)" % label, bool(near) and bool(far),
               "%d in near the door Bond came in by, %d at the far side" % (len(near), len(far)))



# ---- n: more than six people at once (port/mods/people.c: 12 slots with 8 MB of RAM) ----

def people_runs():
    # (guards sent every half second until the slots are full: TWINE_ENDLESS_TEST_FILL)
    base = {"TWINE_RAM_8MB": 1, "TWINE_ENDLESS": 1, "TWINE_ENDLESS_SEED": 4, "TWINE_ENDLESS_TEST_GOD": 1,
            "TWINE_ENDLESS_TEST_FILL": 1, "TWINE_ENDLESS_PEOPLE_FAST": 1, "TWINE_PEOPLE_TRACE": 1,
            "TWINE_ENDLESS_AMMO_LOG": 1, "TWINE_ENEMY_AI": "improved"}  # (the port's AI under -o too: guards that
    # hunt Bond, so each one fires)
    return {"n_fill": Run("n_fill", 5, 120, dict(base, TWINE_ENDLESS_ROOM="5:4", TWINE_ENDLESS_TEST_SHOOT=80)),
            # (Bond faces each one drawn from an extra slot in turn, 4 s each, firing from f900: the game's own hit
            # tests; as played: hw renderer, smooth motion)
            "n_fire": Run("n_fire", 10, 120, dict(base, TWINE_ENDLESS_ROOM="10:-1", TWINE_ENDLESS_TEST_LOOK="extra",
                                                  TWINE_ENDLESS_TEST_ARM=1, TWINE_TEST_PAD="900:4000",
                                                  TWINE_RENDERER="hw", TWINE_SMOOTH=1, TWINE_SHOT=1500)),
            "n_save": Run("n_save", 5, 100, dict(base, TWINE_ENDLESS_ROOM="5:4", TWINE_TEST_SAVE=1000,
                                                 TWINE_TEST_LOAD=1300)),
            "n_long": Run("n_long", 10, 240, dict(base, TWINE_ENDLESS_ROOM="10:-1",
                                                  TWINE_ENDLESS_NEXT="5:4,9:2,7:6,8:-1,3:11,2:4,6:13",
                                                  TWINE_ENDLESS_TEST_EXIT="700,500", TWINE_ENDLESS_TEST_SHOOT=25))}


def alive_frames(run):
    """{frame: {person: (health, x, y, z, slot-less line)}} from the people lines"""
    out = {}
    for m in re.finditer(r" f(\d+) v\d+ people: (\d+) type \d+ \S+, health (-?\d+), ai \d+ at (-?\d+) (-?\d+) (-?\d+)"
                         r".*?shots (\d+)", run.log):
        out.setdefault(int(m.group(1)), {})[int(m.group(2))] = tuple(int(m.group(k)) for k in (3, 4, 5, 6, 7))
    return out


def check_people(runs):
    r = runs["n_fill"]
    fr = alive_frames(r)
    most = max((sum(1 for v in p.values() if v[0] > 0) for p in fr.values()), default=0)
    slots = r.lines(r"people: 12 slots")
    result("p", "12 alive at once (8 MB: 12 slots)", bool(slots) and most >= 12 and not r.crashed(),
           "most alive %d, slots %s, crash %s" % (most, "12" if slots else "-", r.crashed()))
    drawn = max([int(x) for x in re.findall(r"drawn at once \(2 s\): (\d+)", r.log)] or [0])
    result("p", "more than six drawn at once", drawn >= 9, "most drawn %d" % drawn)
    shots = {}
    for p in fr.values():
        for i, v in p.items():
            shots[i] = max(shots.get(i, 0), v[4])
    late = {i: shots.get(i, 0) for i in range(7, 13)}
    result("p", "people 7-12 each fire", all(late.values()), "shots %s" % late)
    fell = {int(m.group(1)) for m in re.finditer(r"test shot: person (\d+) type \d+ health -?\d+ -> (-\d+|0)$",
                                                  r.log, re.M)}
    result("p", "people 1-12 each fall", set(range(1, 13)) <= fell, "fell %s" % sorted(fell))
    r = runs["n_fire"]
    extra = {int(m.group(1)) for m in re.finditer(r"test look: person (\d+) \(slot (\d+)\)", r.log) if int(m.group(2)) >= 6}
    hit = {int(x) for x in re.findall(r"hit: person (\d+) by", r.log)} & extra
    kills = len(r.lines(r"kill: guard"))
    img = read_ppm(os.path.join(r.dir, "screen_1500_0.ppm"))
    level = sum(img[2]) / max(1, len(img[2])) if img and img[2] else -1
    result("p", "real shots hit and kill people in the extra slots (hw, smooth motion)",
           len(hit) >= 3 and kills >= 2 and level > 10 and not r.crashed(),
           "hit %s of %s, %d kills, screen level %.1f" % (sorted(hit), sorted(extra), kills, level))
    r = runs["n_save"]
    saved = re.search(r"savestate: saved .*frame (\d+)\)", r.log)
    loaded = r.log.find("savestate: loaded")
    ok, detail = False, "no save/load"
    if saved and loaded > 0:
        first = r.log[:loaded]
        after = r.log[loaded:]
        fa = {}
        for part, d in ((first, {}), (after, {})):
            for m in re.finditer(r" f(\d+) v\d+ people: (\d+) type \d+ \S+, health (-?\d+), ai \d+ at (-?\d+) (-?\d+) "
                                 r"(-?\d+)", part):
                d.setdefault(int(m.group(1)), {})[int(m.group(2))] = m.group(3, 4, 5, 6)
            fa[part is first] = d
        f0 = min(fa[False]) if fa[False] else -1
        # (the first people line after the load comes a few frames after the state's own; those frames advance by the
        # vblanks each one took, which a loaded run under load needn't repeat: the same people, the same health, each
        # within a step or two of where the first pass had him)
        a, b = fa[True].get(f0, {}), fa[False].get(f0, {})
        moved = max((max(abs(int(a[k][j]) - int(b[k][j])) for j in (1, 2, 3)) for k in a if k in b), default=0)
        same = bool(a) and set(a) == set(b) and all(a[k][0] == b[k][0] for k in a) and moved <= 300
        n = sum(1 for v in b.values() if int(v[0]) > 0)
        ok = same and n >= 10 and not r.crashed() and "people: 12 slots" in r.log
        detail = "back at f%d: %d alive, as when saved %s (the same people and health, moved at most %d), crash %s" % (
            f0, n, same, moved, r.crashed())
    result("p", "a save state with 12 about: loaded as it was", ok, detail)
    r = runs["n_long"]
    rooms = len(r.lines(r"room start: level"))
    free = [int(x) for x in re.findall(r"ext heap \(8 MB of RAM\) has (\d+) bytes free", r.log)]
    result("p", "a long run (12 slots in every room)", rooms >= 6 and not r.crashed() and
           len(r.lines(r"people: 12 slots")) >= rooms and (not free or min(free) > 1000000),
           "%d rooms, ext heap free at least %s, crash %s" % (rooms, min(free) if free else "?", r.crashed()))


# ---- m: a run started from the shell, its weapons not catalogued yet ----

def shell_runs():
    # main menu: NEW GAME, then mission select; down and left onto ENDLESS, chosen; the briefing's X later
    pad = "460:4000,463:0,560:4000,563:0,660:40,663:0,700:80,703:0,760:4000,763:0"
    env = {"TWINE_TEST_NO_FILMS": 1, "TWINE_SMOOTH": 0, "TWINE_RENDERER": "hw"}
    return {"shell_start": Run("shell_start", 0, 110, dict(env, TWINE_SHOT=880, TWINE_TEST_PAD=pad + ",900:4000,903:0")),
            "shell_stall": Run("shell_stall", 0, 80, dict(env, TWINE_SHOT=1450, TWINE_ENDLESS_TEST_SURVEY_STALL=1,
                                                         TWINE_TEST_PAD=pad + ",1500:4000,1503:0"))}


def screen_level(run, frame):
    img = read_ppm(os.path.join(run.dir, "screen_%04d.ppm" % frame))
    return sum(img[2]) / max(1, len(img[2])) if img and img[2] else -1


def check_shell(runs):
    r = runs["shell_start"]
    seen = screen_level(r, 880)
    after = r.log[r.log.find("survey done"):] if "survey done" in r.log else ""
    result("m", "shell start: the briefing seen (not under the survey's cover)", seen > 10, "screen level %.1f" % seen)
    result("m", "shell start: the survey, then the run's first room", bool(after) and "room start:" in after and
           not r.crashed(), "survey done %s, room after it %s, crash %s" % (bool(after), "room start:" in after,
                                                                          r.crashed()))
    r = runs["shell_stall"]
    seen = screen_level(r, 1450)
    after = r.log[r.log.find("survey given up"):] if "survey given up" in r.log else ""
    result("m", "a stalled survey given up after 10 s, the run started", bool(after) and "room start:" in after and
           seen > 10 and not r.crashed(), "given up %s, room after it %s, briefing level %.1f" %
           (bool(after), "room start:" in after, seen))


# ---- n: exits: every pool room's doors used once the exit is open ----

def door_runs(rooms):
    runs = {}
    for l, p in rooms:
        n = "doors_%d_%d" % (l, p if p >= 0 else 99)
        runs[n] = Run(n, l, 100, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "%d:%d" % (l, p), "TWINE_ENDLESS_SEED": 4,
                                 "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_TEST_DOORS": 1})
    # (Masquerade 7:7, the playtest's: its doors the mission's way, refusing; and left by one of them)
    runs["doors_77_mission"] = Run("doors_77_mission", 7, 75, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "7:7",
                                                               "TWINE_ENDLESS_SEED": 4, "TWINE_ENDLESS_TEST_GOD": 1,
                                                               "TWINE_ENDLESS_TEST_DOORS": 1,
                                                               "TWINE_ENDLESS_TEST_NO_LOCKED_EXIT": 1})
    runs["doors_77_leave"] = Run("doors_77_leave", 7, 75, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "7:7",
                                                           "TWINE_ENDLESS_SEED": 4, "TWINE_ENDLESS_TEST_GOD": 1,
                                                           "TWINE_ENDLESS_TEST_DOORS": 1,
                                                           "TWINE_ENDLESS_TEST_DOORS_LEAVE": 1})
    # (Flashpoint 6:13's channel doors, "CHANNELS MUST BE OFFLINE" (the playtest's): beyond the walker's flood, so Bond
    # is put in the door's zone where the playtest's was, use held (hw renderer, smooth motion); the exit open: he
    # leaves by it, and the mission's way: it refuses)
    ch = {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "6:13", "TWINE_ENDLESS_SEED": 4, "TWINE_ENDLESS_TEST_GOD": 1,
          "TWINE_TEST_PAD": "160:2000", "TWINE_ENDLESS_TEST_PUT": "-15927,-1410,-11766,2679",
          "TWINE_ENDLESS_TEST_OPEN": 30, "TWINE_RENDERER": "hw", "TWINE_SMOOTH": 1}
    runs["doors_613_leave"] = Run("doors_613_leave", 6, 45, ch)
    runs["doors_613_mission"] = Run("doors_613_mission", 6, 45, dict(ch, TWINE_ENDLESS_TEST_NO_LOCKED_EXIT=1))
    return runs


def check_doors(runs):
    for k, r in sorted(runs.items()):
        if not re.match(r"doors_\d+_\d+$", k):
            continue
        first = r.log.split("room left (", 1)  # (the room's own: the next one tests its doors too)
        tried = re.findall(r"doortest: door \d+ .*", first[0])
        opens = [l for l in tried if ": opens," in l]
        leaves = [l for l in tried if ": leaves (" in l]
        refused = [re.search(r'refused: "([^"]*)"', l).group(1) for l in tried if "refused:" in l]
        entry = len(first) > 1 and first[1].startswith("the use button at the entry)")
        room = k[6:].replace("_99", "_-1").replace("_", ":")
        result("n", room + " exits", not refused and entry and not r.crashed(),
               "%d doors by his floor: %d open, %d kept locked by the mission lead out, %d refuse%s; entry's way out %s"
               % (len(tried), len(opens), len(leaves), len(refused), (" (" + ", ".join(refused) + ")") if refused else "",
                  "works" if entry else "NOT TAKEN"))
    m = runs["doors_77_mission"]
    refused = m.lines(r"doortest: .*refused:")
    result("n", "7:7 the mission's way: its locked doors refuse (as the playtest)", bool(refused),
           "; ".join(re.search(r'refused: "([^"]*)"', l).group(1) for l in refused) or "none refused")
    m = runs["doors_613_mission"]
    result("n", "6:13 the mission's way: its channel doors refuse (CHANNELS MUST BE OFFLINE, as the playtest)",
           bool(m.lines(r'message shown: "CHANNELS MUST BE OFFLINE"')), "%d shown" %
           len(m.lines(r'message shown: "CHANNELS MUST BE OFFLINE"')))
    v = runs["doors_613_leave"]
    left = v.lines(r"room left \(a door the mission keeps locked, the exit open\)")
    after = v.log[v.log.find("keeps locked, the exit open"):] if left else ""
    result("n", "6:13 left by a channel door the mission keeps locked (hw, smooth; use held)",
           bool(v.lines(r'door: the mission.s "CHANNELS MUST BE OFFLINE" \(the exit open')) and bool(left) and
           "room in level" in after and not v.crashed(), "left %d, the next room %s" %
           (len(left), "started" if "room in level" in after else "not started"))
    v = runs["doors_77_leave"]
    left = v.lines(r"room left \(a door the mission keeps locked, the exit open\)")
    result("n", "7:7 left by a door the mission keeps locked", bool(left) and "room in level" in
           v.log[v.log.find("keeps locked, the exit open"):] and not v.crashed(),
           "left %d, the next room %s" % (len(left), "started" if left and "room start:" in
                                        v.log[v.log.find("keeps locked, the exit open"):] else "not started"))


# ---- o: the early waves' ramp (Meltdown 4: six guards of its own, all hunting at once in the playtest's first wave) ----

RAMP_WAVES = (1, 3, 5)


def ramp_runs():
    base = {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "10:4", "TWINE_ENDLESS_SEED": 4, "TWINE_ENDLESS_TEST_GOD": 1}
    runs = {"ramp_w%d" % w: Run("ramp_w%d" % w, 10, 75, dict(base, TWINE_ENDLESS_TEST_WAVE=w)) for w in RAMP_WAVES}
    runs["ramp_none"] = Run("ramp_none", 10, 75, dict(base, TWINE_ENDLESS_TEST_WAVE=1, TWINE_ENDLESS_TEST_NO_RAMP=1))
    return runs


def damage_profile(run):
    """(Bond's damage each 10 s of the room's first minute, the longest run of frames hit in a row, the most in 2 s,
    the most hunters at once)"""
    st = re.search(r"f(\d+) v\d+ room start", run.log)
    f0 = int(st.group(1)) if st else 0
    dm = [(int(f), int(x)) for f, x in re.findall(r"f(\d+) v\d+ damage: (\d+):", run.log)]
    per = [sum(x for f, x in dm if 0 <= (f - f0) // 200 == k) for k in range(6)]
    frames, longest, cur, prev = sorted({f for f, _ in dm if f - f0 < 1200}), 0, 0, -9
    for f in frames:
        cur = cur + 1 if f - prev <= 1 else 1
        prev, longest = f, max(longest, cur)
    most2 = max([sum(x for g, x in dm if f <= g < f + 40) for f, _ in dm if f - f0 < 1200] or [0])
    hunters = max([int(h) for h in re.findall(r"ai: (\d+) hunters", run.log)] or [-1])
    return per, longest, most2, hunters


def check_ramp(runs):
    prof = {k: damage_profile(r) for k, r in runs.items()}
    for k in sorted(prof):
        per, longest, most2, hunters = prof[k]
        result("o", "damage over time, %s" % ("no ramp, wave 1" if k == "ramp_none" else "wave " + k[6:]),
               not runs[k].crashed() and "room start:" in runs[k].log,
               "per 10 s %s = %d; longest burst %d frames; most in 2 s %d; hunters at once %s" %
               (per, sum(per), longest, most2, hunters if hunters >= 0 else "-"))
    w1, none, w5 = prof["ramp_w1"], prof["ramp_none"], prof["ramp_w5"]
    caps = {1: 2, 3: 4, 5: 64}
    hunters_ok = all(prof["ramp_w%d" % w][3] <= caps[w] for w in RAMP_WAVES)
    # (the game's own AI (-o) presses far less (no hunters: Meltdown 4's guards keep to their places): gentler, not
    # halved)
    result("o", "the first wave gentler: %s, short bursts, 2 hunters" % ("less damage" if AI_ORIGINAL else
                                                                         "half the damage or less"),
           (sum(w1[0]) <= sum(none[0]) if AI_ORIGINAL else sum(w1[0]) * 2 <= sum(none[0])) and w1[1] <= 12 and hunters_ok,
           "wave 1 %d against %d with no ramp; bursts %d frames (no ramp %d); most in 2 s %d (%d)" %
           (sum(w1[0]), sum(none[0]), w1[1], none[1], w1[2], none[2]))
    result("o", "rising by wave", sum(w5[0]) > sum(w1[0]),
           "damage in the first minute: " + ", ".join("wave %d %d" % (w, sum(prof["ramp_w%d" % w][0])) for w in RAMP_WAVES))


# ---- i: the campaign untouched ----

def campaign_runs():
    runs = {"campaign_%d" % l: Run("campaign_%d" % l, l, 30, {}) for l in (2, 5, 8)}
    # (smooth motion's extra drawings and the hit loop: King's Ransom fired in, campaign and a run, smooth motion on and
    # off, hw renderer, TWINE_TRACE=hitloop: the loop runs once a tick, never in an extra drawing, the same per tick)
    pad = ",".join("%d:4000,%d:0" % (t, t + 20) for t in range(200, 1200, 40))
    hl = {"TWINE_TRACE": "hitloop", "TWINE_RENDERER": "hw", "TWINE_TEST_PAD": pad}
    run = {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_SEED": 5, "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_TEST_ARM": 1,
           "TWINE_ENDLESS_TEST_MEET": 1, "TWINE_ENDLESS_ROOM": "8:-1", "TWINE_ENDLESS_TEST_DRAW": "INGALLS"}
    for k, e in (("c", {}), ("e", run)):
        for sm in (0, 1):
            n = "hl_%s_%s" % (k, "smooth" if sm else "off")
            runs[n] = Run(n, 8, 40, dict(hl, TWINE_SMOOTH=sm, **e))
    return runs


def hitloop_ticks(run):
    """the hit loop's runs in a log (TWINE_TRACE=hitloop): the tick's (frame, projectiles with hits, sounds), how many
    in extra drawings, how many extra drawings skipped it"""
    t = [(int(f), int(n), int(s)) for f, n, s in
         re.findall(r"trace: L\d+ f(\d+) hitloop: the tick's, (\d+) with hits, (\d+) sounds", run.log)]
    extra = len(re.findall(r"hitloop: an extra drawing", run.log))
    skipped = sum(int(x) for x in re.findall(r"skipping it since: (\d+)", run.log))
    return t, extra, skipped


# ---- p: the weapons Bond carries, one list: own and lent each an entry with its own checkmark; the weapon buttons (pad,
# mouse wheel, Z/X) step through the checked ones; Q (and L1) the weapon held before; the number keys by group; the
# checkmarks kept for the run; the campaign's stepping the game's own; the HUD strip on a change ----

SEL_INPUTS = [(900, "wheeldown", "FLASH BANG"), (1000, "x", "IAC DEFENDER"), (1100, "dright", "FLASH BANG"),
              (1200, "wheelup", "IAC DEFENDER"), (1300, "z", "FLASH BANG"), (1400, "dleft", "IAC DEFENDER"),
              (1500, "q", "FLASH BANG"), (1600, "l1", "IAC DEFENDER"), (1700, "6", "FLASH BANG"),
              (1800, "6", "CELL PHONE STUNNER"), (1900, "1", "WOLFRAM P2K"), (2000, "1", "IAC DEFENDER")]


def select_runs():
    base = {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_SEED": 5, "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_TEST_ARM": "first",
            "TWINE_ENDLESS_ROOM": "6:0", "TWINE_ENDLESS_TEST_DRAW": "IAC", "TWINE_TRACE": "select,change",
            "TWINE_RENDERER": "hw", "TWINE_SMOOTH": 1}
    runs = {
        # (the playtest's: King's Ransom, the IAC and the flash bang lent into its one shared slot, only they checked)
        "p_kr": Run("p_kr", 6, 130, dict(base, TWINE_ENDLESS_NEXT="8:0", TWINE_ENDLESS_TEST_EXIT="200,5000",
                                         TWINE_ENDLESS_TEST_TICKS="8:IAC|FLASH", TWINE_SHOT="895,925",
                                         TWINE_TEST_INPUT=",".join("%d:%s" % (t, k) for t, k, _ in SEL_INPUTS))),
        # (the checkmarks kept: set in King's Ransom, then Meltdown and Courier)
        "p_ticks": Run("p_ticks", 6, 120, dict(base, TWINE_ENDLESS_NEXT="8:0,10:-1,2:4", TWINE_ENDLESS_TEST_EXIT="200,500",
                                               TWINE_ENDLESS_TEST_TICKS="8:IAC|FLASH",
                                               TWINE_TEST_INPUT="700:wheeldown,1150:wheeldown,1650:wheeldown")),
        # (... and as the inventory sets them: King's Ransom's first row (the P2K) toggled out, then Meltdown)
        "p_inv": Run("p_inv", 8, 100, dict(base, TWINE_ENDLESS_ROOM="8:-1", TWINE_ENDLESS_NEXT="10:-1",
                                           TWINE_ENDLESS_TEST_EXIT="1000,5000", TWINE_ENDLESS_TEST_DRAW="",
                                           TWINE_TEST_PAD="800:0008,803:0,840:0040,843:0,870:4000,873:0,900:8000,903:0,"
                                                          "950:1000,953:0,980:4000,983:0",
                                           TWINE_TEST_INPUT="700:wheeldown,1500:wheeldown")),
        # (every weapon reached: Flashpoint's carried to Meltdown, the next-weapon button all round)
        "p_all": Run("p_all", 6, 140, dict(base, TWINE_ENDLESS_NEXT="10:-1", TWINE_ENDLESS_TEST_EXIT="200,5000",
                                           TWINE_TEST_INPUT=",".join("%d:wheeldown" % t for t in range(900, 2500, 100)))),
    }
    del runs["p_inv"].env["TWINE_ENDLESS_TEST_DRAW"]
    # (the playtest's: the AR36 carried through a door into Meltdown, where the game's slot is the P2K's till the AR36
    # comes out at frame 61: the strip names the AR36 from the room's first frame; the pause menu opened and left over
    # that room's strip; RESTART MISSION, the fresh run's first room)
    runs["p_carry"] = Run("p_carry", 8, 50, dict(base, TWINE_ENDLESS_ROOM="8:-1", TWINE_ENDLESS_NEXT="10:4",
                                                 TWINE_ENDLESS_TEST_EXIT="200,5000", TWINE_ENDLESS_TEST_DRAW="AR36",
                                                 TWINE_ENDLESS_TEST_RESTART=400, TWINE_TRACE="select,change,strip",
                                                 TWINE_TEST_PAD="345:0008,348:0,410:4000,413:0", TWINE_SHOT="330"))
    camp = {"TWINE_TRACE": "select,change", "TWINE_RENDERER": "hw", "TWINE_SMOOTH": 1,
            "TWINE_TEST_INPUT": "600:wheeldown,700:wheeldown,800:wheelup,1200:x,1300:z"}
    runs["p_camp"] = Run("p_camp", 8, 75, camp)
    runs["p_camp_old"] = Run("p_camp_old", 8, 75, dict(camp, TWINE_TEST_SELECT_OLD=1))
    return runs


def in_hand(log, level):
    return [m for m in re.findall(r"trace: L%d f\d+ select: in hand (.+?) \(before" % level, log)]


def strip_letters(img, y0=0.73, y1=0.81):
    """the share of white (the strip's numbers) where the strip is drawn: on top of the game's name band, centred"""
    if not img:
        return 0.0
    w, h, px = img
    n = hit = 0
    for y in range(int(y0 * h), int(y1 * h), 2):
        for x in range(int(0.40 * w), int(0.60 * w), 2):
            i = (y * w + x) * 3
            n += 1
            hit += min(px[i], px[i + 1], px[i + 2]) > 200
    return hit / n if n else 0.0


def strip_rooms(log):
    """each room's start: (level, the weapon drawn as it starts, the strip's names in its first 90 frames, the names
    after a menu over it closed, whether a menu opened in it, how the room came: door / restart / first)"""
    out = []
    parts = re.split(r"(?=endless: \S+ f0 v\d+ room start: level)", log)
    for i, p in enumerate(parts[1:], 1):
        lv = int(re.match(r"endless: \S+ f0 v\d+ room start: level (\d+)", p).group(1))
        drawn = re.search(r"f0 v\d+ weapon: (.+?) drawn", p)
        early = [n for f, n in re.findall(r"trace: L%d f(\d+) strip: \"(.+?)\" drawn" % lv, p) if int(f) < 90]
        early += [n for f, n in re.findall(r"trace: L%d f(\d+) select: strip shown: group \d+ lit, \"(.+?)\"" % lv, p) if int(f) < 90]
        menu = p.split("strip: a menu closed", 1)
        after = re.findall(r"strip: \"(.+?)\" drawn", menu[1]) if len(menu) > 1 else []
        prev = parts[i - 1]
        how = "restart" if "RESTART MISSION: the run begins again" in prev else "door" if "carry: Bond leaves with" in prev else "first"
        out.append((lv, drawn.group(1) if drawn else None, early, after, "strip: a menu open" in p, how))
    return out


def check_select(runs):
    r = runs["p_kr"]
    p = r.log.split("room start: level 8 ", 1)[-1]
    got = in_hand(p, 8)[1:]  # (the first: the carried IAC drawn as the room starts)
    want = [x for _, _, x in SEL_INPUTS]
    result("r", "King's Ransom, IAC + flash bang lent in one slot, only they checked: the wheel, Z/X, the d-pad step "
           "between them, Q and L1 flip, keys 6 and 1 by group (hw, smooth)", got[:len(want)] == want,
           "in hand: %s" % ", ".join(got) if got[:len(want)] != want else "%d changes as asked" % len(want))
    shown = len(re.findall(r"select: strip shown", p))
    before, on = (strip_letters(read_ppm(os.path.join(r.dir, "screen_%04d_0.ppm" % f))) for f in (895, 925))
    band, name = (strip_letters(read_ppm(os.path.join(r.dir, "screen_%04d_0.ppm" % f)), 0.815, 0.885) for f in (895, 925))
    named = len(re.findall(r"select: the name band for the lent weapon in the watch slot", p))
    result("r", "HUD strip on each change, one box with the name in the game's name band's place (its band asked for the lent weapon in King's Ransom's watch "
           "slot too; hw, smooth)", shown >= len(want) and named >= 6 and on > 0.01 and before < on / 4 and
           name > 0.01 and band < name / 4, "%d shown, %d name bands; numbers %.3f -> %.3f, name %.3f -> %.3f" %
           (shown, named, before, on, band, name))
    # the strip at a room's start: the weapon carried in (the AR36 into Meltdown), not the mission's own in the game's
    # slot meanwhile; after the pause menu; the fresh run's first room after RESTART MISSION
    rooms = strip_rooms(runs["p_carry"].log)
    door = [x for x in rooms if x[5] == "door" and x[0] == 10]
    rest = [x for x in rooms if x[5] == "restart"]
    bad = ["L%d (%s) drawn %s, strip %s" % (lv, how, d, ", ".join(sorted(set(n for n in e if n != d))))
           for lv, d, e, a, m, how in rooms if any(n != d for n in e)]
    car = door[0] if door else None
    ok = bool(car) and car[1] == "AR36 SNIPER" and len(car[2]) >= 20 and not bad
    result("r", "HUD strip at a room's start names the weapon carried in (the AR36 through a door into Meltdown: each "
           "frame of its first 90; hw, smooth)", ok, "; ".join(bad) or ("Meltdown: %s drawn, %d strip frames" %
           (car[1], len(car[2])) if car else "no room through a door into Meltdown"))
    # (the menu opened over the strip, at room frame 85: the game's band is over once it closes, so none or the AR36)
    menu = car and car[4] and all(n == "AR36 SNIPER" for n in car[3])
    result("r", "... and after the pause menu over it (none, or the AR36)", bool(menu),
           "menu %s, after it: %s" % (car[4], ", ".join(sorted(set(car[3]))) or "no strip") if car else "no room")
    result("r", "... and in the fresh run's first room after RESTART MISSION", bool(rest) and bool(rest[0][2]) and
           all(n == rest[0][1] for n in rest[0][2]), "%s drawn, strip %s" % (rest[0][1], ", ".join(sorted(set(rest[0][2]))))
           if rest else "no restart")
    # the checkmarks kept: the list in Meltdown and Courier as King's Ransom left it
    lists = re.findall(r"trace: L(\d+) f\d+ select: the next-weapon button: .*?; list (.+)", runs["p_ticks"].log)
    def marks(lst):
        return {re.sub(r"\[-\]$", "", n.rsplit("(", 1)[0]): not n.rsplit("(", 1)[0].endswith("[-]")
                for n in lst.split(", ")}
    ok, det = len(lists) >= 3, []
    for lv, lst in lists[1:]:
        m = marks(lst)
        bad = [n for n, t in m.items() if t != (n.lstrip("*").startswith(("IAC", "FLASH")))]
        ok = ok and not bad
        det.append("L%s %d weapons%s" % (lv, len(m), (": wrong " + ", ".join(bad)) if bad else " as left"))
    result("r", "checkmarks kept for the run (King's Ransom's, in Meltdown and Courier)", ok, "; ".join(det) or "no lists")
    lists = re.findall(r"trace: L(\d+) f\d+ select: the next-weapon button: .*?; list (.+)", runs["p_inv"].log)
    toggled = "WOLFRAM P2K out of the weapon buttons' round" in runs["p_inv"].log
    later = [lst for lv, lst in lists if lv == "10"]
    result("r", "a checkmark taken off in the inventory: kept into the next mission", toggled and bool(later) and
           "WOLFRAM P2K[-]" in later[0], "toggled %s; Meltdown: %s" % (toggled, later[0][:80] if later else "no list"))
    a = runs["p_all"]
    first = re.search(r"select: the next-weapon button: .*?; list (.+)", a.log.split("room start: level 10 ", 1)[-1])
    names = [n.rsplit("(", 1)[0].lstrip("*").replace("[-]", "") for n in first.group(1).split(", ")] if first else []
    held = set(in_hand(a.log.split("room start: level 10 ", 1)[-1], 10))
    miss = [n for n in names if not any(h.startswith(n) for h in held)]
    lent = re.findall(r"weapons: (.+?) (?:lent from level|waits in the pool)", a.log.split("room start: level 10 ", 1)[0].rsplit("room start:", 1)[-1] + a.log.split("room start: level 10 ", 1)[-1].split("room start:", 1)[0])
    result("r", "Meltdown: every weapon carried (%d, %d lent) reached by the next-weapon button" % (len(names), len(lent)),
           bool(names) and len(lent) >= 3 and not miss, "never in hand: %s" % ", ".join(miss) if miss else "all")
    c, o = (re.findall(r"change: to (\d+) .* by the (?:next|previous)-weapon button", runs[k].log) for k in ("p_camp", "p_camp_old"))
    result("r", "campaign: the weapon buttons step as the game's own (King's Ransom)", len(c) >= 4 and c == o,
           "steps %s / old %s" % (" ".join(c), " ".join(o)))


def check_campaign(runs):
    for how, k in (("campaign", "c"), ("a run", "e")):
        (ts, xs, ss), (to, xo, so) = hitloop_ticks(runs["hl_%s_smooth" % k]), hitloop_ticks(runs["hl_%s_off" % k])
        again = len(re.findall(r"hitloop: .*AGAIN this tick", runs["hl_%s_smooth" % k].log))
        # (the campaign's runs go the same way tick for tick; a run's guards don't quite, smooth or not: there each
        # tick's loop once, none in an extra drawing)
        same = ts == to if k == "c" else bool(ts)
        result("i", "smooth motion: the hit loop once a tick, none in extra drawings%s (%s; hw renderer)" % (
               ", as without" if k == "c" else "", how), bool(to) and same and xs == 0 and ss > 0 and not again,
               "with smooth motion %d ticks with hits (%d hits, %d sounds), %d in extra drawings, %d extra drawings skipping it; "
               "without %d ticks (%d hits, %d sounds)%s" % (len(ts), sum(x[1] for x in ts), sum(x[2] for x in ts), xs + again, ss,
                                                           len(to), sum(x[1] for x in to), sum(x[2] for x in to),
                                                           "" if ts == to or k == "e" else "; first different tick: %s / %s" % next(
                                                               ((a, b) for a, b in zip(ts + [None], to + [None]) if a != b), (None, None))))
    for k, r in runs.items():
        if not k.startswith("campaign_"):
            continue
        result("i", "level " + k.split("_")[1], "game main" not in r.log[:0] and not r.lines("endless:") and not r.crashed()
               and "Exec level" in r.log, "endless lines %d, crash %s" % (len(r.lines("endless:")), r.crashed()))


def main():
    global WORK, DISC
    args = sys.argv[1:]
    jobs, checks, rooms, verbose = 4, "abcdefghijklmnopqr", None, False
    while args:
        a = args.pop(0)
        if a == "-j":
            jobs = int(args.pop(0))
        elif a == "-k":
            checks = args.pop(0)
        elif a == "-r":
            rooms = [tuple(int(v) for v in r.split(":")) for r in args.pop(0).split(",")]
        elif a == "-v":
            verbose = True
        elif a == "-8":
            global RAM8
            RAM8 = True
        elif a == "-o":
            global AI_ORIGINAL
            AI_ORIGINAL = True
        elif a == "-p":
            global TURBO
            TURBO = False
        else:
            sys.exit(__doc__)
    jobs = max(1, min(jobs, JOBS_MAX if TURBO else 5))  # (never more at once: the machine's desktop stays well)
    if not os.path.exists(BIN):
        sys.exit("endless_check: build the port first (make -C port)")
    WORK = os.environ.get("TWINE_CHECK_DIR", os.path.join(ROOT, "build", "endless_check"))
    DISC = disc()
    os.makedirs(WORK, exist_ok=True)
    rooms = rooms or pool_rooms()
    groups = {}
    if set(checks) & set("abc"):
        groups["rooms"] = {r: room_run(r) for r in rooms}
    if "d" in checks:
        groups["rules"] = {"rules_kr": Run("rules_kr", 8, 150, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "8:4",
                                                               "TWINE_ENDLESS_SEED": 4, "TWINE_ENDLESS_TEST_GOD": 1}),
                           # (the bank's receptionist watched: Bond put before her, armed, hw renderer, smooth motion)
                           "rules_courier": Run("rules_courier", 2, 40, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "2:-1",
                                                                       "TWINE_ENDLESS_TEST_GOD": 1,
                                                                       "TWINE_ENDLESS_TEST_NEAR": 60,
                                                                       "TWINE_TRACE": "person", "TWINE_TRACE_TYPE": 60,
                                                                       "TWINE_RENDERER": "hw", "TWINE_SMOOTH": 1}),
                           # (King's Ransom's "hurry James", its mission logic's line at f1020 in 8:5: muted; as played,
                           # hw renderer, smooth motion, Bond aiming and firing)
                           "rules_hurry": Run("rules_hurry", 8, 75, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "8:5",
                                                                    "TWINE_ENDLESS_SEED": 5, "TWINE_ENDLESS_TEST_GOD": 1,
                                                                    "TWINE_TRACE": "sound", "TWINE_RENDERER": "hw",
                                                                    "TWINE_SMOOTH": 1, "TWINE_TEST_PAD":
                                                                    "300:0800,900:4800,960:0800,1100:0"})}
    if "e" in checks:
        groups["menus"] = menu_runs()
    if "f" in checks:
        groups["restart"] = restart_runs()
        groups["sound_stop"] = sound_stop_runs()
    if "g" in checks:
        groups["carry"] = carry_runs()
        groups["iris"] = iris_runs()
        groups["lend"] = lend_runs()
    if "h" in checks:
        groups["card"] = card_runs()
    if "i" in checks:
        groups["campaign"] = campaign_runs()
    if "j" in checks:
        groups["ai"] = ai_runs()
    if "k" in checks:
        groups["lent"] = lent_runs()
    if "l" in checks:
        groups["texts"] = text_runs()
    if "m" in checks:
        groups["shell"] = shell_runs()
    if "n" in checks:
        groups["doors"] = door_runs(rooms)
    if "o" in checks:
        groups["ramp"] = ramp_runs()
    if "p" in checks:
        groups["people"] = people_runs()
    if "q" in checks:
        groups["bar"] = bar_runs()
    if "r" in checks:
        groups["select"] = select_runs()
    every = [r for g in groups.values() for r in g.values()]
    every.sort(key=lambda r: -r.cost())  # (the long ones first)
    t0 = time.time()
    print("endless_check: %d runs, %d at once%s ..." % (len(every), jobs, "" if TURBO else ", paced (real time)"), flush=True)
    with cf.ThreadPoolExecutor(jobs) as ex:
        for r in ex.map(Run.go, every):
            pass
    again = [r for r in every if r.timed_out]
    if again:  # (a run the harness had to stop: once more, before any check reads it)
        print("endless_check: stopped (%s), run again: %s" % ("; ".join(sorted({r.stopped for r in again})),
                                                             ", ".join(r.name for r in again)), flush=True)
        for r in again:
            r.first = "%s, %.0f s" % (r.stopped, r.wall)
        with cf.ThreadPoolExecutor(jobs) as ex:
            for r in ex.map(Run.go, again):
                pass
    open(os.path.join(WORK, "times.txt"), "w").write("".join(
        "%-24s %6.1f s wall for %3d s of game time%s%s\n" % (
            r.name, r.wall, r.secs, " (STOPPED: %s)" % r.stopped if r.timed_out else "",
            " (run again; the first stopped: %s)" % getattr(r, "first") if hasattr(r, "first") else "") for r in every))
    stuck = [r.name for r in every if r.timed_out]
    if stuck:
        print("endless_check: STOPPED twice, never reaching their vblanks (their checks STOP, not FAIL): " +
              ", ".join(stuck), flush=True)
    if "rooms" in groups:
        for room, r in groups["rooms"].items():
            checked([r], check_room, r, room, checks)
    if "rules" in groups:
        checked(groups["rules"].values(), check_rules, groups["rules"])
    if "menus" in groups:
        checked(groups["menus"].values(), check_menus, groups["menus"])
    if "restart" in groups:
        checked(groups["restart"].values(), check_restart, groups["restart"])
    if "sound_stop" in groups:
        checked(groups["sound_stop"].values(), check_sound_stop, groups["sound_stop"])
    if "carry" in groups:
        checked(groups["carry"].values(), check_carry, groups["carry"])
    if "iris" in groups:
        checked(groups["iris"].values(), check_iris, groups["iris"])
    if "lend" in groups:
        checked(groups["lend"].values(), check_lend, groups["lend"])
    if "card" in groups:
        checked(groups["card"].values(), check_card, groups["card"])
    if "campaign" in groups:
        checked(groups["campaign"].values(), check_campaign, groups["campaign"])
    if "ai" in groups:
        checked(groups["ai"].values(), check_ai, groups["ai"])
    if "lent" in groups:
        checked(groups["lent"].values(), check_lent, groups["lent"])
    if "texts" in groups:
        checked(groups["texts"].values(), check_texts, groups["texts"])
    if "shell" in groups:
        checked(groups["shell"].values(), check_shell, groups["shell"])
    if "people" in groups:
        checked(groups["people"].values(), check_people, groups["people"])
    if "bar" in groups:
        checked(groups["bar"].values(), check_bar, groups["bar"])
    if "doors" in groups:
        checked(groups["doors"].values(), check_doors, groups["doors"])
    if "ramp" in groups:
        checked(groups["ramp"].values(), check_ramp, groups["ramp"])
    if "select" in groups:
        checked(groups["select"].values(), check_select, groups["select"])
    names = {"a": "guards fight", "b": "nobody unkillable", "c": "spawns valid", "d": "room rules", "e": "menus",
             "f": "restart", "g": "weapon carry", "h": "card save", "i": "campaign", "j": "enemy AI",
             "k": "lent weapons", "l": "text letters", "m": "shell start", "n": "exits", "o": "wave ramp",
             "p": "12 people", "q": "the bar's guards", "r": "weapon list"}
    width = max(len(w) for _, w, _, _ in RESULTS) if RESULTS else 10
    report = ["%s %-18s %-*s %s  %s" % (c, names[c], width, w, s, d) for c, w, s, d in sorted(RESULTS, key=lambda x: x[0])]
    print("\n".join(report))
    open(os.path.join(WORK, "report.txt"), "w").write("\n".join(report) + "\n")
    fails = [x for x in RESULTS if x[2] == "FAIL"]
    stops = [x for x in RESULTS if x[2] in ("STOP", "ENV")]
    print("endless_check: %d passed, %d failed, %d skipped%s in %d s" %
          (sum(x[2] == "PASS" for x in RESULTS), len(fails), sum(x[2] == "SKIP" for x in RESULTS),
           ", %d STOPPED or ENV (a run stopped by the harness, or hw asked with no GPU: not the check's verdict)" % len(stops)
           if stops else "",
           time.time() - t0))
    if verbose:
        for g in groups.values():
            for r in g.values():
                if any(f[1] in r.name or r.name.endswith(f[1].replace(":", "_")) for f in fails):
                    print("---- %s\n%s" % (r.name, "\n".join(r.log.splitlines()[-15:])))
    sys.exit(1 if fails else 2 if stops else 0)


if __name__ == "__main__":
    main()
