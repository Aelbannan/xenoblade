#pragma once

#include <types.h>
#include "functions.hpp"  // func_800407C8_tmp / func_800407C8 (vec4 setter)

// Retail-owned data referenced by sinit_80494300 (monolib bss / sdata2 slice).
extern func_800407C8_tmp lbl_eu_80658648;
extern f32 lbl_eu_8066AA14;
extern f32 lbl_eu_8066AA18;

// Blend-factor step/clamp constants for func_80494208 (sdata2).
extern f32 lbl_eu_8066AA78;
extern f32 lbl_eu_8066AA24;

// Vec4 scale helper (defined in kyoshin/cf/CTaskEnvironment.cpp, flat-name
// symbol): out = in * s with w unscaled.
namespace ml {
struct CVec4;  // full definition in monolib/math/CVec4.hpp
}
extern "C" void func_80058BD8(ml::CVec4* out, const ml::CVec4* in, float s);

// Single-float math helpers used by func_804933AC's direction-vector to
// dir-light-angle conversion (flat-name retail symbols; not yet decompiled).
extern "C" f32 func_8004CC40(f32 value);
extern "C" f32 func_8006D410(f32 value);
extern "C" f32 func_8004EC78(f32 value);

// Word-array copy helper (CTaskEnvironment.cpp, flat-name symbol).
extern "C" void copyWord4(u32* dst, const u32* src);

// Same-TU vec4-add helper (defined in CScnVirtualLight.cpp): re-declared with
// C linkage so func_80494208's call reloc stays flat.
extern "C" void func_804942BC(f32* a, const f32* b);

// Retail vtable data for the reslist<CVirtualLightObjPtr> family. The
// flattened ctors below store them manually (compiler vtables would emit
// __vt__ relocs the retail lacks; cf. CScnFilterMan.cpp).
extern u8 lbl_eu_80526430[];  // reslist<CVirtualLightObjPtr> vtable
// _reslist_base<CVirtualLightObjPtr> vtable
extern u8 lbl_eu_80526448[];

struct CScnVirtualLightData {
    u32 value00;
    u8 _04[4];
    u32 value08;
    u8 _0C[8];
    f32 value14;
    u8 _18[0x14];
    u32 value2C;
    u32 value30;   // flags word (tested by the func_8004B3D8 flag helper)
    f32 value34;
    f32 value38;
    f32 value3C;
    f32 value40;
    f32 value44;
    f32 value48;
    f32 value4C;
    u8 _50[0x14];
    u32 value64;
    u32 value68;
    u8 _6C[0x20];
    f32 field_0x8C[4];  // vec4 (blend target)
    f32 field_0x9C[4];  // vec4 (blend start)
    f32 field_0xAC[4];  // vec4 (blend current, += result)
    f32 field_0xBC;     // blend factor, advanced/clamped by func_80494208
    u8 _C0[0x10];
    u8 enabled;
    u8 _D1[0x3];
    f32 valueD4;
    u8 _D8[0x3A8];
    u32 value480;
    u32 value484;
};

// Value-source node for the func_80492A50 / func_80492A80 value wrappers:
// func_80492A50 reads the value through one pointer hop at +0x04.
struct CScnVirtualLightValueSrc {
    u32 field_00;
    u32* mValue;   // +0x04
};

class CScnCameraMan;

// Camera-item lookup (defined in CScnCameraMan.cpp under the retail flat
// symbol). CScn.hpp only declares the 1-arg stub; this TU's func_80493B78
// passes the real (cam, id) ABI pair, so the full signature is declared here.
extern "C" int func_8049B158(CScnCameraMan* cam, s32 id);

// Light reset (defined in CLight.cpp): writes `value` to the light's +0x2C
// field. Flat-name import; declared here (CScnVirtualLight.hpp) because the
// declaring header CVirtualLightObj.hpp pulls CLight.hpp whose real class
// conflicts with this TU's stub CLight/CVirtualLightObj dtors.
extern "C" void func_804C03A0(u8* self, int value);

struct CVirtualLightAmb;  // defined in CVirtualLightAmb.hpp
class CVirtualLightDir;   // defined in CVirtualLightDir.cpp

// Dir-light factory (defined later in CScnVirtualLight.cpp). Declared with C
// linkage here so func_804933AC's call reloc stays flat (retail func_80493300);
// the plain definition below inherits the linkage.
extern "C" CVirtualLightDir* func_80493300(CScnVirtualLightData* self, u8* field,
                                           const func_800407C8_tmp* data, f32 val1,
                                           f32 val2, f32 val3);

// Amb-light constructor (defined in CVirtualLightAmb.cpp, flat-name symbol):
// builds the base light, installs the vtable and arms the slot (+0x2C = 1).
extern "C" CVirtualLightAmb* __ct__CVirtualLightAmb(CVirtualLightAmb* self);

