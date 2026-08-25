// Auto-scaffolded catalog TU for kyoshin/cf/CfHikariItemManager
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/scn/CScnTimeApi.hpp"

#include "kyoshin/cf/CfHikariItemManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/lib/CLibStaticData.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/math/Random.hpp"
#include "nw4r/g3d/g3d_state.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/math/math_types.h"
#include "revolution/GX.h"
#include "revolution/MTX.h"
#include "revolution/gx/GXHardware.h"

// (us-802b50a4): constructor.  Manual vptr store, registers the
// render callback, loads the static texture archive, initialises four GX
// texture objects at +0x1114 (one per i), then zeroes the record array and
// scratch regions and publishes this into the global lbl_eu_80664C10.
// The ResFile alignment panic mirrors the retail clrlwi. gate: the ResFile is
// constructed (word store) before the check, then Init() runs.
cf::CfHikariItemManager::CfHikariItemManager() {
    this->field_00 = (u32)lbl_eu_8053AE90;
    this->field_1194 = 0;
    this->field_1198 = 0;
    lbl_eu_80663E14->addRenderCB((IScnRender*)this, 1, 0);

    StaticDataHandle handle;
    if (CLibStaticData::getStaticFileData(lbl_eu_80513580, &handle, 0)) {
        nw4r::g3d::ResFile resFile(handle.data);
        if ((u32)handle.data & 0x1F) {
            nw4r::db::Panic(lbl_eu_8052637C, 0x3c, lbl_eu_80526354);
        }
        resFile.Init();
        // off is the byte offset into the +0x1114 tex-object region.  The two
        // GX calls deliberately index differently (i*0x20 vs off) so MWCC does
        // NOT CSE the texobj address across GXInitTexObj - retail recomputes
        // it (add;addi) per call.
        int off;
        int i;
        for (i = 0, off = 0; i < 4; i++, off += 0x20) {
            // Declaration order drives the stack slots (4-byte locals top-down):
            // img 0x20, minLod 0x1c, maxLod 0x18, fmt 0x14, tex 0x10,
            // w 0xc, h 0xa, mipmap 0x8 - retail's exact layout.
            void* img;
            f32 minLod, maxLod;
            GXTexFmt fmt;
            u16 w, h;
            u8 mipmap;
            nw4r::g3d::ResTex tex = resFile.GetResTex(i);
            tex.GetTexObjParam(&img, &w, &h, &fmt, &minLod, &maxLod, &mipmap);
            GXInitTexObj((GXTexObj*)&this->unk1114[i * 0x20], img, w, h, fmt,
                         GX_CLAMP, GX_CLAMP, mipmap);
            // Non-const sdata2 float: the load must sit after GXInitTexObj
            // (a call could modify it), so MWCC schedules it at first use and
            // fmr's the other two LOD args - exactly the retail lfs/fmr run.
            GXInitTexObjLOD((GXTexObj*)&this->unk1114[off], GX_LINEAR,
                            GX_LINEAR, lbl_eu_80668EC8, lbl_eu_80668EC8,
                            lbl_eu_80668EC8, GX_FALSE, GX_FALSE, GX_ANISO_1);
        }
    }

    memset(&this->records, 0, 0x100);
    lbl_eu_80664C10 = this;
    memset(&this->unk104, 0, 0x1000);
    memset(&this->unk1104, 0, 0x10);
    this->field_119C = 0;
    this->field_11A0 = 0;
}

// (us-802b523c): deleting destructor.  Manual vptr store (retail
// lbl_eu_8053AE90), then delete all 0x40 records, zero the +0x1104 scratch
// region and the counter block, and deregister the render callback.  The
// nested `if` replicates the MWCC double-null-check shape around the dtor call.
cf::CfHikariItemManager::~CfHikariItemManager() {
    this->field_00 = (u32)lbl_eu_8053AE90;

    for (int i = 0; i < 0x40; i++) {
        if (this->records[i] != NULL) {
            if (this->records[i] != NULL) {
                __dt__802B37B4(this->records[i], 1);
                this->records[i] = NULL;
            }
            this->records[i] = NULL;
        }
    }

    memset(&this->unk1104, 0, 0x10);
    this->field_1198 = 0;
    this->field_119C = 0;
    this->field_11A0 = 0;
    this->field_1194 = 0;

    lbl_eu_80663E14->removeRenderCB((IScnRender*)this);
}

// (us-802b5304): allocate a 0x44-byte Hikari item record, initialize
// it with func_802B3750, copy the caller's 12-byte vector into +0x00..+0x08
// and append it to the manager's record array.  Returns the record (or NULL
// when the array is full).  Count compares signed (retail cmpwi).
CfHikariItemRecord* func_802B2894(cf::CfHikariItemManager* self, const u32* src,
                                  u16 value) {
    if ((int)self->field_1198 >= 0x40) {
        return NULL;
    }

    CfHikariItemRecord* rec = (CfHikariItemRecord*)allocate__Q23mtl10MemManagerFUlUl(
        0x44, func_80061FFC());
    if (rec != NULL) {
        func_802B3750(rec, value);
    }

    rec->field_00 = src[0];
    rec->field_04 = src[1];
    rec->field_08 = src[2];

    u32 count = self->field_1198;
    self->records[count] = rec;
    self->field_1198 = count + 1;
    return rec;
}

// (us-802b53a8): remove `target` from the manager's record array.
// Searches for the pointer, deletes it, then shifts the tail of the array
// down by one and clears the vacated last slot.
void func_802B2938(cf::CfHikariItemManager* self, CfHikariItemRecord* target) {
    u32 count = self->field_1198;
    if (count == 0) {
        return;
    }
    if (target == NULL) {
        return;
    }

    int i = 0;
    for (i = 0; i < (int)count; i++) {
        if (self->records[i] == target) {
            if (target != NULL) {
                __dt__802B37B4(target, 1);
            }
            self->records[i] = NULL;
            break;
        }
    }

    self->field_1198 -= 1;
    for (; i < (int)self->field_1198; i++) {
        self->records[i] = self->records[i + 1];
    }
    self->records[self->field_1198] = NULL;
}

void func_802B2A08(void* self) {
    *(unsigned long*)((char*)self + 0x1194) |= 2;
}
// (us-802b5488): reset the manager - delete every record, clear the
// +0x1104 scratch region and zero the counter block.  Same cleanup tail as the
// destructor; the nested `if` replicates the MWCC double-null-check shape.
void func_802B2A18(cf::CfHikariItemManager* self) {
    for (int i = 0; i < 0x40; i++) {
        if (self->records[i] != NULL) {
            if (self->records[i] != NULL) {
                __dt__802B37B4(self->records[i], 1);
                self->records[i] = NULL;
            }
            self->records[i] = NULL;
        }
    }

    memset(&self->unk1104, 0, 0x10);
    self->field_1198 = 0;
    self->field_119C = 0;
    self->field_11A0 = 0;
    self->field_1194 = 0;
}

