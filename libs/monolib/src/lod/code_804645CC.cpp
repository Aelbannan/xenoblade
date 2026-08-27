// Decompilation of monolib/src/lod/code_804645CC
// LOD::UnkClass_804645CC - LOD billboard/quad renderer state + draw routines.
//
// Retail symbols use shortened Fv names in symbols.txt (the DOL is stripped),
// while several entry points receive data in r3/r4/f1.  These explicit retail
// name entry points model the extra ABI values as ordinary C++ parameters, as
// documented in docs/MWCC_CASES.md.  Register usage follows the retail
// ABI (r3 is the first explicit value).

#include <harness_catalog.h>
#include <types.h>
#include <revolution/GX.h>
#include <revolution/gx/GXTev.h>
#include <revolution/gx/GXAttr.h>
#include <revolution/gx/GXGeometry.h>
#include <revolution/gx/GXTransform.h>
#include <revolution/os/OSFastCast.h>
#include <revolution/MTX.h>
#include <nw4r/db/db_assert.h>
#include <nw4r/g3d/g3d_camera.h>
#include <nw4r/g3d/g3d_scnroot.h>
#include <nw4r/g3d/res/g3d_resfile.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/res/g3d_restex.h>
#include "monolib/lod/code_804645CC.hpp"
#include "libs/monolib/src/lod/code_80468434.hpp"

namespace LOD {
struct UnkClass_804645CC {
    void func_804645CC();
    void func_80464B84();
    void updateLodState();
    void refreshLodView();
    void func_80465704();
    void resetLodFlags();
    void func_80465730();
    void func_8046577C();
    void func_804657E4();
    void func_80465BC0();
};
}

using namespace LOD;

// TODO(us-8046859c / us-80468b54): billboard quad renderers - not yet
// reconstructed (psq paired-single vertex emission).
void LOD::UnkClass_804645CC::func_804645CC() {}

void LOD::UnkClass_804645CC::func_80464B84() {}

