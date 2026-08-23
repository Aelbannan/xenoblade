#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "kyoshin/CSortMenu.hpp"      // CScrollBarData, CScrollBar_isVisible, func_801F367C
#include "kyoshin/code_80135FDC.hpp"  // func_80137510, func_80137444, func_80136190, func_80136B4C
#include "kyoshin/cf/voice/cvsys/CVS_THREAD_REVIVE.hpp"  // CVoiceHandle, CVoiceHandleVTV, func_802A77E8/802A330C/802A3D54

/* CTutorialList -- tutorial list widget.
   Vtable at lbl_eu_8053A2A8 (split1 .data). Layout-compatible with IWorkEvent
   via vptr at +0x00 for CDeviceFile::readFile, but not a C++ IWorkEvent
   subclass (avoids emitting weak default stubs into this TU's .text).

   Field layout (from retail __ct__CTutorialList 802AF55C + matched call sites):
   +0x00 vptr | +0x04 UnkClass_8045F564 (0x10) | +0x14 six pointers
   (+0x20 Layout*, +0x24/+0x28 AnimTransform*) | +0x2C CCur (0x18)
   | +0x44 CScrollBar (0x40) | +0x84 CSortMenu sub-object | +0x175 control bytes
   | +0x177 mInitialized | +0x17E s8 | +0x180 opaque sub-object | +0x280 u16s
   | +0x288 total size (matches CMenuTutorialList's embedded 0x288 buffer). */
struct CTutorialListLayout {
    void* mVtbl;            // 0x00

    // 0x04-0x83 -- UnkClass_8045F564 (0x10), 6 pointers, CCur (0x18), CScrollBar (0x40)
    u8 gap04[0x80];

    // 0x84 -- CSortMenu sub-object (in-place, ~0x68 bytes)
    u8 mSortMenu[4];        // minimal placeholder for address-of

    // 0x88-0x176 -- copy proxies, data blocks, control flags
    u8 gap88[0xEF];

    u8 mInitialized;        // 0x177 -- non-zero after construction
};

// Data imports (retail symbol names; global data is not C++-mangled).
extern const float lbl_eu_80668DE4;  // anim finish frame bound (.sdata2, sda21)
extern const float lbl_eu_80668DD8;  // scrollbar fade colour set (.sdata2, sda21)
extern const float lbl_eu_80668DDC;
extern const float lbl_eu_80668DE0;
extern char lbl_eu_80510B78[];       // string pool base

class CTutorialList;

// Phantom view over the embedded CCur vtable at +0x2C (MWCC prefixes 2
// implicit vtable entries, so declared index N lands at offset (N+2)*4).
// All-pure so no vtable is emitted; only cast + dispatch a slot.
class CTutorialCurView {
public:
    virtual void vf0() = 0;                          // index 0 -> +0x08
    virtual void vf1() = 0;                          // index 1 -> +0x0C - func_801D20DC
    virtual void vf2(const nw4r::math::VEC3*) = 0;   // index 2 -> +0x10 - func_801D2144
};

// CSortMenu / CCur helpers (retail unmangled symbols; C linkage).
extern "C" u32 func_801D32DC(void*);
extern "C" int func_801D3320(void*);
extern "C" int func_801D3328(void*);
extern "C" void func_801D3064(void*);
extern "C" void func_801D3160(void*);
extern "C" void func_801D3408(void*);
extern "C" void func_801D202C(void*);
extern "C" void func_801D3724(void*);                     // CSortMenu page up
extern "C" void func_801D3620(void*);                     // CSortMenu page down
extern "C" void func_801D377C(void*);                     // CSortMenu page down (alt)
extern "C" void func_801D3698(void*);                     // CSortMenu page up (alt)
extern "C" void func_801D3430(void*, void*);              // set sort-menu anchor pos
extern "C" void func_801D3330(void*);                     // rebuild sort menu
extern "C" void func_801D3454(nw4r::math::VEC3*, void*);  // copy cursor target
extern "C" void func_801D216C(void*, int);
// Device/memory helper (retail unmangled C symbol).
extern "C" int func_800A9D90();
extern "C" void* readCommonArchiveFile__11CDeviceFileFUlPCcP10IWorkEventii(unsigned long, const char*, void*, int, int);
// UI sound: retail symbol is the C++-mangled func_80138078__FUl.
void func_80138078(u32);

// Sort-menu sub-object helpers (retail unmangled C symbols; same declarations
// as CItemBoxGrid.hpp / CMCCrystalBox.hpp, declared here to avoid pulling
// those TUs' headers into this one).
extern "C" u32 func_801D3808(void*);
extern "C" void func_801D350C(void*);
extern "C" void func_801D3518(void*, void*);
extern "C" void func_801D353C(void*, u8);
extern "C" void func_801D3258(void*);
// CBdat index-free helper (retail symbol is the pre-mangled __5CBdatFUl form).
extern "C" void func_8003AA8C__5CBdatFUl(u32);

// Global term/active flag cleared by func_802AD1F4 (.sbss word, sda21).
extern u32 lbl_eu_80664BF0;

// Embedded sub-object destructors used by ~CTutorialList (retail unmangled
// symbols; same pattern as CQstLogList.hpp).
extern "C" void __dt__9CSortMenuFv(void*, int);
extern "C" void __dt__10CScrollBarFv(void*, int);
extern "C" void __dt__6CCur18Fv(void*, int);
extern "C" void __dt__17UnkClass_8045F564Fv(void*, int);

