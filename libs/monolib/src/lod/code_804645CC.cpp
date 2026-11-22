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
#include "monolib/lod/UnkClass_8046D144.hpp"
#include "monolib/scn/CScn.hpp"

struct LodDrawObj;
struct LodTexSel;

namespace LOD {
struct UnkClass_804645CC {
    void drawLodRecordExt();
    void drawLodRecord();
    void updateLodState();
    void refreshLodView();
    void setLodFadeAlpha();
    void resetLodFlags();
    void scaleLodFadeAlpha();
    void setLodColorFactor();
    static void setupLodTexFog(CScn* item, LodDrawObj* obj);
    static void buildLodTexObj(LodTexSel* sel, GXTexMapID map, f32 lodBias);
    void setLodDepthScale();
    static void setupLodTev(void* rec);
};
}

using namespace LOD;

// TODO(us-8046859c / us-80468b54): billboard quad renderers - not yet
// reconstructed (psq paired-single vertex emission).
void LOD::UnkClass_804645CC::drawLodRecordExt() {}

void LOD::UnkClass_804645CC::drawLodRecord() {}

// Reset all four TEV swap-mode tables to the identity RGBA mapping.
void LOD::UnkClass_804645CC::updateLodState() {
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

void selectLodTexCoord(s32 value) {
    if (lbl_eu_806657D0 == value) {
        return;
    }

    u32 offset = ((u32)value << 1) & 0x7C;
    s32 lowBit = value & 1;
    LodTexCoordSetup handler = *(LodTexCoordSetup*)((u8*)sLodTexCoordSetupTable + offset);
    s32 highByte = value >> 8;
    lbl_eu_806657D0 = value;
    handler(lowBit + 2, highByte);
}

// ---------------------------------------------------------------------------
// Imports used by the Fv-named draw entry points below.
// ---------------------------------------------------------------------------

// Panic message strings (nw4r asserts inside the texture loader).
extern const char lbl_eu_8052CF10[];
extern const char lbl_eu_8052CEF4[];
extern const char lbl_eu_8052CF6C[];
extern const char lbl_eu_8052CF50[];
extern const char lbl_eu_80661E98[];
extern const char lbl_eu_80661EA8[];

extern u32 sAmbientScaleLut[];   // tex-mtx work table (0x38 / 0x68 blocks)
extern u32 lbl_eu_80665780;     // bound ResFile pointer

// reloadChildGxLights is a real LOD::LODG3dMgr member (UnkClass_8046D144.hpp).
extern "C" f32 getLodCacheObject__Q23LOD17UnkClass_8046368CFv(s32 index);
extern "C" void fetchSampleXY__Q23LOD17CLODCacheManagerSFv(s32* outA,
                                                           s32* outB);

struct CScnEnvLgtCtrlResList;  // scene env-controller list owner (scnLgtEnterMode20 target)
extern "C" void* getScnRootSlot10(void* self);
extern "C" void scnLgtEnterMode20(CScnEnvLgtCtrlResList* ctrl);

typedef void (*LodCallFn)();  // no-arg dispatch-table entry

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

// Draw object handed to setupLodTexFog (r4) and to the billboard renderers.
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

// Per-record render-state entry (stride 0x2C) consumed by applyLodRecState.
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

// Work area shared by the LOD texture-matrix/fog pass: scale words followed
// by the incoming tex-matrix and the scene projection tex-matrix.
struct LodTexMatrixWork {
    u32 mScaleLut[14];              // 0x00..0x37
    f32 mTexMatrix[3][4];           // 0x38..0x67
    nw4r::math::MTX34 mProjection;  // 0x68..0x97
};

// Texture selector record consumed by buildLodTexObj.
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
// us-8046910c  setupLodTexFog  (frame tex-matrix + fog state setup)
// ---------------------------------------------------------------------------
void LOD::UnkClass_804645CC::setupLodTexFog(CScn* item, LodDrawObj* obj) {
    LodRes11xx* res =
        (LodRes11xx*)((LodSceneResPtr*)item->unk64)->field_0x08;
    u32* mtx = sAmbientScaleLut;

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
    sLastColorChanMode = -1;
    sLastAlphaChanMode = 0;
    lbl_eu_806657F8 = -1;
    sCachedCurrentMtx = 0;
    lbl_eu_80665800 = (-1.0f);
    lbl_eu_80665804 = 0;
    lbl_eu_8066580C = 0;
    lbl_eu_806657E8 = 0;
    sAmbientAlpha = 0xFF;

    Vec v;
    v.x = 10000.0f;
    v.y = 10000.0f;
    v.z = 0.0f;
    PSMTXMultVec(*lbl_eu_80657FD8, &v, &v);

    sLodLightMask0 = obj->field_0x410;
    sLodLightMask1 = obj->field_0x69C;
    sLodLightMask2 = obj->field_0x928;
    GXLoadTexMtxImm(((LodTexMatrixWork*)sAmbientScaleLut)->mTexMatrix,
                    0x40, GX_MTX_3x4);

    nw4r::g3d::Camera cam =
        ((nw4r::g3d::ScnRoot*)getScnRootSlot10(item))->GetCurrentCamera();
    cam.GetProjectionTexMtx(
        &((LodTexMatrixWork*)sAmbientScaleLut)->mProjection);

    scnLgtEnterMode20(item->mLgtCtrl);
    lbl_eu_806657FC = 1;
    nw4r::g3d::G3DState::LoadFog(0);
    lbl_eu_80665810 = (u32)obj;
}

void LOD::UnkClass_804645CC::refreshLodView() {
    if ((lbl_eu_806657E8 & 1) == 0) {
        if (sCachedCurrentMtx != 0) {
            sCachedCurrentMtx = 0;
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
    sAmbientAlpha = 0xFF;
    lbl_eu_806657E8 &= ~2u;
}

// ---------------------------------------------------------------------------
// us-8046931c  applyLodRecState  (per-record render-state apply)
// ---------------------------------------------------------------------------
void applyLodRecState__Q23LOD17UnkClass_804645CCFv(s32 value) {
    u8* tbl = (u8*)sLodTexCoordSetupTable;
    LodStateRec* rec = (LodStateRec*)(lbl_eu_8066581C + value * 0x2c);

    if ((rec->field_0x0C & 0x40) != 0) {
        f32 cur = getLodCacheObject__Q23LOD17UnkClass_8046368CFv(rec->field_0x1E);
        f32 delta = 1.0f - cur;
        if (1.0f != delta) {
            s32 fade = sAmbientAlpha;
            sAmbientAlpha = (s32)((f32)fade * delta);
            lbl_eu_806657E8 |= 2;
        }
    }

    // Texcoord handler dispatch (shared table, +0xB0), then the light/
    // texture reload through the memory manager.
    LodCallFn* tblB0 = (LodCallFn*)(tbl + 0xB0);
    LodCallFn texCoordFn = tblB0[lbl_eu_80665804 + rec->field_0x09];
    texCoordFn();
    int lightIdx = ((int*)(tbl + 0x128))[rec->field_0x0A];
    ((LOD::LODG3dMgr*)lbl_eu_80665810)->reloadChildGxLights(lightIdx);

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

        if (sLastAlphaChanMode != 3) {
            sLastAlphaChanMode = 3;
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
        lbl_eu_80665800 != sLodThresholdDist) {
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
    UnkClass_804645CC::setupLodTev(rec);

    u16 zFlags = rec->field_0x0C;
    GXSetZMode((GXBool)((zFlags >> 1) & 1), GX_LEQUAL, (GXBool)(zFlags & 1));

    lbl_eu_806657E8 &= ~1u;
    lbl_eu_806657D4 = (u32)value;
    lbl_eu_80665800 = sLodThresholdDist;
}

// ---------------------------------------------------------------------------
// us-804697d0  buildLodTexObj  (texture object build + bind)
// ---------------------------------------------------------------------------
void LOD::UnkClass_804645CC::buildLodTexObj(LodTexSel* sel, GXTexMapID map,
                                            f32 lodBias) {
    nw4r::g3d::ResTex tex;
    u16 idxA;
    u16 idxB;

    if ((sel->field_0x07 & 0x20) != 0) {
        // Indices live in the LOD cache manager's sample area.
        fetchSampleXY__Q23LOD17CLODCacheManagerSFv((s32*)&idxA, (s32*)&idxB);
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
                        lbl_eu_80661E98, "ref");
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
                            lbl_eu_80661EA8, "ref");
        }
        u16 numEntries = pltt.ref().numEntries;
        if (pltt.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2a, lbl_eu_8052CF50,
                            lbl_eu_80661EA8, "ref");
        }
        GXTlutFmt tlutFmt = pltt.ref().fmt;
        if (pltt.ptr() == NULL) {
            nw4r::db::Panic(lbl_eu_8052CF6C, 0x2a, lbl_eu_8052CF50,
                            lbl_eu_80661EA8, "ref");
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
        GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f,
                        0.0f, 0.0f, GX_FALSE, GX_FALSE,
                        GX_ANISO_1);
    }
    GXLoadTexObj(&texObj, map);
}

