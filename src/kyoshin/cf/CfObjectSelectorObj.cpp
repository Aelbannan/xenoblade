// Auto-scaffolded catalog TU for kyoshin/cf/CfObjectSelectorObj
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"

// ---------------------------------------------------------------------
// Imports (retail symbol names). Plain global declarations: MWCC does not
// mangle global-scope names, so these emit the exact retail symbols.
// ---------------------------------------------------------------------
// Retail emits bare (unmangled-C) symbol names for these imports; keep
// C linkage so the relocs reference the exact retail names.
extern "C" void __ct__cf_CfObjEnumList(u8* self);
extern "C" u8* getHandleMEM2__Q23mtl10MemManagerFv();
extern "C" u8* allocate__Q23mtl10MemManagerFUlUl(u32 size, u8* heap);
extern "C" void __dl__FPv(void* p);
extern "C" void __dla__FPv(void* p);

extern u8 lbl_eu_8052585C[];   // reslist base vtable
extern u8 lbl_eu_8052BDD8[];   // selector inner vtable
extern const f32 lbl_eu_80666EF8;
extern f32 lbl_eu_80661DB0;

f32 lbl_eu_80663F10;
extern const f32 lbl_eu_8066A1F8;
extern const f32 lbl_eu_80666F0C;
extern const f32 lbl_eu_80666F08;

namespace cf {

// Retail instance-data layout for CfObjectSelectorObj.
// Retail accessors address fields relative to a flat 0x0 base (e.g.
// stw r4, 0x608c(r3)), but the class header declares a vtable plus
// oversized padding, which shifts the member offsets away from retail.
// Cast `this` to this layout to reproduce the retail offsets exactly.
// (Kept in the .cpp: the class header is outside this session's scope.)
struct CfSelectorLayout {
    u8 _pad_00[0x608C];
    u32 field_0x608C;
    u8 _pad_6090[0x6094 - 0x6090];
    u32 field_0x6094;
    u32 field_0x6098;
    u8 _pad_609C[0x66C4 - 0x609C];
    u32 field_0x66C4;
    u8 _pad_66C8[0x90D4 - 0x66C8];
    u32 field_0x90D4;
    u8 _pad_90D8[0x90E4 - 0x90D8];
    u32 field_0x90E4; // in-flight token / busy flag (0 = idle)
    u32 field_0x90E8;
    u32 field_0x90EC;
    u8 _pad_90F0[0x90F8 - 0x90F0];
    f32 field_0x90F8;
    u8 _pad_90FC[0x9108 - 0x90FC];
    u32 field_0x9108; // flags; bit0 = result pending
    u8 _pad_910C[0x9730 - 0x910C];
    u32 field_0x9730;
    u8 _pad_9734[0xC140 - 0x9734];
    u32 field_0xC140;
    u8 _pad_C144[0xC150 - 0xC144];
    u32 field_0xC150;
    u8 _pad_C154[0xC164 - 0xC154];
    f32 field_0xC164;
    u8 _pad_C168[0xC174 - 0xC168];
    u32 field_0xC174; // flags; bit0 = clear pending
    struct CfSelectorTarget {
        u8 _pad_00[0x3068];
        u32 field_0x3068; // flags; bits 2/3 toggled by func_800FE920/938
    };
    CfSelectorTarget* field_0xC178;
    u32 field_0xC17C;
    u32 field_0xC180; // flags; low byte + bits 20-23
};

// ---------------------------------------------------------------------
// Resource-list containers embedded in the selector object.
//
// Retail CfObjEnumList embeds a reslist<CfObject*> at offset 0
// (vtable lbl_eu_8052585C, head pointer at +4, backing buffer at +0x14,
// capacity at +0x18, owned-flag at +0x1C). The destructor chains of
// CfObjEnumList / reslist / _reslist_base are inlined at the delete and
// clear sites; these classes reproduce that hierarchy so MWCC emits the
// same nested null-guard chains.
// ---------------------------------------------------------------------

// Doubly-linked list node (mNext@0, mPrev@4).
struct CfSelListNode {
    CfSelListNode* next;   // 0x00
    CfSelListNode* prev;   // 0x04
};

// _reslist_base<CfObject*>-equivalent (0x20 bytes).
class CfSelectorResList {
public:
    u32 vtable;              // 0x00 (written as lbl_eu_8052585C by the dtor)
    CfSelListNode* head;     // 0x04
    u8 _pad08[0x14 - 0x08];  // 0x08-0x13
    u8* buf;                 // 0x14
    u32 count;               // 0x18
    u8 flag;                 // 0x1C
    u8 _pad1D[0x20 - 0x1D];  // 0x1D-0x1F

