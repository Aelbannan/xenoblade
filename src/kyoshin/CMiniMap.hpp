#pragma once
#include "kyoshin/cf/CfCamSymbols.hpp"
#include <types.h>
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/work/CProcess.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
#include "kyoshin/IUICf.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// Defined in CMiniMap.cpp; retail exports the plain C name.
extern "C" s32 MiniMapHasGlobalData();

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

// IScnRender without a virtual destructor (CMiniMap.cpp defines
// ISCN_RENDER_NO_DTOR): retail fills the dtor slot of the +0x5C sub-vtable
// (composite block lbl_eu_8052C858+0xAC) with the manual adjuster
// MiniMap2DtorThunk5C, and a virtual dtor would make MWCC emit an extra @88@
// thunk that retail does not have.
#include "monolib/scn/IScnRender.hpp"

#include "monolib/scn/CScn.hpp"

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" float sClockFrameStep;
extern "C" float sClockFrameBase;
// Conversion-magic doubles for Move() are DEFINED in CMiniMap.cpp as extern
// const .sdata2 symbols so MWCC's constant pool reuses the retail names for
// the (f32) casts instead of emitting TU-local @N labels.
// Panic file/format strings used by MiniMapUpdateMapView (u8 pools).
extern u8 lbl_eu_8052CBC0[];
extern u8 lbl_eu_8052CB8C[];
extern u8 lbl_eu_8052CB80[];
extern u8 lbl_eu_8052CB4C[];
extern u8 lbl_eu_8052CC00[];
extern u8 lbl_eu_8052CBCC[];
// BDAT row-filter helper defined in code_80135FDC.cpp (extern "C" there too).

// Global data imports (MWCC does not mangle global-scope data names).
extern u32 __ptmf_null[3];      // null pointer-to-member-function constant
// Presentation/mode flag words (.sbss) tested by cbRenderBefore.
extern u32 gmPauseFlags;
// Minimap process singleton instance pointer (.sbss).
extern u32 spMiniMap2;
// CMiniMap vtable + the +0x2C subobject vtable (ctor stores).
// Current BDAT table pointer / per-row name-pointer table used by the ctor.
extern const char* sMiniMapLayoutNames[];

#include "kyoshin/CTaskGame.hpp"

#include "kyoshin/code_801F3BE0.hpp"
#include "kyoshin/CUICfManager.hpp"

// Cross-TU layout/BDAT helpers. Plain C++ declarations so MWCC re-derives the
// retail mangled names (buildLayout__FPP... / drawLayout__FPP... / ...).
// BDAT/string helpers with unmangled retail symbols: extern "C" keeps the
// emitted call relocs matching retail (Bdat_GetTable_AA34 / getFP__FPCc).
extern "C" void* Bdat_GetTable_AA34();   // matches code_801862C0.hpp's declaration
// Gimmick-row helpers used by MiniMapRebuildGimmickPanes (unmangled retail symbols;
// same declaration scheme as CFloorMap.hpp).
extern "C" unsigned long CtrlObjectParam_GetWorkField50();
extern "C" int isVisionAnimating();
// Move() gate/state helpers (unmangled retail symbols - C linkage).
extern "C" u32 isMapSelectActive();
extern "C" bool MapSelectSCIsCreated();

// (getCameraDataBlock__Q22cf13CfGameManagerFv: single winning decl on
// CfGameManagerApi.hpp; this TU uses the CfGameManager.hpp member form.)
// cf::CfGameManager pad-mode probe with an explicit int arg: the retail symbol
// is the pre-mangled name, so it must stay C linkage (inside extern "C") or
// MWCC appends a __Fi suffix (see CQuestWindow.hpp:280 block). The s16 inline
// wrapper in CfGameManager.hpp would DCE the `li r3,-1` arg setup.
// CProcess base ctor is out-of-line in retail (CProcess is abstract, so the
// factory calls it as a plain function).
void __ct__8CProcessFv(CProcess* self);

class CMMClock : public IWorkEvent {
public:
    CMMClock();
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

class CMMTex : public IWorkEvent {
public:
    CMMTex() : mFileHandle(0), mData(0), mReady(0), mRow(0), mMode(0) {}
    virtual ~CMMTex();
    bool OnFileEvent(CEventFile* pEventFile);

