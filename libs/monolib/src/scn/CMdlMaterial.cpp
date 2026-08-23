// Auto-scaffolded catalog TU for monolib/src/scn/CMdlMaterial
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>
#include "monolib/scn/CMdlMaterial.hpp"
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/g3d_scnmdl.h>
#include <nw4r/db/db_assert.h>
#include <revolution/GX.h>
#include <monolib/util/MemManager.hpp>

// Forward declarations for external C-ABI functions (retail unmangled names).
extern "C" void* func_80488938(void* owner, u32 size);
extern "C" void* func_80488954(void* owner, u32 size);
extern "C" u32 func_80496018(u32 handle);

// Node user-data scanner (retail unmangled symbol; declared with C linkage in
// CScnItemModelNw4r.hpp, which is not self-contained - mirrored here).
extern "C" void func_804E5990(void* self, void* node, void* mtx);

// Virtual-dispatch view of the owner object for the vtable+0x18 getter
// (CScnItemModelNw4r::v04, func_80489C94) used by the node user-data scan.
class __declspec(novtable) MdlMatOwnerVt {
public:
    virtual ~MdlMatOwnerVt();
    // Two destructor slots (0x0/0x4) + fillers; getter lands at vtable +0x18.
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual void v04() = 0;
    virtual u32 v05() = 0;   // vtable +0x18
};

// Local .sdata "ref" strings (defined at the bottom of this TU).
extern "C" u32 lbl_sd_04;
extern "C" u32 lbl_sd_05;

// Assertion strings emitted by the inlined nw4r ResUserData/ResDict accessors
// (declared here at global scope; C++ does not mangle global variable names,
// so these emit the raw lbl_* symbols).
extern const char lbl_eu_80530D18[];
extern const char lbl_eu_80530CFC[];
extern const char lbl_eu_80530CF0[];
extern const char lbl_eu_80530F08[];
extern const char lbl_eu_80530EE0[];
extern const char lbl_eu_80530D54[];
extern const char lbl_eu_80530D2C[];
extern const char lbl_eu_80530DC4[];
extern const char lbl_eu_80530DA8[];
extern const char lbl_eu_80530D68[];
extern const char lbl_eu_80530E1C[];
extern const char lbl_eu_80530DD8[];
extern const char lbl_eu_80530D94[];
extern const char lbl_eu_80530D78[];
extern const char lbl_eu_8056E43C[];
extern const char lbl_eu_8056E420[];
extern const char lbl_eu_8056E3D0[];
extern const char lbl_eu_8056E398[];
extern const char lbl_eu_8056E068[];
extern const char lbl_eu_8056E04C[];
extern const char lbl_eu_806638E8[7];   // .sdata Panic arg
extern const char lbl_eu_80663928[];
extern const char lbl_eu_80663C48[];
extern const char lbl_eu_80663C4C[];
extern const char lbl_eu_80663C50[];
extern const char lbl_eu_80663C54[];

// lbl_eu_80663C34 (.sdata) holds the char* of the node user-data key.
extern const char* lbl_eu_80663C34;

// g3d_resmat_ac.h assertion strings (inlined accessor asserts).
extern const char lbl_eu_80570100[];
extern const char lbl_eu_805700E4[];
extern const char lbl_eu_80570138[];
extern const char lbl_eu_80570110[];
extern const char lbl_eu_80570170[];
extern const char lbl_eu_80570148[];
extern const char lbl_eu_805701A0[];
extern const char lbl_eu_80570180[];

// The .sdata "ref" message strings are defined locally below as lbl_sd_04..09
// (retail labels 80663C40..80663C54); reference them through the local
// definitions so MWCC emits SDA-relative addressing.
#define lbl_eu_80663C40 ((const char*)&lbl_sd_04)
#define lbl_eu_80663C44 ((const char*)&lbl_sd_05)

// .sdata2 float constant used to scale colours (255.0f).
extern f32 lbl_eu_8066B304;

// CScnItemModelNw4r helper (retail unmangled symbol).
extern "C" void func_80488C20(void* owner, void* arg, s32 subIdx);