    ~CfSelectorResList() {
        if (this == NULL) return;
        this->vtable = (u32)lbl_eu_8052585C;
        CfSelListNode* cur = this->head->next;
        while (cur != this->head) {
            CfSelListNode* old = cur;
            cur = cur->next;
            old->next = NULL;
        }
        this->head->next = this->head;
        this->head->prev = this->head;
        if (this->flag == 0 && this->buf != NULL) {
            __dla__FPv(this->buf);
            this->buf = NULL;
        }
    }
};

// CfObjEnumList-equivalent (0x3040 bytes): reslist base at offset 0.
class CfSelectorList : public CfSelectorResList {
public:
    u8 _pad20[0x3040 - 0x20];  // 0x20-0x303F

    ~CfSelectorList() {
        if (this == NULL) return;
    }

    // Retail reslist::destroyList: clear + free + reset capacity.
    void destroyList() {
        CfSelListNode* cur = this->head;
        cur = cur->next;
        while (cur != this->head) {
            CfSelListNode* old = cur;
            cur = cur->next;
            old->next = NULL;
        }
        this->head->next = this->head;
        this->head->prev = this->head;
        if (this->flag == 0 && this->buf != NULL) {
            __dla__FPv(this->buf);
            this->buf = NULL;
        }
        this->count = 0;
    }
};

// Inner selector object (0x3044): vtable at 0x00, CfObjEnumList at 0x04.
class CfSelectorInner {
public:
    u32 vtable;              // 0x00 (lbl_eu_8052BDD8)
    CfSelectorList mList;    // 0x04

    ~CfSelectorInner() {
        if (this == NULL) return;
    }
};

// Full selector data block (retail size 0xC188).
class CfObjectSelectorData {
public:
    CfSelectorList mList1;       // 0x0000
    CfSelectorList mList2;       // 0x3040
    u32 field6080;               // 0x6080
    u8 _pad6084[0x608C - 0x6084]; // 0x6084-0x608B
    u32 field608C;               // 0x608C
    u8 _pad6090[0x6094 - 0x6090]; // 0x6090-0x6093
    u32 field6094;               // 0x6094
    u32 field6098;               // 0x6098
    u8 _pad609C[0x60A0 - 0x609C]; // 0x609C-0x609F
    CfSelectorInner mInner1;     // 0x60A0
    u32 field90E4;               // 0x90E4
    u32 field90E8;               // 0x90E8
    u32 field90EC;               // 0x90EC
    u32 field90F0;               // 0x90F0
    u32 field90F4;               // 0x90F4
    f32 field90F8;               // 0x90F8
    f32 field90FC;               // 0x90FC
    f32 field9100;               // 0x9100
    u32 field9104;               // 0x9104
    u32 field9108;               // 0x9108
    CfSelectorInner mInner2;     // 0x910C
    u32 fieldC150;               // 0xC150
    u32 fieldC154;               // 0xC154
    u32 fieldC158;               // 0xC158
    u32 fieldC15C;               // 0xC15C
    u32 fieldC160;               // 0xC160
    f32 fieldC164;               // 0xC164
    f32 fieldC168;               // 0xC168
    f32 fieldC16C;               // 0xC16C
    u32 fieldC170;               // 0xC170
    u32 fieldC174;               // 0xC174
    u8* fieldC178;               // 0xC178
    u32 fieldC17C;               // 0xC17C
    u32 fieldC180;               // 0xC180
    u32 fieldC184;               // 0xC184

