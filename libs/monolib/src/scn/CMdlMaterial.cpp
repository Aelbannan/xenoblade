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

// Forward declarations for external C-ABI functions.
void* func_80488938(void* owner, u32 size);
void* func_80488954(void* owner, u32 size);
u32 func_80496018(u32 handle);

// ===========================================================================
// nw4r g3d ResUserData reconstruction (g3d_resuser_ac.h accessor semantics)
// ===========================================================================

// One named item: name string, value type, and a relative offset to the value.
struct ResUserDataItem {
    u32 field_0x00;      // +0x00
    s32 dataOffset;      // +0x04: relative offset to the value (0 = null)
    u32 field_0x08;      // +0x08
    u32 valueType;       // +0x0C: 0 = S32 (asserted)
    s32 nameOffset;      // +0x10: relative offset to the name string
};

// A single 0x10-byte reference slot in the user-data array.
struct ResUserDataRef {
    s32 dataOffset;      // +0x00: relative offset (from ud+4) to ResUserDataItem
    u32 field_0x04;
    u32 field_0x08;
    u32 field_0x0C;
};

// User-data block header + reference array (starts 0x28 bytes in).
struct ResUserData {
    u32 field_0x00;
    u32 field_0x04;
    u32 numData;             // +0x08: number of reference slots
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u32 field_0x20;
    u32 field_0x24;
    ResUserDataRef refs[1];  // +0x28
};

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
extern const char lbl_eu_806638E8[];
extern const char lbl_eu_80663928[];
extern const char lbl_eu_80663C44[];
extern const char lbl_eu_80663C48[];
extern const char lbl_eu_80663C4C[];
extern const char lbl_eu_80663C50[];
extern const char lbl_eu_80663C54[];

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
    if (func_80488938(arg, resMdl.GetResMatNumEntries() * sizeof(GXColor) * 4) != NULL) {
        buffer = func_80488954(arg, resMdl.GetResMatNumEntries() * sizeof(GXColor) * 4);
        flag_0x10 = 1;
    } else {
        // Fallback: allocate via MemManager (buffer owned by this object).
        u32 handle = func_80496018(
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(arg) + 4));
        buffer = mtl::MemManager::allocate_array(
            resMdl.GetResMatNumEntries() * sizeof(GXColor) * 4, handle);
    }

    nw4r::g3d::ScnMdl* pScnMdl = reinterpret_cast<nw4r::g3d::ScnMdl*>(
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(arg) + 0x147C));

    u32 counter = 0;

    for (u32 matIdx = 0; matIdx < resMdl.GetResMatNumEntries(); matIdx++) {
        nw4r::g3d::ResMat resMat = resMdl.GetResMat(matIdx);

        if (!resMat.IsValid()) {
            nw4r::db::Panic(lbl_eu_8056E068, 0x26d, lbl_eu_8056E04C,
                            lbl_eu_806638E8, lbl_eu_80663C44);
        }

        nw4r::g3d::ScnMdl::CopiedMatAccess access(pScnMdl, resMat.ref().id);
        nw4r::g3d::ResMatChan chan = access.GetResMatChan(false);

        GXColor* colorBuf = reinterpret_cast<GXColor*>(buffer);

        chan.GXGetChanAmbColor(static_cast<GXChannelID>(0), &colorBuf[field_0x0C]);
        field_0x0C++;
        chan.GXGetChanAmbColor(static_cast<GXChannelID>(2), &colorBuf[field_0x0C]);
        field_0x0C++;
        chan.GXGetChanAmbColor(static_cast<GXChannelID>(1), &colorBuf[field_0x0C]);
        field_0x0C++;
        chan.GXGetChanAmbColor(static_cast<GXChannelID>(3), &colorBuf[field_0x0C]);
        field_0x0C++;

        // Scan the material's user-data block for an item matching the target name.
        ResUserData* ud = reinterpret_cast<ResUserData*>(resMat.GetResUserData());

        if (ud != NULL) {
            u8* base = reinterpret_cast<u8*>(ud) + 4;

            for (u32 i = 0;; i++) {
                if (ud == NULL) {
                    nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC,
                                    lbl_eu_80530CF0, lbl_eu_80663C48);
                }
                if (reinterpret_cast<u32>(base) & 3) {
                    nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
                }

                u32 numData = (base != NULL) ? ud->numData : 0;
                if (i >= numData) {
                    break;
                }

                ResUserDataItem* item = NULL;

                if (ud == NULL) {
                    nw4r::db::Panic(lbl_eu_80530D18, 0x57, lbl_eu_80530CFC,
                                    lbl_eu_80530CF0, lbl_eu_80663C48);
                }
                if (reinterpret_cast<u32>(base) & 3) {
                    nw4r::db::Panic(lbl_eu_80530F08, 0x54, lbl_eu_80530EE0);
                }

                if (base != NULL) {
                    if (static_cast<s32>(i) < 0 || static_cast<s32>(i) > static_cast<s32>(numData) - 1) {
                        nw4r::db::Panic(lbl_eu_8056E3D0, 0x2A, lbl_eu_8056E398,
                                        static_cast<int>(i), 0,
                                        static_cast<int>(numData) - 1);
                    } else {
                        s32 refOff = ud->refs[i].dataOffset;
                        if (refOff != 0) {
                            item = reinterpret_cast<ResUserDataItem*>(base + refOff);
                        }
                    }
                }

                if (reinterpret_cast<u32>(item) & 3) {
                    nw4r::db::Panic(lbl_eu_80530D54, 0x26, lbl_eu_80530D2C);
                }
                if (item == NULL) {
                    nw4r::db::Panic(lbl_eu_80530DC4, 0x26, lbl_eu_80530DA8,
                                    lbl_eu_80530D68, lbl_eu_80663C50);
                }

                const char* name = (item->nameOffset != 0)
                    ? reinterpret_cast<const char*>(
                          reinterpret_cast<u8*>(item) + item->nameOffset)
                    : NULL;

                if (strcmp(name, lbl_eu_80663C30) == 0) {
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

void func_804E5E38(){}

void func_804E5FD4(){}

void func_804E6158(){}

void func_804E6358(){}

void func_804E64B0(){}