// ===========================================================================
// Local layout views / context structs for the helpers below.
// ===========================================================================

// Context object (CScnItemModelNw4r) fields consumed by the CMdlMaterial
// helpers: model resource pointer and scene model at fixed offsets.
struct MdlMatContext {
    /* 0x0000 */ u8 pad_0x00[0x146C];
    /* 0x146C */ nw4r::g3d::ResMdlData* mdlData;
    /* 0x147C */ nw4r::g3d::ScnMdl* scnMdl;
};

// Owner block embedding a CMdlMaterial at a fixed offset.
struct MdlMaterialOwner {
    /* 0x0000 */ u8 pad_0x00[0x16C8];
    /* 0x16C8 */ CMdlMaterial material;
};

// Ambient-colour applier state shared by the apply helpers.
struct MdlMatApplier {
    /* 0x00 */ u8 pad_0x00[4];
    /* 0x04 */ MdlMatContext* ctx;
    /* 0x08 */ GXColor* colors;     // Null disables application
};

// Mirror of CMdlMaterial with signedness matching the retail codegen of the
// helpers below (unsigned group value, byte flag table, signed entry count).
struct CMdlMatView {
    /* 0x00 */ void* vtable;
    /* 0x04 */ void* field_0x04;
    /* 0x08 */ void* buffer;
    /* 0x0C */ u32 field_0x0C;
    /* 0x10 */ u8 flag_0x10;
    /* 0x11 */ u8 pad_0x11[3];
    /* 0x14 */ u32 field_0x14;      // Initialized to -1
    /* 0x18 */ u16 field_0x18[8];   // Accumulated material values
    /* 0x28 */ u8 field_0x28[8];    // Per-material flag bytes
    /* 0x30 */ u32 field_0x30;
    /* 0x34 */ s32 field_0x34;      // Write index for field_0x18 array
};

// ===========================================================================
// nw4r g3d ResUserData reconstruction (g3d_resuser_ac.h accessor semantics)
// ===========================================================================

// One named item: name string, value type, and relative offsets.
struct ResUserDataItem {
    u32 field_0x00;      // +0x00
    s32 dataOffset;      // +0x04: relative offset to the value (0 = null)
    u32 field_0x08;      // +0x08
    u32 valueType;       // +0x0C: 0 = S32 (asserted)
    s32 nameOffset;      // +0x10: relative offset to the name string
};

// A single 0x10-byte reference slot in the dic index array.
struct ResUserDataRef {
    s32 dataOffset;      // +0x00: relative offset (from the dic base) to ResUserDataItem
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
};

// Dic accessor base: sits 4 bytes into the user-data block (ud + 4).
// numData at +0x04, index array (0x10-byte refs) at +0x24.
struct ResUserDataDic {
    u32 field_0x00;
    u32 numData;             // +0x04: number of reference slots
    u32 field_0x08;
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u32 field_0x20;
    ResUserDataRef refs[1];  // +0x24
};

// ---------------------------------------------------------------------------
// Inlined nw4r g3d ResUserData accessors (retail debug asserts).
// The retail g3d_resuser_ac.h / g3d_resdict_ac.h accessors carry assertion
// panics that were inlined at every use site; reproducing them keeps the
// panic call sites byte-identical.
// ---------------------------------------------------------------------------

// GetNumData(): null + alignment asserts, then the item count.
static inline u32 ResUserDataNumData(const void* ud, ResUserDataDic* dic) {
    if (ud == NULL) {
        nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC,
                        lbl_eu_80530CF0, lbl_eu_80663C48);
    }
    if (reinterpret_cast<u32>(dic) & 3) {
        nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
    }
    return (dic != NULL) ? dic->numData : 0;
}

