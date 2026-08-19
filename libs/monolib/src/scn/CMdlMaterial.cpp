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

void func_804E5E38(){}

void func_804E5FD4(){}

void func_804E6158(){}

void func_804E6358(){}

void func_804E64B0(){}
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
