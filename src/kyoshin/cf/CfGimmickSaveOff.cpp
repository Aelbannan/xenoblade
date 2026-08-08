// CfGimmickSaveOff — save point gimmick

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfGimmickSaveOff.hpp"

extern u8 lbl_eu_805357E8[];
extern u8 lbl_eu_80510B28[];
extern u8 lbl_eu_805765A0[];

// Destructor provided by extern "C" function to match retail ABI.
// Class header declares virtual ~CfGimmickSaveOff(); for vtable entry.

extern "C" void __ct__cf_CfGimmickSaveOff(void* self, s32 param) {
    extern u8 __vt__Q22cf16CfGimmickSaveOff[];

    __ct__cf_CfGimmick(self);
    *(void**)self = (void*)__vt__Q22cf16CfGimmickSaveOff;
    *(u16*)((u8*)self + 0x82) = 8;

    void* alloc = func_8003AA34();
    u32 localVar = 0;

    *(u16*)((u8*)self + 0x64) = param;

    func_80208F34(self, (u8*)self + 4, alloc, &localVar);
    func_80209020(self, (u8*)self + 0x1C, alloc, &localVar);
    func_80209288(self, (u8*)self + 0x10, alloc, &localVar);

    void* filePtr = *(void**)&localVar;

    const char* s1 = getBdatStringColumnValue(filePtr, *(const char**)(lbl_eu_805357E8 + 0x34), param);
    *(u16*)((u8*)self + 0x6C) = *(u16*)&s1;

    const char* s2 = getBdatStringColumnValue(filePtr, *(const char**)(lbl_eu_805357E8 + 0x38), param);
    *(u16*)((u8*)self + 0x6E) = *(u16*)&s2;

    const char* s3 = getBdatStringColumnValue(filePtr, (const char*)lbl_eu_80510B28, param);
    *(u32*)((u8*)self + 0x84) = *(u8*)s3;

    void** vtable = *(void***)self;
    void (*vfunc)(void*) = (void (*)(void*))vtable[0x20 / 4];
    vfunc(self);
}

extern "C" void* __dt__Q22cf16CfGimmickSaveOffFv(void* self, int flag) {
    if (self != NULL) {
        __dt__Q22cf9CfGimmickFv(self, 0);
        if (flag > 0) {
            operator delete(self);
        }
    }
    return self;
}

extern "C" void func_802ABCB4(void* self) {
    u16 maxVal = *(u16*)((u8*)self + 0x6E);
    u16 minVal = *(u16*)((u8*)self + 0x6C);

    if (maxVal != 0) {
        u32 count = func_800822F4__Q22cf13CfGameManagerFv();
        if (minVal > count || count > maxVal) {
            return;
        }
    }

    u32 state = *(u32*)((u8*)self + 0x60);
    void* self1C = (u8*)self + 0x1C;
    void* self04 = (u8*)self + 0x04;

    // State dispatch via jumptable_eu_80535830 (5 case handlers); a zero
    // handler result returns early (retail checks the dispatched call's r3).
    int dispatchResult = jumptable_eu_80535830[state](self1C, (void*)lbl_eu_805765A0, self04);
    if (dispatchResult == 0) {
        return;
    }

    if (*(u32*)((u8*)self + 0x84) == 2) {
        void* list = func_800B6BC8();
        void* listEnd = *(void**)((u8*)list + 4);
        void* entry = *(void**)list;
        while (entry != listEnd) {
            void* obj = *(void**)((u8*)entry + 8);
            void** objVtable = *(void***)obj;
            void* result = (*(void* (*)(void*))(objVtable[0x110 / 4]))(obj);
            if (result != NULL) {
                func_8008B95C((u8*)result + 0x84);
            }
            entry = *(void**)entry;
        }
    } else if (*(u32*)((u8*)self + 0x84) == 1) {
        func_8020A03C();
    } else {
        func_8020A0CC();
    }
}
