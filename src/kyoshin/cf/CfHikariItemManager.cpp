// Auto-scaffolded catalog TU for kyoshin/cf/CfHikariItemManager
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

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

// Target 1 (us-802b50a4): constructor.  Manual vptr store, registers the
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

// Target 4 (us-802b523c): deleting destructor.  Manual vptr store (retail
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

// Target 3 (us-802b5304): allocate a 0x44-byte Hikari item record, initialize
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

// Target 5 (us-802b53a8): remove `target` from the manager's record array.
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
// Target 2 (us-802b5488): reset the manager - delete every record, clear the
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

// Target 2 (us-802b5528): toggle flag bit 0 of +0x1194 - nonzero arg clears
// the bit, zero sets it.
void func_802B2AB8(CfHikariItemRecord* self, u32 enable) {
    if (enable != 0) {
        self->field_1194 &= ~1u;
    } else {
        self->field_1194 |= 1u;
    }
}

// Target 2 (us-802b5550): render-callback body.  Bails when there is no
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
    CfMtxCopy invMtx = viewFrame->mtx;
    PSMTXInverse(invMtx.m, invMtx.m);
    GXSetCurrentMtx(0);
    GXLoadPosMtxImm(invMtx.m, 0);

    // Second copy with the translation column zeroed: the two camera axes
    // (x/y rows of the rotation part) are transformed by it, then normalized.
    CfMtxCopy mtx2 = viewFrame->mtx;
    mtx2.m[0][3] = lbl_eu_80668EC8;
    mtx2.m[1][3] = lbl_eu_80668EC8;
    mtx2.m[2][3] = lbl_eu_80668EC8;
    ml::CVec3 vec1;
    ml::CVec3 vec2;
    vec1.x = lbl_eu_80668ED0;
    vec1.y = lbl_eu_80668EC8;
    vec1.z = lbl_eu_80668EC8;
    vec2.x = lbl_eu_80668EC8;
    vec2.y = lbl_eu_80668ED0;
    vec2.z = lbl_eu_80668EC8;
    PSMTXMultVec(mtx2.m, vec1, vec1);
    PSMTXMultVec(mtx2.m, vec2, vec2);

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
    GXColor matColor;
    GXColor ambColor;
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

    // ------------------------------------------------------------------
    // Draw passes.  Each quad corner is a combination of the two normalized
    // camera axes scaled by the pass-specific constants (retail vectorises
    // the x/y pairs into ps_muls0/ps_add runs).
    // ------------------------------------------------------------------
    CfHikariQuadCorners corners;
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

// Target 3 (us-802b5fd8): claim the next slot in the inline 0x80-entry
// record pool (bitfield at +0x1104, 0x20-byte entries at +0x104), bail if it
// is already active, else perturb the spawn position by scaled random offsets
// (each component gets its own random draw) and init the entry via func_802B4358.
extern "C" void func_802B3568(cf::CfHikariItemManager* self, const f32* src,
                              u16 val, f32 scaleX, f32 scaleY) {
    u32 count = self->field_119C;
    u32* words = (u32*)&self->unk1104[0];
    u32 word = words[count >> 5];
    u32 bit = 1u << (count & 0x1F);
    if (word & bit) {
        return;
    }
    words[count >> 5] = word | bit;

    // Stack shapes mirror retail: the spawn position is copied to +0x14
    // before the first mtRand, the three randoms are converted through the
    // shared sdata2 magic double and scaled (+0x08..+0x10), then folded back
    // onto the copy.
    f32 out[3];
    f32 rnd[3];
    out[0] = src[0];
    out[1] = src[1];
    out[2] = src[2];

    rnd[0] = (f32)ml::math::mtRand(-100, 100) * lbl_eu_80668EEC;
    rnd[1] = (f32)ml::math::mtRand(-100, 100) * lbl_eu_80668EEC;
    rnd[2] = (f32)ml::math::mtRand(-100, 100) * lbl_eu_80668EEC;

    out[0] += rnd[0] * scaleX;
    out[1] += rnd[1] * scaleX;
    out[2] += rnd[2] * scaleX;

    func_802B4358((CfHikariItemRecord*)&self->unk104[count * 0x20],
                  (const u32*)out, val, scaleY);

    count++;
    if (count >= 0x80) {
        count = 0;
    }
    self->field_119C = count;
}

