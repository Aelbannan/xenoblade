import json
w3=json.load(open('.scratch/wave3.json'))
nw=json.load(open('.scratch/next_wave.json'))

def chunk(lst,n):
    k,m=divmod(len(lst),n); out=[]; i=0
    for j in range(n):
        size=k+(1 if j<m else 0); out.append(lst[i:i+size]); i+=size
    return [c for c in out if c]

batches={
 'kyoshin-leftover-1': chunk(w3['kyo'],3)[1],
 'kyoshin-leftover-0': chunk(w3['kyo'],3)[0],
 'criware-data-0': w3['cri'][:6],
 'monolib-regression-sweep': """main/monolib/src/core/CProc
main/monolib/src/core/CView
main/monolib/src/device/CDeviceGX
main/monolib/src/lib/CLibHbm
main/monolib/src/lib/CLibStaticData
main/monolib/src/lib/UnkClass_80460C34
main/monolib/src/lod/UnkClass_8046368C
main/monolib/src/nand/CNBanner
main/monolib/src/scn/CScnItemPool
main/monolib/src/scn/code_804BC9EC
main/monolib/src/util/CChildListNode
main/monolib/src/work/CProcess
main/monolib/src/work/CWorkThread""".split(),
 'monolib-regressions': ["main/monolib/src/effect/CERand","main/monolib/src/nand/CNReqtaskSave"],
 'rvl-data-2': chunk(nw['rvl'],3)[2],
}
json.dump(batches, open('.tmp_batches.json','w'), indent=1)
for k,v in batches.items():
    print(k, len(v), v[:4], '...' if len(v)>4 else '')
