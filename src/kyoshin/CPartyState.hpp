#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "kyoshin/CBaseCur.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"

/* Cursor widget embedded at +0x34 (loads layout "curs22"). Its constructor
   and destructor are defined in the CCur unit (__ct__CCur22 / __dt__6CCur22Fv);
   CPartyState's destructor calls __dt__6CCur22Fv explicitly. */

class CPartyState {
public:
    CPartyState();
    ~CPartyState();
    void OnFileEvent();
    u8 func_801FD17C();
    u8 func_801FD184();
    u8 func_801FD18C();
    u32 func_801FD580();
    u8 func_801FD5F4();
    u8 func_801FD5FC();

    void* mVtbl;                                // 0x00 - lbl_eu_805353C8
    UnkClass_8045F564 mMemRegion;               // 0x04 (0x10 bytes)
    CFileHandle* mFileHandle;                   // 0x14
    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // 0x18
    nw4r::lyt::Layout* mLayout;                 // 0x1C
    nw4r::lyt::AnimTransform* mAnimTrans1;      // 0x20
    nw4r::lyt::AnimTransform* mAnimTrans0;      // 0x24
    u8 field_0x28;                              // 0x28
    u8 _29[0x2C - 0x29];                        // 0x29
    s32 field_0x2C;                             // 0x2C (advance/state word; retail compares signed)
    u8 field_0x30;                              // 0x30
    u8 field_0x31;                              // 0x31
    u8 _32[0x34 - 0x32];                        // 0x32
    CBaseCur mCur22;                            // 0x34 (0x18 bytes)
    u8 field_0x4C;                              // 0x4C
    s8 field_0x4D;                              // 0x4D
    u8 field_0x4E;                              // 0x4E
    u8 field_0x4F[0x57 - 0x4F];                 // 0x4F - per-member slot byte (pane-name index)
    u8 field_0x57;                              // 0x57
};

// Character/party-slot object returned by func_8009EC9C. +0x176C holds a
// state word that is 1 when the slot's character is busy (cannot be swapped).
struct CPartyCharData {
    u8 _pad[0x176C];
    s32 field_0x176C;
};

/* Vtable-dispatch view of the embedded cursor. MWCC puts two implicit dtor
   slots at the start of every polymorphic vtable, so func_801D2180 (retail
   cursor slot 3) is declared as the SECOND virtual to land at vtable offset
   0xC, matching the retail dispatch in func_801FD0F4. All-pure, never
   constructed directly; only used to reproduce the retail vtable dispatch. */
class CPartyStateCur {
public:
    virtual void vfn0() = 0;          // MWCC vtable slot 2
    virtual void func_801D2180() = 0; // MWCC vtable slot 3 (retail slot 3)
};

/* ---- imports (declared here, never locally in the .cpp) ---- */

// UI sound effect; retail symbol is the C++ mangled func_80138078__FUl.
void func_80138078(u32);

// nw4r anim-transform frame checker; retail symbol is the C++ mangled
// func_80137444__FPQ34nw4r3lyt13AnimTransformf.
u32 func_80137444(nw4r::lyt::AnimTransform*, float);

// nw4r anim-transform frame checker; retail symbol is unmangled (C linkage).
extern "C" u32 func_80137510(nw4r::lyt::AnimTransform*, float);

// Layout/accessor release helpers (C++ mangled retail symbols, mirroring the
// canonical declarations in code_80135FDC.hpp, which conflicts with
// CEquipItemBox.hpp's func_80136190 in this TU).
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);

// C-linkage helpers from other units (retail symbols are unmangled).
// func_801D216C / func_80139198 are declared in CEquipItemBox.hpp.
extern "C" void func_801D2BFC(CBaseCur*, u8);
extern "C" u8 code80135FDC_getByte_64077();

// Party-select helpers (unmangled retail symbols).
extern "C" int* func_8009ECB0();
extern "C" void func_8009E168(int*, u8, u8);

