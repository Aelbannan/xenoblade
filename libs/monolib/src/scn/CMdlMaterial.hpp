#ifndef CMdlMaterial_HPP
#define CMdlMaterial_HPP

#include <types.h>
#include <nw4r/g3d/res/g3d_resmat.h>

/**
 * CMdlMaterial — model material descriptor.
 *
 * Layout derived from constructor (__ct__CMdlMaterial at 0x804E98E0)
 * and vtable at lbl_eu_805700D8 (single virtual: destructor).
 * Size: 0x38 bytes (inferred from spacing before CMdlMouth at 0x1700
 * vs CMdlMaterial at 0x16C8 in CScnItemModelNw4r).
 */
class CMdlMaterial {
public:
    virtual ~CMdlMaterial();

    void func_804E54B8(void* arg);

    /* 0x04 */ void* field_0x04;    // Pointer to owner/context object
    /* 0x08 */ void* buffer;        // Dynamically allocated GXColor array (delete[]'d in sub-dtor)
    /* 0x0C */ u32 field_0x0C;      // Write index for GXColor buffer
    /* 0x10 */ u8 flag_0x10;        // Controls buffer ownership: 0 = owned (delete[]), non-0 = unowned
    /* 0x11 */ u8 pad_0x11[3];
    /* 0x14 */ s32 field_0x14;      // Initialized to -1
    /* 0x18 */ u16 field_0x18[12];   // Accumulated material values (max 12 entries, indexed by field_0x34)
    /* 0x30 */ u32 field_0x30;
    /* 0x34 */ u32 field_0x34;      // Write index for field_0x18 array
};

#endif // CMdlMaterial_HPP
