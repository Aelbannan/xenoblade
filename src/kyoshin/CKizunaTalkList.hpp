#pragma once

/*
 * CKizunaTalkList.hpp
 * Kizuna (Friendship) Talk List manager.
 * Displays affinity talk messages between characters.
 *
 * Layout (size 0x1494):
 *   +0x00: IWorkEvent (vtable + virtual event handlers)
 *   +0x04: UnkClass_8045F564 - memory region for entries
 *   +0x14: mEntryCount (or part of header)
 *   +0x18: mUnknown18
 *   +0x1C: mUnknown1C
 *   +0x20: mUnknown20
 *   +0x24: mUnknown24
 *   +0x28: mUnknown28
 *   +0x2C: mCursor - CCur18 cursor widget (0x18 bytes)
 *   +0x44: mScrollBar - CScrollBar widget (0x40 bytes)
 *   +0x84: mState84
 *   +0x85: mState85 (state machine progression)
 *   +0x86: mUnknown86
 *   +0x87: mNeedsRebuild - 1 = needs (re)build, 0 = current
 *   +0x88: mUnknown88
 *   +0x8A: mUnknown8A (short)
 *   +0x8C: mEntryArray - array of talk entries (0x1408 bytes)
 */

#include <types.h>
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CScrollBar.hpp"

// Talk list entry (0x14 bytes each, 256 max)
struct TalkListEntry {
    u32 field_00;   // 0x00
    u32 field_04;   // 0x04
    u32 field_08;   // 0x08
    u32 field_0C;   // 0x0C
    s16 field_10;   // 0x10
    s8 field_12;    // 0x12, initialised to -1
    u8 field_13;    // 0x13
};

// Array container holding up to 256 entries
struct TalkListEntryArray {
    TalkListEntry mEntries[256]; // 0x000-0x13FF
    u8 mCount;                   // 0x1400
    u32 mParent;                 // 0x1404
};

class CKizunaTalkList : public IWorkEvent {
public:
    /* 0x04 */ u8 _pad04[0x10];     // UnkClass_8045F564 (stub)
    /* 0x14 */ u32 mEntryCount;     // number of valid entries
    /* 0x18 */ u32 mUnknown18;
    /* 0x1C */ u32 mUnknown1C;
    /* 0x20 */ nw4r::lyt::Layout* mpLayout20; // layout used for BindAnim-style anim control
    /* 0x24 */ nw4r::lyt::AnimTransform* mpAnim24;
    /* 0x28 */ nw4r::lyt::AnimTransform* mpAnim28;
    /* 0x2C */ u8 mCursor[0x18];     // CCur18 cursor (stub, 0x18 bytes)
    /* 0x44 */ CScrollBar mScrollBar;
    /* 0x84 */ u8 mState84;
    /* 0x85 */ u8 mState85;         // state machine progression flag
    /* 0x86 */ u8 mUnknown86;
    /* 0x87 */ u8 mNeedsRebuild;    // 1 = pending rebuild, 0 = built
    u8 func_8027355C() const { return mNeedsRebuild; }
    /* 0x88 */ s8 mUnknown88;
    /* 0x89 */ u8 _pad89;           // padding
    /* 0x8A */ s16 mUnknown8A;
    /* 0x8C */ TalkListEntryArray mEntryArray;
    CKizunaTalkList();
    virtual ~CKizunaTalkList();
    virtual bool OnFileEvent(CEventFile* pEventFile);
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names — do not "fix" the mangled forms).
// Relocated from CKizunaTalkList.cpp so every TU that uses these symbols gets
// them from this header instead of inline pseudo-import declarations.
// ---------------------------------------------------------------------------

// External helpers (C-linkage retail symbols) for file loading, scrollbar /
// cursor widgets and sound effects.
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" u32 func_800A9D90();
extern "C" void func_801F34F4(void*);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F367C(void*);
extern "C" void func_801F369C(void*);
extern "C" void func_801F36BC(void*, int, int);
extern "C" void func_801F3850(void*, u16);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D216C(void*, int);
extern "C" void func_80138078__FUl(u32);
extern "C" void func_801C4B60(void*, u32, u32, u32, u32);
extern "C" void __dl__FPv(void*);
extern "C" void __construct_array(void*, void* ctor, void* dtor, int size, int n);
extern "C" void __destroy_arr(void*, void* dtor, int size, int n);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);

// Layout-build helpers used by CKizunaTalkList::OnFileEvent (mangled retail
// symbol names so the reloc targets line up).
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool value);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor* self, void* data, const char* name);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);
extern "C" void* func_801355F4();
extern "C" void __ct__CCur18(void* self, void* param);
extern "C" void func_8003AA78__5CBdatFUlPv(u32 value, void* data);
extern "C" void func_8003AA34();
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void* self, void* base);
extern "C" void __dt__14Class_8045F858Fv(void* self, int dealloc);
extern "C" void* getFP__FPCc(const char* name);
extern "C" int sprintf(char* str, const char* fmt, ...);

// Data/build helpers used by func_80272810.
extern "C" u32 func_8003B1EC(void* fp);
extern "C" u32 func_8009CF8C(u32);
extern "C" u16 func_80136254(const void* fp, const char* name, u16 id);
extern "C" u16 func_8013A7D0(u8 a, u8 b);
extern "C" u32 func_8027305C(TalkListEntryArray* self, u8 v);
extern "C" void* lbl_eu_806640A8;                            // .sbss table pointer

// data / rodata labels
extern "C" char lbl_eu_8050E990[];            // file-name table (target 7)
extern "C" void* lbl_eu_80664090;            // .sbss shared character table (target 7)
extern "C" void* lbl_eu_806648B8;             // .sbss loaded file pointer (target 8)
extern "C" void* lbl_eu_806648C0;             // .sbss colour entries (target 8)
extern "C" void* lbl_eu_806648C8;
extern "C" void* lbl_eu_806648D0;
extern "C" void* lbl_eu_806648D8;
extern "C" f32 lbl_eu_806689C8;               // .sdata2 float constants (target 10)
extern "C" f32 lbl_eu_806689CC;
extern "C" f32 lbl_eu_806689D0;

// State-transition helpers for CKizunaTalkList (defined in CKizunaTalkList.cpp;
// extern "C" + noinline keeps callers emitting real unmangled bl branches —
// retail keeps them as separate out-of-line functions in this TU).
extern "C" __declspec(noinline) void func_80273AD0(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_80273A70(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_80273B30(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_802740E4(CKizunaTalkList* self);

// Misc helpers used by the row-draw / visibility paths.
extern "C" u8 CScrollBar_isVisible(void*);
extern "C" void func_80124270(void*, u32);
extern "C" __declspec(noinline) void* func_80273040(void* self, u32 r4);