// operator[](int): null + bounds asserts, then resolve the item pointer.
static inline ResUserDataItem* ResUserDataItemAt(const void* ud,
                                                 ResUserDataDic* dic,
                                                 s32 idx) {
    if (ud == NULL) {
        nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC,
                        lbl_eu_80530CF0, lbl_eu_80663C48);
    }
    if (reinterpret_cast<u32>(dic) & 3) {
        nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
    }

    ResUserDataItem* item = NULL;

    if (dic != NULL) {
        if (idx >= 0 && idx <= static_cast<s32>(dic->numData) - 1) {
            item = reinterpret_cast<ResUserDataItem*>(1);  // valid marker
        }
        if (item == NULL) {
            if (dic == NULL) {
                nw4r::db::Panic(lbl_eu_8056E43C, 0x54, lbl_eu_8056E420,
                                lbl_eu_80663928, lbl_eu_80663C54);
            }
            nw4r::db::Panic(lbl_eu_8056E3D0, 0x2A, lbl_eu_8056E398, idx, 0,
                            static_cast<s32>(dic->numData) - 1);
        }
        if (dic == NULL) {
            nw4r::db::Panic(lbl_eu_8056E43C, 0x54, lbl_eu_8056E420,
                            lbl_eu_80663928, lbl_eu_80663C54);
        }
        s32 refOff = dic->refs[idx].dataOffset;
        if (refOff != 0) {
            item = reinterpret_cast<ResUserDataItem*>(
                reinterpret_cast<u8*>(dic) + refOff);
        } else {
            item = NULL;
        }
    }

    return item;
}

// =============================================================================
// CMdlMaterial destructor
// =============================================================================
// CMdlMaterial::~CMdlMaterial() - D0 deleting destructor.
// Body is empty; MWCC generates the conditional operator delete via the r4 flag.
CMdlMaterial::~CMdlMaterial() {}

// =============================================================================
// func_804E54B8 - CMdlMaterial setup from model resource
// =============================================================================
// Initializes the material descriptor from a model resource. Allocates a
// GXColor buffer (4 channels per material), extracts ambient channel colours,
// and scans each material's user-data block for an item whose name matches a
// known constant, accumulating its S32 value into a 16-bit table.
void CMdlMaterial::func_804E54B8(void* arg) {
    field_0x04 = arg;

    // Build a local ResMdl from the model data pointer embedded in arg.
    nw4r::g3d::ResMdl resMdl(reinterpret_cast<nw4r::g3d::ResMdlData*>(
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(arg) + 0x146C)));

    // Try to attach to an existing resident buffer first.
    if (func_80488938(arg, resMdl.GetResMatNumEntries() * 16) != NULL) {
        buffer = func_80488954(arg, resMdl.GetResMatNumEntries() * 16);
        flag_0x10 = 1;
    } else {
        // Fallback: allocate via MemManager (buffer owned by this object).
        u32 handle = func_80496018(
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(arg) + 4));
        buffer = mtl::MemManager::allocate_array(
            resMdl.GetResMatNumEntries() * 16, handle);
    }

    u32 counter = 0;

    for (u32 matIdx = 0; matIdx < resMdl.GetResMatNumEntries(); matIdx++) {
        nw4r::g3d::ResMat resMat = resMdl.GetResMat(matIdx);

        if (!resMat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, lbl_eu_80663C44);
        }

        nw4r::g3d::ScnMdl::CopiedMatAccess access(
            *reinterpret_cast<nw4r::g3d::ScnMdl**>(reinterpret_cast<u8*>(arg) + 0x147C),
            resMat.ref().id);
        nw4r::g3d::ResMatChan chan = access.GetResMatChan(false);

        // Four ambient channels per material; post-increment writes the next
        // slot index before the call (matches the retail store ordering).
        chan.GXGetChanAmbColor((GXChannelID)0,
                               &reinterpret_cast<GXColor*>(buffer)[field_0x0C++]);
        chan.GXGetChanAmbColor((GXChannelID)2,
                               &reinterpret_cast<GXColor*>(buffer)[field_0x0C++]);
        chan.GXGetChanAmbColor((GXChannelID)1,
                               &reinterpret_cast<GXColor*>(buffer)[field_0x0C++]);
        chan.GXGetChanAmbColor((GXChannelID)3,
                               &reinterpret_cast<GXColor*>(buffer)[field_0x0C++]);

        // Scan the material's user-data block for an item matching the target
        // name; accumulate its S32 value into the 16-bit table.
        void* ud = resMat.GetResUserData();

        if (ud != NULL) {
            ResUserDataDic* dic =
                reinterpret_cast<ResUserDataDic*>(reinterpret_cast<u8*>(ud) + 4);

            for (u32 i = 0; i < ResUserDataNumData(ud, dic); i++) {
                ResUserDataItem* item =
                    ResUserDataItemAt(ud, dic, static_cast<s32>(i));

                if (reinterpret_cast<u32>(item) & 3) {
                    nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
                }
                if (item == NULL) {
                    nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                                    lbl_eu_80530D68, lbl_eu_80663C50);
                }

                const char* name = (item->nameOffset != 0)
                    ? reinterpret_cast<const char*>(
                          reinterpret_cast<const u8*>(item) + item->nameOffset)
                    : NULL;

                if (strcmp(name, lbl_eu_80663C30) == 0) {
                    if (item == NULL) {
                        nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                                        lbl_eu_80530D68, lbl_eu_80663C50);
                    }
                    if (item->valueType != 0) {
                        nw4r::db::Panic(lbl_eu_80530E1C, 0x36, lbl_eu_80530DD8);
                    }
                    if (item == NULL) {
                        nw4r::db::Panic(lbl_eu_80530D94, 0x26, lbl_eu_80530D78,
                                        lbl_eu_80530D68, lbl_eu_80663C4C);
                    }

                    u8* data = (item->dataOffset != 0)
                        ? reinterpret_cast<u8*>(item) + item->dataOffset
                        : NULL;

                    field_0x18[field_0x34] =
                        (u16)(counter + *reinterpret_cast<const u32*>(data));
                    field_0x34++;
                    break;
                }
            }
        }

        counter += 10;
    }
}

