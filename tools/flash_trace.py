#!/usr/bin/env python3
"""The flash bang at home (Courier) and lent, compared from runs' logs (TWINE_TRACE=flash,sound; port/mods/trace.c and
port/mods/endless.c write the lines).

Usage: tools/flash_trace.py HOME_LOG LENT_LOG [LENT_LEVEL]   prints both and their differences; exit 1 if any
       (both runs with TWINE_ENDLESS_TEST_FLASH_VANILLA=1: in a run the flash is made twice as long)
       tools/flash_trace.py --long LOG [LEVEL]                 a run's flash: twice Courier's (white, blinded)
       tools/flash_trace.py --summary LOG [LEVEL]             one log's flashes and blinded guards

What is compared:
  - each flash: weapon state 15's frames, its counter, the screen fade (on, colour) and its prims (solid or
    see-through) frame by frame (a frame where another fade of the game's covers it, Bond hurt's, not compared); its
    sounds (as program, tone, note of Courier's bank: 0x34 as it starts, 0xD at its
    end), on which frame of the state;
  - each guard blinded (flagged 0x40000, then asked for Courier's blinded animation, known by its data's hash in any
    mission): the animations asked after it (by hash) and how long each lasts, in units of his rate (Courier's GOON1
    routine: blinded 29, standing 22 (its blend's one and 21), recovering 14: rate 2 or 6, the recovering one at rate
    2 whatever his (Courier's routine jumps there, op 0, which resets it)), root motion off but for the recovering
    one, one "my eyes!" (program 9 tone 2 of Courier's bank, or tone 1 of Flashpoint's) for each guard blinded at the
    flag's frame;
  - no "my eyes!" of the port's callouts.
Home guards running Courier's HEADGOON routine (blinded, then 0x31 standing: 29 and 19 units) are listed, not
compared: lent ones all run GOON1's.
"""
import re
import sys

BLIND, STAND, RECOVER, HEAD_STAND = "218CBAAB", "7A8606CB", "2EF0A4D2", "15B669F7"
NAMES = {BLIND: "blinded", STAND: "standing", RECOVER: "recovering", HEAD_STAND: "standing (HEADGOON's)"}
FLASH_RGB = ("FF FF FF", "E1 E1 FF", "C3 C3 FF", "-")  # (the flash's own fade colours)
GOON1 = [(BLIND, 29), (STAND, 22), (RECOVER, 14)]  # (units of his rate; the recovering one's of rate 2)
HEADGOON = [(BLIND, 29), (HEAD_STAND, 19)]


