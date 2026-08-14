#pragma once

#include <types.h>
#include "kyoshin/cf/IFactoryEvent.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/IBattleEvent.hpp"
#include "kyoshin/cf/chain/CChain.hpp"
#include "kyoshin/cf/chain/UnkClass_800D8DBC.hpp"
#define func_800F3970 battleManagerVisionDispatchUnused
#include "kyoshin/cf/CVision.hpp"
#undef func_800F3970
// CChain.hpp and CSuddenCommu.hpp expose incompatible provisional signatures
// for imports also declared by CBattleManager.cpp. CBattleManager only needs
// the CSuddenCommu type here, so hide those unused declarations in this header.
#define func_802A3680 battleManagerSuddenCommuVoiceUnused
#define func_80174C98 battleManagerSuddenCommuActorQueryUnused
#define func_800D81A8 battleManagerSuddenCommuScaleUnused
#define func_8018C820 battleManagerSuddenCommuGaugeUnused
#define func_80260264 battleManagerSuddenCommuStatQueryUnused
#define func_8006EF04__Fi battleManagerSuddenCommuGameFlagUnused
#define func_80086F9C__Q22cf13CfGameManagerFv battleManagerSuddenCommuGateUnused
#define func_80080F44__Q22cf13CfGameManagerFv battleManagerSuddenCommuObjectUnused
#define func_8017FD4C battleManagerSuddenCommuTriggerQueryUnused
#define func_8017FEF0 battleManagerSuddenCommuTriggerUpdateUnused
#define lbl_eu_80663E14 battleManagerSuddenCommuSceneUnused
#define lbl_eu_80663E24 battleManagerSuddenCommuFlagsUnused
#include "kyoshin/cf/CSuddenCommu.hpp"
#undef lbl_eu_80663E24
#undef lbl_eu_80663E14
#undef func_8017FEF0
#undef func_8017FD4C
#undef func_80080F44__Q22cf13CfGameManagerFv
#undef func_80086F9C__Q22cf13CfGameManagerFv
#undef func_8006EF04__Fi
#undef func_80260264
#undef func_8018C820
#undef func_800D81A8
#undef func_80174C98
#undef func_802A3680
#include "monolib/util.hpp"
#include <cstring>

namespace cf{
    class UnkClass_8018C5FC {
    public:
        UnkClass_8018C5FC();
        
        u8 unk0[8];
    };

    class UnkClass_80192BF4 {
    public:
        UnkClass_80192BF4();
        void __ct__80192C10();

        u8 unk0[0xC];
    };

    // 8 bytes: 4-byte key + 1-byte count + 3 padding
    struct CBattleManager_Struct1 {
        CBattleManager_Struct1() : key(0), count(0) {
        }

        s32 key;    // +0x00
        u8 count;   // +0x04
        u8 pad[3];  // +0x05..0x07
    };

    struct CBattleManager_Struct2 {
        CBattleManager_Struct2(){
            clear();
        }

        void clear(){
            std::memset(unk0, 0, sizeof(unk0));
        }

        CBattleManager_Struct1 unk0[32];
    };

    //size: 0x283D8
    class CBattleManager : public IFactoryEvent {
    public:
        CBattleManager();
        virtual ~CBattleManager(); //0x8
        virtual void FactoryEvent2(); //0x10
        virtual void func_80085220(u32 r4, u32 r5); //0x1C
        virtual void func_800E2584(u32 mask); //0x20
        virtual void func_800F42A0(); //0x24
        virtual void func_800885F0(); //0x28
        virtual void func_800EA410(); //0x2C
        virtual void* func_800EA420(); //0x30
        virtual void func_800EA460(float a, float b, unsigned long c); //0x34
        virtual void func_800EA470(); //0x38
        virtual void func_800EA998(); //0x3C

        void* func_800EA444();

        static CBattleManager* getInstance();
        static void func_800D9190();
        static void func_800D91D0();
        void func_800D9218();