    ~CfObjectSelectorData() {}
};

// Compile-only stub; retail exports the bare (C-linkage) symbol name.
extern "C" void func_800FD774(cf::CfSelectorInner* list);
void CfObjectSelectorObj::func_800FE694(float val) {
    CfSelectorLayout* self = reinterpret_cast<CfSelectorLayout*>(this);
    self->field_0x90F8 = val;
    self->field_0xC164 = val;
}

unsigned long CfObjectSelectorObj::func_800FE910() {
    CfSelectorLayout* self = reinterpret_cast<CfSelectorLayout*>(this);
    return (self->field_0xC180 >> 10) & 1;
}

void CfObjectSelectorObj::func_800FE920() {
    CfSelectorLayout* self = reinterpret_cast<CfSelectorLayout*>(this);
    self->field_0xC178->field_0x3068 |= 4;
}

void CfObjectSelectorObj::func_800FE938() {
    CfSelectorLayout* self = reinterpret_cast<CfSelectorLayout*>(this);
    self->field_0xC178->field_0x3068 |= 8;
}

void CfObjectSelectorObj::func_800FE950(unsigned int a, unsigned int b, unsigned int c) {
    CfSelectorLayout* self = reinterpret_cast<CfSelectorLayout*>(this);
    self->field_0x608C = a;
    self->field_0x6094 = b;
    self->field_0x6098 = c;
    self->field_0x90E8 = a;
    self->field_0x90EC = b;
}

}

cf::CfObjectSelectorData* lbl_eu_80663F14;

cf::CfObjectSelectorObj* func_800FE68C() {
    return (cf::CfObjectSelectorObj*)lbl_eu_80663F14;
}

// Common selector-request sequence: point 0xC178 at the primary inner list,
// mark the request active (0xC180 low byte = 2), carry over the adjusted
// handle, flag 0x9108 as pending, run the list walk, report result arrival.
bool func_800FE6A4(cf::CfObjectSelectorObj* obj, unsigned int a, unsigned int b, unsigned int c) {
    cf::CfObjectSelectorData* self = reinterpret_cast<cf::CfObjectSelectorData*>(obj);
    u32 handle = self->fieldC17C;
    self->fieldC178 = (u8*)&self->mInner1;
    self->fieldC180 = (self->fieldC180 & 0xFFFFFF00) | 2;
    // 0xC17C holds a rebased handle; adjust it back into the real address space.
    if (handle != 0) {
        handle = handle + 0x3E9C;
    }
    self->field90F4 = handle;
    self->field9108 = (self->field9108 | 1) & 0xFFFFFFF3;
    self->field90E4 = c;
    self->field90E8 = a;
    self->field90EC = b;
    func_800FD774(&self->mInner1);
    return self->field90E4 != 0;
}

// Re-issues the current filter values (0x608C/0x6094/0x6098) as a request.
bool func_800FE738(cf::CfObjectSelectorObj* obj, unsigned int) {
    cf::CfObjectSelectorData* self = reinterpret_cast<cf::CfObjectSelectorData*>(obj);
    u32 sel = self->fieldC180;
    u32 handle = self->fieldC17C;
    self->fieldC178 = (u8*)&self->mInner1;
    self->fieldC180 = (sel & 0xFFFFFF00) | 2;
    u32 val6098 = self->field6098;
    u32 val6094 = self->field6094;
    u32 val608C = self->field608C;
    if (handle != 0) {
        handle = handle + 0x3E9C;
    }
    self->field90F4 = handle;
    self->field9108 = (self->field9108 | 1) & 0xFFFFFFF3;
    self->field90E4 = val6098;
    self->field90E8 = val608C;
    self->field90EC = val6094;
    func_800FD774(&self->mInner1);
    return self->field90E4 != 0;
}