// func_804E5990 - scan a scene-node's user-data block. An item named via the
// lbl_eu_80663C34 pointer whose name is 8 chars ending in a digit sets the
// group id (digit value); items named via lbl_eu_80663C30 append their S32
// values as bytes to the flag table.
void func_804E5990(void* selfPtr, void* nodePtr, void* /*mtx*/) {
    CMdlMatView* self = reinterpret_cast<CMdlMatView*>(selfPtr);

    // Pass the node handle straight through (no stack temporary).
    void* ud = reinterpret_cast<nw4r::g3d::ResNode*>(nodePtr)->GetResUserData();

    if (ud == NULL) {
        return;
    }

    ResUserDataDic* dic =
        reinterpret_cast<ResUserDataDic*>(reinterpret_cast<u8*>(ud) + 4);

    for (u32 i = 0; i < ResUserDataNumData(ud, dic); i++) {
        ResUserDataItem* item = ResUserDataItemAt(ud, dic, static_cast<s32>(i));

        if (reinterpret_cast<u32>(item) & 3) {
            nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
        }
        if (item == NULL) {
            nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                            lbl_eu_80530D68, lbl_eu_80663C50);
        }

        const char* name = (item->nameOffset != 0)
            ? reinterpret_cast<const char*>(
                  reinterpret_cast<const u8*>(item) + item->nameOffset)
            : NULL;

        if (strcmp(name, lbl_eu_80663C34) == 0) {
            // Group id: last char of the owner's vtable+0x18 string, when the
            // string is 8 chars and ends in a digit. Retail re-issues the
            // virtual call for each use.
            const char* s1 = reinterpret_cast<const char*>(
                reinterpret_cast<MdlMatOwnerVt*>(self->field_0x04)->v05());
            if (strlen(s1) == 8) {
                const char* s2 = reinterpret_cast<const char*>(
                    reinterpret_cast<MdlMatOwnerVt*>(self->field_0x04)->v05());
                char c = s2[7];
                if (c >= '0') {
                    if (c <= '9') {
                        const char* s3 = reinterpret_cast<const char*>(
                            reinterpret_cast<MdlMatOwnerVt*>(self->field_0x04)
                                ->v05());
                        self->field_0x14 = s3[7] - '0';
                    }
                }
            }
        } else {
            // Second compare re-resolves the name pointer.
            const char* name2 = (item->nameOffset != 0)
                ? reinterpret_cast<const char*>(
                      reinterpret_cast<const u8*>(item) + item->nameOffset)
                : NULL;
            if (strcmp(name2, lbl_eu_80663C30) != 0) {
                continue;
            }

            // Append every S32 value of the item to the byte flag table.
            for (u32 j = 0; j < item->field_0x08; j++) {
                if (item == NULL) {
                    nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                                    lbl_eu_80530D68, lbl_eu_80663C50);
                }
                if (item->valueType != 0) {
                    nw4r::db::Panic(lbl_eu_80530E1C, 0x36, lbl_eu_80530DD8);
                }
                if (item == NULL) {
                    nw4r::db::Panic(lbl_eu_80530D94, 0x26, lbl_eu_80530D78,
                                    lbl_eu_80530D68, lbl_eu_80663C4C);
                }

                u8* data = (item->dataOffset != 0)
                    ? reinterpret_cast<u8*>(item) + item->dataOffset
                    : NULL;

                self->field_0x28[self->field_0x30] =
                    static_cast<u8>(*reinterpret_cast<const u32*>(data));
                self->field_0x30++;
            }
        }
    }
}

