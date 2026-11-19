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
    virtual void cbRenderBefore();

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
    virtual void cbRenderBefore();
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
