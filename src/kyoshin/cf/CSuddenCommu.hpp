#pragma once

#include <types.h>
#include "kyoshin/cf/object/IObjectInfo.hpp"

// Global-scope fwd decl (cue object defined below); used by the CSuddenCommu
// field_20 volatile-view union.
struct CSuddenCommuVoiceCue;

namespace cf{
    //size: 0x30?
    // novtable: the retail vtable is a split-data object (lbl_eu_805339C0);
    // suppressing MWCC's own __vt__ keeps the ctor reloc name byte-identical.
    class __declspec(novtable) CSuddenCommu : public IObjectInfo {
    public:
        CSuddenCommu();
        virtual ~CSuddenCommu(){}
        virtual void IObjectInfo_UnkVirtualFunc1();

        //0x0: vtable
        //0x0-4: IObjectInfo?
        /* 0x04 */ s16 field_4;
        /* 0x06 */ union { struct { s16 field_6; s16 field_8; }; s16 pair_6[2]; };
        /* 0x0A */ s16 field_A;
        /* 0x0C */ s16 field_C;
        /* 0x0E */ s16 field_E;
        /* 0x10 */ volatile u32 field_10; // voice id probed via CCharVoiceMan_IsNodeActive
        /* 0x14 */ u32 field_14;
        /* 0x18 */ volatile float field_18;
        /* 0x1C */ float field_1C;
        /* 0x20 */ union {
            u32 field_20;
            // Typed-pointer view used by SuddenCommuEnterBattle: accessing the cue
            // through the real CSuddenCommuVoiceCue* type lets MWCC's alias
            // analysis prove the field_B0 store cannot touch the sdata2
            // float pool, which reproduces the retail issue order.
            ::CSuddenCommuVoiceCue* field_20p;
        };
        /* 0x24 */ volatile u32 field_24;
        /* 0x28 */ u32 unk28;
        /* 0x2C */ float unk2C;

        // Clears the pairing timer (shared by CBattleManager reset paths).
        void resetPairTimer(){
            unk28 = 0;
            unk2C = 0.0f;
        }
    };
}

struct CSuddenCommuActor;

void SuddenCommuDriveFrame(cf::CSuddenCommu* self);
unsigned long SuddenCommuIsStateActive(void* self);
void SuddenCommuDispatchPartner(cf::CSuddenCommu* self, CSuddenCommuActor* attacker, CSuddenCommuActor* target);


// Thin owner-API header for kyoshin/cf/CSuddenCommu (declarations mirror the
// canonical copies in CSuddenCommu.hpp).
namespace cf { class CSuddenCommu; }
struct CSuddenCommuActor;

void SuddenCommuDriveFrame(cf::CSuddenCommu* self);
unsigned long SuddenCommuIsStateActive(void* self);
void SuddenCommuDispatchPartner(cf::CSuddenCommu* self, CSuddenCommuActor* attacker, CSuddenCommuActor* target);
