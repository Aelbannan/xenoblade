#pragma once

#include <types.h>
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/work/CProcess.hpp"

// Forward declarations (full definitions live in the monolib/nw4r headers,
// included by the TU that needs them).
namespace nw4r {
namespace lyt {
class ArcResourceAccessor;
class AnimTransform;
class DrawInfo;
class Layout;
class Pane;
}
}
class CScn;
class IScnRender;

// Minimal CScn declaration (same guard scheme as CTaskGameEff.hpp): only the
// render-callback members used by Term/cbRenderBefore, so member calls emit the
// retail mangled symbols addRenderCB__4CScnFP10IScnRenderUlUl /
// removeRenderCB__4CScnFP10IScnRender without dragging in the full class.
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" float lbl_eu_806670A0;
extern "C" float lbl_eu_80661E48;
extern "C" float lbl_eu_806670CC;
extern "C" float lbl_eu_80663FB4;
extern "C" float lbl_eu_80667090;
extern f32 lbl_eu_80667094;   // marker grid scale multiplier
// Conversion-magic doubles for Move() are DEFINED in CMiniMap.cpp as extern
// const .sdata2 symbols so MWCC's constant pool reuses the retail names for
// the (f32) casts instead of emitting TU-local @N labels.
extern const f64 lbl_eu_80667098;
extern const f64 lbl_eu_806670A8;
extern f32 lbl_eu_806670A4;   // clock anim frame target (func_80137444/80137510 arg)
extern f32 lbl_eu_806670B4;   // func_8011B05C SRT rotation constant
extern f32 lbl_eu_806670BC;   // func_8011B05C scale factor
extern f32 lbl_eu_8066A1F8;   // func_8011B05C scale divisor
// Panic file/format strings used by func_8011B05C (u8 pools).
extern u8 lbl_eu_8052CBC0[];
extern u8 lbl_eu_8052CB8C[];
extern u8 lbl_eu_8052CB80[];
extern u8 lbl_eu_8052CB4C[];
extern u8 lbl_eu_8052CC00[];
extern u8 lbl_eu_8052CBCC[];
// BDAT row-filter helper defined in code_80135FDC.cpp (extern "C" there too).
extern "C" u32 func_80138574(const char* name, u32 id);

// Global data imports (MWCC does not mangle global-scope data names).
extern u32 __ptmf_null[3];      // null pointer-to-member-function constant
// Temp (CProcess) vtable written before the __ptmf_null copy, then overwritten
// by the composite vtable (see __ct__8011C1B8).
extern char lbl_eu_8052BF70[];
extern char lbl_eu_8052C858[];  // CMenuMiniMap2 composite vtable block (+0x24/+0xAC slots)
extern char lbl_eu_8052CA88[];  // CMMClock vtable
// Presentation/mode flag words (.sbss) tested by cbRenderBefore.
extern u32 lbl_eu_80663E28;
extern u32 lbl_eu_80663E24;
// Minimap process singleton instance pointer (.sbss).
extern u32 lbl_eu_80663FB0;

// Minimal CTaskGame decl (retail symbols getInstance__9CTaskGameFv /
// func_800426F0__9CTaskGameFv; same scheme as CSystemWindow.hpp).
class CTaskGame {
public:
    static CTaskGame* getInstance();
    static bool func_800426F0();
};

// Global gimmick container returned by getUnk80664658 (flag word at +0x214).
struct CMMGlobalGimmick {
    u8 field_0x00[0x214];       // 0x00..0x214
    u32 field_214;              // 0x214
};
CMMGlobalGimmick* getUnk80664658();

