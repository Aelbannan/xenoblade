#pragma once

#include <types.h>
#include <revolution/MTX.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/IUICf.hpp"

class CProcess;
class CScn;
struct SymbolMarkRenderItem;

struct SymbolMarkEntry {
    u32 unk00;    // 0x00
    u32 unk04;    // 0x04
    u8* layout; // 0x08
    u8 flag0;     // 0x0C
    u8 flag1;     // 0x0D
    u8 flag2;     // 0x0E
    u8 pad0F;     // 0x0F
    f32 worldX;   // 0x10
    f32 worldY;   // 0x14
    f32 worldZ;   // 0x18
    u32 unk1C;    // 0x1C
};

class CMenuSymbolMark : public IUICf, public IWorkEvent, public IScnRender {
public:
    CMenuSymbolMark(CScn* scn);
    virtual ~CMenuSymbolMark();

    static CMenuSymbolMark* create(CProcess* parent, CScn* scene);

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);

    static CMenuSymbolMark* sInstance;

    /* 0x60 */ CScn* mScn; // owning scene
    UnkClass_8045F564 mUnkClass; // 0x64 (size 0x10)

    SymbolMarkEntry mEntries[16]; // 0x74-0x273

    u8 mField_274; // 0x274
    u8 mEntryCount; // 0x275
    u8 _pad276[2]; // 0x276
    f32 mTimer; // 0x278
    u8* mArchiveFP; // 0x27C
    u8* mSomeFP; // 0x280
    u8* mAnotherFP; // 0x284
    u32 mSomeValue; // 0x288
    u32 mSomeValue2; // 0x28C
    u32 mSomeValue3; // 0x290
    u32 mSomeValue4; // 0x294
    u32 mBuffer[0x80]; // 0x298-0x497
    u32 mField_498; // 0x498
    f32 mField_49C; // 0x49C
    u32 mArray4A0[0x80]; // 0x4A0-0x69F
    u32 mField_6A0; // 0x6A0
    f32 mField_6A4; // 0x6A4
    u32 mArray6A8[0x80]; // 0x6A8-0x8A7
    u32 mField_8A8; // 0x8A8
    f32 mField_8AC; // 0x8AC
    SymbolMarkRenderItem* mRenderItem; // 0x8B0
    void SymMark_RebuildEntryBuf();
    void SymMark_RebuildIdsA();
    void SymMark_RebuildIdsB();
    void SymMark_AddTouchMark();
    void SymMark_Refresh();
    void SymMark_AddKindMarks();
    void SymMark_SyncBufMarks();
    void SymMark_AddRangeMarks();
    void SymMark_Store(u32 id, Vec* pos, void* argC, u32 argD, u32 argE);
};

// Header of the arrow resource blob held by CArrow3D::mDataPtr: offsets are
// measured from the start of the aligned resource handle.
struct ArrowResourceHeader {
    u32 arrPosOff;   // 0x00: vertex array for attr 9 (stride 0xC)
    u32 arrUvOff;    // 0x04: attr 0xA (stride 0xC)
    u32 arrColOff;   // 0x08: attr 0xB (stride 4)
    u32 arrIdxOff;   // 0x0C: attr 0xD index array (stride 8), 0 = none
    u8 pad10[4];
    u32 dlListOff;   // 0x10: display-list entry table offset
    u32 dlCount;     // 0x14: number of display-list entries
};

class CArrow3D : public CTTask<CArrow3D>, public IScnRender {
public:
    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);
    virtual ~CArrow3D();

    // 0x00-0x54: CTTask<CArrow3D>
    // 0x54: IScnRender secondary base
    u8* mDataHandle; // 0x58
    u8* mDataPtr; // 0x5C
    f32 mPosX; // 0x60
    f32 mPosY; // 0x64
    f32 mPosZ; // 0x68
    u8 mFlag6C; // 0x6C
    u8 _pad6D[3]; // 0x6D
    u8* mLayout; // 0x70
    u8 _pad74[4]; // 0x74
    u8* mAlignedData; // 0x78
};

// ---- imports folded from the former CMenuSymbolMarkImports.hpp ----
#include "kyoshin/menu/CMenuSymbolMark.hpp"

