#pragma once

#include <types.h>
#include "kyoshin/cf/object/IObjectInfo.hpp"

namespace cf {
    //size: 0xC
    class CChainEffect : public IObjectInfo {
    public:
        CChainEffect();
        virtual ~CChainEffect();
        virtual void IObjectInfo_UnkVirtualFunc1();

        //0x0: vtable
        //0x0-4: IObjectInfo
        u32 unk4;
        u32 unk8;
    };
}

extern "C" void chainBindEffectLink(cf::CChainEffect*, int a, int b, int c, int d, int e);

// ---------------------------------------------------------------------------
// Effect linkage / registry shapes (global scope members of this TU).
// ---------------------------------------------------------------------------

// Effect linkage object returned by bindIndexedEffect (owns the caller effect).
struct CChainObj {
    u8 pad_00[0x98];
    u32 field_98;                 //0x98
    u8 pad_9C[0x14];              //0x9C
    cf::CChainEffect* field_b0;   //0xB0
};

// Singleton chain manager returned by getReslistBE8.
struct CChainManager {
    u32 field_00;                        //0x0
    struct CChainNode* field_04;         //0x4: head of circular node list
};

// Node in the manager's circular list.
struct CChainNode {
    struct CChainNode* field_00;         //0x0: next
    u32 field_04;                        //0x4
    void* field_08;                      //0x8: object pointer
};

// Object produced by CollObjHasFlag64Bit5 (examined in chainUnbindMatchingObjects).
struct CChainItem {
    u8 pad_00[0x8C];
    u16 field_8C;                 //0x8C
    u8 pad_8E[0xE];               //0x8E
    u32 field_9C;                 //0x9C
};