// File-load / layout-build imports with unmangled retail names (same set as
// CKizunaTalkList.hpp; these retail symbols are not C++-mangled).
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" void createRegion__17UnkClass_8045F564FiiPCci(void*, int, int, const char*, int);
extern "C" void __ct__14Class_8045F858FP17UnkClass_8045F564(void* self, void* base);
extern "C" void __dt__14Class_8045F858Fv(void* self, int dealloc);
extern "C" void func_80434A4C__Q23mtl10MemManagerFb(bool value);
extern "C" nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor__10CLibLayoutFv();
extern "C" bool Attach__Q34nw4r3lyt19ArcResourceAccessorFPvPCc(nw4r::lyt::ArcResourceAccessor* self, void* data, const char* name);
extern "C" void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32 arg,
                                                                     nw4r::lyt::Layout* layout);
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4();
extern "C" void __ct__CCur18(void* self, void* param);
extern "C" void func_8045F810__17UnkClass_8045F564Fv(void*);
extern "C" u32 func_8003B1EC(void* fp);
extern "C" void func_8003AA78__5CBdatFUlPv(u32 value, void* data);
extern "C" void* func_8003AA34();
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int);
// .sdata2 int->float conversion magic double (0x4330000000000000) referenced
// by the u16 pane-size conversions in OnFileEvent.
extern const double lbl_eu_80668DE8;

// Overlay structs for OnFileEvent's 'timg' message resource (same pattern
// as CQstLogList.hpp): the resource object holds a chain whose first field
// points at the u16 row/column pair used to size the message panes.
struct CTutorialCoords {
    u16 c0;  // 0x0 column
    u16 c2;  // 0x2 row
};
struct CTutorialMsgChain {
    CTutorialCoords* pCoords;  // 0x0
};
struct CTutorialMsgObj {
    u8 gap[0x8];
    CTutorialMsgChain* chain;  // 0x8
};

// Field view of the embedded CCur18 cursor (+0x2C). OnFileEvent copies a
// stack-constructed cursor into it without touching the +0x00 vtable.
struct CTutorialCur18Data {
    void* mVtbl;    // +0x00 - not copied
    void* field_4;  // +0x04
    void* field_8;  // +0x08
    void* field_C;  // +0x0C
    void* field_10; // +0x10
    u8 field_14;    // +0x14
    u8 field_15;    // +0x15
};

// Same-TU widget helpers. Retail strips the C++ mangling for these (bare
// func_ names), so they are defined under C linkage and call sites bind the
// literal retail symbols.
extern "C" void func_802ADAE8(CTutorialList*);
extern "C" void func_802ADB3C(CTutorialList*);
extern "C" void func_802ADB90(CTutorialList*);
extern "C" void func_802ADBDC(CTutorialList*);
extern "C" void func_802ADC28(CTutorialList*);
extern "C" void func_802ADC88(CTutorialList*);
extern "C" void func_802ADCE8(CTutorialList*);
extern "C" void func_802ADFA8(CTutorialList*);
extern "C" void func_802ADE18(CTutorialList*);
extern "C" void func_802ADEE4(CTutorialList*);
extern "C" void func_802ACC30(u8*, u16, int);

class CTutorialList {
public:
    CTutorialList(u16 count);
    virtual ~CTutorialList();
    int OnFileEvent(CEventFile* event);

    u8 func_802AD300();
    u8 func_802AD984();
    u8 func_802AD2A4();

    // Fields are public: the retail widget functions are C-linkage free
    // functions (func_802AD308 etc.) that need raw member access.
    // vptr at +0x00 (implicit from virtual dtor)
    u8 mGap04[0x10];                      // 0x04: UnkClass_8045F564 (0x10)
    void* mField14;                       // 0x14: CFileHandle*
    void* mField18;                       // 0x18: ArcResourceAccessor*
    void* mField1C;                       // 0x1C: Layout*
    nw4r::lyt::Layout* mLayout20;         // 0x20
    nw4r::lyt::AnimTransform* mAnim24;    // 0x24
    nw4r::lyt::AnimTransform* mAnim28;    // 0x28
    u8 mGap2C[0x18];                      // 0x2C: CCur (0x18 bytes)
    CScrollBarData mScrollBar;            // 0x44: CScrollBar (0x40 bytes)
    u8 mSortMenu84[0xF0];                 // 0x84: CSortMenu sub-object (opaque)
    u8 mField174;                         // 0x174: run/visible gate byte
    u8 mState175;                         // 0x175: anim/state byte
    u8 mField176;                         // 0x176: visible-selection byte
    u8 mInitialized;                      // 0x177: non-zero after construction
    s8 mField178;                         // 0x178: sign-extended page byte
    u16 mField17A;                        // 0x17A: selection id (halfword)
    u16 mField17C;                        // 0x17C: count/limit (halfword)
    s8 mField17E;                         // 0x17E: sign-extended counter base
    u8 mPad17F;                           // 0x17F: padding (retail sub-object at 0x180)
    u8 mSubObj180[0x100];                 // 0x180: opaque sub-object (func_802ACC30)
    u16 mField280;                        // 0x280: content size (halfword)
    s16 mField282;                        // 0x282: sign-extended row id
    s16 mField284;                        // 0x284: sign-extended selection id
    u8 mGap286[2];                        // 0x286-0x287: pad to 0x288 total size
};