u32 func_800FE7D8(cf::CfObjectSelectorObj* obj) {
    cf::CfSelectorLayout* self = reinterpret_cast<cf::CfSelectorLayout*>(obj);
    if (self->field_0x90E4 == 0) return 0;

    u32 flags = self->field_0xC174;
    self->field_0xC178 = NULL;
    // Retail keeps only the top byte of 0xC180, sets bit0, then clears bits
    // 8-11 (two statements: MWCC forwards the second store into the first).
    self->field_0xC180 = (self->field_0xC180 & 0xFFFFFF00) | 1;
    self->field_0xC180 &= ~0xF00;
    if (flags & 1) {
        self->field_0xC174 = 0;
        self->field_0xC150 = 0;
        self->field_0x9730 = 0;
        self->field_0xC140 = 0;
    }

    u32 result;
    if (!(self->field_0x9108 & 1)) {
        result = 0;
    } else {
        result = self->field_0x90E4;
        self->field_0x9108 = 0;
        self->field_0x90E4 = 0;
        self->field_0x66C4 = 0;
        self->field_0x90D4 = 0;
    }
    return result;
}

// Stores the new third filter into 0x6098 before re-issuing the request.
bool func_800FE96C(cf::CfObjectSelectorObj* obj, unsigned int c) {
    cf::CfObjectSelectorData* self = reinterpret_cast<cf::CfObjectSelectorData*>(obj);
    u32 sel = self->fieldC180;
    u32 handle = self->fieldC17C;
    u32 val6094 = self->field6094;
    u32 val608C = self->field608C;
    self->field6098 = c;
    self->field90E8 = val608C;
    self->field90EC = val6094;
    self->fieldC180 = (sel & 0xFFFFFF00) | 2;
    self->fieldC178 = (u8*)&self->mInner1;
    if (handle != 0) {
        handle = handle + 0x3E9C;
    }
    self->field90F4 = handle;
    self->field9108 = (self->field9108 | 1) & 0xFFFFFFF3;
    self->field90E4 = c;
    func_800FD774(&self->mInner1);
    return self->field90E4 != 0;
}

void sinit_800FEA14() {
    // lbl_eu_80663F10 = lbl_eu_80666F08 * (lbl_eu_8066A1F8 / lbl_eu_80666F0C);
    lbl_eu_80663F10 = lbl_eu_80666F08 * (lbl_eu_8066A1F8 / lbl_eu_80666F0C);
}

extern "C" void func_800FD774(cf::CfSelectorInner* list) {}

// ---------------------------------------------------------------------
// Target: func_800FDE4C (retail 0x800FE934, size 0xAC).
// Re-initialises the selector state when the singleton exists.
// ---------------------------------------------------------------------
void func_800FDE4C(cf::CfObjectSelectorData* obj, u32 a4, u32 a5) {
    if (lbl_eu_80663F14 == NULL) return;

    u32 zero = 0;
    u32 v608C = 0x80000004;
    u32 v6094 = 0x4002;
    obj->field90E4 = zero;
    obj->field90E8 = zero;
    obj->field90EC = zero;
    obj->field90F0 = a5;
    obj->field90F8 = lbl_eu_80666EF8;
    obj->field9108 = zero;
    obj->fieldC150 = zero;
    obj->fieldC154 = zero;
    obj->fieldC158 = zero;
    obj->fieldC15C = a5;
    obj->fieldC164 = lbl_eu_80666EF8;
    obj->fieldC174 = zero;
    obj->field90FC = lbl_eu_80661DB0;
    obj->fieldC168 = lbl_eu_80661DB0;
    obj->field9100 = lbl_eu_80663F10;
    obj->fieldC16C = lbl_eu_80663F10;
    obj->field9104 = a4;
    obj->fieldC170 = a4;
    obj->fieldC184 = a5;
    obj->fieldC178 = NULL;
    obj->fieldC17C = a4;
    obj->field6080 = zero;
    obj->fieldC180 = obj->field6080 | 1;
    obj->field608C = v608C;
    obj->field6094 = v6094;
    obj->field6098 = zero;
    obj->field90E8 = v608C;
    obj->field90EC = v6094;
}

