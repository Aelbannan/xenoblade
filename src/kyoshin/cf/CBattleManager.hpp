#pragma once

#include <types.h>
#include "kyoshin/cf/IFactoryEvent.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/IBattleEvent.hpp"
#include "kyoshin/cf/chain/CChain.hpp"
#include "kyoshin/cf/chain/UnkClass_800D8DBC.hpp"
#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CSuddenCommu.hpp"
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

    struct CBattleManager_Struct1 {
        CBattleManager_Struct1() {
            std::memset(this, 0, sizeof(*this));
        }

        s32 key;   // +0x00
        u8 count;  // +0x04
        u8 pad[3]; // +0x05
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
        virtual int func_800885F0(u32 r4); //0x28
        virtual void func_800EA410(); //0x2C
        virtual void func_800EA420(); //0x30
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
        static CBattleManager* spInstance;
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

// Struct for func_800F3734: accessing CfObjectActor fields at known offsets
struct CfObjectActor_Fields_3F00 {
    u8 pad_00[0x3F00];
    u32 field_0x3F00;   // flags: bit 1 selects list 2 vs list 3
    u8 pad_3F04[0xC];
    u32 field_0x3F10;   // passed as arg to virtual function call
};

// Struct for func_800EA2A4 accessed object field at +0x3c (16-bit)
struct Func800EA2A4_Obj {
    u8 pad_00[0x3C];
    u16 field_0x3C;     // compared with 3
};

// Forward declarations for external functions called by func_800F3734
// (in CMenuBattleDamage unit)
extern void func_8010989C(u8 val);
extern void func_80109888(u8 val);
extern void func_80109874(u8 val);
extern void func_8010975C(u8 val);
extern void func_80109770(u8 val);
extern void func_80109734(u32 arg1, u32 arg2);

// Struct for func_800F3734: accessing field at +0x30 (flags)
struct Func800F3734_Param5_Fields {
    u8 pad_00[0x30];
    u32 field_0x30;
};

// Helper struct for virtual function call at vtable offset 0x2C8
// Must have the same vtable layout as CfObjectActor up to that offset
struct VirtualFunc2C8 {
    virtual void _v08() = 0;  // 0x08
    virtual void _v0C() = 0;  // 0x0C
    virtual void _v10() = 0;  // 0x10
    virtual void _v14() = 0;  // 0x14
    virtual void _v18() = 0;  // 0x18
    virtual void _v1C() = 0;  // 0x1C
    virtual void _v20() = 0;  // 0x20
    virtual void _v24() = 0;  // 0x24
    virtual void _v28() = 0;  // 0x28
    virtual void _v2C() = 0;  // 0x2C
    virtual void _v30() = 0;  // 0x30
    virtual void _v34() = 0;  // 0x34
    virtual void _v38() = 0;  // 0x38
    virtual void _v3C() = 0;  // 0x3C
    virtual void _v40() = 0;  // 0x40
    virtual void _v44() = 0;  // 0x44
    virtual void _v48() = 0;  // 0x48
    virtual void _v4C() = 0;  // 0x4C
    virtual void _v50() = 0;  // 0x50
    virtual void _v54() = 0;  // 0x54
    virtual void _v58() = 0;  // 0x58
    virtual void _v5C() = 0;  // 0x5C
    virtual void _v60() = 0;  // 0x60
    virtual void _v64() = 0;  // 0x64
    virtual void _v68() = 0;  // 0x68
    virtual void _v6C() = 0;  // 0x6C
    virtual void _v70() = 0;  // 0x70
    virtual void _v74() = 0;  // 0x74
    virtual void _v78() = 0;  // 0x78
    virtual void _v7C() = 0;  // 0x7C
    virtual void _v80() = 0;  // 0x80
    virtual void _v84() = 0;  // 0x84
    virtual void _v88() = 0;  // 0x88
    virtual void _v8C() = 0;  // 0x8C
    virtual void _v90() = 0;  // 0x90
    virtual void _v94() = 0;  // 0x94
    virtual void _v98() = 0;  // 0x98
    virtual void _v9C() = 0;  // 0x9C
    virtual void _vA0() = 0;  // 0xA0
    virtual void _vA4() = 0;  // 0xA4
    virtual void _vA8() = 0;  // 0xA8
    virtual void _vAC() = 0;  // 0xAC
    virtual void _vB0() = 0;  // 0xB0
    virtual void _vB4() = 0;  // 0xB4
    virtual void _vB8() = 0;  // 0xB8
    virtual void _vBC() = 0;  // 0xBC
    virtual void _vC0() = 0;  // 0xC0
    virtual void _vC4() = 0;  // 0xC4
    virtual void _vC8() = 0;  // 0xC8
    virtual void _vCC() = 0;  // 0xCC
    virtual void _vD0() = 0;  // 0xD0
    virtual void _vD4() = 0;  // 0xD4
    virtual void _vD8() = 0;  // 0xD8
    virtual void _vDC() = 0;  // 0xDC
    virtual void _vE0() = 0;  // 0xE0
    virtual void _vE4() = 0;  // 0xE4
    virtual void _vE8() = 0;  // 0xE8
    virtual void _vEC() = 0;  // 0xEC
    virtual void _vF0() = 0;  // 0xF0
    virtual void _vF4() = 0;  // 0xF4
    virtual void _vF8() = 0;  // 0xF8
    virtual void _vFC() = 0;  // 0xFC
    virtual void _v100() = 0; // 0x100
    virtual void _v104() = 0; // 0x104
    virtual void _v108() = 0; // 0x108
    virtual void _v10C() = 0; // 0x10C
    virtual void _v110() = 0; // 0x110
    virtual void _v114() = 0; // 0x114
    virtual void _v118() = 0; // 0x118
    virtual void _v11C() = 0; // 0x11C
    virtual void _v120() = 0; // 0x120
    virtual void _v124() = 0; // 0x124
    virtual void _v128() = 0; // 0x128
    virtual void _v12C() = 0; // 0x12C
    virtual void _v130() = 0; // 0x130
    virtual void _v134() = 0; // 0x134
    virtual void _v138() = 0; // 0x138
    virtual void _v13C() = 0; // 0x13C
    virtual void _v140() = 0; // 0x140
    virtual void _v144() = 0; // 0x144
    virtual void _v148() = 0; // 0x148
    virtual void _v14C() = 0; // 0x14C
    virtual void _v150() = 0; // 0x150
    virtual void _v154() = 0; // 0x154
    virtual void _v158() = 0; // 0x158
    virtual void _v15C() = 0; // 0x15C
    virtual void _v160() = 0; // 0x160
    virtual void _v164() = 0; // 0x164
    virtual void _v168() = 0; // 0x168
    virtual void _v16C() = 0; // 0x16C
    virtual void _v170() = 0; // 0x170
    virtual void _v174() = 0; // 0x174
    virtual void _v178() = 0; // 0x178
    virtual void _v17C() = 0; // 0x17C
    virtual void _v180() = 0; // 0x180
    virtual void _v184() = 0; // 0x184
    virtual void _v188() = 0; // 0x188
    virtual void _v18C() = 0; // 0x18C
    virtual void _v190() = 0; // 0x190
    virtual void _v194() = 0; // 0x194
    virtual void _v198() = 0; // 0x198
    virtual void _v19C() = 0; // 0x19C
    virtual void _v1A0() = 0; // 0x1A0
    virtual void _v1A4() = 0; // 0x1A4
    virtual void _v1A8() = 0; // 0x1A8
    virtual void _v1AC() = 0; // 0x1AC
    virtual void _v1B0() = 0; // 0x1B0
    virtual void _v1B4() = 0; // 0x1B4
    virtual void _v1B8() = 0; // 0x1B8
    virtual void _v1BC() = 0; // 0x1BC
    virtual void _v1C0() = 0; // 0x1C0
    virtual void _v1C4() = 0; // 0x1C4
    virtual void _v1C8() = 0; // 0x1C8
    virtual void _v1CC() = 0; // 0x1CC
    virtual void _v1D0() = 0; // 0x1D0
    virtual void _v1D4() = 0; // 0x1D4
    virtual void _v1D8() = 0; // 0x1D8
    virtual void _v1DC() = 0; // 0x1DC
    virtual void _v1E0() = 0; // 0x1E0
    virtual void _v1E4() = 0; // 0x1E4
    virtual void _v1E8() = 0; // 0x1E8
    virtual void _v1EC() = 0; // 0x1EC
    virtual void _v1F0() = 0; // 0x1F0
    virtual void _v1F4() = 0; // 0x1F4
    virtual void _v1F8() = 0; // 0x1F8
    virtual void _v1FC() = 0; // 0x1FC
    virtual void _v200() = 0; // 0x200
    virtual void _v204() = 0; // 0x204
    virtual void _v208() = 0; // 0x208
    virtual void _v20C() = 0; // 0x20C
    virtual void _v210() = 0; // 0x210
    virtual void _v214() = 0; // 0x214
    virtual void _v218() = 0; // 0x218
    virtual void _v21C() = 0; // 0x21C
    virtual void _v220() = 0; // 0x220
    virtual void _v224() = 0; // 0x224
    virtual void _v228() = 0; // 0x228
    virtual void _v22C() = 0; // 0x22C
    virtual void _v230() = 0; // 0x230
    virtual void _v234() = 0; // 0x234
    virtual void _v238() = 0; // 0x238
    virtual void _v23C() = 0; // 0x23C
    virtual void _v240() = 0; // 0x240
    virtual void _v244() = 0; // 0x244
    virtual void _v248() = 0; // 0x248
    virtual void _v24C() = 0; // 0x24C
    virtual void _v250() = 0; // 0x250
    virtual void _v254() = 0; // 0x254
    virtual void _v258() = 0; // 0x258
    virtual void _v25C() = 0; // 0x25C
    virtual void _v260() = 0; // 0x260
    virtual void _v264() = 0; // 0x264
    virtual void _v268() = 0; // 0x268
    virtual void _v26C() = 0; // 0x26C
    virtual void _v270() = 0; // 0x270
    virtual void _v274() = 0; // 0x274
    virtual void _v278() = 0; // 0x278
    virtual void _v27C() = 0; // 0x27C
    virtual void _v280() = 0; // 0x280
    virtual void _v284() = 0; // 0x284
    virtual void _v288() = 0; // 0x288
    virtual void _v28C() = 0; // 0x28C
    virtual void _v290() = 0; // 0x290
    virtual void _v294() = 0; // 0x294
    virtual void _v298() = 0; // 0x298
    virtual void _v29C() = 0; // 0x29C
    virtual void _v2A0() = 0; // 0x2A0
    virtual void _v2A4() = 0; // 0x2A4
    virtual void _v2A8() = 0; // 0x2A8
    virtual void _v2AC() = 0; // 0x2AC
    virtual void _v2B0() = 0; // 0x2B0
    virtual void _v2B4() = 0; // 0x2B4
    virtual void _v2B8() = 0; // 0x2B8
    virtual void _v2BC() = 0; // 0x2BC
    virtual void _v2C0() = 0; // 0x2C0
    virtual void _v2C4() = 0; // 0x2C4
    virtual void targetFunc(u32 arg) = 0; // 0x2C8
};