// func_804E5E38: for each entry in the material value table whose group id
// (value / 10) matches the requested value, copy that material's cull mode
// from the model resource into the scene model.
void func_804E5E38(CMdlMaterial* selfPtr, u32 val, bool useMatCull) {
    CMdlMatView* self = reinterpret_cast<CMdlMatView*>(selfPtr);
    MdlMatContext* ctx = reinterpret_cast<MdlMatContext*>(self->field_0x04);

    for (s32 i = 0; i < self->field_0x34; i++) {
        s32 entry = self->field_0x18[i];

        // Entries encode <group>*10 + sub-index; select by sub-index.
        if ((u32)(entry % 10) != val) {
            continue;
        }

        nw4r::g3d::ResMat mat =
            nw4r::g3d::ResMdl(ctx->mdlData).GetResMat((int)(entry / 10));
        nw4r::g3d::ResMatData* matData = mat.ptr();

        nw4r::g3d::ScnMdl::CopiedMatAccess access(ctx->scnMdl, matData->id);
        nw4r::g3d::ResGenMode genMode = access.GetResGenMode(false);

        GXCullMode cull = GX_CULL_ALL;
        if (useMatCull) {
            // Inlined null assert before reading the copied GenMode data.
            if (genMode.ptr() == NULL) {
                nw4r::db::Panic(lbl_eu_805701A0, 0xdf, lbl_eu_80570180,
                                lbl_eu_806638E8, lbl_eu_80663C40);
            }
            cull = genMode.GXGetCullMode();
        }

        if (matData == NULL) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, lbl_eu_80663C40);
        }

        // Redundant re-check: compiler folds the branch but keeps the
        // assertion-string address setup alive in the prologue.
        if (matData == NULL) {
            nw4r::db::Panic(lbl_eu_80570100, 0x26d, lbl_eu_805700E4,
                            lbl_eu_806638E8, lbl_eu_80663C40);
        }

        // Inlined NW4R_G3D_ASSERT_ALIGNMENT on the GenMode data.
        if (reinterpret_cast<u32>(genMode.ptr()) & 3) {
            nw4r::db::Panic(lbl_eu_80570170, 0xaf, lbl_eu_80570148,
                            lbl_eu_806638E8, lbl_eu_80663C40);
        }

        genMode.GXSetCullMode(cull);
    }
}

