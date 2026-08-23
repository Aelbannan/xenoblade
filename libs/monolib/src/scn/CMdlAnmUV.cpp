// monolib/src/scn/CMdlAnmUV - UV-animation helper object.
#include <harness_catalog.h>
#include <nw4r/g3d/g3d_scnmdl.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/res/g3d_resanmchr.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resnode.h>
#include <nw4r/db/db_assert.h>
#include <string.h>
#include <monolib/util/FixStr.hpp>
#include "libs/monolib/src/scn/CMdlAnmUV.hpp"

// Foreign Panic/file/format strings (other TUs' data).
extern const char lbl_eu_8056E068[];
extern const char lbl_eu_8056E04C[];
extern const char lbl_eu_806638E8[];
extern const char lbl_eu_8056E1C8[];
extern const char lbl_eu_8056E1A8[];
extern const char lbl_eu_8056E194[];
extern const char lbl_eu_8056E178[];

extern "C" const char lbl_eu_805247DC[0xC];  // def below (this TU .rodata)
extern "C" u32 lbl_eu_80663C80;  // def below (this TU .sdata, "ref")
extern "C" nw4r::g3d::ResMdl func_80488F44(void* model, int flag);  // CScnItemModel.cpp: model -> ResMdl accessor


CMdlAnmUV::CMdlAnmUV() {
    field_04 = 0;
    field_38 = 0;
    field_3c = 0;
    field_3d = 0;
}

// Retail delete-flag dtor fragment (same recipe as CMdlAnmEye.cpp):
// cmpi r3,0 / beq end; cmpi r4,0 / ble end; bl __dl__FPv(this); end: return self.
extern "C" void __dl__FPv(void*);
extern "C" void* __dt__9CMdlAnmUVFv(CMdlAnmUV* self, int flag) {
    if (self != 0 && flag > 0) {
        __dl__FPv(self);
    }
    return self;
}

// Owner-model views for func_804E72D0: the ScnMdl pointer (+0x147C) and the
// two vtable slots it calls (0x60, 0x68; declared virtual N lands at 8+4*N).
struct CMdlAnmUVModelVt {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void v08() = 0;
    virtual void v09() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual u32 v22() = 0;   // vtable 0x60
    virtual void v23() = 0;
    virtual int v24() = 0;   // vtable 0x68
};

struct CMdlAnmUVModel {
    u8 pad_00[0x147C];
    void* field_0x147C;      // g3d ScnMdl
};

// Per-frame UV SRT push. For every registered (node,material) pair whose node
// id matches, fetch the node translation and push a flip/scale effect matrix
// (diag lbl_eu_8066B350 / off-diag lbl_eu_8066B354, negated translation) into
// the material's texture SRT. Returns whether any pair was processed.
extern "C" bool func_804E72D0(CMdlAnmUV* self, u32 nodeId,
                              const nw4r::g3d::ChrAnmResult* result) {
    s32 count = 0;
    nw4r::g3d::ResTexSrt texSrt(NULL);
    for (u32 i = 0; i < self->field_38; i++) {
        if (self->field_08[i] != nodeId) {
            continue;
        }
        if (i == 2 && ((CMdlAnmUVModelVt*)self->field_04)->v22() != 0) {
            continue;
        }
        nw4r::math::VEC3 t;
        count++;
        result->GetTranslate(&t);
        if (i < 2) {
            s32 xySmall = 0;
            s32 allSmall = 0;
            if ((f32)__fabs(t.x) <= lbl_eu_8066A208) {
                if ((f32)__fabs(t.y) <= lbl_eu_8066A208) {
                    xySmall = 1;
                }
            }
            if (xySmall != 0) {
                if ((f32)__fabs(t.z) <= lbl_eu_8066A208) {
                    allSmall = 1;
                }
            }
            if (allSmall != 0 && ((CMdlAnmUVModelVt*)self->field_04)->v24() != 1) {
                continue;
            }
        }
        nw4r::g3d::ScnMdl::CopiedMatAccess cma(
            (nw4r::g3d::ScnMdl*)((CMdlAnmUVModel*)self->field_04)->field_0x147C,
            self->field_18[i]);
        texSrt = cma.GetResTexSrt(false);
        if (!texSrt.IsValid()) {
            continue;
        }
        nw4r::math::MTX34 mtx;
        if ((self->field_3c != 0 && i == 0) || self->field_3d != 0) {
            f32 ty = t.y;
            f32 tz = t.z;
            mtx._00 = lbl_eu_8066B350; mtx._01 = lbl_eu_8066B354; mtx._02 = lbl_eu_8066B354;
            mtx._03 = -tz;
            mtx._10 = lbl_eu_8066B354; mtx._11 = lbl_eu_8066B350; mtx._12 = lbl_eu_8066B354;
            mtx._13 = -ty;
            mtx._20 = lbl_eu_8066B354; mtx._21 = lbl_eu_8066B354; mtx._22 = lbl_eu_8066B350; mtx._23 = lbl_eu_8066B354;
        } else {
            f32 tz = t.z;
            f32 tx = t.x;
            mtx._00 = lbl_eu_8066B350; mtx._01 = lbl_eu_8066B354; mtx._02 = lbl_eu_8066B354;
            mtx._03 = -tx;
            mtx._10 = lbl_eu_8066B354; mtx._11 = lbl_eu_8066B350; mtx._12 = lbl_eu_8066B354;
            mtx._13 = -tz;
            mtx._20 = lbl_eu_8066B354; mtx._21 = lbl_eu_8066B354; mtx._22 = lbl_eu_8066B350; mtx._23 = lbl_eu_8066B354;
        }
        texSrt.SetEffectMtx(0, &mtx);
        texSrt.SetEffectMtx(1, &mtx);
        texSrt.SetEffectMtx(2, &mtx);
    }
    return count != 0;
}