// cf::CfGameManager helper. US retail keeps the no-arg Fv suffix but the call
// site passes two arguments; declare the retail symbol verbatim under C
// linkage (a C++ declaration would re-mangle the suffix into an overload).
extern "C" void func_80080888__Q22cf13CfGameManagerFv(u32, u32);

// CDeviceFile::getFileSize - retail symbol keeps the single-arg FPCc suffix
// but call sites pass a second flags argument; the C++ declaration in
// CDeviceFile.hpp only covers the single-arg form.
extern "C" int getFileSize__11CDeviceFileFPCc(const char*, int);

// Scene object behind the common-archive alloc handle (virtual at vtable
// 0x2C of *lbl_eu_80663E14; returns an mtl alloc handle).
class CScn;
extern CScn* lbl_eu_80663E14;
extern "C" u32 func_80495FF0(CScn*);

// Party-state layout arc path pointer (.sdata).
extern char* lbl_eu_80662728;

// Unit-local helpers (defined/stubbed in this TU; unmangled retail symbols).
extern "C" void func_801FE154(CPartyState*);
extern "C" void func_801FD848(CPartyState*);

// Cursor per-frame update (defined in the CCur unit).
extern "C" void func_801D202C(CBaseCur*);

// Embedded cursor ctor/dtor (defined in the CCur unit).
extern "C" CBaseCur* __ct__CCur22(CBaseCur*, nw4r::lyt::ArcResourceAccessor*);
extern "C" CBaseCur* __dt__6CCur22Fv(CBaseCur*, int);

// Internal helpers of this unit (unmangled retail symbols).
extern "C" void func_801FD8F8(CPartyState*);
extern "C" void func_801FDA7C(CPartyState*);
extern "C" void func_801FD604(CPartyState*);
extern "C" void func_801FD48C(CPartyState*);
extern "C" void func_801FE39C(CPartyState*, float, float, u32, u32);

// C library formatted output (retail symbol unmangled).
extern "C" int sprintf(char*, const char*, ...);

// Frame-timer getter (kyoshin/cf/CfTFile.cpp, retail unmangled).
extern "C" u32 func_8006A80C();

// Text/layout binding helper (code_80135FDC.cpp, retail unmangled; the
// canonical header code_80135FDC.hpp conflicts with CEquipItemBox.hpp in
// this TU).
extern "C" void func_80136A1C(nw4r::lyt::Layout*, char*, char*, u32);

// Character-data lookup (retail unmangled).
extern "C" void* func_8009EC9C(u32);

// Cursor pane-position/show helpers (CCur unit, retail unmangled).
extern "C" void func_801D2C80(CBaseCur*, const nw4r::math::VEC3*, u8);
extern "C" void func_801D2CF4(CBaseCur*, u8, u8);

// Pane translate accumulation (code_80135FDC.cpp, retail unmangled).
extern "C" void func_801375A0(nw4r::math::VEC3*, nw4r::lyt::Pane*);

// Pane size/position copy helpers (retail unmangled).
extern "C" void func_80127BC4(float*, float*);
extern "C" void func_80124288(nw4r::lyt::Pane*, float*);

// Party-state string table (.rodata).
extern char lbl_eu_80507D40[];
// Party-state 5-float position table (.rodata).
extern float lbl_eu_80507D20[];

// Party-state panel math constants (.sdata2).
extern const float lbl_eu_80668230;
extern const float lbl_eu_80668240;
extern const float lbl_eu_80668244;
extern const float lbl_eu_80668248;

// CPartyState vtable (.data) - stored at +0 by the constructor. Declared as
// an array so MWCC uses full 32-bit (lis/addi) addressing like the retail.
extern "C" void* lbl_eu_805353C8[];

// Animation target frame for func_801FD76C (.sdata2, retail lbl_eu_80668218).
// const lets MWCC hoist the lfs to the function top (matches the matched
// CItemBoxInfo/CCollepedia frame-check shapes).
extern const float lbl_eu_80668218;