// func_804E5FD4: apply a colour (built from an RGB float triple scaled by
// 255.0f) as ambient colour on all four channels of every material.
bool func_804E5FD4(MdlMatApplier* ap, const f32* rgb) {
    if (ap->colors == NULL) {
        return false;
    }

    GXColor color;
    color.a = 0xFF;
    color.r = (u8)(lbl_eu_8066B304 * rgb[0]);
    color.g = (u8)(lbl_eu_8066B304 * rgb[1]);
    color.b = (u8)(lbl_eu_8066B304 * rgb[2]);

    for (u32 i = 0;
         i < nw4r::g3d::ResMdl(ap->ctx->mdlData).GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat =
            nw4r::g3d::ResMdl(ap->ctx->mdlData).GetResMat(i);

        if (!mat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, lbl_eu_80663C44);
        }

        nw4r::g3d::ScnMdl::CopiedMatAccess access(ap->ctx->scnMdl,
                                                  mat.ref().id);
        nw4r::g3d::ResMatChan chan = access.GetResMatChan(false);

        chan.GXSetChanAmbColor((GXChannelID)0, color);
        chan.GXSetChanAmbColor((GXChannelID)2, color);
        chan.GXSetChanAmbColor((GXChannelID)1, color);
        chan.GXSetChanAmbColor((GXChannelID)3, color);
    }

    return true;
}

// func_804E6158: like func_804E5FD4, but restricted to materials whose light
// set index matches the requested one (all materials when lightSet == -1).
// Returns whether any material was updated.
bool func_804E6158(MdlMatApplier* ap, const f32* rgb, s32 lightSet) {
    bool applied = false;

    if (ap->colors == NULL) {
        return false;
    }

    GXColor color;
    color.a = 0xFF;
    color.r = (u8)(lbl_eu_8066B304 * rgb[0]);
    color.g = (u8)(lbl_eu_8066B304 * rgb[1]);
    color.b = (u8)(lbl_eu_8066B304 * rgb[2]);

    for (u32 i = 0;
         i < nw4r::g3d::ResMdl(ap->ctx->mdlData).GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat =
            nw4r::g3d::ResMdl(ap->ctx->mdlData).GetResMat(i);

        if (lightSet != -1) {
            // Inlined ResMatMisc accessor asserts from g3d_resmat_ac.h.
            if (mat.ptr() == NULL) {
                nw4r::db::Panic(lbl_eu_80570100, 0x26d, lbl_eu_805700E4,
                                lbl_eu_806638E8, lbl_eu_80663C40);
            }
            nw4r::g3d::ResMatMisc misc(&mat.ref().misc);
            if (reinterpret_cast<u32>(misc.ptr()) & 3) {
                nw4r::db::Panic(lbl_eu_80570138, 0xf3, lbl_eu_80570110,
                                lbl_eu_806638E8, lbl_eu_80663C40);
            }

            if (misc.GetLightSetIdx() != lightSet) {
                continue;
            }
        }

        if (!mat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, lbl_eu_80663C44);
        }

        nw4r::g3d::ScnMdl::CopiedMatAccess access(ap->ctx->scnMdl,
                                                  mat.ref().id);
        nw4r::g3d::ResMatChan chan = access.GetResMatChan(false);

        chan.GXSetChanAmbColor((GXChannelID)0, color);
        chan.GXSetChanAmbColor((GXChannelID)2, color);
        chan.GXSetChanAmbColor((GXChannelID)1, color);
        chan.GXSetChanAmbColor((GXChannelID)3, color);

        applied = true;
    }

    return applied;
}

// func_804E6358: apply per-channel ambient colours taken sequentially from
// the applier's colour array to every material of the model.
bool func_804E6358(MdlMatApplier* ap) {
    if (ap->colors == NULL) {
        return false;
    }

    GXColor* colors = ap->colors;

    for (u32 i = 0;
         i < nw4r::g3d::ResMdl(ap->ctx->mdlData).GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat =
            nw4r::g3d::ResMdl(ap->ctx->mdlData).GetResMat(i);

        if (!mat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, lbl_eu_80663C44);
        }

        nw4r::g3d::ScnMdl::CopiedMatAccess access(ap->ctx->scnMdl,
                                                  mat.ref().id);
        nw4r::g3d::ResMatChan chan = access.GetResMatChan(false);

        chan.GXSetChanAmbColor((GXChannelID)0, *colors++);
        chan.GXSetChanAmbColor((GXChannelID)2, *colors++);
        chan.GXSetChanAmbColor((GXChannelID)1, *colors++);
        chan.GXSetChanAmbColor((GXChannelID)3, *colors++);
    }

    return true;
}