// (us-802b5528): toggle flag bit 0 of +0x1194 - nonzero arg clears
// the bit, zero sets it.
void func_802B2AB8(CfHikariItemRecord* self, u32 enable) {
    if (enable != 0) {
        self->field_1194 &= ~1u;
    } else {
        self->field_1194 |= 1u;
    }
}

// (us-802b5550): render-callback body.  Bails when there is no
// player, when the game-state flags carry bit 0x02000000/0x200, or while the
// manager's bit-0 toggle is set.  Otherwise it loads the current view frame's
// projection + inverse position matrix, normalizes the two camera axes into
// vec1/vec2, sets up a fixed TEV/channel/texgen state, then either updates
// every live record (fade == 0) or runs the delta-update/removal path
// (func_802B3810).  Finally it draws the record quads in four passes, one per
// sbss counter (C18/C1C with func_802B3E04/3F20, C20 with func_802B403C,
// C24 with func_802B44C8), each building its quad corners from the two
// normalized axes scaled by the pass-specific constants.
void cf::CfHikariItemManager::cbRenderBefore() {
    // Declaration order drives the stack slots (first declared = highest
    // address, matching retail: invMtx 0x98, rotMtx 0x68, corners 0x28,
    // vec1 0x1c, vec2 0x10, matColor 0xc, ambColor 0x8).
    CfMtxCopy invMtx;
    CfMtxCopy rotMtx;
    CfHikariQuadCorners corners;
    ml::CVec3 vec1;
    ml::CVec3 vec2;
    GXColor matColor;
    GXColor ambColor;

    if (cf::CfGameManager::getPlayer(0) == 0) {
        return;
    }
    if (lbl_eu_80663E24 & 0x02000200) {
        return;
    }
    if (this->field_1194 & 1) {
        return;
    }

    CfViewFrame* viewFrame = (CfViewFrame*)func_8049626C(lbl_eu_80663E14,
                                                         lbl_eu_80663E10);
    GXSetProjection(viewFrame->projection, GX_PERSPECTIVE);
    GXSetCurrentMtx(0);

    // Inverse of the view matrix as the position matrix for the quads.
    invMtx = viewFrame->mtx;
    PSMTXInverse(invMtx.m, invMtx.m);
    GXSetCurrentMtx(0);
    GXLoadPosMtxImm(invMtx.m, 0);

    // Second copy with the translation column zeroed: the two camera axes
    // (x/y rows of the rotation part) are transformed by it, then normalized.
    rotMtx = viewFrame->mtx;
    rotMtx.m[0][3] = lbl_eu_80668EC8;
    rotMtx.m[1][3] = lbl_eu_80668EC8;
    rotMtx.m[2][3] = lbl_eu_80668EC8;
    vec1.x = lbl_eu_80668ED0;
    vec1.y = lbl_eu_80668EC8;
    vec1.z = lbl_eu_80668EC8;
    vec2.x = lbl_eu_80668EC8;
    vec2.y = lbl_eu_80668ED0;
    vec2.z = lbl_eu_80668EC8;
    PSMTXMultVec(rotMtx.m, vec1, vec1);
    PSMTXMultVec(rotMtx.m, vec2, vec2);

    f32 len1 = vec1.x * vec1.x + vec1.y * vec1.y + vec1.z * vec1.z;
    if (len1 == lbl_eu_80668EC8) {
        vec1 = ml::CVec3::zero;
    } else {
        PSVECNormalize(vec1, vec1);
    }
    f32 len2 = vec2.x * vec2.x + vec2.y * vec2.y + vec2.z * vec2.z;
    if (len2 == lbl_eu_80668EC8) {
        vec2 = ml::CVec3::zero;
    } else {
        PSVECNormalize(vec2, vec2);
    }

    // Fixed pixel/TEV state for the additive quads.
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_LEQUAL, 0xff);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT0,
                  GX_DF_NONE, GX_AF_NONE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY,
                      GX_FALSE, GX_PTIDENTITY);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC,
                    GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                    GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA,
                    GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
                    GX_TRUE, GX_TEVPREV);
    u32 colorWord = lbl_eu_80668ECC;
    *(u32*)&matColor = colorWord;
    *(u32*)&ambColor = colorWord;
    GXSetChanMatColor(GX_COLOR0A0, matColor);
    GXSetChanAmbColor(GX_COLOR0A0, ambColor);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXLoadTexObj((GXTexObj*)&this->unk1114[0x00], GX_TEXMAP0);
    GXLoadTexObj((GXTexObj*)&this->unk1114[0x20], GX_TEXMAP1);
    GXLoadTexObj((GXTexObj*)&this->unk1114[0x40], GX_TEXMAP2);
    GXLoadTexObj((GXTexObj*)&this->unk1114[0x60], GX_TEXMAP3);
    nw4r::g3d::G3DState::LoadFog(
        ((const CfScnFogView*)lbl_eu_80663E14)->fogInfo->fogId);

    // Fade amount from the frame timer; forced to 0 while the game is in a
    // menu/fade state or when the manager's bit-1 toggle was already set.
    f32 fade = func_80496288(lbl_eu_80663E14);
    if (cf::CfGameManager::func_800829B8()) {
        fade = lbl_eu_80668EC8;
    }
    if (this->field_1194 & 2) {
        this->field_1194 &= ~2;
    } else {
        fade = lbl_eu_80668EC8;
    }
    func_802B371C((CfHikariItemRecord*)&viewFrame->rec[0]);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    if (fade == lbl_eu_80668EC8) {
        // Full update of every live record, then a pass over the pool slots
        // bumping the C24 counter for still-active entries.
        for (u32 i = 0; i < this->field_1198; i++) {
            func_802B3CA0(this->records[i]);
        }
        u32 slot = this->field_11A0;
        for (u32 iter = 0; iter < 0x80; iter++) {
            if (this->field_119C == slot) {
                break;
            }
            u32 bit = 1u << (slot & 0x1F);
            if (((u32*)&this->unk1104[0])[slot >> 5] & bit) {
                func_802B4460((CfHikariItemRecord*)&this->unk104[slot * 0x20]);
            }
            slot++;
            if (slot >= 0x80) {
                slot = 0;
            }
        }
    } else {
        // Fade path: per-record update; records whose update returns nonzero
        // are deleted and the array tail shifted down.
        for (u32 i = 0; i < this->field_1198; i++) {
            if (func_802B3810(this->records[i], fade) != 0) {
                if (this->records[i] != NULL) {
                    __dt__802B37B4(this->records[i], 1);
                    this->records[i] = NULL;
                }
                this->field_1198--;
                for (u32 j = i; j < this->field_1198; j++) {
                    this->records[j] = this->records[j + 1];
                }
                this->records[this->field_1198] = NULL;
                i--;
            }
        }
        u32 slot = this->field_11A0;
        for (u32 iter = 0; iter < 0x80; iter++) {
            if (this->field_119C == slot) {
                break;
            }
            u32 bit = 1u << (slot & 0x1F);
            if (((u32*)&this->unk1104[0])[slot >> 5] & bit) {
                if (func_802B4470((CfHikariItemRecord*)&this->unk104[slot * 0x20],
                                  fade) != 0) {
                    ((u32*)&this->unk1104[0])[slot >> 5] &= ~bit;
                    this->field_11A0++;
                    if (this->field_11A0 >= 0x80) {
                        this->field_11A0 = 0;
                    }
                }
            }
            slot++;
            if (slot >= 0x80) {
                slot = 0;
            }
        }
    }

    // Draw passes: each quad corner combines the two normalized camera axes
    // scaled by the pass-specific constants (retail vectorises the x/y pairs
    // into ps_muls0/ps_add runs).
    if (lbl_eu_80664C18 != 0) {
        corners.v[0][0] = vec1.x * lbl_eu_80668ED4 + vec2.x * lbl_eu_80668EDC;
        corners.v[0][1] = vec1.y * lbl_eu_80668ED4 + vec2.y * lbl_eu_80668EDC;
        corners.v[0][2] = vec1.z * lbl_eu_80668ED4 + vec2.z * lbl_eu_80668EDC;
        corners.v[1][0] = vec1.x * lbl_eu_80668ED4 + vec2.x * lbl_eu_80668EE0;
        corners.v[1][1] = vec1.y * lbl_eu_80668ED4 + vec2.y * lbl_eu_80668EE0;
        corners.v[1][2] = vec1.z * lbl_eu_80668ED4 + vec2.z * lbl_eu_80668EE0;
        corners.v[2][0] = vec1.x * lbl_eu_80668ED8 + vec2.x * lbl_eu_80668EE0;
        corners.v[2][1] = vec1.y * lbl_eu_80668ED8 + vec2.y * lbl_eu_80668EE0;
        corners.v[2][2] = vec1.z * lbl_eu_80668ED8 + vec2.z * lbl_eu_80668EE0;
        corners.v[3][0] = vec1.x * lbl_eu_80668ED8 + vec2.x * lbl_eu_80668EDC;
        corners.v[3][1] = vec1.y * lbl_eu_80668ED8 + vec2.y * lbl_eu_80668EDC;
        corners.v[3][2] = vec1.z * lbl_eu_80668ED8 + vec2.z * lbl_eu_80668EDC;
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0A0);
        GXBegin(GX_QUADS, GX_VTXFMT0, lbl_eu_80664C18 * 4);
        for (u32 i = 0; i < this->field_1198; i++) {
            func_802B3E04(this->records[i], &corners);
        }
    }
    if (lbl_eu_80664C1C != 0) {
        corners.v[0][0] = vec1.x * lbl_eu_80668EE4 + vec2.x * lbl_eu_80668EE4;
        corners.v[0][1] = vec1.y * lbl_eu_80668EE4 + vec2.y * lbl_eu_80668EE4;
        corners.v[0][2] = vec1.z * lbl_eu_80668EE4 + vec2.z * lbl_eu_80668EE4;
        corners.v[1][0] = vec1.x * lbl_eu_80668EE4 + vec2.x * lbl_eu_80668EE8;
        corners.v[1][1] = vec1.y * lbl_eu_80668EE4 + vec2.y * lbl_eu_80668EE8;
        corners.v[1][2] = vec1.z * lbl_eu_80668EE4 + vec2.z * lbl_eu_80668EE8;
        corners.v[2][0] = vec1.x * lbl_eu_80668EE8 + vec2.x * lbl_eu_80668EE8;
        corners.v[2][1] = vec1.y * lbl_eu_80668EE8 + vec2.y * lbl_eu_80668EE8;
        corners.v[2][2] = vec1.z * lbl_eu_80668EE8 + vec2.z * lbl_eu_80668EE8;
        corners.v[3][0] = vec1.x * lbl_eu_80668EE8 + vec2.x * lbl_eu_80668EE4;
        corners.v[3][1] = vec1.y * lbl_eu_80668EE8 + vec2.y * lbl_eu_80668EE4;
        corners.v[3][2] = vec1.z * lbl_eu_80668EE8 + vec2.z * lbl_eu_80668EE4;
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP3, GX_COLOR0A0);
        GXBegin(GX_QUADS, GX_VTXFMT0, lbl_eu_80664C1C * 4);
        for (u32 i = 0; i < this->field_1198; i++) {
            func_802B3F20(this->records[i], &corners);
        }
    }

    nw4r::g3d::G3DState::LoadFog(0x40);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
    if (lbl_eu_80664C20 != 0) {
        corners.v[0][0] = vec1.x * lbl_eu_80668EDC;
        corners.v[0][1] = vec1.y * lbl_eu_80668EDC;
        corners.v[0][2] = vec1.z * lbl_eu_80668EDC;
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXBegin(GX_QUADS, GX_VTXFMT0, lbl_eu_80664C20 * 4);
        for (u32 i = 0; i < this->field_1198; i++) {
            func_802B403C(this->records[i], &corners);
        }
    }
    if (lbl_eu_80664C24 != 0) {
        corners.v[0][0] = vec1.x * lbl_eu_80668EDC + vec2.x * lbl_eu_80668EDC;
        corners.v[0][1] = vec1.y * lbl_eu_80668EDC + vec2.y * lbl_eu_80668EDC;
        corners.v[0][2] = vec1.z * lbl_eu_80668EDC + vec2.z * lbl_eu_80668EDC;
        corners.v[1][0] = vec1.x * lbl_eu_80668EDC + vec2.x * lbl_eu_80668EE0;
        corners.v[1][1] = vec1.y * lbl_eu_80668EDC + vec2.y * lbl_eu_80668EE0;
        corners.v[1][2] = vec1.z * lbl_eu_80668EDC + vec2.z * lbl_eu_80668EE0;
        corners.v[2][0] = vec1.x * lbl_eu_80668EE0 + vec2.x * lbl_eu_80668EE0;
        corners.v[2][1] = vec1.y * lbl_eu_80668EE0 + vec2.y * lbl_eu_80668EE0;
        corners.v[2][2] = vec1.z * lbl_eu_80668EE0 + vec2.z * lbl_eu_80668EE0;
        corners.v[3][0] = vec1.x * lbl_eu_80668EE0 + vec2.x * lbl_eu_80668EDC;
        corners.v[3][1] = vec1.y * lbl_eu_80668EE0 + vec2.y * lbl_eu_80668EDC;
        corners.v[3][2] = vec1.z * lbl_eu_80668EE0 + vec2.z * lbl_eu_80668EDC;
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR0A0);
        GXBegin(GX_QUADS, GX_VTXFMT0, lbl_eu_80664C24 * 4);
        u32 slot = this->field_11A0;
        for (u32 iter = 0; iter < 0x80; iter++) {
            if (this->field_119C == slot) {
                break;
            }
            u32 bit = 1u << (slot & 0x1F);
            if (((u32*)&this->unk1104[0])[slot >> 5] & bit) {
                func_802B44C8((CfHikariItemRecord*)&this->unk104[slot * 0x20],
                              &corners);
            }
            slot++;
            if (slot >= 0x80) {
                slot = 0;
            }
        }
    }
}

