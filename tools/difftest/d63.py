import re,sys,subprocess,random,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from mips import Sim
asm='asm/level01/nonmatchings/text_8002C808/func_800463C8.s'; cand=sys.argv[1]; NT=int(sys.argv[2])
BASE=0x20000000; SIZE=0x400000
STACK=0x203B0000; RETTAB=0x203E0000; LOG=0x203F0000
SYMB=0x20100000
src=open(cand).read()
names=sorted(set(re.findall(r'\b(?:D|func|jtbl)_80[0-9A-F]{6}\b',open(asm).read()+src))|{'g_Args','g_Player','g_Weapons','g_CutScene'})
sym={}
for i,n in enumerate(names): sym[n]=SYMB+i*0x4000
OT=0x20300000; ARGS=0x20310000; PLAYER=0x20320000; CB=0x20330000
sym['fakecb']=CB
nargs={'func_80029A34':0,'func_80044BCC':1,'func_80045A28':1,'func_80047318':2,'func_8004A9E4':2,'func_8004783C':0,'Menu_ResetState':0,'func_8002E190':0,'func_80047B50':1,'func_8004626C':1,'func_8004B084':0,'func_8004B06C':0,'func_800475BC':0,'func_800324C0':0,'func_80028858':1,'func_80028810':2,'func_80045084':0,'fakecb':1}
# Menu_ResetState name in asm?
asmnames=set(re.findall(r'jal\s+(\w+)',open(asm).read()))
ids={n:i+1 for i,n in enumerate(sorted(set(nargs)|asmnames))}
for n in asmnames:
    if n not in sym: sym[n]=SYMB+len(sym)*0x4000
    nargs.setdefault(n,0)
fns=[n for n in sorted(asmnames)]
def host_src():
    s=src.replace('#include "twine.h"','')
    h='typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef short s16;typedef unsigned int u32;typedef int s32;\n'
    h+='#include <stdio.h>\n#include <string.h>\n#include <stdlib.h>\n#include <sys/mman.h>\n#define NULL 0\ntypedef s32 M2C_UNK;\n'
    h+='typedef struct { u32 flags; u32 flags2; u8 pad[0x10D]; u8 unk115; } Args;\ntypedef struct { u8 pad0[0x38]; s32 flags; u8 rest[0x7C]; } Weapon;\n'
    h+='#define g_Args (*(Args**)0x%X)\n#define g_Player (*(void**)0x%X)\n#define g_Weapons ((Weapon*)0x%X)\n'%(sym['g_Args'],sym['g_Player'],sym['g_Weapons'])
    out=[]
    for l in s.split('\n'):
        m=re.match(r'extern\s+(.+?)\s*(\(\*)?\s*([\w]+)\)?(\[[^\]]*\])*(\(u32\))?\s*;',l)
        m1=re.match(r'extern M2C_UNK \(\*(D_\w+)\)\(u32\);',l)
        if m1:
            h+='#define %s (*(u32(**)(u32))0x%X)\n'%(m1.group(1),sym[m1.group(1)]); continue
        m=re.match(r'extern\s+([\w ]+?)\s+(\w+)((?:\[[^\]]*\])*)\s*;',l)
        if m:
            T,n,dims=m.group(1),m.group(2),m.group(3)
            if n not in sym: out.append(l); continue
            if dims:
                dl=re.findall(r'\[([^\]]*)\]',dims)
                if all(d=='' for d in dl) and len(dl)==1: h+='#define %s ((%s*)0x%X)\n'%(n,T,sym[n])
                else:
                    first=dl[0]; rest=''.join('[%s]'%d for d in dl[1:])
                    h+='#define %s ((%s(*)%s)0x%X)\n'%(n,T,rest,sym[n]) if rest else '#define %s ((%s*)0x%X)\n'%(n,T,sym[n])
            else: h+='#define %s (*(%s*)0x%X)\n'%(n,T,sym[n])
            continue
        out.append(l)
    body='\n'.join(out)
    h+='u32 *LOGP=(u32*)0x%X; u32 *RT=(u32*)0x%X; int NC=0;\n'%(LOG,RETTAB)
    h+='static u32 logcall(int id,u32 a,u32 b,u32 c,u32 d){ LOGP[NC*5]=id;LOGP[NC*5+1]=a;LOGP[NC*5+2]=b;LOGP[NC*5+3]=c;LOGP[NC*5+4]=d; return RT[NC++]; }\n'
    return h,body