// Reset all four TEV swap-mode tables to the identity RGBA mapping.
void LOD::UnkClass_804645CC::updateLodState() {
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

void func_80465314(s32 value) {
    if (lbl_eu_806657D0 == value) {
        return;
    }

    u32 offset = ((u32)value << 1) & 0x7C;
    s32 lowBit = value & 1;
    LodTexCoordSetup handler = *(LodTexCoordSetup*)((u8*)lbl_eu_8056D728 + offset);
    s32 highByte = value >> 8;
    lbl_eu_806657D0 = value;
    handler(lowBit + 2, highByte);
}

// ---------------------------------------------------------------------------
// Imports used by the Fv-named draw entry points below.
// ---------------------------------------------------------------------------
extern const f32 lbl_eu_8066A618;
extern const f32 lbl_eu_8066A61C;
extern const f32 lbl_eu_8066A620;
extern const f32 lbl_eu_8066A624;

// Panic message strings (nw4r asserts inside the texture loader).
extern const char lbl_eu_8052CF10[];
extern const char lbl_eu_8052CEF4[];
extern const char lbl_eu_8052CF6C[];
extern const char lbl_eu_8052CF50[];
extern const char lbl_eu_8052CF3C[];
extern const char lbl_eu_8052CF20[];
extern const char lbl_eu_80661E98[];
extern const char lbl_eu_80661EA8[];
extern const char lbl_eu_80663808[];
extern const char lbl_eu_80663810[];
extern const char lbl_eu_8066380C[];

extern u32 lbl_eu_80658348[];   // tex-mtx work table (0x38 / 0x68 blocks)
extern u32 lbl_eu_80665780;     // bound ResFile pointer

extern "C" void func_80471484__Q23LOD9LODMemManFv(void* self, int childIdx);
extern "C" f32 getLodCacheObject__Q23LOD17UnkClass_8046368CFv(s32 index);
extern "C" void func_8046339C__Q23LOD17CLODCacheManagerSFv(s32* outA,
                                                           s32* outB);
extern "C" void func_80465BC0__Q23LOD17UnkClass_804645CCFv(void* rec);

class CScnEnvLgtCtrl;  // scene env-light controller (func_804C19B8 target)
extern "C" void* func_8048ECD8(void* self);
extern "C" void func_804C19B8(CScnEnvLgtCtrl* ctrl);

typedef void (*LodCallFn)();  // no-arg dispatch-table entry

// Scene item handed to func_8046513C (r3).
struct LodSceneItem {
    u8 pad_00[0x64];
    u32 field_0x64;  // resource-pointer holder
    u8 pad_68[0x7C - 0x68];
    u32 field_0x7C;  // env-light ctrl
};

struct LodSceneResPtr {
    u8 pad_00[0x08];
    u32 field_0x08;  // base of the 0x11xx word block
};

struct LodRes11xx {
    u8 pad_00[0x11A8];
    u32 field_0x11A8;
    u32 field_0x11AC;
    u32 field_0x11B0;
    u32 field_0x11B4;
    u8 pad_11B8[0x11C8 - 0x11B8];
    u32 field_0x11C8;
    u32 field_0x11CC;
    u32 field_0x11D0;
    u32 field_0x11D4;
};

// Draw object handed to func_8046513C (r4) and to the billboard renderers.
struct LodDrawObj {
    u8 pad_00[0x30];
    f32 field_0x30;
    f32 field_0x34;
    u8 pad_38[0x3C - 0x38];
    s16 field_0x3C;
    u8 pad_3E[0x44 - 0x3E];
    u16 field_0x44;
    u8 pad_46[0x410 - 0x46];
    u32 field_0x410;
    u8 pad_414[0x69C - 0x414];
    u32 field_0x69C;
    u8 pad_6A0[0x928 - 0x6A0];
    u32 field_0x928;
};

// Per-record render-state entry (stride 0x2C) consumed by func_8046534C.
struct LodStateRec {
    u8 pad_00[0x04];
    u8 field_0x04;   // blend-mode table select
    u8 field_0x05;   // cull mode
    u8 field_0x06;   // channel-control table select
    u8 field_0x07;   // fog id
    u8 pad_08;
    u8 field_0x09;   // texcoord handler index
    u8 field_0x0A;   // texture / light index
    u16 field_0x0C;  // flags: 0x40 fade update, 0x20 colour dirty, 0x02 z-test
    u8 pad_0E[0x10 - 0x0E];
    union {
        u32 raw;
        GXColor color;
    } field_0x10;  // material colour
    u8 pad_14[0x1E - 0x14];
    u16 field_0x1E;  // fade-distance cache index
};

// Texture selector record consumed by func_80465800.
struct LodTexSel {
    union {
        struct {
            u16 field_0x00;
            u16 field_0x02;
            u8 pad_04[3];
        };
        u8 raw_00[0x07];
    };
    u8 field_0x07;  // bits: 0x20 = cached indices, 0x10 = pltt-info array,
                    // 0x03/0xC0 = wrap modes
    u16 pad_08;
    u16 field_0x0A;
};

// ---------------------------------------------------------------------------
// us-8046910c  func_8046513C  (frame tex-matrix + fog state setup)
// ---------------------------------------------------------------------------
extern "C" void func_8046513C__Q23LOD17UnkClass_804645CCFv(LodSceneItem* item,
                                                           LodDrawObj* obj) {
    LodRes11xx* res =
        (LodRes11xx*)((LodSceneResPtr*)item->field_0x64)->field_0x08;
    u32* mtx = lbl_eu_80658348;

    mtx[0] = res->field_0x11A8;
    mtx[1] = res->field_0x11AC;
    mtx[2] = res->field_0x11B0;
    mtx[3] = res->field_0x11B4;
    mtx[4] = res->field_0x11C8;
    mtx[5] = res->field_0x11CC;
    mtx[6] = res->field_0x11D0;
    mtx[7] = res->field_0x11D4;

    lbl_eu_806657D0 = -1;
    lbl_eu_806657D4 = -1;
    lbl_eu_806657D8 = -1;
    lbl_eu_806657DC = -1;
    lbl_eu_806657E0 = 0;
    lbl_eu_806657F8 = -1;
    lbl_eu_80665818 = 0;
    lbl_eu_80665800 = lbl_eu_8066A618;
    lbl_eu_80665804 = 0;
    lbl_eu_8066580C = 0;
    lbl_eu_806657E8 = 0;
    lbl_eu_80665814 = 0xFF;

    Vec v;
    v.x = lbl_eu_8066A61C;
    v.y = lbl_eu_8066A61C;
    v.z = lbl_eu_8066A620;
    PSMTXMultVec(*lbl_eu_80657FD8, &v, &v);

    lbl_eu_806657EC = obj->field_0x410;
    lbl_eu_806657F0 = obj->field_0x69C;
    lbl_eu_806657F4 = obj->field_0x928;
    f32 (*mtxBlock)[3][4] = (f32 (*)[3][4])(void*)((u8*)lbl_eu_80658348 + 0x38);
    GXLoadTexMtxImm(*mtxBlock, 0x40, GX_MTX_3x4);

    nw4r::g3d::Camera cam =
        ((nw4r::g3d::ScnRoot*)func_8048ECD8(item))->GetCurrentCamera();
    cam.GetProjectionTexMtx(
        (nw4r::math::MTX34*)(void*)((u8*)lbl_eu_80658348 + 0x68));

    func_804C19B8((CScnEnvLgtCtrl*)item->field_0x7C);
    lbl_eu_806657FC = 1;
    nw4r::g3d::G3DState::LoadFog(0);
    lbl_eu_80665810 = (u32)obj;
}

void LOD::UnkClass_804645CC::refreshLodView() {
    if ((lbl_eu_806657E8 & 1) == 0) {
        if (lbl_eu_80665818 != 0) {
            lbl_eu_80665818 = 0;
            GXSetCurrentMtx(0);
        }
        GXSetCullMode(GX_CULL_NONE);
        GXSetArray(GX_VA_TEX0, (const void*)lbl_eu_80665770, 8);
        GXSetArray(GX_VA_CLR0, (const void*)lbl_eu_80665774, 4);
        lbl_eu_806657E8 |= 1;
    }
}

// Store 0xFF into the pending-value slot and clear flag bit 1 of the state word.
void LOD::UnkClass_804645CC::resetLodFlags() {
    lbl_eu_80665814 = 0xFF;
    lbl_eu_806657E8 &= ~2u;
}

// ---------------------------------------------------------------------------
// us-8046931c  func_8046534C  (per-record render-state apply)
// ---------------------------------------------------------------------------
void func_8046534C__Q23LOD17UnkClass_804645CCFv(s32 value) {
    u8* tbl = (u8*)lbl_eu_8056D728;
    LodStateRec* rec = (LodStateRec*)(lbl_eu_8066581C + value * 0x2c);

    if ((rec->field_0x0C & 0x40) != 0) {
        f32 cur = getLodCacheObject__Q23LOD17UnkClass_8046368CFv(rec->field_0x1E);
        f32 delta = lbl_eu_8066A624 - cur;
        if (lbl_eu_8066A624 != delta) {
            s32 fade = lbl_eu_80665814;
            lbl_eu_80665814 = (s32)((f32)fade * delta);
            lbl_eu_806657E8 |= 2;
        }
    }

    // Texcoord handler dispatch (shared table, +0xB0), then the light/
    // texture reload through the memory manager.
    LodCallFn* tblB0 = (LodCallFn*)(tbl + 0xB0);
    LodCallFn texCoordFn = tblB0[lbl_eu_80665804 + rec->field_0x09];
    texCoordFn();
    int lightIdx = ((int*)(tbl + 0x128))[rec->field_0x0A];
    func_80471484__Q23LOD9LODMemManFv((void*)lbl_eu_80665810, lightIdx);

    if ((lbl_eu_806657E8 & 0xC) != 0) {
        if (lbl_eu_806657D8 == (u32)(value + 0xbb8)) {
            goto checkTail;
        }

        LodCallFn alphaFn = ((LodCallFn*)(tbl + 0x104))[rec->field_0x0A];
        alphaFn();
        GXSetAlphaCompare(GX_ALWAYS, 1, GX_AOP_AND, GX_GEQUAL, 255);
        if (rec->field_0x04 != 0) {
            LodCallFn blendFn =
                *(LodCallFn*)(tbl + 0x98 + (((u32)rec->field_0x04 << 2) & 0xFF));
            blendFn();
        } else {
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
                           GX_LO_CLEAR);
        }

        lbl_eu_806657D8 = (u32)(value + 0xbb8);
        if (lbl_eu_806657D4 == (u32)value) {
            goto checkTail;
        }
        goto applyState;
    } else {
        if (lbl_eu_806657D8 == (u32)value) {
            goto checkTail;
        }

        if (lbl_eu_806657E0 != 3) {
            lbl_eu_806657E0 = 3;
            GXSetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX,
                          GX_LIGHT0, GX_DF_NONE, GX_AF_NONE);
        }
        LodCallFn chanFn = ((LodCallFn*)(tbl + 0x70))[rec->field_0x06];
        chanFn();
        LodCallFn blendFn2 = ((LodCallFn*)(tbl + 0x98))[rec->field_0x04];
        blendFn2();

        lbl_eu_806657D8 = (u32)value;
        if (lbl_eu_806657D4 == (u32)value) {
            goto checkTail;
        }
        goto applyState;
    }