// s32 -> f32 via MWCC's xoris/0x4330 bit pattern, subtracting the shared
// sdata2 magic double so the constant reloc names lbl_eu_80668EF0 instead of
// an anonymous pool constant.
static inline f32 hikariS32ToF32(Convert64& c, s32 v) {
    c.lo = (u32)v ^ 0x80000000;
    c.hi = 0x43300000;
    return (f32)(c.d - lbl_eu_80668EF0);
}

// (us-802b5fd8): claim the next slot in the inline 0x80-entry
// record pool (bitfield at +0x1104, 0x20-byte entries at +0x104), bail if it
// is already active, else perturb the spawn position by scaled random offsets
// (each component gets its own random draw) and init the entry via func_802B4358.
extern "C" void func_802B3568(cf::CfHikariItemManager* self, const f32* src,
                              s16 val, f32 scaleX, f32 scaleY) {
    // Signed count so MWCC emits the retail srawi/clrlwi/slwi bit-math run.
    // The slot address is recomputed from memory at the call site (retail
    // reloads field_119C instead of keeping count live).
    int count = self->field_119C;
    u32* wordPtr = (u32*)((char*)self + 0x1104 + ((count >> 5) << 2));
    u32 bit = 1u << (count & 0x1F);
    u32 word = *wordPtr;
    if (word & bit) {
        return;
    }
    *wordPtr = word | bit;

    // Stack shapes mirror retail: rnd at +0x08, out at +0x14, one Convert64
    // temp per mtRand (+0x20/+0x28/+0x30); each random is converted through
    // the shared sdata2 magic double and scaled.
    f32 rnd[3];
    f32 out[3];
    Convert64 c0;
    Convert64 c1;
    Convert64 c2;
    out[0] = src[0];
    out[1] = src[1];
    out[2] = src[2];

    rnd[0] = hikariS32ToF32(c0, ml::math::mtRand(-100, 100)) * lbl_eu_80668EEC;
    rnd[1] = hikariS32ToF32(c1, ml::math::mtRand(-100, 100)) * lbl_eu_80668EEC;
    rnd[2] = hikariS32ToF32(c2, ml::math::mtRand(-100, 100)) * lbl_eu_80668EEC;

    out[0] += rnd[0] * scaleX;
    out[1] += rnd[1] * scaleX;
    out[2] += rnd[2] * scaleX;

    func_802B4358((CfHikariItemRecord*)&self->unk104[self->field_119C * 0x20],
                  (const u32*)out, val, scaleY);

    // Retail reloads and stores the incremented count, then wraps.
    count = self->field_119C + 1;
    self->field_119C = count;
    if (count < 0x80) {
        return;
    }
    self->field_119C = 0;
}

