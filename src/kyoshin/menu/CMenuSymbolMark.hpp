#pragma once

#include <types.h>
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CTTask.hpp"

class CScn;

struct SymbolMarkEntry {
    u32 unk00;    // 0x00
    u32 unk04;    // 0x04
    void* layout; // 0x08
    u8 flag0;     // 0x0C
    u8 flag1;     // 0x0D
    u8 flag2;     // 0x0E
    u8 pad0F;     // 0x0F
    f32 worldX;   // 0x10
    f32 worldY;   // 0x14
    f32 worldZ;   // 0x18
    u32 unk1C;    // 0x1C
};

class CMenuSymbolMark : public CProcess {
public:
    CMenuSymbolMark(CScn* scn);
    virtual ~CMenuSymbolMark();
    void Init();
    void Term();
    void Move();
    void cbRenderBefore();

    // 0x00-0x3C: CProcess (CDoubleListNode + vtable + CChildListNode + flags)
    // 0x3C: ptmfMove (12 bytes)
    u32 ptmfMove[3]; // 0x3C
    // 0x48: ptmfDraw (12 bytes)
    u32 ptmfDraw[3]; // 0x48
    u8 mUnk54; // 0x54
    u8 mUnk55; // 0x55
    u8 _pad56[2]; // 0x56
    // 0x58: IWorkEvent vtable ptr (manual, not via inheritance)
    void* mIWorkEventVt; // 0x58
    // 0x5C: IScnRender vtable ptr (manual)
    void* mIScnRenderVt; // 0x5C
    CScn* mScn; // 0x60
    UnkClass_8045F564 mUnkClass; // 0x64 (size 0x10)

    SymbolMarkEntry mEntries[16]; // 0x74-0x273

    u8 mField_274; // 0x274
    u8 mEntryCount; // 0x275
    u8 _pad276[2]; // 0x276
    f32 mTimer; // 0x278
    void* mArchiveFP; // 0x27C
    void* mSomeFP; // 0x280
    void* mAnotherFP; // 0x284
    u32 mSomeValue; // 0x288
    u32 mSomeValue2; // 0x28C
    u32 mSomeValue3; // 0x290
    u32 mSomeValue4; // 0x294
    u8 mBuffer[0x200]; // 0x298-0x497
    u32 mField_498; // 0x498
    f32 mField_49C; // 0x49C
    u32 mArray4A0[8][4]; // 0x4A0-0x69F
    u32 mField_6A0; // 0x6A0
    f32 mField_6A4; // 0x6A4
    u32 mArray6A8[8][4]; // 0x6A8-0x8A7
    u32 mField_8A8; // 0x8A8
    f32 mField_8AC; // 0x8AC
    void* mRenderItem; // 0x8B0
};

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
    // 0x54: IScnRender vtable ptr (manual)
    void* mIScnRenderVt; // 0x54
    void* mDataHandle; // 0x58
    void* mDataPtr; // 0x5C
    f32 mPosX; // 0x60
    f32 mPosY; // 0x64
    f32 mPosZ; // 0x68
    u8 mFlag6C; // 0x6C
    u8 _pad6D[3]; // 0x6D
    void* mLayout; // 0x70
    u8 _pad74[4]; // 0x74
    void* mAlignedData; // 0x78
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void __dt__17UnkClass_8045F564Fv(void* self, int deleteFlag);