checkTail:
    // Only refresh the material colour / fog when the distance changed.
    if ((rec->field_0x0C & 0x20) != 0 &&
        lbl_eu_80665800 != lbl_eu_80665754) {
        goto applyState;
    }
    return;

applyState:
    GXSetChanMatColor(GX_COLOR0A0, rec->field_0x10.color);
    GXSetCullMode((GXCullMode)rec->field_0x05);
    if (rec->field_0x07 != 0) {
        if (lbl_eu_806657FC != rec->field_0x07) {
            lbl_eu_806657FC = rec->field_0x07;
            nw4r::g3d::G3DState::LoadFog(rec->field_0x07 - 1);
        }
    } else {
        if (lbl_eu_806657FC != 0) {
            lbl_eu_806657FC = 0;
            nw4r::g3d::G3DState::LoadFog(0x40);
        }
    }
    LodCallFn finalFn = ((LodCallFn*)(tbl + 0xE0))[rec->field_0x0A];
    finalFn();
    func_80465BC0__Q23LOD17UnkClass_804645CCFv(rec);

    u16 zFlags = rec->field_0x0C;
    GXSetZMode((GXBool)((zFlags >> 1) & 1), GX_LEQUAL, (GXBool)(zFlags & 1));

    lbl_eu_806657E8 &= ~1u;
    lbl_eu_806657D4 = (u32)value;
    lbl_eu_80665800 = lbl_eu_80665754;
}