// Cross-TU layout/BDAT helpers. Plain C++ declarations so MWCC re-derives the
// retail mangled names (func_80136E84__FPP... / func_80137038__FPP... / ...).
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_80137250(nw4r::lyt::DrawInfo* pDrawInfo);
void func_80139124(nw4r::lyt::ArcResourceAccessor* accessor);
// BDAT/string helpers with unmangled retail symbols: extern "C" keeps the
// emitted call relocs matching retail (func_8003AA34 / getFP__FPCc).
extern "C" void func_8003AA34();
extern "C" void* getFP__FPCc(const char* name);
extern "C" int func_8013BE88();
extern "C" int func_801AC124();
// Move() gate/state helpers (unmangled retail symbols - C linkage).
extern "C" u32 func_80242354();
extern "C" bool func_80251550();
bool func_8006EF04(int mask);   // func_8006EF04__Fi (mangled C++)
extern "C" u32 func_80134538();
extern "C" void func_8013ACFC();
extern "C" int func_8013BE58();
// cf::CfGameManager battle-target probe (pre-mangled static member, C linkage).
extern "C" void* func_800821F8__Q22cf13CfGameManagerFv();
// cf::CfGameManager pad-mode probe with an explicit int arg: the retail symbol
// is the pre-mangled name, so it must stay C linkage (inside extern "C") or
// MWCC appends a __Fi suffix (see CQuestWindow.hpp:280 block). The s16 inline
// wrapper in CfGameManager.hpp would DCE the `li r3,-1` arg setup.
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
// CProcess base ctor is out-of-line in retail (CProcess is abstract, so the
// factory calls it as a plain function).
void __ct__8CProcessFv(CProcess* self);

class CMMClock {
public:
    virtual ~CMMClock();
    bool OnFileEvent(CEventFile* pEventFile);

    // +0x00: vtable (virtual dtor)
    CFileHandle* mFileHandle;                   // 0x04 - requested file handle
    nw4r::lyt::ArcResourceAccessor* mAccessor;  // 0x08 - arc resource accessor
    nw4r::lyt::Layout* mLayout;                 // 0x0C - loaded layout
    nw4r::lyt::AnimTransform* mAnimTrans0;      // 0x10 - animation transform 0
    nw4r::lyt::AnimTransform* mAnimTrans1;      // 0x14 - animation transform 1
    u8 field_0x18;                              // 0x18
    u8 field_0x19;                              // 0x19
    u8 mReady;                                  // 0x1A - load-complete flag
    u8 pad_0x1B;                                // 0x1B
    UnkClass_8045F564 mUnk1C;   // 0x1C..0x2C
};

class CMMTex {
public:
    virtual ~CMMTex();
    bool OnFileEvent(CEventFile* pEventFile);

    CFileHandle* mFileHandle; // 0x4 - requested file handle
    u8* mData;                // 0x8 - loaded file data buffer
    u8 mReady;                // 0xC - load-complete flag
};

class CMiniMap {
public:
    CMiniMap();
    virtual ~CMiniMap();
    bool OnFileEvent(CEventFile* pEventFile);

    // +0x00: vtable (virtual dtor)
    CFileHandle* mFileHandle;              // 0x04 - requested file handle
    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x08 - arc resource accessor
    nw4r::lyt::Layout* mLayout0C;          // 0x0C..0x10 (drawn by cbRenderBefore)
    nw4r::lyt::AnimTransform* mAnimTrans0; // 0x10 - animation transform 0
    nw4r::lyt::AnimTransform* mAnimTrans1; // 0x14 - animation transform 1
    u8 field_0x18;                         // 0x18
    u8 field_0x19;                         // 0x19
    u8 mReady;                             // 0x1A - load-complete flag
    u8 mFlag1B;                            // 0x1B..0x1C (draw gate checked by cbRenderBefore)
    u8 pad_0x1C[0x20 - 0x1C];              // 0x1C..0x20
    f32 mField20;                          // 0x20 - marker grid scale
    u8 pad_0x24[0x28 - 0x24];              // 0x24..0x28
    u32 mField28;                          // 0x28
    u8 pad_0x2C[0x34 - 0x2C];              // 0x2C..0x34
    void* mField34;                        // 0x34 (abs 0xC4) - cached 'timg' resource (Move)
    u8 mField38;                           // 0x38 (abs 0xC8) - clock-show flag (Move)
    u8 pad_0x39[0x3C - 0x39];              // 0x39..0x3C
    u8 mField3C[0x17C - 0x3C];             // 0x3C..0x17C (marker-table region, func_801160A8)
    u8 mField17C[0x824 - 0x17C];           // 0x17C..0x824 (gimmick-view region, func_80116670)
    UnkClass_8045F564 m824;                // 0x824..0x834 (declared first -> destroyed last)
    UnkClass_8045F564 m834;                // 0x834..0x844 (declared second -> destroyed first)
};

