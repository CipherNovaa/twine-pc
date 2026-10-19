import re,sys,subprocess,random
asm=sys.argv[1]; cfile=sys.argv[2]
A,B=0x37,0xC9
SYM={'D_800AE940':0x10000,'D_800AB604':0x20000,'D_800AB608':0x20004}
mem={0x20000:A,0x20004:B}
R={}
def g(r): return 0 if r=='$zero' else R.get(r,0)&0xffffffff
def s(r,v): R[r]=v&0xffffffff
def val(x):
    m=re.match(r'%(hi|lo)\((\w+)\)',x)
    if m: return SYM[m.group(2)] if m.group(1)=='hi' else 0
    return int(x,0)
def st(a,v,n):
    for i in range(n): mem[a+i]=(v>>(8*i))&0xff
def ld(a,n): return sum(mem.get(a+i,0)<<(8*i) for i in range(n))
for l in open(asm):
    m=re.search(r'\*/\s+(\w+)\s+(.*)$',l)
    if not m: continue
    op,args=m.group(1),[a.strip() for a in m.group(2).split(',',2)]
    if op in('nop','jr'): continue
    if op=='lui': s(args[0],val(args[1])); continue
    if op in('addiu','ori'): 
        v=val(args[2])
        if op=='ori': v&=0xffff; s(args[0],g(args[1])|v)
        else: s(args[0],g(args[1])+v)
        continue
    if op=='addu': s(args[0],g(args[1])+g(args[2])); continue
    mm=re.match(r'(.*)\((\$\w+)\)',args[1])
    a=(val(mm.group(1))+g(mm.group(2)))&0xffffffff
    n={'b':1,'h':2,'w':4}[op[1]]
    if op[0]=='s': st(a,g(args[0]),n)
    else:
        if op[2:]=='u' or op[1]=='w': s(args[0],ld(a,n)) 
        else: sys.exit('op '+op)
def dump(): return bytes(mem.get(0x10000+i,0) for i in range(0x470))
ref=dump()
# host build
src=open(cfile).read().replace('#include "twine.h"','typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef short s16;typedef unsigned int u32;typedef int s32;')
src=src.replace('extern u8 D_800AB604;','u8 D_800AB604=%d;'%A).replace('extern u8 D_800AB608;','u8 D_800AB608=%d;'%B).replace('extern u8 D_800AE940[];','u8 D_800AE940[0x470];')
src+='\n#include <stdio.h>\nint main(){func_800274F4();fwrite(D_800AE940,1,0x470,stdout);return 0;}\n'
open('build/difftest_h.c','w').write(src)
subprocess.run(['gcc','-w','-O0','-o',cfile+'.bin',  'build/difftest_h.c'],check=True)
out=subprocess.run([cfile+'.bin'],capture_output=True).stdout
print('match' if out==ref else 'DIFF', sum(1 for x,y in zip(out,ref) if x!=y), sum(1 for x in ref if x))
for i,(x,y) in enumerate(zip(out,ref)):
    if x!=y: print(hex(i),'C',x,'asm',y)
