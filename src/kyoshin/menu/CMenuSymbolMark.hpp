#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CTTask.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"



// Scene interface used by Term (retail removeRenderCB__4CScnFP10IScnRender).
#include <nw4r/lyt.h>

class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};

// Global float constant pool (SDA-relative float loads). Declared const so
// MWCC treats the loads as constants and hoists them above the frame stores.
extern const f32 lbl_eu_806670D4;
extern const f32 lbl_eu_806670D8;
extern const f32 lbl_eu_806670DC;
extern const f32 lbl_eu_806670E0;
extern const f32 lbl_eu_806670E4;
extern const f32 lbl_eu_806670E8;
extern const f32 lbl_eu_806670EC;
extern const f32 lbl_eu_806670F0;
extern const f32 lbl_eu_806670F4;
extern const f32 lbl_eu_806670F8;
extern const f32 lbl_eu_806670FC;
extern const f32 lbl_eu_80667100;
extern const f32 lbl_eu_80667104;
extern const f32 lbl_eu_80667108;
extern f32 lbl_eu_8066710C;
extern f32 lbl_eu_8066A1F8;
// Magic int-to-float conversion bias (MWCC 0x4330/xoris idiom).
extern const f64 lbl_eu_80667110;

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
    u32 state70;           // 0x70 (compared against 0x1A by func_8011E540)
    u8 field74;            // 0x74
    u8 pad75[0x78 - 0x75];
    u32 field78;           // 0x78 (zeroed by Init)
};


class CDeviceVI {
public:
    static void waitForDrawDone();
    static bool isWideAspectRatio();
};

// Object prefix view: interface vtable installed at +0x10 by the ctor.
struct ProcPrefixView {
    u8 pad00[0x10];
    void* vt10;
};

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

// Scene-bound transform block returned by func_80496264(scene, -1):
// normal-transform matrix at +0x9C and projection rows at +0x118/+0x120.
struct ScnXformBlock {
    u8 pad00[0x9C];
    f32 mtx9C[12];            // +0x9C
    u8 padCC[0x118 - 0xCC];
    f32 proj118[4];           // +0x118
    f32 proj120[4];           // +0x120
};

// Retail imports used by func_8011EFB0.
extern "C" void* func_800B6CF8(u32 idx);
extern "C" void* func_80193804();

// Input record consumed by func_8011E778: three world floats at +0xC/+0x1C/+0x2C.
struct EntryInputPos {
    u8 pad00[0x0C];
    f32 posX;                 // +0x0C
    u8 pad10[0x1C - 0x10];
    f32 posY;                 // +0x1C
    u8 pad20[0x2C - 0x20];
    f32 posZ;                 // +0x2C
};

// Raw lyt pane field view: translate/rotate/scale triples + visibility flag.
struct LytPaneFields {
    u8 pad00[0x2C];
    f32 trX, trY, trZ;        // 0x2C
    f32 rotX, rotY, rotZ;     // 0x38
    f32 scX, scY, scZ;        // 0x44
    u8 pad50[0xBB - 0x50];
    u8 flagBB;                // 0xBB bit0: hidden flag
};

// Layout object view: root pane pointer at +0x10.
struct LytLayoutFields {
    u8 pad00[0x10];
    LytPaneFields* rootPane;  // +0x10
};

// Real Pane type provides FindPaneByName at 0x3C (replaces LytPaneFindView pad)

// Current map/area id global (read by func_8011E540's state gate).
extern u32 lbl_eu_80664184;

// Pointer-to-member-function triple (copied as a struct so MWCC lowers the
// assignment with its load-ahead schedule).
struct PtmfTriple {
    u32 f0;
    u32 f1;
    u32 f2;
};

class CMenuSymbolMark : public CProcess {
public:
    CMenuSymbolMark(CScn* scn, u32 idx = 0);
    virtual ~CMenuSymbolMark();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x00-0x3C: CProcess (CDoubleListNode + vtable + CChildListNode + flags)
    // 0x3C: ptmfMove
    PtmfTriple ptmfMove; // 0x3C
    // 0x48: ptmfDraw
    PtmfTriple ptmfDraw; // 0x48
    u8 mUnk54; // 0x54
    u8 mUnk55; // 0x55
    u8 _pad56[2]; // 0x56
    // 0x58: IWorkEvent vtable ptr (manual, not via inheritance) - kept void*
    void* mIWorkEventVt; // 0x58
    // 0x5C: IScnRender vtable ptr (manual) - kept void*
    void* mIScnRenderVt; // 0x5C
    CScn* mScn; // 0x60
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
};

