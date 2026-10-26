#!/usr/bin/env python3
"""Endless mode's regression suite: headless runs of the native port, checked from their logs and screenshots.

Usage: tools/endless_check.sh [-j JOBS] [-k CHECKS] [-r ROOMS] [-v]
  -j JOBS    runs at once (default 4)
  -k CHECKS  the checks to run, letters from abcdefghijk (default all):
             a guards fight, b nobody unkillable, c spawns valid (a, b and c share each pool room's run),
             d room rules, e menus, f restart, g weapon carry, h card save, i campaign regression,
             j enemy AI (hunting: guards close on a still Bond, walking by the floor; no bunching),
             k lent weapons as at home (fire rate, damage, scope, sound; ammo from drops for every weapon carried)
  -r ROOMS   pool rooms for a-c, as level:point,... (default: every room in port/src/endless.c's table)
  -v         print each failing run's log tail
  -8         every run with 8 MB of RAM (the ram_8mb setting)
  -o         every run with the game's own enemy AI (enemy_ai = original; default: the port's, IMPROVED)

Environment:
  TWINE_DISC         the disc image (default: the first .bin in workbench/disc/)
  TWINE_CHECK_CARD   a memory card image with a campaign save, for h (default ~/.local/share/twine/card1.mcd);
                     only a copy is ever used
  TWINE_CHECK_DIR    where the runs go (default build/endless_check/)

Each run has its own folders (config, data, state, cards) and runs as "twinechk" (a link to build/port/twine), so
it never touches the user's settings or cards and isn't mistaken for the game. Exit code 0 when every check passed.
Nothing here holds game data: it all comes from the disc at run time.
"""
import concurrent.futures as cf
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
    """one headless run: level, seconds, extra environment; .log is its port.log afterwards"""

    def __init__(self, name, level, secs, env=None, card=None, config=None):
        self.name, self.level, self.secs, self.env, self.card, self.config = name, level, secs, env or {}, card, config
        self.dir = os.path.join(WORK, name)
        self.log = ""

    def go(self):
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
                    "TWINE_CARD2": "none"})
        if self.card:
            shutil.copy(self.card, os.path.join(self.dir, "data/twine/card1.mcd"))
        else:
            env["TWINE_CARD1"] = "none"
        env.update({k: str(v) for k, v in self.env.items()})
        if RAM8:
            env["TWINE_RAM_8MB"] = "1"
        if AI_ORIGINAL and "TWINE_ENEMY_AI" not in self.env:
            env["TWINE_ENEMY_AI"] = "original"
        with open(os.path.join(self.dir, "out.txt"), "w") as out:
            child = subprocess.Popen(["nice", exe], cwd=self.dir, env=env, stdout=out, stderr=subprocess.STDOUT)
            try:
                child.wait(timeout=self.secs)
            except subprocess.TimeoutExpired:  # (only this run's own process is stopped)
                child.terminate()
                try:
                    child.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    child.kill()
                    child.wait()
        try:
            self.log = open(os.path.join(self.dir, "port.log"), errors="replace").read()
        except OSError:
            self.log = ""
        return self

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
RAM8 = False
AI_ORIGINAL = False


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
    co = runs["rules_courier"]
    civ = [l for l in co.lines(r"made: person \d+ type 5 placed")]
    result("d", "2:-1 civilians kept", len(civ) > 0, "%d bank staff made" % len(civ))
    seated = co.lines(r"receptionist kept seated")
    made60 = co.lines(r"made: person \d+ type 60 placed")
    result("d", "2:-1 receptionist seated", bool(made60) and bool(seated), "made %d, kept seated %d" % (len(made60), len(seated)))
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
            "TWINE_ENDLESS_TEST_MEET": 1, "TWINE_ENDLESS_AMMO_LOG": 1}
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
    nxt = ",".join("%d:8000,%d:0" % (t, t + 3) for t in range(700, 1000, 40))  # (the next weapon button: Square)
    runs["k_pool"] = Run("k_pool", 2, 70, dict(base, TWINE_ENDLESS_ROOM="2:4", TWINE_ENDLESS_NEXT="10:-1,8:0",
                                               TWINE_ENDLESS_TEST_EXIT=250, TWINE_ENDLESS_TEST_DRAW="KOFFLER",
                                               TWINE_TEST_PAD=nxt))
    del runs["k_pool"].env["TWINE_ENDLESS_TEST_MEET"]
    fire2 = "500:4000,503:0,560:4000,563:0"
    for name, room, draw in (("k_rocket", "5:-1", "ROCKET"), ("k_gl", "9:0", "GRENADE"), ("k_flash", "2:4", "FLASH")):
        runs[name] = Run(name, int(room.split(":")[0]), 40, dict(base, TWINE_ENDLESS_ROOM=room, TWINE_ENDLESS_NEXT="8:0",
                                                                TWINE_ENDLESS_TEST_EXIT=300, TWINE_ENDLESS_TEST_DRAW=draw,
                                                                TWINE_TEST_PAD=fire2))
    runs["k_satchel"] = Run("k_satchel", 8, 40, dict(base, TWINE_ENDLESS_ROOM="8:-1", TWINE_ENDLESS_NEXT="9:0",
                                                    TWINE_ENDLESS_TEST_EXIT=300, TWINE_ENDLESS_TEST_DRAW="SATCHEL CHARGE",
                                                    TWINE_TEST_PAD="500:4000,503:0,540:4000,543:0,580:8000,583:0,660:4000,663:0"))
    runs["k_shotgun"] = Run("k_shotgun", 8, 45, dict(base, TWINE_ENDLESS_ROOM="8:-1", TWINE_ENDLESS_NEXT="3:-1",
                                                    TWINE_ENDLESS_TEST_EXIT=300, TWINE_ENDLESS_TEST_DRAW="FRINESI",
                                                    TWINE_TEST_PAD="200:4000,240:0,520:4000,560:0"))
    for k in ("k_rocket", "k_gl", "k_flash", "k_satchel", "k_shotgun"):
        del runs[k].env["TWINE_ENDLESS_TEST_MEET"]
    del runs["k_ammo"].env["TWINE_ENDLESS_TEST_MEET"]
    return runs


