#pragma once

#include <types.h>
#include "functions.hpp"  // CTaskGame_setVec4_tmp / CTaskGame_setVec4 (vec4 setter)
#include "monolib/math/CVec3.hpp"  // ml::CVec3 (light position vec, returned by value)
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"  // ConstructorDestructor (__construct_new_array)

// Retail-owned data referenced by sinit_80494300 (monolib bss / sdata2 slice).
extern CTaskGame_setVec4_tmp lbl_eu_80658648;
extern f32 lbl_eu_8066AA14;
extern f32 lbl_eu_8066AA18;

// Blend-factor step/clamp constants for scnVlBlendStep (sdata2).
extern f32 lbl_eu_8066AA78;
extern f32 lbl_eu_8066AA24;

// sdata2 constants for the light manager ctor / init walks (scnVlInitLights /
// scnVlInitLights2) and the slot-distance loop (scnVlUpdate).
extern f32 lbl_eu_8066AA10;
extern f32 lbl_eu_8066AA1C;
extern f32 lbl_eu_8066AA20;
extern f32 lbl_eu_8066AA28;
extern f32 lbl_eu_8066AA2C;
extern f32 lbl_eu_8066AA30;
extern f32 lbl_eu_8066AA34;
extern f32 lbl_eu_8066AA38;
extern f32 lbl_eu_8066AA3C;
extern f32 lbl_eu_8066AA40;
extern f32 lbl_eu_8066AA44;
extern f32 lbl_eu_8066AA48;
extern f32 lbl_eu_8066AA4C;
extern f32 lbl_eu_8066AA50;
extern f32 lbl_eu_8066AA54;
extern f32 lbl_eu_8066AA58;
extern f32 lbl_eu_8066AA5C;
extern f32 lbl_eu_8066AA60;
extern f32 lbl_eu_8066AA64;
extern f32 lbl_eu_8066AA68;
extern f32 lbl_eu_8066AA6C;
extern f32 lbl_eu_8066AA70;
extern f32 lbl_eu_8066AA74;

// Debug region name for the light-manager allocation (create() arg, .rodata
// string).
extern const char lbl_eu_80524044[];

// Vec4 scale helper (defined in kyoshin/cf/CTaskEnvironment.cpp, flat-name
// symbol): out = in * s with w unscaled.
namespace ml {
struct CVec4;  // full definition in monolib/math/CVec4.hpp
}
extern "C" void func_80058BD8(ml::CVec4* out, const ml::CVec4* in, float s);

// Single-float math helpers used by func_804933AC's direction-vector to
// dir-light-angle conversion (flat-name retail symbols; not yet decompiled).
extern "C" f32 atan2AnimFIdx(f32 value);
extern "C" f32 cfCam_mulSda2Const(f32 value);
extern "C" f32 safeFSqrt(f32 value);

// Word-array copy helper (CTaskEnvironment.cpp, flat-name symbol). Returns
// the destination pointer (retail scnVlInitLights threads the result).
extern "C" u32* copyWord4(u32* dst, const u32* src);

// Same-TU vec4-add helper (defined in CScnVirtualLight.cpp): re-declared with
// C linkage so scnVlBlendStep's call reloc stays flat.
extern "C" void scnVlVec4Add(f32* a, const f32* b);

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
    u32 value30;   // flags word (tested by the testFlagMask flag helper)
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
    f32 field_0xBC;     // blend factor, advanced/clamped by scnVlBlendStep
    u8 _C0[0x10];
    u8 enabled;
    u8 _D1[0x3];
    f32 valueD4;
    u8 _D8[0x3A8];
    u32 value480;
    u32 value484;
};

// Value-source node for the scnVlLoadValue / scnVlCopyValue value wrappers:
// scnVlLoadValue reads the value through one pointer hop at +0x04.
struct CScnVirtualLightValueSrc {
    u32 field_00;
    u32* mValue;   // +0x04
};

class CScnCameraMan;