// ---------------------------------------------------------------------
// Target: __ct__800FDB4C (retail 0x800FE634, size 0xD0).
// Singleton factory: allocate the 0xC188 data block, construct the four
// resource lists (two top-level, one inside each inner selector), then
// reset the scalar state fields.
// ---------------------------------------------------------------------
void __ct__800FDB4C() {
    if (lbl_eu_80663F14 == NULL) {
        cf::CfObjectSelectorData* obj = (cf::CfObjectSelectorData*)allocate__Q23mtl10MemManagerFUlUl(0xC188, getHandleMEM2__Q23mtl10MemManagerFv());
        if (obj != NULL) {
            __ct__cf_CfObjEnumList((u8*)&obj->mList1);
            __ct__cf_CfObjEnumList((u8*)&obj->mList2);
            u32 vt = (u32)lbl_eu_8052BDD8;
            obj->mInner1.vtable = vt;
            __ct__cf_CfObjEnumList((u8*)&obj->mInner1.mList);
            u32 zero = 0;
            obj->field90E4 = zero;
            obj->field90E8 = zero;
            obj->field90F0 = zero;
            obj->field90F4 = zero;
            obj->field90F8 = lbl_eu_80666EF8;
            obj->field9108 = zero;
            obj->mInner2.vtable = vt;
            __ct__cf_CfObjEnumList((u8*)&obj->mInner2.mList);
            obj->fieldC150 = zero;
            obj->fieldC154 = zero;
            obj->fieldC15C = zero;
            obj->fieldC160 = zero;
            obj->fieldC164 = lbl_eu_80666EF8;
            obj->fieldC174 = zero;
            obj->fieldC180 = zero;
        }
        lbl_eu_80663F14 = obj;
    }
}

// ---------------------------------------------------------------------
// Target: __dt__800FDC1C (retail 0x800FE704, size 0x230).
// Singleton destroy: deleting destructor of the data block (four member
// destructors inlined) then clear the singleton pointer.
// ---------------------------------------------------------------------
void __dt__800FDC1C() {
    if (lbl_eu_80663F14 != NULL) {
        delete (cf::CfObjectSelectorData*)lbl_eu_80663F14;
        lbl_eu_80663F14 = NULL;
    }
}

// ---------------------------------------------------------------------
// Target: __dt__800FDEF8 (retail 0x800FE9E0, size 0x20C).
// Non-deleting reset: re-initialise the scalar state and clear the four
// resource lists (walk + free + reset capacity) when the singleton exists.
// ---------------------------------------------------------------------
void __dt__800FDEF8(cf::CfObjectSelectorData* obj) {
    if (lbl_eu_80663F14 != NULL) {
        obj->field90E4 = 0;
        obj->field90E8 = 0;
        obj->field90EC = 0;
        obj->field90F0 = 0;
        obj->field90F4 = 0;
        obj->field90F8 = lbl_eu_80666EF8;
        obj->field9108 = 0;
        obj->mInner1.mList.destroyList();
        obj->fieldC150 = 0;
        obj->fieldC154 = 0;
        obj->fieldC158 = 0;
        obj->fieldC15C = 0;
        obj->fieldC160 = 0;
        obj->fieldC164 = lbl_eu_80666EF8;
        obj->fieldC174 = 0;
        obj->mInner2.mList.destroyList();
        obj->mList1.destroyList();
        obj->mList2.destroyList();
        obj->fieldC178 = NULL;
        obj->fieldC17C = 0;
        obj->fieldC180 = 1;
    }
}

// ---------------------------------------------------------------------
// Target: func_800FE860 (retail 0x800FF348, size 0xB0).
// Toggles the 0x40 "selected" bits on both selector-target states and the
// 0x100 "active" bit, but only when the request direction matches the
// current activation state.
// ---------------------------------------------------------------------
void func_800FE860(cf::CfObjectSelectorData* obj, u32 arg4) {
    // Proceed only when the request direction differs from the active state.
    u32 flag = obj->fieldC180 & 0x100;
    if (flag == 0 && arg4 == 0) return;
    if (flag != 0 && arg4 != 0) return;
    if (arg4 != 0) {
        obj->field9108 |= 0x40;
    } else {
        obj->field9108 &= ~0x40;
    }
    if (arg4 != 0) {
        obj->fieldC174 |= 0x40;
    } else {
        obj->fieldC174 &= ~0x40;
    }
    if (arg4 == 0) {
        obj->fieldC180 &= ~0x100;
    } else {
        obj->fieldC180 |= 0x100;
    }
}

extern "C" void func_800FE104() {}
