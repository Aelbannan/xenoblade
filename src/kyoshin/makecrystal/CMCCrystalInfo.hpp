#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"

// vtable at lbl_eu_80535CF8
extern "C" void* lbl_eu_80535CF8[];

// 4 packed shorts returned in r3:r4 by func_801397AC.
struct FourShorts { s16 a, b, c, d; };

// Item/crystal data header read by func_8021A9A8. word0 packs the item code
// (bits 20-31) and a type field (bits 12-15); byte 7 holds flags in bits 0-1.
struct CMCCItemData {
    u32 word0;   // 0x00
    u8 field04;  // 0x04
    u8 field05;  // 0x05
    u8 field06;  // 0x06
    u8 field07;  // 0x07
};

// Crystal result buffer written by func_8021B188 and consumed by
// func_8021B2E0. field21 tracks how many entries were stored while filling.
// Sized 0x30 to match the retail stack allocation (only the first 0x24
// bytes are copied to the caller).
struct CrystalItemBuf {
    u8 count;          // 0x00 number of matching crystal items
    u8 pad00[3];       // 0x01
    char* str;         // 0x04 description string
    char* names[4];    // 0x08 item name pointers (valid up to count)
    u8 flags[4];       // 0x1C per-item flags
    u8 field20;        // 0x20
    u8 field21;        // 0x21 running count of stored entries
    u8 pad22[14];      // 0x22 (pad to 0x30)
};

// Fake polymorphic facade over the object returned by
// CItem_initItemImplInstances (real class layout unknown; only the dispatch
// slots 2/19/25 at vtable offsets 0x8/0x4C/0x64 are used). novtable keeps
// MWCC from emitting a vtable — the object is created elsewhere and the
// dispatch loads the vptr from the object at runtime (retail lwz r12 form).
// MWCC reserves vtable slots 0-1 for the implicit dtor pair, so the first
// declared virtual lands at slot 2 (offset 0x08).
class __declspec(novtable) CItemImplInstancesFacade {
public:
    virtual u8 GetCount(void* item);         // [2] = 0x08
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual u16 GetName(void* item, u8 idx); // [19] = 0x4C
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual u8 GetFlag(void* item, u8 idx);  // [25] = 0x64
};

/* Crystal info for the makecrystal UI. Manages crystal information display
   state. Inherits from IWorkEvent for file-load callbacks. */
class CMCCrystalInfo : public IWorkEvent {
public:
    CMCCrystalInfo();
    virtual ~CMCCrystalInfo();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    /* 0x00 */ // IWorkEvent base (vptr)
    /* 0x04 */ UnkClass_8045F564 mMemRegion1;
    /* 0x14 */ UnkClass_8045F564 mMemRegion2;
    /* 0x24 */ u32 mFileHandle1;
    /* 0x28 */ u32 mFileHandle2;
    /* 0x2C */ void* mArcResAccessor;     // nw4r::lyt::ArcResourceAccessor*
    /* 0x30 */ u32 mField30;
    /* 0x34 */ void* mLayout;              // nw4r::lyt::Layout*
    /* 0x38 */ void* mAnimTransform1;      // nw4r::lyt::AnimTransform*
    /* 0x3C */ void* mAnimTransform2;      // nw4r::lyt::AnimTransform*
    /* 0x40 */ void* mAnimTransform3;      // nw4r::lyt::AnimTransform*
    /* 0x44 */ void* mAnimTransform4;      // nw4r::lyt::AnimTransform*
    /* 0x48 */ u8 mEnabled;                // Set when layout is ready; gates proc/draw
    /* 0x4C */ s32 mState;                 // State (0=inactive, 1/2/4-8=active steps)
    /* 0x50 */ u8 mIsActive;               // Active flag; set when layout initialised
    u8 func_8021A8EC() const { return mIsActive; }
    /* 0x51 */ u8 mField51;                // Cleared when state transitions from 0→1
};

// ---------------------------------------------------------------------------
// Imports. All retail names here are unmangled, so plain global C++ (MWCC
// does not mangle global-scope names) binds to the same symbols. The special
// MWCC ctor/dtor/delete helpers below keep extern "C".
// ---------------------------------------------------------------------------
void func_8021B52C(CMCCrystalInfo* self);
void func_8021B5B4(CMCCrystalInfo* self);
void func_8021B63C(CMCCrystalInfo* self);
void func_8021B6C4(CMCCrystalInfo* self);
extern char lbl_eu_80508DF8[];
extern const float lbl_eu_80668498;   // 1.0f animation advance constant (.sdata2)
extern u32 func_801355BC();
extern FourShorts func_801397AC(void*, u32);
extern void CopyVec4s(short* dst, const short* src);
extern "C" void* lbl_eu_806646D8;
extern "C" void* lbl_eu_806646E0;
extern "C" void* lbl_eu_806646E8;
extern "C" void* lbl_eu_806646F0;
extern "C" void* lbl_eu_806646F8;
extern "C" void* lbl_eu_80664700;
extern "C" void* lbl_eu_80664708;
extern "C" void* lbl_eu_80664710;
extern "C" void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dl__FPv(void*);

// Cross-TU imports with unmangled retail symbols (C linkage required so call
// relocs bind to the retail names, not MWCC-mangled C++ forms).
extern "C" u32 func_801392E4(u32);
extern "C" u16 func_80139358(u32);
extern "C" void* CItem_initItemImplInstances(void*);