// Camera-item lookup (defined in CScnCameraMan.cpp under the retail flat
// symbol). CScn.hpp only declares the 1-arg stub; this TU's scnVlGetCamId
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
// linkage here so func_804933AC's call reloc stays flat (retail scnVlCreateDir);
// the plain definition below inherits the linkage.
extern "C" CVirtualLightDir* scnVlCreateDir(CScnVirtualLightData* self, u8* field,
                                           const CTaskGame_setVec4_tmp* data, f32 val1,
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
// (scnVlCreateAmb / func_804933AC call sites) stay byte-identical; the
// definitions below inherit the linkage and keep emitting the flat symbol.
extern "C" void scnVlInsertItem(CScnVirtualLightData* self, u32 arg2);
extern "C" void scnVlSetIntens(void* self, float val);

// Value-compare helper (defined in CScnVirtualLight.cpp): != on two words.
// Same-TU C-linkage redeclaration keeps scnVlFindNode's call reloc flat.
extern "C" u32 scnVlWordsNe(const u32* a, const u32* b);

// Ring-walk helpers (defined in CScnVirtualLight.cpp, flat-name symbols):
// scnVlNodeItem returns the next node (+8 of *self), scnVlAdvance advances
// a node to its successor, scnVlCopyWord copies one word. Declared here so
// scnVlFindNode (defined before them in the TU) can call them with flat
// relocs.
extern "C" u32* scnVlNodeItem(void* self);
extern "C" void scnVlAdvance(u32* self);
extern "C" int* scnVlCopyWord(int* dst, int* src);  // returns dst (retail keeps r3)

// Reslist iterator helpers used by the dir-light apply loops (scnVlApplyDir6C /
// scnVlApplyDir4C). Declared with C linkage so those call relocs stay flat
// (retail flat names); the plain definitions below inherit the linkage.
extern "C" void scnVlLoadValue(CScnVirtualLightData* self, CScnVirtualLightValueSrc* src);
extern "C" void scnVlCopyValue(void* self, void* src);
extern "C" s32 scnVlGetType(void* self);
extern "C" s32 scnVlIterNe(const u32* a, const u32* b);
extern "C" int* scnVlCopyInt(int* dst, int* src);
extern "C" void* scnVlIterItem(void* self);
extern "C" void scnVlIterNext(u32* self);

// Same-TU helpers used by the ctor / light-init walks / per-frame update
// (scnVlInitLights, scnVlInitLights2, func_80493F08, scnVlUpdate). Declared
// with C linkage so those call relocs stay flat (retail flat names); the
// plain definitions below inherit the linkage.
class CScnVirtualLight;  // full definition below
struct CScnVirtualLightNode;  // full definition below
class CLightEnv;       // full definition in CVirtualLightObj.hpp
class CScnEnvLgtData;  // full definition in code_804BF59C.hpp
extern "C" void scnVlInitVec4(void* self);
extern "C" void scnVlReserve(void* self, u32 handle, int capacity);
extern "C" void scnVlInitLights(CScnVirtualLight* self);
extern "C" void scnVlInitLights2(CScnVirtualLight* self);
extern "C" void scnVlMakeAmb0C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg, f32 val);
extern "C" void scnVlMakeDir0C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg,
                              const CTaskGame_setVec4_tmp* val, f32 value);
extern "C" void scnVlMakeAmb4C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg, f32 val);
extern "C" void scnVlMakeAmb2C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg, f32 val);
extern "C" void scnVlMakeDir2C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg,
                              const CTaskGame_setVec4_tmp* val, f32 value);
extern "C" void scnVlMakeAmb6C(CScnVirtualLightData* self, const CTaskGame_setVec4_tmp* arg, f32 val);
extern "C" s32 scnVlTestArmed(CScnVirtualLightData* self);
extern "C" s32 scnVlTestFlags(CScnVirtualLightData* self, u32 flags);
extern "C" void scnVlVecSub(Vec* a, Vec* out, const Vec* b);
extern "C" void* scnVlGetPad04(void* self);
extern "C" void* scnVlGetOff10C(void* self);
extern "C" float scnVlGetRange(void* self);
extern "C" int scnVlGetCamId(CScnCameraMan* cam);
extern "C" u32 scnVlGetVal68(void* self);
extern "C" u32 scnVlGetSlotFld(void* self, u32 idx);
extern "C" u32 scnVlGetSlotCnt(void* self, u32 idx);
extern "C" u32 scnVlGetSlotPtr(void* self, u32 idx);
extern "C" void scnVlSetSlotFld(void* self, u32 val, u32 idx);
extern "C" void scnVlUpdate(CScnVirtualLight* self, CScnEnvLgtData* data,
                               const ml::CVec3* vec, int mode, f32 f1);
