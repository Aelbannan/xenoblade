#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CFileHandle;

class CSLCur {
public:
    CSLCur();
    void func_8028EA74();
    void func_8028EEC0();

    // Vtable-like pointer set manually (initialized to lbl_eu_8053884C)
    void* mField0;    // 0x00
    void* mField4;    // 0x04 - ArcResourceAccessor* (or int from constructor)
    void* mField8;    // 0x08 - Layout* (or null)
    u8 mFieldC;       // 0x0C
    u8 mFieldD;       // 0x0D
    u8 mFieldE;       // 0x0E
    u8 _pad_0F;       // 0x0F (padding byte)
    u8 mField10;      // 0x10
    u8 mField11;      // 0x11
    u8 mField12;      // 0x12
    u8 mField13;      // 0x13
    u8 mField14;      // 0x14
    u8 mField15;      // 0x15
};

class CSaveLoad {
public:
    CSaveLoad();
    virtual ~CSaveLoad();
    void func_8028F23C();
    u8 func_8028F664();
    u8 func_8028FEC4();

    // +0x00: vtable (implicit)
    char _pad_04[0x14 - 0x04];                         // 0x04-0x13
    CFileHandle* mFileHandle;                            // 0x14 - file handle from readFile
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;       // 0x18 - arc resource accessor (passed to func_80139124)
    nw4r::lyt::Layout* mLayout;                         // 0x1C - layout for animation control
    nw4r::lyt::AnimTransform* mAnimTransA;              // 0x20 - animation transform A
    nw4r::lyt::AnimTransform* mAnimTransB;              // 0x24 - animation transform B
    char _pad_28[0x3C - 0x28];                          // 0x28-0x3B
    u8 mField3C;                                         // 0x3C
    char _pad_3D[0x58 - 0x3D];                          // 0x3D-0x57
    u8 mScrollbar[0x98 - 0x58];                          // 0x58-0x97 - scrollbar state (passed to func_801F34F4/func_801F369C)
    char mSysWin98[0xD4 - 0x98];                           // 0x98-0xD3 - first CSysWin-like object
    char mSysWinD4[0x11C - 0xD4];                          // 0xD4-0x11B - second CSysWin-like object
    u8 mField11C;                                        // 0x11C
    u8 mField11D;                                        // 0x11D (padding/gap)
    u8 mField11E;                                        // 0x11E
    u8 _pad_11F;                                        // 0x11F
    u8 mField120;                                       // 0x120
    u8 mField121;                                        // 0x121
    u8 mField122;                                        // 0x122
    u8 mField123;                                        // 0x123
    u8 mField124;                                        // 0x124
    u8 _pad_125;                                         // 0x125
    u16 mField126;                                       // 0x126 (halfword cursor position)
    u8 mField128;                                        // 0x128
    u8 mField129;                                        // 0x129
    u8 mField12A;                                        // 0x12A
    u8 mField12B;                                        // 0x12B
    u8 mField12C;                                        // 0x12C
    u8 mField12D;                                        // 0x12D
    u8 mField12E;                                        // 0x12E
    u8 mField12F;                                        // 0x12F
    void* mField130;                                    // 0x130 - heap pointer to deallocate
    void* mField134;                                    // 0x134 - heap pointer to deallocate
    void* mField138;                                    // 0x138 - heap pointer to deallocate
    CSLCur mCur;                                         // 0x13C - cursor/save slot state
};

namespace cf {

struct CfAward {
    void* mVtbl;       // +0x00 - vtable pointer (points to lbl_eu_80538858 + 8)
    void* mSecondBase; // +0x04 - second vtable base (points to lbl_eu_80538858)

    // No virtual functions - vtable managed manually.
    // The destructor is a regular member function (not virtual).
    // MWCC generates the delete flag handling automatically.
    ~CfAward();
    // func_80291B18 is implemented as an extern "C" function (unmangled retail symbol)
};

} // namespace cf

// Abstract struct for CSysWin vtable dispatch at slot 34 (offset 0x88)
// MWCC adds 2 implicit entries (RTTI/dtor) before first user function.
// MWCC doesn't generate a vtable for pure abstract classes.
struct CSysWinProxy {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0; // vtable slot 34 = offset 0x88 (after 2 implicit entries)
};

// Opaque object whose first virtual (vtable offset 0x8 after RTTI pad slots) is a
// deleting-destructor-like virtual taking an int flag. Abstract (pure virtual)
// so MWCC emits no vtable.
class UnkObj {
public:
    virtual void vf2(int) = 0;
};

// Abstract class for CCur18 cursor object (0x18 bytes, embedded sub-object).
// vtable layout: 2 MWCC implicit entries (RTTI/dtor) then user virtuals.
// Slot 3 (offset 0x0C) is the second user virtual, used by func_8028F4AC.
class CCur18Obj {
public:
    virtual void vf2(int) = 0;  // slot 2 (0x08) - destructor
    virtual void vf3() = 0;     // slot 3 (0x0C) - cursor update function
};

// Struct with ArcResourceAccessor at +0x00, Layout at +0x04, AnimTransform at +0x08.
// Used by func_8028ED70; compatible with UnkTwoPtr (UnkTwoPtr's _pad_00[4] absorbs the accessor).
struct UnkED70_Struct {
    nw4r::lyt::ArcResourceAccessor* mAccessor;  // +0x00
    nw4r::lyt::Layout* mLayout;                  // +0x04
    nw4r::lyt::AnimTransform* mAnimTrans;        // +0x08
};

