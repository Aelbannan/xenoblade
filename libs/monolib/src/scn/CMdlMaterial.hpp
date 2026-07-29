#ifndef CMdlMaterial_HPP
#define CMdlMaterial_HPP

#include <types.h>
#include "nw4r/g3d/res/g3d_resmdl.h"

namespace nw4r { namespace g3d { class ScnMdl; } }

class CMdlModelOwner;

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

    /* 0x04 */ CMdlModelOwner* owner;
    /* 0x08 */ u8* buffer;
    /* 0x0C */ u32 field_0x0C;
    /* 0x10 */ u8 flag_0x10;
    /* 0x11 */ u8 pad_0x11[3];
    /* 0x14 */ s32 field_0x14;
    /* 0x18 */ u8 pad_0x18[0x10];
    /* 0x28 */ u8 byteArray[8];
    /* 0x30 */ u32 field_0x30;
    /* 0x34 */ u32 field_0x34;
};

// Container that owns a CMdlMaterial at offset 0x16C8.
// Fields below are only those accessed by CMdlMaterial member functions.
struct CMdlModelOwner {
    u8 pad_0x0000[0x146C];
    /* 0x146C */ nw4r::g3d::ResMdl resMdl;
    u8 pad_0x1470[0xC];
    /* 0x147C */ nw4r::g3d::ScnMdl* scnMdl;
    u8 pad_0x1480[0x16DC - 0x1480];
    /* 0x16DC */ s32 targetQuotient;
};

#endif // CMdlMaterial_HPP