extern "C" void func_80493F08(void* unused, CLightEnv* env, CScnVirtualLightNode* node);
extern "C" float scnVlGetFade(void* self);
extern "C" u32 scnVlGetEnabled(void* self);
extern "C" void* scnVlPtrVal14(void* self);
extern "C" u32 scnVlGetHandle(void* self);
extern "C" u32 scnVlGetVal64(void* self);
extern "C" s32 scnVlTestBit0(CScnVirtualLightData* self);
extern "C" u32 scnVlGetCount(void* self);
extern "C" void* scnVlSlotAt(void* self, u32 idx);
extern "C" float scnVlGetAngleX(void* self);
extern "C" float scnVlGetAngleY(void* self);
extern "C" void* scnVlPtrDir(void* self);
extern "C" float scnVlGetColor(void* self);
extern "C" float scnVlGetSpotD(void* self);
extern "C" float scnVlGetSpotC(void* self);
extern "C" void* scnVlPtrSpotDir(void* self);
extern "C" float scnVlGetSpotB(void* self);
extern "C" float scnVlGetSpotA(void* self);

// --- Cross-TU light-env / pool imports (flat retail names) ---
class CLight;  // full definition below
namespace nw4r {
namespace g3d {
class LightObj;  // full definition below
}
}

// Scene pool backing the virtual lights (CScnVirtualLight::mSub, +0x4).
// Opaque here; only the flat getters below touch it.
struct CScnVirtualLightSrc;
// +0x7C sub-object of the pool (getSubField7C result; tested by checkBitFlag
// and passed to scnLgtGatherLights).
struct CScnVirtualLightSub;

// Pool getters (CScnItemPool.cpp / retail 0x80058E58 / 0x80059F74).
extern "C" u32 CScnItemPool_resolveScene(CScnVirtualLightSrc* self);
extern "C" CScnVirtualLightSub* getSubField7C(CScnVirtualLightSrc* self);
extern "C" u32 checkBitFlag(CScnVirtualLightSub* self);

// Light-slot selection / env-state helpers used by func_804936AC's per-frame
// environment push (CVirtualLightObj.cpp / CScnEnvLgtCtrl.cpp flat-name
// imports). scnLgtDispatchEnv's definition is a no-arg catalog stub; the retail
// call site passes (sub, env), so the full ABI pair is declared here.
extern "C" void func_80495644(CLightEnv* self, u32 idx);
extern "C" void func_804956F8(void* self);
extern "C" void scnLgtDispatchEnv(CScnVirtualLightSub* sub, CLightEnv* env);

// Runtime array constructor (retail __construct_new_array; block layout
// includes the size/count header). ctor/dtor take MWCC ConstructorDestructor
// (void*) - constructor/destructor addresses convert to it via the compiler
// extension (MWCPlusLib.h).
extern "C" void* __construct_new_array(void* block, ConstructorDestructor ctor,
                                       ConstructorDestructor dtor, u32 size,
                                       u32 count);

// Light-setup imports (CLight.cpp / CVirtualLightObj.cpp / CScnEnvLgtCtrl.cpp).
extern "C" void func_804C0398(CLight* self, nw4r::g3d::LightObj* lightObj);
extern "C" void func_804C02E4(CLight* self, u32 value);
extern "C" void func_804C0928(CLight* self, f32 intensity);
extern "C" void scnLgtGatherLights(CScnVirtualLightSub* self, f32 f, CScnEnvLgtData* data,
                               const ml::CVec3* vec);