// (us-802b618c): publish the record's first three words into the
// .bss global state block and reset the four sbss counters (retail emits
// four SDA-relocated word stores to lbl_eu_80664C18..24).
// (us-802b618c): publish the record's first three words into the
// .bss global state block and reset the four sbss counters (retail emits
// four SDA-relocated word stores to lbl_eu_80664C18..24).
extern "C" void func_802B371C(const CfHikariItemRecord* self) {
    u32* dst = (u32*)&lbl_eu_80577680[0];
    lbl_eu_80664C18 = 0;
    lbl_eu_80664C1C = 0;
    lbl_eu_80664C20 = 0;
    lbl_eu_80664C24 = 0;
    dst[0] = self->field_00;
    dst[1] = self->field_04;
    dst[2] = self->field_08;
}

// (us-802b61c0): initialize a freshly allocated 0x44-byte Hikari
// item record - zero the vector/accumulator floats, set the four color words
// and the caller-supplied u16 at +0x40.  Field order mirrors the retail
// store sequence (0x42 is written before 0x20/0x24).
extern "C" void func_802B3750(CfHikariItemRecord* self, u16 value) {
    f32 zero = lbl_eu_80668EF8; // const sdata2 float: schedules the lfs early
    *(f32*)&self->field_00 = zero;
    *(f32*)&self->field_04 = zero;
    *(f32*)&self->field_08 = zero;
    *(f32*)&self->field_0C = zero;
    *(f32*)&self->field_10 = zero;
    self->field_14 = zero;
    self->field_18 = zero;
    self->field_42 = 0;
    self->field_20 = zero;
    self->field_24 = zero;
    self->field_40 = value;
    self->field_30 = 0x96FFFFFF;
    self->field_34 = 0x20402064;
    self->field_38 = 0x20AAFF00;
    self->field_3C = 0x20AAFF64;
}

// (us-802b6224): destructor for the unknown class at 0x802B37B4
// (address-suffixed retail symbol).
extern "C" __declspec(noinline) void* __dt__802B37B4(void* self, int flag) {
    if (self != 0 && flag > 0) {
        ::operator delete(self);
    }
    return self;
}

// (us-802b6264): flag the u16 at +0x42 with 0x40 and write the
// sdata2 float constant to +0x1C and +0x14.
extern "C" void func_802B37F4(CfHikariItemRecord* self) {
    u16 val = self->field_42 | 0x40;    // lhz, ori
    self->field_42 = val;               // sth
    f32 f = lbl_eu_80668EF8;            // lfs
    self->field_1C = f;                 // stfs 0x1c
    self->field_14 = f;                 // stfs 0x14
}

