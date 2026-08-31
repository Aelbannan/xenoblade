#include "types.h"
#include "kyoshin/cf/CfCollSphereImpl.hpp"
#include "kyoshin/cf/object/CfObjectColl.hpp"

extern "C" const double lbl_eu_80666920;
static inline float cvt_u32_f32(u32 v) {
    union { double d; unsigned int i[2]; } u;
    u.i[0] = 0x43300000;
    u.i[1] = v;
    double d = u.d - lbl_eu_80666920;
    return (float)d;
}
// func_800AAD28: debug draw for sphere collision shape (single call).
extern "C" void func_800AAD28(void* context, cf::CfObjectColl* shape) {
    u32 uval = static_cast<u32>(shape->field_0xB8);
    ml::CVec3* pos = shape->CfObject_UnkVirtualFunc23();
    renderSphere__Q22cf18CfDebugDrawManagerFv(reinterpret_cast<void*>(pos), cvt_u32_f32(uval));
}

// func_800AAD94: debug draw for sphere with extra transform params.
extern "C" void func_800AAD94(void* context, cf::CfObjectColl* shape, void* a, void* b) {
    u32 uval = static_cast<u32>(shape->field_0xB8);
    ml::CVec3* pos = shape->CfObject_UnkVirtualFunc23();
    func_800A5738(a, pos, cvt_u32_f32(uval), b);
}

// --- absorb: retail data sections (split1) ---
// .rodata 0x70
__declspec(section ".rodata") __attribute__((aligned(8), used)) const unsigned char __rodata_CfCollSphereImpl[0x70] = {
    0x63,0x66,0x3A,0x3A,0x43,0x66,0x4F,0x62,0x6A,0x65,0x63,0x74,0x43,0x6F,0x6C,0x6C,0x00,0x00,0x00,0x00,
    0x63,0x66,0x3A,0x3A,0x43,0x66,0x43,0x6F,0x6C,0x6C,0x43,0x61,0x70,0x73,0x75,0x6C,0x65,0x49,0x6D,0x70,0x6C,0x00,0x00,0x00,
    0x63,0x66,0x3A,0x3A,0x43,0x66,0x43,0x6F,0x6C,0x6C,0x43,0x79,0x6C,0x69,0x6E,0x64,0x65,0x72,0x49,0x6D,0x70,0x6C,0x00,
    0x00,
    0x63,0x66,0x3A,0x3A,0x43,0x66,0x43,0x6F,0x6C,0x6C,0x41,0x41,0x42,0x42,0x49,0x6D,0x70,0x6C,0x00,
    0x00,
    0x63,0x66,0x3A,0x3A,0x43,0x66,0x43,0x6F,0x6C,0x6C,0x53,0x70,0x68,0x65,0x72,0x65,0x49,0x6D,0x70,0x6C,0x00,0x00,0x00,0x00,
};

