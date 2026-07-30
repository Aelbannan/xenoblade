#pragma once

#include <types.h>
#include "monolib/work/CWorkThread.hpp"
#include <nw4r/lyt/lyt_arcResourceAccessor.h>

class CLibLayout : public CWorkThread {
public:
    CLibLayout(const char* pName, CWorkThread* pParent);
    virtual ~CLibLayout();

    DECL_WORKTHREAD_CREATE(CLibLayout);

    static bool isInitialized();
    static CLibLayout* getInstance();
    static nw4r::lyt::ArcResourceAccessor* createArcResourceAccessor();
    void getAllocHandle();
    void createLayout();
    void createPicture();
    void createTextbox();
    void deleteTextboxOrPicture();
    void func_8045F438();
    void func_8045F4E4();

    virtual void wkUpdate() override;  //0x88
    virtual bool wkStandbyLogin();
    virtual bool wkStandbyLogout();

    //0x0: vtable
    //0x0-1c4: CWorkThread
    u32 unk1C4;                       // 0x1C4: unknown field
    u8 pad_1C8[0x58];                 // 0x1C8-0x21F
    void** hashTable;                  // 0x220-0x223: hash table for Class_8045F858
    s32 hashAccum;                     // 0x224-0x227
    s32 hashCount;                     // 0x228-0x22B
    s32 hashDivisor;                   // 0x22C-0x22F
    u8 pad_230[0x8];                   // 0x230-0x237
    void** instanceArray;              // 0x238-0x23B: tracking array for UnkClass_8045F564
    u8 pad_23C[0x7C];                  // 0x23C-0x2B7
    u32 instanceCount;                 // 0x2B8-0x2BB
    u8 pad_2BC[0x4];                   // 0x2BC-0x2BF
};