// The retail table keeps these entry points under shortened Fv names while
// passing their real ABI values in the argument registers.  Keep the linker
// names explicit and model those values as ordinary C++ parameters.
void setLodFadeAlpha(s32 value) {
    sAmbientAlpha = value;
    lbl_eu_806657E8 |= 2;
}

// Scale the pending colour value: convert s32 to f32, scale, and round back to s32.
// (retail free function scaleLodFadeAlpha__Ff)
void scaleLodFadeAlpha(f32 scale) {
    s32 value = sAmbientAlpha;
    f32 fv = (f32)value;
    lbl_eu_806657E8 |= 2;
    sAmbientAlpha = (s32)(fv * scale);
}

// 0xFF clears the colourfulness flag; otherwise record the colour and its alpha
// divisor (6) into the shared state. (retail free function setLodColorFactor__Fl)
void setLodColorFactor(s32 value) {
    if (value == 0xFF) {
        lbl_eu_806657E8 &= 0xFFFFFFFB;
        lbl_eu_80665804 = 0;
        return;
    }

    lbl_eu_806657E8 |= 4;
    lbl_eu_80665804 = 6;
    sAmbientAlphaScale = 0.003921569f * (f32)value;
}

extern "C" void setLodDepthScale__Q23LOD17UnkClass_804645CCFv(s16 value) {
    s16 local = value;
    OSs16tof32(&local, &sAmbientIntensityScale);
}
