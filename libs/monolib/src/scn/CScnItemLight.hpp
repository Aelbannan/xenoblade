#pragma once

#include <types.h>

namespace nw4r {
namespace g3d {
class ScnRoot;
}
} // namespace nw4r

class CScnItemLightNw4r;

// A scene light item. Holds a reference to the owning CScnItemLightNw4r
// object (+0x04), a small opaque "light env" sub-object (+0x14) constructed
// by func_804950F4/destroyed by __dt__80495200, an nw4r g3d LightSetting
// (+0x944) used to push lighting into a scene root, and a display name.
class CScnItemLight {
public:
    CScnItemLight(CScnItemLightNw4r* pNw4r, char* name, u16 a2, void* a3);
    ~CScnItemLight();

    void* field_0x0;           // +0x00: manually-managed vtable pointer
    CScnItemLightNw4r* mpNw4r; // +0x04
    u16 field_0x8;             // +0x08
    u8 _A[2];                  // +0x0A
    void* field_0xc;           // +0x0C
    u16 field_0x10;            // +0x10
    u8 _12[2];                 // +0x12
    u8 mEnv[0x48];             // +0x14 -> 0x5C
    void* field_0x5c;          // +0x5C
    u8 _60[0x944 - 0x60];      // +0x60 -> 0x944
    u8 mLightSetting[0x10];    // +0x944 (nw4r g3d LightSetting)
    u8 _954[0x11d8 - 0x954];   // +0x954 -> 0x11D8
    char mName[0x40];          // +0x11D8
    u32 field_0x1218;          // +0x1218
    u8 field_0x121c;           // +0x121C
    u8 _121d[0x1220 - 0x121d]; // +0x121D -> 0x1220
};

class CScnItemLightNw4r : public CScnItemLight {
public:
    ~CScnItemLightNw4r();

    nw4r::g3d::ScnRoot* mpScnRoot; // +0x1220
    // total size 0x1224
};

// free functions (retail symbols)
extern "C" void func_80482388(CScnItemLight* self);
extern "C" void func_80482400(CScnItemLightNw4r* self);