def parse(path, level=None):
    lines = open(path, errors="replace").read().splitlines()
    if level is None:  # (the log's mission with the most flash frames)
        lv = {}
        for l in lines:
            m = re.match(r"trace: L(\d+) f\d+ flash: weapon state 15", l)
            if m:
                lv[m.group(1)] = lv.get(m.group(1), 0) + 1
        level = int(max(lv, key=lv.get)) if lv else 0
    pre = "trace: L%d f" % level
    flashes, cur, sounds, guards, flags, callouts, backs, gone = [], None, [], {}, [], 0, [], []
    for l in lines:
        if 'calls out "my eyes' in l:
            callouts += 1
        if not l.startswith(pre):
            continue
        m = re.match(r"trace: L\d+ f(\d+) (.*)", l)
        f, rest = int(m.group(1)), m.group(2)
        m = re.match(r"flash: weapon state (\d+) counter (-?\d+), fade (on|off) (\w\w \w\w \w\w) for \d+, prims (\S+) (\S+)", rest)
        if m:
            st = int(m.group(1))
            if st == 15 and (cur is None or cur["end"] is not None):
                cur = {"start": f, "frames": [], "end": None, "sounds": []}
                flashes.append(cur)
            if cur is not None and (cur["end"] is None or f <= cur["end"] + 4):
                cur["frames"].append((f - cur["start"], st, int(m.group(2)), m.group(3),
                                      m.group(4) if m.group(3) == "on" else "-", m.group(5), m.group(6)))
                if st != 15 and cur["end"] is None:
                    cur["end"] = f
            continue
        m = re.match(r"sound: (?:lent \(level (\d+) program (\d+) tone (\d+) note (\d+)\)|(\d+) flags \w+ \(program (\d+) tone (\d+) note (\d+))", rest)
        if m:
            if m.group(1):
                key = (int(m.group(1)), int(m.group(2)), int(m.group(3)), int(m.group(4)))
            else:
                key = (level, int(m.group(6)), int(m.group(7)), int(m.group(8)))
            sounds.append((f, key))
            continue
        m = re.match(r"flash: #(\d+) gone", rest)
        if m:
            gone.append((int(m.group(1)), f))
            continue
        m = re.match(r"flash: #(\d+) back to his script after", rest)
        if m:
            backs.append((int(m.group(1)), f))
            continue
        m = re.match(r"flash: #(\d+) type (\d+) flagged 0x40000 \(distance (-?\d+), combat", rest)
        if m:
            flags.append((f, int(m.group(1)), int(m.group(2)), int(m.group(3))))
            continue
        m = re.match(r"flash: #(\d+) anim -?\d+ \(\d+ frames \w+\) -> (-?\d+) \((\d+) frames (\w+)\) rate (-?\d+) keep (\d+), combat state (-?\d+), script (\S+)", rest)
        if m:
            guards.setdefault(int(m.group(1)), []).append((f, m.group(4), int(m.group(5)), int(m.group(6)),
                                                           int(m.group(7)), m.group(8)))
    for fl in flashes:  # (the sounds of its first frame, the frame before the trace's first (the trace is at the
        # frame's top), and of its end)
        s0 = fl["start"] - 1
        fl["sounds"] = [(f - s0, k) for f, k in sounds if s0 <= f <= (fl["end"] or s0 + 40) and k[1] in (1, 4) and k[2] == 12]
    blinded, starts = [], {}
    for f, i, t, d in flags:  # (a flag his blinded animation follows at once)
        seq = [x for x in guards.get(i, []) if f <= x[0] <= f + 1200]
        if not seq or seq[0][1] != BLIND or seq[0][0] > f + 1:
            continue
        starts.setdefault(f, set()).add(i)  # (blinded again as he recovers: a "my eyes!" too)
        if any(b["guard"] == i and b["start"] <= seq[0][0] <= b["start"] + b["len"] for b in blinded):
            continue
        phases, start, rate = [], seq[0][0], seq[0][2]
        for k, x in enumerate(seq):  # (until his script is out of the routine: the mod's (lent), Courier's GOON1
            # 0x41D-0x47C or HEADGOON 0xF4-0x12E (home); or an animation not of it)
            at = int(x[5][1:], 16) if x[5].startswith("+") else None
            if x[1] not in NAMES or (x[5] != "mod" and not (at is not None and (0x41D <= at <= 0x47C or 0xF4 <= at <= 0x12E))):
                end = x[0]
                break
            if not phases or phases[-1][0] != x[1]:
                phases.append([x[1], x[0], x[3]])
        else:
            end = None
        if any(g == i and start < n <= (end if end is not None else n + 1) for g, n in gone):
            continue  # (taken out or down meanwhile: not a whole one)
        back = [g for g, n in backs if g == i and start < n <= (end or n)]
        if back:  # (lent: the frame his script had him again, by endless.c)
            end = min(n for g, n in backs if g == i and start < n) + 1
        if end is None:
            continue
        units = [((phases[k + 1][1] if k + 1 < len(phases) else end) - p[1]) for k, p in enumerate(phases)]
        voice = [k for s, k in sounds if f - 2 <= s <= f and k[1] == 9 and (k[0], k[2]) in ((2, 2), (6, 1))]
        blinded.append({"guard": i, "type": t, "dist": d, "start": start, "len": end - start, "rate": rate,
                        "phases": [(p[0], u / (2 if p[0] == RECOVER else rate) if rate else 0, p[2])
                                   for p, u in zip(phases, units)], "voice": voice, "flag": f})
    voices = {f: [k for s, k in sounds if f - 2 <= s <= f and k[1] == 9 and (k[0], k[2]) in ((2, 2), (6, 1))] for f in starts}
    return {"level": level, "flashes": flashes, "blinded": blinded, "callouts": callouts, "starts": starts,
            "voices": voices}


def show(name, r):
    print("%s (level %d): %d flashes, %d guards blinded, %d \"my eyes!\" callouts of the port's" %
          (name, r["level"], len(r["flashes"]), len(r["blinded"]), r["callouts"]))
    for fl in r["flashes"][:2]:
        print("  flash at f%d: %s; sounds %s" % (fl["start"], " ".join("%d:%d/%d/%s/%s/%s" % (x[0], x[1], x[2], x[3][:2], x[4].replace(" ", ""), x[5][:5])
                                                                      for x in fl["frames"]), fl["sounds"]))
    for b in r["blinded"]:
        print("  guard #%d type %d at %d, f%d: rate %d, %d frames: %s; voice %s" % (
            b["guard"], b["type"], b["dist"], b["start"], b["rate"], b["len"],
            ", ".join("%s %g%s" % (NAMES[p[0]], p[1], "" if p[2] else " (root motion)") for p in b["phases"]), b["voice"]))


def fade_frames(fl):
    """a flash's frames with its own fade on: (solid, see-through)"""
    on = [x for x in fl["frames"] if x[3] == "on" and x[4].replace(" ", "") in ("FFFFFF", "E1E1FF", "C3C3FF")]
    return sum(1 for x in on if x[5] == "solid"), sum(1 for x in on if x[5] != "solid")


