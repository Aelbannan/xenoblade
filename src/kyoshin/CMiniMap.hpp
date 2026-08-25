#pragma once

#include <types.h>
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/work/CProcess.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

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
extern const f32 lbl_eu_80667090;
extern f32 lbl_eu_80667094;   // marker grid scale multiplier
// Conversion-magic doubles for Move() are DEFINED in CMiniMap.cpp as extern
// const .sdata2 symbols so MWCC's constant pool reuses the retail names for
// the (f32) casts instead of emitting TU-local @N labels.
extern const f64 lbl_eu_80667098;
extern const f64 lbl_eu_806670A8;
extern f32 lbl_eu_806670A4;   // clock anim frame target (func_80137444/80137510 arg)
extern f32 lbl_eu_806670B0;   // ctor grid-scale multiplier
extern f32 lbl_eu_806670B4;   // func_8011B05C SRT rotation constant
extern f32 lbl_eu_806670BC;   // func_8011B05C scale factor
extern f32 lbl_eu_8066A1F8;   // func_8011B05C scale divisor
extern f32 lbl_eu_806670C0;   // func_8011B05C battle-target node scale
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
// Minimap process singleton instance pointer (.sbss).
extern u32 lbl_eu_80663FB0;
// CMiniMap vtable + the +0x2C subobject vtable (ctor stores).
extern char lbl_eu_8052C958[];
extern char lbl_eu_8052C9F0[];
// Current BDAT table pointer / per-row name-pointer table used by the ctor.
extern char* lbl_eu_806640A8;
extern char* lbl_eu_8052C7E8[];

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
extern "C" CMMGlobalGimmick* getUnk80664658();

// Cross-TU layout/BDAT helpers. Plain C++ declarations so MWCC re-derives the
// retail mangled names (func_80136E84__FPP... / func_80137038__FPP... / ...).
void func_80136E84(nw4r::lyt::Layout**, nw4r::lyt::ArcResourceAccessor*, const char*);
void func_80136F08(nw4r::lyt::Layout*, nw4r::lyt::AnimTransform**, nw4r::lyt::ArcResourceAccessor*, char*);
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
void func_80137250(nw4r::lyt::DrawInfo* pDrawInfo);
void func_80139124(nw4r::lyt::ArcResourceAccessor* accessor);
// BDAT/string helpers with unmangled retail symbols: extern "C" keeps the
// emitted call relocs matching retail (func_8003AA34 / getFP__FPCc).
extern "C" void* func_8003AA34();   // matches code_801862C0.hpp's declaration
extern "C" void* getFP__FPCc(const char* name);
// Gimmick-row helpers used by func_80116B40 (unmangled retail symbols;
// same declaration scheme as CFloorMap.hpp).
extern "C" u32 func_8013C038(u16 id);
struct CMMGimmickPos { f32 x; f32 y; f32 z; };
extern "C" CMMGimmickPos* func_801F4E68(CMMGlobalGimmick* mgr, u16 id);
extern "C" unsigned long func_8009ECF0();
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

// Unknown loader subobject embedded at CMiniMap+0x2C. Retail's inlined member
// ctor writes the lbl_eu_8052C9F0 vtable and zeroes every field.
struct CMMSub {
    void* mVtable;   // 0x00
    void* mPtr04;    // 0x30
    u8* mPtr08;      // 0x34 - cached 'timg' resource (Move)
    u8 mFlag0C;      // 0x38 - clock-show flag (Move)
    u8 mFlag0D;      // 0x39
    u8 mFlag0E;      // 0x3A
};

// Loader-subobject head stored at CMiniMap+0x2C. The three flag bytes live at
// the head of CMMTableBlock (+0x38) so Init's whole-region copies line up with
// retail's block-copy loop bounds.
struct CMMSubHead {
    void* mVtable;   // 0x00 (lbl_eu_8052C9F0)
    void* mPtr04;    // 0x30 abs
    u8* mPtr08;      // 0x34 abs - cached 'timg' resource (Move)
};

// Marker-table region 0x38..0x178 (0x140 bytes). The leading bytes alias the
// loader flags.
struct CMMTableBlock {
    u8 mFlag38;      // 0x38 - clock-show flag (Move)
    u8 mFlag39;      // 0x39
    u8 mFlag3A;      // 0x3A
    u8 mPad3B;
    u32 mW[0x4F];    // 0x3C..0x178 (func_801160A8 marker table)
};

// Gimmick-view region 0x178..0x820.
struct CMMViewBlock {
    u32 mW[0x1AA];   // func_80116670 gimmick view
};

// Stack mirror of CMMClock used by CMenuMiniMap2::Init: retail hand-builds the
// temp (vtable + zeroed fields + gate bytes), member-copies it into mClock,
// then runs the embedded UnkClass dtor, so the UnkClass storage stays raw here
// and is placement-new'd by Init.
struct CMMClockInit {
    void* mVtable;                              // lbl_eu_8052CA88
    CFileHandle* mFileHandle;
    nw4r::lyt::ArcResourceAccessor* mAccessor;
    nw4r::lyt::Layout* mLayout;
    nw4r::lyt::AnimTransform* mAnimTrans0;
    nw4r::lyt::AnimTransform* mAnimTrans1;
    u8 field_0x18;
    u8 field_0x19;
    u8 mReady;
    u8 pad_0x1B;
    u8 mUnk1C[0x10];
};

// Minimap map-image loader object (func_80117734): requests the current map's
// image arc into MEM2 and records the selected BDAT row / resource variant.
struct CMMMapImg {
    void* mVtable;              // 0x00 (untouched here)
    CFileHandle* mFileHandle;   // 0x04 - requested file handle
    u8* mData;                  // 0x08 - loaded buffer (freed on re-request)
    u8 mReady;                  // 0x0C - load-complete flag
    u8 mRow;                    // 0x0D - selected BDAT row (forced min 1)
    u8 mMode;                   // 0x0E - selected unlock-gated resource variant
};