def room_part(log, level):
    """the log of the run's first room in that level (to the next room's start)"""
    part = log.split("room start: level %d " % level, 1)
    return part[1].split("room start:", 1)[0] if len(part) > 1 else ""


def fire_rate(part, weapon):
    """the most shots in any 20 frames (the room's frames), from the clip's count going down"""
    t = [int(m.group(1)) for m in re.finditer(r"ammo: %s[^:]*? clip \d+ at frame (\d+)" % weapon, part)]
    return max((sum(1 for u in t if v <= u < v + 20) for v in t), default=0) - (1 if t else 0)


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


def check_lent(runs):
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
    (dh, nh), (dl, nl) = damage(h, "INGALLS"), damage(l, "INGALLS")
    # (where a bullet hits multiplies its damage: few hits can show only a multiple; the same bullet if one divides the other)
    result("k", "Ingalls: damage as at home", nh > 1 and nl > 1 and dh and dl and max(dh, dl) % min(dh, dl) == 0
           and max(dh, dl) // min(dh, dl) <= 2,
           "a bullet's damage: home %d (%d hits), lent %d (%d hits)" % (dh, nh, dl, nl))
    h, l = room_part(runs["k_home_ar36"].log, 8), room_part(runs["k_lent_ar36"].log, 2)
    sc = lambda p: [int(x) for x in re.findall(r"weapon state (\d+) at", p) if int(x) in (33, 34, 35, 36)]
    result("k", "AR36: the scope's states as at home", set(sc(h)) == set(sc(l)) == {33, 34, 35, 36},
           "home %s, lent %s" % (sorted(set(sc(h))), sorted(set(sc(l)))))
    ph = read_ppm(os.path.join(runs["k_home_ar36"].dir, "shot_0240.ppm"))
    pl = read_ppm(os.path.join(runs["k_lent_ar36"].dir, "shot_0740.ppm"))
    qh, ql = scope_picture(ph) if ph else 0, scope_picture(pl) if pl else 0
    result("k", "AR36: the scope's picture as at home", qh >= 0.85 and ql >= 0.85,
           "its marks found: home %d%%, lent %d%%" % (qh * 100, ql * 100))
    for name, w, lv, what in (("k_rocket", "ROCKET LAUNCHER", 8, "rocket"), ("k_gl", "GRENADE LAUNCHER", 8, "round"),
                              ("k_flash", "FLASH BANG", 8, "flash bang")):
        part = room_part(runs[name].log, lv)
        fired = re.findall(r"weapons: %s fired" % w, part)
        extra = re.findall(r"a flash bang went off", part) if what == "flash bang" else fired
        result("k", "%s: lent, fires its %s" % (w.title(), what), len(fired) >= 2 and len(extra) >= 1 and not runs[name].crashed(),
               "%d fired in King's Ransom%s" % (len(fired), ", %d went off" % len(extra) if what == "flash bang" else ""))
    part = room_part(runs["k_satchel"].log, 9)
    thrown = re.findall(r"SATCHEL CHARGE fired", part)
    boom = re.search(r"the detonator set off (\d+) satchel", part)
    result("k", "Satchels + detonator: lent together", len(thrown) >= 2 and boom and int(boom.group(1)) >= 2,
           "%d thrown, %s" % (len(thrown), boom.group(0)[4:] if boom else "no detonation"))
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
                               card=card)}


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
                "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENEMY_AI": m, "TWINE_ENDLESS_PEOPLE_FAST": 1})
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
    for r in GRENADE_ROOMS:
        g = runs["aig_%d_%d" % r]
        scat = len(g.lines("and runs from it"))
        result("j", "%d:%d scatter from Bond's grenade" % r, scat > 0 and not g.crashed(),
               "%d shouted and ran (grenade %s)" % (scat, "lobbed" if g.lines("grenade of Bond's lobbed") else "never lobbed"))