// ---------------------------------------------------------------------------
// us-804697d0  func_80465800  (texture object build + bind)
// ---------------------------------------------------------------------------
extern "C" void func_80465800__Q23LOD17UnkClass_804645CCFv(LodTexSel* sel,
                                                           GXTexMapID map,
                                                           f32 lodBias) {
    nw4r::g3d::ResTex tex;
    u16 idxA;
    u16 idxB;

    if ((sel->field_0x07 & 0x20) != 0) {
        // Indices live in the LOD cache manager's sample area.
        func_8046339C__Q23LOD17CLODCacheManagerSFv((s32*)&idxA, (s32*)&idxB);
    } else {
        idxA = sel->field_0x00;
        idxB = sel->field_0x02;
    }
    if ((sel->field_0x07 & 0x10) != 0) {
        if (lbl_eu_8066580C != 0) {
            nw4r::g3d::ResTexPlttInfo* infos =
                (nw4r::g3d::ResTexPlttInfo*)lbl_eu_8066580C;
            tex = infos[idxA].GetResTex();
        } else {
            tex = nw4r::g3d::ResFile((void*)lbl_eu_80665780).GetResTex(0);
        }
    } else {
        tex = nw4r::g3d::ResFile((void*)lbl_eu_80665780).GetResTex(idxA);
    }
    if (tex.ptr() == NULL) {
        nw4r::db::Panic(lbl_eu_8052CF10, 0x26, lbl_eu_8052CEF4,
                        lbl_eu_80661E98, lbl_eu_80663808);
    }

    GXTexObj texObj;
    f32 minLod;
    f32 maxLod;
    GXBool mipMap;
    if (tex.ref().flag & 1) {
        // Colour-index textures: fetch/refresh the palette, then build a CI
        // texture object with the shared TLUT slot.
        nw4r::g3d::ResPltt pltt;
        if ((sel->field_0x07 & 0x10) != 0) {
            if (lbl_eu_8066580C != 0) {
                nw4r::g3d::ResTexPlttInfo* infos =
                    (nw4r::g3d::ResTexPlttInfo*)lbl_eu_8066580C;
                tex = infos[idxA].GetResTex();
            } else {
                pltt = tex.GetParent().GetResPltt(0);
            }
        } else {
            pltt = tex.GetParent().GetResPltt(idxB);
        }

        void* image;
        u16 width;
        u16 height;
        GXCITexFmt ciFmt;
        tex.GetTexObjCIParam(&image, &width, &height, &ciFmt, &minLod,
                             &maxLod, &mipMap);
        u32 tlutName = (ciFmt == GX_TF_C14X2) ? 0x10 : 0;

        if (pltt.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2a, lbl_eu_8052CF50,
                            lbl_eu_80661EA8, lbl_eu_80663810);
        }
        u16 numEntries = pltt.ref().numEntries;
        if (pltt.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2a, lbl_eu_8052CF50,
                            lbl_eu_80661EA8, lbl_eu_80663810);
        }
        GXTlutFmt tlutFmt = pltt.ref().fmt;
        if (pltt.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2a, lbl_eu_8052CF50,
                            lbl_eu_80661EA8, lbl_eu_8066380C);
        }

        GXTlutObj tlutObj;
        GXInitTlutObj(&tlutObj, pltt.ofs_to_ptr<void>(pltt.ref().toPlttData),
                      tlutFmt, numEntries);
        GXLoadTlut(&tlutObj, tlutName);

        GXInitTexObjCI(&texObj, image, width, height, (GXTexFmt)ciFmt,
                       (GXTexWrapMode)((sel->field_0x07 >> 0) & 3),
                       (GXTexWrapMode)((sel->field_0x07 >> 6) & 3), mipMap,
                       tlutName);
    } else {
        void* image;
        u16 width;
        u16 height;
        GXTexFmt fmt;
        tex.GetTexObjParam(&image, &width, &height, &fmt, &minLod, &maxLod,
                           &mipMap);
        GXInitTexObj(&texObj, image, width, height, fmt,
                     (GXTexWrapMode)((sel->field_0x07 >> 0) & 3),
                     (GXTexWrapMode)((sel->field_0x07 >> 6) & 3), mipMap);
    }

    if (mipMap != 0) {
        GXInitTexObjLOD(&texObj, GX_LIN_MIP_LIN, GX_LINEAR, minLod, maxLod,
                        lodBias, GX_FALSE, GX_FALSE, GX_ANISO_1);
    } else {
        GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, lbl_eu_8066A620,
                        lbl_eu_8066A620, lbl_eu_8066A620, GX_FALSE, GX_FALSE,
                        GX_ANISO_1);
    }
    GXLoadTexObj(&texObj, map);
}