// (us-802b6280): per-frame update of an active Hikari record.  Clears
// the low flag bits (keeping the 0x40/0x80/0x100 tests on the original word),
// then branches on the 0x40 flag: the spawn path advances the 0x1C/0x14
// timers (bursting 8 particles when 0x1C hits zero) and returns 1 when 0x1C
// crosses its 10-frame threshold (caller removes the record); the main path
// re-derives the two gradient colours from the global anchor when the record
// is within 2500 units (moving it toward the anchor), bumps the sbss counters
// when the 0x80 flag is clear, advances the 0x18 accumulator and spawns
// sparkle offsets, then refreshes the 0x38/0x3C colours through the
// 0x10/0x20 flag pair.  Returns 0 while the record stays alive.
extern "C" s32 func_802B3810(CfHikariItemRecord* self, f32 delta) {
    u32 flags = self->field_42;
    CfHikariItemRecord* rec = self;
    s32 ret = 1;
    bool countersOn = !(flags & 0x80); // gates the sbss counters
    const u32* g = lbl_eu_80577680;
    rec->field_42 = flags & 0xFFF0;

    if (flags & 0x40) {
        // Spawn trail: burst a fan of 8 particles when the 0x1C timer resets.
        if (rec->field_1C == lbl_eu_80668EF8) {
            for (int i = 0; i < 8; i++) {
                cf::CfHikariItemManager* mgr = func_802B262C();
                func_802B3568(mgr, (const f32*)rec, rec->field_40,
                              lbl_eu_80668EFC, lbl_eu_80668F00);
            }
        }
        rec->field_1C += delta;
        if (rec->field_1C >= lbl_eu_80668F04) {
            rec->field_1C = lbl_eu_80668F08; // crossed the threshold: expire
        } else {
            ret = 0;
            rec->field_14 += delta;
            if (rec->field_14 < lbl_eu_80668F0C) {
                rec->field_14 = lbl_eu_80668EF8;
                cf::CfHikariItemManager* mgr = func_802B262C();
                func_802B3568(mgr, (const f32*)rec, rec->field_40,
                              lbl_eu_80668EFC, lbl_eu_80668F00);
            }
        }
    } else {
        ret = 0;
        // Squared distance from the record position to the global anchor
        // (retail pairs off the component subtracts into ps ops).
        nw4r::math::VEC3 d;
        nw4r::math::VEC3Sub(&d, (nw4r::math::VEC3*)&g[0],
                            (nw4r::math::VEC3*)&rec->field_00);
        f32 dist2 = d.x * d.x + d.y * d.y + d.z * d.z;

        if (dist2 <= lbl_eu_80668F10) {
            // Near the anchor: drift toward it and re-derive both gradient
            // colours from the moving progress values.
            rec->field_0C += delta;
            rec->field_10 += delta;
            rec->field_30 = func_802B41E4(
                &rec->field_0C,
                (const f32*)&g[4 + (s16)rec->field_40 * 8],
                lbl_eu_80513588, 2);
            rec->field_34 = func_802B41E4(
                &rec->field_10,
                (const f32*)&g[0x14 + (s16)rec->field_40 * 8],
                lbl_eu_80513588, 2);
            rec->field_42 |= 0x100;
        } else if (!(flags & 0x100)) {
            rec->field_30 = func_802B41E4(
                &rec->field_0C,
                (const f32*)&g[4 + (s16)rec->field_40 * 8],
                lbl_eu_80513588, 2);
            rec->field_34 = func_802B41E4(
                &rec->field_10,
                (const f32*)&g[0x14 + (s16)rec->field_40 * 8],
                lbl_eu_80513588, 2);
            rec->field_42 |= 0x100;
        }

        if (countersOn) {
            rec->field_42 |= 0x3;
            lbl_eu_80664C18++;
            lbl_eu_80664C1C++;
        }

        // Accumulator: advance it, wrapping at the gradient table's second
        // threshold.  The post-store value is re-read from memory (retail
        // keeps the pre-add value in f3 for the band check below).
        f32 old18 = rec->field_18;
        rec->field_18 = old18 + delta;
        if (rec->field_18 >= lbl_eu_80513598[1]) {
            rec->field_18 = lbl_eu_80668EF8;
        }

        if (dist2 <= lbl_eu_80668F14) {
            f32 new18 = rec->field_18;
            if (new18 <= lbl_eu_80668EF8) {
                rec->field_20 = lbl_eu_80668EF8;
                rec->field_42 |= 0x10;
                rec->field_28 = lbl_eu_80668F18 +
                                lbl_eu_80668F1C * (f32)ml::math::mtRand(0, 100);
            } else if (old18 < lbl_eu_80668F20 && new18 <= lbl_eu_80668F28) {
                rec->field_24 = lbl_eu_80668EF8;
                rec->field_42 |= 0x20;
                rec->field_2C = lbl_eu_80668F18 +
                                lbl_eu_80668F1C * (f32)ml::math::mtRand(0, 100);
            }
            if ((s16)rec->field_40 == 0 && dist2 <= lbl_eu_80668F30) {
                rec->field_14 += delta;
                if (rec->field_14 >= lbl_eu_80668F04) {
                    rec->field_14 = lbl_eu_80668EF8;
                    cf::CfHikariItemManager* mgr = func_802B262C();
                    func_802B3568(mgr, (const f32*)rec, rec->field_40,
                                  lbl_eu_80668F34, lbl_eu_80668F0C);
                }
            }
        }
    }

    // Refresh the 0x38/0x3C sparkle colours for any set 0x10/0x20 flag.
    f32* fp = &rec->field_20;
    for (int i = 0; i < 2; i++) {
        if (rec->field_42 & (0x10 << i)) {
            ret = 0;
            *fp += delta;
            rec->colors[2 + i] = func_802B41E4(
                fp, (const f32*)&g[0x24 + (s16)rec->field_40 * 8],
                lbl_eu_80513598, 2);
            if (*fp == lbl_eu_80668EF8) {
                rec->field_42 &= ~(0x10 << i);
            } else if (countersOn) {
                rec->field_42 |= (0x4 << i);
                lbl_eu_80664C20++;
            }
        }
        fp++;
    }
    return ret;
}