extern "C" void func_804952C4(CLightEnv* self, const ml::CVec3* v);
extern "C" void func_80495704(CLightEnv* self, u32 a, f32 f1, f32 f2);
extern "C" void func_804957E4(CLightEnv* self, u32 a, u32 b, f32 f);
extern "C" void func_804958B8(CLightEnv* self, f32 a, f32 b, void* c, void* d,
                               f32 e, f32 f);
extern "C" void func_80495AF4(CLightEnv* env, CScnEnvLgtData* data, int slot);

// Scene light-data update imports (code_804BF59C.cpp). The 5-arg shape is
// the retail call seen at the scnVlUpdate call site (reset in r6, float
// in f1); the code_804BF59C.hpp declaration is a 4-arg stub.
extern "C" void func_804BFA70(CScnEnvLgtData* self, const ml::CVec3* vec, int mode,
                              int reset, f32 f);
extern "C" void func_804BFEB4(CScnEnvLgtData* self);

// Vector builders (retail 3-float setter / vec3 copy / vec helpers).
extern "C" ml::CVec3* writeVec3f(ml::CVec3* out, f32 x, f32 y, f32 z);
extern "C" ml::CVec3* scaleVec3f(ml::CVec3* out, const ml::CVec3* in, f32 f);
extern "C" void noopAnimVec3(ml::CVec3* p);
extern "C" f32 cfCam_vecLenSq(const ml::CVec3* v);

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
    u8 _pad[0x40];   // size 0x40 (retail CLight stride; only the dtor is real here)
};

class CVirtualLightObj {
public:
    ~CVirtualLightObj();
};

// Addresses of the CLight / nw4r LightObj constructors and destructors for
// __construct_new_array (retail reloc names __ct__6CLightFv /
// __dt__6CLightFv / __ct__Q34nw4r3g3d8LightObjFv /
// __dt__Q34nw4r3g3d8LightObjFv). MWCC cannot take &Class::Class, so the
// symbols are referenced through their flat emitted names (cf. CNand.cpp).
extern "C" void __ct__6CLightFv(void* self);
extern "C" void __dt__6CLightFv(void* self, int flag);
extern "C" void __ct__Q34nw4r3g3d8LightObjFv(void* self);
extern "C" void __dt__Q34nw4r3g3d8LightObjFv(void* self, int flag);

// Virtual-dispatch view of the light object (CVirtualLightObj-derived) used
// by func_80493F08: slot +0x0C returns the light's position as ml::CVec3 by
// value. Declared-only / abstract; never instantiated (the retail object's
// own vtable is dispatched through). MWCC emits two vtable header slots, so
// the dtor lands at +0x08 and the first user virtual at +0x0C.
class CVirtualLightDispatch {
public:
    virtual ~CVirtualLightDispatch() = 0;   // +0x08
    virtual ml::CVec3 GetPosition() = 0;    // +0x0C
};

// Virtual-dispatch view of the allocation-source object behind CScnItemPool_resolveScene
// (the pool's +0x8C): slot +0x2C returns the ALLOC_HANDLE the ctor passes to
// mtl::MemManager::create. Declared-only / abstract; never instantiated.
// dtor + 8 filler virtuals place GetHandle at +0x2C (after the two header
// slots and the dtor).
class CScnVirtualLightHandleSrc {
public:
    virtual ~CScnVirtualLightHandleSrc() = 0;  // +0x08
    virtual void vfn01() = 0;                  // +0x0C
    virtual void vfn02() = 0;                  // +0x10
    virtual void vfn03() = 0;                  // +0x14
    virtual void vfn04() = 0;                  // +0x18
    virtual void vfn05() = 0;                  // +0x1C
    virtual void vfn06() = 0;                  // +0x20
    virtual void vfn07() = 0;                  // +0x24
    virtual void vfn08() = 0;                  // +0x28
    virtual u32 GetHandle() = 0;               // +0x2C
};

// Minimal view of nw4r::g3d::LightObj for the CScnVirtualLight dtor's
// `delete[] mLightObjs` (the retail emits __destroy_new_array with the
// out-of-line dtor __dt__Q34nw4r3g3d8LightObjFv). Declared-only here so MWCC
// treats the dtor as non-trivial without emitting a local copy.
namespace nw4r {
namespace g3d {
class LightObj {
public:
    ~LightObj();
    u8 _pad[0x44];   // size 0x44 (retail LightObj stride; only the dtor is real here)
};
}
}