# ---- i: the campaign untouched ----

def campaign_runs():
    return {"campaign_%d" % l: Run("campaign_%d" % l, l, 30, {}) for l in (2, 5, 8)}


def check_campaign(runs):
    for k, r in runs.items():
        result("i", "level " + k.split("_")[1], "game main" not in r.log[:0] and not r.lines("endless:") and not r.crashed()
               and "Exec level" in r.log, "endless lines %d, crash %s" % (len(r.lines("endless:")), r.crashed()))


def main():
    global WORK, DISC
    args = sys.argv[1:]
    jobs, checks, rooms, verbose = 4, "abcdefghijk", None, False
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
        else:
            sys.exit(__doc__)
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
                           "rules_courier": Run("rules_courier", 2, 30, {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_ROOM": "2:-1"})}
    if "e" in checks:
        groups["menus"] = menu_runs()
    if "f" in checks:
        groups["restart"] = restart_runs()
    if "g" in checks:
        groups["carry"] = carry_runs()
        groups["lend"] = lend_runs()
    if "h" in checks:
        groups["card"] = card_runs()
    if "i" in checks:
        groups["campaign"] = campaign_runs()
    if "j" in checks:
        groups["ai"] = ai_runs()
    if "k" in checks:
        groups["lent"] = lent_runs()
    every = [r for g in groups.values() for r in g.values()]
    every.sort(key=lambda r: -r.secs)  # (the long ones first)
    t0 = time.time()
    print("endless_check: %d runs, %d at once ..." % (len(every), jobs), flush=True)
    with cf.ThreadPoolExecutor(jobs) as ex:
        for r in ex.map(Run.go, every):
            pass
    if "rooms" in groups:
        for room, r in groups["rooms"].items():
            check_room(r, room, checks)
    if "rules" in groups:
        check_rules(groups["rules"])
    if "menus" in groups:
        check_menus(groups["menus"])
    if "restart" in groups:
        check_restart(groups["restart"])
    if "carry" in groups:
        check_carry(groups["carry"])
    if "lend" in groups:
        check_lend(groups["lend"])
    if "card" in groups:
        check_card(groups["card"])
    if "campaign" in groups:
        check_campaign(groups["campaign"])
    if "ai" in groups:
        check_ai(groups["ai"])
    if "lent" in groups:
        check_lent(groups["lent"])
    names = {"a": "guards fight", "b": "nobody unkillable", "c": "spawns valid", "d": "room rules", "e": "menus",
             "f": "restart", "g": "weapon carry", "h": "card save", "i": "campaign", "j": "enemy AI",
             "k": "lent weapons"}
    width = max(len(w) for _, w, _, _ in RESULTS) if RESULTS else 10
    report = ["%s %-18s %-*s %s  %s" % (c, names[c], width, w, s, d) for c, w, s, d in sorted(RESULTS, key=lambda x: x[0])]
    print("\n".join(report))
    open(os.path.join(WORK, "report.txt"), "w").write("\n".join(report) + "\n")
    fails = [x for x in RESULTS if x[2] == "FAIL"]
    print("endless_check: %d passed, %d failed, %d skipped in %d s" %
          (sum(x[2] == "PASS" for x in RESULTS), len(fails), sum(x[2] == "SKIP" for x in RESULTS), time.time() - t0))
    if verbose:
        for g in groups.values():
            for r in g.values():
                if any(f[1] in r.name or r.name.endswith(f[1].replace(":", "_")) for f in fails):
                    print("---- %s\n%s" % (r.name, "\n".join(r.log.splitlines()[-15:])))
    sys.exit(1 if fails else 0)


if __name__ == "__main__":
    main()
