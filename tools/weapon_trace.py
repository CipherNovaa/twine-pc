#!/usr/bin/env python3
"""A weapon at home against the same weapon lent, from TWINE_TRACE=weapon,change logs (port/mods/trace.c).

Usage: weapon_trace.py HOME_LOG LENT_LOG NAME [HOME_LEVEL LENT_LEVEL]
       weapon_trace.py --proj ...   (Bond's projectiles compared for any weapon, not only the launchers)
       weapon_trace.py --summary LOG NAME [LEVEL]   (exit 1 when its animations' playhead jumps: e.g. a real play's log)

For the weapon whose name starts with NAME, in each log (only frames in LEVEL when given), it gathers:
  - each state the weapon went through (D_800F11F0), with the animation each state asks for and how long that
    animation runs (its frames left when it starts) and how far its playhead moves a frame (the first track's
    position, +0x16: the animation's speed as seen),
  - the muzzle flash: frames it is shown in each state (a flash shown while idle is a stuck flash),
  - the weapon changes (who asked), and how many changes the game made by itself in a row (constant switching).
Then it prints both and the differences. Exit code 0 when the lent weapon behaves as at home (same states, the
same animation in each, same lengths and speeds, no flash shown outside firing, no change churn), else 1.

Run it on headless runs with the same pad script in the same room type (tools/endless_check.py check k does).

With TWINE_TRACE=proj in both logs, Bond's projectiles are compared too (what his weapon makes when it fires: a
rocket, a grenade launcher's round, a gun's bullets): each one's subtype, hit mask (+0x3A), +0x0E, blast flag (+0x64
bit 23), damage (+0x34), +0x68 and its type's entry (but its number, each mission's own), how fast it flies (its
speed along the ground a frame) and drops (its height's change from frame to frame), and the blast where it went
off (the reach it starts and ends at, and grows by)."""
import re
import sys
from collections import defaultdict, Counter

LINE = re.compile(r"trace: L(\d+) f(-?\d+) weapon: slot (-?\d+) \(before (-?\d+), to (-?\d+)\) (.*?), state (-?\d+), "
                  r"model (-?\d+) loaded (-?\d+)(?: \(loading\))? file (\S+), anim (-?\d+) asked (-?\d+) left (-?\d+), track frame "
                  r"(-?\d+) len (-?\d+) speed (-?\d+) start (-?\d+), flash (\w+), clip (-?\d+) reserve (-?\d+), fire (\d) "
                  r"timer (-?\d+), rate (\d+)/(\d+)(, menu)?")
CHANGE = re.compile(r"trace: L(\d+) f(-?\d+) change: to (-?\d+) \((.*?)\) from (-?\d+), in hand (-?\d+), by (.*)")


def frames(log, name, level=None):
    """[dict] of the trace's weapon lines while the weapon NAME is in hand (in LEVEL)"""
    out = []
    for line in open(log, errors="replace"):
        m = LINE.search(line)
        if not m:
            continue
        g = m.groups()
        if level is not None and int(g[0]) != level:
            continue
        if not g[5].startswith(name):
            continue
        out.append(dict(level=int(g[0]), f=int(g[1]), slot=int(g[2]), name=g[5], state=int(g[6]), model=int(g[7]),
                        loaded=int(g[8]), file=g[9], anim=int(g[10]), asked=int(g[11]), left=int(g[12]),
                        length=int(g[14]), speed=int(g[15]), at=int(g[16]), flash=g[17], clip=int(g[18]),
                        fire=int(g[20]), rate=int(g[22]), menu=bool(g[24])))
    return out


def changes(log, level=None):
    out = []
    for line in open(log, errors="replace"):
        m = CHANGE.search(line)
        if m and (level is None or int(m.group(1)) == level):
            out.append((int(m.group(2)), int(m.group(3)), m.group(4), m.group(7)))
    return out


