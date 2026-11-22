// Auto-scaffolded catalog TU for monolib/src/scn/CMdlMaterial
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <string.h>
#include "monolib/scn/CMdlMaterial.hpp"
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/res/g3d_resuserdata.h>
#include <nw4r/g3d/g3d_scnmdl.h>
#include <nw4r/db/db_assert.h>
#include <revolution/GX.h>
#include <monolib/util/MemManager.hpp>

// Forward declarations for external C-ABI functions (retail unmangled names).
extern "C" void* scnImN4PoolHasSpc(void* owner, u32 size);
extern "C" void* scnImN4PoolAlloc(void* owner, u32 size);
extern "C" u32 Scn_CallUnk8C_V8(u32 handle);

// Node user-data scanner (retail unmangled symbol; declared with C linkage in
// CScnItemModelNw4r.hpp, which is not self-contained - mirrored here).
extern "C" void scanNodeUserData(void* self, void* node, void* mtx);

// Virtual-dispatch view of the owner object for the vtable+0x18 getter
// (CScnItemModelNw4r::v04, scnImN4CreateModel) used by the node user-data scan.
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

// .sdata "ref" Panic args (defined at the bottom of this TU, after the RTTI
// locator, to keep the retail .sdata order). Retail: __FUNCTION__ statics of
// the inlined asserting nw4r ResCommon<T>::ref() accessors; they become
// compiler-emitted once the nw4r g3d headers carry their asserts.
extern char sResMatRefFn[4];
extern char sResMatRefFn2[4];
extern char sResUserDataRefFn[4];
extern char sResUserDataItemRefFn2[4];
extern char sResUserDataItemRefFn[4];
extern char sResDicRefFn[4];

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

// Node user-data keys owned by this TU (.sdata pointers to short literals).
const char* sMatCullModeKey = "Hide";
const char* sMatGroupIdKey = "PID";

// g3d_resmat_ac.h assertion strings (inlined accessor asserts).
extern const char lbl_eu_80570100[];
extern const char lbl_eu_805700E4[];
extern const char lbl_eu_80570138[];
extern const char lbl_eu_80570110[];
extern const char lbl_eu_80570170[];
extern const char lbl_eu_80570148[];
extern const char lbl_eu_805701A0[];
extern const char lbl_eu_80570180[];

// Hand-built vtable (defined with the rest of the dissolved .data below).
extern "C" u32 CMdlMaterialVtable[];


// .sdata2 float constant used to scale colours (255.0f).


