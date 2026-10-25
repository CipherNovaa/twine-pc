#!/usr/bin/env python3
"""Dump a level's sound bank (SOUNDFX/LnFX.VH + .VB in Lnn/LVLnnCD.BIN) as WAV files, one per sample, to pick sounds
out by ear (endless mode's guard shouts: port/mods/endless.c shout_sound[]).

Usage: vab_dump.py LEVEL [OUTDIR]
  LEVEL   1-11; the archive is read from workbench/extracted/Lnn/LVLnnCD.BIN
  OUTDIR  default workbench/research/sounds/Lnn/ (gitignored: the samples are the game's)

Each file is named by the sample's number in the bank (as a program's tone points at it) and the first program and
tone using it: v<sample>_p<program>_t<tone>.wav, decoded from the PS1's ADPCM at 11025 Hz (the rate the voices were
made at, as heard: the guard's "grenade!" is right there; the game plays a sound at its table entry's note). The sound numbers scripts play (op 0x1D) are program numbers here."""
import struct
import sys
import wave
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def files(level):
    arc = (ROOT / "workbench" / "extracted" / ("L%02d" % level) / ("LVL%02dCD.BIN" % level)).read_bytes()
    out, at = {}, 0
    while at + 0x1C <= len(arc) and arc[at]:
        name = arc[at:at + 26].split(b"\0")[0].decode(errors="replace")
        out[name] = struct.unpack_from("<H", arc, at + 26)[0] * 0x800
        at += 0x1C
    return arc, out


def decode(vb):
    pos, neg = (0, 60, 115, 98, 122), (0, 0, -52, -55, -60)
    s1 = s2 = 0
    pcm = []
    for b in range(0, len(vb) - 15, 16):
        shift, filt = vb[b] & 15, min(vb[b] >> 4, 4)
        for k in range(28):
            n = (vb[b + 2 + k // 2] >> ((k & 1) * 4)) & 15
            smp = ((n << 12) & 0xFFFF)
            smp = (smp - 0x10000 if smp & 0x8000 else smp) >> shift
            smp += (s1 * pos[filt] + s2 * neg[filt] + 32) >> 6
            smp = max(-32768, min(32767, smp))
            s2, s1 = s1, smp
            pcm.append(smp)
        if vb[b + 1] & 1 and b > 0:
            break
    return pcm


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    level = int(sys.argv[1])
    outdir = Path(sys.argv[2]) if len(sys.argv) > 2 else ROOT / "workbench" / "research" / "sounds" / ("L%02d" % level)
    arc, names = files(level)
    vh_name = next((n for n in names if n.startswith("SOUNDFX/") and n.endswith(".VH")), None)
    if not vh_name:
        sys.exit("no sound bank in level %d" % level)
    vh = arc[names[vh_name]:]
    vb = arc[names[vh_name[:-3] + ".VB"]:]
    if vh[:4] != b"pBAV":
        sys.exit("not a VAB header")
    programs = struct.unpack_from("<H", vh, 0x12)[0]
    tones = 0x20 + 0x800
    sizes = struct.unpack_from("<256H", vh, tones + programs * 16 * 32)
    first = {}
    for p in range(programs):
        for t in range(16):
            vag = struct.unpack_from("<H", vh, tones + (p * 16 + t) * 32 + 0x16)[0]
            if vag and vag not in first:
                first[vag] = (p, t)
    outdir.mkdir(parents=True, exist_ok=True)
    off = 0
    for v in range(1, 256):
        size = sizes[v] * 8
        if not size:
            continue
        if v in first:
            pcm = decode(vb[off:off + size])
            p, t = first[v]
            with wave.open(str(outdir / ("v%03d_p%03d_t%02d.wav" % (v, p, t))), "wb") as w:
                w.setnchannels(1)
                w.setsampwidth(2)
                w.setframerate(11025)
                w.writeframes(struct.pack("<%dh" % len(pcm), *pcm))
        off += size
    print("%s: %d samples to %s" % (vh_name, len(first), outdir))


if __name__ == "__main__":
    main()