// Target 3 (us-802b618c): publish the record's first three words into the
// .bss global state block and reset the four sbss counters (retail emits
// four SDA-relocated word stores to lbl_eu_80664C18..24).
// Target 3 (us-802b618c): publish the record's first three words into the
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

// Target 1 (us-802b61c0): initialize a freshly allocated 0x44-byte Hikari
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

// Target 4 (us-802b6224): destructor for the unknown class at 0x802B37B4
// (address-suffixed retail symbol).
extern "C" __declspec(noinline) void* __dt__802B37B4(void* self, int flag) {
    if (self != 0 && flag > 0) {
        ::operator delete(self);
    }
    return self;
}

// Target 1 (us-802b6264): flag the u16 at +0x42 with 0x40 and write the
// sdata2 float constant to +0x1C and +0x14.
extern "C" void func_802B37F4(CfHikariItemRecord* self) {
    u16 val = self->field_42 | 0x40;    // lhz, ori
    self->field_42 = val;               // sth
    f32 f = lbl_eu_80668EF8;            // lfs
    self->field_1C = f;                 // stfs 0x1c
    self->field_14 = f;                 // stfs 0x14
}

// Target 1 (us-802b6280): per-frame update of an active Hikari record.  Clears
// the upper flag bits (keeping the 0x40 spawn flag test on the original word),
// then branches on the 0x40 flag: the spawn path advances the 0x1C/0x14
// timers (bursting 8 particles when 0x1C hits zero) and returns 1 when 0x1C
// crosses its 10-frame threshold (caller removes the record); the main path
// re-derives the two gradient colours from the global anchor when the record
// is within 2500 units (moving it toward the anchor), bumps the sbss counters
// when the 0x80 flag is clear, advances the 0x18 accumulator and spawns
// sparkle offsets, then refreshes the 0x38/0x3C colours through the
// 0x10/0x20 flag pair.  Returns 0 while the record stays alive.
extern "C" s32 func_802B3810(CfHikariItemRecord* self, f32 delta) {
    u16 flags = self->field_42;
    const f32* base = (const f32*)lbl_eu_80577680; // hoisted lis/addi (r29)
    int r28 = (flags & 0x80) ? 0 : 1; // mirrors the 0x80 flag (0 when set)
    self->field_42 = flags & 0xFFF0;   // clears the low 4 bits (retail mask 16,27)
    s32 r25 = 1;

    if (flags & 0x40) {
        // Spawn trail: burst a fan of 8 particles when the 0x1C timer resets.
        if (self->field_1C == lbl_eu_80668EF8) {
            for (int i = 0; i < 8; i++) {
                func_802B3568(func_802B262C(), (const f32*)self, (s16)self->field_40,
                              lbl_eu_80668EFC, lbl_eu_80668F00);
            }
        }
        self->field_1C += delta;
        if (self->field_1C >= lbl_eu_80668F04) {
            self->field_1C = lbl_eu_80668F08; // crossed the threshold: expire
        } else {
            r25 = 0;
            self->field_14 += delta;
            if (self->field_14 < lbl_eu_80668F0C) {
                self->field_14 = lbl_eu_80668EF8;
                func_802B3568(func_802B262C(), (const f32*)self, (s16)self->field_40,
                              lbl_eu_80668EFC, lbl_eu_80668F00);
            }
        }
    } else {
        r25 = 0;
        // Squared distance from the record position to the global anchor.
        f32 d[3];
        d[0] = base[0] - self->field_00f;
        d[1] = base[1] - self->field_04f;
        d[2] = base[2] - self->field_08f;
        f32 dist2 = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];

        if (dist2 < lbl_eu_80668F10) {
            // Near the anchor: drift toward it and re-derive both gradient
            // colours from the moving progress values.
            self->field_0C += delta;
            self->field_10 += delta;
            self->field_30 = func_802B41E4(
                &self->field_0C,
                &base[4 + (s16)self->field_40 * 8],
                lbl_eu_80513588, 2);
            self->field_34 = func_802B41E4(
                &self->field_10,
                &base[0x14 + (s16)self->field_40 * 8],
                lbl_eu_80513588, 2);
            self->field_42 |= 0x100;
        } else if (!(flags & 0x100)) {
            self->field_30 = func_802B41E4(
                &self->field_0C,
                &base[4 + (s16)self->field_40 * 8],
                lbl_eu_80513588, 2);
            self->field_34 = func_802B41E4(
                &self->field_10,
                &base[0x14 + (s16)self->field_40 * 8],
                lbl_eu_80513588, 2);
            self->field_42 |= 0x100;
        }

        if (r28 != 0) {
            self->field_42 |= 0x3;
            lbl_eu_80664C18++;
            lbl_eu_80664C1C++;
        }

        // Accumulator: advance it, wrapping at the gradient table's second
        // threshold.  The pre-accumulation value stays live for the 20-unit
        // band check below (retail keeps it in f3 across the store).
        f32 old18 = self->field_18;
        self->field_18 = old18 + delta;
        if (self->field_18 >= lbl_eu_80513598[1]) {
            self->field_18 = lbl_eu_80668EF8;
        }

        if (dist2 < lbl_eu_80668F14) {
            if (self->field_18 <= lbl_eu_80668EF8) {
                self->field_20 = lbl_eu_80668EF8;
                self->field_42 |= 0x10;
                self->field_28 = lbl_eu_80668F18 +
                                 lbl_eu_80668F1C * (f32)ml::math::mtRand(0, 100);
            } else if (old18 < lbl_eu_80668F20 && self->field_18 >= lbl_eu_80668F28) {
                self->field_24 = lbl_eu_80668EF8;
                self->field_42 |= 0x20;
                self->field_2C = lbl_eu_80668F18 +
                                 lbl_eu_80668F1C * (f32)ml::math::mtRand(0, 100);
            }
            if (self->field_40 == 0 && dist2 < lbl_eu_80668F30) {
                self->field_14 += delta;
                if (self->field_14 >= lbl_eu_80668F04) {
                    self->field_14 = lbl_eu_80668EF8;
                    func_802B3568(func_802B262C(), (const f32*)self,
                                  (s16)self->field_40, lbl_eu_80668F34,
                                  lbl_eu_80668F0C);
                }
            }
        }
    }

    // Refresh the 0x38/0x3C sparkle colours for any set 0x10/0x20 flag.
    f32* fptr = &self->field_20;
    for (int i = 0; i < 2; i++) {
        if (self->field_42 & (0x10 << i)) {
            r25 = 0;
            *fptr += delta;
            self->colors[2 + i] = func_802B41E4(
                fptr, &base[0x24 + (s16)self->field_40 * 8],
                lbl_eu_80513598, 2);
            if (*fptr != lbl_eu_80668EF8) {
                if (r28 != 0) {
                    self->field_42 |= (0x4 << i);
                    lbl_eu_80664C20++;
                }
            } else {
                self->field_42 &= ~(0x10 << i);
            }
        }
        fptr++;
    }
    return r25;
}