// CScnItemModelNw4r helper (retail unmangled symbol).
extern "C" void scnImN4MatFn(void* owner, void* arg, s32 subIdx);

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
                        lbl_eu_80530CF0, sResUserDataRefFn);
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
                        lbl_eu_80530CF0, sResUserDataRefFn);
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
                                lbl_eu_80663928, sResDicRefFn);
            }
            nw4r::db::Panic(lbl_eu_8056E3D0, 0x2A, lbl_eu_8056E398, idx, 0,
                            static_cast<s32>(dic->numData) - 1);
        }
        if (dic == NULL) {
            nw4r::db::Panic(lbl_eu_8056E43C, 0x54, lbl_eu_8056E420,
                            lbl_eu_80663928, sResDicRefFn);
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
// CMdlMaterial constructor (flat retail symbol __ct__CMdlMaterial)
// =============================================================================
// Stores the hand-built vtable pointer (CMdlMaterialVtable, this TU) and zeroes
// the owner/buffer/index fields; field_0x14 starts at -1. The u16 table at
// 0x18 is deliberately left uninitialised (retail stores nothing there).
extern "C" void __ct__CMdlMaterial(CMdlMaterial* self) {
    *(void**)self = (void*)CMdlMaterialVtable;
    self->field_0x04 = 0;
    self->buffer = 0;
    self->field_0x0C = 0;
    self->flag_0x10 = 0;
    self->field_0x14 = -1;
    self->field_0x30 = 0;
    self->field_0x34 = 0;
}

// Buffer-owning teardown (flat retail symbol __dt__804E5DE0): release the
// GXColor array when this object owns it (flag_0x10 == 0).
extern "C" void __dt__804E5DE0(CMdlMaterial* self) {
    if (self->flag_0x10 == 0) {
        if (self->buffer != 0) {
            operator delete[](self->buffer);
            self->buffer = 0;
        }
    } else {
        self->buffer = 0;
    }
}

// =============================================================================
// CMdlMaterial destructor
// =============================================================================
// CMdlMaterial::~CMdlMaterial() - D0 deleting destructor.
// Body is empty; MWCC generates the conditional operator delete via the r4 flag.
CMdlMaterial::~CMdlMaterial() {}

// =============================================================================
// setupFromModel - CMdlMaterial setup from model resource
// =============================================================================
// Initializes the material descriptor from a model resource. Allocates a
// GXColor buffer (4 channels per material), extracts ambient channel colours,
// and scans each material's user-data block for an item whose name matches a
// known constant, accumulating its S32 value into a 16-bit table.
extern "C" void setupFromModel(CMdlMaterial* self, void* arg) {
    self->field_0x04 = arg;

    // Build a local ResMdl from the model data pointer embedded in arg.
    nw4r::g3d::ResMdl resMdl(reinterpret_cast<nw4r::g3d::ResMdlData*>(
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(arg) + 0x146C)));

    // Try to attach to an existing resident buffer first.
    if (scnImN4PoolHasSpc(arg, resMdl.GetResMatNumEntries() * 16) != NULL) {
        self->buffer = scnImN4PoolAlloc(arg, resMdl.GetResMatNumEntries() * 16);
        self->flag_0x10 = 1;
    } else {
        // Fallback: allocate via MemManager (buffer owned by this object).
        u32 handle = Scn_CallUnk8C_V8(
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(arg) + 4));
        self->buffer = mtl::MemManager::allocate_array(
            resMdl.GetResMatNumEntries() * 16, handle);
    }

    u32 counter = 0;

    for (u32 matIdx = 0; matIdx < resMdl.GetResMatNumEntries(); matIdx++) {
        nw4r::g3d::ResMat resMat = resMdl.GetResMat(matIdx);

        if (!resMat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, sResMatRefFn2);
        }

        nw4r::g3d::ScnMdl::CopiedMatAccess access(
            *reinterpret_cast<nw4r::g3d::ScnMdl**>(reinterpret_cast<u8*>(arg) + 0x147C),
            resMat.ref().id);
        nw4r::g3d::ResMatChan chan = access.GetResMatChan(false);

        // Four ambient channels per material; post-increment writes the next
        // slot index before the call (matches the retail store ordering).
        chan.GXGetChanAmbColor((GXChannelID)0,
                               &reinterpret_cast<GXColor*>(self->buffer)[self->field_0x0C++]);
        chan.GXGetChanAmbColor((GXChannelID)2,
                               &reinterpret_cast<GXColor*>(self->buffer)[self->field_0x0C++]);
        chan.GXGetChanAmbColor((GXChannelID)1,
                               &reinterpret_cast<GXColor*>(self->buffer)[self->field_0x0C++]);
        chan.GXGetChanAmbColor((GXChannelID)3,
                               &reinterpret_cast<GXColor*>(self->buffer)[self->field_0x0C++]);

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
                                    lbl_eu_80530D68, sResUserDataItemRefFn);
                }

                const char* name = (item->nameOffset != 0)
                    ? reinterpret_cast<const char*>(
                          reinterpret_cast<const u8*>(item) + item->nameOffset)
                    : NULL;

                if (strcmp(name, sMatCullModeKey) == 0) {
                    if (item == NULL) {
                        nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                                        lbl_eu_80530D68, sResUserDataItemRefFn);
                    }
                    if (item->valueType != 0) {
                        nw4r::db::Panic(lbl_eu_80530E1C, 0x36, lbl_eu_80530DD8);
                    }
                    if (item == NULL) {
                        nw4r::db::Panic(lbl_eu_80530D94, 0x26, lbl_eu_80530D78,
                                        lbl_eu_80530D68, sResUserDataItemRefFn2);
                    }

                    u8* data = (item->dataOffset != 0)
                        ? reinterpret_cast<u8*>(item) + item->dataOffset
                        : NULL;

                    self->field_0x18[self->field_0x34] =
                        (u16)(counter + *reinterpret_cast<const u32*>(data));
                    self->field_0x34++;
                    break;
                }
            }
        }

        counter += 10;
    }
}

