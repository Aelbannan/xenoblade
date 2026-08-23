#pragma once

#include <types.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/res/g3d_resmdl.h>
#include <nw4r/g3d/g3d_scnmdl.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" const char* lbl_eu_80663C30;

/**
 * CMdlMaterial - model material descriptor.
 *
 * Layout derived from constructor (__ct__CMdlMaterial at 0x804E98E0)
 * and vtable at lbl_eu_805700D8 (single virtual: destructor).
 * Size: 0x38 bytes (inferred from spacing before CMdlMouth at 0x1700
 * vs CMdlMaterial at 0x16C8 in CScnItemModelNw4r).
 */
class __declspec(novtable) CMdlMaterial {
public:
    virtual ~CMdlMaterial();

    void func_804E54B8(void* arg);

    /* 0x04 */ void* field_0x04;    // Pointer to owner/context object
    /* 0x08 */ void* buffer;        // Dynamically allocated GXColor array (delete[]'d in sub-dtor)
    /* 0x0C */ u32 field_0x0C;      // Write index for GXColor buffer
    /* 0x10 */ u8 flag_0x10;        // Controls buffer ownership: 0 = owned (delete[]), non-0 = unowned
    /* 0x11 */ u8 pad_0x11[3];
    /* 0x14 */ u32 field_0x14;      // Initialized to -1
    /* 0x18 */ u16 field_0x18[8];   // Accumulated material values
    /* 0x28 */ u8 field_0x28[8];    // Per-material flag bytes
    /* 0x30 */ u32 field_0x30;
    /* 0x34 */ s32 field_0x34;      // Write index for field_0x18 array
};

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