#include <types.h>
// KEEP (blockers-A 2026-10-23): 32-bit view of BdatGetU16Direct (canonical u16 in
// kyoshin/code_80135FDC_Bdat.hpp); the u16 form adds clrlwi and moves the
// callers here further from retail.
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CTTask.hpp"
#include "kyoshin/IUICf.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"



// Scene interface used by Term (retail removeRenderCB__4CScnFP10IScnRender).
#include <nw4r/lyt.h>

#include "monolib/scn/CScn.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// Global float constant pool (SDA-relative float loads). Declared const so
// MWCC treats the loads as constants and hoists them above the frame stores.
extern const f32 sMarkZero;
extern const f32 sMarkResetB;
extern const f32 sMarkResetC;
extern const f32 sMarkTimerLimit;
extern const f32 sMarkTimerExpired;
extern const f32 sMarkStep;
extern const f32 sMarkRangeLimitSq;
extern const f32 sMarkFarYOffset;
extern const f32 sWideScaleFactor;
extern const f32 sMarkScaleBase;
extern const f32 sMarkCenterX;
extern const f32 sMarkCenterY;
extern const f32 sMarkSlideRate;
extern const f32 sMarkSlideMin;
extern f32 sMarkSlideMax;
// Magic int-to-float conversion bias (MWCC 0x4330/xoris idiom).
extern const f64 sS16ToF64Bias;


// Object tracked at CMenuSymbolMark+0x8B0: Term flags the byte at +0x39
// before dropping the pointer.
// Layout: CProcess base prefix (built by __ct__8CProcessFv), then an
// IWorkEvent-style vtable at +0x10, the ptmf pairs at +0x3C/+0x48 (cleared to
// __ptmf_null by Init), a second interface vtable at +0x54, and payload words
// at +0x70..+0x78 (+0x70 doubles as a state id compared against 0x1A).
struct SymbolMarkRenderItem {
    u8 procPrefix[0x10];   // 0x00: CProcess base
    void* vt10;            // 0x10: interface vtable
    u8 pad14[0x39 - 0x14];
    u8 flag39;             // 0x39: flagged by Term before release
    u8 pad3A[0x3C - 0x3A];
    u32 ptmfMove[3];       // 0x3C
    u32 ptmfDraw[3];       // 0x48
    void* vt54;            // 0x54: second interface vtable
    u8 pad58[0x60 - 0x58];
    u32 field60;           // 0x60
    u32 field64;           // 0x64
    u32 field68;           // 0x68
    u8 field6C;            // 0x6C: live flag
    u8 pad6D[0x70 - 0x6D];
    u32 state70;           // 0x70 (compared against 0x1A by SymMark_Store)
    u8 field74;            // 0x74
    u8 pad75[0x78 - 0x75];
    u32 field78;           // 0x78 (zeroed by Init)
};


// CDeviceVI is the real monolib class; included above the folded imports.
#include "monolib/device/CDeviceVI.hpp"

// Field view over actor objects reached through findObjectById: measured-id
// halfword at +0x8C, dispatch-kind byte at +0x91, marker name word at +0x74.
struct CfActorFields {
    u8 pad00[0x74];
    u32 name74;               // +0x74
    u8 pad78[0x8C - 0x78];
    u16 field8C;              // +0x8C
    u8 pad8E[0x91 - 0x8E];
    u8 kind91;                // +0x91
};

// Fixed 0x4C-byte row of the scene menu table; measured-id halfword at 0x1C.
struct MenuTableRow {
    u8 pad00[0x1C];
    u16 field1C;
    u8 pad1E[0x4C - 0x1E];
};

// Scene menu table base: row count pinned at +0x9800, rows start at +0x00.
struct SceneMenuTable {
    u8 pad00[0x9800];
    u32 rowCount9800;
};

// Scene-bound transform block returned by Scn_FindCamItem(scene, -1):
// normal-transform matrix at +0x9C and projection rows at +0x118/+0x120.
struct ScnXformBlock {
    u8 pad00[0x9C];
    f32 mtx9C[12];            // +0x9C
    u8 padCC[0x118 - 0xCC];
    f32 proj118[4];           // +0x118
    f32 proj120[4];           // +0x120
};