// scanNodeUserData - scan a scene-node's user-data block. An item named via the
// sMatGroupIdKey pointer whose name is 8 chars ending in a digit sets the
// group id (digit value); items named via sMatCullModeKey append their S32
// values as bytes to the flag table.
void scanNodeUserData(void* selfPtr, void* nodePtr, void* /*mtx*/) {
    CMdlMaterial* self = reinterpret_cast<CMdlMaterial*>(selfPtr);

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
                            lbl_eu_80530D68, sResUserDataItemRefFn);
        }

        const char* name = (item->nameOffset != 0)
            ? reinterpret_cast<const char*>(
                  reinterpret_cast<const u8*>(item) + item->nameOffset)
            : NULL;

        if (strcmp(name, sMatGroupIdKey) == 0) {
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
            if (strcmp(name2, sMatCullModeKey) != 0) {
                continue;
            }

            // Append every S32 value of the item to the byte flag table.
            for (u32 j = 0; j < item->field_0x08; j++) {
                if (item == NULL) {
                    nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                                    lbl_eu_80530D68, sResUserDataItemRefFn);
                }
                if (item->valueType != 0) {
                    nw4r::db::Panic(lbl_eu_80530E1C, 0x36, lbl_eu_80530DD8);
                }
                if (item == NULL) {
                    nw4r::db::Panic(lbl_eu_80530D94, 0x26, lbl_eu_80530D78,
                                    lbl_eu_80530D68, sResUserDataItemRefFn2);
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

// applyCullByGroup: for each entry in the material value table whose group id
// (value / 10) matches the requested value, copy that material's cull mode
// from the model resource into the scene model.
extern "C" void applyCullByGroup(CMdlMaterial* selfPtr, u32 val, bool useMatCull) {
    CMdlMaterial* self = reinterpret_cast<CMdlMaterial*>(selfPtr);
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
                                lbl_eu_806638E8, sResMatRefFn);
            }
            cull = genMode.GXGetCullMode();
        }

        if (matData == NULL) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, sResMatRefFn);
        }

        // Redundant re-check: compiler folds the branch but keeps the
        // assertion-string address setup alive in the prologue.
        if (matData == NULL) {
            nw4r::db::Panic(lbl_eu_80570100, 0x26d, lbl_eu_805700E4,
                            lbl_eu_806638E8, sResMatRefFn);
        }

        // Inlined NW4R_G3D_ASSERT_ALIGNMENT on the GenMode data.
        if (reinterpret_cast<u32>(genMode.ptr()) & 3) {
            nw4r::db::Panic(lbl_eu_80570170, 0xaf, lbl_eu_80570148,
                            lbl_eu_806638E8, sResMatRefFn);
        }

        genMode.GXSetCullMode(cull);
    }
}