class CMiniMap {
public:
    ~CMiniMap();
    bool OnFileEvent(CEventFile* pEventFile);

    void* mVtable;                         // 0x00 - retail vtable (lbl_eu_8052C958)
    CFileHandle* mFileHandle;              // 0x04 - requested file handle
    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x08 - arc resource accessor
    nw4r::lyt::Layout* mLayout0C;          // 0x0C..0x10 (drawn by cbRenderBefore)
    nw4r::lyt::AnimTransform* mAnimTrans0; // 0x10 - animation transform 0
    nw4r::lyt::AnimTransform* mAnimTrans1; // 0x14 - animation transform 1
    u8 field_0x18;                         // 0x18
    u8 field_0x19;                         // 0x19
    u8 mReady;                             // 0x1A - load-complete flag
    u8 mFlag1B;                            // 0x1B
    u16 field_0x1C;                        // 0x1C - BDAT pane id (func_80136330)
    u16 field_0x1E;                        // 0x1E - BDAT pane id (func_80136330)
    f32 mField20;                          // 0x20 - marker grid scale
    char* mField24;                        // 0x24 - getFP-resolved pane name
    u32 mField28;                          // 0x28 - getFP-resolved layout name
    CMMSubHead mSub;                       // 0x2C..0x38 (flags in mField3C head)
    CMMTableBlock mField3C;                // 0x38..0x178 (marker-table region, func_801160A8)
    CMMViewBlock mField17C;                // 0x178..0x820 (gimmick-view region, func_80116670)
    u32 mPad820;                           // 0x820..0x824
    // Raw UnkClass_8045F564 storage: retail constructs these mid-ctor body
    // (after the table init calls), so they are placement-new'd there instead
    // of being auto-constructed at ctor entry.
    u32 m824[4];                           // 0x824..0x834 (destroyed last)
    u32 m834[4];                           // 0x834..0x844 (destroyed first)
};

class CMenuMiniMap2 : public CProcess {
public:
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
    u8 field_2E[15];              // 0x2E..0x3D
    u8 field_3D[15];              // 0x3D..0x4C - pane-attached flag
    ml::CVec3 field_4C[3][5];     // 0x4C..0x100 - per-marker world position
    nw4r::lyt::Pane* field_100[3][5]; // 0x100..0x13C - per-marker pane
    nw4r::lyt::Pane* field_13C;   // 0x13C - pane parent (layout root pane)
};

// Pane-entry record written by func_801168A0: integer-truncated grid coords
// at +0x2C/+0x30, constant at +0x34, and a per-entry enable byte at +0xB7
// (bit0 cleared when a row exceeds the grid radius).
struct CMMMapEntry {
    u8 pad_00[0x2C];
    f32 field_2C;
    f32 field_30;
    f32 field_34;
    u8 pad_38[0xBB - 0x38];
    u8 field_BB;
};

// Gimmick/marker view tables used by func_801165EC / func_801167EC /
// func_801168A0 (subobject of the minimap object at +0x17C).
struct CMiniMapGimmickView {
    const char* field_0x00;            // 0x00 - bdat table pointer
    u32 field_0x04;                    // 0x04
    f32 field_0x08;                    // 0x08
    s32 field_0x0C;                    // 0x0C - row budget (signed)
    s32 field_0x10;                    // 0x10 - fill cursor (signed)
    u32 field_0x14;                    // 0x14
    u8 field_0x18[0xE0 - 0x18];        // 0x18..0xE0 - per-row enable flags
    s16 field_0xE0[25];                 // 0xE0..0x112 - per-row display ids
    u8 field_0x112[0x270 - 0x112];      // 0x112..0x270
    u8 field_0x270[25];                 // 0x270..0x289 - per-row valid flags
    u8 field_0x289[0x338 - 0x289];      // 0x289..0x338
    u8 field_0x338[25 * 8];             // 0x338..0x400
    u8 field_0x400[25 * 8];             // 0x400..0x4C8
    u8 field_0x4C8[0x590 - 0x4C8];     // 0x4C8..0x590
    u32 field_0x590;                   // 0x590
    // Per-row working set (0x594..0x6A4). Three interleaved views over the
    // same storage: pane-entry pointers stride 4 from 0x594, marker world
    // positions stride 0xC from 0x5D4, and per-row flags stride 1 from 0x694.
    union {
        u8 field_0x594[0x110];                 // 0x594..0x6A4 raw view
        struct {
            u8 pad_0x594[0x40];
            ml::CVec3 field_0x5D4[17];         // 0x5D4.. per-row world positions
        };
        struct {
            CMMMapEntry* field_0x594_entries[64]; // 0x594.. per-row pane records
            u8 field_0x694[16];                // 0x694.. per-row update flags
        };
    };
    u32 field_0x6A4;                   // 0x6A4 - live row count
};

// Same-unit minimap helpers whose retail symbols are unmangled (func_*):
// extern "C" keeps the emitted call relocs matching retail
// (func_80118058 / __ct__CMiniMap) instead of re-deriving mangled names.
extern "C" void func_80118058(CMiniMap* self);
extern "C" void __ct__CMiniMap(CMiniMap* self);
MiniMapTable* func_80115FD0(MiniMapTable* self);
void func_801165EC(CMiniMapGimmickView* self);
// Per-frame gimmick-view updater (retail unmangled symbol).
extern "C" void func_801168A0(CMiniMapGimmickView* self);
