// Auto-scaffolded catalog TU for monolib/src/mpfsys/UnkClass_80471EC8
// Replace stubs with high-level C/C++ during decomp.

#include "monolib/mpfsys/UnkClass_80471EC8.hpp"
#include <harness_catalog.h>

#include <revolution/GX.h>

extern "C" {
    extern void* lbl_eu_80665844;
    extern s32 lbl_eu_80665854;
    extern s16 lbl_eu_80665858;
    extern s16 lbl_eu_8066585A;
    extern u32 lbl_eu_8066586C;
}

void mpfsys::UnkClass_80471EC8::func_80471EC8(void) {}

void mpfsys::UnkClass_80471EC8::func_80471FC8(void) {}

void mpfsys::UnkClass_80471EC8::func_80471FCC(void) {}

void mpfsys::UnkClass_80471EC8::func_80472064(void) {}

void mpfsys::UnkClass_80471EC8::func_8047230C(void) {}

void mpfsys::UnkClass_80471EC8::func_8047233C(void) {}

void mpfsys::UnkClass_80471EC8::func_80472370(void) {}

void mpfsys::UnkClass_80471EC8::func_804723A4(void) {}

void mpfsys::UnkClass_80471EC8::func_804724DC(void) {}

void mpfsys::UnkClass_80471EC8::func_80472864(void) {}

void mpfsys::UnkClass_80471EC8::func_80472870(void) {}

void mpfsys::UnkClass_80471EC8::func_804728E8() {}

void mpfsys::UnkClass_80471EC8::func_80473394() {}

void* mpfsys::UnkClass_80471EC8::func_804734F4(u8 layerIndex) {
    return (u8*)this + layerIndex * 0x1680;
}

#pragma dont_inline on
void mpfsys::UnkClass_80471EC8::func_80473500(int texIndex, int texMap, f32 texScale) {}
#pragma dont_inline off

void mpfsys::UnkClass_80471EC8::func_804737CC(s16 texIndex, f32 texScale) {
    if (lbl_eu_80665858 == texIndex) {
        return;
    }
    if (texIndex == -1) {
        return;
    }
    lbl_eu_80665858 = texIndex;
    func_80473500(texIndex, 0, texScale);
}

void mpfsys::UnkClass_80471EC8::func_804737F0() {}

void mpfsys::UnkClass_80471EC8::func_80473984() {}

void mpfsys::UnkClass_80471EC8::func_80474064() {}

void mpfsys::UnkClass_80471EC8::func_804742BC() {
    if (lbl_eu_8066585A == 0) {
        return;
    }
    GXSetTevColorIn((GXTevStageID)0, (GXTevColorArg)0xF, (GXTevColorArg)0xA, (GXTevColorArg)0x8, (GXTevColorArg)0xF);
    GXSetTevColorOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)2, (GXBool)1, (GXTevRegID)0);
    GXSetTevAlphaIn((GXTevStageID)0, (GXTevAlphaArg)0x7, (GXTevAlphaArg)0x6, (GXTevAlphaArg)0x4, (GXTevAlphaArg)0x7);
    GXSetTevAlphaOp((GXTevStageID)0, (GXTevOp)0, (GXTevBias)0, (GXTevScale)0, (GXBool)1, (GXTevRegID)0);
    GXClearVtxDesc();
    GXSetVtxDesc((GXAttr)0x9, (GXAttrType)0x1);
    GXSetVtxDesc((GXAttr)0xB, (GXAttrType)0x1);
    GXSetVtxDesc((GXAttr)0xD, (GXAttrType)0x2);
    GXSetArray((GXAttr)0xD, lbl_eu_80665844, 0x8);
    lbl_eu_8066585A = 0;
    if (lbl_eu_80665854 != 1) {
        GXSetNumTevStages(1);
        lbl_eu_80665854 = 1;
    }
    if (lbl_eu_8066586C & 0x10) {
        GXSetChanCtrl((GXChannelID)0, (GXBool)1, (GXColorSrc)0, (GXColorSrc)1, (GXLightID)1, (GXDiffuseFn)0, (GXAttnFn)2);
        lbl_eu_8066586C &= ~0x10;
    }
}

void mpfsys::UnkClass_80471EC8::func_804743E0() {}

void mpfsys::UnkClass_80471EC8::func_804744EC() {}

void mpfsys::UnkClass_80471EC8::func_8047466C() {}

void mpfsys::UnkClass_80471EC8::func_80474780() {}

void mpfsys::UnkClass_80471EC8::func_8047491C() {}

void mpfsys::UnkClass_80471EC8::func_80474A40() {}

void mpfsys::UnkClass_80471EC8::func_80474AA0() {}

void mpfsys::UnkClass_80471EC8::func_80474B00() {}

void mpfsys::UnkClass_80471EC8::func_80474CC4() {}

void mpfsys::UnkClass_80471EC8::func_80474CF4() const {}

void mpfsys::UnkClass_80471EC8::func_80474D50() {}

void mpfsys::UnkClass_80471EC8::func_80474DAC() {}

void mpfsys::UnkClass_80471EC8::func_80474DF8() {}

void mpfsys::UnkClass_80471EC8::func_80474E24() {}

void mpfsys::UnkClass_80471EC8::func_80474E68() {}

void mpfsys::UnkClass_80471EC8::func_80474EB0() {}

void mpfsys::UnkClass_80471EC8::func_80474F2C() {}

void mpfsys::UnkClass_80471EC8::func_80474F54() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// Static initializer referenced by .ctors section.
// No runtime initialisation required; returns immediately (blr).
extern "C" void sinit_80474F7C(void)
{
}