// Target 4 (us-802b6710): per-frame Hikari record update.  Clears the upper
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

// Target 2 (us-802b6874): emit a Hikari quad through the GX FIFO - four
// vertices at self.pos + corner offset, with the 0x30 color word and the
// (1,0)/(1,1)/(0,1)/(0,0) byte pair.  Runs only while flag bit 0 is set.
extern "C" void func_802B3E04(CfHikariItemRecord* self, const CfHikariQuadCorners* corners) {
    if ((self->field_42 & 1) == 0) {
        return;
    }

    // Direct per-vertex expressions: MWCC floats the corner loads early and
    // keeps each sum in a scratch FPR, matching the retail lfs/fadds/stfs run.
    // Vertex layout: xyz, color word, then the (s,t) uv byte pair.
    WGPIPE.f = self->field_00f + corners->v[0][0];
    WGPIPE.f = self->field_04f + corners->v[0][1];
    WGPIPE.f = self->field_08f + corners->v[0][2];
    WGPIPE.ui = self->field_30;
    WGPIPE.uc = 1;
    WGPIPE.uc = 0;

    WGPIPE.f = self->field_00f + corners->v[1][0];
    WGPIPE.f = self->field_04f + corners->v[1][1];
    WGPIPE.f = self->field_08f + corners->v[1][2];
    WGPIPE.ui = self->field_30;
    WGPIPE.uc = 1;
    WGPIPE.uc = 1;

    WGPIPE.f = self->field_00f + corners->v[2][0];
    WGPIPE.f = self->field_04f + corners->v[2][1];
    WGPIPE.f = self->field_08f + corners->v[2][2];
    WGPIPE.ui = self->field_30;
    WGPIPE.uc = 0;
    WGPIPE.uc = 1;

    WGPIPE.f = self->field_00f + corners->v[3][0];
    WGPIPE.f = self->field_04f + corners->v[3][1];
    WGPIPE.f = self->field_08f + corners->v[3][2];
    WGPIPE.ui = self->field_30;
    WGPIPE.uc = 0;
    WGPIPE.uc = 0;
}

