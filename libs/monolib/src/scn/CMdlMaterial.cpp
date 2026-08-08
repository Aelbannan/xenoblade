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

// Forward declarations for external functions
void* func_80488938(void* owner, u32 size);
void* func_80488954(void* owner, u32 size);
u32 func_80496018(u32 handle);

// User data string constant for comparison

// =============================================================================
// ResUserData - Resource user data structure
// =============================================================================
// Layout recovered from func_804E54B8's access pattern.
// The header occupies 0x28 bytes, followed by an offset table (stride 0x10).

struct ResUserDataHeader {
    u32 field_0x00;
    u32 field_0x04;
    u32 count;          // +0x08: number of entries in the offset table
    u32 field_0x0C;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1C;
    u32 field_0x20;
    u32 field_0x24;     // first entry at header+0x28 (redundant: entries follow at +0x28)
};

// Each offset table entry is a u32 at the start of a 0x10-byte slot
// (only the first 4 bytes are used; rest is padding for 16-byte stride)

// Named sub-entry within user data, pointed to by an offset table entry
struct ResUserDataNamed {
    u32 field_0x00;
    u32 data_offset;    // +0x04: offset to actual data value
    u32 field_0x08;
    u32 field_0x0C;     // +0x0C: expected to be 0
    u32 name_offset;    // +0x10: offset to name string (relative to this struct)
};

// =============================================================================
// CMdlMaterial destructor
// =============================================================================
// CMdlMaterial::~CMdlMaterial() - D0 deleting destructor.
// Body is empty; MWCC generates the conditional operator delete via the r4 flag.
CMdlMaterial::~CMdlMaterial() {}

// =============================================================================
// func_804E54B8 - CMdlMaterial setup from model resource
// =============================================================================
// Initializes the material descriptor from a CScnItemModelNw4r's embedded
// model resource. Allocates a GXColor buffer (4 channels per material),
// extracts ambient channel colours, and accumulates named user-data values
// into a 16-bit table (field_0x18).
void CMdlMaterial::func_804E54B8(void* arg) {
    field_0x04 = arg;

    // Create a local ResMdl from the model data pointer embedded in arg
    nw4r::g3d::ResMdl resMdl(
        reinterpret_cast<nw4r::g3d::ResMdlData*>(
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(arg) + 0x146C)));

    u32 numMat = resMdl.GetResMatNumEntries();
    u32 bufSize = numMat * sizeof(GXColor) * 4; // 4 channels per material

    // Try fast allocation path first
    void* alloc = func_80488938(arg, bufSize);

    if (alloc != NULL) {
        // Fast path succeeded - use the allocation and mark as externally owned
        buffer = func_80488954(arg, bufSize);
        flag_0x10 = 1;
    } else {
        // Fallback: allocate via MemManager
        u32 allocHandle = func_80496018(
            *reinterpret_cast<u32*>(reinterpret_cast<u8*>(arg) + 4));
        buffer = mtl::MemManager::allocate_array(bufSize, allocHandle);
        // flag_0x10 remains 0 (ctor default) - buffer is owned
    }

    // Iterate over all materials
    u32 counter = 0;
    nw4r::g3d::ScnMdl* pScnMdl =
        *reinterpret_cast<nw4r::g3d::ScnMdl**>(
            reinterpret_cast<u8*>(arg) + 0x147C);

    for (u32 matIdx = 0; matIdx < numMat; matIdx++) {
        nw4r::g3d::ResMat resMat = resMdl.GetResMat(matIdx);

        // Panic if material lookup failed
        if (!resMat.IsValid()) {
            nw4r::db::Panic("CMdlMaterial.cpp", 0x26D,
                "ERROR: Cannot find material at index %d in model %s.\n",
                "ResMat pointer is null.\n");
        }

        // Create a copied material accessor
        nw4r::g3d::ScnMdl::CopiedMatAccess access(pScnMdl, resMat.ref().id);

        // Get the material channel descriptor
        nw4r::g3d::ResMatChan chan = access.GetResMatChan(false);

        // Read ambient colours for channels 0, 2, 1, 3 in that order.
        // The buffer is an array of GXColor values indexed by field_0x0C.
        GXColor* colorBuf = reinterpret_cast<GXColor*>(buffer);

        chan.GXGetChanAmbColor(static_cast<GXChannelID>(0),
            &colorBuf[field_0x0C]);
        field_0x0C++;

        chan.GXGetChanAmbColor(static_cast<GXChannelID>(2),
            &colorBuf[field_0x0C]);
        field_0x0C++;

        chan.GXGetChanAmbColor(static_cast<GXChannelID>(1),
            &colorBuf[field_0x0C]);
        field_0x0C++;

        chan.GXGetChanAmbColor(static_cast<GXChannelID>(3),
            &colorBuf[field_0x0C]);
        field_0x0C++;

        // Process user data attached to this material
        ResUserDataHeader* userData =
            reinterpret_cast<ResUserDataHeader*>(resMat.GetResUserData());

        if (userData != NULL) {
            // r31 = (u8*)userData + 4 - base for offset calculations
            u8* udBase = reinterpret_cast<u8*>(userData) + 4;

            // Validate alignment (must be 4-byte aligned)
            if (reinterpret_cast<u32>(udBase) & 3) {
                nw4r::db::Panic("CMdlMaterial.cpp", 0x54,
                    "ResUserData: unaligned base pointer.\n", NULL);
            }

            // Iterate through the offset table entries
            u32 udCount = *reinterpret_cast<u32*>(udBase + 4);
            u32 entryOffset = 0; // accumulates i * 0x10 (stride)

            for (u32 i = 0; i < udCount; i++, entryOffset += 0x10) {
                // Get offset to named sub-entry from the table
                u32 subOffs = *reinterpret_cast<u32*>(udBase + entryOffset + 0x24);

                ResUserDataNamed* named = NULL;
                if (subOffs != 0) {
                    named = reinterpret_cast<ResUserDataNamed*>(udBase + subOffs);

                    // Validate alignment
                    if (reinterpret_cast<u32>(named) & 3) {
                        nw4r::db::Panic("CMdlMaterial.cpp", 0x26,
                            "ResUserData: unaligned named entry.\n", NULL);
                    }
                }

                if (named == NULL) {
                    nw4r::db::Panic("CMdlMaterial.cpp", 0x26,
                        "ERROR: Named user data entry is null.\n",
                        "named\n");
                }

                // Get the name string and compare with target
                const char* name = (named->name_offset != 0)
                    ? reinterpret_cast<const char*>(
                        reinterpret_cast<u8*>(named) + named->name_offset)
                    : NULL;

                if (strcmp(name, lbl_eu_80663C30) == 0) {
                    // Found the target entry - extract its data value
                    if (named->field_0x0C != 0) {
                        nw4r::db::Panic("CMdlMaterial.cpp", 0x36,
                            "ResUserData: field_0x0C expected to be 0.\n", NULL);
                    }

                    u32* dataPtr = (named->data_offset != 0)
                        ? reinterpret_cast<u32*>(
                            reinterpret_cast<u8*>(named) + named->data_offset)
                        : NULL;

                    u32 value = *dataPtr;
                    counter += value;
                    field_0x18[field_0x34] = static_cast<u16>(counter);
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