def summary(fr, ch):
    """{state: {anims, lengths, steps, flash}} and other facts"""
    st = defaultdict(lambda: {"anims": Counter(), "lengths": Counter(), "steps": Counter(), "jumps": 0, "moves": 0,
                              "flash": 0, "frames": 0})
    prev = None
    for x in fr:
        s = st[x["state"]]
        s["frames"] += 1
        # (a state's first frame shows the animation asked before it: the state asks its own as it runs, seen the
        # frame after; a state left after one frame (the idle animation's turn taken as fire is pressed) has none)
        if x["asked"] >= 0 and prev and prev["state"] == x["state"] and prev["f"] + 1 == x["f"]:
            s["anims"][x["asked"]] += 1
        if x["flash"] == "SHOWN":
            s["flash"] += 1
        if prev and prev["f"] + 1 == x["f"] and prev["anim"] == x["anim"] and x["anim"] >= 0:
            if prev["left"] == 0 and x["left"] > 0 or x["left"] > prev["left"]:  # (an animation (re)starts)
                s["lengths"][x["left"]] += 1
            else:
                d = x["at"] - prev["at"]
                if d >= 0:
                    s["steps"][(x["rate"], d)] += 1
                # (a playhead moving more than a frame's worth a game frame, or back without the animation
                # starting again: the animation run on in frames that aren't the game's, e.g. smooth motion's
                # extra drawings when the model's memory isn't put back after them)
                s["moves"] += 1
                if d > max(2, x["rate"] // 10 + 1) or d < 0 and x["left"] <= prev["left"] and prev["left"] > 1:
                    s["jumps"] += 1
        prev = x
    churn, run = 0, 0
    for i in range(1, len(ch)):
        if ch[i][3] == "the game" and ch[i][0] - ch[i - 1][0] < 20:
            run += 1
            churn = max(churn, run)
        else:
            run = 0
    return {"states": st, "churn": churn, "models": Counter((x["model"], x["loaded"]) for x in fr),
            "files": Counter(x["file"] for x in fr if x["state"] == 1), "frames": len(fr)}


PNEW = re.compile(r"trace: L(\d+) f(-?\d+) proj: #(\d+) new kind (\d+) type \d+/(\d+) \(entry ([0-9A-F]{8}) ([0-9A-F]{8}) "
                  r"([0-9A-F]{8}) .*?\) 3A (\d+) 3B \d+ 0E ([0-9A-F]{4}) 34 (-?\d+) 64 ([0-9A-F]{8}) 68 ([0-9A-F]{8}) by Bond "
                  r"at (-?\d+) (-?\d+) (-?\d+)")
PAT = re.compile(r"trace: L(\d+) f(-?\d+) proj: #(\d+) at (-?\d+) (-?\d+) (-?\d+)")
PGONE = re.compile(r"trace: L(\d+) f(-?\d+) proj: #(\d+) gone")
PBLAST = re.compile(r"trace: L(\d+) f(-?\d+) proj: blast \d+ at (-?\d+) (-?\d+) (-?\d+) by \w+, reach (\d+) to (\d+), (\d+) a frame")


def projectiles(log, level=None):
    """Bond's projectiles (kind 1) in the log (TWINE_TRACE=proj): [dict] with what it is, its path, its blast"""
    out, live, gone = [], {}, []
    for line in open(log, errors="replace"):
        m = PNEW.search(line)
        if m:
            g = m.groups()
            if level is not None and int(g[0]) != level:
                continue
            p = dict(f=int(g[1]), sub=int(g[4]), entry=(g[5] + g[6] + g[7])[2:], mask=int(g[8]), e=g[9], dmg=int(g[10]),
                     blastflag=int(g[11], 16) >> 23 & 1, b68=g[12], path=[tuple(map(int, g[13:16]))], blast=None,
                     end=None)
            live[g[2]] = p
            out.append(p)
            continue
        m = PAT.search(line)
        if m and m.group(3) in live:
            live[m.group(3)]["path"].append(tuple(map(int, m.groups()[3:6])))
            continue
        m = PGONE.search(line)
        if m and m.group(3) in live:
            p = live.pop(m.group(3))
            p["end"] = int(m.group(2))
            gone.append(p)
            continue
        m = PBLAST.search(line)
        if m:
            f, at = int(m.group(2)), tuple(map(int, m.groups()[2:5]))
            for p in gone[-4:]:
                last = p["path"][-1]
                if p["blast"] is None and 0 <= f - p["end"] <= 2 and all(abs(a - b) < 1500 for a, b in zip(at, last)):
                    p["blast"] = tuple(map(int, m.groups()[5:8]))
    return out


def projectile_facts(p):
    """what's the same wherever it's fired: the projectile, its ground speed and drop over its first frames, its blast"""
    path = p["path"][:5]
    speed = [round(((b[0] - a[0]) ** 2 + (b[2] - a[2]) ** 2) ** 0.5 / 10) * 10 for a, b in zip(path, path[1:])]
    dy = [b[1] - a[1] for a, b in zip(path, path[1:])]
    drop = [b - a for a, b in zip(dy, dy[1:])]
    return dict(what=(p["sub"], p["entry"], p["mask"], p["e"], p["dmg"], p["blastflag"], p["b68"]), speed=speed[:3],
                drop=drop[:2], blast=p["blast"])


def compare_projectiles(hl, ll):
    """differences between Bond's projectiles at home and lent ([] when alike)"""
    bad = []
    if not hl and not ll:
        return bad
    if not hl or not ll:
        return ["projectiles: %d at home, %d lent" % (len(hl), len(ll))]
    fh, fl = Counter(), Counter()
    for p in hl:
        fh[projectile_facts(p)["what"]] += 1
    for p in ll:
        fl[projectile_facts(p)["what"]] += 1
    if set(fh) != set(fl):
        bad.append("projectiles: at home %s, lent %s (subtype, entry, mask, +0E, damage, blast, +68)" % (sorted(fh), sorted(fl)))
    sh = Counter(str(projectile_facts(p)["speed"][:1]) for p in hl if len(p["path"]) > 1)
    sl = Counter(str(projectile_facts(p)["speed"][:1]) for p in ll if len(p["path"]) > 1)
    if sh and sl and common(sh) != common(sl):
        bad.append("projectiles: speed %s at home, %s lent" % (common(sh), common(sl)))
    dh = Counter(str(projectile_facts(p)["drop"][:1]) for p in hl if len(p["path"]) > 2)
    dl = Counter(str(projectile_facts(p)["drop"][:1]) for p in ll if len(p["path"]) > 2)
    if dh and dl and common(dh) != common(dl):
        bad.append("projectiles: drop a frame %s at home, %s lent" % (common(dh), common(dl)))
    bh = Counter(p["blast"] for p in hl if p["blast"])
    bl = Counter(p["blast"] for p in ll if p["blast"])
    if set(bh) != set(bl) or (sum(bh.values()) > 0) != (sum(bl.values()) > 0):
        bad.append("projectiles: blasts %s at home, %s lent" % (dict(bh), dict(bl)))
    return bad


def show_projectiles(title, pl):
    if pl:
        f = projectile_facts(pl[0])
        print("%s: %d projectiles of Bond's; the first: %s, speed %s, drop %s, blast %s" %
              (title, len(pl), f["what"], f["speed"], f["drop"], f["blast"]))


def show(title, s):
    print(f"{title}: {s['frames']} frames in hand, models (slot's, loaded) {dict(s['models'])}, files while idle "
          f"{dict(s['files'])}, change churn {s['churn']}")
    for k in sorted(s["states"]):
        v = s["states"][k]
        print(f"  state {k:3d}: {v['frames']:4d} frames, anims {dict(v['anims'])}, lengths {dict(v['lengths'])}, "
              f"steps {dict(v['steps'])}, jumps {v['jumps']} of {v['moves']}, flash shown {v['flash']}")


def jumpy(s, what):
    """states whose playhead jumps (more than 10% of its moves): a list of differences"""
    return [f"state {k}: the playhead jumps in {v['jumps']} of {v['moves']} frames ({what})"
            for k, v in sorted(s["states"].items()) if v["moves"] >= 8 and v["jumps"] * 10 > v["moves"]]


def common(c):
    return c.most_common(1)[0][0] if c else None


def compare(h, l):
    bad = []
    # (state 6: the next weapon's model still being read from the disc after the put-away; how long depends on the
    # disc, not on the weapon)
    for k in sorted((set(h["states"]) | set(l["states"])) - {6}):
        a, b = h["states"].get(k), l["states"].get(k)
        if not a or not b:
            if (a or b)["frames"] > 2:
                bad.append(f"state {k} only {'at home' if a else 'lent'}")
            continue
        if a["anims"] and b["anims"] and common(a["anims"]) != common(b["anims"]):
            bad.append(f"state {k}: animation {common(a['anims'])} at home, {common(b['anims'])} lent")
        if a["lengths"] and b["lengths"] and common(a["lengths"]) != common(b["lengths"]):
            bad.append(f"state {k}: lasts {common(a['lengths'])} frames at home, {common(b['lengths'])} lent")
        for rate in sorted({r for r, _ in a["steps"]} & {r for r, _ in b["steps"]}):  # (the game's rate: frames)
            sa = Counter({st: n for (r, st), n in a["steps"].items() if r == rate})
            sb = Counter({st: n for (r, st), n in b["steps"].items() if r == rate})
            if sum(sa.values()) >= 4 and sum(sb.values()) >= 4 and common(sa) != common(sb):
                bad.append(f"state {k} at rate {rate}: playhead {common(sa)} a frame at home, {common(sb)} lent")
    bad += jumpy(l, "lent") + jumpy(h, "at home")
    hf, lf = common(h["files"]), common(l["files"])
    if hf and lf and hf.split("/")[-1] != lf.split("/")[-1]:
        bad.append(f"model file {hf} at home, {lf} lent")
    if len(l["files"]) > 1:
        bad.append(f"more than one model file while idle (lent): {dict(l['files'])}")
    idle, hidle = l["states"].get(1), h["states"].get(1)
    if idle and idle["flash"] > 2 + (hidle["flash"] if hidle else 0):  # (the flash fading as the shot's frame ends: home too)
        bad.append(f"muzzle flash shown {idle['flash']} frames while idle (lent; at home {hidle['flash'] if hidle else 0})")
    if l["churn"] > 3:
        bad.append(f"the game changed weapons {l['churn']} times in a row (lent)")
    return bad


def main():
    a = sys.argv[1:]
    proj_all = "--proj" in a
    a = [x for x in a if x != "--proj"]
    if a and a[0] == "--summary":
        lv = int(a[3]) if len(a) > 3 else None
        sm = summary(frames(a[1], a[2], lv), changes(a[1], lv))
        show(a[2], sm)
        bad = jumpy(sm, a[2])
        for b in bad:
            print("  " + b)
        return 1 if bad else 0
    if len(a) < 3:
        sys.exit(__doc__)
    hl = int(a[3]) if len(a) > 3 else None
    ll = int(a[4]) if len(a) > 4 else None
    h = summary(frames(a[0], a[2], hl), changes(a[0], hl))
    l = summary(frames(a[1], a[2], ll), changes(a[1], ll))
    show("home", h)
    show("lent", l)
    bad = compare(h, l)
    ph, pl = projectiles(a[0], hl), projectiles(a[1], ll)
    show_projectiles("home", ph)
    show_projectiles("lent", pl)
    # (the launchers' projectiles live for frames and are compared; a gun's bullets mostly go within the frame they
    # are made in, so whether any is seen at all is chance: shown, compared with --proj)
    if a[2].startswith(("ROCKET", "GRENADE LAUNCHER")) or proj_all:
        bad += compare_projectiles(ph, pl)
    print("differences:" if bad else "no differences")
    for b in bad:
        print("  " + b)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