// applyAmbientAll: apply a colour (built from an RGB float triple scaled by
// 255.0f) as ambient colour on all four channels of every material.
extern "C" bool applyAmbientAll(MdlMatApplier* ap, const f32* rgb) {
    if (ap->colors == NULL) {
        return false;
    }

    GXColor color;
    color.a = 0xFF;
    color.r = (u8)(255.0f * rgb[0]);
    color.g = (u8)(255.0f * rgb[1]);
    color.b = (u8)(255.0f * rgb[2]);

    for (u32 i = 0;
         i < nw4r::g3d::ResMdl(ap->ctx->mdlData).GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat =
            nw4r::g3d::ResMdl(ap->ctx->mdlData).GetResMat(i);

        if (!mat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, sResMatRefFn2);
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

// applyAmbientByLight: like applyAmbientAll, but restricted to materials whose light
// set index matches the requested one (all materials when lightSet == -1).
// Returns whether any material was updated.
extern "C" bool applyAmbientByLight(MdlMatApplier* ap, const f32* rgb, s32 lightSet) {
    bool applied = false;

    if (ap->colors == NULL) {
        return false;
    }

    GXColor color;
    color.a = 0xFF;
    color.r = (u8)(255.0f * rgb[0]);
    color.g = (u8)(255.0f * rgb[1]);
    color.b = (u8)(255.0f * rgb[2]);

    for (u32 i = 0;
         i < nw4r::g3d::ResMdl(ap->ctx->mdlData).GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat =
            nw4r::g3d::ResMdl(ap->ctx->mdlData).GetResMat(i);

        if (lightSet != -1) {
            // Inlined ResMatMisc accessor asserts from g3d_resmat_ac.h.
            if (mat.ptr() == NULL) {
                nw4r::db::Panic(lbl_eu_80570100, 0x26d, lbl_eu_805700E4,
                                lbl_eu_806638E8, sResMatRefFn);
            }
            nw4r::g3d::ResMatMisc misc(&mat.ref().misc);
            if (reinterpret_cast<u32>(misc.ptr()) & 3) {
                nw4r::db::Panic(lbl_eu_80570138, 0xf3, lbl_eu_80570110,
                                lbl_eu_806638E8, sResMatRefFn);
            }

            if (misc.GetLightSetIdx() != lightSet) {
                continue;
            }
        }

        if (!mat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, sResMatRefFn2);
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

// applyAmbientArray: apply per-channel ambient colours taken sequentially from
// the applier's colour array to every material of the model.
extern "C" bool applyAmbientArray(MdlMatApplier* ap) {
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
                            lbl_eu_806638E8, sResMatRefFn2);
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

// notifyGroupChain: for each flag byte whose group id (byte / 10) equals the
// owner material's accumulated value, invoke the chain-notify helper with
// the byte's sub-index (byte % 10).
extern "C" void notifyGroupChain(CMdlMaterial* selfPtr, void* arg, MdlMaterialOwner* owner) {
    CMdlMaterial* self = reinterpret_cast<CMdlMaterial*>(selfPtr);
    for (u32 i = 0; i < self->field_0x30; i++) {
        s32 b = self->field_0x28[i];
        s32 subIdx = b % 10;

        if ((u32)(b / 10) == owner->material.field_0x14) {
            scnImN4MatFn(owner, arg, subIdx);
        }
    }
}
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x805700D8-0x805701B0 (0xD8): vtable (3 words) + 8 embedded
// nw4r assertion strings ("%s::%s: Object not valid." etc., retail bytes).
extern "C" u32 __RTTI__12CMdlMaterial;          // .sdata RTTI locator (defined below)


extern "C" void __dt__12CMdlMaterialFv();  // member dtor (this TU, retail reloc)

extern "C" u32 CMdlMaterialVtable[54] = {
    (u32)&__RTTI__12CMdlMaterial, 0x00000000, (u32)&__dt__12CMdlMaterialFv,
    0x25733A3A, 0x25733A20, 0x4F626A65, 0x6374206E, 0x6F742076, 0x616C6964, 0x2E000000,
    0x6733645F, 0x7265736D, 0x61745F61, 0x632E6800, 0x4E573452, 0x3A466169, 0x6C656420,
    0x61737365, 0x7274696F, 0x6E202128, 0x28753332, 0x29702026, 0x20307833, 0x29000000,
    0x6733645F, 0x7265736D, 0x61745F61, 0x632E6800, 0x4E573452, 0x3A466169, 0x6C656420,
    0x61737365, 0x7274696F, 0x6E202128, 0x28753332, 0x29702026, 0x20307833, 0x29000000,
    0x6733645F, 0x7265736D, 0x61745F61, 0x632E6800, 0x4E573452, 0x3A466169, 0x6C656420,
    0x61737365, 0x7274696F, 0x6E204973, 0x56616C69, 0x64282900, 0x6733645F, 0x7265736D,
    0x61745F61, 0x632E6800,
};

// [.sdata] 0x80663C30-0x80663C58 (0x28): RTTI pairs + "ref" strings.

extern "C" const char* lbl_sd_02[2] = { "CMdlMaterial", NULL };

// name-guess: the second retail RTTI-adjacent name has no surviving use site.
// Keeping it in a non-static function makes MWCC pool it after "CMdlMaterial"
// while mwld dead-strips the function itself.
u32 CMdlMaterial_mtIris03Name() {
    return strlen("mtIris03");
}

// name-guess: __FUNCTION__ of the six distinct inlined ResCommon<T>::ref()
// accessors (ResMat x2, ResUserData, ResUserDataItem x2, ResDic), named after
// the class-name string each is paired with at its Panic call sites.
char sResMatRefFn[4] = "ref";
char sResMatRefFn2[4] = "ref";
char sResUserDataRefFn[4] = "ref";
char sResUserDataItemRefFn2[4] = "ref";
char sResUserDataItemRefFn[4] = "ref";
char sResDicRefFn[4] = "ref";