// (us-802b6710): per-frame Hikari record update.  Clears the upper
// flag bits, re-derives the two gradient colors from field_40 when the 0x40
// flag is clear, then re-derives the 0x38/0x3C colors through the 0x10/0x20
// flag pair.  r30 mirrors the 0x80 flag (0 when set, 1 when clear) - it gates
// the sbss counters and the 0x3/0x4<<i flag bits.
extern "C" void func_802B3CA0(CfHikariItemRecord* self) {
    // Hoisted base pointers mirror the retail prologue (lis/addi up front).
    u32* g = lbl_eu_80577680;
    u16 flags = self->field_42;
    int r30 = (flags & 0x40) != 0; // spawn flag gates the sbss counters
    self->field_42 = flags & 0xFFF0; // retail mask 16,27 = clear low 4 bits

    if (!(flags & 0x40)) {
        if (!(flags & 0x100)) {
            self->field_30 = func_802B41E4(
                &self->field_0C,
                (const f32*)&g[4 + (s16)self->field_40 * 8],
                lbl_eu_80513588, 2);
            self->field_34 = func_802B41E4(
                &self->field_10,
                (const f32*)&g[0x14 + (s16)self->field_40 * 8],
                lbl_eu_80513588, 2);
            self->field_42 |= 0x100;
        }
        if (r30 != 0) {
            self->field_42 |= 0x3;
            lbl_eu_80664C18++;
            lbl_eu_80664C1C++;
        }
    }

    // Tail loop walks src/dst pointers (retail r29/r28 bump by 4).
    f32* src = &self->field_20;
    const f32* rows = (const f32*)&g[0x24];
    for (int i = 0; i < 2; i++) {
        if (self->field_42 & (0x10 << i)) {
            self->colors[2 + i] = func_802B41E4(
                src,
                rows + (s16)self->field_40 * 8,
                lbl_eu_80513598, 2);
            if (r30 != 0) {
                self->field_42 = (u16)(self->field_42 | (0x4 << i));
                lbl_eu_80664C20++;
            }
        }
        src += 1;
        rows += 4;
    }
}

// (us-802b6874): emit a Hikari quad through the GX FIFO - four
// vertices at self.pos + corner offset, with the 0x30 color word and the
// (1,0)/(1,1)/(0,1)/(0,0) byte pair.  Runs only while flag bit 0 is set.
// Per-vertex sums go into named locals first (same shape as func_802B3F20)
// so the volatile FIFO stores don't perturb the load/fadd schedule.
extern "C" void func_802B3E04(CfHikariItemRecord* self, const CfHikariQuadCorners* corners) {
    if ((self->field_42 & 1) == 0) {
        return;
    }

    f32 z0 = self->field_08f + corners->v[0][2];
    f32 x0 = self->field_00f + corners->v[0][0];
    f32 y0 = self->field_04f + corners->v[0][1];
    WGPIPE.f = x0;
    WGPIPE.f = y0;
    WGPIPE.f = z0;
    WGPIPE.ui = self->field_30;
    WGPIPE.uc = 1;
    WGPIPE.uc = 0;

    f32 x1 = self->field_00f + corners->v[1][0];
    f32 y1 = self->field_04f + corners->v[1][1];
    f32 z1 = self->field_08f + corners->v[1][2];
    WGPIPE.f = x1;
    WGPIPE.f = y1;
    WGPIPE.f = z1;
    WGPIPE.ui = self->field_30;
    WGPIPE.uc = 1;
    WGPIPE.uc = 1;

    f32 x2 = self->field_00f + corners->v[2][0];
    f32 z2 = self->field_08f + corners->v[2][2];
    f32 y2 = self->field_04f + corners->v[2][1];
    WGPIPE.f = x2;
    WGPIPE.f = y2;
    WGPIPE.f = z2;
    WGPIPE.ui = self->field_30;
    WGPIPE.uc = 0;
    WGPIPE.uc = 1;

    f32 x3 = self->field_00f + corners->v[3][0];
    f32 y3 = self->field_04f + corners->v[3][1];
    f32 z3 = self->field_08f + corners->v[3][2];
    WGPIPE.f = x3;
    WGPIPE.f = y3;
    WGPIPE.f = z3;
    WGPIPE.ui = self->field_30;
    WGPIPE.uc = 0;
    WGPIPE.uc = 0;
}

// (us-802b6990): same quad emitter as func_802B3E04 but gated by
// flag bit 1 and using the 0x34 color word.
extern "C" void func_802B3F20(CfHikariItemRecord* self, const CfHikariQuadCorners* corners) {
    if ((self->field_42 & 2) == 0) {
        return;
    }

    f32 x0 = self->field_00f + corners->v[0][0];
    f32 y0 = self->field_04f + corners->v[0][1];
    f32 z0 = corners->v[0][2] + self->field_08f;
    WGPIPE.f = x0;
    WGPIPE.f = y0;
    WGPIPE.f = z0;
    WGPIPE.ui = self->field_34;
    WGPIPE.uc = 1;
    WGPIPE.uc = 0;

    f32 x1 = self->field_00f + corners->v[1][0];
    f32 y1 = self->field_04f + corners->v[1][1];
    f32 z1 = self->field_08f + corners->v[1][2];
    WGPIPE.f = x1;
    WGPIPE.f = y1;
    WGPIPE.f = z1;
    WGPIPE.ui = self->field_34;
    WGPIPE.uc = 1;
    WGPIPE.uc = 1;

    f32 x2 = self->field_00f + corners->v[2][0];
    f32 y2 = self->field_04f + corners->v[2][1];
    f32 z2 = self->field_08f + corners->v[2][2];
    WGPIPE.f = x2;
    WGPIPE.f = y2;
    WGPIPE.f = z2;
    WGPIPE.ui = self->field_34;
    WGPIPE.uc = 0;
    WGPIPE.uc = 1;

    f32 x3 = self->field_00f + corners->v[3][0];
    f32 y3 = self->field_04f + corners->v[3][1];
    f32 z3 = self->field_08f + corners->v[3][2];
    WGPIPE.f = x3;
    WGPIPE.f = y3;
    WGPIPE.f = z3;
    WGPIPE.ui = self->field_34;
    WGPIPE.uc = 0;
    WGPIPE.uc = 0;
}

