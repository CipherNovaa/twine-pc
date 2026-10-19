import re
REGS=['$zero','$at','$v0','$v1','$a0','$a1','$a2','$a3','$t0','$t1','$t2','$t3','$t4','$t5','$t6','$t7','$s0','$s1','$s2','$s3','$s4','$s5','$s6','$s7','$t8','$t9','$k0','$k1','$gp','$sp','$fp','$ra']
M32=0xffffffff
def sx(v,b): v&=(1<<b)-1; return v-(1<<b) if v>>(b-1) else v
class Sim:
    def __init__(s,asmfile,sym,mem,regs,stubs):
        s.sym=sym; s.mem=mem; s.R={r:0 for r in REGS}; s.R.update(regs); s.stubs=stubs; s.calls=[]
        s.ins=[]; s.lab={}
        for l in open(asmfile):
            m=re.match(r'\s*(\.L\w+):',l)
            if m: s.lab[m.group(1)]=len(s.ins); continue
            m=re.search(r'\*/\s+(\w+)\s*(.*)$',l)
            if not m: continue
            op=m.group(1); a=m.group(2).strip()
            args=[x.strip() for x in a.split(',')] if a else []
            # 'sw $a, off($b)' splits fine on commas
            s.ins.append((op,args))
    def v(s,x):
        if x in s.R: return s.R[x]&M32
        m=re.match(r'%(hi|lo)\((\w+)\)$',x)
        if m: return s.sym[m.group(2)] if m.group(1)=='hi' else 0
        m=re.match(r'\((.+) >> 16\)$',x)
        if m: return (int(m.group(1),0)>>16)<<16 if m.group(1).startswith('0x') else (s.sym[re.match(r'%hi\((\w+)\)',m.group(1)).group(1)] if False else 0)
        m=re.match(r'\((.+) & 0xFFFF\)$',x)
        if m: return int(m.group(1),0)&0xffff
        return int(x,0)
    def rd(s,a,n): return sum(s.mem.get((a+i)&M32,0)<<(8*i) for i in range(n))
    def wr(s,a,v,n):
        for i in range(n): s.mem[(a+i)&M32]=(v>>(8*i))&0xff
    def addr(s,x):
        m=re.match(r'(.*)\((\$\w+)\)$',x)
        return (s.v(m.group(1) if m.group(1) else '0')+s.v(m.group(2)))&M32
    def tgt(s,x):
        return s.lab[x]
    def step_one(s,i):
        """execute non-branch at i; returns None"""
        op,a=s.ins[i]; R=s.R; v=s.v
        def w(r,val): 
            if r!='$zero': R[r]=val&M32
        if op=='nop': return
        if op=='lui': w(a[0],v(a[1]) if a[1].startswith('%') else (v(a[1]) if '>>' in a[1] else int(a[1],0)<<16)); return
        if op=='addiu' or op=='addi': w(a[0],v(a[1])+sx(v(a[2]),32)); return
        if op=='ori': w(a[0],v(a[1])|(v(a[2])&0xffff)); return
        if op=='andi': w(a[0],v(a[1])&(v(a[2])&0xffff)); return
        if op=='xori': w(a[0],v(a[1])^(v(a[2])&0xffff)); return
        if op=='addu': w(a[0],v(a[1])+v(a[2])); return
        if op=='subu': w(a[0],v(a[1])-v(a[2])); return
        if op=='and': w(a[0],v(a[1])&v(a[2])); return
        if op=='or': w(a[0],v(a[1])|v(a[2])); return
        if op=='xor': w(a[0],v(a[1])^v(a[2])); return
        if op=='nor': w(a[0],~(v(a[1])|v(a[2]))); return
        if op=='sll': w(a[0],v(a[1])<<int(a[2],0)); return
        if op=='srl': w(a[0],v(a[1])>>int(a[2],0)); return
        if op=='sra': w(a[0],sx(v(a[1]),32)>>int(a[2],0)); return
        if op=='sllv': w(a[0],v(a[1])<<(v(a[2])&31)); return
        if op=='srlv': w(a[0],v(a[1])>>(v(a[2])&31)); return
        if op=='srav': w(a[0],sx(v(a[1]),32)>>(v(a[2])&31)); return
        if op=='slt': w(a[0],int(sx(v(a[1]),32)<sx(v(a[2]),32))); return
        if op=='sltu': w(a[0],int(v(a[1])<v(a[2]))); return
        if op=='slti': w(a[0],int(sx(v(a[1]),32)<sx(v(a[2]),32))); return
        if op=='sltiu': w(a[0],int(v(a[1])<(sx(v(a[2]),32)&M32))); return
        if op=='move': w(a[0],v(a[1])); return
        if op=='lw': w(a[0],s.rd(s.addr(a[1]),4)); return
        if op=='lh': w(a[0],sx(s.rd(s.addr(a[1]),2),16)); return
        if op=='lhu': w(a[0],s.rd(s.addr(a[1]),2)); return
        if op=='lb': w(a[0],sx(s.rd(s.addr(a[1]),1),8)); return
        if op=='lbu': w(a[0],s.rd(s.addr(a[1]),1)); return
        if op=='sw': s.wr(s.addr(a[1]),v(a[0]),4); return
        if op=='sh': s.wr(s.addr(a[1]),v(a[0]),2); return
        if op=='sb': s.wr(s.addr(a[1]),v(a[0]),1); return
        raise Exception('op '+op+' '+str(a))
    def run(s,limit=2000000):
        i=0; n=0
        while True:
            n+=1
            if n>limit: raise Exception('limit')
            op,a=s.ins[i]; v=s.v
            br=None
            if op in('beq','bne','beqz','bnez','blez','bgtz','bltz','bgez','j','jal','jr','jalr'):
                if op=='beq': t=v(a[0])==v(a[1]); lab=a[2]
                elif op=='bne': t=v(a[0])!=v(a[1]); lab=a[2]
                elif op=='beqz': t=v(a[0])==0; lab=a[1]
                elif op=='bnez': t=v(a[0])!=0; lab=a[1]
                elif op=='blez': t=sx(v(a[0]),32)<=0; lab=a[1]
                elif op=='bgtz': t=sx(v(a[0]),32)>0; lab=a[1]
                elif op=='bltz': t=sx(v(a[0]),32)<0; lab=a[1]
                elif op=='bgez': t=sx(v(a[0]),32)>=0; lab=a[1]
                elif op=='j': t=True; lab=a[0]
                elif op=='jal': t=True; lab=a[0]
                elif op=='jr': t=True; lab=None
                elif op=='jalr': t=True; lab=None
                # delay slot
                tv=None
                if op=='jr': tv='ret'
                if op in('jal','jalr'):
                    name=a[0] if op=='jal' else None
                    tgtv=None if op=='jal' else v(a[-1] if op=='jalr' else a[0])
                s.step_one(i+1)
                if op=='jr':
                    if a[0]=='$ra': return
                    i=None; raise Exception('jr non-ra')
                if op in('jal','jalr'):
                    nm=name if name else s.rev.get(tgtv,hex(tgtv))
                    args=[s.R[r]&M32 for r in('$a0','$a1','$a2','$a3')]
                    s.calls.append((nm,args))
                    s.R['$v0']=s.stubs(nm,args,s)&M32
                    i+=2; continue
                if t: i=s.tgt(lab)
                else: i+=2
                continue
            s.step_one(i); i+=1
