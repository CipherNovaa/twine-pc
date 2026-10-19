import re,sys,subprocess
asm,cfile,fname=sys.argv[1:4]
src=open(cfile).read()
names=sorted(set(re.findall(r'\bD_80[0-9A-F]{6}\b',src+open(asm).read())))
funcs=sorted(set(re.findall(r'\bfunc_80[0-9A-F]{6}\b',open(asm).read()))-{fname})
SYM={}
vals={}
base=0x40000
for i,n in enumerate(names):
    SYM[n]=base+i*8
    vals[n]=(i*3+1)%60 if n not in('D_800E0904','D_800D86E0') else 0
for i,n in enumerate(funcs): SYM[n]=0xF0000000+i
BUF1=0x100000; BUF2=0x200000
mem={}
def st(a,v,n):
    for i in range(n): mem[a+i]=(v>>(8*i))&0xff
def ld(a,n): return sum(mem.get(a+i,0)<<(8*i) for i in range(n))
for n in names:
    if n=='D_800E0904': SYM[n]=BUF1
    elif n=='D_800D86E0': st(SYM[n],BUF2,4)
    else: st(SYM[n],vals[n],4)
R={}
def g(r): return 0 if r=='$zero' else R.get(r,0)&0xffffffff
def s(r,v): R[r]=v&0xffffffff
def val(x):
    m=re.match(r'%(hi|lo)\((\w+)\)',x)
    if m: return SYM[m.group(2)] if m.group(1)=='hi' else 0
    m=re.match(r'\((0x[0-9A-Fa-f]+) >> 16\)',x)
    if m: return (int(m.group(1),0)>>16)<<16
    m=re.match(r'\((0x[0-9A-Fa-f]+) & 0xFFFF\)',x)
    if m: return int(m.group(1),0)&0xffff
    return int(x,0)
def sx(v,b): v&=(1<<b)-1; return v-(1<<b) if v>>(b-1) else v
for l in open(asm):
    m=re.search(r'\*/\s+(\w+)\s+(.*)$',l)
    if not m: continue
    op,args=m.group(1),[a.strip() for a in m.group(2).split(',',2)]
    if op in('nop','jr','jal'): continue
    if op=='lui': s(args[0],val(args[1])); continue
    if op=='ori': s(args[0],g(args[1])|(val(args[2])&0xffff)); continue
    if op=='addiu': s(args[0],g(args[1])+val(args[2])); continue
    if op=='addu': s(args[0],g(args[1])+g(args[2])); continue
    if op=='subu': s(args[0],g(args[1])-g(args[2])); continue
    if op=='sll': s(args[0],g(args[1])<<int(args[2],0)); continue
    mm=re.match(r'(.*)\((\$\w+)\)',args[1])
    a=(val(mm.group(1))+g(mm.group(2)))&0xffffffff
    n={'b':1,'h':2,'w':4}[op[1]]
    if op[0]=='s': st(a,g(args[0]),n)
    elif op=='lw': s(args[0],ld(a,4))
    elif op=='lbu': s(args[0],ld(a,1))
    else: sys.exit('op '+op)
ref=bytes(mem.get(BUF1+i,0) for i in range(0x70*64))+bytes(mem.get(BUF2+i,0) for i in range(0x400))
h='typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef short s16;typedef unsigned int u32;typedef int s32;\n'
h+='#include <stdio.h>\n#include <string.h>\n#define _P(x) ((void*)(unsigned long)(x))\n'
c=src.replace('#include "twine.h"','')
c=re.sub(r'^extern .*$','',c,flags=re.M)
c=re.sub(r'^void func_\w+\(.*\);$','',c,flags=re.M)
for i,n in enumerate(funcs):
    c=c.replace('(void *)%s'%n,'(void *)0x%X'%(0xF0000000+i))
    c=c.replace('(void *)(%s)'%n,'0x%X'%(0xF0000000+i))
c=c.replace('*(void **)(','*(u32 *)(')
c=c.replace('*(void * *)(','*(u32 *)(')
c=re.sub(r'= \(void \*\)(0xF[0-9A-F]+);',r'= \1;',c)
c=re.sub(r'= \(void \*\)func_\w+;',r'= 0;',c)
dec=''
for n in names:
    if n=='D_800E0904': dec+='u8 D_800E0904[0x70*64];\n'
    elif n=='D_800D86E0': dec+='u8 BUF2[0x400]; u8 *D_800D86E0=BUF2;\n'
    else: dec+='long long D_pad_%s; int %s=%d;\n'%(n,n,vals[n]) if False else 'int %s=%d;\n'%(n,vals[n])
dec=dec.replace('int D_8009D2C8=','unsigned char D_8009D2C8=')
c=re.sub(r'void func_800679AC\(void\) \{','XX',c) if False else c
c=c.replace('(u8 *)D_800D86E0','D_800D86E0')
main='\n\nint main(){%s();fwrite(D_800E0904,1,0x70*64,stdout);fwrite(BUF2,1,0x400,stdout);return 0;}\n'%fname
open(sys.argv[2]+'.h.c','w').write(h+dec+'void func_800679AC(void){}\nvoid func_80032424(void*a){}\n'+c+main)
subprocess.run(['gcc','-w','-O0','-o',cfile+'.bin',cfile+'.h.c'],check=True)
out=subprocess.run([cfile+'.bin'],capture_output=True).stdout
d=[i for i,(x,y) in enumerate(zip(out,ref)) if x!=y]
print('match' if not d and len(out)==len(ref) else 'DIFF',len(d),len(out),len(ref),sum(1 for x in ref if x))
for i in d[:20]: print(hex(i),'C',out[i],'asm',ref[i])