def long_problems(r, solid=8, lengths=(260, 668)):
    """a run's (not TWINE_ENDLESS_TEST_FLASH_VANILLA) flash: the white twice Courier's (8 frames solid, his 4), the
    blinded guards twice as long (260 or 668 frames, his 130 or 334), a "my eyes!" each"""
    p = []
    for fl in r["flashes"]:
        if fl["end"] is not None and fade_frames(fl)[0] != solid:
            p.append("flash f%d: %d frames solid white (%d wanted)" % (fl["start"], fade_frames(fl)[0], solid))
    for b in r["blinded"]:
        if b["len"] not in lengths:
            p.append("guard #%d f%d: blinded %d frames (%s wanted)" % (b["guard"], b["start"], b["len"], lengths))
    for f in sorted(r["starts"]):
        if len(r["voices"][f]) != len(r["starts"][f]):
            p.append("f%d: %d guards blinded, %d \"my eyes!\"" % (f, len(r["starts"][f]), len(r["voices"][f])))
    return p


def kind(b):
    ph = [(p[0], p[1]) for p in b["phases"]]
    return "GOON1" if ph == GOON1 else "HEADGOON" if ph == HEADGOON else "other"


def compare(h, l):
    diffs = []
    fh = [tuple(x[1:]) for x in h["flashes"][0]["frames"]] if h["flashes"] else None
    if not h["flashes"] or not l["flashes"]:
        diffs.append("no flash: home %d, lent %d" % (len(h["flashes"]), len(l["flashes"])))
    for k, fl in enumerate(l["flashes"]):
        if fl["end"] is None or (fh is not None and len(fl["frames"]) < len(fh)):
            continue  # (cut short by the run's end)
        own = lambda fr: [x if x[3] in FLASH_RGB or x[2] == "off" else None for x in fr]  # (another fade of the
        # game's over it: Bond hurt, D2 8C 46, ...: that frame not compared)
        a, b = own([tuple(x[1:]) for x in fl["frames"]]), own(fh) if fh is not None else None
        if b is not None and any(x is not None and y is not None and x != y for x, y in zip(a, b)):
            diffs.append("lent flash %d (f%d): state/counter/fade/prims %s, home %s" % (k, fl["start"], fl["frames"], h["flashes"][0]["frames"]))
        snd = [(t, k2[1:]) for t, k2 in fl["sounds"]]
        hs = [(t, k2[1:]) for t, k2 in h["flashes"][0]["sounds"]] if h["flashes"] else None
        if hs is not None and snd != hs:
            diffs.append("lent flash %d (f%d): sounds %s, home %s" % (k, fl["start"], fl["sounds"], h["flashes"][0]["sounds"]))
        if [k2[0] for t, k2 in fl["sounds"]] and set(k2[0] for t, k2 in fl["sounds"]) != {2}:
            diffs.append("lent flash %d: sounds not Courier's bank: %s" % (k, fl["sounds"]))
    hk = [kind(b) for b in h["blinded"]]
    if "GOON1" not in hk:
        diffs.append("no home guard blinded by Courier's GOON1 routine to compare with (%s)" % hk)
    if not l["blinded"]:
        diffs.append("no lent guard blinded")
    for b in l["blinded"]:
        if kind(b) != "GOON1":
            diffs.append("lent guard #%d: %s, not GOON1's %s" % (b["guard"], b["phases"], GOON1))
        if b["rate"] not in (2, 6):
            diffs.append("lent guard #%d: rate %d" % (b["guard"], b["rate"]))
        if [p[2] for p in b["phases"]] != [1, 1, 0]:
            diffs.append("lent guard #%d: root motion off %s, home GOON1's [1, 1, 0]" % (b["guard"], [p[2] for p in b["phases"]]))
    for name, r in (("home", h), ("lent", l)):  # (a "my eyes!" for each guard blinded at a flag's frame)
        for f in sorted(r["starts"]):
            n, v = len(r["starts"][f]), r["voices"][f]
            if len(v) != n:
                diffs.append("%s f%d: %d guards blinded, %d \"my eyes!\" (%s)" % (name, f, n, len(v), v))
    if l["callouts"] or h["callouts"]:
        diffs.append("the port's \"my eyes!\" callouts: home %d, lent %d" % (h["callouts"], l["callouts"]))
    return diffs


def main():
    a = sys.argv[1:]
    if a and a[0] == "--long":  # (a run's flash in a log: tools/flash_trace.py --long LOG LEVEL)
        r = parse(a[1], int(a[2]) if len(a) > 2 else None)
        show(a[1], r)
        p = long_problems(r)
        print("\n".join(p) if p else "twice as long: as wanted")
        return 1 if p else 0
    if a and a[0] == "--summary":
        show(a[1], parse(a[1], int(a[2]) if len(a) > 2 else None))
        return 0
    if len(a) < 2:
        sys.exit(__doc__)
    h, l = parse(a[0], 2), parse(a[1], int(a[2]) if len(a) > 2 else None)
    show("home", h)
    show("lent", l)
    d = compare(h, l)
    print("differences:" if d else "no differences")
    for x in d:
        print("  " + x)
    return 1 if d else 0


if __name__ == "__main__":
    sys.exit(main())
