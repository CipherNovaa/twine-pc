#!/usr/bin/env python3
"""Every lent weapon at home against the same weapon lent, as played (hw renderer, smooth motion): a mission with it
first, then its home mission or one it's lent to (the same way in: carried, taken out at the room's start); idle, fire
held to a reload, put away and drawn (number key 1, then Q). Each pair compared by tools/weapon_trace.py.

Usage: tools/lent_matrix.py [WORKDIR [8 [NAME...]]]   (8: 8 MB of RAM; NAMEs: only those runs)
With TWINE_TRACE=proj too: Bond's projectiles (the launchers' rocket and round) compared as well (weapon_trace.py).
Runs at most 4 at once, headless, with SDL's dummy sound driver (tools/endless_check.py's Run)."""
import sys, os, subprocess, concurrent.futures as cf
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import endless_check as ec
ec.WORK = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ec.ROOT, "build", "lent_matrix")
ec.DISC = ec.disc(); ec.RAM8 = len(sys.argv) > 2 and sys.argv[2] == "8"
os.makedirs(ec.WORK, exist_ok=True)
W = [("KOFFLER", "3:-1", "2:4", "8:0"), ("INGALLS", "9:0", "8:-1", "2:4"), ("AR36", "3:-1", "8:-1", "2:4"),
     ("BELGIQUE", "5:-1", "10:-1", "8:-1"), ("FRINESI AUTO", "10:-1", "8:-1", "3:-1"), ("IAC", "10:4", "10:-1", "8:-1"),
     ("KAZAKOVICH", "9:2", "9:0", "8:-1"), ("FRINESI SHOTGUN", "2:5", "2:4", "8:-1"), ("ROCKET", "5:4", "5:-1", "2:4"),
     ("GRENADE LAUNCHER", "9:2", "9:0", "8:0"), ("FRAG", "8:3", "8:-1", "2:4"), ("MEYER", "6:0", "6:9", "2:4")]
def pad(t0):
    return ",".join(["%d:4000" % (t0 + 100), "%d:0" % (t0 + 400)])
base = {"TWINE_ENDLESS": 1, "TWINE_ENDLESS_SEED": 5, "TWINE_ENDLESS_TEST_GOD": 1, "TWINE_ENDLESS_TEST_ARM": 1,
        "TWINE_TRACE": "weapon,change,proj", "TWINE_RENDERER": "hw", "TWINE_SMOOTH": 1}
runs = []
def key(w):
    return w.split()[0].lower() + ("_" + w.split()[1].lower() if " " in w else "")
for w, first, home, host in W:
    fl = int(first.split(":")[0])
    for kind, nxt in (("home", home), ("lent", host)):
        runs.append(ec.Run("m_%s_%s" % (kind, key(w)), fl, 75, dict(base, TWINE_ENDLESS_ROOM=first, TWINE_ENDLESS_NEXT=nxt,
                       TWINE_ENDLESS_TEST_EXIT="200,5000", TWINE_ENDLESS_TEST_DRAW=w, TWINE_TEST_PAD=pad(500),
                       TWINE_TEST_WEAPON="1100:1,1250:1")))
only = sys.argv[3:] 
if only:
    runs = [r for r in runs if any(o in r.name for o in only)]
with cf.ThreadPoolExecutor(4) as ex:
    list(ex.map(lambda r: r.go(), runs))
for w, first, home, host in W:
    k = key(w)
    h, l = os.path.join(ec.WORK, "m_home_" + k, "port.log"), os.path.join(ec.WORK, "m_lent_" + k, "port.log")
    if not os.path.exists(l):
        continue
    r = subprocess.run(["python3", os.path.join(ec.ROOT, "tools", "weapon_trace.py"), h, l, w, home.split(":")[0],
                        host.split(":")[0]], capture_output=True, text=True)
    lent = [x for x in open(l, errors="replace") if "lent from" in x and w.split()[0] in x]
    print("==", w, home, "->", host, "lent" if lent else "NOT LENT", "|", r.stdout.split("differences:")[-1].strip()
          if "differences:" in r.stdout else "no differences")