// Name-driven node/material registration (draft from asm packet; iterate
// against hexdiff). "pc06"-prefixed models flag name-mode and bail;
// "np705011" models flag name-mode and register the base pair plus every
// node/mat pair sharing the prefix and trailing two-digit index.
struct CMdlAnmUVRegVt {
    virtual void v00() = 0;
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual void v05() = 0;
    virtual const char* v06() = 0;   // vtable 0x18: model name query
};

struct CMdlAnmUVResModel {
    u8 pad_00[0x146C];
    nw4r::g3d::ResMdlData* field_0x146C;
};

void func_804E6C80(CMdlAnmUV* self, CMdlAnmUVResModel* model) {
    self->field_04 = model;
    const char* modelName = ((CMdlAnmUVRegVt*)model)->v06();
    u32 len = strlen(modelName);
    if (len == 8) {
        if (modelName[0] == 'p' && modelName[1] == 'c' && modelName[2] == '0' &&
            modelName[3] == '6') {
            self->field_3d = 1;
            return;
        }
        if (!(modelName[0] == 'n' && modelName[1] == 'p' && modelName[2] == '7' &&
              modelName[3] == '5' && modelName[4] == '0' && modelName[5] == '1' &&
              modelName[6] == '0' && modelName[7] == '1')) {
            return;
        }
        self->field_3d = 1;
    } else {
        return;
    }

    nw4r::g3d::ResMdl mdl(model->field_0x146C);
    nw4r::g3d::ResNode baseNode = mdl.GetResNode((const char*)lbl_eu_80663C74);
    if (!baseNode.IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E1C8, 56, lbl_eu_8056E1A8);
    }
    const u16 baseNodeId = baseNode.ref().id;
    nw4r::g3d::ResMat baseMat = mdl.GetResMat((const char*)lbl_eu_80663C74);
    if (!baseMat.IsValid()) {
        nw4r::db::Panic(lbl_eu_8056E1C8, 56, lbl_eu_8056E1A8);
    }
    const u16 baseMatId = baseMat.ref().id;

    // Base pair, stage -1.
    u32 n = self->field_38;
    self->field_08[n] = baseNodeId;
    self->field_18[n] = baseMatId;
    self->field_28[n] = 0xFFFF;
    self->field_38 = n + 1;
    self->field_3c = 1;

    const char* prefix = lbl_eu_80663C70;
    const u32 nodeSfxOfs = strlen(prefix) + 2;
    const char* baseName = (const char*)lbl_eu_80663C74;
    const u32 matSfxOfs = strlen(baseName) + 2;

    nw4r::g3d::ResMdl scanMdl = func_80488F44(model, 0);
    const u32 nNodes = scanMdl.GetResNodeNumEntries();
    for (u32 i = 0; i < nNodes; i++) {
        nw4r::g3d::ResNode node = scanMdl.GetResNode(i);
        if (!node.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E194, 44, lbl_eu_8056E178);
        }
        const char* nodeName = node.ofs_to_ptr<char>(node.ref().name);
        ml::FixStr<16> expect;
        expect.format(lbl_eu_805247DC, nodeName);
            const u32 nMats = scanMdl.GetResMatNumEntries();
            for (u32 j = 0; j < nMats; j++) {
                nw4r::g3d::ResMat mat = scanMdl.GetResMat(j);
            if (!mat.IsValid()) {
                nw4r::db::Panic(lbl_eu_8056E068, 0x26D, lbl_eu_8056E04C,
                                lbl_eu_806638E8, lbl_eu_80663C80);
            }
            const char* matName = mat.ofs_to_ptr<char>(mat.ref().name);
            if (strstr(matName, prefix) != matName) {
                continue;
            }
            if (strlen(matName) != matSfxOfs) {
                continue;
            }
            const char* mp = matName + matSfxOfs;
            if ((u32)(mp[-2] - '0') > 9 || (u32)(mp[-1] - '0') > 9) {
                continue;
            }
            const char* np = nodeName + nodeSfxOfs;
            if ((u32)(np[-2] - '0') > 9 || (u32)(np[-1] - '0') > 9) {
                continue;
            }
            if (np[-2] != mp[-2] || np[-1] != mp[-1]) {
                continue;
            }
            n = self->field_38;
            self->field_08[n] = node.ref().id;
            self->field_18[n] = mat.ref().id;
            self->field_28[n] = (np[-2] - '0') * 10 + (np[-1] - '0');
            self->field_38 = n + 1;
            self->field_3c = 1;
        }
    }

    if (self->field_3d != 0 && self->field_38 == 3) {
        u16 tmp = self->field_18[0];
        self->field_18[0] = self->field_18[1];
        self->field_18[1] = tmp;
    }
}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace AUBlob { extern "C" void __dt__9CMdlAnmUVFv(); }
extern "C" u32 lbl_eu_8066B348;    // foreign .sdata2 string