// (us-802b6aac): sparkle-streak emitter for the C20 draw pass.  For each
// of the two sparkle slots (flag bits 0x4/0x8) derives a size from the
// slot's progress value at +0x20/+0x24, offsets the single corner offset
// symmetrically around the record position (retail vectorises the +/- into
// paired-single ops on stacked VEC3s), then emits one quad stretched
// upward: bottom vertices get the +0x28/+0x2C brightness added to y, top
// vertices the -0.3 constant.
extern "C" void func_802B403C(CfHikariItemRecord* self,
                              const CfHikariQuadCorners* corners) {
    if ((self->field_42 & 0xC) == 0) {
        return;
    }

    for (int i = 0; i < 2; i++) {
        if (!(self->field_42 & (0x4 << i))) {
            continue;
        }
        f32 k = lbl_eu_80668F40 * (&self->field_20)[i];
        f32 s = lbl_eu_80668F44 *
                (lbl_eu_80668F48 - lbl_eu_80668F4C * k);
        f32 bright = k + (&self->field_28)[i];
        f32 px = self->field_00f;
        f32 py = self->field_04f;
        f32 pz = self->field_08f;

        nw4r::math::VEC3 va;
        nw4r::math::VEC3 vb;
        // Scaled corner folded into va/vb in place (retail reuses the temp
        // slots for the +/- results); each product computed independently.
        va.x = corners->v[0][0] * s;
        va.y = corners->v[0][1] * s;
        va.z = corners->v[0][2] * s;
        nw4r::math::VEC3Add(&va, (nw4r::math::VEC3*)&px, &va);

        vb.x = corners->v[0][0] * s;
        vb.y = corners->v[0][1] * s;
        vb.z = corners->v[0][0] * s; // retail quirk: z uses the x-scaled term
        nw4r::math::VEC3Sub(&vb, (nw4r::math::VEC3*)&px, &vb);

        WGPIPE.f = va.x;
        WGPIPE.f = va.y + bright;
        WGPIPE.f = va.z;
        WGPIPE.ui = self->colors[2 + i];
        WGPIPE.uc = 1;
        WGPIPE.uc = 0;

        WGPIPE.f = va.x;
        WGPIPE.f = lbl_eu_80668F50 + va.y;
        WGPIPE.f = va.z;
        WGPIPE.ui = self->colors[2 + i];
        WGPIPE.uc = 1;
        WGPIPE.uc = 1;

        WGPIPE.f = vb.x;
        WGPIPE.f = lbl_eu_80668F50 + vb.y;
        WGPIPE.f = vb.z;
        WGPIPE.ui = self->colors[2 + i];
        WGPIPE.uc = 0;
        WGPIPE.uc = 1;

        WGPIPE.f = vb.x;
        WGPIPE.f = vb.y + bright;
        WGPIPE.f = vb.z;
        WGPIPE.ui = self->colors[2 + i];
        WGPIPE.uc = 0;
        WGPIPE.uc = 0;
    }
}

// (us-802b6c54): gradient color lookup.  self[0] is a progress value;
// table holds count thresholds followed by count blend weights, rows holds
// count 4-float (A,R,G,B) rows.  Past the last threshold the value wraps to 0
// and row 0 is returned; otherwise the row pair bracketing the value is
// linearly blended.  The packed result is B | (G<<8) | (R<<16) | (A<<24).
// The threshold scan uses a walking pointer (retail mr/addi on the table base)
// while the weight lookup re-indexes from the base (lfsx) - keep both forms.
// noinline: retail always calls this (never inlines it into the quad
// emitters); leaving it inlineable perturbs the callers' register allocation.
extern "C" __declspec(noinline) u32 func_802B41E4(f32* self, const f32* rows, const f32* table, int count) {
    f32 out[4];
    // End-pointer threshold check (retail add/lfs -4(r7)).
    const f32* end = table + count;

    if (self[0] >= end[-1]) {
        self[0] = lbl_eu_80668EF8;
        out[0] = rows[0];
        out[1] = rows[1];
        out[2] = rows[2];
        out[3] = rows[3];
    } else {
        const f32* t = table;
        f32 prev = lbl_eu_80668EF8; // zero constant loaded via lfs like retail
        for (int i = 0; i < count; i++) {
            if (self[0] <= *t) {
                f32 inv = lbl_eu_80668F54 - (self[0] - prev) * table[i + count];
                f32 blend = (self[0] - prev) * table[i + count];
                int next = (i + 1) % count;
                out[0] = inv * rows[i * 4 + 0] + blend * rows[next * 4 + 0];
                out[1] = inv * rows[i * 4 + 1] + blend * rows[next * 4 + 1];
                out[2] = inv * rows[i * 4 + 2] + blend * rows[next * 4 + 2];
                out[3] = inv * rows[i * 4 + 3] + blend * rows[next * 4 + 3];
                break;
            }
            prev = *t;
            t++;
        }
    }

    return (u32)(s32)out[3] | ((u32)(s32)out[2] << 8) | ((u32)(s32)out[1] << 16) |
           ((u32)(s32)out[0] << 24);
}

// (us-802b6dc8): (re)initialise a Hikari record's motion: copy the
// spawn position words, write the (0, val) halfword pair at +0x1C, then set
// random velocity components scaled by the sdata2 constants (and the f1 arg
// for the +0x10 axis) with a zeroed +0x18 accumulator.  Field 0x14 is written
// before 0x18 so MWCC schedules the last mtRand call ahead of the zero store
// (retail order: call, conversion, 0x18 store, 0x14 store).
// noinline: retail calls this (bl from func_802B3568 / func_802B3810); leaving
// it inlineable makes MWCC absorb it into the spawn caller.
extern "C" __declspec(noinline) void func_802B4358(CfHikariItemRecord* self, const u32* src, u16 val, f32 scale) {
    self->field_1C_h[0] = 0;
    self->field_1C_h[1] = val;
    self->field_00 = src[0];
    self->field_04 = src[1];
    self->field_08 = src[2];

    self->field_0C = (f32)ml::math::mtRand(-10, 10) * lbl_eu_80668F58;
    self->field_10 = (f32)ml::math::mtRand(3, 8) * lbl_eu_80668F5C * scale;
    self->field_14 = lbl_eu_80668F58 * (f32)ml::math::mtRand(-10, 10);
    self->field_18 = lbl_eu_80668EF8;
}

extern "C" void func_802B4460(CfHikariItemRecord* self) {
    extern unsigned long lbl_eu_80664C24;
    ++lbl_eu_80664C24;
}

// (us-802b6ee0): accumulate delta into +0x18; once it reaches the
// sdata2 limit return 1, otherwise advance the +0x00 vector by the +0x0C
// vector (retail paired-single via the nw4r VEC3Add inline), bump the sbss
// counter and return 0.
extern "C" __declspec(noinline) s32 func_802B4470(CfHikariItemRecord* self,
                                                   f32 delta) {
    f32 acc = self->field_18 + delta;
    self->field_18 = acc;
    if (acc >= lbl_eu_80668F60) {
        return 1;
    }
    nw4r::math::VEC3Add((nw4r::math::VEC3*)&self->field_00,
                        (nw4r::math::VEC3*)&self->field_00,
                        (nw4r::math::VEC3*)&self->field_0C);
    ++lbl_eu_80664C24;
    return 0;
}

