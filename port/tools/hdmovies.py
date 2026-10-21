#!/usr/bin/env python3
"""Cuts the mission movies' film clips from the user's own copy of the film, for the port to show in place of the
PS1's (port/src/hdmovie.c): hdmovies.py FILM OUTDIR [NAME...] writes OUTDIR/<NAME>.mpg (MPEG-1 1920x816, MP2
stereo), then `hd_movies = OUTDIR` in twine.ini. An HDR film is tone mapped to SDR.

The ranges were found by matching every movie on the disc against the film (the 2160p release, 2:08:00 long);
they start a second early and end a little late, and a movie the PS1 cut together from parts far apart plays
those parts in order. Movies with no film in them (logos, the briefings) keep the PS1's own."""
import subprocess, sys, os

RANGES = {
    'LVL01IN': '0:11-0:45', 'LVL01OUT': '3:58-4:28',
    'LVL02IN': '7:16-8:15 9:29-10:05 12:28-13:05', 'LVL02OUT': '13:22-14:16',
    'LVL03IN': '34:13-36:09', 'LVL03OUT': '39:09-40:19',
    'LVL04IN': '42:23-42:38', 'LVL04OUT': '46:41-48:29',
    'LVL05IN': '51:51-52:28', 'LVL05OUT': '54:16-54:24 55:13-55:40',
    'LVL06IN': '56:31-56:39 59:31-59:49', 'LVL06OUT': '67:04-68:33',
    'LVL07IN': '72:50-73:36', 'LVL07OUT': '76:22-76:52 77:59-78:35',
    'LVL08IN': '87:27-87:40 88:33-89:41', 'LVL08OUT': '94:30-95:47',
    'LVL09IN': '99:48-100:30', 'LVL09OUT': '100:52-100:58',
    'LVL10IN': '103:04-104:16 107:05-108:33', 'LVL10OUT': '110:34-110:50',
    'LVL11IN': '110:52-111:08', 'LVL11OUT': '121:59-123:14',
}

def secs(t):
    m, s = t.split(':')
    return int(m) * 60 + float(s)

def hdr(film):
    out = subprocess.run(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries',
                          'stream=color_transfer', '-of', 'csv=p=0', film], capture_output=True, text=True).stdout
    return out.strip() in ('smpte2084', 'arib-std-b67')

def cut(film, out, name, tonemap):
    parts = [tuple(map(secs, r.split('-'))) for r in RANGES[name].split()]
    args = ['ffmpeg', '-y', '-v', 'error']
    for a, b in parts:
        args += ['-ss', str(a), '-to', str(b), '-i', film]
    total = sum(b - a for a, b in parts)
    video = 'scale=1920:816:flags=bicubic'
    if tonemap:
        video += (',zscale=t=linear:npl=100,format=gbrpf32le,zscale=p=bt709,tonemap=hable:desat=0,'
                  'zscale=t=bt709:m=bt709:r=tv')
    video += ',format=yuv420p,fade=in:d=0.5,fade=out:st=%.2f:d=0.7' % (total - 0.7)
    inputs = ''.join('[%d:v:0][%d:a:0]' % (i, i) for i in range(len(parts)))
    graph = '%sconcat=n=%d:v=1:a=1[v][a];[v]%s[vo];[a]afade=in:d=0.5,afade=out:st=%.2f:d=0.7[ao]' % (
        inputs, len(parts), video, total - 0.7)
    args += ['-filter_complex', graph, '-map', '[vo]', '-map', '[ao]',
             '-c:v', 'mpeg1video', '-b:v', '24M', '-maxrate', '30M', '-bufsize', '8M', '-q:v', '2', '-bf', '0',
             '-c:a', 'mp2', '-b:a', '384k', '-ar', '44100', '-ac', '2', '-f', 'mpeg', os.path.join(out, name + '.mpg')]
    print(name, '%.0f s' % total, flush=True)
    subprocess.run(args, check=True)

def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    film, out = sys.argv[1], sys.argv[2]
    os.makedirs(out, exist_ok=True)
    tonemap = hdr(film)
    for name in sys.argv[3:] or RANGES:
        cut(film, out, name, tonemap)

main()