// CfActorView pad deleted: use cf::CfObject / cf::CfObjectMove for 0xAC/0x12C/0x160 and CfObjectMove extended 0x228, and cf::CActorParam for 0x128 hp gate

// Circular marker-object list shared by func_800B6BEC / func_800B6C58:
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

// Field view over the marker objects walked by func_8011EDDC: flag bits at
// +0x64 (0x10000/0x20000 gate) and a name/id word at +0x74.
struct CfMarkerFields {
    u8 pad00[0x64];
    u32 flags64;
    u8 pad68[0x74 - 0x68];
    u32 name74;
};

// Scene-bound object view used by func_8011EDDC: two world-space positions
// (anchor at +0x10C, extent at +0x138) fed to func_8013A4B4.
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

namespace cf {
class CfGameManager {
public:
    static void* getPlayer(int index);
};
} // namespace cf

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

class CArrow3D : public CTTask<CArrow3D> {
public:
    virtual ~CArrow3D();
    void cbRenderBefore();
    void Term();
    void Init();
    // NOTE: no Move() override - retail leaves the vtable's Move slot on
    // CTTask<CArrow3D>::Move (see split1 .data vtable) and emits the empty
    // Move__8CArrow3DFv as a separate symbol (stubbed below). Declaring an
    // override here would shadow the CTTask<CArrow3D>::Move specialization
    // and MWCC would drop the retail Move__17CTTask<8CArrow3D>Fv symbol.

    // 0x00-0x54: CTTask<CArrow3D>
    // 0x54: IScnRender vtable ptr (manual) - kept void*
    void* mIScnRenderVt; // 0x54
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

// Extra float constant pool entries used by CArrow3D::cbRenderBefore.
extern const f32 lbl_eu_80667118;
extern const f32 lbl_eu_8066711C;
extern const f32 lbl_eu_80667120;
extern const f32 lbl_eu_80667124;
extern const f32 lbl_eu_80667128;
extern const f32 lbl_eu_8066712C;
extern const f32 lbl_eu_80667130;
extern const f32 lbl_eu_8066A1FC;
extern const f32 lbl_eu_8066A210;

// Real CActorParam provides CActorParam_UnkVirtualFunc37 at 0x128 (replaces PlayerHpGate pad)

// Raw field views over the arrow resource blob pointed to by mDataPtr (+0x5C).
struct ArrowResPtrView {
    u32 arrPosOff;   // 0x00: vertex array for attr 9 (stride 0xC)
    u32 arrUvOff;    // 0x04: attr 0xA (stride 0xC)
    u32 arrColOff;   // 0x08: attr 0xB (stride 4)
    u32 arrIdxOff;   // 0x0C: attr 0xD index array (stride 8), 0 = none
    u8 pad10[4];
    u32 dlListOff;   // 0x10: display-list entry table offset
    u32 dlCount;     // 0x14: number of display-list entries
};

struct ArrowDLEntry {
    u32 dlOff;      // 0x00: display list offset into the archive blob
    u32 dlSize;     // 0x04: display list byte size
    u16 texIdx;     // 0x08: texture index
    u16 pltIdx;     // 0x0A: palette index
    u32 flags;      // 0x0C: bit0 selects the per-entry Z mode (second pass)
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int deleteFlag);

// Unmangled retail helper imports.
extern "C" {
u32 func_80136254(const void* table, const char* str, u32 index);
int func_80138234(const void* table, u32 index);
void func_8049B59C(nw4r::math::VEC3* out, ScnXformBlock* pose,
                   const nw4r::math::VEC3* in);
void* func_8003AA34(void);
u32 func_8003B1EC(u8* bdat);
}
// func_80496264 provided by CfObjectMove.hpp as void* func_80496264(void*, int)

// Naturally mangled retail helpers (global scope: no extern "C" needed).
void* getFP(const char* name);
extern u32 lbl_eu_80573D18[];