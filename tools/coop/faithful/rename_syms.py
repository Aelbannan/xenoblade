import sys,re
# usage: rename_syms.py old=new ...   (applies by name to us/eu/jp symbols.txt and targets.json names)
pairs=[a.split('=',1) for a in sys.argv[1:]]
for reg in ('us','eu','jp'):
    p=f'config/{reg}/symbols.txt'; s=open(p).read(); n=0
    for o,nw in pairs:
        pat=re.compile(r'^'+re.escape(o)+r' = ',re.M)
        if pat.search(s):
            s=pat.sub(lambda m: nw+' = ',s,count=1); n+=1
    open(p,'w').write(s); print(reg,n,'of',len(pairs))
p='tools/coop/targets.json'; s=open(p).read(); n=0
for o,nw in pairs:
    for key in ('"symbol": "','"name": "'):
        k=key+o+'"'
        if k in s: s=s.replace(k,key+nw+'"'); n+=1
open(p,'w').write(s); print('targets',n)
