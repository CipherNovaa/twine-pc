import re,sys,subprocess,random,os
sys.path.insert(0,os.path.dirname(__file__))
from mips import Sim
BASE=0x20000000; SIZE=0x100000
STACK=0x200B0000; RETTAB=0x200E0000; LOG=0x200F0000
CMP=[(0x20000000,0x200A0000),(0xE0000,0xE0001)]
def alloc_syms(asm,cand,extra=()):
    names=sorted(set(re.findall(r'\b(?:D|func|jtbl)_80[0-9A-F]{6}\b',open(asm).read()+open(cand).read()))|set(extra))
    sym={}
    a=0x20090000
    for n in names:
        sym[n]=a; a+=0x10
    return sym
def build_host(cand,sym,fname,stubs_c,callexpr,out):
    src=open(cand).read()
    src=src.replace('#include "twine.h"','')
    h='typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef short s16;typedef unsigned int u32;typedef int s32;\n'
    h+='#include <stdio.h>\n#include <string.h>\n#include <stdlib.h>\n#include <sys/mman.h>\n#define NULL 0\n'
    h+='#define M2C_FIELD(p,t,o) (*(t)((u8*)(p)+(o)))\n'
    outl=[]
    for l in src.split('\n'):
        m=re.match(r'extern\s+([\w ]+?)\s*(\*?)\s*(D_80[0-9A-F]{6})(\[\])?\s*;',l)
        if m:
            T=(m.group(1)+(' *' if m.group(2) else '')).strip()
            n=m.group(3); a=sym[n]
            if m.group(4): h+='#define %s ((%s*)0x%X)\n'%(n,T,a)
            else: h+='#define %s (*(%s*)0x%X)\n'%(n,T,a)
            continue
        outl.append(l)
    src='\n'.join(outl)
    h+='''
u32 *LOGP=(u32*)0x%X; u32 *RT=(u32*)0x%X; int NC=0;
static u32 logcall(int id,u32 a,u32 b,u32 c,u32 d){ LOGP[NC*5]=id;LOGP[NC*5+1]=a;LOGP[NC*5+2]=b;LOGP[NC*5+3]=c;LOGP[NC*5+4]=d; return RT[NC++]; }
'''%(LOG,RETTAB)
    h+=stubs_c+'\n'
    main='''
int main(int argc,char**argv){
  void*p=mmap((void*)0x%X,0x%X,PROT_READ|PROT_WRITE,MAP_FIXED|MAP_ANONYMOUS|MAP_PRIVATE|0x40,-1,0);
  if(p==MAP_FAILED){perror("mmap");return 1;}
  FILE*f=fopen(argv[1],"rb"); fread(p,1,0x%X,f); fclose(f);
  %s
  fwrite(p,1,0x%X,stdout); return 0; }
'''%(BASE,SIZE,SIZE,callexpr,SIZE)
    open(out+'.c','w').write(h+src+main)
    r=subprocess.run(['gcc','-m32','-w','-O0','-o',out,out+'.c'],capture_output=True,text=True)
    if r.returncode: print(r.stderr[:3000]); sys.exit(1)
def run_pair(asm,cand,fname,sym,mem,regs,stubs_c,callexpr,stubids,tag):
    # sim
    smem=dict(mem)
    rng=random.Random(5)
    for i in range(64): 
        for k in range(4): smem[RETTAB+i*4+k]=rng.randrange(256) if (i%2==0) else 0
    img=bytearray(SIZE)
    for a,b in smem.items():
        if BASE<=a<BASE+SIZE: img[a-BASE]=b
    imgf=cand+'.img'; open(imgf,'wb').write(img)
    rev={v:k for k,v in sym.items()}
    def stub(nm,args,sim):
        n=len(sim.calls)-1
        sim.wr(LOG+n*20,stubids[nm],4)
        for j,x in enumerate(args): sim.wr(LOG+n*20+4+j*4,x,4)
        return sim.rd(RETTAB+n*4,4)
    s=Sim(asm,sym,smem,dict(regs,**{'$sp':STACK}),stub); s.rev=rev
    s.run()
    build_host(cand,sym,fname,stubs_c,callexpr,cand+'.host')
    out=subprocess.run([cand+'.host',imgf],capture_output=True).stdout
    bad=0; first=[]
    for lo,hi in [(0x20000000,0x200A0000),(LOG,LOG+0x1000)]:
        for a in range(lo,hi):
            x=out[a-BASE]; y=smem.get(a,0) if a>=lo else 0
            if x!=y:
                bad+=1
                if len(first)<12: first.append((hex(a),x,y))
    print(tag,'calls',len(s.calls),'DIFF' if bad else 'MATCH',bad,first)
    return s
