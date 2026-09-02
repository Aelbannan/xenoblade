#!/usr/bin/env python3
import pathlib, struct, shutil, sys
sys.path.insert(0, ".")
from tools.postprocess_reloc_names import UnitRules, postprocess_object

assigned = [
    ("build/us/obj/kyoshin/cf/CfCam.o", "build/us/src/kyoshin/cf/CfCam.o"),
    ("build/us/obj/kyoshin/cf/CfBdat.o", "build/us/src/kyoshin/cf/CfBdat.o"),
    ("build/us/obj/kyoshin/cf/CfResPcImpl.o", "build/us/src/kyoshin/cf/CfResPcImpl.o"),
    ("build/us/obj/kyoshin/cf/CfNandManager.o", "build/us/src/kyoshin/cf/CfNandManager.o"),
    ("build/us/obj/kyoshin/cf/CfGimmickEne.o", "build/us/src/kyoshin/cf/CfGimmickEne.o"),
]

for retail, decomp in assigned:
    p = pathlib.Path(decomp)
    if p.exists():
        rules = UnitRules(copy_data_sections=(".data",".rodata",".sdata",".sdata2",".bss",".sbss",".sbss2"))
        postprocess_object(p, rules)
        print(f"copied {decomp}")