// (us-802b6f38): gradient-driven quad emitter.  Derives a scale from
// the +0x18 progress value against the table thresholds, scales each corner
// offset by it, then pushes four vertices (pos + scaled corner) with the
// func_802B41E4 colour and the (1,0)/(1,1)/(0,1)/(0,0) byte pair.
extern "C" void func_802B44C8(CfHikariItemRecord* self, const CfHikariQuadCorners* corners) {
    u32 color = func_802B41E4(
        &self->field_18,
        (const f32*)&lbl_eu_80577750[(s16)self->field_1C_h[1] * 8],
        lbl_eu_805135A8, 2);

    // Gradient-driven size: linear ramp up to table[0], ramp down after.
    f32 progress = self->field_18;
    f32 scale;
    if (progress < lbl_eu_805135A8[0]) {
        scale = lbl_eu_80668F64 + lbl_eu_80668F68 * progress * lbl_eu_805135A8[2];
    } else {
        scale = lbl_eu_80668F6C -
                lbl_eu_805135A8[3] * (lbl_eu_80668F68 * (progress - lbl_eu_805135A8[0]));
    }

    // Scaled corner offsets (paired (x,y) first, z second - MWCC vectorises
    // the pairs into psq_l/ps_muls0, one per corner, like retail).
    CfHikariQuadCorners* c = (CfHikariQuadCorners*)corners;
    f32 sc[4][3];
    sc[0][0] = c->v[0][0] * scale;
    sc[0][1] = c->v[0][1] * scale;
    sc[0][2] = c->v[0][2] * scale;
    sc[1][0] = c->v[1][0] * scale;
    sc[1][1] = c->v[1][1] * scale;
    sc[1][2] = c->v[1][2] * scale;
    sc[2][0] = c->v[2][0] * scale;
    sc[2][1] = c->v[2][1] * scale;
    sc[2][2] = c->v[2][2] * scale;
    sc[3][0] = c->v[3][0] * scale;
    sc[3][1] = c->v[3][1] * scale;
    sc[3][2] = c->v[3][2] * scale;

    f32 x, y, z;
    x = self->field_00f + sc[0][0];
    y = self->field_04f + sc[0][1];
    z = self->field_08f + sc[0][2];
    WGPIPE.f = x;
    WGPIPE.f = y;
    WGPIPE.f = z;
    WGPIPE.ui = color;
    WGPIPE.uc = 1;
    WGPIPE.uc = 0;

    x = self->field_00f + sc[1][0];
    y = self->field_04f + sc[1][1];
    z = self->field_08f + sc[1][2];
    WGPIPE.f = x;
    WGPIPE.f = y;
    WGPIPE.f = z;
    WGPIPE.ui = color;
    WGPIPE.uc = 1;
    WGPIPE.uc = 1;

    WGPIPE.f = self->field_00f + sc[2][0];
    WGPIPE.f = self->field_04f + sc[2][1];
    WGPIPE.f = self->field_08f + sc[2][2];
    WGPIPE.ui = color;
    WGPIPE.uc = 0;
    WGPIPE.uc = 1;

    x = self->field_00f + sc[3][0];
    y = self->field_04f + sc[3][1];
    z = self->field_08f + sc[3][2];
    WGPIPE.f = x;
    WGPIPE.f = y;
    WGPIPE.f = z;
    WGPIPE.ui = color;
    WGPIPE.uc = 0;
    WGPIPE.uc = 0;
}

// (us-802b7150): static-init the .bss gradient table.  Four
// 16-float rows at +0x10/+0x50/+0x90/+0xd0 of lbl_eu_80577680 (func_802B3CA0
// reads the first three rows indexed by field_40).  Constants are read
// directly from sdata2 so MWCC CSEs one lfs per value up-front and colours
// them exactly like retail (f27-f31 for {F70,F74,F78,F7C,F80}, volatiles
// f13..f0 descending by first use).  Row bases are declared in the retail
// addi order; element 0 of each row goes through the block base.
void sinit_802B46E0() {
    f32* t = (f32*)lbl_eu_80577680;

    f32* r5 = t + 0x50 / 4;
    f32* r3 = t + 0xD0 / 4;
    f32* r6 = t + 0x10 / 4;
    f32* r4 = t + 0x90 / 4;

    t[0x10 / 4] = lbl_eu_80668F70;
    r6[1] = lbl_eu_80668F74;
    r6[2] = lbl_eu_80668F78;
    r6[3] = lbl_eu_80668F7C;
    r6[4] = lbl_eu_80668F70;
    r6[5] = lbl_eu_80668F80;
    r6[6] = lbl_eu_80668F84;
    r6[7] = lbl_eu_80668F7C;
    r6[8] = lbl_eu_80668F88;
    r6[9] = lbl_eu_80668F8C;
    r6[10] = lbl_eu_80668F70;
    r6[11] = lbl_eu_80668F90;
    r6[12] = lbl_eu_80668F94;
    r6[13] = lbl_eu_80668F98;
    r6[14] = lbl_eu_80668F70;
    r6[15] = lbl_eu_80668F90;

    t[0x50 / 4] = lbl_eu_80668F70;
    r5[1] = lbl_eu_80668F9C;
    r5[2] = lbl_eu_80668F98;
    r5[3] = lbl_eu_80668F70;
    r5[4] = lbl_eu_80668F70;
    r5[5] = lbl_eu_80668FA0;
    r5[6] = lbl_eu_80668FA4;
    r5[7] = lbl_eu_80668F70;
    r5[8] = lbl_eu_80668FA8;
    r5[9] = lbl_eu_80668F70;
    r5[10] = lbl_eu_80668F70;
    r5[11] = lbl_eu_80668F70;
    r5[12] = lbl_eu_80668F98;
    r5[13] = lbl_eu_80668F70;
    r5[14] = lbl_eu_80668F70;
    r5[15] = lbl_eu_80668F70;

    t[0x90 / 4] = lbl_eu_80668F70;
    r4[1] = lbl_eu_80668F74;
    r4[2] = lbl_eu_80668F78;
    r4[3] = lbl_eu_80668EF8;
    r4[4] = lbl_eu_80668F70;
    r4[5] = lbl_eu_80668F74;
    r4[6] = lbl_eu_80668F78;
    r4[7] = lbl_eu_80668FAC;
    r4[8] = lbl_eu_80668F88;
    r4[9] = lbl_eu_80668F8C;
    r4[10] = lbl_eu_80668F70;
    r4[11] = lbl_eu_80668EF8;
    r4[12] = lbl_eu_80668F88;
    r4[13] = lbl_eu_80668F8C;
    r4[14] = lbl_eu_80668F70;
    r4[15] = lbl_eu_80668FAC;

    t[0xD0 / 4] = lbl_eu_80668F70;
    r3[1] = lbl_eu_80668FB0;
    r3[2] = lbl_eu_80668F90;
    r3[3] = lbl_eu_80668EF8;
    r3[4] = lbl_eu_80668F70;
    r3[5] = lbl_eu_80668FA0;
    r3[6] = lbl_eu_80668FA4;
    r3[7] = lbl_eu_80668F70;
    r3[8] = lbl_eu_80668F88;
    r3[9] = lbl_eu_80668FB4;
    r3[10] = lbl_eu_80668F70;
    r3[11] = lbl_eu_80668EF8;
    r3[12] = lbl_eu_80668F98;
    r3[13] = lbl_eu_80668FAC;
    r3[14] = lbl_eu_80668F70;
    r3[15] = lbl_eu_80668F70;
}
