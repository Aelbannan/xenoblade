#pragma once

#include <types.h>

namespace cf {
    //min size: 0x10
    class CObjectState {
    public:
        virtual void setStateBitMask(u32 bits);  //0x8
        virtual int checkStateFlags(int mask);  //0xC
        virtual void setStateBitFlag(u32 mask);  //0x10
        virtual void clearStateWord4();  //0x14
        virtual void applyStateFlags(int arg);  //0x18
        virtual void clearStateWord8();  //0x1C
        virtual void clearStateFlags8(int arg);  //0x20
        virtual int checkStateFlags8(int arg);  //0x24
        virtual int checkStateFlagsC(int mask);  //0x28
        // Two parameters: the retail body reads r5 and the ordinary callers
        // (pluginEve onTalk/onTalkEnd, CtrlNpc, ocUnit, CArtsInfo) set it. The
        // child-link sites dispatch through a scene-item interface whose 0x2C
        // slot takes one argument (see docs/MWCC_CASES.md "cf::CfObject slots
        // whose retail callers and callee disagree").
        virtual int setStateBitMask0(int mask, int flag); //0x2C
        virtual void* getStateData(); //0x30
        virtual void* setStateBitMask2(); //0x34
        virtual void setStateBitMask3(); //0x38

        // Wave-37 caller alias (non-virtual, inline): legacy UnkVirtualFunc8
        // spelling for the 0x24 state-word query. Same arity; forwards to
        // the renamed virtual so other TUs keep compiling unchanged.
        int CObjectState_UnkVirtualFunc8(int arg) { return checkStateFlags8(arg); }
        // Wave-63 mop-up aliases (non-virtual, inline): legacy UnkVirtualFunc4/6
        // spellings for the +0x14/+0x1C clear-word slots.
        void CObjectState_UnkVirtualFunc4() { clearStateWord4(); }
        void CObjectState_UnkVirtualFunc6() { clearStateWord8(); }
        // Legacy Unk spelling for the 0x28 +0xC-word query; Unk Fv body stays
        // for hand-built vtables. MWCC inlines into the virtual dispatch.


        //0x0: vtable
        u32 unk4;          // 0x04
        u32 unk8;          // 0x08
        u32 unkC;          // 0x0C
    };
}