    CFileHandle* mFileHandle; // 0x4 - requested file handle
    u8* mData;                // 0x8 - loaded file data buffer
    u8 mReady;                // 0xC - load-complete flag
    u8 mRow;                  // 0xD - selected row
    u8 mMode;                 // 0xE - resource variant
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

// Marker-table region 0x3C..0x178; the three loader flags live in the
// CMMTex subobject at +0x3C..+0x3E (CMiniMap::mSub).
struct CMMTableBlock {
    CMMTableBlock(const char* fmt);
    u32 mW[0x4F];    // 0x3C..0x178 (MiniMapBuildMarkerPanes marker table)
};


// Minimap map-image loader object (MiniMapRequestMapImage): requests the current map's
// image arc into MEM2 and records the selected BDAT row / resource variant.
struct CMMMapImg {
    void* mVtable;              // 0x00 (untouched here)
    CFileHandle* mFileHandle;   // 0x04 - requested file handle
    u8* mData;                  // 0x08 - loaded buffer (freed on re-request)
    u8 mReady;                  // 0x0C - load-complete flag
    u8 mRow;                    // 0x0D - selected BDAT row (forced min 1)
    u8 mMode;                   // 0x0E - selected unlock-gated resource variant
    void MiniMapRequestMapImage();
};

// Gimmick/marker view tables used by MiniMapResetGimmickTables / MiniMapFillMarkerEntry /
// MiniMapUpdateGimmickRows (subobject of the minimap object at +0x17C).
// Pane-entry record written by MiniMapUpdateGimmickRows: integer-truncated grid coords
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

// name-guess: embedded gimmick table at CMiniMap+0x178; retail name not recovered.
struct CMiniMapGimmickData {
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
    u32 FillMarkerEntry();
    void InitGimmickTables(u32 table, void* layout, f32 scale);
};

// Gimmick-view region 0x178..0x820.
struct CMMViewBlock {
    CMMViewBlock();
    u32 mW[0x1AA];   // MiniMapInitGimmickTables gimmick view
};

class CMiniMap : public IWorkEvent {
public:
    CMiniMap();
    virtual ~CMiniMap();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    CFileHandle* mFileHandle;              // 0x04 - requested file handle
    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x08 - arc resource accessor
    nw4r::lyt::Layout* mLayout0C;          // 0x0C..0x10 (drawn by cbRenderBefore)
    nw4r::lyt::AnimTransform* mAnimTrans0; // 0x10 - animation transform 0
    nw4r::lyt::AnimTransform* mAnimTrans1; // 0x14 - animation transform 1
    u8 field_0x18;                         // 0x18
    u8 field_0x19;                         // 0x19
    u8 mReady;                             // 0x1A - load-complete flag
    u8 mFlag1B;                            // 0x1B
    u16 field_0x1C;                        // 0x1C - BDAT pane id (BdatGetS16Direct)
    u16 field_0x1E;                        // 0x1E - BDAT pane id (BdatGetS16Direct)
    f32 mField20;                          // 0x20 - marker grid scale
    char* mField24;                        // 0x24 - getFP-resolved pane name
    u32 mField28;                          // 0x28 - getFP-resolved layout name
    CMMTex mSub;                           // 0x2C..0x3C (real loader subobject)
    CMMTableBlock mField3C;                // 0x38..0x178 (marker-table region, MiniMapBuildMarkerPanes)
    CMMViewBlock mField17C;                // 0x178..0x820 (gimmick-view region, MiniMapInitGimmickTables)
    u32 mPad820;                           // 0x820..0x824
    // Raw UnkClass_8045F564 storage: retail constructs these mid-ctor body
    // (after the table init calls), so they are placement-new'd there instead
    // of being auto-constructed at ctor entry.
    UnkClass_8045F564 m824;                // 0x824..0x834 (destroyed last)
    UnkClass_8045F564 m834;                // 0x834..0x844 (destroyed first)
};

class CMenuMiniMap2 : public IUICf, public IWorkEvent, public IScnRender {
public:
    CMenuMiniMap2(CScn* scn);
    virtual ~CMenuMiniMap2();
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);  // IScnRender override (+0x5C subobject)

    // name-guess: create (singleton factory; JP names sibling CMenu* factories
    // CMenuSymbolMark::create / CMenuKeyAssign::create(CProcess*, CScn*)).
    static CMenuMiniMap2* create(CProcess* parent, CScn* scene);

    CScn* mScn;                   // 0x60 - owning scene (removeRenderCB receiver)
    CMMClock mClock;              // 0x64..0x90 (declared first -> destroyed last)
    CMiniMap mMiniMap;            // 0x90..0x8D4 (declared second -> destroyed first)
    u8 mField8D4;                 // 0x8D4
};

// Table object initialized by MiniMapInitTable3x5: BDAT table pointer plus 3x5
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
    void BuildMarkerPanes(void* table, void* layout, f32 scale);
};

// Same-unit minimap helpers whose retail symbols are unmangled (func_*):
// extern "C" keeps the emitted call reloc matching retail
// (MiniMapTeardownMarkerPanes) instead of re-deriving a mangled name.
extern "C" void MiniMapTeardownMarkerPanes(CMiniMap* self);
MiniMapTable* MiniMapInitTable3x5(MiniMapTable* self);
void MiniMapResetGimmickTables(CMiniMapGimmickData* self);
// Per-frame gimmick-view updater (retail unmangled symbol).
extern "C" void MiniMapUpdateGimmickRows(CMiniMapGimmickData* self);
