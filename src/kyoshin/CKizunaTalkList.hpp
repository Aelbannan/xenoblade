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
class CBaseCur;
#include "monolib/work/IWorkEvent.hpp"
#include "kyoshin/CScrollBar.hpp"

// IWorkEvent-compatible vtable for CKizunaTalkList (split1 .data).
extern "C" void* lbl_eu_80537D28[];

// Abstract view into the embedded CCur18 cursor vtable. MWCC prepends
// offset-to-top + RTTI entries, so vtable offset = (index + 2) * 4;
// index 2 -> +0x10 is the "Move" virtual taking a VEC3*.
class CCur18View {
public:
    virtual void v00() = 0;      // 0x08
    virtual void v01() = 0;      // 0x0C
    virtual void v02(void*) = 0; // 0x10 - Move
};

/* Sets mVtbl before anything else (retail ctor stores lbl_eu_80537D28 first).
   Same idiom as CScrollBarVtblBase: deriving directly would emit the TU-local
   __vt__ symbol instead of the retail vtable label. */
struct CKizunaTalkListVtblBase {
    void* mVtbl; // 0x0 - lbl_eu_80537D28
    CKizunaTalkListVtblBase() { mVtbl = lbl_eu_80537D28; }
};

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

class CKizunaTalkList : public CKizunaTalkListVtblBase {
public:
    /* 0x04 */ u8 _pad04[0x10];     // UnkClass_8045F564 (stub)
    /* 0x14 */ u32 mEntryCount;     // number of valid entries
    /* 0x18 */ u32 mUnknown18;
    /* 0x1C */ u32 mUnknown1C;
    /* 0x20 */ nw4r::lyt::Layout* mpLayout20; // layout used for BindAnim-style anim control
    /* 0x24 */ nw4r::lyt::AnimTransform* mpAnim24;
    /* 0x28 */ nw4r::lyt::AnimTransform* mpAnim28;
    /* 0x2C */ u8 mCursor[0x18];     // CCur18 cursor (stub, 0x18 bytes)
    /* 0x44 */ u8 mScrollBar[0x40];  // CScrollBar widget; constructed/copied
                                     // explicitly in the ctor (retail calls
                                     // __ct__CScrollBar in the body, so the
                                     // member is held as raw storage)
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
    ~CKizunaTalkList();
    bool OnFileEvent(CEventFile* pEventFile);
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - do not "fix" the mangled forms).
// Relocated from CKizunaTalkList.cpp so every TU that uses these symbols gets
// them from this header instead of inline pseudo-import declarations.
// ---------------------------------------------------------------------------

// External helpers (C-linkage retail symbols) for file loading, scrollbar /
// cursor widgets and sound effects.
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, void*, int, int);
extern "C" int func_800A9D90();
extern "C" void func_801F34F4(void*);
extern "C" void func_801F35B0(void*, void*);
extern "C" void func_801F3670(void*, void*);
extern "C" void func_801F367C(void*);
extern "C" void func_801F369C(void*);
extern "C" void func_801F36BC(void*, int, int);
extern "C" void func_801F3540(void*);
extern "C" void func_801F3850(void*, u16);
extern "C" void func_801D20B0(void*, void*);
extern "C" void func_801D202C(void*);
extern "C" void func_801D216C(void*, u8);
extern "C" void playUISound__FUl(u32);
extern "C" void func_801C4B60(void*, s16, s16, s16, s16);
extern "C" void __dl__FPv(void*);
extern "C" void __construct_array(void*, void* ctor, void* dtor, int size, int n);
extern "C" void __destroy_arr(void*, void* dtor, int size, int n);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void getEntry__5CBdatFUl(u32);
extern "C" void func_801390E0(CFileHandle**);
extern "C" void releaseArcResourceAccessor(nw4r::lyt::ArcResourceAccessor*);
extern "C" void func_801F35DC(void*);
extern "C" void func_8045F778(UnkClass_8045F564* self);

// Layout-build helpers used by CKizunaTalkList::OnFileEvent (mangled retail
// symbol names so the reloc targets line up).
extern "C" void setMemInitFlag__Q23mtl10MemManagerFb(bool value);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor* self, void* data, const char* name);
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg, nw4r::lyt::Layout* layout);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" CBaseCur* __ct__CCur18(void* self, void* param);
extern "C" void __ct__17UnkClass_8045F564Fv(void* self);
extern "C" void __ct__CScrollBar(void* self, u8 direction);
extern "C" void func_80137924(void* out, void* paneA, void* paneB, void* paneC);extern "C" void setBdatEntry__5CBdatFUlPv(u32 value, void* data);
extern "C" void* func_8003AA34();
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void* self, void* base);
extern "C" void __dt__14Class_8045F858Fv(void* self, int dealloc);
extern "C" void* getFP__FPCc(const char* name);
extern "C" int sprintf(char* str, const char* fmt, ...);

// Data/build helpers used by func_80272810.
extern "C" u32 func_8003B1EC(void* fp);
extern "C" u32 func_8009CF8C(u32);
extern "C" u16 func_80136254(const void* fp, const void* name, int id);
extern "C" u16 func_8013A7D0(u8 a, u8 b);
u32 func_8027305C(TalkListEntryArray* self, u8 v);
extern "C" u8* lbl_eu_806640A8;                            // .sbss BDAT table pointer

// data / rodata labels
extern "C" char lbl_eu_8050E990[];            // file-name table (target 7)
extern "C" void* lbl_eu_80664090;            // .sbss shared BDAT character table (target 7)
extern "C" void* lbl_eu_806648B8;             // .sbss loaded file pointer (target 8); assigned from void* getFP -> kept void*
extern "C" void* lbl_eu_806648C0;             // .sbss colour entries (target 8) - address anchor
// lbl_eu_806648C0/C8/D0/D8 are address anchors (&lbl used directly): keep void*.
extern "C" void* lbl_eu_806648C8;
extern "C" void* lbl_eu_806648D0;
extern "C" void* lbl_eu_806648D8;
extern "C" f32 lbl_eu_806689C8;               // .sdata2 float constants (target 10)
extern "C" f32 lbl_eu_806689CC;
extern "C" f32 lbl_eu_806689D0;
extern "C" const f32 lbl_eu_806689D4;         // 2.0f - anim frame step for entry show/hide

// State-transition helpers for CKizunaTalkList (defined in CKizunaTalkList.cpp;
// extern "C" + noinline keeps callers emitting real unmangled bl branches -
// retail keeps them as separate out-of-line functions in this TU).
extern "C" __declspec(noinline) void func_80273AD0(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_80273A70(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_80273B30(CKizunaTalkList* self);
extern "C" __declspec(noinline) void func_802740E4(CKizunaTalkList* self);

// Misc helpers used by the row-draw / visibility paths.
extern "C" int CScrollBar_isVisible(void*);
extern "C" void func_80124270(void*, u32);
extern "C" __declspec(noinline) void* func_80273040(void* self, u32 r4);