class CMenuMiniMap2 : public CProcess {
public:
    virtual ~CMenuMiniMap2();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x3C..0x58: IUIWindow/Move region - two null PMF triples + control words
    u32 mPtMf3C[3];               // 0x3C..0x48 null pointer-to-member-function
    u32 mPtMf48[3];               // 0x48..0x54 null pointer-to-member-function
    u8 mField54;                  // 0x54
    u8 mField55;                  // 0x55
    u8 mPad56[0x58 - 0x56];       // 0x56..0x58
    u32 mFn58;                    // 0x58 (lbl_eu_8052C858 + 0x24 slot)
    u32 mScnRender;               // 0x5C (IScnRender vtable slot; passed to CScn)
    CScn* mScn;                   // 0x60 - owning scene (removeRenderCB receiver)
    CMMClock mClock;              // 0x64..0x90 (declared first -> destroyed last)
    CMiniMap mMiniMap;            // 0x90..0x8D4 (declared second -> destroyed first)
    u8 mField8D4;                 // 0x8D4
};

// Manual-layout twin of the CMenuMiniMap2 object used by the __ct__8011C1B8
// factory (CMenuGameClear idiom): the factory fills the CProcess storage and
// the IUIWindow/Move region by hand, then hand-builds CMMClock/CMiniMap.
struct CMenuMiniMap2CreateView {
    u8 mProcess[0x10];            // 0x00..0x10
    u32 mVtable10;                // 0x10 - CProcess vtable (temp then composite)
    u8 mProcess14[0x3C - 0x14];   // 0x14..0x3C
    u32 mPtMf3C[3];               // 0x3C..0x48
    u32 mPtMf48[3];               // 0x48..0x54
    u8 mField54;                  // 0x54
    u8 mField55;                  // 0x55
    u8 mPad56[0x58 - 0x56];       // 0x56..0x58
    u32 mFn58;                    // 0x58
    u32 mScnRender;               // 0x5C
    CScn* mScn;                   // 0x60
    CMMClock mClock;              // 0x64..0x90 (vtable written by hand)
    CMiniMap mMiniMap;            // 0x90..0x8D4
    u8 mField8D4;                 // 0x8D4
};

// Table object initialized by func_80115FD0: BDAT table pointer plus 3x5
// element columns (row strides 0xA/0x5/0x5/0x14).
struct MiniMapTable {
    u32 field_00;                 // 0x00
    u32 field_04;                 // 0x04 - getFP() table pointer
    u32 field_08;                 // 0x08 - nonzero when the table is live
    f32 field_0C;                 // 0x0C - grid scale divisor
    u16 field_10[3][5];           // 0x10..0x2E - marker resource ids (0 = end)
    u8 field_2E[3][5];            // 0x2E..0x3D
    u8 field_3D[3][5];            // 0x3D..0x4C - pane-attached flag
    ml::CVec3 field_4C[3][5];     // 0x4C..0x100 - per-marker world position
    nw4r::lyt::Pane* field_100[3][5]; // 0x100..0x13C - per-marker pane
    nw4r::lyt::Pane* field_13C;   // 0x13C - pane parent (layout root pane)
};

// Gimmick/marker view tables used by func_801165EC / func_801167EC /
// func_801168A0 (subobject of the minimap object at +0x17C).
struct CMiniMapGimmickView {
    const char* field_0x00;            // 0x00 - bdat table pointer
    u32 field_0x04;                    // 0x04
    f32 field_0x08;                    // 0x08
    u32 field_0x0C;                    // 0x0C
    u32 field_0x10;                    // 0x10
    u32 field_0x14;                    // 0x14
    u8 field_0x18[0x338 - 0x18];       // 0x18..0x338
    u8 field_0x338[25 * 8];             // 0x338..0x400
    u8 field_0x400[25 * 8];             // 0x400..0x4C8
    u8 field_0x4C8[0x590 - 0x4C8];     // 0x4C8..0x590
    u32 field_0x590;                   // 0x590
    u8 field_0x594[0x6A4 - 0x594];     // 0x594..0x6A4
    u32 field_0x6A4;                   // 0x6A4
};

// Same-unit minimap helpers whose retail symbols are unmangled (func_*):
// extern "C" keeps the emitted call relocs matching retail
// (func_80118058 / __ct__CMiniMap) instead of re-deriving mangled names.
extern "C" void func_80118058(CMiniMap* self);
extern "C" void __ct__CMiniMap(CMiniMap* self);
