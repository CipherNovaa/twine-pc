#!/usr/bin/env python3
"""Cuts the mission movies' film clips from the user's own copy of the film, for the port to show in place of the
PS1's (port/src/hdmovie.c): hdmovies.py FILM OUTDIR [NAME...] writes OUTDIR/<NAME>.mpg (MPEG-1 1920x816, MP2
stereo), then `hd_movies = OUTDIR` in twine.ini. An HDR film is tone mapped to SDR.

The ranges were found by matching every movie on the disc against the film (the 2160p release, 2:08:00 long);
they start a second early and end a little late, and a movie the PS1 cut together from parts far apart plays
those parts in order. Movies with no film in them (logos, the briefings) keep the PS1's own."""
import json, subprocess, sys, os

RANGES = {
    'LVL01IN': '0:11-0:45', 'LVL01OUT': '3:58-4:32',
    'LVL02IN': '7:16-13:05', 'LVL02OUT': '13:22-14:16',
    'LVL03IN': '34:13-36:09', 'LVL03OUT': '39:09-40:19',
    'LVL04IN': '42:23-42:38', 'LVL04OUT': '~46:20-~48:34',
    'LVL05IN': '52:32-52:45', 'LVL05OUT': '54:16-54:24 55:13-55:40',
    'LVL06IN': '~57:07-~57:11 ~59:31-59:49', 'LVL06OUT': '67:04-68:33',
    'LVL07IN': '~72:54-~73:41', 'LVL07OUT': '76:22-76:52 77:59-78:35',
    'LVL08IN': '~87:38-~88:07 ~88:29-~89:38', 'LVL08OUT': '~94:22-96:26.3',
    'LVL09IN': '99:48-~100:32', 'LVL09OUT': '100:52-100:58',
    'LVL10IN': '~102:38-~102:45 ~103:28-~103:56 ~106:37.5-~108:10', 'LVL10OUT': '110:34-110:50',
    'LVL11IN': '~110:12-~111:13', 'LVL11OUT': '121:59-~122:34 ~123:14-~124:01 ~124:16-124:21',
}

# a longer fade at the end, where a clip stops mid-scene
FADE_OUT = {'LVL05IN': 1.5, 'LVL08OUT': 1.2, 'LVL10IN': 1.0}

# the parts of a clip join with a dissolve this long (picture and sound), made of each part's own frames
JOIN = 0.5

# how loud the clips are made (EBU R128 integrated, LUFS): the PS1's movies are about -12 to -15
LOUDNESS = -14.0

def secs(t):
    m, s = t.split(':')
    return int(m) * 60 + float(s)

def snap(film, t):
    """The film's shot change nearest after t - 0.4 s (within 1.6 s): the time its new shot's first frame shows."""
    a = t - 0.4
    raw = subprocess.run(['ffmpeg', '-v', 'error', '-ss', str(a), '-t', '1.6', '-i', film, '-an', '-vf',
                          'scale=128:56:flags=area,format=gray', '-f', 'rawvideo', '-'], capture_output=True).stdout
    n = len(raw) // (128 * 56)
    frames = [raw[i * 7168:(i + 1) * 7168] for i in range(n)]
    diff = [sum(abs(x - y) for x, y in zip(frames[i], frames[i + 1])) for i in range(n - 1)]
    best = max(range(n - 1), key=diff.__getitem__)
    return a + (best + 1) * 1001 / 24000

def when(film, t):
    return snap(film, secs(t[1:])) if t.startswith('~') else secs(t)

def hdr(film):
    out = subprocess.run(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries',
                          'stream=color_transfer', '-of', 'csv=p=0', film], capture_output=True, text=True).stdout
    return out.strip() in ('smpte2084', 'arib-std-b67')

def cut(film, out, name, tonemap):
    parts = [tuple(when(film, t) for t in r.split('-')) for r in RANGES[name].split()]
    fade = FADE_OUT.get(name, 0.7)
    args = ['ffmpeg', '-y', '-v', 'error']
    for a, b in parts:
        args += ['-ss', str(a), '-to', str(b), '-i', film]
    total = sum(b - a for a, b in parts) - JOIN * (len(parts) - 1)
    video = 'scale=1920:816:flags=bicubic'
    if tonemap:
        video += (',zscale=t=linear:npl=100,format=gbrpf32le,zscale=p=bt709,tonemap=hable:desat=0,'
                  'zscale=t=bt709:m=bt709:r=tv')
    video += ',format=yuv420p,fade=in:d=0.5,fade=out:st=%.2f:d=%.1f' % (total - fade, fade)
    graph, v, a, at = '', '[0:v:0]', '[0:a:0]', parts[0][1] - parts[0][0]
    for i in range(1, len(parts)):
        graph += '%s[%d:v:0]xfade=transition=fade:duration=%.2f:offset=%.3f[v%d];' % (v, i, JOIN, at - JOIN, i)
        graph += '%s[%d:a:0]acrossfade=d=%.2f[a%d];' % (a, i, JOIN, i)
        v, a, at = '[v%d]' % i, '[a%d]' % i, at - JOIN + parts[i][1] - parts[i][0]
    graph += '%s%s[vo];%safade=in:d=0.5,afade=out:st=%.2f:d=%.1f,%s[ao]' % (
        v, video, a, total - fade, fade, level(args, len(parts)))
    args += ['-filter_complex', graph, '-map', '[vo]', '-map', '[ao]',
             '-c:v', 'mpeg1video', '-b:v', '24M', '-maxrate', '30M', '-bufsize', '8M', '-q:v', '2', '-bf', '0',
             '-c:a', 'mp2', '-b:a', '384k', '-ar', '44100', '-ac', '2', '-f', 'mpeg', os.path.join(out, name + '.mpg')]
    print(name, '%.0f s' % total, ' '.join('%d:%06.3f-%d:%06.3f' % (a // 60, a % 60, b // 60, b % 60)
                                              for a, b in parts), flush=True)
    subprocess.run(args, check=True)

def level(inputs, n):
    """loudnorm's measuring pass over the clip's sound, as the filter that brings it to LOUDNESS (linearly)"""
    graph = ''.join('[%d:a:0]' % i for i in range(n)) + 'concat=n=%d:v=0:a=1,aformat=channel_layouts=stereo,' % n
    norm = 'loudnorm=I=%.1f:TP=-1:LRA=20' % LOUDNESS
    err = subprocess.run(inputs + ['-v', 'info', '-filter_complex', graph + norm + ':print_format=json', '-f', 'null', '-'],
                         capture_output=True, text=True).stderr
    m = json.loads(err[err.rindex('{'):err.rindex('}') + 1])
    return 'aformat=channel_layouts=stereo,%s:measured_I=%s:measured_TP=%s:' \
           'measured_LRA=%s:measured_thresh=%s:offset=%s:linear=true,aresample=44100' % (
               norm, m['input_i'], m['input_tp'], m['input_lra'], m['input_thresh'], m['target_offset'])

def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    film, out = sys.argv[1], sys.argv[2]
    os.makedirs(out, exist_ok=True)
    tonemap = hdr(film)
    for name in sys.argv[3:] or RANGES:
        cut(film, out, name, tonemap)

main()