        //0x0: vtable
        //0x0-4: IFactoryEvent
        reslist<CfObjectActor*> mActorList1; //0x4
        reslist<CfObjectActor*> mActorList2; //0x24
        reslist<CfObjectActor*> mActorList3; //0x44
        reslist<IBattleEvent*> mBattleEventList; //0x64
        u32 unk84; //0x84
        float unk88;
        u32 unk8C;
        float unk90;
        CBattleManager_Struct2 unk94;
        UnkClass_8018C5FC unk194; //0x194
        UnkClass_80192BF4 unk19C; //0x19C
        CChain mChain; //0x1A8
        UnkClass_800D8DBC unk20C8; //0x20C8
        CSuddenCommu mSuddenCommu; //0x216C
        CVision mVision; //0x219C

        static u32 lbl_804F8228[];

    protected:
        // static CBattleManager* spInstance; -> extern "C" lbl_eu_80663F00 in .cpp
    };
}

// --- Standalone function access structs (CBattleManager.cpp) ---

// Intrusive linked-list node: +0x00 = next, +0x04 = unk, +0x08 = data ptr
struct SimpleListNode {
    SimpleListNode* next; // +0x00
    u32 field_4;          // +0x04
    void* data;           // +0x08
};

// Return layout of func_8009EC9C (accessed at +0x1C, cast_int_arith L63/L80)
struct UnkStruct_8009EC9C_Ret {
    u8 pad_00[0x1C];
    u8 unk1C;
};

// Layout for func_800EA384 self: list sentinel ptr at +0x08
struct Func800EA384_Self {
    u8 pad_00[0x08];
    SimpleListNode* listHead;
};

// Layout for func_800F4004 this_: list sentinel ptr at +0x48
struct Func800F4004_Self {
    u8 pad_00[0x48];
    SimpleListNode* listHead;
};

// Object layout used by func_800EA9A8 / func_800DBA2C
// Has a field at +0x3f00 (flags), +0x3f10 (ptr), +0x3f28 (u16)
struct BattleObjAccessor {
    u8 pad_00[0x3f00];
    u32 field_3f00;     // flags at +0x3f00
    u8 pad_3f04[0x0C];  // 0x3f04-0x3f0F
    void* field_3f10;   // ptr at +0x3f10
    u8 pad_3f14[0x14];  // 0x3f14-0x3f27
    u16 field_3f28;     // u16 at +0x3f28
};

// Object layout used by func_800DBA2C for arg2 access at +0x78
// Has a field at +0x50 (ptr), +0x78 (flags)
struct BattleMoveObjAccessor {
    u8 pad_00[0x50];
    void* field_50;     // ptr at +0x50
    u8 pad_54[0x24];
    u32 field_78;       // flags at +0x78
};

// Object layout used by func_800DBA2C for r31 (*(arg2+0x50))
// Has fields at +0x3c (u16 type), +0x40 (u16)
struct BattleSubObjAccessor {
    u8 pad_00[0x3c];
    u16 type_3c;        // type at +0x3c
    u16 field_3e;       // padding?
    u16 field_40;       // value at +0x40
};

// Object layout for the removed-target access in FactoryEvent2 / func_800D9CA0
// +0x3ED4 = ptr to vtable-holder, +0x3F00 = flags
struct BattleRemoveObjAccessor {
    u8 pad_00[0x3ED4];
    void* field_3ed4;    // +0x3ED4 ptr (has vtable[0x7C] call)
    u8 pad_3ed8[0x28];   // 0x3ED8-0x3EFF
    u32 field_3f00;      // +0x3F00 flags
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Battle event dispatch table entry for lbl_eu_804FC828 (12 bytes each)
struct BattleTableEntry {
    u32 selector;
    s16 val;
    s8 byteVal;
    u8 pad_07;
    u32 pad_08;
};

extern "C" void __dl__FPv(void*);
extern "C" void __dla__FPv(void*);
extern "C" float lbl_eu_80666DDC;
extern "C" float lbl_eu_80666DD4;
extern "C" f64 lbl_eu_80666DE0;
extern "C" BattleTableEntry lbl_eu_804FC828[];
extern "C" u32 lbl_eu_80663E24;