h,body=host_src()
stubs=''
for n in fns:
    if n=='func_80028858':
        stubs+='void func_80028858(char*t,s16*x,s16*w){u32 r=logcall(%d,(u32)t,0,0,0); *x=r&0xFFFF; *w=r>>16;}\n'%ids[n]
    else:
        k=nargs[n]; params=','.join('u32 a%d'%i for i in range(k)) or 'void'
        call=','.join(['a%d'%i if i<k else '0' for i in range(4)])
        ret='s32' 
        stubs+='%s %s(%s){return logcall(%d,%s);}\n'%(ret,n,params,ids[n],call)
stubs+='u32 fakecb(u32 a0){return logcall(%d,a0,0,0,0);}\n'%ids['fakecb']
# the candidate prototypes may conflict with stub return types -> put stubs before and drop prototypes
body2='\n'.join(l for l in body.split('\n') if not re.match(r'^(void|s32) \w+\(.*\);$',l))
main='''
int main(int argc,char**argv){
  void*p=mmap((void*)0x%X,0x%X,PROT_READ|PROT_WRITE,MAP_FIXED|MAP_ANONYMOUS|MAP_PRIVATE|0x40,-1,0);
  if(p==MAP_FAILED){perror("mmap");return 1;}
  FILE*f=fopen(argv[1],"rb"); fread(p,1,0x%X,f); fclose(f);
  { u32 *slot=(u32*)0x%X; u32 old=*slot; *slot=old?(u32)fakecb:0; func_800463C8(); *slot=old; }
  fwrite(p,1,0x%X,stdout); return 0; }
'''%(BASE,SIZE,SIZE,sym['D_800D5F48'],SIZE)
open(cand+'.host.c','w').write(h+stubs+body2+main)
r=subprocess.run(['gcc','-m32','-w','-fpermissive','-Wno-error=int-conversion','-O0','-o',cand+'.host',cand+'.host.c'],capture_output=True,text=True)
if r.returncode: print(r.stderr[:3000]); sys.exit(1)
rev={v:k for k,v in sym.items()}
tot_calls=set(); nbad=0
for trial in range(NT):
    rng=random.Random(1000+trial)
    mem=bytearray(SIZE)
    for k in range(len(sym)):
        o=SYMB-BASE+k*0x4000
        mem[o:o+0x400]=bytes(rng.choice([0,0,0,rng.randrange(256)]) for _ in range(0x400))
    def w(a,v,n):
        for i in range(n): mem[a-BASE+i]=(v>>(8*i))&0xff
    for a in range(OT-BASE,OT-BASE+0x100): mem[a]=rng.randrange(256)
    for i in range(64): w(RETTAB+i*4,rng.choice([0,0,1,0x10000,5,0x30002,rng.randrange(1<<32)]),4)
    # control globals small values
    small=[n for n in names if n.startswith('D_')]
    for n in small:
        v=rng.choice([0,0,1,2,3,4,5])
        w(sym[n],v,4)
    # restore some specifics
    w(sym['D_800B8F24'],rng.choice([0,1]),4)
    for b in (0,1): w(sym['D_800B8EFC']+b*20+4,OT,4)
    w(sym['g_Args'],ARGS,4); w(sym['g_Player'],PLAYER,4)
    for a in range(0,0x200): mem[ARGS-BASE+a]=rng.choice([0,0,1,rng.randrange(256)])
    for a in range(0,0x80): mem[PLAYER-BASE+a]=rng.choice([0,0,1,2,rng.randrange(256)])
    w(sym['D_800D5F48'],rng.choice([0,CB]),4)
    w(sym['D_800D5F40'],rng.choice([0,1,2,-1&0xffffffff]),4); w(sym['D_800D5F40']+4,rng.choice([0,3,100,rng.randrange(1<<32)]),4)
    w(sym['D_8009C180'],rng.choice([0,3,50,rng.randrange(1<<32)]),4)
    w(sym['D_8009CDA8'],rng.choice([0,3,100]),4); w(sym['D_800D79D0'],rng.choice([0,3,100]),4); w(sym['D_8009D00C'],rng.choice([0,3,100]),4)
    w(sym['D_800B0B9C'],rng.choice([0,0,1,4,5,6,-1&0xffffffff]),4)
    w(sym['D_8009EED0'],rng.choice([0,1,5]),4)
    w(sym['D_8009D020'],rng.choice([0,1,2,3]),4)
    w(sym['D_800D7DF8'],rng.choice([0,1,2]),1)
    w(sym['D_800D811C'],rng.choice([0,1,2,3,4,5,-1&0xffffffff]),4)
    w(sym['D_800D7DFC']+0x328,rng.choice([0,1,4,2]),4)
    w(sym['D_800D7DFC']+0x32C,rng.choice([0,1,2]),1)
    w(sym['D_800D7DFC']+0x324,rng.choice([0,1,2]),4); w(sym['D_800D7DFC']+0x320,rng.choice([0,1,2]),4)
    w(sym['D_8009CFF6'],rng.choice([0,1,2]),2)
    for k in range(6): w(sym['g_Weapons']+k*0xB8+0x38, rng.choice([0,rng.randrange(1<<32),0xFFFFFFFF,1<<22,1<<23,1<<27]),4)
    for n in ['D_800B0DD4','D_800B0DDC']: w(sym[n],rng.choice([0,1,2,5]),2)
    for n in ['D_8009D018','D_8009D01A','D_8009CAA4','D_800D7BE8','D_800D7BF0']: w(sym[n],rng.choice([0,0,1,rng.randrange(65536)]),2)
    if trial%2:
        for n in ['D_8009CD00','D_8009CABF','D_8009C1F0','D_8009CAA4','g_CutScene','D_800AC38C','D_8009D018','D_800B0B9C','D_8009CA90','D_800D74F8','D_800D8150','D_8009CD4C']:
            w(sym[n],0,4)
        w(sym['D_8009EED0'],rng.choice([1,5]),4)
        for k in range(6): w(sym['g_Weapons']+k*0xB8+0x38, rng.choice([0,1<<22,1<<23]),4)
        w(sym['D_800D7DFC']+0x32C,0,1)
        for i in range(64): w(RETTAB+i*4,rng.choice([1,0x10000,5,0x30002,0x7fff0001]),4)
    imgf=cand+'.img'; open(imgf,'wb').write(mem)
    smem={}
    for k in range(len(sym)):
        o=SYMB-BASE+k*0x4000
        for i in range(0x400):
            if mem[o+i]: smem[BASE+o+i]=mem[o+i]
    for lo,hi in ((OT,OT+0x100),(ARGS,ARGS+0x200),(PLAYER,PLAYER+0x80),(RETTAB,RETTAB+0x100)):
        for a in range(lo,hi):
            if mem[a-BASE]: smem[a]=mem[a-BASE]
    def stub(nm,args,sim):
        n=len(sim.calls)-1
        sim.wr(LOG+n*20,ids[nm],4)
        k=nargs.get(nm,0)
        for j in range(4): sim.wr(LOG+n*20+4+j*4,args[j] if j<k else 0,4)
        r=sim.rd(RETTAB+n*4,4)
        if nm=='func_80028858':
            sim.wr(args[1],r&0xFFFF,2); sim.wr(args[2],r>>16,2)
        return r
    s=Sim(asm,sym,smem,{'$sp':STACK},stub); s.rev=rev
    s.run()
    out=subprocess.run([cand+'.host',imgf],capture_output=True).stdout
    bad=[]
    ref=bytearray(SIZE)
    for a,b in smem.items(): ref[a-BASE]=b
    sl=sym['D_800D5F48']-BASE
    ref[sl:sl+4]=out[sl:sl+4]
    for lo,hi in [(SYMB,OT+0x1000),(LOG,LOG+0x1000)]:
        if out[lo-BASE:hi-BASE]!=ref[lo-BASE:hi-BASE]:
            for a in range(lo,hi):
                if out[a-BASE]!=ref[a-BASE]: bad.append((hex(a),out[a-BASE],ref[a-BASE]))
    for c in s.calls: tot_calls.add(c[0])
    if bad: nbad+=1; print('trial',trial,'DIFF',len(bad),bad[:6],[c[0] for c in s.calls][:12])
print('trials',NT,'bad',nbad,'distinct calls',sorted(tot_calls))