// Retail imports used by SymMark_AddTouchMark.
extern "C" void* prepareReslistArg(int idx);
extern "C" void* CPartsChange_GetLandmarkTable();

// Input record consumed by SymMark_Place: three world floats at +0xC/+0x1C/+0x2C.
struct EntryInputPos {
    u8 pad00[0x0C];
    f32 posX;                 // +0x0C
    u8 pad10[0x1C - 0x10];
    f32 posY;                 // +0x1C
    u8 pad20[0x2C - 0x20];
    f32 posZ;                 // +0x2C
};

// Current map/area id global (read by SymMark_Store's state gate).
extern u32 lbl_eu_80664184;

// Pointer-to-member-function triple (copied as a struct so MWCC lowers the
// assignment with its load-ahead schedule).
struct PtmfTriple {
    u32 f0;
    u32 f1;
    u32 f2;
};


// CfActorView pad deleted: use cf::CfObject / cf::CfObjectMove for 0xAC/0x12C/0x160 and CfObjectMove extended 0x228, and cf::CActorParam for 0x128 hp gate

// Circular marker-object list shared by getReslistB68 / getReslistBC8:
// sentinel node at +0x04, nodes chained through their next at +0x00 with the
// payload object at +0x08.
struct CfObjListNode {
    CfObjListNode* next; // +0x00
    u8 pad04[4];         // +0x04
    void* object;        // +0x08
};
struct CfObjList {
    u32 field00;          // +0x00
    CfObjListNode* sentinel; // +0x04
};

// Scene-bound object view used by SymMark_RebuildIdsB: two world-space positions
// (anchor at +0x10C, extent at +0x138) fed to CheckViewAngle60.
struct ScnObjPositions {
    u8 pad00[0x10C];
    f32 posX;  // +0x10C
    f32 posY;  // +0x110
    f32 posZ;  // +0x114
    u8 pad118[0x138 - 0x118];
    f32 extX;  // +0x138
    f32 extY;  // +0x13C
    f32 extZ;  // +0x140
};

#include "kyoshin/cf/CfGameManager.hpp" // getPlayer

// Static-file loader (retail CLibStaticData::getStaticFileData).
class StaticDataHandle;
class CLibStaticData {
public:
    static bool getStaticFileData(const char* path, StaticDataHandle** out,
                                  u32* outSize);
};

// Aligned-binary descriptor returned through getStaticFileData: offset of the
// 32-byte-aligned payload lives at +0x18.
struct StaticDataObj {
    u8 pad00[0x18];
    u32 dataOffset; // +0x18
};

namespace nw4r {
namespace db {
void Panic(const char* file, int line, const char* msg, unsigned char sev);
void Warning(const char* file, int line, const char* msg, ...);
} // namespace db
} // namespace nw4r

// Real ArcResourceAccessor provides GetResource at 0xC (replaces ArcAccessorView pad)


// Extra float constant pool entries used by CArrow3D::cbRenderBefore.
extern const f32 sArrowAngleScale;
extern const f32 sFidxUnitScale;
extern const f32 sArrowProjNear;
extern const f32 sArrowProjFar;
extern const f32 sArrowLightScale;
extern const f32 sArrowLightZ;
extern const f32 sArrowLodBias;

// Real CActorParam provides CActorParam_getHp at 0x128 (replaces PlayerHpGate pad)

struct ArrowDLEntry {
    u32 dlOff;
    u32 dlSize;     // 0x04: display list byte size
    u16 texIdx;     // 0x08: texture index
    u16 pltIdx;     // 0x0A: palette index
    u32 flags;      // 0x0C: bit0 selects the per-entry Z mode (second pass)
};

// Unmangled retail helper imports.
extern "C" {
void CamMan_ProjectToScreen(nw4r::math::VEC3* out, ScnXformBlock* pose,
                   const nw4r::math::VEC3* in);
// Bdat_GetTable_AA34 / Bdat_GetMaxRow_B1EC are declared in the owning TU's
// import block in CMenuSymbolMark.cpp; the u8* callers convert to void*.

// Scn_FindCamItem provided by CfObjectMove.hpp as void* Scn_FindCamItem(void*, int)
}

// Naturally mangled retail helpers (global scope: no extern "C" needed).
void* getFP(const char* name);