// [.rodata] 0x805247D0-0x805247E8 (24B): class-name string + FixStr format
extern "C" __declspec(align(4)) const char lbl_eu_805247D0[0xC] = {
    0x43,0x4D,0x64,0x6C,0x41,0x6E,0x6D,0x55,0x56,0x00,0x00,0x00,
};
extern "C" __declspec(align(4)) const char lbl_eu_805247DC[0xC] = {
    0x25,0x73,0x25,0x64,0x25,0x64,0x00,0x00,0x00,0x00,0x00,0x00,
};

// [.sdata] 0x80663C74-0x80663C88 (20B, 5 words) -- defined as 5 separate scalars to satisfy header's scalar decl for lbl_eu_80663C74
// Header declares lbl_eu_80663C74 as u32; we define it and the following 4 words as separate globals placed contiguously in .sdata
extern "C" __declspec(section ".sdata") u32 lbl_eu_80663C74 = (u32)&lbl_eu_8066B348;
extern "C" u32 lbl_eu_80663C78 = (u32)&lbl_eu_805247D0;
// zero-init would land in .sbss; const + explicit section keeps it PROGBITS
// in .sdata (same recipe as CMdlAnmEye.cpp)
extern "C" __declspec(section ".sdata") const u32 lbl_eu_80663C7C = 0;
extern "C" u32 lbl_eu_80663C80 = 0x72656600;
extern "C" u32 lbl_eu_80663C84 = 0x72656600;

// retail: lwz r3, lbl_eu_80663C74; blr
extern "C" u32 func_804E6C78() { return lbl_eu_80663C74; }

// [.data] 0x805701C0-0x805701D0 (16B): CMdlAnmUV vtable
extern "C" u32 lbl_eu_805701C0[4] = {
    (u32)&lbl_eu_80663C78, 0x00000000,
    (u32)&AUBlob::__dt__9CMdlAnmUVFv, 0x00000000,
};