void LOD::UnkClass_804645CC::func_80465BC0() {}

// The retail table keeps these entry points under shortened Fv names while
// passing their real ABI values in the argument registers.  Keep the linker
// names explicit and model those values as ordinary C++ parameters.
void func_80465704(s32 value) {
    lbl_eu_80665814 = value;
    lbl_eu_806657E8 |= 2;
}

// Scale the pending colour value: convert s32 to f32, scale, and round back to s32.
// (retail free function func_80465730__Ff)
void func_80465730(f32 scale) {
    s32 value = lbl_eu_80665814;
    f32 fv = (f32)value;
    lbl_eu_806657E8 |= 2;
    lbl_eu_80665814 = (s32)(fv * scale);
}

// 0xFF clears the colourfulness flag; otherwise record the colour and its alpha
// divisor (6) into the shared state. (retail free function func_8046577C__Fl)
void func_8046577C(s32 value) {
    if (value == 0xFF) {
        lbl_eu_806657E8 &= 0xFFFFFFFB;
        lbl_eu_80665804 = 0;
        return;
    }

    lbl_eu_806657E8 |= 4;
    lbl_eu_80665804 = 6;
    lbl_eu_80665808 = lbl_eu_8066A630 * (f32)value;
}

extern "C" void func_804657E4__Q23LOD17UnkClass_804645CCFv(s16 value) {
    s16 local = value;
    OSs16tof32(&local, &lbl_eu_806657E4);
}