// func_804E64B0: for each flag byte whose group id (byte / 10) equals the
// owner material's accumulated value, invoke the chain-notify helper with
// the byte's sub-index (byte % 10).
void func_804E64B0(CMdlMaterial* selfPtr, void* arg, MdlMaterialOwner* owner) {
    CMdlMatView* self = reinterpret_cast<CMdlMatView*>(selfPtr);
    for (u32 i = 0; i < self->field_0x30; i++) {
        s32 b = self->field_0x28[i];
        s32 subIdx = b % 10;

        if ((u32)(b / 10) == owner->material.field_0x14) {
            func_80488C20(owner, arg, subIdx);
        }
    }
}
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x805700D8-0x805701B0 (0xD8): vtable (3 words) + 8 embedded
// nw4r assertion strings ("%s::%s: Object not valid." etc., retail bytes).
extern "C" u32 lbl_eu_80663C38;          // .sdata RTTI locator (defined below)
extern "C" u32 lbl_eu_8066B2F8;          // shared .sdata2 constant pair
extern "C" u32 lbl_eu_8066B300;
extern "C" const u32 lbl_eu_80524798[7];  // .rodata name string (defined below)
extern "C" void __dt__12CMdlMaterialFv();  // member dtor (this TU, retail reloc)

extern "C" u32 lbl_eu_805700D8[54] = {
    (u32)&lbl_eu_80663C38, 0x00000000, (u32)&__dt__12CMdlMaterialFv,
    0x25733A3A, 0x25733A20, 0x4F626A65, 0x6374206E, 0x6F742076, 0x616C6964, 0x2E000000,
    0x6733645F, 0x7265736D, 0x61745F61, 0x632E6800, 0x4E573452, 0x3A466169, 0x6C656420,
    0x61737365, 0x7274696F, 0x6E202128, 0x28753332, 0x29702026, 0x20307833, 0x29000000,
    0x6733645F, 0x7265736D, 0x61745F61, 0x632E6800, 0x4E573452, 0x3A466169, 0x6C656420,
    0x61737365, 0x7274696F, 0x6E202128, 0x28753332, 0x29702026, 0x20307833, 0x29000000,
    0x6733645F, 0x7265736D, 0x61745F61, 0x632E6800, 0x4E573452, 0x3A466169, 0x6C656420,
    0x61737365, 0x7274696F, 0x6E204973, 0x56616C69, 0x64282900, 0x6733645F, 0x7265736D,
    0x61745F61, 0x632E6800,
};

// [.rodata] 0x80524798-0x805247B4 (0x1C): "CMdlMaterial" + "mtIris03" names.
extern "C" const u32 lbl_eu_80524798[7] = {
    0x434D646C, 0x4D617465, 0x7269616C, 0x00000000, 0x6D744972, 0x69733033, 0x00000000,
};

// [.sdata] 0x80663C30-0x80663C58 (0x28): RTTI pairs + "ref" strings.
extern "C" u32 lbl_sd_00[2] = { (u32)&lbl_eu_8066B2F8, (u32)&lbl_eu_8066B300 };
extern "C" u32 lbl_sd_02[2] = { (u32)&lbl_eu_80524798, 0x00000000 };
extern "C" u32 lbl_sd_04 = 0x72656600;
extern "C" u32 lbl_sd_05 = 0x72656600;
extern "C" u32 lbl_sd_06 = 0x72656600;
extern "C" u32 lbl_sd_07 = 0x72656600;
extern "C" u32 lbl_sd_08 = 0x72656600;
extern "C" u32 lbl_sd_09 = 0x72656600;
