#pragma once

#include <types.h>
#include <nw4r/g3d/res/g3d_resfile.h>

/**
 * CScnItemAnim - Scene animation item (size: 0x58)
 *
 * Manages a character animation resource within the scene graph.
 * Stores an nw4r::g3d::ResFile reference and the name of the first
 * animation track found in that resource.
 *
 * Vtable: lbl_eu_8056EC90 (0x20 bytes, 8 entries)
 * Base class destructor: __dt__80482054
 */
class CScnItemAnim {
public:
    /**
     * Constructor. Takes a ResFile handle and an extra parameter.
     * Initializes the ResFile, binds it, allocates the object,
     * and copies the name of the first animation into mName.
     */
    CScnItemAnim(nw4r::g3d::ResFile resFile, u32 param);

    virtual ~CScnItemAnim();

    nw4r::g3d::ResFile GetResFile() const;
    char* GetName();
    u32 GetNameLen() const;
    nw4r::g3d::ResFile GetResFileCopy() const;

    // 0x00: vtable pointer (lbl_eu_8056EC90)
    CScnItemAnim* mParent;              // 0x04 - parent/owner pointer
    u16 mType;                          // 0x08 = 2
    u8 pad_0A[2];                       // 0x0A - padding

    nw4r::g3d::ResFile mResFile;        // 0x0C - embedded ResFile handle (4 bytes)
    char mName[0x40];                   // 0x10 - animation name buffer
    u32 mNameLen;                       // 0x50 - strlen of mName
    nw4r::g3d::ResFile mResFileCopy;    // 0x54 - copy of the ResFile handle
}; // size: 0x58
