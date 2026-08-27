import pathlib
edits = [
    ("src/kyoshin/cf/object/CfObject.hpp", "func_800BE898", "isVoiceActive"),
    ("src/kyoshin/cf/object/CfObjectMove.cpp", "func_800BE898", "isVoiceActive"),
    ("src/kyoshin/cf/object/CfObjectTbox.hpp", "func_801F8DD0", "updateState"),
    ("src/kyoshin/cf/object/CfObjectTbox.cpp", "func_801F8DD0", "updateState"),
    ("libs/monolib/include/monolib/core/CDrawGX.hpp", "func_80456570", "setBlendMode"),
    ("libs/monolib/include/monolib/core/CDrawGX.hpp", "func_8045657C", "setAlphaMode"),
    ("libs/monolib/src/core/CDrawGX.cpp", "func_80456570", "setBlendMode"),
    ("libs/monolib/src/core/CDrawGX.cpp", "func_8045657C", "setAlphaMode"),
    ("libs/monolib/include/monolib/core/CViewFrame.hpp", "func_8043FC60", "setOwner"),
    ("libs/monolib/include/monolib/core/CViewFrame.hpp", "func_80441290", "mapWorkType"),
    ("libs/monolib/src/core/CViewFrame.cpp", "func_8043FC60", "setOwner"),
    ("libs/monolib/src/core/CViewFrame.cpp", "func_80441290", "mapWorkType"),
    ("libs/monolib/src/device/CDeviceFileDvd.cpp", "func_80451984", "setAllocHandle"),
    ("libs/monolib/src/device/CDeviceFileDvd.cpp", "func_80451CBC", "addReadProgress"),
]
for path, old, new in edits:
    p = pathlib.Path(path)
    if not p.exists():
        print(f"missing {path}")
        continue
    t = p.read_text()
    if old in t:
        t = t.replace(old, new)
        p.write_text(t)
        print(f"edited {path}: {old} -> {new}")
    else:
        print(f"no {old} in {path}")
