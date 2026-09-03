// Decompiled CREvtModelMap - realtime event model map
// High-level C++ reconstruction.

#include <string.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/realtimeevt/CREvtModelMap.hpp"
#include <decomp.h>
#include "kyoshin/realtimeevt/CREvtLight.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/CfGimmick.hpp"

// Map effect object at +0x2F3C (real owner of slot 0x68) - definition mirrors
// kyoshin/cf/object/CfObjectMap.hpp UnkMapFxObj (hot header in scope conceptually,
// but redeclared here to avoid pulling that header's conflicting extern "C"
// overloads into this TU). Full virtual list ensures vfunc_0x68 lands at 0x68.
class __declspec(novtable) UnkMapFxObj {
public:
    virtual void vfunc_0x08();
    virtual void vfunc_0x0C();
    virtual void vfunc_0x10();
    virtual void vfunc_0x14();
    virtual void vfunc_0x18();
    virtual void vfunc_0x1C();
    virtual void vfunc_0x20();
    virtual void vfunc_0x24();
    virtual void vfunc_0x28();
    virtual void vfunc_0x2C();
    virtual void vfunc_0x30();
    virtual void vfunc_0x34();
    virtual void vfunc_0x38();
    virtual void vfunc_0x3C();
    virtual void vfunc_0x40();
    virtual void vfunc_0x44();
    virtual void vfunc_0x48();
    virtual void vfunc_0x4C();
    virtual void vfunc_0x50();
    virtual void vfunc_0x54();
    virtual void vfunc_0x58();
    virtual void vfunc_0x5C();
    virtual void vfunc_0x60();
    virtual void vfunc_0x64();
    virtual void vfunc_0x68(int arg);
    virtual void vfunc_0x6C(u32 arg);
    virtual void vfunc_0x70();
    virtual void vfunc_0x74();
    virtual void vfunc_0x78();
    virtual void vfunc_0x7C();
    virtual void vfunc_0x80();
    virtual void vfunc_0x84(u32 r4, u32 r5);
    virtual void vfunc_0x88(u32 r4, u32 r5, void* r6, void* r7);
    virtual void vfunc_0x8C(u32 r4, u32 r5, void* r6);
    virtual void vfunc_0x90();
    virtual void vfunc_0x94(u32 r4, u32 r5, u32 r6, u32 r7, u32 r8, u32 r9, u32 r10);
};

#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"
#include "include/kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

// ---------------------------------------------------------------------------
// Action table entries - {startTime, creatureId, callback}
// ---------------------------------------------------------------------------
struct ActionEntry {
    s32 startTime;
    s32 creatureId;
    void* callback;
};






__declspec(section ".rodata") const unsigned char rodata_all[48] __attribute__((used, aligned(1))) = {
    0x63,0x66,0x3a,0x3a,0x43,0x52,0x45,0x76,0x74,0x4d,0x6f,0x64,0x65,0x6c,0x4d,0x61,
    0x70,0x00,0x00,0x00,0x2e,0x6c,0x6f,0x64,0x00,0x2f,0x6f,0x62,0x6a,0x2f,0x00,0x2e,
    0x6d,0x61,0x70,0x00,0x2e,0x6c,0x67,0x74,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
#define lbl_eu_80503718 ((const char*)rodata_all)
#define lbl_eu_8050372C ((const char*)(rodata_all+20))