// Target 3 (us-802b6990): same quad emitter as func_802B3E04 but gated by
// flag bit 1 and using the 0x34 color word.
extern "C" void func_802B3F20(CfHikariItemRecord* self, const CfHikariQuadCorners* corners) {
    if ((self->field_42 & 2) == 0) {
        return;
    }

    f32 x0 = self->field_00f + corners->v[0][0];
    f32 y0 = self->field_04f + corners->v[0][1];
    f32 z0 = self->field_08f + corners->v[0][2];
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

extern "C" void func_802B403C(CfHikariItemRecord* self,
                              const CfHikariQuadCorners* corners) {}

// Target 5 (us-802b6c54): gradient color lookup.  self[0] is a progress value;
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

// Target 1 (us-802b6dc8): (re)initialise a Hikari record's motion: copy the
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

// Target 5 (us-802b6ee0): accumulate delta into +0x18; once it reaches the
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

// Target 5 (us-802b6f38): gradient-driven quad emitter.  Derives a scale from
// the +0x18 progress value against the table thresholds, scales each corner
// offset by it, then pushes four vertices (pos + scaled corner) with the
// func_802B41E4 colour and the (1,0)/(1,1)/(0,1)/(0,0) byte pair.
extern "C" void func_802B44C8(CfHikariItemRecord* self, const CfHikariQuadCorners* corners) {
    u32 color = func_802B41E4(
        &self->field_18,
        (const f32*)&lbl_eu_80577750[(s16)self->field_1C_h[1] * 8],
        lbl_eu_805135A8, 2);

    f32 progress = self->field_18;
    f32 scale;
    if (progress < lbl_eu_805135A8[0]) {
        scale = lbl_eu_80668F64 + lbl_eu_80668F68 * progress * lbl_eu_805135A8[2];
    } else {
        scale = lbl_eu_80668F6C -
                lbl_eu_805135A8[3] * (lbl_eu_80668F68 * (progress - lbl_eu_805135A8[0]));
    }

    // Scaled corner offsets (paired (x,y) first, z second - MWCC vectorises
    // the pairs into psq_l/ps_muls0, one per corner, like retail).  Access
    // through a non-const view so MWCC does not hoist the corner loads ahead
    // of the func_802B41E4 call into a callee-saved FPR.
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

    x = self->field_00f + sc[2][0];
    y = self->field_04f + sc[2][1];
    z = self->field_08f + sc[2][2];
    WGPIPE.f = x;
    WGPIPE.f = y;
    WGPIPE.f = z;
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

// Target 4 (us-802b7150): static-init the .bss gradient table.  Four
// 16-float rows at +0x10/+0x50/+0x90/+0xd0 of lbl_eu_80577680 (func_802B3CA0
// reads the first three rows indexed by field_40).  Access goes through a
// 16-float-stride row pointer so MWCC materialises one addi row base per row
// (retail r6/r5/r4/r3) instead of folding everything into block-relative
// displacements.
void sinit_802B46E0() {
    f32 (*rows)[16] = (f32 (*)[16])&lbl_eu_80577680[0x10 / 4];

    rows[0][0] = lbl_eu_80668F70;
    rows[0][1] = lbl_eu_80668F74;
    rows[0][2] = lbl_eu_80668F78;
    rows[0][3] = lbl_eu_80668F7C;
    rows[0][4] = lbl_eu_80668F70;
    rows[0][5] = lbl_eu_80668F80;
    rows[0][6] = lbl_eu_80668F84;
    rows[0][7] = lbl_eu_80668F7C;
    rows[0][8] = lbl_eu_80668F88;
    rows[0][9] = lbl_eu_80668F8C;
    rows[0][10] = lbl_eu_80668F70;
    rows[0][11] = lbl_eu_80668F90;
    rows[0][12] = lbl_eu_80668F94;
    rows[0][13] = lbl_eu_80668F98;
    rows[0][14] = lbl_eu_80668F70;
    rows[0][15] = lbl_eu_80668F90;

    rows[1][0] = lbl_eu_80668F70;
    rows[1][1] = lbl_eu_80668F9C;
    rows[1][2] = lbl_eu_80668F98;
    rows[1][3] = lbl_eu_80668F70;
    rows[1][4] = lbl_eu_80668F70;
    rows[1][5] = lbl_eu_80668FA0;
    rows[1][6] = lbl_eu_80668FA4;
    rows[1][7] = lbl_eu_80668F70;
    rows[1][8] = lbl_eu_80668FA8;
    rows[1][9] = lbl_eu_80668F70;
    rows[1][10] = lbl_eu_80668F70;
    rows[1][11] = lbl_eu_80668F70;
    rows[1][12] = lbl_eu_80668F98;
    rows[1][13] = lbl_eu_80668F70;
    rows[1][14] = lbl_eu_80668F70;
    rows[1][15] = lbl_eu_80668F70;

    rows[2][0] = lbl_eu_80668F70;
    rows[2][1] = lbl_eu_80668F74;
    rows[2][2] = lbl_eu_80668F78;
    rows[2][3] = lbl_eu_80668EF8;
    rows[2][4] = lbl_eu_80668F70;
    rows[2][5] = lbl_eu_80668F74;
    rows[2][6] = lbl_eu_80668F78;
    rows[2][7] = lbl_eu_80668FAC;
    rows[2][8] = lbl_eu_80668F88;
    rows[2][9] = lbl_eu_80668F8C;
    rows[2][10] = lbl_eu_80668F70;
    rows[2][11] = lbl_eu_80668EF8;
    rows[2][12] = lbl_eu_80668F88;
    rows[2][13] = lbl_eu_80668F8C;
    rows[2][14] = lbl_eu_80668F70;
    rows[2][15] = lbl_eu_80668FAC;

    rows[3][0] = lbl_eu_80668F70;
    rows[3][1] = lbl_eu_80668FB0;
    rows[3][2] = lbl_eu_80668F90;
    rows[3][3] = lbl_eu_80668EF8;
    rows[3][4] = lbl_eu_80668F70;
    rows[3][5] = lbl_eu_80668FA0;
    rows[3][6] = lbl_eu_80668FA4;
    rows[3][7] = lbl_eu_80668F70;
    rows[3][8] = lbl_eu_80668F88;
    rows[3][9] = lbl_eu_80668FB4;
    rows[3][10] = lbl_eu_80668F70;
    rows[3][11] = lbl_eu_80668EF8;
    rows[3][12] = lbl_eu_80668F98;
    rows[3][13] = lbl_eu_80668FAC;
    rows[3][14] = lbl_eu_80668F70;
    rows[3][15] = lbl_eu_80668F70;
}
