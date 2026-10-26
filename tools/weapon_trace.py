#!/usr/bin/env python3
"""A weapon at home against the same weapon lent, from TWINE_TRACE=weapon,change logs (port/mods/trace.c).

Usage: weapon_trace.py HOME_LOG LENT_LOG NAME [HOME_LEVEL LENT_LEVEL]
       weapon_trace.py --summary LOG NAME [LEVEL]

For the weapon whose name starts with NAME, in each log (only frames in LEVEL when given), it gathers:
  - each state the weapon went through (D_800F11F0), with the animation each state asks for and how long that
    animation runs (its frames left when it starts) and how far its playhead moves a frame (the first track's
    position, +0x16: the animation's speed as seen),
  - the muzzle flash: frames it is shown in each state (a flash shown while idle is a stuck flash),
  - the weapon changes (who asked), and how many changes the game made by itself in a row (constant switching).
Then it prints both and the differences. Exit code 0 when the lent weapon behaves as at home (same states, the
same animation in each, same lengths and speeds, no flash shown outside firing, no change churn), else 1.

Run it on headless runs with the same pad script in the same room type (tools/endless_check.py check k does)."""
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
    st = defaultdict(lambda: {"anims": Counter(), "lengths": Counter(), "steps": Counter(), "flash": 0, "frames": 0})
    prev = None
    for x in fr:
        s = st[x["state"]]
        s["frames"] += 1
        if x["asked"] >= 0:
            s["anims"][x["asked"]] += 1
        if x["flash"] == "SHOWN":
            s["flash"] += 1
        if prev and prev["f"] + 1 == x["f"] and prev["anim"] == x["anim"] and x["anim"] >= 0:
            if prev["left"] == 0 and x["left"] > 0 or x["left"] > prev["left"]:  # (an animation (re)starts)
                s["lengths"][x["left"]] += 1
            elif x["at"] >= prev["at"]:
                s["steps"][(x["rate"], x["at"] - prev["at"])] += 1
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


def show(title, s):
    print(f"{title}: {s['frames']} frames in hand, models (slot's, loaded) {dict(s['models'])}, files while idle "
          f"{dict(s['files'])}, change churn {s['churn']}")
    for k in sorted(s["states"]):
        v = s["states"][k]
        print(f"  state {k:3d}: {v['frames']:4d} frames, anims {dict(v['anims'])}, lengths {dict(v['lengths'])}, "
              f"steps {dict(v['steps'])}, flash shown {v['flash']}")


def common(c):
    return c.most_common(1)[0][0] if c else None


def compare(h, l):
    bad = []
    for k in sorted(set(h["states"]) | set(l["states"])):
        a, b = h["states"].get(k), l["states"].get(k)
        if not a or not b:
            if (a or b)["frames"] > 2:
                bad.append(f"state {k} only {'at home' if a else 'lent'}")
            continue
        if common(a["anims"]) != common(b["anims"]):
            bad.append(f"state {k}: animation {common(a['anims'])} at home, {common(b['anims'])} lent")
        if a["lengths"] and b["lengths"] and common(a["lengths"]) != common(b["lengths"]):
            bad.append(f"state {k}: lasts {common(a['lengths'])} frames at home, {common(b['lengths'])} lent")
        for rate in sorted({r for r, _ in a["steps"]} & {r for r, _ in b["steps"]}):  # (the game's rate: frames)
            sa = Counter({st: n for (r, st), n in a["steps"].items() if r == rate})
            sb = Counter({st: n for (r, st), n in b["steps"].items() if r == rate})
            if sum(sa.values()) >= 4 and sum(sb.values()) >= 4 and common(sa) != common(sb):
                bad.append(f"state {k} at rate {rate}: playhead {common(sa)} a frame at home, {common(sb)} lent")
    hf, lf = common(h["files"]), common(l["files"])
    if hf and lf and hf.split("/")[-1] != lf.split("/")[-1]:
        bad.append(f"model file {hf} at home, {lf} lent")
    if len(l["files"]) > 1:
        bad.append(f"more than one model file while idle (lent): {dict(l['files'])}")
    idle = l["states"].get(1)
    if idle and idle["flash"] > 2:
        bad.append(f"muzzle flash shown {idle['flash']} frames while idle (lent)")
    if l["churn"] > 3:
        bad.append(f"the game changed weapons {l['churn']} times in a row (lent)")
    return bad


def main():
    a = sys.argv[1:]
    if a and a[0] == "--summary":
        lv = int(a[3]) if len(a) > 3 else None
        show(a[2], summary(frames(a[1], a[2], lv), changes(a[1], lv)))
        return 0
    if len(a) < 3:
        sys.exit(__doc__)
    hl = int(a[3]) if len(a) > 3 else None
    ll = int(a[4]) if len(a) > 4 else None
    h = summary(frames(a[0], a[2], hl), changes(a[0], hl))
    l = summary(frames(a[1], a[2], ll), changes(a[1], ll))
    show("home", h)
    show("lent", l)
    bad = compare(h, l)
    print("differences:" if bad else "no differences")
    for b in bad:
        print("  " + b)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
