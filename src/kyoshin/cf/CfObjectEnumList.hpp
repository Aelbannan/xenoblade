#pragma once

#include <types.h>

namespace cf {

class CfObjEnumList {
public:
    struct sObjInfo {
        u32 objectId;
        void* object;
        u32 field_08;
        u32 field_0C;
        u32 field_10;
        float field_14;
        bool field_18;
    };

    CfObjEnumList();
    ~CfObjEnumList();

    void* func_800F6E98(int index);
    void* func_800F6EAC(unsigned long idx);
    void* func_800F6EC0(int index);

    // reslist base: vtable pointer (0x00), sentinel, backing storage, and
    // ownership flag. The methods below are plain (direct-call) methods, but
    // the OBJECT carries a reslist vtable pointer at +0x00, so the array and
    // count sit at +0x20 / +0x620 (retail __ct__cf_CfObjEnumList stw r7,0x620).
    void* m_vtable;                // 0x00
    u8 _pad_04[0x20 - 0x04];       // 0x04..0x20
    sObjInfo* mPtrArray[384];      // 0x0020
    u32 mPtrCount;             // 0x0620
    void* mSortVtableA;        // 0x0624
    void* mSortVtableB;        // 0x0628
    u8 _pad_062C[4];           // 0x062C
    sObjInfo mObjInfo[384];    // 0x0630
    u32 mObjInfoCount;         // 0x3030
    u32 field_3034;
    u32 field_3038;
    u32 field_303C;
};

} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_800F4798(void* self);
