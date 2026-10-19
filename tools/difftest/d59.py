import sys,random
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from harness import *
asm='asm/level01/nonmatchings/text_8002C808/func_80059E4C.s'; cand=sys.argv[1]
sym=alloc_syms(asm,cand)
OBJ=0x20000000; OT=0x20010000
for trial in range(int(sys.argv[2]) if len(sys.argv)>2 else 6):
    rng=random.Random(trial)
    mem={}
    def w(a,v,n):
        for i in range(n): mem[a+i]=(v>>(8*i))&0xff
    for a in range(0,0x300): mem[OBJ+a]=rng.randrange(256)
    for a in range(0,0x4000): mem[OT+a]=rng.randrange(256)
    # make z values small-ish so index varies
    w(sym['D_800DD83C'],0,1)
    d4=[0,1,2,3,4,5,6,-1][trial%8]
    w(sym['D_8009D8D4'],d4&0xff,1); w(sym['D_8009D8DC'],[0,1,2,0,0,2,1,0][trial%8],1); w(sym['D_8009D8DD'],trial%2,1)
    w(sym['D_800B8F24'],trial%2,4); w(sym['D_800D3234'],OT,4)
    if trial%3==2:
        # NULL arg: object lives at D_800DD83C
        for a in range(0,0x300): mem[sym['D_800DD83C']+a]=rng.randrange(256)
        arg=0
    else: arg=OBJ
    stubs='';
    run_pair(asm,cand,'func_80059E4C',sym,mem,{'$a0':arg},stubs,'func_80059E4C((s32*)0x%X);'%arg,{}, 'trial%d'%trial)