// Abstract class matching the vtable layout of the object pointed to by
// UnkPtrHolder::mPtr. vf2 is the first user virtual (vtable offset 0x08,
// after 2 MWCC implicit RTTI/dtor entries). Has virtuals at slots 7 (0x1C),
// 8 (0x20), 11 (0x2C), and 14 (0x38). Pure abstract so MWCC emits no vtable.
class UnkPtrObj {
public:
    virtual void vf2(int) = 0;      // slot 2 (0x8) - deleting destructor (first user virtual)
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void vf7(void*) = 0;    // slot 7 (0x1C)
    virtual void vf8(void*) = 0;    // slot 8 (0x20)
    virtual void vf9() = 0;
    virtual void vf10() = 0;
    virtual void vf11(void*, int) = 0;  // slot 11 (0x2C)
    virtual void vf12() = 0;
    virtual void vf13() = 0;
    virtual void vf14(int) = 0;     // slot 14 (0x38)
};

// Opaque object whose vtable (after 2 RTTI pad slots) has virtuals at
// slots 11 (0x2C) and 14 (0x38) - used by func_8028EF74's dispatch.
// Abstract so MWCC emits no vtable.
class UnkVtblObj {
public:
    virtual void vf2(int) = 0;
    virtual void vf3(int) = 0;
    virtual void vf4(int) = 0;
    virtual void vf5(int) = 0;
    virtual void vf6(int) = 0;
    virtual void vf7(int) = 0;
    virtual void vf8(int) = 0;
    virtual void vf9(int) = 0;
    virtual void vf10(int) = 0;
    virtual void vf11(void* arg, int flag) = 0; // vtable 0x2C
    virtual void vf12(int) = 0;
    virtual void vf13(int) = 0;
    virtual void vf14(int) = 0;                 // vtable 0x38
};

// C-linkage imports — retail symbol names; extern "C" so calls bind to the
// literal (un)mangled identifiers. (Retail names are the true symbol names;
// do not rename.) Forward decls for structs whose full definitions live in
// the TU.
struct UnkPtrHolder;
struct UnkTwoPtr;

extern "C" void func_801D216C(void*, u8);
extern "C" int func_8028E964(CSLCur* cur);
extern "C" int func_8028E998(CSLCur* cur, u8 index);
extern "C" void func_80290844(CSaveLoad* p);
extern "C" void func_802908A4(CSaveLoad* p);
extern "C" void func_80290994(CSaveLoad* p);
extern "C" void func_802907E4(CSaveLoad* p);
extern "C" void func_802910D4(CSaveLoad* p);
extern "C" void func_8028EED8(CSLCur* cur);
extern "C" void func_8028EF24(CSLCur* self);
extern "C" void* __dt__Q22cf7CfAwardFv(cf::CfAward*, int);
extern "C" u8 lbl_eu_80538858[];
extern "C" void func_8009D514(void*);
extern "C" u32 getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u8 lbl_eu_8050F7CC[];
extern "C" CSaveLoad* lbl_eu_806649F4;
extern "C" u32 lbl_eu_80662AD0;
extern "C" s32 lbl_eu_80662AC8;
extern "C" u32 lbl_eu_80662ACC;
extern "C" int CSysWin_isActive(void*);
extern "C" u32 CSysWin_isReady(void*);
extern "C" int CScrollBar_isVisible(void*);
extern "C" void func_80145018();
extern "C" void func_80142C80();
extern "C" void func_8014A2D0();
extern "C" void invalidateQstFlag();
extern "C" void* getInstance__9CTaskGameFv();
extern "C" void func_800426A8();
extern "C" void func_80084F50__Q22cf13CfGameManagerFv();
extern "C" void func_801F34F4(void*);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F36BC(void*, int, int);
extern "C" void func_801F3850(void*, u32);
extern "C" u32 func_8009CF8C(u32);
extern "C" void* allocate_head__Q23mtl10MemManagerFUlUli(u32 handle, u32 size, int align);
extern "C" int CSysWin_getUnk34(void*);
extern "C" void func_8022B8E4(void*);
extern "C" void __ct__CCur18(void* self, void* param);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void* self, void* base);
extern "C" void __dt__14Class_8045F858Fv(void* self, int dealloc);
extern "C" u32 func_801355D8();
extern "C" u32 func_801355BC();
extern "C" void func_80124270(void* obj, int value);
extern "C" u16 func_80136254(u16, const char*, u16);
extern "C" u8 func_80141BA0(u16, u8);
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor* self, void* data, const char* name);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);
extern "C" void func_8045F810__17UnkClass_8045F564Fv(void* self);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" void* func_801355F4();
extern "C" void func_8022B9B4(void*, u32, int);
extern "C" void func_8022BF6C(void*, u32, u32);
extern "C" void func_8022BFC8(void*, int);
extern "C" void func_8022B8B8(void*);
extern "C" int func_80291C60(int v);
extern "C" void func_8023FA64(void*, int, void (*)(int, int, u8));
extern "C" void func_8028E9E0(int, int, u8);
extern "C" void func_8028EC74(UnkPtrHolder* self);
extern "C" void func_8028EC28(UnkPtrHolder* self);
extern "C" void __dl__FPv(void*);
extern "C" void __dt__7CSysWinFv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void func_8028EF74(UnkTwoPtr*);
extern "C" void func_eu_804521BC(int);
extern "C" void func_801F35DC(void*);
extern "C" void func_8022B7F4(void*);
extern "C" void func_80291204(int, int, int, u8);
extern "C" void func_8023F860(int, void*);
extern "C" void func_8023FB28(int, void*);
extern "C" void func_8023F3C0(int, void*, u8);
extern "C" void func_8009D018(u32, u32);
extern "C" void func_80083470__Q22cf13CfGameManagerFv(int, int, int);
extern "C" void code80135FDC_thunk_BFE8C(u8*);
extern "C" void func_8027EEF4(u32);
extern "C" u32 func_8027EE88(u32, u32);
extern "C" int func_800824FC__Q22cf13CfGameManagerFv(int, int);