// .data 0x220 — 113 pointer relocs, 23 immediates (all zero)
extern "C" {
extern char lbl_eu_80661C78[];
extern char lbl_eu_80661C80[];
extern char lbl_eu_80661C88[];
extern char lbl_eu_80661C90[];
extern char lbl_eu_80661C98[];
extern char lbl_eu_80661CA8[];
extern char lbl_eu_80661CB0[];
extern char lbl_eu_806618F0[];
extern char lbl_eu_806618F8[];
extern char lbl_eu_80661900[];
extern void CObjectState_UnkVirtualFunc1__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc2__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc3__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc4__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc5__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc6__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc7__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc10__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc11__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc12__Q22cf12CObjectStateFv();
extern void CObjectState_UnkVirtualFunc13__Q22cf12CObjectStateFv();
extern void CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv();
extern void CObjectParam_UnkVirtualFunc2__Q22cf12CObjectParamFv();
extern void CObjectParam_UnkVirtualFunc3__Q22cf12CObjectParamFv();
extern void loadPointData__Q22cf13CfObjectPointFv();
extern void CObjectParam_UnkVirtualFunc5__Q22cf12CObjectParamFv();
extern void CObjectParam_UnkVirtualFunc6__Q22cf12CObjectParamFv();
extern void __dt__Q22cf12CfObjectCollFv();
extern void resetCollTimer__Q22cf12CfObjectCollFv();
extern void enableCollision__Q22cf12CfObjectCollFv();
extern void updatePointState__Q22cf13CfObjectPointFv();
extern void CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv();
extern void releasePointLink__Q22cf13CfObjectPointFv();
extern void refreshCollLink__Q22cf12CfObjectCollFv();
extern void setChildPoint__Q22cf13CfObjectPointFv();
extern void CfObject_UnkVirtualFunc9__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc10__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc13__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc14__Q22cf8CfObjectFf();
extern void CfObject_UnkVirtualFunc15__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc16__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc17__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc18__Q22cf8CfObjectFv();
extern void func_80047814__Q22cf13CfObjectPointFv();
extern void setPointPosition__Q22cf13CfObjectPointFv();
extern void CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv();
extern void syncCollVectors__Q22cf12CfObjectCollFv();
extern void CfObject_UnkVirtualFunc23__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc24__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv();
extern void copyCollPosition__Q22cf12CfObjectCollFv();
extern void CfObject_UnkVirtualFunc27__Q22cf8CfObjectFPv();
extern void CfObject_UnkVirtualFunc28__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc29__Q22cf8CfObjectFf();
extern void CfObject_UnkVirtualFunc30__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc31__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc32__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc33__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc34__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc35__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc36__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc46__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc47__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc48__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc49__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc50__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc53__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc54__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc56__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc58__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc61__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc63__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc64__Q22cf8CfObjectFi();
extern void CfObject_UnkVirtualFunc65__Q22cf8CfObjectFv();
extern void setPointEnabled__Q22cf13CfObjectPointFv();
extern void CfObject_UnkVirtualFunc67__Q22cf8CfObjectFv();
extern void isCollEnabled__Q22cf13CfObjectPointFv();
extern void CfObject_UnkVirtualFunc69__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc70__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc71__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc72__Q22cf8CfObjectFv();
extern void CfObject_UnkVirtualFunc73__Q22cf8CfObjectFv();
extern void func_800AB2E4();
extern void func_800AB3B8();
extern void func_800AB010();
extern void func_800AB248();
extern void func_800AAE24();
extern void func_800AAFF4();
}
__declspec(section ".data") __attribute__((aligned(8), used)) const void* __data_CfCollSphereImpl[136] = {
    &lbl_eu_80661C78, (void*)0,
    &CObjectState_UnkVirtualFunc1__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc2__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc3__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc4__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc5__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc6__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc7__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc8__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc9__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc10__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc11__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc12__Q22cf12CObjectStateFv,
    &CObjectState_UnkVirtualFunc13__Q22cf12CObjectStateFv,
    &CObjectParam_UnkVirtualFunc1__Q22cf12CObjectParamFv,
    &CObjectParam_UnkVirtualFunc2__Q22cf12CObjectParamFv,
    &CObjectParam_UnkVirtualFunc3__Q22cf12CObjectParamFv,
    &loadPointData__Q22cf13CfObjectPointFv,
    &CObjectParam_UnkVirtualFunc5__Q22cf12CObjectParamFv,
    &CObjectParam_UnkVirtualFunc6__Q22cf12CObjectParamFv,
    &__dt__Q22cf12CfObjectCollFv,
    &resetCollTimer__Q22cf12CfObjectCollFv,
    &enableCollision__Q22cf12CfObjectCollFv,
    &updatePointState__Q22cf13CfObjectPointFv,
    &CfObject_UnkVirtualFunc5__Q22cf8CfObjectFv,
    &releasePointLink__Q22cf13CfObjectPointFv,
    &refreshCollLink__Q22cf12CfObjectCollFv,
    &setChildPoint__Q22cf13CfObjectPointFv,
    &CfObject_UnkVirtualFunc9__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc10__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc11__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc12__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc13__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc14__Q22cf8CfObjectFf,
    &CfObject_UnkVirtualFunc15__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc16__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc17__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc18__Q22cf8CfObjectFv,
    &func_80047814__Q22cf13CfObjectPointFv,
    &setPointPosition__Q22cf13CfObjectPointFv,
    &CfObject_UnkVirtualFunc21__Q22cf8CfObjectFv,
    &syncCollVectors__Q22cf12CfObjectCollFv,
    &CfObject_UnkVirtualFunc23__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc24__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv,
    &copyCollPosition__Q22cf12CfObjectCollFv,
    &CfObject_UnkVirtualFunc27__Q22cf8CfObjectFPv,
    &CfObject_UnkVirtualFunc28__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc29__Q22cf8CfObjectFf,
    &CfObject_UnkVirtualFunc30__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc31__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc32__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc33__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc34__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc35__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc36__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc37__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc38__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc39__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc40__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc41__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc42__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc43__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc44__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc45__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc46__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc47__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc48__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc49__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc50__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc51__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc52__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc53__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc54__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc55__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc56__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc57__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc58__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc59__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc60__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc61__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc62__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc63__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc64__Q22cf8CfObjectFi,
    &CfObject_UnkVirtualFunc65__Q22cf8CfObjectFv,
    &setPointEnabled__Q22cf13CfObjectPointFv,
    &CfObject_UnkVirtualFunc67__Q22cf8CfObjectFv,
    &isCollEnabled__Q22cf13CfObjectPointFv,
    &CfObject_UnkVirtualFunc69__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc70__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc71__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc72__Q22cf8CfObjectFv,
    &CfObject_UnkVirtualFunc73__Q22cf8CfObjectFv,
    &lbl_eu_80661900, (void*)0,
    &lbl_eu_806618F8, (void*)0,
    &lbl_eu_806618F0, (void*)0,
    &lbl_eu_80661CB0, (void*)0, (void*)0, (void*)0,
    &lbl_eu_80661C80, (void*)0, &func_800AB2E4, &func_800AB3B8,
    &lbl_eu_80661CA8, (void*)0, (void*)0, (void*)0,
    &lbl_eu_80661C88, (void*)0, &func_800AB010, &func_800AB248,
    &lbl_eu_80661CA8, (void*)0, (void*)0, (void*)0,
    &lbl_eu_80661C90, (void*)0, &func_800AAE24, &func_800AAFF4,
    &lbl_eu_80661CA8, (void*)0, (void*)0, (void*)0,
    &lbl_eu_80661C98, (void*)0, &func_800AAD28, &func_800AAD94,
    &lbl_eu_80661CA8, (void*)0, (void*)0, (void*)0,
};

// .sdata 0x28 — 10 pointer relocs
extern "C" {
extern char lbl_eu_804FC088[];
extern char lbl_eu_804FC09C[];
extern char lbl_eu_804FC0B4[];
extern char lbl_eu_804FC0CC[];
extern char lbl_eu_804FC0E0[];
extern char lbl_eu_80528778[];
extern char lbl_eu_805287B0[];
extern char lbl_eu_805287D0[];
extern char lbl_eu_805287F0[];
extern char lbl_eu_80528810[];
}
__declspec(section ".sdata") __attribute__((aligned(8), used)) const void* __sdata_CfCollSphereImpl[10] = {
    &lbl_eu_804FC088, &lbl_eu_80528778,
    &lbl_eu_804FC09C, &lbl_eu_805287B0,
    &lbl_eu_804FC0B4, &lbl_eu_805287D0,
    &lbl_eu_804FC0CC, &lbl_eu_805287F0,
    &lbl_eu_804FC0E0, &lbl_eu_80528810,
};
