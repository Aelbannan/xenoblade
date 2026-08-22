// CfGimmickSaveOff - save point gimmick

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CfGimmickSaveOff.hpp"


// The retail ctor symbol __ct__cf_CfGimmickSaveOff is a C-linkage name (no
// C++ mangling markers), so it is emitted as an extern "C" free function
// rather than a C++ member constructor (same convention as __ct__cf_CfGimmick).
// The class carries a manual vptr at +0x00 (set to lbl_eu_8053A1E0).
extern "C" void* __ct__cf_CfGimmickSaveOff(cf::CfGimmickSaveOff* self, s32 param) {
    __ct__cf_CfGimmick(self);
    *(void**)self = (void*)lbl_eu_8053A1E0;
    self->mFlag = 8;

    void* mgr = func_8003AA34();
    void* holder = lbl_eu_80664140;
    self->mParam = (u16)param;

    func_80208F34(self, (u8*)self + 4, mgr, &holder);
    func_80209020(self, (u8*)self + 0x1C, mgr, &holder);
    func_80209288(self, (u8*)self + 0x10, mgr, &holder);

    // Read three bdat string cells (the cells are string pointers); lower
    // 16 bits go into the u16 bounds, the low byte into the type.
    u32 vMin = getBdatStringColumnValue(holder, *(const char**)(lbl_eu_805357E8 + 0x34), param);
    self->mMinVal = *(u16*)&vMin;
    u32 vMax = getBdatStringColumnValue(holder, *(const char**)(lbl_eu_805357E8 + 0x38), param);
    self->mMaxVal = *(u16*)&vMax;
    u32 vType = getBdatStringColumnValue(holder, (const char*)lbl_eu_80510B28, param);
    self->mType = *(u8*)&vType;

    // Invoke the derived vtable's init hook (slot 0x20: a real virtual call).
    self->vinit();
    return self;
}

// Complete-object destructor: MWCC auto-generates the this!=0 guard and the
// delete-flag wrapper around this body (same convention as CfGimmickItem).
cf::CfGimmickSaveOff::~CfGimmickSaveOff() {
    __dt__Q22cf9CfGimmickFv((void*)this, 0);
}

// Opaque interface whose vtable slot 0x44 (offset 0x110) is the getter the
// retail loop calls on each list node's object. Declared-only (pure virtual),
// so nothing is emitted; it exists to make MWCC use its canonical virtual-call
// sequence (lwz r12,0(r3); lwz r12,0x110(r12)).
struct IUnkVt110 {
    virtual void u00() = 0;
    virtual void u01() = 0;
    virtual void u02() = 0;
    virtual void u03() = 0;
    virtual void u04() = 0;
    virtual void u05() = 0;
    virtual void u06() = 0;
    virtual void u07() = 0;
    virtual void u08() = 0;
    virtual void u09() = 0;
    virtual void u10() = 0;
    virtual void u11() = 0;
    virtual void u12() = 0;
    virtual void u13() = 0;
    virtual void u14() = 0;
    virtual void u15() = 0;
    virtual void u16() = 0;
    virtual void u17() = 0;
    virtual void u18() = 0;
    virtual void u19() = 0;
    virtual void u20() = 0;
    virtual void u21() = 0;
    virtual void u22() = 0;
    virtual void u23() = 0;
    virtual void u24() = 0;
    virtual void u25() = 0;
    virtual void u26() = 0;
    virtual void u27() = 0;
    virtual void u28() = 0;
    virtual void u29() = 0;
    virtual void u30() = 0;
    virtual void u31() = 0;
    virtual void u32() = 0;
    virtual void u33() = 0;
    virtual void u34() = 0;
    virtual void u35() = 0;
    virtual void u36() = 0;
    virtual void u37() = 0;
    virtual void u38() = 0;
    virtual void u39() = 0;
    virtual void u40() = 0;
    virtual void u41() = 0;
    virtual void u42() = 0;
    virtual void u43() = 0;
    virtual void u44() = 0;
    virtual void u45() = 0;
    virtual void u46() = 0;
    virtual void u47() = 0;
    virtual void u48() = 0;
    virtual void u49() = 0;
    virtual void u50() = 0;
    virtual void u51() = 0;
    virtual void u52() = 0;
    virtual void u53() = 0;
    virtual void u54() = 0;
    virtual void u55() = 0;
    virtual void u56() = 0;
    virtual void u57() = 0;
    virtual void u58() = 0;
    virtual void u59() = 0;
    virtual void u60() = 0;
    virtual void u61() = 0;
    virtual void u62() = 0;
    virtual void u63() = 0;
    virtual void u64() = 0;
    virtual void u65() = 0;
    virtual void* getObjAt84() = 0; // vtable offset 0x110
};

// State machine tick: optional range gate on the game-manager counter,
// then dispatch through the state jump table, then act on mType.
extern "C" void func_802ABCB4(cf::CfGimmickSaveOff* self) {
    if (self->mMaxVal != 0) {
        u32 count = func_800822F4__Q22cf13CfGameManagerFv();
        if (self->mMinVal > count || count > self->mMaxVal) {
            return;
        }
    }

    // State dispatch via jumptable_eu_80535830 (5 case handlers); a zero
    // handler result returns early (retail checks the dispatched call's r3).
    int dispatchResult =
        jumptable_eu_80535830[self->mState]((cf::CfGimmick*)((u8*)self + 0x1C), &lbl_eu_805765A0, (const CfGimmickVec3*)((u8*)self + 0x04));
    if (dispatchResult == 0) {
        return;
    }

    if ((s32)self->mType == 2) {
        // Walk an intrusive linked list; the terminator is re-read from
        // list+4 every iteration (sentinel node). Each node's vtable slot
        // 0x44 (0x110/4) returns an object whose +0x84 gets poked.
        void* list = func_800B6BC8();
        void* tail = *(void**)((u8*)list + 4);
        void* entry = *(void**)tail;
        while (entry != *(void**)((u8*)list + 4)) {
            IUnkVt110* obj = *(IUnkVt110**)((u8*)entry + 8);
            void* result = obj->getObjAt84();
            if (result != NULL) {
                func_8008B95C((u8*)result + 0x84);
            }
            entry = *(void**)entry;
        }
    } else if ((s32)self->mType == 1) {
        func_8020A03C();
    } else {
        func_8020A0CC();
    }
}