// Scene light manager: vtable slot at +0 (stored manually from
// lbl_eu_8056E868), four CVirtualLightObjPtr reslist members (+0x0C..+0x8B)
// holding the amb/dir light nodes, three vec4 blend fields (+0x8C..+0xBB)
// and the light arrays / allocations torn down by the dtor (+0xC0..+0xC8).
// __declspec(novtable) suppresses MWCC's automatic vtable stores (the dtor
// stores lbl_eu_8056E868 manually); the virtual dtor still supplies the
// deleting epilogue (flag > 0 -> __dl__FPv).
class __declspec(novtable) CScnVirtualLight {
public:
    virtual ~CScnVirtualLight();

    CScnVirtualLightSrc* mSub;      // +0x04 (scene pool; ctor arg)
    u32 value08;                    // +0x08 (ALLOC_HANDLE, erased by the dtor)
    CScnVirtualLightReslist res_0C; // +0x0C
    CScnVirtualLightReslist res_2C; // +0x2C
    CScnVirtualLightReslist res_4C; // +0x4C
    CScnVirtualLightReslist res_6C; // +0x6C
    f32 field_0x8C[4];              // +0x8C (blend target)
    f32 field_0x9C[4];              // +0x9C (blend start)
    f32 field_0xAC[4];              // +0xAC (blend current)
    f32 field_0xBC;                 // +0xBC (blend factor)
    CLight* mCLights;               // +0xC0 (delete[]'d by the dtor)
    u32* mExtraAlloc;               // +0xC4 (deallocated by the dtor; 128 u32 slots)
    nw4r::g3d::LightObj* mLightObjs;  // +0xC8 (delete[]'d by the dtor)
    u32 valueCC;                    // +0xCC (slot bound in scnVlUpdate)
    u8 enabled;                     // +0xD0 (armed flag, set to 1 by the ctor)
    u8 _D1[3];                      // +0xD1
    f32 valueD4;                    // +0xD4
    u8 _D8[0xA8];                   // +0xD8..+0x17F
    u32 value480;                   // +0x180
};

// Flat-name ctor (retail __ct__CScnVirtualLight is unmangled; cf.
// CScnItemLightNw4r.cpp). Stores the vtable manually (novtable), keeps the
// scene pool at +0x4 and seeds the allocation handle.
extern "C" CScnVirtualLight* __ct__CScnVirtualLight(CScnVirtualLight* self,
                                                    CScnVirtualLightSrc* src);

// CScnVirtualLight vtable (retail .data). Array-typed so MWCC emits absolute
// lis/addi addressing for the dtor's manual store.
extern u8 lbl_eu_8056E868[];

// reslist<CVirtualLightObj> teardown (nw4r library, flat-name symbol): called
// explicitly with flag -1 for each reslist member at the end of the
// CScnVirtualLight dtor.
extern "C" void __dt__reslist_CVirtualLightObj(CScnVirtualLightReslist* self, int flag);

// Scene-light reset helper (defined later in CScnVirtualLight.cpp; retail
// func_804923F8, a large init walk). Called by the dtor with a 0 flag.
extern "C" void func_804923F8(CScnVirtualLight* self, int arg);

// One slot of the virtual-light pool (0x40-byte stride). Content is opaque
// here; the pool only allocates/frees whole slots.
struct CScnVirtualLightPoolSlot {
    u8 data[0x40];
};

// Pool backing CScnVirtualLight's 128-slot light array (the object CETrail
// passes as CResHolder::m_res / light allocation source): record array
// (+0xC0, 0x40-byte stride), parallel handle array (+0xC4) and slot count
// (+0xCC). scnVlAllocSlot allocates a free slot, scnVlFreeSlot frees one.
struct CScnVirtualLightPool {
    u8 _00[0xC0];
    CScnVirtualLightPoolSlot* mRecords;  // +0xC0
    CScnVirtualLightPoolSlot** mHandles; // +0xC4
    u8 _C8[0x04];
    u32 mCount;                          // +0xCC
};