// Dir-light constructor (defined in CVirtualLightDir.cpp; retail flat name
// __ct__CVirtualLightDir): builds the base light and sets +0x34/+0x38 to 1000.
extern "C" CVirtualLightDir* __ct__CVirtualLightDir(CVirtualLightDir* self);

// Two-float setter on the dir light (CVirtualLightDir.cpp, flat-name symbol):
// writes the +0x34/+0x38 fields.
extern "C" void func_8049474C(CVirtualLightDir* self, float a, float b);

// Word-array copy helper (kyoshin CTaskEnvironment.cpp, flat-name symbol).
extern "C" void copyWord4Offset(u32* dst, const u32* src);

// Same-TU helpers re-declared with C linkage so the flat retail call relocs
// (func_804930BC / func_804933AC call sites) stay byte-identical; the
// definitions below inherit the linkage and keep emitting the flat symbol.
extern "C" void func_80493148(CScnVirtualLightData* self, u32 arg2);
extern "C" void func_80493140(void* self, float val);

// Value-compare helper (defined in CScnVirtualLight.cpp): != on two words.
// Same-TU C-linkage redeclaration keeps func_804929C0's call reloc flat.
extern "C" u32 func_80492A88(const u32* a, const u32* b);

// Ring-walk helpers (defined in CScnVirtualLight.cpp, flat-name symbols):
// func_80492A64 returns the next node (+8 of *self), func_80492A70 advances
// a node to its successor, func_80492AA8 copies one word. Declared here so
// func_804929C0 (defined before them in the TU) can call them with flat
// relocs.
extern "C" u32* func_80492A64(void* self);
extern "C" void func_80492A70(u32* self);
extern "C" void func_80492AA8(int* dst, int* src);

class CScnVirtualLight {
public:
    CScnVirtualLight();
    virtual ~CScnVirtualLight();

    u32 value00;
    u32 value08;
    u8 _0C[0x08];
    f32 value14;
    u8 _18[0x1C];
    f32 value34;
    f32 value38;
    f32 value3C;
    f32 value40;
    f32 value44;
    f32 value48;
    f32 value4C;
    u8 _50[0x14];
    u32 value64;
    u8 _68[0x5C];
    u32 valueC4;
    u8 _C8[0x0C];
    f32 valueD4;
    u8 enabled;
    u8 _D9[0xA7];
    u32 value480;
};

class CVirtualLightObj;  // full definition below

// Sentinel/list node of the reslist ring (mirrors _reslist_node<T> from
// monolib/util/reslist.hpp). The item is the stored CVirtualLightObjPtr
// (a raw CVirtualLightObj*; 4-byte item keeps the node stride at 0xC).
struct CScnVirtualLightNode {
    CScnVirtualLightNode* mNext;  // +0x00
    CScnVirtualLightNode* mPrev;  // +0x04
    CVirtualLightObj* mItem;      // +0x08
};

// reslist<CVirtualLightObjPtr>-shaped object: vtable + _reslist_base storage
// (sentinel ring at +0x08, backing array +0x14, capacity +0x18, ownership
// flag +0x1C). Mirrors _reslist_base<T> in monolib/util/reslist.hpp with a
// named vtable slot. The flattened ctors __ct__804920B0 / __ct__80492074
// (below) initialize it.
class CScnVirtualLightReslist {
public:
    void* mVtable;                     // +0x00
    CScnVirtualLightNode* mStartNodePtr;  // +0x04 (points at mStartNode)
    CScnVirtualLightNode mStartNode;   // +0x08 (mNext/mPrev/mItem)
    CScnVirtualLightNode* mList;       // +0x14
    s32 mCapacity;                     // +0x18
    u8 field_0x1C;                     // +0x1C
};

class CLight {
public:
    ~CLight();
};

class CVirtualLightObj {
public:
    ~CVirtualLightObj();
};

// One slot of the virtual-light pool (0x40-byte stride). Content is opaque
// here; the pool only allocates/frees whole slots.
struct CScnVirtualLightPoolSlot {
    u8 data[0x40];
};

// Pool backing CScnVirtualLight's 128-slot light array (the object CETrail
// passes as CResHolder::m_res / light allocation source): record array
// (+0xC0, 0x40-byte stride), parallel handle array (+0xC4) and slot count
// (+0xCC). func_80494128 allocates a free slot, func_80494188 frees one.
struct CScnVirtualLightPool {
    u8 _00[0xC0];
    CScnVirtualLightPoolSlot* mRecords;  // +0xC0
    CScnVirtualLightPoolSlot** mHandles; // +0xC4
    u8 _C8[0x04];
    u32 mCount;                          // +0xCC
};
