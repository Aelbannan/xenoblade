// Auto-scaffolded catalog TU for kyoshin/code_800B06A4
// Replace stubs with high-level C/C++ during decomp.

#include <types.h>
#include "kyoshin/code_800B06A4.hpp"
#include <string.h>
#include <math.h>
#include <new>

void UnkClass_800B0AD8::clearCounters() {
    unkB00 = 0;
    unkAFC = 0;
}
DECOMP_DONT_INLINE UnkClass_805764CC* func_800B07E8();
#pragma inline
u32 func_800AA2BC(u32 a, u32 b);
extern "C" void func_80193810(unsigned long a, void* b);
extern "C" void func_801F3CCC(unsigned long a, void* b);
extern "C" void func_801F45B4(unsigned long a, void* b);
extern "C" void func_802074F0(unsigned long a, void* b);
extern "C" void func_8019397C(unsigned long a, void* b);
extern "C" void func_80193D48(unsigned long a, void* b);
extern "C" void func_80195E5C(unsigned long a, float b);

// Forward declarations for callees used by my targets
struct CfMapMineManager;
extern void func_80206BD4(CfMapMineManager* self);
extern void func_800B44A0(UnkClass_805764CC* self, void* arg);
extern void* func_800B61FFC();
extern "C" void func_800B137C(void* self, unsigned long handle, unsigned long count);
extern void func_800B8524(void* singleton, void* self, void* other);
void* func_800B47A8(void* a, void* b, void* c, void* d, void* e, void* f, void* g);

void func_80081258(void* self);
void func_80081264(void* self, cf::CfObject* obj);

// Target 4: us-800b0f70 - Store float and its square to several globals.
// Retail emits frsp f2,f1 for `float sq` - the float->double->float round-trip
// (double d = a; float sq = (float)d;) keeps the explicit conversion; a plain
// `float sq = a` lets MWCC drop it.
extern "C" void func_800B06A4(float a) {
    extern float lbl_eu_80661CCC, lbl_eu_80661CD0;
    extern float lbl_eu_80663EC8, lbl_eu_80663ECC, lbl_eu_80663ED0, lbl_eu_80663ED4;
    double d = (double)a;
    float sq = (float)d;
    float c = lbl_eu_80661CD0;
    lbl_eu_80661CCC = a;
    sq = sq * sq;
    lbl_eu_80663EC8 = a;
    lbl_eu_80663ECC = sq;
    lbl_eu_80663ED0 = c;
    lbl_eu_80663ED4 = sq;
}

void init_0A90(void){}
// Target 1: us-800b1118 - func_800B084C
// Calls func_80061FFC() to get a handle, then passes it along with `count` to func_800B0894.
#pragma push
#pragma auto_inline off
void func_800B084C(UnkClass_805764CC* self, unsigned long count) {
    func_800B0894(self, func_80061FFC(), count);
}
#pragma pop

// Target 1: us-800b15d8 - reslist<cf::IFactoryEvent*>::reslist() constructor
// (flattened: base init with the _reslist_base vtable, then the derived
// vtable install overwrites +0). Array-typed externs force absolute lis/addi
// addressing; the base vtable store is volatile so MWCC keeps both stores.
// Local reslist view (mirror of the _reslist_base layout: vtable@0,
// mStartNodePtr@4, sentinel node@8 (mNext/mPrev/mItem, 0xC stride), mList@0x14,
// mCapacity@0x18, ownership flag@0x1C). The struct-typed sentinel access
// reproduces the retail scheduling (MWCC keeps li r0 first + the v2 lis/addi
// pair adjacent).
struct CfReslistLayout {
    void* mVtable;           // 0x00
    CfReslistNode* mStartNodePtr;  // 0x04
    CfReslistNode mStartNode;      // 0x08 (0xC bytes)
    void* mList;             // 0x14
    int mCapacity;           // 0x18
    bool field_0x1C;         // 0x1C
};

void __ct__reslist_cf_IFactoryEvent(void* self) {
    extern void* lbl_eu_805290B8[];
    extern void* lbl_eu_805290A0[];
    CfReslistLayout* obj = (CfReslistLayout*)self;
    *(volatile u32*)((u8*)self) = (u32)lbl_eu_805290B8;
    obj->mList = 0;
    obj->mCapacity = 0;
    obj->field_0x1C = false;
    obj->mStartNodePtr = &obj->mStartNode;
    obj->mStartNodePtr->mNext = &obj->mStartNode;
    obj->mStartNodePtr->mPrev = &obj->mStartNode;
    obj->mVtable = (void*)lbl_eu_805290A0;
}

// Target 1: us-800b1368 - reslist<cf::CfObject*>::reslist() constructor
// (flattened: base init with the _reslist_base vtable, then the derived
// vtable install overwrites +0). Array-typed externs force absolute lis/addi
// addressing instead of SDA21. The base vtable store is volatile so MWCC's
// dead-store elimination keeps it (the derived install provably overwrites
// +0, but retail emits both stores).
void __ct__reslist_cf_CfObject(void* self) {
    extern void* lbl_eu_8052585C[];
    extern void* lbl_eu_805290E8[];
    CfReslistLayout* obj = (CfReslistLayout*)self;
    *(volatile u32*)((u8*)self) = (u32)lbl_eu_8052585C;
    obj->mList = 0;
    obj->mCapacity = 0;
    obj->field_0x1C = false;
    obj->mStartNodePtr = &obj->mStartNode;
    obj->mStartNodePtr->mNext = &obj->mStartNode;
    obj->mStartNodePtr->mPrev = &obj->mStartNode;
    obj->mVtable = (void*)lbl_eu_805290E8;
}
// Target 3: us-800b186c - func_800B0FA0
#pragma push
#pragma auto_inline off
extern "C" DECOMP_DONT_INLINE void func_800B0FA0(UnkClass_805764CC* self) {
    if (func_800B0FEC(&self->field_0xC80) == 0) {
        func_800B0FF4(&self->field_0xC80, func_80061FFC(), 4);
    }
}
#pragma pop
#pragma inline

void init_0FA0(){}
u32 UnkClass_805764CC::get_u32_18(){return *(u32*)((u8*)this + 0x18);}
// func_800B0A90: zero the u32 at self (retail: li r0,0; stw r0,0x0(r3))
extern "C" void func_800B0A90(void* self) { *(u32*)self = 0; }
void init_dispatchTarget_1(){}
// func_800B1808: zero the global flag word (retail: li r0,0; stw r0,lbl_eu_80663EE0@sda21)
#pragma push
#pragma auto_inline off
extern "C" void func_800B1808() { extern u32 lbl_eu_80663EE0; lbl_eu_80663EE0 = 0; }
#pragma pop
void gflag_setBits(unsigned long flags){extern unsigned long lbl_eu_80663EE0;lbl_eu_80663EE0 |= flags;}
void FactoryEvent3__Q22cf13IFactoryEventFv(){}
void init_137C(){}
void node_copyNextPtr(void* dst, void* src){void* ptr = *(void**)((char*)src + 4); *(unsigned long*)dst = *(unsigned long*)ptr;}
void* node_getDataPtr(void* self){return (char*)(*(void**)self) + 8;}
void init_14E0(){}
void node_copyNextU32(void* dst, void* src){*(unsigned long*)dst = *(unsigned long*)((char*)src + 4);}
void init_14FC(){}
void __dt__800B151C();
// Retail callers (func_800B15A4) pass the list address in r3; the thunk
// forwards it to __dt__800B151C, so the parameter is part of the ABI even
// though the body ignores it.
#pragma push
#pragma auto_inline off
extern "C" void func_800B1518(void* self){void(*dtor)() = __dt__800B151C; dtor();}
#pragma pop
// Target 1: us-800b1de8 - reslist<cf::IFactoryEvent*> destructor (flattened).
// Unlinks every chained node (each node's next is nulled as it is walked),
// resets the sentinel links, frees the pool array when the list owns it
// (flag at 0x1C clear), and clears the entry count.
void __dt__800B151C(FactoryPoolList* self) {
    CfReslistNode* cur = self->mStartNodePtr->mNext;
    // Unlinks every chained node (each node's next is nulled as it is
    // walked). The sentinel pointer is re-read from self each iteration
    // (retail reloads 0x4(r3)) because the node store may alias it.
    while (cur != self->mStartNodePtr) {
        CfReslistNode* dead = cur;
        cur = dead->mNext;
        dead->mNext = 0;
    }
    self->mStartNodePtr->mNext = self->mStartNodePtr;
    self->mStartNodePtr->mPrev = self->mStartNodePtr;
    if (self->field_0x1C == 0) {
        if (self->mList != 0) {
            __dla__FPv(self->mList);
            self->mList = 0;
        }
    }
    self->mCapacity = 0;
}
void init_dispatchTarget_2(){}
extern u32 lbl_eu_80663F24;
void gvar_clearF24(){lbl_eu_80663F24 = 0;}
void FactoryEvent4__Q22cf13IFactoryEventFv(){}
void UnkClass_805764CC::set_u32_00(u32 val){*(u32*)((u8*)this + 0x0) = val;}
void copy_int_ptr(int* dst, int* src){*dst = *src;}
void init_182C(){}
#pragma push
#pragma auto_inline off
#pragma pop
// Target 2: us-800b2198 - second reslist<cf::IFactoryEvent*> destructor,
// identical body to __dt__800B151C over a different list instance.
// Declaration order (dead before cur, both outer-scope) gives retail's
// coloring: cur=r4, dead=r5, sentinel reloaded from self each iteration.
void __dt__800B18CC(FactoryPoolList* self) {
    CfReslistNode* cur = self->mStartNodePtr->mNext;
    while (cur != self->mStartNodePtr) {
        CfReslistNode* dead = cur;
        cur = dead->mNext;
        dead->mNext = 0;
    }
    CfReslistNode* sentinel = self->mStartNodePtr;
    sentinel->mNext = sentinel;
    sentinel = self->mStartNodePtr;
    sentinel->mPrev = sentinel;
    if (self->field_0x1C == 0) {
        if (self->mList != 0) {
            __dla__FPv(self->mList);
            self->mList = 0;
        }
    }
    self->mCapacity = 0;
}
void init_dispatchTarget_5(){}
// Target 1: func_800B93AC
extern "C" void func_800B93AC() {
    func_800B1B2C(func_800B07E8());
}

// Target 4: us-800b9df0 - func_800B94D4
void func_800B94D4(cf::CfObject* obj) {
    u8 stackBuf[0x28];
    func_80081258(stackBuf);
    func_80081264(stackBuf, obj);
    func_800B20B4(func_800B07E8(), 0x200, (const B20B4Payload*)stackBuf, 0);
}

// Target 2: us-800b23f8 - func_800B1B2C
// Reset pass over the singleton: copy the default float, then release each
// optional sub-object (each null-guarded) with its own teardown entry point.
extern "C" void func_8019380C(u32 obj);
extern "C" void func_80173338(u32 obj);
extern "C" void func_801F4504(u32 obj);
extern "C" void func_802B2A18(u32 obj);
extern "C" void func_80206388(u32 obj);
extern "C" int func_800B6508(void* self);
extern "C" void func_800B1A8C(void* iter, void* list);
extern "C" void* func_800B1A9C(void* iter);
extern "C" int func_800B64AC(void* p);
extern "C" int func_800B7680(void* self);
extern float lbl_eu_80663EDC;
#pragma push
#pragma auto_inline off
void func_800B1B2C(UnkClass_805764CC* self) {
    // const decl lets MWCC hoist the sdata2 load above the frame stores.
    extern const float lbl_eu_806669D8;
    lbl_eu_80663EDC = lbl_eu_806669D8;
    if (self->field_0xCA0 != 0) {
        func_8019380C(self->field_0xCA0);
    }
    if (self->field_0xCA4 != 0) {
        func_80173338(self->field_0xCA4);
    }
    if (self->field_0xCA8 != 0) {
        func_80173338(self->field_0xCA8);
    }
    if (self->field_0xCFC != 0) {
        func_801F4504(self->field_0xCFC);
    }
    if (self->field_0xD00 != 0) {
        func_802B2A18(self->field_0xD00);
    }
    if (self->field_0xCAC != 0) {
        func_80206388(self->field_0xCAC);
    }
}
#pragma pop

// Target 3: us-800b765c - func_800B6D3C
// Reset an object-list-backed reslist: bail (return 0) when the guard says
// the manager is busy; otherwise take the game manager's first item, and if
// it passes func_800B64AC, re-bind it via func_800B6DD0; otherwise fall back
// to restarting the iteration and returning the raw first item.
#pragma push
#pragma auto_inline off
void* func_800B6D3C(void* self) {
    if (func_800B6508(self) != 0) {
        return 0;
    }
    // Retail uses two separate iterator slots (first at sp+0xC, fallback at
    // sp+0x8) - a fresh iterator is started on the fallback path.
    F8C0IteratorNode it[2];
    func_8007F8C0__Q22cf13CfGameManagerFv(&it[1], (F8C0ListSource*)self);
    cf::CfObject* item = (cf::CfObject*)*func_8007F8D0__Q22cf13CfGameManagerFv(&it[1]);
    if (func_800B64AC(item) != 0) {
        return func_800B6DD0(self, item);
    }
    func_8007F8C0__Q22cf13CfGameManagerFv(&it[0], (F8C0ListSource*)self);
    return *func_8007F8D0__Q22cf13CfGameManagerFv(&it[0]);
}
#pragma pop

// Target 4: us-800b7f08 - func_800B75EC
// When the TboxInfo reslist holds >= 0x14 entries, inspect the current
// entry: if its id resolves via func_800B708C, fire func_800B9404; either
// way step the iterator back and rebind the removed entry via func_800B73E8.
#pragma push
#pragma auto_inline off
void func_800B75EC() {
    UnkClass_805764CC* ctx = func_800B07E8();
    if ((u32)func_800B7680(&ctx->field_0xC48) < 0x14) {
        return;
    }
    // Scalar locals reproduce retail's stack layout: prevDst=sp+0x8,
    // outDst=sp+0xC, iter=sp+0x10. MWCC allocates scalars in reverse
    // declaration order.
    u32 iter;
    u32 outDst;
    u32 prevDst;
    func_800B1A8C(&iter, &ctx->field_0xC48);
    // Retail calls func_800B1A9C once per test.
    if (*(u32*)func_800B1A9C(&iter) != 0) {
        // Retail forwards the lookup result in r3 straight into func_800B9404.
        void* hit = func_800B708C__Fi(*(s32*)func_800B1A9C(&iter));
        if (hit != 0) {
            func_800B9404(hit);
        }
    }
    func_800B73E8(&outDst, &ctx->field_0xC48, func_800B1AC0(&prevDst, &iter));
}
#pragma pop

// Target 2: func_800B1A5C
extern "C" DECOMP_DONT_INLINE void func_800B1A5C(void* list) {
    u32 sentinel, cur, next, zero;
    u32* p;
    sentinel = *(u32*)((u32*)list + 1);
    cur = *(u32*)sentinel;
    zero = 0;
    goto check;
loop:
    p = (u32*)cur;
    cur = *p;
    *p = zero;
check:
    sentinel = *(u32*)((u32*)list + 1);
    if (cur != sentinel) goto loop;
    *(u32*)sentinel = sentinel;
}
#pragma inline

// Target: us-800b23c0 - func_800B1AF4: run the list/state init via
// func_800B72DC, then clear the 0x100 mask bit via func_800B4278.
extern "C" void* func_800B6CA0();
extern "C" void func_800B137C(void* self, unsigned long handle, unsigned long count);

// Game-manager/player-like object whose status callback is at vtable slot
// 0x1D (+0x74); used by func_800B1CDC.
class IDispB74 {
public:
    virtual void unk00();
    virtual void unk01();
    virtual void unk02();
    virtual void unk03();
    virtual void unk04();
    virtual void unk05();
    virtual void unk06();
    virtual void unk07();
    virtual void unk08();
    virtual void unk09();
    virtual void unk0A();
    virtual void unk0B();
    virtual void unk0C();
    virtual void unk0D();
    virtual void unk0E();
    virtual void unk0F();
    virtual void unk10();
    virtual void unk11();
    virtual void unk12();
    virtual void unk13();
    virtual void unk14();
    virtual void unk15();
    virtual void unk16();
    virtual void unk17();
    virtual void unk18();
    virtual void unk19();
    virtual void unk1A();
    virtual bool unk1D();   // vtable +0x74 (compiler pads 2 slots)
};

// Object created by func_800B957C: the final dispatch on the owner goes to
// vtable slot 0x1C (+0x70) with the freshly constructed child.
class IDispB970C {
public:
    virtual void unk00();
    virtual void unk01();
    virtual void unk02();
    virtual void unk03();
    virtual void unk04();
    virtual void unk05();
    virtual void unk06();
    virtual void unk07();
    virtual void unk08();
    virtual void unk09();
    virtual void unk0A();
    virtual void unk0B();
    virtual void unk0C();
    virtual void unk0D();
    virtual void unk0E();
    virtual void unk0F();
    virtual void unk10();
    virtual void unk11();
    virtual void unk12();
    virtual void unk13();
    virtual void unk14();
    virtual void unk15();
    virtual void unk16();
    virtual void unk17();
    virtual void unk18();
    virtual void unk19();
    virtual void unk1C(void* child);   // vtable +0x70 (compiler pads 2 slots)
};
// Target: us-800b7bfc - fetch the node list, run the teardown dtor, then
// pass (list, handle, 0x14) to the binder helper. func_80061FFC is declared
// via the TU-wide extern (see line 59 family).
extern "C" void func_800B72DC(void* self) {
    reslist<cf::TboxInfo>* obj = (reslist<cf::TboxInfo>*)func_800B6CA0();
    obj->~reslist();
    func_800B137C(obj, func_80061FFC(), 0x14);
}
extern "C" void func_800B4278(void* object, u32 mask);

extern "C" void func_800B1AF4(void* self) {
    func_800B72DC(self);
    func_800B4278(self, 256);
}

// Target: us-800b1bf8 - cf::CfValueItemManager ctor: base ctor call then
// derived vtable install (retail lbl_eu_805316D0); returns this (retail
// emits the mr r3, r31 return-this after the vtable load).
extern "C" void* __ct__cf_CfMapItemManager(void* self);
#pragma push
#pragma auto_inline off
extern "C" void* __ct__cf_CfValueItemManager(void* self) {
    extern void* lbl_eu_805316D0[];
    __ct__cf_CfMapItemManager(self);
    *(u32*)((u8*)self + 0) = (u32)lbl_eu_805316D0;
    return self;
}
#pragma pop

void init_1A8C(){}
void* node_getDataPtr_alt(void* self){return (char*)(*(void**)self) + 8;}
void init_1AA8(){}
void copy_int_ptr_alt(int* dst, int* src){*dst = *src;}
void node_copyNextU32_alt(void* dst, void* src){*(unsigned long*)dst = *(unsigned long*)((char*)src + 4);}
void init_1AD8(){}
void init_1AF4(){}
void init_dispatchTarget_6(){}
void init_1BBC(){}
// us-800b2488: if the flag bit-6 test is set, null the self arg, then call
// func_800B1C24(8, self-or-0) (declared in code_800B06A4.hpp).
extern "C" void func_800B1BBC(void* self) {
    if (func_800B1C00()) {
        self = 0;
    }
    func_800B1C24(8, self);
}
// func_800B1C00: bit 6 of the global flag word (retail: lwz r0,lbl_eu_80663EE0; extrwi r3,r0,1,25 = (x>>6)&1)
extern "C" DECOMP_DONT_INLINE u32 func_800B1C00(){ extern u32 lbl_eu_80663EE0; return (lbl_eu_80663EE0 >> 6) & 1; }
void init_1C0C(){}
void init_1C24(){}
#pragma push
#pragma auto_inline off
extern "C" s32 func_800B1C40() {
    if (func_800B1C00()) {
        return 0;
    }
    return func_800B1C0C(8);
}
#pragma pop
// us-800b2544: fetch the singleton, then bind the 0xCA4/0xCA8 sub-objects
// to `self` via func_80173C6C, each null-guarded.
void func_800B1C78(UnkClass_805764CC* self) {
    UnkClass_805764CC* obj = func_800B07E8();
    if (obj->field_0xCA4 != 0) {
        func_80173C6C((void*)obj->field_0xCA4, self);
    }
    if (obj->field_0xCA8 != 0) {
        func_80173C6C((void*)obj->field_0xCA8, self);
    }
}
void init_1CDC(){}
void init_1E18(){}
// field_0xCA0; if nonzero tail-call with (field, r4-passthrough) (retail lwz;cmpwi;beqlr;b)
extern "C" void func_800B1E18(UnkClass_805764CC* self, void* obj){if (self->field_0xCA0){func_8019397C(self->field_0xCA0, obj);}}
void init_1E2C(){}

// Target 4: us-800b26f8 - reset the singleton's FixStr scratch buffer, then
// either seed it with `name` and check it against the split1 string table
// (+0x35), or just re-clear it when no name was given.
extern "C" void func_800B1E2C(const char* name) {
    UnkClass_805764CC* obj = func_800B07E8();
    func_800B1B2C(obj);
    if (name != 0) {
        func_80068A30(&obj->field_0xCB0, name);
        func_800B1EB8(2);
        if (strcmp(name, lbl_eu_804FC4D8 + 0x35) == 0) {
            // Retail emits this call without materializing r3 (the argument
            // register keeps the strcmp result), so call through a no-arg
            // pointer type to suppress the argument move.
            ((void (*)())func_800B1EC8)();
        }
    } else {
        func_800B9A30(&obj->field_0xCB0);
        func_800B1368(2);
    }
}
void init_1EB8(){}
// us-800b2794: singleton teardown driver - reset the 0x2 event mask, and if
// clear, run the 0xB2C/0x1F2C release pair; then the 0x1F6C/0x205C/0x2034
// update sequence.
#pragma push
#pragma auto_inline off
extern "C" void func_800B1EC8(UnkClass_805764CC* self) {
    UnkClass_805764CC* obj = func_800B07E8();
    if (func_800B1C0C(2) == 0) {
        func_800B1B2C(obj);
        // Retail passes no second arg (r4 dead at callee); the 1-param
        // pointer type skips li r4,0 while the matched 2-arg definition
        // in this TU stays untouched.
        ((void (*)(UnkClass_805764CC*))func_800B1F2C)(obj);
    }
    func_800B1F6C(obj);
    func_800B205C(obj);
    ((void (*)(UnkClass_805764CC*))func_800B2034)(obj);
}
#pragma pop
void init_1F2C(){}
#pragma push
#pragma auto_inline off
extern "C" void func_800B1F2C(UnkClass_805764CC* self, void* obj){if (self->field_0xCA0){func_80193D48(self->field_0xCA0, obj);}}
#pragma pop
extern "C" void func_800B1F40(UnkClass_805764CC* self, void* obj){if (self->field_0xCA0){func_80193810(self->field_0xCA0, obj);}}

// Target 1: us-800b2820 - field_0xCA0; if nonzero tail-call func_80195E5C(field, const)
#pragma push
#pragma auto_inline off
extern "C" void func_800B1F54(UnkClass_805764CC* self) {
    if (self->field_0xCA0 == 0) return;
    extern float lbl_eu_80663EC8;
    func_80195E5C(self->field_0xCA0, lbl_eu_80663EC8);
}
#pragma pop
void init_1F6C(){}
extern "C" u32 func_80082900__Q22cf13CfGameManagerFv();
#pragma push
#pragma auto_inline off
// us-800b2838: with the game manager active, run the virtual slot-2 callback
// on the optional 0xCA4 / 0xCA8 sub-objects (each null-guarded).
void func_800B1F6C(UnkClass_805764CC* self) {
    if (func_80082900__Q22cf13CfGameManagerFv() == 0) {
        return;
    }
    if (self->field_0xCA4 != 0) {
        ((IDispB1F6C*)self->field_0xCA4)->unk02();
    }
    if (self->field_0xCA8 != 0) {
        ((IDispB1F6C*)self->field_0xCA8)->unk02();
    }
}
#pragma pop
// us-800b28a4: release the three optional sub-objects (0xCA4/0xCA8 via
// func_801742D4, 0xD00 via func_802B2A08), each null-guarded.
#pragma push
#pragma auto_inline off
extern "C" void func_800B1FD8(UnkClass_805764CC* self) {
    if (self->field_0xCA4 != 0) {
        func_801742D4((void*)self->field_0xCA4);
    }
    if (self->field_0xCA8 != 0) {
        func_801742D4((void*)self->field_0xCA8);
    }
    if (self->field_0xD00 != 0) {
        func_802B2A08((void*)self->field_0xD00);
    }
}
#pragma pop
#pragma push
#pragma auto_inline off
extern "C" void func_800B2034(UnkClass_805764CC* self, void* obj){if (self->field_0xCFC){func_801F3CCC(self->field_0xCFC, obj);}}
#pragma pop
extern "C" void func_800B2048(UnkClass_805764CC* self, void* obj){if (self->field_0xCFC){func_801F45B4(self->field_0xCFC, obj);}}
// TEST_FUNC_205C
#pragma push
#pragma auto_inline off
extern "C" void func_800B20A0(UnkClass_805764CC* self, void* obj){if (self->field_0xCAC){func_802074F0(self->field_0xCAC, obj);}}
#pragma pop
void init_20B4(){}
// us-800b2928: if the game-manager getter is nonzero and self->field_0xCAC
// is set, drive func_80206BD4 with it.
extern "C" u32 func_80082900__Q22cf13CfGameManagerFv();
#pragma push
#pragma auto_inline off
extern "C" void func_800B205C(UnkClass_805764CC* self) {
    if (func_80082900__Q22cf13CfGameManagerFv() && self->field_0xCAC) {
        func_80206BD4((CfMapMineManager*)self->field_0xCAC);
    }
}
#pragma pop
u32 UnkClass_805764CC::get_u32_04(){return *(u32*)((u8*)this + 0x4);}
// Target 5: us-800b35fc - __dt__800B2D30
// Destructor that calls subobject destructor, then frees memory if flags > 0.
// Returns self (standard MWCC destructor convention).
void* __dt__800B2D30(void* self, int flags) {
    void* ret = self;
    if (self != NULL) {
        __dt__8047BDA8(self);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return ret;
}
void FactoryEvent1__Q22cf13IFactoryEventFv(){}
void UnkClass_805764CC::fwdTo_2DB0(int param){extern void fwd_2DB0_body(void*, void*); fwd_2DB0_body(this, &param);}
void fwd_2DB0_body(){}

// Target 3: us-800b367c - claim a free pool slot and append the new entry
// before the sentinel (push-back onto the circular chain).
// Finds the first empty slot (entry.next == 0) in the pool array, copies the
// payload's first word into entry.data, then links the entry after the
// sentinel's prev node. The count and pool base are re-read from self inside
// the search loop (retail reloads them each iteration).
extern "C" void func_800B2DB0(FactoryPoolList* self, void* payloadSrc) {
    int idx = 0;
    int off = 0;
    int count;
    CfReslistNode* sentinel = self->mStartNodePtr; // r9: loaded before the loop
    count = self->mCapacity;
    while (idx < count) {
        // Empty slot claimed by this insert: its link word is still null.
        if (*(void**)((u8*)self->mList + off) == 0) {
            break;
        }
        off += 0xC;
        idx++;
    }
    // Base is re-read after the search (retail reloads mList a third time).
    void* poolBase = self->mList;
    FactoryPoolEntry* entry = (FactoryPoolEntry*)((u8*)poolBase + idx * 0xC);
    void** slot = &entry->data;
    if (slot != 0) {
        // reslist setItem wraps its store in try/catch; under C++ exceptions
        // the inlined guard makes MWCC emit the unwinder sp-save (stw r1).
        try {
            *slot = *(void**)payloadSrc;
        } catch (...) {
            throw;
        }
    }
    entry->next = sentinel;
    // Retail reloads sentinel->prev instead of CSE-ing it.
    entry->prev = sentinel->mPrev;
    ((CfReslistNode*)sentinel->mPrev)->mNext = (CfReslistNode*)entry;
    sentinel->mPrev = (CfReslistNode*)entry;
}
// Target 2: us-800b3704 - func_800B2E38
// Insert a node into a doubly-linked list with a fixed-size entry pool.
// Finds the first empty slot (entry[0] == 0) in the pool at list+0x14 (count at list+0x18),
// copies data from r6 into entry[8], copies the node template from r5[0] into entry[0..7],
// and links it into the list before the node pointed to by r5[0].
// Returns the new entry pointer via *r3.
extern "C" void func_800B2E38(void** out, void* list, void* templ, void* data) {
    s32 idx = 0;
    s32 byteOff = 0;

    // Find first empty slot (entry[0] == 0). Count and entry base re-read
    // per iteration (retail cmp r8,r10; blt loop, not counted mtctr).
    for (; idx < *(s32*)((u8*)list + 0x18); idx++) {
        if (*(u32*)(*(u32*)((u8*)list + 0x14) + byteOff) == 0) {
            break;
        }
        byteOff += 0xC;
    }

    // Calculate entry pointer
    u32* newEntry = (u32*)(*(u32*)((u8*)list + 0x14) + idx * 0xC);

    // Copy data word into entry[8] (retail guards the computed address:
    // addic. r4,r7,8; beq - kept from a source-level pointer null check).
    u32* p8 = (u32*)((u8*)newEntry + 8);
    if (p8 != 0) {
        *p8 = *(u32*)data;
    }

    // Insert before the node pointed to by templ[0]
    u32* targetNode = *(u32**)templ;
    newEntry[0] = (u32)targetNode;    // entry->next = target
    newEntry[1] = targetNode[1];       // entry->prev = target->prev
    // Link forward: target->prev->next = entry
    ((u32*)targetNode[1])[0] = (u32)newEntry;
    // Link backward: target->prev = entry
    targetNode[1] = (u32)newEntry;

    *out = newEntry;
}
void init_2ED0(){}
int obj_testBit_64_v0(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v1(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v2(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v3(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v4(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v5(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
int obj_testBit_64_v6(void* p){return (*(int*)((char*)p + 0x64) >> 1) & 1;}
u32 shift_u32_hi8(u32 val){return (val >> 16) & 0xFF;}
// Sorted circular-buffer insertion ordered by the element's sort key at
// offset 0x74. Fast-path appends when the new key sorts after the last
// element; otherwise binary-searches the insertion point and shifts the
// shorter side (front shift advances the head, back shift does not).
extern "C" void func_800B3210(UnkClass_800B0AD8* self, UnkClass_805764CC** item_ptr) {
    u32 count = self->unkB00;

    if (count != 0) {
        UnkClass_805764CC* item = *item_ptr;
        u32 key = ((RingSortKeyView*)item)->sortKey;
        UnkClass_805764CC** buf = (UnkClass_805764CC**)self->unkAF8;

        // Append fast path: new key >= last element's key.
        {
            u32 lastIdx = (self->unkAFC + count - 1) % self->unkB04;
            if (((RingSortKeyView*)buf[lastIdx])->sortKey >= key) {
                buf[(self->unkAFC + count) % self->unkB04] = item;
                self->unkB00 = count + 1;
                return;
            }
        }

        // Binary search: number of existing entries whose key < new key.
        u32 lo = 0;
        s32 range = count;
        while (range > 0) {
            s32 mid = range / 2;
            u32 probe = lo + mid;
            u32 idx = (self->unkAFC + probe) % self->unkB04;
            if (((RingSortKeyView*)buf[idx])->sortKey < key) {
                lo = probe + 1;
                range -= mid + 1;
            } else {
                range = mid;
            }
        }

        if (lo < count / 2) {
            // Front-shift [0, lo) down one slot, then retreat the head
            // (frees the slot just below the insertion point).
            for (s32 i = lo - 1; i >= 0; i--) {
                u32 src = (self->unkAFC + i) % self->unkB04;
                buf[(src - 1) % self->unkB04] = buf[src];
            }
            self->unkAFC = (self->unkAFC - 1) % self->unkB04;
        } else {
            // Back-shift (lo, count-1] up one slot.
            for (u32 i = count - 1; i > lo; i--) {
                u32 src = (self->unkAFC + i) % self->unkB04;
                buf[(src + 1) % self->unkB04] = buf[src];
            }
        }

        buf[(self->unkAFC + lo) % self->unkB04] = item;
        self->unkB00 = count + 1;
        return;
    }

    // Empty ring: drop the item straight into the head slot.
    ((UnkClass_805764CC**)self->unkAF8)[self->unkAFC] = *item_ptr;
    self->unkB00 = count + 1;
}
u32 UnkClass_805764CC::get_u32_74(){return *(u32*)((u8*)this + 0x74);}
void init_39C8(){}

// Target 5: us-800b39C8 variant of func_800B2DB0 that inserts BEFORE the
// current head node instead of before the sentinel: the head pointer is
// captured up front (sentinel->next) and used as the insertion anchor.
extern "C" void func_800B39C8(FactoryPoolList* self, void* payloadSrc) {
    int idx = 0;
    int off = 0;
    CfReslistNode* sentinel = self->mStartNodePtr;   // r5
    int count = self->mCapacity;                     // r8
    CfReslistNode* head = (CfReslistNode*)sentinel->mNext; // r9
    while (idx < count) {
        // Empty slot claimed by this insert: its link word is still null.
        if (*(void**)((u8*)self->mList + off) == 0) {
            break;
        }
        off += 0xC;
        idx++;
    }
    // Base is re-read after the search (retail reloads mList a third time).
    void* poolBase = self->mList;
    FactoryPoolEntry* entry = (FactoryPoolEntry*)((u8*)poolBase + idx * 0xC);
    void** slot = &entry->data;
    if (slot != 0) {
        // reslist setItem wraps its store in try/catch; under C++ exceptions
        // the inlined guard makes MWCC emit the unwinder sp-save (stw r1).
        try {
            *slot = *(void**)payloadSrc;
        } catch (...) {
            throw;
        }
    }
    entry->next = head;
    // Retail reloads head->prev instead of CSE-ing it.
    entry->prev = head->mPrev;
    ((CfReslistNode*)head->mPrev)->mNext = (CfReslistNode*)entry;
    head->mPrev = (CfReslistNode*)entry;
}
void copy_int_ptr_alt2(int* dst, int* src){*dst = *src;}
void UnkClass_805764CC::fwdTo_99EC(int param){extern void fwd_99EC_body(void*, void*); fwd_99EC_body(this, &param);}
void init_3A88(){}
u32 shift_u32_byte3(u32 val){return val >> 24;}
u32 shift_u32_hi8_alt1(u32 val){return (val >> 16) & 0xFF;}
void init_3D4C(){}
void init_3D64(){}
void init_4120(){}
void init_4278(){}
void init_42E8(){}
#pragma push
#pragma auto_inline off
// us-800b4b74: walk the reslist at self+4; for each node whose payload's
// field_0x64 mask intersects `mask`, invoke func_800B3A88. The head pointer
// is re-read from memory every iteration (retail reloads self+4).
extern "C" void func_800B4278(void* object, u32 mask) {
    CfReslistNode* headCell = *(CfReslistNode**)((u8*)object + 4);
    CfReslistNode* cur = (CfReslistNode*)headCell->mNext;
    while (cur != *(CfReslistNode**)((u8*)object + 4)) {
        CfObjFieldView* data = (CfObjFieldView*)((CfReslistNode*)cur->mItem);
        cur = (CfReslistNode*)cur->mNext;
        if (mask & data->field_0x64) {
            func_800B3A88((UnkClass_805764CC*)object, data);
        }
    }
}
#pragma pop
#pragma push
#pragma auto_inline off
// us-800b4be4: same walk over the reslist at self+0xBCC, matching payloads
// whose type id (field_0x94) is 5 and whose low 16 bits of field_0x9C equal
// the search value.
extern "C" void func_800B42E8(void* object, u32 value) {
    CfReslistNode* headCell = *(CfReslistNode**)((u8*)object + 0xBCC);
    CfReslistNode* cur = (CfReslistNode*)headCell->mNext;
    while (cur != *(CfReslistNode**)((u8*)object + 0xBCC)) {
        CfObjFieldView* data = (CfObjFieldView*)((CfReslistNode*)cur->mItem);
        cur = (CfReslistNode*)cur->mNext;
        if (data->field_0x94 == 5 && (s32)value == (s32)(u16)data->field_0x9C) {
            func_800B3A88((UnkClass_805764CC*)object, data);
        }
    }
}
#pragma pop
// Target 3: us-800b4c64 - func_800B4368
// Walk a linked list of CfObject nodes starting from self->field_0xBCC,
// find entries where data->field_0x94 == 2, and if name is NULL or
// strcmp(name, data->field_0x120) == 0, call func_800B3A88(self, data).
// The list has sentinel at *(self+0xBCC), nodes are [0]=next, [8]=data_ptr.
extern "C" DECOMP_DONT_INLINE void func_800B4368(UnkClass_805764CC* self, const char* name) {
    u8* cur = *(u8**)(*(u8**)((u8*)self + 0xBCC));

    while (cur != *(u8**)((u8*)self + 0xBCC)) {
        u8* data = *(u8**)(cur + 8);
        cur = *(u8**)cur;

        if (*(s32*)(data + 0x94) == 2) {
            if (name == NULL || strcmp((const char*)(data + 0x120), name) == 0) {
                func_800B3A88(self, (cf::CfObject*)data);
            }
        }
    }
}
#pragma inline
// List-walk search: start at *headPtr and skip nodes until the cursor equals
// *valA or its +8 link equals *valB, advancing *headPtr past each skipped
// node; store the found node to *out (retail func_800B4554).
extern "C" void func_800B4554(void** out, void** headPtr, void** valA, void** valB) {
    void* node;
    while ((node = *headPtr) != *valA && *(void**)((u8*)node + 8) != *valB) {
        *headPtr = *(void**)node;
    }
    *out = node;
}

// Null-guarded triple dispatch: fetch a handle, reset, then pass the reset
// result + handle to the forwarder (retail: handle in r4).
extern "C" void* func_800B720C();
extern "C" void* func_800B6CA0();
extern "C" void func_800B7214(void* a, void* b);

extern "C" void func_800B71CC(void* arg) {
    if (arg != 0) {
        void* v = func_800B720C();
        func_800B7214(func_800B6CA0(), v);
    }
}

// Target 2: us-800b4cfc - func_800B4400
// Clears all nodes from reslist at field_0xC28, then reinitializes its sentinel.
// Then iterates reslist at field_0xBC8, and for each node whose data's field_0x94
// is 1 or 6, calls func_800B2D88(&this->field_0xC28).
extern "C" void func_800B4400(UnkClass_805764CC* self) {
    // Clears the pool list at field_0xC28, re-links its sentinel, then walks
    // the field_0xBC8 list calling func_800B2D88 for nodes whose payload type
    // id (+0x94) is 1 or 6. The sentinel values are re-read through the typed
    // members every pass: the loop store / call keeps MWCC from caching them.
    CfReslistNode* cur = (CfReslistNode*)self->field_0xC28.field_0x04;
    CfReslistNode* p;
    cur = cur->mNext;
    goto check;
loop:
    p = cur;
    cur = cur->mNext;
    p->mNext = 0;
check:
    if (cur != (CfReslistNode*)self->field_0xC28.field_0x04) goto loop;

    CfReslistNode* s = (CfReslistNode*)self->field_0xC28.field_0x04;
    s->mNext = s;
    s = (CfReslistNode*)self->field_0xC28.field_0x04;
    s->mPrev = s;

    cur = ((CfReslistNode*)self->field_0xBC8.field_0x04)->mNext;
    goto check2;
loop2:
    void* data = cur->mItem;                 // node->field_0x08 payload
    s32 type = *(s32*)((u8*)data + 0x94);    // data->field_0x94 type id
    if (type == 1 || type == 6) {
        func_800B2D88(&self->field_0xC28, data);
    }
    cur = cur->mNext;
check2:
    if (cur != (CfReslistNode*)self->field_0xBC8.field_0x04) goto loop2;
}
void init_44A0(){}
void init_4554(){}
void init_4588(u32 val){}
u32 shift_u32_hi8_alt2(u32 val){return (val >> 16) & 0xFF;}
void init_45A0(){}
void init_47A8(){}
void init_4A24(){}
int CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv(void* self){return 0;}
// us-800b5408: flag cascade over field_0x15F0 - equivalent to (4 <= v <= 8)
// but written as retail's chained guards so each stage short-circuits into
// its own compare/branch pair.
int func_800B4B0C(UnkClass_805764CC* self) {
    int ret = 1;
    int c = 1;
    int b = 1;
    int a = 1;
    int v = self->field_0x15F0;
    if (v != 4 && v != 5) {
        a = 0;
    }
    if (a == 0 && v != 6) {
        b = 0;
    }
    if (b == 0 && v != 7) {
        c = 0;
    }
    if (c == 0 && v != 8) {
        ret = 0;
    }
    return ret;
}
int func_800B4B74(UnkClass_805764CC* self, u32 val){return (self->field_0x15F0 == val) ? 1 : 0;}
void init_4B88(){}
extern const float lbl_eu_806669D8;
// Target us-800b559c: func_800B4CA0
// Battle-state gate: pass when the object's slot-0xAF callback reports set or
// the 0x3F08 bit-4 flag is on; otherwise require the battle manager to be up,
// this to be in the battle list, and the sub-record's bit 18 before firing
// the slot-0x46 float callback.
extern "C" s32 func_800B4CA0(Func4CA0Obj* self) {
    // Battle-state gate: returns 0 when the slot-0xAF callback reports set or
    // the +0x3F08 bit-4 flag is on; otherwise requires the battle manager,
    // membership via func_800DA06C, and the sub-record's bit 18 before firing
    // the slot-0x46 float callback and returning 1.
    s32 flag;
    void* mgr = getInstance__Q22cf14CBattleManagerFv();
    flag = 0;
    if (self->unkAF() != 0 || (self->field_3F08 & 0x08000000) != 0) {
        flag = 1;
    }
    if (flag == 0) {
        if ((self->field_3F00 & 0x08000000) == 0) {
            if (mgr != 0) {
                if (func_800DA06C(mgr, self) != 0) {
                    Func4CA0Sub* sub = (Func4CA0Sub*)self->field_3F60;
                    s32 bit = 0;
                    if (sub != 0) {
                        bit = (sub->field_4EC >> 18) & 1;
                    }
                    if (bit != 0) {
                        self->unk46(lbl_eu_806669D8);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
void init_4D84(){}
u32 shift_u32_hi8_alt3(u32 val){return (val >> 16) & 0xFF;}
extern u16 lbl_eu_8066408C;
unsigned short gvar_get408C(){return lbl_eu_8066408C;}
#pragma push
#pragma auto_inline off
extern "C" u16 func_800B4F64(){extern u16 lbl_eu_80664314; return lbl_eu_80664314;}
#pragma pop
void init_4F6C(){}
void init_4F80(){}
void init_4F90(){}
u32 UnkClass_805764CC::get_u32_70(){return *(u32*)((u8*)this + 0x70);}
void init_4FAC(){}
void init_535C(){}
void init_570C(){}
void init_587C(){}
void init_5944(){}
void init_5948(){}
void UnkClass_805764CC::clear_380(){*(u32*)((u8*)this + 896) = 0;}
void* UnkClass_805764CC::getNull(){return 0;}
u32 UnkClass_805764CC::get_u32_380(){return *(u32*)((u8*)this + 0x380);}

// Target 2: us-800b13c0 - __dt__800B0AF4
// Destructor for UnkClass_800B0AD8. Clears counters and frees memory if flags > 0.
// Returns self (standard MWCC destructor convention).
extern "C"
UnkClass_800B0AD8* __dt__800B0AF4(void* selfv, int flags) {
    UnkClass_800B0AD8* self = (UnkClass_800B0AD8*)selfv;
    if (self != NULL) {
        self->unkB00 = 0;
        self->unkAFC = 0;
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

// Target 2: us-800b6274 - Store value at array index, increment counter.
// Single-expression postfix form `arr[(*cnt)++] = *val` is required: it puts
// the rlwinm addr in r0 and the addi next in r4 (dead param reg) exactly like
// retail; the two-statement form always emits the swapped colors (r0/r4).
extern "C" void func_800B5978(UnkClass_805764CC* self, const u32* val) {
    u32* cnt = (u32*)((u8*)self + 0x380);
    ((u32*)self)[(*cnt)++] = *val;
}
void init_5994(){}
void init_6484(){}
void init_6494(){}
u32 shift_u32_hi8_alt4(u32 val){return (val >> 16) & 0xFF;}
void init_64B8(){}
u32 shift_u32_and(u32 a, u32 b){return a & b;}
u32 UnkClass_805764CC::get_u32_98(){return *(u32*)((u8*)this + 0x98);}
u16 UnkClass_805764CC::get_u16_45C0(){return *(u16*)((u8*)this + 0x45C0);}
u32 shift_u32_hi8_alt5(u32 val){return (val >> 16) & 0xFF;}
void init_64F8(){}
void init_6508(){}
void UnkClass_805764CC::appendToBuffer(void* other){
    unsigned int count = *(unsigned int*)((unsigned char*)this + 0x700);
    unsigned int val = *(unsigned int*)other;
    unsigned int newCount = count + 1;
    *(unsigned int*)((unsigned char*)this + 0x700) = newCount;
    float fval = *(float*)((unsigned char*)other + 4);
    void* dest = (unsigned char*)this + count * 8;
    *(unsigned int*)dest = val;
    *(float*)((unsigned char*)dest + 4) = fval;
}
void init_6544(){}
void init_655C(){}
void init_66AC(){}
void init_66BC(){}

// Target 3: us-800b70c8 - Return 1 if byte at offset 2 is in [1, 24].
// (retail codegen: lbz; li r3,0; cmplwi r0,1; bltlr; cmplwi r0,24; bgtlr; li r3,1; blr -
//  matches under GC/3.0a5.2; Wii/1.1 folds the range into (u8)(val-1)<=23)
extern "C" DECOMP_DONT_INLINE int func_800B67CC(void* self) {
    u8 val = *(u8*)((u8*)self + 2);
    int result = 0;
    if (val >= 1) {
        if (val <= 24) {
            result = 1;
        }
    }
    return result;
}
#pragma inline
void* UnkClass_805764CC::getPtr_1A8(){return (void*)((u8*)this + 0x1a8);}
void UnkClass_805764CC::clear_700(){*(u32*)((u8*)this + 1792) = 0;}
void init_6800(){}
void init_68A8(){}
void init_6AF4(){}
extern "C" UnkClass_805764CC* func_800B6BA0() { return func_800B07E8(); }
void* sub_getReslist_B28(){return &UnkClass_805764CC::func_800B07E8()->field_0xB28;}
void* sub_getReslist_B48(){return &UnkClass_805764CC::func_800B07E8()->field_0xB48;}
void* sub_getReslist_B68(){return &UnkClass_805764CC::func_800B07E8()->field_0xB68;}
void* sub_getReslist_B88(){return &UnkClass_805764CC::func_800B07E8()->field_0xB88;}
void* sub_getReslist_BE8(){return &UnkClass_805764CC::func_800B07E8()->field_0xBE8;}
void* sub_getReslist_BC8(){return &UnkClass_805764CC::func_800B07E8()->field_0xBC8;}
void* sub_getReslist_C08(){return &UnkClass_805764CC::func_800B07E8()->field_0xC08;}
void* sub_getReslist_C48(){return &UnkClass_805764CC::func_800B07E8()->field_0xC48;}
extern "C" reslist<cf::CfObject*>* func_800B6CC4() {
    UnkClass_805764CC* obj = func_800B07E8();
    func_800B4400(obj);
    return &obj->field_0xC28;
}

// Target: us-800b76f0 - func_800B6DD0
// Locate `obj` in the object list (walk via func_800B4554 between two fresh
// iterators), then double-check it against two more iterator snapshots
// (func_800B182C advances the found node) before extracting the matched
// entry. A valid entry is re-bound through a recursive call.
extern "C" void* func_800B6DD0(void* reslist, void* obj) {
    if (obj == 0) {
        return 0;
    }
    F8C0IteratorNode itA;      // retail sp+0x14
    F8C0IteratorNode itB;      // retail sp+0x18
    void* found;               // retail sp+0x1c
    func_8007F8F4__Q22cf13CfGameManagerFv(&itA, (F8C0ListSource*)reslist);
    func_8007F8C0__Q22cf13CfGameManagerFv(&itB, (F8C0ListSource*)reslist);
    func_800B4554(&found, (void**)&itB, (void**)&itA, &obj);
    F8C0IteratorNode itC;      // retail sp+0x10
    void* result;
    func_8007F8F4__Q22cf13CfGameManagerFv(&itC, (F8C0ListSource*)reslist);
    result = 0;
    if (func_8007F900__Q22cf13CfGameManagerFv((const u32*)&found, (const u32*)&itC)) {
        F8C0IteratorNode itD;  // retail sp+0xC
        func_8007F8F4__Q22cf13CfGameManagerFv(&itD, (F8C0ListSource*)reslist);
        func_800B182C(&found);
        if (func_8007F900__Q22cf13CfGameManagerFv((const u32*)&found, (const u32*)&itD)) {
            result = *func_8007F8D0__Q22cf13CfGameManagerFv(&found);
        }
    }
    if (result != 0 && func_800B64AC(result)) {
        return func_800B6DD0(reslist, result);
    }
    return result;
}
extern "C" void* func_800B6CF8(void* arg) {
    UnkClass_805764CC* obj = func_800B07E8();
    func_800B44A0(obj, arg);
    return &obj->field_0xC28;
}
void* func_800B6D3C(void*);
void fwd_6DD0_body(){}
void init_6EC0(){}
u32 UnkClass_800B0AD8::getCount(){return *(u32*)((u8*)this + 0xB00);}
u32 UnkClass_800B0AD8::getSize(){return *(u32*)((u8*)this + 0xB04);}
void* UnkClass_800B0AD8::getRingElem(u32 index){
    u32 start = *(u32*)((u8*)this + 0xAFC); u32 count = *(u32*)((u8*)this + 0xB04);
    u32* base = *(u32**)((u8*)this + 0xAF8); return &base[(start + index) % count];
}
void sub_mainReset(){func_800B6D3C(func_800B07E8());}
void init_7058(){}
void init_708C(){}
void init_70FC(){}
u32 gvar_get40F4(void){extern u32 lbl_eu_806640F4; return lbl_eu_806640F4;}
void init_71CC(){}
void* UnkClass_805764CC::getPtr_720(){return (void*)((u8*)this + 0x720);}
void init_7214(){}
void init_72DC(){}
void init_7320(){}
void list_removeNode(void* out_prev, void* unused, void* list_ptr){
    (void)unused; void* entry = *(void**)list_ptr; void* prev = *(void**)entry;
    void* next = *(void**)((char*)entry + 4); *(void**)next = prev;
    *(void**)((char*)prev + 4) = next; entry = *(void**)list_ptr;
    *(void**)entry = NULL; *(void**)out_prev = prev;
}
void init_7410(){}
unsigned short gvar_get408C_alt1(){return lbl_eu_8066408C;}
unsigned short gvar_get408C_alt2(){return lbl_eu_8066408C;}
void UnkClass_805764CC::maskField_6C(u32 mask, int enable){
    u32* field = (u32*)((u8*)this + 0x6C);
    if (enable) { *field |= mask; } else { *field &= ~mask; }
}
#pragma push
#pragma auto_inline off
extern "C" u16 func_800B75B4(){extern u16 lbl_eu_80663E42; return lbl_eu_80663E42;}
extern "C" u16 func_800B75BC(){extern u16 lbl_eu_80663E44; return lbl_eu_80663E44;}
#pragma pop
void init_75EC(){}

// Target 5: us-800b7f9c - Count nodes in a circular linked list.
// Retail assigns head=r5/cur=r4. Winning shape: declare cur BEFORE head
// (uninitialized) so head's vreg is born after cur's, giving head the
// higher register r5 (retail lwz r5,4(r3); lwz r4,0(r5); cmpl r4,r5).
// auto_inline off: retail calls this (not inlined) from func_800B75EC.
#pragma push
#pragma auto_inline off
extern "C" int func_800B7680(void* self) {
    void* cur;
    void* head = *(void**)((u8*)self + 4);
    cur = *(void**)head;
    int count = 0;
    while (cur != head) { cur = *(void**)cur; count++; }
    return count;
}
#pragma pop
void sub_resetReslist_B28(void* self){func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB28);}
void sub_resetReslist_B48(void* self){func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB48);}
void sub_resetReslist_B68(void* self){func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB68);}
void* sub_resetReslist_B88(void* self){return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xB88);}
void* sub_resetReslist_BE8(void* self){return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBE8);}
void* sub_resetReslist_BC8(void* self){return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBC8);}
void* sub_resetReslist_C08(void* self){return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xC08);}
void* sub_resetReslist_BA8(void* self){return func_800B6D3C(&UnkClass_805764CC::func_800B07E8()->field_0xBA8);}
// auto_inline off: retail keeps these list-step helpers as real calls (bl)
// from func_800B8B94/func_800B8C78; without this the -ipa pass folds them in.
#pragma push
#pragma auto_inline off
cf::CfObject* func_800B77E4(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    cf::CfObject* next = (cf::CfObject*)func_800B6DD0(&ctx->field_0xB28, obj);
    // Retail returns the next entry in r3 (kept from func_800B6DD0).
    return next;
}
#pragma pop

void func_800B7854(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xB68, obj);
}

#pragma push
#pragma auto_inline off
// extern "C": retail exports this helper under the unmangled name (reloc fix).
extern "C" cf::CfObject* func_800B781C(cf::CfObject* obj);
cf::CfObject* func_800B781C(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    cf::CfObject* next = (cf::CfObject*)func_800B6DD0(&ctx->field_0xB48, obj);
    // Retail returns the next entry in r3 (kept from func_800B6DD0).
    return next;
}
#pragma pop

#pragma push
#pragma auto_inline off
// extern "C": retail symbol is the unmangled name (see reloc fix in hexdiff).
extern "C" void* func_800B78C4(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xBE8, obj);
    // No explicit return: retail keeps whatever func_800B6DD0 left in r3
    // (the next list entry) as the return value.
}
#pragma pop

// Target 1: us-800b8218 - Add CfObject to reslist at field_0xBC8
void func_800B78FC(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xBC8, obj);
}

// Target 2: us-800b8250 - Add CfObject to reslist at field_0xBA8
void func_800B7934(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xBA8, obj);
}

// Target 3: us-800b8288 - Add CfObject to reslist at field_0xC08
void func_800B796C(cf::CfObject* obj) {
    UnkClass_805764CC* ctx = func_800B07E8();
    func_800B6DD0(&ctx->field_0xC08, obj);
}
void init_79A4(){}
extern "C" void* func_8007DA00__Q22cf13CfGameManagerFv();
extern "C" void* func_800B7744();
#pragma push
#pragma auto_inline off
// us-800b82c0: while the game manager is active, iterate the object chain
// starting at func_800B7744(), invoking virtual slot 0x67 (vtable + 0x19C)
// with the singleton as argument; func_800B78C4 yields the next entry.
void func_800B79A4(UnkClass_805764CC* self) {
    if (func_8007DA00__Q22cf13CfGameManagerFv() == 0) {
        return;
    }
    IDispB79A4* node = (IDispB79A4*)func_800B7744();
    while (node != 0) {
        node->unk67(self);
        node = (IDispB79A4*)func_800B78C4((cf::CfObject*)node);
    }
}
#pragma pop
void init_7A18(){}
void init_7AF0(){}
void init_83AC(){}
void init_8524(){}
void init_dispatchTarget_3(){}
void init_dispatchTarget_4(){}
void init_8804(){}
// Target 4: us-800b91fc - func_800B88E0
// Remove nodes matching a given ID from a linked list at offset 0xC84.
// `next` is declared before `node` so MWCC colors node=r7/next=r6 like
// retail (the reverse declaration order swaps the registers).
void func_800B88E0(u8* self, u32 targetId) {
    u32* head = *(u32**)(self + 0xC84);
    u32* sentinel = head;
    u32* next;
    u32* node = (u32*)*head;
    while (node != sentinel) {
        next = (u32*)*node;
        if (node[2] == targetId) {
            u32* prev = (u32*)node[1];
            *prev = (u32)next;
            next[1] = (u32)prev;
            *node = 0;
        }
        node = next;
    }
}
extern "C" {
    extern s8 lbl_eu_80663EE8;
    extern u8 lbl_eu_80572CD4[];
    extern u8 lbl_eu_80572CC8[];
    extern void __dt__17UnkClass_805764CCFv(void*, int);
    extern void __ct__17UnkClass_805764CCFv(void*);
    extern void __register_global_object(void*, void*, void*);
    extern void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
    extern int func_800B64AC(void* p);
}
// Target: us-800b10b4 - func_800B07E8 (singleton sinit: init once, then return &singleton)
// auto_inline off: DECOMP_DONT_INLINE is empty under GC/3.0a5.2 (__MWERKS__ 0x4199)
// so the -ipa pass folds this body into func_800B6CC4/6CF8's spans.
#pragma push
#pragma auto_inline off
UnkClass_805764CC* func_800B07E8() {
    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }
    return (UnkClass_805764CC*)lbl_eu_80572CD4;
}
#pragma pop
#pragma inline
// Target: us-800b1160 - func_800B0894 (allocate + zero-fill array of count*0xc, store at +0x14/+0x18)
// auto_inline off: without it the -ipa pass folds this 0x12C body into
// func_800B084C (its only caller), inflating the wrapper to 316B. Retail's
// func_800B084C stays a plain wrapper, so the inline must be blocked.
#pragma push
#pragma auto_inline off
extern "C" void func_800B0894(UnkClass_805764CC* self, unsigned long handle, unsigned long count) {
    u32* arr = (u32*)allocate_array__Q23mtl10MemManagerFUlUl(count * 0xc, handle);
    self->field_0x14 = (u32)arr;
    for (int i = 0; i < (int)count; i++) {
        arr[i * 3] = 0;
    }
    self->field_0x18 = (u32)count;
    // OPEN ITEM: retail unrolls this loop 8x with per-store reload of self->field_0x14
    // (aliasing pattern); 3 source shapes tried (local arr, ptr loop, member-cast),
    // best 23.4%/44 structural/0x134 vs 0x12c. Next: element-struct type or
    // reslist-method access form.
}
#pragma pop
#pragma inline
// Target: us-800b2220 - func_800B1954 (list cleanup)
extern "C" void func_800B1A8C(void* a, void* b);
extern "C" void* func_800B1A9C(void* a);
extern "C" void func_800B1AA8(void* a, void* b, int c);
extern "C" void func_800B1ACC(void* a, void* b);
extern "C" int func_800B1AD8(void* a, void* b);
extern "C" void func_800B0B40(void* a);
extern "C" void func_800B4278(void* object, u32 mask);
extern float lbl_eu_80663EDC;
extern u32 lbl_eu_80663EE4;
extern "C" void func_800B1954(UnkClass_805764CC* self) {
    func_800B4278(self, 0xFFFFFDFF); // retail: lis 1, subi 0x201 = 0x0000FDFF; li-form here aligns rest better (OPEN ITEM)
    func_800B4278(self, 0xFFFFFFFF);
    func_800B1A5C((void*)((u8*)self + 0xc28));
    func_800B1A5C((void*)((u8*)self + 0xc08));
    func_800B1A5C((void*)((u8*)self + 0xbc8));
    func_800B1A5C((void*)((u8*)self + 0xbe8));
    func_800B1A5C((void*)((u8*)self + 0xb88));
    func_800B1A5C((void*)((u8*)self + 0xb68));
    func_800B1A5C((void*)((u8*)self + 0xb48));
    func_800B1A5C((void*)((u8*)self + 0xb28));
    func_800B1A5C((void*)((u8*)self + 0xba8));
    func_800B1A5C(self);
    func_800B0B40((void*)((u8*)self + 0x20));
    self->field_0xD0E = 0;
    self->field_0xD10 = 0;
    self->field_0xD04 = 0;
    lbl_eu_80663EDC = 0.0f;
    lbl_eu_80663EE4 = 0;
    u32 sbA[4], sbB[4], sbC[4];
    func_800B1A8C(sbA, (void*)((u8*)self + 0xc48));
    for (;;) {
        func_800B1ACC(sbB, (void*)((u8*)self + 0xc48));
        if (func_800B1AD8(sbA, sbB) != 0) {
            void* item = func_800B1A9C(sbA);
            *(u32*)item = 0;
            func_800B1AA8(sbC, sbA, 0);
        } else {
            break;
        }
    }
}
// Target: us-800b6fb8 - func_800B66BC (guard-chain + event queue dispatch)
extern "C" void func_800B655C(UnkClass_805764CC* self, const F8C0ListSource* list);
extern "C" void* func_800B67EC();
extern "C" void func_800B67F4(void* buf);
extern "C" void func_800B5994(void* self, void* arg, void* list, void* buf, float f);
extern "C" void func_800B4D84(void* self, void* buf);
extern "C" int CfRes_getE24Bit22();
extern "C" float func_80069EA0();
extern "C" int func_800829B8__Q22cf13CfGameManagerFv();
extern "C" void* getInstance__Q22cf14CBattleManagerFv();
extern float lbl_eu_80663ED8;
extern float lbl_eu_80663EC8;
extern float lbl_eu_80661CCC;
extern "C" void func_800B66BC(UnkClass_805764CC* self, void* arg) {
    if (arg == 0) return;
    if (func_800829B8__Q22cf13CfGameManagerFv() != 0) return;
    if (func_800B1C0C(2) == 0) {
        func_800B655C(self, (const F8C0ListSource*)((u8*)self + 0xb48));
    }
    if (lbl_eu_80663ED8 > 0.0f) {
        float dt = func_80069EA0();
        lbl_eu_80663ED8 -= dt;
        if (lbl_eu_80663ED8 < 0.0f) {
            lbl_eu_80663ED8 = 0.0f;
        }
    }
    int result = 0;
    if (CfRes_getE24Bit22() == 0 && getInstance__Q22cf14CBattleManagerFv() != 0) {
        void* tmp = func_800B67EC();
        result = func_800B67CC(tmp);
    }
    if (result == 0) {
        char buf[0x700];
        func_800B67F4(buf);
        func_800B5994(self, arg, (void*)((u8*)self + 0xb48), buf, lbl_eu_80663EC8);
        func_800B5994(self, arg, (void*)((u8*)self + 0xb68), buf, lbl_eu_80661CCC);
        func_800B4D84(self, buf);
    }
}
// Target: us-800b70fc - func_800B6800
extern "C" void func_800B6800(UnkClass_805764CC* self, void* arg, int flag, float value) {
    extern const float lbl_eu_806669D8;
    if (flag) {
        self->field_0xCF4 = (u32)func_800B39C0(arg);
        self->field_0xCF8 = value;
    } else {
        self->field_0xCF8 = lbl_eu_806669D8;
        self->field_0xCF4 = 0;
        func_800B68A8(self, 0, (void*)&self->field_0xB48, flag, lbl_eu_806669D8);
        func_800B68A8(self, 0, (void*)&self->field_0xB68, flag, lbl_eu_806669D8);
        // OPEN ITEM: best 46.5%/19 structural; residual lfs/stfs f0-vs-f1 PS-float juggling
        // (retail: single lfs f1 hoisted to top, stfs f1; decomp: lfs f0 for stfs + lfs f1
        // per call). 7 source shapes tried; next: PS-float angle / expression order.
    }
    func_800B1C24(4, (void*)flag);
}
// Target: us-800b79ac - func_800B708C (singleton lookup + flag check)
// auto_inline off: retail calls this out-of-line from func_800B75EC.
#pragma push
#pragma auto_inline off
extern "C" void* func_800B708C__Fi(int id) {
    UnkClass_805764CC* obj = func_800B07E8();
    void* result = func_800B6EC0(obj, id);
    bool valid = false;
    if (result != 0 && func_800B64AC(result) == 0) {
        valid = true;
    }
    return valid ? result : 0;
}
#pragma pop
// Target 5: us-800b923c - func_800B8920
// Checks if an address is aligned and within a valid range [0x80000000, 0x93800000),
// then looks up the singleton and calls func_800B6EC0(&singleton, *(this+0x74)).
// Returns 1 if the check passes and func_800B6EC0 returns non-zero, else 0.
extern "C" int func_800B8920(u32 addr) {
    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }

    // Retail computes the singleton address once here (dominating the checks)
    // and reuses it for the func_800B6EC0 call below.
    void* inst = (void*)lbl_eu_80572CD4;

    // Check alignment: low 2 bits must be 0, and address must be in [0x80000000, 0x93800000)
    if ((addr & 3) != 0 || addr < 0x80000000 || addr >= 0x93800000) {
        return 0;
    }

    u32 val = *(u32*)(addr + 0x74);
    return func_800B6EC0((UnkClass_805764CC*)inst, val) != 0;
}
// Target 4: us-800b92e8 - func_800B89CC
// Singleton accessor for UnkClass_805764CC at lbl_eu_80572CD4.
// If the singleton hasn't been initialized (lbl_eu_80663EE8 == 0),
// construct it and register as a global object.
// Then access field_0xCA0 and call func_80193AB0(field_0xCA0, id).
// Returns the result of func_80193AB0, or 0 if field_0xCA0 is NULL.
extern "C" {
    extern s8 lbl_eu_80663EE8;
    extern u8 lbl_eu_80572CD4[];
    extern u8 lbl_eu_80572CC8[];
    extern void __dt__17UnkClass_805764CCFv(void*, int);
    extern void __ct__17UnkClass_805764CCFv(void*);
    extern void __register_global_object(void*, void*, void*);
    extern void* func_80193AB0(void*, u32);
}

extern "C" void* func_800B89CC(u32 id) {
    void* result = NULL;

    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }

    void* ca0 = *(void**)(lbl_eu_80572CD4 + 0xCA0);
    if (ca0 != NULL) {
        result = func_80193AB0(ca0, id);
    }

    return result;
}

// Target 5: us-800b9380 - func_800B8A64
// Same singleton pattern as func_800B89CC, but reads a u16 from
// this->field_0x45C0 and passes it as the id to func_80193AB0.
#pragma push
#pragma auto_inline off
extern "C" void* func_800B8A64(void* self) {
    u16 id = *(u16*)((u8*)self + 0x45C0);
    void* result = NULL;

    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }

    void* ca0 = *(void**)(lbl_eu_80572CD4 + 0xCA0);
    if (ca0 != NULL) {
        result = func_80193AB0(ca0, id);
    }

    return result;
}
#pragma pop

// Target 1: us-800b9418 - func_800B8AFC
// Singleton accessor + field_0xCA0 lookup, then calls func_80193CD0(field_0xCA0, this).
extern "C" void* func_800B8AFC(void* self) {
    void* result = NULL;

    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }

    void* ca0 = *(void**)(lbl_eu_80572CD4 + 0xCA0);
    if (ca0 != NULL) {
        result = func_80193CD0(ca0, self);
    }

    return result;
}
// Target us-800b94b0 / us-800b9594: func_800B8B94 / func_800B8C78
// Walk an object list (head via func_800B76A4/func_800B76CC, advance via
// func_800B77E4/func_800B781C); when an entry's +0x8C id matches, resolve it
// through func_8016FE34 and return. Each step re-runs the inlined singleton
// init guard.
extern "C" void* func_800B76A4();
extern "C" void* func_800B76CC();

void* func_800B8B94(s32 id) {
    if (id == 0) {
        return 0;
    }
    if (!lbl_eu_80663EE8) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }
    B8B94Obj* obj = (B8B94Obj*)func_800B76A4();
    while (obj != 0) {
        if (obj->field_8C == id) {
            return func_8016FE34(obj);
        }
        if (!lbl_eu_80663EE8) {
            __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
            __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
            lbl_eu_80663EE8 = 1;
        }
        obj = (B8B94Obj*)func_800B77E4((cf::CfObject*)obj);
    }
    return 0;
}

void* func_800B8C78(s32 id) {
    if (id == 0) {
        return 0;
    }
    if (!lbl_eu_80663EE8) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }
    B8B94Obj* obj = (B8B94Obj*)func_800B76CC();
    while (obj != 0) {
        // Operand order (id first) matches retail's cmpw r26, r0.
        if (id == obj->field_8C) {
            return func_8016FE34(obj);
        }
        if (!lbl_eu_80663EE8) {
            __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
            __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
            lbl_eu_80663EE8 = 1;
        }
        obj = (B8B94Obj*)func_800B781C((cf::CfObject*)obj);
    }
    return 0;
}
void init_8D5C(){}
void init_8FC4(){}
void sub_dispatchInit_1(){func_800B07E8(); ((void(*)())init_dispatchTarget_1)();}
void sub_dispatchInit_2(){func_800B07E8(); ((void(*)())init_dispatchTarget_2)();}
extern "C" void func_800B8524(void* singleton, void* self, void* other);
// Target: us-800b9c18 - pass the singleton, self and arg to the pair helper.
extern "C" void func_800B92FC(void* self, void* arg) {
    UnkClass_805764CC* obj = func_800B07E8();
    func_800B8524(obj, self, arg);
}
void sub_dispatchInit_3(){func_800B07E8(); ((void(*)())init_dispatchTarget_3)();}
void sub_dispatchInit_4(){func_800B07E8(); ((void(*)())init_dispatchTarget_4)();}
void sub_dispatchInit_5(){func_800B07E8(); ((void(*)())init_dispatchTarget_5)();}
void sub_dispatchInit_6(){func_800B07E8(); ((void(*)())init_dispatchTarget_6)();}
void init_93D0(){}
void init_9404(){}
void init_9438(){}
void init_946C(){}
void init_94A0(){}
void init_dispatchTarget_7(){}
void sub_dispatchWithArgs(u32 a, u32 b){func_800AA2BC(a, b); ((void(*)())init_dispatchTarget_7)();}
void init_9548(){}
void init_957C(){}
void UnkClass_805764CC::set_u32_734(u32 val){*(u32*)((u8*)this + 0x734) = val;}
void UnkClass_805764CC::set_u8_73A(u8 a, u8 b){this->field_0x20.unk0[0x71A] = a; this->field_0x20.unk0[0x71B] = b;}
void UnkClass_805764CC::set_u32_720(u32 val){*(u32*)((u8*)this + 0x720) = val;}
void UnkClass_805764CC::set_u16_738(u16 val){*(u16*)((u8*)this + 0x738) = val;}
void init_96E8(){}
void UnkClass_805764CC::set_float_730(float val){*(float*)((u8*)this + 0x730) = val;}
// Target 1: us-800ba028 - __ct__800B970C
// Constructor for a class with base vtable lbl_eu_8052AC98, final vtable lbl_eu_80537FB0,
// CCharVoice at offset 0x28, and fields at 0x6C/0x70.
// Size: 0x7C (allocated via allocate(0x7c, ...)).

// Forward declarations for symbols in this TU
struct CCharVoice;
extern "C" {
    extern u8 lbl_eu_8052AC98[];
    extern u8 lbl_eu_80537FB0[];
    extern void __ct__CCharVoice(CCharVoice* self);
}

extern "C" void* __ct__800B970C(void* self) {
    u8* s = (u8*)self;
    u8* baseVtab = lbl_eu_8052AC98;
    u8* finalVtab = lbl_eu_80537FB0;

    // Set base vtable and zero fields
    *(u32*)(s + 0x4) = 0;
    *(u32*)(s + 0x8) = 0;
    *(void**)s = baseVtab;
    *(void**)(s + 0xC) = baseVtab + 0xB4;
    *(void**)(s + 0x10) = baseVtab + 0xC4;
    *(u32*)(s + 0x14) = 0;
    *(u32*)(s + 0x18) = 0;
    *(u32*)(s + 0x1C) = 0;

    // Construct CCharVoice at offset 0x28
    __ct__CCharVoice((CCharVoice*)(s + 0x28));

    // Switch to final vtable and zero extra fields
    *(u32*)(s + 0x6C) = 0;
    *(void**)s = finalVtab;
    *(void**)(s + 0xC) = finalVtab + 0xB4;
    *(void**)(s + 0x10) = finalVtab + 0xC4;
    *(u32*)(s + 0x70) = 0;

    return self;
}
// Target: us-800ba0bc - func_800B97A0
// Reads the packed resource token at record+0x4, extracts its type field
// (bits 27..5) and translates it into a func_800B20B4 spawn mask, then
// spawns into the manager singleton, forwarding the record as payload.
extern "C" u32 func_800B2D28(u8* self);
#pragma push
#pragma auto_inline off
void* func_800B97A0(void* self, u32 arg) {
    u32 idx = CfRes_extractBits27_5((void*)(uintptr_t)func_800B2D28((u8*)self));
    u32 mask = 0;
    switch (idx) {
    case 2:  mask = 0x2; break;
    case 3:  mask = 0x8; break;
    case 4:  mask = 0x4; break;
    case 5:  mask = 0x80; break;
    case 12: mask = 0x100020; break;
    case 14: mask = 0x400020; break;
    case 15:
    case 16:
    case 17: mask = 0x800020; break;
    }
    return func_800B20B4(func_800B07E8(), mask, (const B20B4Payload*)self, arg);
}
#pragma pop
// CfGameManager imports used by func_800B985C (retail mangled names).
extern "C" void func_80081258__Q22cf13CfGameManagerFv(void* self);
extern "C" void func_80081264__Q22cf13CfGameManagerFv(void* self, void* value);
extern "C" void func_800812F4__Q22cf13CfGameManagerFv(void* self, unsigned long value);
extern "C" void* func_800B97A0(void* self, void* value);
#pragma push
#pragma auto_inline off
// us-800ba178: build a scratch Unk812Data record on the stack, seed it from
// the three arguments via the CfGameManager setters, then hand it to
// func_800B97A0.
void func_800B985C(void* argA, void* argB, void* argC) {
    u8 buf[0x28];
    func_80081258__Q22cf13CfGameManagerFv(buf);
    func_80081264__Q22cf13CfGameManagerFv(buf, argA);
    func_800812F4__Q22cf13CfGameManagerFv(buf, (unsigned long)argB);
    func_800B97A0(buf, argC);
}
#pragma pop
void init_98C8(){}
u32 UnkClass_805764CC::get_u32_620(){return *(u32*)((u8*)this + 0x620);}
void init_998C(){}
void init_99BC(){}
void fwd_99EC_body(){}
void init_9A30(){}

// Target 1: us-800b141c - reslist<cf::TboxInfo>::reslist() constructor
// (flattened: base init with the _reslist_base vtable, then the derived
// vtable install overwrites +0). Array-typed externs force absolute lis/addi
// addressing; the base vtable store is volatile so MWCC keeps both stores.
// Struct-typed sentinel access reproduces the retail scheduling (same shape
// as the CfObject/IFactoryEvent siblings, with the zeros at 0x2c/0x30/0x34).
void __ct__reslist_cf_TboxInfo(void* self) {
    extern void* lbl_eu_805290DC[];
    extern void* lbl_eu_805290C4[];
    // Retail scheduling: DC addr -> li r0 -> store DC -> node ptr -> zero 0x2c
    // -> C4 addr -> zero 0x30/0x34 -> sentinel links -> store C4.
    TboxInfoReslistLayout* obj = (TboxInfoReslistLayout*)self;
    // Volatile pins the base-vtable store (dead otherwise: the derived
    // install overwrites +0). The derived vtable expression sits mid-body so
    // its lis/addi lands between the zero stores like retail; the store
    // itself sinks past the sentinel links.
    *(volatile u32*)&obj->mVtable = (u32)lbl_eu_805290DC;
    obj->field_0x2c = 0;
    obj->mVtable = (void*)lbl_eu_805290C4;
    obj->field_0x30 = 0;
    obj->field_0x34 = 0;
    obj->mStartNodePtr = &obj->mStartNode;
    obj->mStartNodePtr->mNext = obj->mStartNodePtr;
    obj->mStartNodePtr->mPrev = obj->mStartNodePtr;
}

// Base reslist ctor is unused (the specialized ctors above are flattened
// free functions); kept as an empty template so member-ctor instantiations
// (e.g. generic reslist<T>) never emit stray code.
template <typename T>
_reslist_base<T>::_reslist_base() {}

// Target 5: us-800b128c - UnkClass_805764CC::UnkClass_805764CC()
// Retail constructs the reslist subobjects with flattened free-function ctors
// (reloc names __ct__reslist_cf_*). Real member ctors would mangle to
// __ct__23reslist<...> and drift, so each subobject ctor is called manually
// in declaration order via extern "C" declarations (C linkage emits the exact
// retail names); the FixStr<64> at 0xCB0 is placement-new'd in place.
extern "C" void __ct__reslist_cf_CfObject(void*);
extern "C" void __ct__reslist_cf_TboxInfo(void*);
extern "C" void __ct__reslist_cf_IFactoryEvent(void*);
extern "C" void __ct__17UnkClass_800B0AD8Fv(void*);

UnkClass_805764CC::UnkClass_805764CC() {
    __ct__reslist_cf_CfObject(this);
    __ct__17UnkClass_800B0AD8Fv((u8*)this + 0x20);
    __ct__reslist_cf_CfObject((u8*)this + 0xb28);
    __ct__reslist_cf_CfObject((u8*)this + 0xb48);
    __ct__reslist_cf_CfObject((u8*)this + 0xb68);
    __ct__reslist_cf_CfObject((u8*)this + 0xb88);
    __ct__reslist_cf_CfObject((u8*)this + 0xba8);
    __ct__reslist_cf_CfObject((u8*)this + 0xbc8);
    __ct__reslist_cf_CfObject((u8*)this + 0xbe8);
    __ct__reslist_cf_CfObject((u8*)this + 0xc08);
    __ct__reslist_cf_CfObject((u8*)this + 0xc28);
    __ct__reslist_cf_TboxInfo((u8*)this + 0xc48);
    __ct__reslist_cf_IFactoryEvent((u8*)this + 0xc80);
    field_0xCA0 = 0;
    field_0xCA4 = 0;
    field_0xCA8 = 0;
    field_0xCAC = 0;
    new ((void*)&field_0xCB0) ml::FixStr<64>();
    field_0xCFC = 0;
    field_0xD00 = 0;
    field_0xD04 = 0;
    field_0xD0E = 0;
    field_0xD10 = 0;
    extern u32 lbl_eu_80663EE0;
    func_800B0A90(&lbl_eu_80663EE0);
}

// Target us-800b145c - _reslist_base<cf::TboxInfo>::~_reslist_base()
// Same teardown shape as the derived ~reslist (retail compiles the body into
// both dtors independently): reinstall the base vtable, unlink every chained
// node (nulling each node's next as walked), re-link the sentinel to itself,
// free the pool array when the ownership flag (0x34) is clear, and clear the
// pool pointer (0x2c, stored biased by 0x10). The deleting-dtor tail comes
// from MWCC's member-destructor codegen.
template <>
_reslist_base<cf::TboxInfo>::~_reslist_base() {
    // (the caller-side this==0 guard is MWCC-generated)
    extern void* lbl_eu_805290DC[];
    TboxInfoReslistLayout& obj = *(TboxInfoReslistLayout*)this;
    // Reinstall the base vtable (nothing overwrites +0 afterwards).
    obj.mVtable = (void*)lbl_eu_805290DC;
    CfReslistNode* node = obj.mStartNodePtr->mNext;
    // The sentinel pointer is re-read each iteration (the node store may
    // alias it).
    while (node != obj.mStartNodePtr) {
        CfReslistNode* cur = node;
        node = node->mNext;
        cur->mNext = NULL;
    }
    obj.mStartNodePtr->mNext = obj.mStartNodePtr;
    obj.mStartNodePtr->mPrev = obj.mStartNodePtr;
    if (obj.field_0x34 == 0) {
        char* pool = (char*)obj.field_0x2c;
        // Retail emits two identical null tests around the free.
        if (pool != 0) {
            if (pool != 0) {
                __dla__FPv(pool - 0x10);
            }
            obj.field_0x2c = 0;
        }
    }
}

// Target us-800b1780: ~UnkClass_805764CC()
// Teardown is fully explicit: retail destroys field_0x20 via a direct call to
// the flat __dt__800B0AF4 symbol (a wrapper member-dtor would emit its own
// mangled symbol and IPA zeroes the dead flags argument), so the flat call
// must live in this body - which forces the reslist members to be destroyed
// explicitly ahead of it, in retail order (C80 IFactoryEvent pool, C48
// TboxInfo list, nine CfObject* lists C28..B28). The reslist<cf::CfObject*>
// base at +0 is destroyed implicitly after the body, and MWCC appends the
// delete-this tail when the caller's flags argument > 0.
UnkClass_805764CC::~UnkClass_805764CC() {
    field_0xC80.~reslist();
    field_0xC48.~reslist();
    field_0xC28.~reslist();
    field_0xC08.~reslist();
    field_0xBE8.~reslist();
    field_0xBC8.~reslist();
    field_0xBA8.~reslist();
    field_0xB88.~reslist();
    field_0xB68.~reslist();
    field_0xB48.~reslist();
    field_0xB28.~reslist();
    __dt__800B0AF4(&field_0x20, -1);
}



// Target 2: us-800b6178 - func_800B587C
// Selection sort over a [start,end) array of 8-byte SortEntry pairs using an
// indirect comparator; the best entry each pass is swapped into place.
void func_800B587C(SortEntry* start, SortEntry* end, SortEntryCompare compare) {
    if (start == end) {
        return;
    }
    SortEntry* last = end - 1;
    while (start != last) {
        SortEntry* best = start;
        for (SortEntry* it = start + 1; it != end; it++) {
            if (compare(it, best) != 0) {
                best = it;
            }
        }
        if (best != start) {
            // Swap through explicit temps (retail round-trips the value
            // word through a stack spill).
            void* tempPtr = best->mPointer;
            float tempVal = best->mValue;
            *best = *start;
            start->mPointer = tempPtr;
            start->mValue = tempVal;
        }
        start++;
    }
}

// Target 3: us-800b7a1c - func_800B70FC
// Walks the item-manager list via the CfGameManager iterator primitives; for
// every node whose type id matches arg1, raises func_800BFDE0 with the node's
// container base (the raw node pointer sits at +0x3E9C inside the container).
void func_800B70FC(u32 arg1, u32 arg2) {
    func_800B71C4();
    // node declared first so its vreg is born earliest (retail colors it
    // r31); source follows and lands in r30.
    u32 node;
    const F8C0ListSource* source = (const F8C0ListSource*)func_800B6BA4__Fv();
    // Stack slots (MWCC reverse-decl allocation): outerIter=sp+0x10,
    // dest=sp+0xC, innerIter=sp+0x8.
    F8C0IteratorNode outerIter;
    u32 dest;
    F8C0IteratorNode innerIter;
    func_8007F8C0__Q22cf13CfGameManagerFv(&outerIter, source);
    // Rotated while: MWCC jumps to the condition first (retail's leading
    // `b` over the body); the condition advances the inner iterator via
    // the comma expression, then compares the two cursors.
    while (func_8007F900__Q22cf13CfGameManagerFv(
               (const u32*)&outerIter,
               (func_8007F8F4__Q22cf13CfGameManagerFv(&innerIter, source), (const u32*)&innerIter))) {
        void** nodeSlot = func_8007F8D0__Q22cf13CfGameManagerFv(&outerIter);
        node = (u32)*nodeSlot;
        if (node != 0) {
            node -= 0x3E9C;
        }
        u16 id = func_8007F8B8__Q22cf13CfGameManagerFv((void*)(node + 0x3E9C));
        if (arg1 == id) {
            func_800BFDE0((void*)node, arg2);
        }
        func_8007F8DC__Q22cf13CfGameManagerFv((F8C0IteratorNode*)&dest, &outerIter, 0);
    }
}

// Target 4: us-800b7b34 - func_800B7214
// Allocates the first free pool slot of the TboxInfo reslist (link word == 0),
// copies the payload record in, then links the slot at the head of the list.
void func_800B7214(TboxInfoReslistPoolView* list, TboxPayload* src) {
    s32 index = 0;
    u32 off = 0;
    s32 count;
    CfReslistNode* sentinel = list->mStartNodePtr; // r9: loaded before the loop
    count = list->mSlotCount;                      // r8: loaded once
    while (index < count) {
        // Empty slot claimed by this insert: its link word is still null.
        // Pool base re-read every iteration (retail reloads +0x2c in the loop).
        if (*(CfReslistNode**)((u8*)list->mSlots + off) == 0) {
            break;
        }
        off += 0x24;
        index++;
    }
    // Base is re-read after the search (retail reloads +0x2c a second time).
    TboxPoolSlot* slot = (TboxPoolSlot*)((u8*)list->mSlots + index * 0x24);
    TboxPayload* dst = &slot->mInfo;
    if (dst != 0) {
        dst->field_00 = src->field_00;
        dst->field_04 = src->field_04;
        dst->field_08 = src->field_08;
        dst->field_0C = src->field_0C;
        dst->field_10 = src->field_10;
        dst->field_14 = src->field_14;
        dst->field_18 = src->field_18;
        dst->field_1A = src->field_1A;
        dst->field_1B = src->field_1B;
    }
    // Link at the front: slot <-> sentinel chain update. Wrapping the link
    // back-write in try/catch pins the unwinder sp-save (stw r1) at the
    // skip-label - the closest reproducible position to retail's, which
    // schedules it one slot earlier (inside the final byte store's stall).
    try {
        slot->mLink0 = sentinel;
        // Retail reloads sentinel->prev instead of CSE-ing it.
        slot->mLink1 = sentinel->mPrev;
        ((CfReslistNode*)sentinel->mPrev)->mNext = (CfReslistNode*)slot;
        sentinel->mPrev = (CfReslistNode*)slot;
    } catch (...) {
        throw;
    }
}

// Target 1: us-800b1518 - reslist<cf::TboxInfo>::~reslist()
// Reinstalls the base vtable (lbl_eu_805290DC), unlinks every chained node
// (nulling each node's next as walked), resets the sentinel links, frees the
// pool array when the ownership flag (0x34) is clear, and clears the pool
// pointer (0x2c). Retail compiles this body independently of the base dtor.
// auto_inline off keeps ~UnkClass_805764CC's explicit member call out-of-line
// (the visible body would otherwise be folded into the caller).
#pragma push
#pragma auto_inline off
reslist<cf::TboxInfo>::~reslist() {
    if (this != 0) {
    extern void* lbl_eu_805290DC[];
    TboxInfoReslistLayout& obj = *(TboxInfoReslistLayout*)this;
    obj.mVtable = (void*)lbl_eu_805290DC;
    CfReslistNode* node = obj.mStartNodePtr->mNext;
    // The sentinel pointer is re-read from the object each iteration
    // (the node stores may alias it), so it is not cached in a local.
    // Comparison written sentinel-first to mirror retail's cmpl operand
    // order under MWCC's register assignment.
    while (obj.mStartNodePtr != node) {
        CfReslistNode* cur = node;
        node = node->mNext;
        cur->mNext = NULL;
    }
    obj.mStartNodePtr->mNext = obj.mStartNodePtr;
    obj.mStartNodePtr->mPrev = obj.mStartNodePtr;
    if (obj.field_0x34 == 0) {
        char* pool = (char*)obj.field_0x2c;
        // Pool base is stored biased by 0x10 (header before array); retail
        // emits two identical null tests around the free.
        if (pool != 0) {
            if (pool != 0) {
                __dla__FPv(pool - 0x10);
            }
            obj.field_0x2c = 0;
        }
    }
    }
}
#pragma pop
// Target 3: us-800b1614 - _reslist_base<cf::IFactoryEvent*>::~_reslist_base()
// Reinstalls the base vtable, unlinks every node (next=0), re-links the
// sentinel to itself, then frees the owned pool buffer (mList) unless the
// ownership flag at 0x1C is set. The null-check and flags-delete tails are
// MWCC-generated for member destructors; the goto/check loop mirrors the
// matched func_800B1A5C shape (retail re-reads field_0x04 every iteration).
template <>
_reslist_base<cf::IFactoryEvent*>::~_reslist_base() {
    extern void* lbl_eu_805290B8[];
    CfReslistLayout* obj = (CfReslistLayout*)this;
    *(void* volatile*)&obj->mVtable = (void*)lbl_eu_805290B8;
    CfReslistNode* zero = NULL;
    CfReslistNode* sentinel;
    CfReslistNode* cur;
    CfReslistNode* p;
    sentinel = obj->mStartNodePtr;
    cur = (CfReslistNode*)sentinel->mNext;
    goto check;
loop:
    p = cur;
    cur = (CfReslistNode*)cur->mNext;
    p->mNext = zero;
check:
    sentinel = obj->mStartNodePtr;
    if (cur != sentinel) goto loop;
    obj->mStartNodePtr->mNext = obj->mStartNodePtr;
    obj->mStartNodePtr->mPrev = obj->mStartNodePtr;
    if (!obj->field_0x1C) {
        if (obj->mList != 0) {
            __dla__FPv(obj->mList);
            obj->mList = 0;
        }
    }
}
// reslist<cf::IFactoryEvent*>::~reslist() - retail compiles the derived
// specialization as its own full teardown copy (not a tail-call to the base
// dtor), identical in shape to ~_reslist_base<cf::IFactoryEvent*>.
// auto_inline off keeps ~UnkClass_805764CC's explicit member call out-of-line.
#pragma push
#pragma auto_inline off
template <>
reslist<cf::IFactoryEvent*>::~reslist() {
    extern void* lbl_eu_805290B8[];
    CfReslistLayout* obj = (CfReslistLayout*)this;
    *(void* volatile*)&obj->mVtable = (void*)lbl_eu_805290B8;
    CfReslistNode* zero = NULL;
    CfReslistNode* sentinel;
    CfReslistNode* cur;
    CfReslistNode* p;
    sentinel = obj->mStartNodePtr;
    cur = (CfReslistNode*)sentinel->mNext;
    goto check;
loop:
    p = cur;
    cur = (CfReslistNode*)cur->mNext;
    p->mNext = zero;
check:
    sentinel = obj->mStartNodePtr;
    if (cur != sentinel) goto loop;
    obj->mStartNodePtr->mNext = obj->mStartNodePtr;
    obj->mStartNodePtr->mPrev = obj->mStartNodePtr;
    if (!obj->field_0x1C) {
        if (obj->mList != 0) {
            __dla__FPv(obj->mList);
            obj->mList = 0;
        }
    }
}
#pragma pop

// Target 3: us-800ba2a8 - func_800B998C
void* func_800B998C(void* self, void* a1, void* a2, void* a3, void* a4, void* a5) {
    return func_800B47A8((void*)1, self, a1, a2, a3, a4, a5);
}

// Target 4: us-800ba2d8 - func_800B99BC
void* func_800B99BC(void* self, void* a1, void* a2, void* a3, void* a4, void* a5) {
    return func_800B47A8((void*)0, self, a1, a2, a3, a4, a5);
}

// Target: us-800ba308 - func_800B99EC: walk the sentinel list at +4 and
// unlink every node whose +8 field equals *(u32*)arg.
extern "C" void func_800B99EC(void* list, void* arg) {
    u32 sentinel = *(u32*)((u32*)list + 1);
    u32 next;
    u32 cur = *(u32*)sentinel;
    u32 zero = 0;
    while (cur != sentinel) {
        next = *(u32*)cur;
        if (*(u32*)(cur + 8) == *(u32*)arg) {
            u32 prev = *(u32*)(cur + 4);
            *(u32*)prev = next;
            *(u32*)(next + 4) = prev;
            *(u32*)cur = zero;
        }
        cur = next;
    }
}

// Target 5: us-800ba35c - sinit_800B9A40
extern "C" void sinit_800B9A40() {
    extern float lbl_eu_80661CC8, lbl_eu_80661CCC, lbl_eu_80661CD0;
    extern float lbl_eu_80663EC8, lbl_eu_80663ECC, lbl_eu_80663ED0, lbl_eu_80663ED4;
    extern u32 lbl_eu_80663EE0;
    float e = lbl_eu_80661CD0;
    float c = lbl_eu_80661CC8;
    float c2 = c * c;
    float d = lbl_eu_80661CCC;
    lbl_eu_80663EC8 = c;
    lbl_eu_80663ECC = c2;
    lbl_eu_80663ED0 = e;
    lbl_eu_80663ED4 = d * d;
    lbl_eu_80663EE0 = 0;
}

// Target 4: us-800b1da0 - deref field at +0x0 and add 8
#pragma push
#pragma auto_inline off
u32 func_800B14D4(u8* self) {
    return *(u32*)self + 8;
}
#pragma pop
// Target 5: us-800b1dc8 - compare two u32 for inequality (dual-subf/or/srwi idiom)
u32 func_800B14FC(int* a, int* b) {
    int va = *a;
    int vb = *b;
    return va != vb;
}
// Target 1: us-800b35f4 - accessor returning field at +0x4
// auto_inline off: retail calls this out-of-line (e.g. from func_800B97A0).
#pragma push
#pragma auto_inline off
extern "C" u32 func_800B2D28(u8* self) {
    return *(u32*)(self + 0x4);
}
#pragma pop
// Target 5: us-800b3aa0 - extract bit 7 of field at +0x64
// auto_inline off: retail calls this out-of-line from func_800B4120.
// extern "C": retail exports the unmangled name.
#pragma push
#pragma auto_inline off
extern "C" u32 func_800B31D4(u8* self) {
    return (*(u32*)(self + 0x64) >> 7) & 1;
}
#pragma pop
// Target 1: us-800b4630 - return bit 31 of field at +0x64
u32 func_800B3D34(u8* self) {
    return (*(u32*)(self + 0x64) >> 31) & 1;
}
// Target 2: us-800b463c - return bit 27 of field at +0x6C
u32 func_800B3D40(u8* self) {
    return (*(u32*)(self + 0x6C) >> 27) & 1;
}
// Target us-800b5320: func_800B4A24
// Rejects null / non-enabled args, then checks whether the parent container's
// field_0x15F0 type id lies in {4,5,6,7,8}. Retail presets four flags to 1
// then clears them in a cascading chain (each stage only re-tests when the
// previous flag cleared).
extern "C" s32 func_800B4A24(CEvtTypeArg* arg) {
    s32 result = 0;
    if (arg != 0) {
        if (arg->unk7E() != 0 && (arg->flags & 4) != 0) {  // vtable +0x200
            // Recover the enclosing object (arg sits at +0x3E9C within it);
            // retail keeps a redundant null guard on the fixup.
            UnkClass_805764CC* container = (UnkClass_805764CC*)arg;
            if (container != 0) {
                container = (UnkClass_805764CC*)((u8*)container - 0x3E9C);
            }
            // Retail presets four flags to 1, clearing each in cascade.
            // Declaration order drives MWCC's volatile coloring (free-list
            // order r0,r3,r4,...): f0->r0, f3->r3, f4->r4, f5->r5, value->r6.
            int f0 = 1;
            int f3 = 1;
            int f4 = 1;
            int f5 = 1;
            s32 value = (s32)container->field_0x15F0;
            if (value != 4 && value != 5) {
                f5 = 0;
            }
            if (f5 == 0 && value != 6) {
                f4 = 0;
            }
            if (f4 == 0 && value != 7) {
                f3 = 0;
            }
            if (f3 == 0 && value != 8) {
                f0 = 0;
            }
            if (f0 != 0) {
                result = 1;
            }
        }
    }
    return result;
}
// Target 4: us-800b5868 - clear bit 0 and set bit 1 of the field at +0x6C
void func_800B4F6C(u8* self) {
    *(u32*)(self + 0x6C) = (*(u32*)(self + 0x6C) & ~1u) | 2u;
}
// Target 2: us-800b58a0 - accessor returning field at +0x70
u32 func_800B4FA4(u8* self) {
    return *(u32*)(self + 0x70);
}
// Target 3: us-800b6dcc - return mask & field at +0x6C
u32 func_800B64D0(u8* self, u32 mask) {
    return mask & *(u32*)(self + 0x6C);
}
// Target 2: us-800b3654 - func_800B2D88
// Thunk: copy arg to a stack local, then forward its address to func_800B2DB0.
// auto_inline off: retail calls this out-of-line from func_800B4400 etc.
#pragma push
#pragma auto_inline off
void func_800B2D88(void* self, void* arg) {
    func_800B2DB0((FactoryPoolList*)self, &arg);
}
#pragma pop

// Target 1: us-800b9bf4 - func_800B92D8
// Wrapper: fetch the singleton and forward it to func_800B15A4.
void func_800B92D8() {
    func_800B15A4(func_800B07E8());
}

// Target 4: us-800b9cec - func_800B93D0
// Wrapper: fetch the singleton and forward (singleton, obj) to func_800B1E18.
void func_800B93D0(void* obj) {
    func_800B1E18(func_800B07E8(), obj);
}

// Target 5: us-800b9d20 - func_800B9404
// Wrapper: fetch the singleton and forward (singleton, obj) to func_800B3A88.
// auto_inline off: retail calls this out-of-line from func_800B75EC.
#pragma push
#pragma auto_inline off
void func_800B9404(void* obj) {
    func_800B3A88(func_800B07E8(), obj);
}
#pragma pop

// Target 3: us-800b7978 - func_800B7058
// Wrapper: fetch the singleton and insert obj into it via func_800B6DD0.
void func_800B7058(void* obj) {
    func_800B6DD0(func_800B07E8(), obj);
}

// ---------------------------------------------------------------------------
// Batch targets: us-800b18c0 / us-800b25a8 / us-800b77e0 / us-800b1c48 / us-800b9e98

// Target 1: us-800b18c0 - func_800B0FF4
// Allocate the IFactoryEvent pool (count * 0xc) and park it on the reslist,
// clear every node's next link (MWCC unrolls the store loop x8), then store
// the entry count. The loop indexes through the list member so MWCC reloads
// the pool base each iteration (the stores may alias it).
// OPEN ITEM: best shape is structural:0 / 29 pure reg-swaps / size match.
// The {pool-reload, zero-value, loop-index} vregs color cyclically shifted
// vs retail (mine acc=r3/base=r4/val=r5; retail acc=r5/base=r3/val=r4).
// Tried: signed vs unsigned index, named fill/node temps, uninitialized decl,
// delayed init, extra bound var - coloring invariant. Next: element type with
// user-declared default ctor (new[]-style inline ctor loop) as the source form.
#pragma push
#pragma auto_inline off
extern "C" void func_800B0FF4(void* listv, unsigned long handle, unsigned long count) {
    reslist<cf::IFactoryEvent*>* list = (reslist<cf::IFactoryEvent*>*)listv;
    int i;
    list->field_0x14 = (u32)allocate_array__Q23mtl10MemManagerFUlUl(count * 0xc, handle);
    i = 0;
    while (i < (int)count) {
        ((CfReslistNode*)list->field_0x14)[i].mNext = 0;
        i++;
    }
    list->field_0x18 = (u32)count;
}
#pragma pop

// CfGameManager / player objects whose status callback lives at vtable
// slot 0x1D (+0x74).
extern "C" IDispB74* func_80083298__Q22cf13CfGameManagerFv();
extern "C" IDispB74* getPlayer__Q22cf13CfGameManagerFi(int index);
extern "C" bool CfRes_checkFlags_2000400();
extern "C" void func_80198108(int arg);
extern "C" void func_8007D190__Q22cf13CfGameManagerFv(void* arg);
extern "C" int testResInfoFlag(unsigned long mask);
extern "C" bool CfRes_getE24Bit18();

// Target 2: us-800b25a8 - func_800B1CDC
// Per-frame state driver: on a resource-reload flag, kick the reload helpers
// and clear the event word. Otherwise require game manager + player to report
// ready; bail out early on the scene-blockers, otherwise tear down and rebuild
// the optional sub-objects (with the E24-bit18 shortcut restarting the reload).
#pragma push
#pragma auto_inline off
extern "C" void func_800B1CDC(UnkClass_805764CC* self) {
    IDispB74* mgr = func_80083298__Q22cf13CfGameManagerFv();
    IDispB74* player = getPlayer__Q22cf13CfGameManagerFi(0);
    if (CfRes_checkFlags_2000400()) {
        func_80198108(1);
        func_8007D190__Q22cf13CfGameManagerFv((void*)0x80000000);
        self->field_0xD0E = 0;
        return;
    }
    if (mgr == 0 || player == 0 || !mgr->unk1D() || !player->unk1D()) {
        self->field_0xD0E = 0;
        return;
    }
    // Retail calls this import without materializing r3 (leftover from the
    // preceding virtual call).
    if (func_800829B8__Q22cf13CfGameManagerFv() != 0) {
        return;
    }
    if (testResInfoFlag(0x04000000)) {
        return;
    }
    // Retail forwards only self (r4 keeps the stale virtual-call result).
    ((void (*)(UnkClass_805764CC*))func_800B2048)(self);
    if (CfRes_getE24Bit18()) {
        func_80198108(1);
        func_8007D190__Q22cf13CfGameManagerFv((void*)0x80000000);
        self->field_0xD0E = 0;
        return;
    }
    func_800B1FD8(self);
    ((void (*)(UnkClass_805764CC*))func_800B20A0)(self);
    if (func_800B1C0C(2) == 0) {
        func_800B1F54(self);
    }
}
#pragma pop

// Ring-buffer accessors over the AD8 block (sorted by the func_800B39C0 key).
extern "C" unsigned long func_800B6FFC(void* ctx);
extern "C" unsigned long func_800B7004(void* ctx);
extern "C" void** func_800B700C(void* ctx, int index);

// Target 3: us-800b77e0 - func_800B6EC0
// Binary search over the sorted ring buffer at singleton+0x20 by the key
// returned from func_800B39C0; boundary elements are checked before the
// (unsigned) range rejection, then the classic lo/hi bisection runs.
extern "C" void* func_800B6EC0(UnkClass_805764CC* self, int id) {
    if (id == 0) {
        return 0;
    }
    UnkClass_805764CC* inst = func_800B07E8();
    void* ctx = (u8*)inst + 0x20;
    int lo = 0;
    unsigned long count = func_800B6FFC(ctx);
    func_800B7004(ctx);
    if (count == 0) {
        return 0;
    }
    void* entry = *func_800B700C(ctx, 0);
    if ((u32)func_800B39C0(entry) == (u32)id) {
        return entry;
    }
    if ((u32)id < (u32)func_800B39C0(entry)) {
        return 0;
    }
    entry = *func_800B700C(ctx, (int)count - 1);
    if ((u32)func_800B39C0(entry) == (u32)id) {
        return entry;
    }
    if ((u32)id > (u32)func_800B39C0(entry)) {
        return 0;
    }
    int hi = (int)count;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        entry = *func_800B700C(ctx, mid);
        int delta = (int)((u32)id - (u32)func_800B39C0(entry));
        if (delta == 0) {
            return entry;
        }
        if (delta < 0) {
            hi = mid - 1;
        }
        // Retail re-tests the same delta with a >= guard (blt-skip form).
        if (delta >= 0) {
            lo = mid + 1;
        }
    }
    return 0;
}

// Target 4: us-800b1c48 - func_800B137C
// Initialize the TboxInfo pool: allocate count*0x24 slots (+0x10 header) with
// the element constructor helper (__construct_new_array), clear every slot's
// front link (MWCC unrolls the store loop x8 - the loop indexes through the
// member so the base pointer is reloaded each iteration), then store count.
// OPEN ITEM: 6 structural + 29 reg-swaps at exact size match. Residuals:
// (a) retail passes the allocate() r3 straight into __construct_new_array and
//     commits the mSlots store AFTER the call; member-based source commits it
//     early (a local slots temp adds a saved-reg spill instead).
// (b) same cyclic vreg-coloring shift in the unrolled loop as func_800B0FF4.
extern "C" void __construct_new_array(void* block, void* dtor, unsigned long flag,
                                     unsigned long elemSize, unsigned long count);
extern "C" void func_800B0B8C();

#pragma push
#pragma auto_inline off
extern "C" void func_800B137C(void* self, unsigned long handle, unsigned long count) {
    TboxInfoReslistPoolView* list = (TboxInfoReslistPoolView*)self;
    __construct_new_array(
        list->mSlots = (TboxPoolSlot*)allocate_array__Q23mtl10MemManagerFUlUl(
            count * 0x24 + 0x10, handle),
        (void*)func_800B0B8C, 0, 0x24, count);
    for (int i = 0; i < (int)count; i++) {
        list->mSlots[i].mLink0 = 0;
    }
    list->mSlotCount = (u32)count;
}
#pragma pop

// Imports for func_800B957C (voice/actor spawn path).
extern "C" void* func_eu_80065640(unsigned long a, unsigned long b, unsigned long c, void* d);
extern "C" void* func_800B96C4(void* obj, void* arg);
extern "C" void func_800B96CC(void* obj, unsigned long a, unsigned long b);
extern "C" void func_800B96D8(u8* self, u32 val);
extern "C" void func_800B96E0(void* obj, void* arg);
extern "C" void func_800B96E8(void* obj, void* arg);
extern "C" void func_800B9704(void* obj, float val);
extern "C" void* func_80061FE8();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(unsigned long size, unsigned long handle);

// Target 5: us-800b9e98 - func_800B957C
// Spawn a voice/actor object: resolve two resources keyed by the tag, build a
// scratch record via the CfGameManager setters, create the object through
// func_800B20B4 (mask 0x100), populate its fields, heap-construct a 0x7c-byte
// child and hand it to the owner's vtable slot 0x1C.
extern "C" void* func_800B957C(void* tag, void* argB, void* argC, float val) {
    void* resA = func_eu_80065640(6, 0x53, 0, tag);
    void* resB = func_eu_80065640(0xa, 0x53, 0, tag);
    u8 buf[0x28];
    func_80081258__Q22cf13CfGameManagerFv(buf);
    func_80081264__Q22cf13CfGameManagerFv(buf, resA);
    func_800812F4__Q22cf13CfGameManagerFv(buf, (unsigned long)resB);
    UnkClass_805764CC* inst = func_800B07E8();
    void* obj = func_800B20B4(inst, 0x100, (const B20B4Payload*)buf, 0);
    if (obj != 0) {
        func_800B96C4(obj, tag);
        // Results land in saved regs unmasked; the (u16) zero-extension is
        // deferred to the 96CC argument setup like retail.
        u16 w, v;
        w = func_800B75BC();
        v = func_800B75B4();
        func_800B96CC(obj, v, w);
        func_800B96D8((u8*)obj, (u32)func_800B39C0(obj));
        func_800B96E0(obj, argB);
        func_800B96E8(obj, argC);
        func_800B9704(obj, val);
        void* src = func_80061FE8();
        void* child = allocate__Q23mtl10MemManagerFUlUl(0x7c, (unsigned long)src);
        if (child != 0) {
            child = __ct__800B970C(child);
        }
        ((IDispB970C*)obj)->unk1C(child);
    }
    return obj;
}
// Target 4: us-800b7c40 - func_800B7320
// Iterate the TboxInfo reslist at singleton->field_0xC48; for each node whose
// data's first word equals func_800B39C0(self), remove that node (via
// func_800B73E8 with a saved cursor) and return. The 4-byte list cursors are
// local u32s; the goto/check loop shape reproduces retail's layout (body
// first, head-check at the end, init jumps to the head). func_800B1AC0 is
// declared void* so MWCC reuses its r3 (== &saved) as arg3 of func_800B73E8
// exactly like retail's mr r5,r3.
extern "C" void func_800B7320(void* self) {
    if (self == 0) {
        return;
    }
    UnkClass_805764CC* singleton = func_800B07E8();
    u32 cursor;      // main cursor (r1+0x18)
    u32 tmp73E8;     // func_800B73E8 dst (r1+0x14)
    u32 saved;       // saved cursor for removal (r1+0x10)
    u32 tmpAA8;      // func_800B1AA8 dst (r1+0xc)
    u32 cursorEnd;   // sentinel cursor (r1+0x8)
    func_800B1A8C(&cursor, &singleton->field_0xC48);
    goto check;
loop:
    void* item = func_800B39C0(self);
    void* data = func_800B1A9C(&cursor);
    if (*(u32*)data == (u32)item) {
        void* cur = func_800B1AC0(&saved, &cursor);
        func_800B73E8(&tmp73E8, &singleton->field_0xC48, cur);
        return;
    } else {
        func_800B1AA8(&tmpAA8, &cursor, 0);
    }
check:
    func_800B1ACC(&cursorEnd, &singleton->field_0xC48);
    if (func_800B1AD8(&cursor, &cursorEnd) != 0) {
        goto loop;
    }
}
// Target 1: us-800b8334 - func_800B7A18
// Iterate the two circular object lists (func_800B6BC8 then func_800B6BEC)
// with the CfGameManager item-list iterator primitives; for each object whose
// type id ((u16)func_800BE93C) equals arg, trigger func_800BF2E0 on it.
extern "C" void func_800B7A18(s32 arg) {
    if (arg < 0) {
        return;
    }
    for (s32 i = 0; i < 2; i++) {
        void* list = i != 0 ? func_800B6BEC() : func_800B6BC8();
        F8C0IteratorNode iterator;
        func_8007F8C0__Q22cf13CfGameManagerFv(&iterator, (const F8C0ListSource*)list);
        F8C0IteratorNode previous;
        F8C0IteratorNode end;
        while (func_8007F8F4__Q22cf13CfGameManagerFv(&end, (const F8C0ListSource*)list),
               func_8007F900__Q22cf13CfGameManagerFv(&iterator.field_0x0, &end.field_0x0)) {
            void* object = *func_8007F8D0__Q22cf13CfGameManagerFv(&iterator);
            if (arg == (u16)func_800BE93C(object)) {
                func_800BF2E0(object);
            }
            func_8007F8DC__Q22cf13CfGameManagerFv(&previous, &iterator, 0);
        }
    }
}
extern "C" void func_800B87FC() {}
// Target us-800b9120: func_800B8804(self, event)
// Ensure the reslist pool @ field_0xC80 is set up, then look for an existing
// node whose data pointer equals `event`; if none, claim the first empty
// pool slot (entry[0]==0) and insert a new node holding `event` before the
// sentinel (head).
extern "C" void func_800B8804__FPvPQ22cf13IFactoryEvent(UnkClass_805764CC* self, cf::IFactoryEvent* event) {
    // Decl order drives Chaitin coloring: idx=r4, byteOff=r5, count=r6,
    // sentinel=r7 (retail).  Values are assigned in execution order below.
    int idx;
    u32 byteOff;
    int count;
    CFactoryEventPoolNode* sentinel;
    if (event == 0) {
        return;
    }
    func_800B0FA0(self);

    // Walk the node list at field_0xC80; node next/prev/data links.
    sentinel = (CFactoryEventPoolNode*)(*(u32*)((u8*)self + 0xc84));
    CFactoryEventPoolNode* node = sentinel->next;
    while (node != sentinel) {
        if (node->data == event) {
            return; // already registered
        }
        node = node->next;
    }

    // Claim the first empty slot in the fixed 0xc-byte-entry pool.  Retail
    // re-reads the pool base from self->[0xc94] every slot check (aliasing),
    // and keeps an explicit running byte-offset (r5) alongside the slot index
    // (r4): `lwzx r0,r3,r5` + `addi r5,r5,12` + `addi r4,r4,1`.
    count = *(int*)((u8*)self + 0xc98);
    idx = 0;
    byteOff = 0;
    while (idx < count) {
        if (*(u32*)(*(u8**)((u8*)self + 0xc94) + byteOff) == 0) {
            break;
        }
        byteOff += 0xc;
        idx++;
    }

    // Fill the new node and link it in just before the sentinel.  The data
    // write mirrors reslist::_reslist_node::setItem: the `&data` pointer is
    // null-checked and wrapped in try/catch, which is what forces retail's
    // frame pointer (mr r31,r1) + stw-r1 frame-anchor + the addic. null check.
    CFactoryEventPoolNode* newEntry = (CFactoryEventPoolNode*)(*(u8**)((u8*)self + 0xc94) + idx * 0xc);
    cf::IFactoryEvent** ptr = &newEntry->data;
    if (ptr != nullptr) {
        try {
            *ptr = event;
        } catch (...) {
            throw;
        }
    }
    newEntry->next = sentinel;
    newEntry->prev = sentinel->prev;
    sentinel->prev->next = newEntry;
    sentinel->prev = newEntry;
}
// Target 1: us-800b9d54 - func_800B9438
// Fetch the singleton and forward (singleton, arg) to func_800B4278.
void func_800B9438(void* arg) {
    func_800B4278(func_800B07E8(), (u32)arg);
}
// Target 2: us-800b9d88 - func_800B946C
// Fetch the singleton and forward (singleton, arg) to func_800B42E8.
void func_800B946C(void* arg) {
    func_800B42E8(func_800B07E8(), (u32)arg);
}
// Target 3: us-800b9dbc - func_800B94A0
// Fetch the singleton and forward (singleton, name) to func_800B4368.
void func_800B94A0(const char* name) {
    func_800B4368(func_800B07E8(), name);
}
// Target 4: us-800b9e64 - func_800B9548
// Fetch the singleton and forward (singleton, 0x8000, 0, 0) to func_800B20B4.
void func_800B9548() {
    func_800B20B4(func_800B07E8(), 0x8000, 0, 0);
}
// Target 3: us-800b9ff4 - setter storing val at +0x720
#pragma push
#pragma auto_inline off
void func_800B96D8(u8* self, u32 val) {
    *(u32*)(self + 0x720) = val;
}
#pragma pop

// Target 1: us-800b6e58 - func_800B655C
// Walk the game-manager item list; for each entry whose container passes the
// event-type check and whose name matches the split1 string at +0x47, reload
// its data record (guarded by the 0x802 flag test). The two resource-reload
// flags gate entry into the walk and pick the restart helper.
void func_800B655C(UnkClass_805764CC* self, const F8C0ListSource* list) {
    int big = testResInfoFlag(0x04000000) != 0;
    int small = testResInfoFlag(0x100) != 0;
    if (func_800829B8__Q22cf13CfGameManagerFv() == 0 || big != 0) {
        if (small == 0) {
        void* ctx = func_8006A6D0();
        getInstance__Q22cf14CBattleManagerFv();
        // Retail keeps three distinct iterator slots (init / step-back / end).
        F8C0IteratorNode outer;
        F8C0IteratorNode mid;
        F8C0IteratorNode end;
        func_8007F8C0__Q22cf13CfGameManagerFv(&outer, list);
        goto check;
    loop:
        // Node pointers sit 0x3e9c into their containers; retail keeps one
        // adjusted pointer and re-adds the offset at each use.
        void* p = *func_8007F8D0__Q22cf13CfGameManagerFv(&outer);
        if (p != 0) {
            p = (u8*)p - 0x3e9c;
        }
        func_8007F8DC__Q22cf13CfGameManagerFv(&mid, &outer, 0);
        if (func_800B64AC((u8*)p + 0x3e9c) == 0) {
            u8 matched = (u8)func_800AF7E4(p, lbl_eu_804FC4D8 + 0x47);
            if (matched != 0 && func_801949E0(ctx) == 0 &&
                func_8006DBD4(p, 0x802) != 0) {
                func_800BE824((u8*)p + 0x3e9c, 0);
                if (big != 0) {
                    func_800B66AC((u8*)p + 0x3e9c);
                } else {
                    func_800B64F8((u8*)p + 0x3e9c);
                }
            }
        }
    check:
        func_8007F8F4__Q22cf13CfGameManagerFv(&end, list);
        if (func_8007F900__Q22cf13CfGameManagerFv((const u32*)&outer, (const u32*)&end)) {
            goto loop;
        }
        }
    }
}

// Target 2: us-800b4a1c - func_800B4120
// Route an object into the reslist matching its type: the first predicate
// that fires selects the destination list.
#pragma push
#pragma auto_inline off
extern "C" void func_800B4120(UnkClass_805764CC* self, cf::CfObject* obj) {
    if (obj == 0) {
        return;
    }
    if (func_8006C1B0(obj)) {
        func_800B3A60(&self->field_0xB28, obj);
    } else if (func_800B31BC(obj)) {
        func_800B3A60(&self->field_0xC08, obj);
    } else if (func_8006DF9C(obj)) {
        func_800B3A60(&self->field_0xB48, obj);
    } else if (func_800B31C8(obj)) {
        func_800B3A60(&self->field_0xB68, obj);
    } else if (func_800B31D4((u8*)obj)) {
        func_800B3A60(&self->field_0xB88, obj);
    } else if (func_800B31E0(obj) || func_800B31EC(obj)) {
        func_800B3A60(&self->field_0xBE8, obj);
    } else if (func_800B31F8(obj) || func_800B3204(obj)) {
        func_800B3A60(&self->field_0xBC8, obj);
    } else if (func_800B31B0(obj)) {
        func_800B3A60(&self->field_0xBA8, obj);
    }
}
#pragma pop

// Target 3: us-800b6008 - func_800B570C
// Three-entry insertion step of a sort: order (a,b,c) according to the
// indirect comparator. Both probe results are taken up front; when exactly
// one reports equality the entries are rotated via compare(b,a).
// Swaps save through explicit temps so MWCC round-trips the value word
// through a stack spill like retail (see func_800B587C).
void func_800B570C(SortEntry* a, SortEntry* b, SortEntry* c, const SortEntryCompare* vt) {
    bool ca = (*vt)(c, a) == 0;
    bool bc = (*vt)(b, c) == 0;
    if (!ca) goto L1;
    if (bc) {
        return;
    }
L1:
    if (ca) goto L2;
    if (bc) goto L2;
    { SortEntry t = *a; *a = *b; *b = t; }
    return;
L2:
    if ((*vt)(b, a) != 0) {
        SortEntry t = *a; *a = *b; *b = t;
    }
    if (ca) {
        SortEntry t = *b; *b = *c; *c = t;
    } else {
        SortEntry t = *a; *a = *c; *c = t;
    }
}

// Target 4: us-800b8cc8 - func_800B83AC
// Per-party-member status refresh: for each occupied slot with a valid id,
// copy the source word into the status controller and either reset it (when
// the progress value reached the threshold or the lookup failed) or fire the
// trigger sub-object; finally mark the controller reloaded.
void func_800B83AC() {
    if (func_800829B8__Q22cf13CfGameManagerFv() != 0) {
        return;
    }
    if (func_8007F91C__Q22cf13CfGameManagerFv() == 0) {
        void* party = func_8009ECB0();
        const float threshold = lbl_eu_806669D8;
        // Retail walks a cursor based at party+4 and reads the slot at +0xc
        // from it (member words start at base+0x10).
        s32* it = (s32*)((u8*)party + 4);
        for (int i = 0; i < 6; i++) {
            s32 slotVal = it[3];
            if (slotVal > 0 && slotVal <= 13) {
                B83ACCharObj* chr = (B83ACCharObj*)func_8009EC9C((u16)slotVal);
                u32 src = *(u32*)((B83ACSrc180*)chr->field_180)->unk0C();
                if (func_80174C98(&chr->m17c, &src, 0x1c) == 0 && chr->m17c.unk74() <= threshold) {
                    chr->m17c.unk2D(1);
                    chr->m17c.unk2E();
                    chr->m17c.unk30();
                } else if (func_80148778(&chr->m184, 0x35) == 0) {
                    chr->m184.unk05(0x35);
                }
                CfRes_getD80Flag();
                func_80496288();
                func_801765A4(&chr->m17c, 1);
            }
            it++;
        }
    }
}

// Target 5: us-800b7d30 - func_800B7410
// Scan the TboxInfo list (rebuild trigger) for a free entry matching both language bytes and
// spawn a voice/actor object from it; the spawn result is written back over
// the consumed entry.
void func_800B7410() {
    UnkClass_805764CC* ctx = func_800B07E8();
    u16 langA = func_800B75B4();
    u16 langB = func_800B75BC();
    u32 cursor;
    u32 nextCur;
    u32 endCur;
    func_800B1A8C(&cursor, &ctx->field_0xC48);
    goto check;
loop:
    // Retail re-reads the current entry through func_800B1A9C once per test;
    // each failed guard jumps straight to the iterator advance.
    if (*(u32*)func_800B1A9C(&cursor) != 0) {
        goto advance;
    }
    if ((u16)langA != ((B7410Node*)func_800B1A9C(&cursor))->field_1A) {
        goto advance;
    }
    if ((u16)langB != ((B7410Node*)func_800B1A9C(&cursor))->field_1B) {
        goto advance;
    }
    {
        B7410Node* nVol = (B7410Node*)func_800B1A9C(&cursor);
        B7410Node* nTag = (B7410Node*)func_800B1A9C(&cursor);
        B7410Node* nId = (B7410Node*)func_800B1A9C(&cursor);
        B7410Node* nSrc = (B7410Node*)func_800B1A9C(&cursor);
        IDispB7410* obj = (IDispB7410*)func_800B957C(
            (void*)nSrc->field_14, (void*)(u32)nId->field_18,
            (u8*)nTag + 4, nVol->field_10);
        // Retail uses the spawn result without a null guard.
        obj->unk39((u8*)func_800B1A9C(&cursor) + 4);
        obj->unk49(((B7410Node*)func_800B1A9C(&cursor))->field_10);
        obj->unk55(lbl_eu_80666A10);
        func_800B75C4(obj, 0x20000000, 1);
        func_800BC3B0(obj, lbl_eu_80666A08);
        *(void**)func_800B1A9C(&cursor) = func_800B39C0(obj);
    }
advance:
    func_800B1AA8(&nextCur, &cursor, 0);
check:
    func_800B1ACC(&endCur, &ctx->field_0xC48);
    if (func_800B1AD8(&cursor, &endCur)) {
        goto loop;
    }
}

// ---------------------------------------------------------------------------
// Batch targets: us-800b5680 / us-800b4e9c / us-800b19ec / us-800b1e70 / us-800b9678

// Target 1: us-800b5680 - func_800B4D84
// Post-processing pass over the sorted scratch buffer built by func_800B5994:
// sort it (func_800B4FAC with the func_800B6544 comparator), run the
// resource-flag side effects once, then walk the entries. Each live entry is
// either flagged via func_800B4F6C/func_800B4F90 (depending on the D10 gate
// and the running count), and its id (+0x70 value) is appended to the local
// dedup list unless already present; the count accumulates the per-entry
// weight from func_eu_800BFC7C.
void func_800B4D84(UnkClass_805764CC* self, void* buf) {
    void** end = func_800B5948(buf);
    func_800B4FAC(func_800B5944(buf), end, func_800B6544);
    testResInfoFlag(0x10);
    testResInfoFlag(0x40);
    if (getUnk80664658() != 0) {
        func_800B4F58(getUnk80664658());
    }
    testResInfoFlag(0x20);
    int hasVal = (s16)self->field_0xD10 != 0;
    u32 lateFlag = 0;
    if (func_800B1C40() != 0) {
        // Retail clrlwi's the getter result before the unsigned < 4 compare.
        if ((u32)func_800B4F64() >= 4) {
            lateFlag = 1;
        }
    }
    // Dedup list + count cursor; func_800B5978 writes at buf+0x380, so the
    // scratch must span that far (retail frame: spill @ sp+0x8, it @ sp+0xc).
    u32 v;
    u8 itBuf[0x384];
    u32 count = 0;
    func_800B5958(itBuf);
    func_800A8C84();
    void** cur = func_800B5944(buf);
    while (cur != func_800B5948(buf)) {
        void* obj = *cur;
        if (func_80082FCC__Q22cf13CfGameManagerFv(obj) != 0) {
            if (hasVal != 0 || count >= 8) {
                if (func_800B3D4C(obj, 1) != 0) {
                    func_800B4F6C(obj);
                }
            } else {
                if (func_800B3D4C(obj, 1) == 0) {
                    func_800B4F90(obj);
                }
            }
            if (count == 0 && lateFlag != 0) {
                count = 8;
            }
            v = func_800B4FA4(obj);
            if (v != 0) {
                u32 found = 0;
                for (u32 i = 0; i < func_800B5970(itBuf); i++) {
                    if (*(u32*)func_800B5964(itBuf, i) == v) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    func_800B5978((UnkClass_805764CC*)itBuf, &v);
                }
                count += func_eu_800BFC7C(obj);
            }
        }
        cur++;
    }
}

// Inlined by func_800B45A0 once per list (retail emits four copies).
static bool listReady45A0(void* list) {
    CfReslistNode* cur = ((CfReslistNode*)*(u32*)((u8*)list + 4))->mNext;
    while (cur != *(CfReslistNode**)((u8*)list + 4)) {
        B45A0ObjView* data = (B45A0ObjView*)cur->mItem;
        if ((data->field_64 & 0x10000) != 0 && !data->vf1D()) {
            return false;
        }
        cur = cur->mNext;
    }
    return true;
}

// Target 2: us-800b4e9c - func_800B45A0
// Validity check across four object lists (the B28-family heads): every
// entry whose +0x64 flag word has bit 16 set must report ready through its
// slot-0x1D callback. Any failure aborts with false.
bool func_800B45A0() {
    if (!lbl_eu_80663EE8) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }
    if (!listReady45A0(func_800B6BA4__Fv())) {
        return false;
    }
    if (!listReady45A0(func_800B6BC8())) {
        return false;
    }
    if (!listReady45A0(func_800B6BEC())) {
        return false;
    }
    if (!listReady45A0(func_800B6C34())) {
        return false;
    }
    return true;
}

// Target 3: us-800b19ec - func_800B1120
// Singleton construction body: size every reslist pool, build the TboxInfo
// pool, heap-construct the six optional sub-managers, clear the state words,
// then drain the IFactoryEvent queue firing each event's slot-5 callback.
void func_800B1120(UnkClass_805764CC* self) {
    func_800B084C(self, 0x2be);
    func_800B084C((UnkClass_805764CC*)&self->field_0xBA8, 2);
    func_800B084C((UnkClass_805764CC*)&self->field_0xB28, 9);
    func_800B084C((UnkClass_805764CC*)&self->field_0xB48, 0x60);
    func_800B084C((UnkClass_805764CC*)&self->field_0xB68, 0x80);
    func_800B084C((UnkClass_805764CC*)&self->field_0xB88, 0x40);
    func_800B084C((UnkClass_805764CC*)&self->field_0xBE8, 0x100);
    func_800B084C((UnkClass_805764CC*)&self->field_0xBC8, 0x80);
    func_800B084C((UnkClass_805764CC*)&self->field_0xC08, 0x14);
    func_800B084C((UnkClass_805764CC*)&self->field_0xC28, 0x80);
    func_800B137C(&self->field_0xC48, func_80061FFC(), 0x14);
    func_800B0B40(&self->field_0x20);

    // Retail relies on each ctor returning its argument (r3) so the store
    // sits on the join path without a saved temp.
    void* p = allocate__Q23mtl10MemManagerFUlUl(0xB27C, func_80061FFC());
    self->field_0xCA0 =
        (u32)(p != 0 ? __ct__80193270(p) : p);
    p = allocate__Q23mtl10MemManagerFUlUl(0x3808, func_80061FFC());
    self->field_0xCA4 =
        (u32)(p != 0 ? __ct__cf_CfMapItemManager(p) : p);
    p = allocate__Q23mtl10MemManagerFUlUl(0x3808, func_80061FFC());
    self->field_0xCA8 =
        (u32)(p != 0 ? __ct__cf_CfValueItemManager(p) : p);
    p = allocate__Q23mtl10MemManagerFUlUl(0x220, func_80061FFC());
    self->field_0xCFC =
        (u32)(p != 0 ? __ct__801F3BE8(p) : p);
    p = allocate__Q23mtl10MemManagerFUlUl(0x11a4, func_80061FFC());
    self->field_0xD00 =
        (u32)(p != 0 ? __ct__cf_CfHikariItemManager(p) : p);
    p = allocate__Q23mtl10MemManagerFUlUl(0x554, func_80061FFC());
    self->field_0xCAC =
        (u32)(p != 0 ? __ct__80205A7C(p) : p);

    self->field_0xD0E = 0;
    self->field_0xD10 = 0;
    self->field_0xD04 = 0;
    lbl_eu_80663EE4 = 0;
    func_800B4588(self);
    func_80063160(-1);
    func_800631FC(-1);
    func_800B1368(1);
    func_800B0FA0(self);

    // Drain the IFactoryEvent queue, invoking slot 5 on each entry.
    u32 it[3];
    u32 endCur;
    func_800B14C4(it, &self->field_0xC80);
    goto check;
loop:
    ((IFeEventBase*)*func_800B14D4(it))->vf05();
    func_800B14E0(it);
check:
    func_800B14F0(&endCur, &self->field_0xC80);
    if (func_800B14FC(it, &endCur) != 0) {
        goto loop;
    }
}

// Target 4: us-800b1e70 - func_800B15A4
// Singleton teardown: release each optional sub-manager (each null-guarded,
// field_0xD00 via its slot-2 reset), re-file every game-manager object into
// the matching reslist while nulling the queue slots, destroy all reslists
// in reverse construction order, then drain the IFactoryEvent queue firing
// slot 6 and destroy the queue itself.
void func_800B15A4(UnkClass_805764CC* self) {
    // Local block laid out to mirror retail stack slots.
    u32 d00;
    F8C0IteratorNode outer;
    u32 it;
    F8C0IteratorNode mid;
    F8C0IteratorNode end;
    u32 itEnd;

    if (self->field_0xCAC != 0) {
        __dt__80206170((void*)self->field_0xCAC, 1);
        self->field_0xCAC = 0;
    }
    if (self->field_0xCA8 != 0) {
        __dt__801732F8((void*)self->field_0xCA8, 1);
        self->field_0xCA8 = 0;
    }
    if (self->field_0xCA4 != 0) {
        __dt__801732F8((void*)self->field_0xCA4, 1);
        self->field_0xCA4 = 0;
    }
    if (self->field_0xCA0 != 0) {
        __dt__80193538((void*)self->field_0xCA0, 1);
        self->field_0xCA0 = 0;
    }
    if (self->field_0xCFC != 0) {
        __dt__801F3C08((void*)self->field_0xCFC, 1);
        self->field_0xCFC = 0;
    }
    // Retail loads field_0xD00 once and keeps a redundant second null
    // guard around the dispatch.
    d00 = self->field_0xD00;
    if (d00 != 0) {
        if (d00 != 0) {
            ((IDispD00*)d00)->vf02(1);
        }
        self->field_0xD00 = 0;
    }

    // Walk the game-manager item list: file each object into this singleton
    // via func_800B4120, fire its slot-21 reset, and clear the queue slot.
    func_8007F8C0__Q22cf13CfGameManagerFv(&outer, (const F8C0ListSource*)self);
    goto check;
body:
    if (*func_8007F8D0__Q22cf13CfGameManagerFv(&outer) == 0) {
        goto advance;
    }
    func_800B4120(self, (cf::CfObject*)*func_8007F8D0__Q22cf13CfGameManagerFv(&outer));
    if (*func_8007F8D0__Q22cf13CfGameManagerFv(&outer) == 0) {
        goto advance;
    }
    {
        IDispB15A4* disp = (IDispB15A4*)*func_8007F8D0__Q22cf13CfGameManagerFv(&outer);
        // Retail keeps a redundant null guard ahead of the dispatch.
        if (disp != 0) {
            disp->vf15(1);
        }
    }
    *func_8007F8D0__Q22cf13CfGameManagerFv(&outer) = 0;
advance:
    func_8007F8DC__Q22cf13CfGameManagerFv(&mid, &outer, 0);
check:
    func_8007F8F4__Q22cf13CfGameManagerFv(&end, (const F8C0ListSource*)self);
    if (func_8007F900__Q22cf13CfGameManagerFv((const u32*)&outer, (const u32*)&end) != 0) {
        goto body;
    }

    // Destroy every reslist in reverse construction order.
    func_800B1518(&self->field_0xC28);
    func_800B1518(&self->field_0xC08);
    func_800B1518(&self->field_0xBC8);
    func_800B1518(&self->field_0xBE8);
    func_800B1518(&self->field_0xB88);
    func_800B1518(&self->field_0xB68);
    func_800B1518(&self->field_0xB48);
    func_800B1518(&self->field_0xB28);
    func_800B1518(&self->field_0xBA8);
    func_800B1518(self);
    func_800B0B40(&self->field_0x20);
    __dt__800B183C(&self->field_0xC48);
    func_800B1808();

    // Drain the queue firing slot 6, then destroy the queue list.
    func_800B14C4(&it, &self->field_0xC80);
    goto check2;
loop2:
    ((IFeEventBase*)*func_800B14D4(&it))->vf06();
    func_800B14E0(&it);
check2:
    func_800B14F0(&itEnd, &self->field_0xC80);
    if (func_800B14FC(&it, &itEnd) != 0) {
        goto loop2;
    }
    __dt__800B18CC((FactoryPoolList*)&self->field_0xC80);
}

// Target 5: us-800b9678 - func_800B8D5C
extern u32 lbl_eu_80663E24;
extern u32 lbl_eu_80663E28;
// Battle-readiness scan over the B28 object list: resolve each node payload
// to its container, skip blocked ones (flag bits / slot-0x1D status), then
// depending on the container's 0x15F0 type id either run the not-in-battle
// checks (returning success when one passes) or the in-battle dispatch chain
// comparing the slot-0x5B float against the shared threshold. On full-scan
// failure a retry counter at singleton+0xD0E is bumped (up to 2).
int func_800B8D5C() {
    // rlwinm field masks: bits [9,9] / [27,27] (MSB-numbered).
    if ((lbl_eu_80663E24 & 0x400000) != 0) {
        CfRes_callFunc_68254();
    }
    if ((lbl_eu_80663E28 & 0x10) != 0 && func_800B1C40() != 0) {
        return 0;
    }
    if (!lbl_eu_80663EE8) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv, lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }
    u8* inst = lbl_eu_80572CD4;
    if (*(u32*)(inst + 0xCA0) != 0) {
        func_80195E5C(*(u32*)(inst + 0xCA0), lbl_eu_80663EC8);
    }
    void* list = func_800B6BC8();
    const float threshold = lbl_eu_806669D8;
    CfReslistNode* cur = ((CfReslistNode*)*(u32*)((u8*)list + 4))->mNext;
    // Sentinel re-read from the list header every iteration; the cursor
    // advance sits at the single continue-target (retail label .L_800B988C).
    while (cur != *(CfReslistNode**)((u8*)list + 4)) {
        B8D5CObj* obj = (B8D5CObj*)func_800AD860__FPv(cur->mItem);
        if (obj == 0) {
            goto advance;
        }
        if (((obj->field_3F04 & 0x40) != 0) || ((obj->field_3F04 & 0x20) != 0)) {
            goto advance;
        }
        if (obj->field_3E9C.vf1C() != 0) {
            goto advance;
        }
        // Retail presets four flags via li and clears each stage.
        u32 v = obj->field_15F0;
        int a = (v == 4 || v == 5) ? 1 : 0;
        int b = a;
        if (b == 0 && v != 6) {
            b = 0;
        }
        int c = b;
        if (c == 0 && v != 7) {
            c = 0;
        }
        int d = c;
        if (d == 0 && v != 8) {
            d = 0;
        }
        if (d == 0) {
            // Not in battle: require the flag triple, a clean validator and
            // an empty 3F60 pointer to report success.
            if ((obj->field_3F08 & 0x40000000) != 0) {
                goto advance;
            }
            if ((obj->field_3F04 & 0x100000) == 0) {
                goto advance;
            }
            if ((obj->field_3F08 & 1) == 0) {
                goto advance;
            }
            if (obj->field_3E9C.vf58() != 0) {
                goto advance;
            }
            if (obj->field_3F60 != 0) {
                goto advance;
            }
            return 0;
        } else {
            // In battle: run reset + validate; on validator success compare
            // the slot-0x5B float against the threshold.
            if (obj->field_3E9C.vf1D() != 0) {
                goto advance;
            }
            if (obj->field_3E9C.vf58() == 0) {
                return 0;
            }
            if (threshold == obj->field_3E9C.vf5B()) {
                goto advance;
            }
            return 0;
        }
    advance:
        cur = cur->mNext;
    }
    u16 cnt = *(u16*)(inst + 0xD0E);
    if (cnt >= 2) {
        return 1;
    }
    *(u16*)(inst + 0xD0E) = cnt + 1;
    return 0;
}

// ---------------------------------------------------------------------------
// Batch targets: us-800b71a4 / us-800b50a4 / us-800b8e40 / us-800b4354 / us-800b379c

// Target: us-800b71a4 - func_800B68A8
// OPEN ITEM: 121 mismatch / 100 structural. Residuals: (a) float-color shift -
// retail colors consts D8->f27, DC->f31, EC->f30, A0C->f29, limit->f26; decomp
// assigns them shifted down by 2 (try reordering the four const declarations
// so the longest-lived constant is declared last); (b) FP-save block emitted
// before `addi r11` frame-pointer setup instead of after.
// Per-object update over a game-manager list. Entries are filtered (busy check
// via func_800B4594, resource-flag 0x100); with flag==0 the object's slot-0x174
// level gates a func_800BC3B0 boost; otherwise the partner distance drives a
// fade toward the shared cap before func_800BE824 commits the result.
extern "C" void func_800B68A8(UnkClass_805764CC* self, void* partner,
                              const void* listv, int flag, float limit) {
    const F8C0ListSource* list = (const F8C0ListSource*)listv;
    const float zero = lbl_eu_806669D8;
    const float one = lbl_eu_806669DC;
    const float gainK = lbl_eu_806669EC;
    const float rangeK = lbl_eu_80666A0C;
    F8C0IteratorNode outer;
    F8C0IteratorNode stepBack;
    F8C0IteratorNode end;
    u8 distBuf[0x10];
    func_8007F8C0__Q22cf13CfGameManagerFv(&outer, list);
    for (;;) {
        cf::CfObject* obj = (cf::CfObject*)*func_8007F8D0__Q22cf13CfGameManagerFv(&outer);
        func_8007F8DC__Q22cf13CfGameManagerFv(&stepBack, &outer, 0);
        if (func_800B4594(obj) == 0 && func_8006EF04(0x100) != 0) {
            int apply = 1;
            if (flag == 0) {
                // Level gate: only boost objects above the zero level.
                if (func_800B64B8(obj, 0x10) != 0) {
                    float level = ((IB68Obj*)obj)->unk5D();
                    if (level > zero) {
                        func_800BC3B0(obj, lbl_eu_80666A08);
                    } else {
                        apply = 0;
                    }
                } else {
                    apply = 0;
                }
            } else if (partner != 0) {
                void* objPos = ((IB68Obj*)obj)->unk2B();
                void* partnerPos = ((IB68Obj*)partner)->unk2B();
                func_8004CB80(distBuf, partnerPos, objPos);
                float dist = func_80073F88(distBuf);
                if (dist < limit) {
                    apply = 0;
                    if (func_800B64B8(obj, 0xC00) == 0 && func_800B64B8(obj, 0x10) != 0) {
                        float gap = limit - dist;
                        if (gap < rangeK) {
                            // Inside the range ramp: fade volume down from one.
                            float fade = gainK * (rangeK - gap);
                            if (fade < zero) {
                                fade = zero;
                            }
                            ((IB68Obj*)obj)->unk5A(one - fade);
                        } else if (((IB68Obj*)obj)->unk5B() < one) {
                            ((IB68Obj*)obj)->unk5A(one);
                        }
                    }
                }
            }
            func_800BE824(obj, apply);
        }
        func_8007F8F4__Q22cf13CfGameManagerFv(&end, list);
        if (func_8007F900__Q22cf13CfGameManagerFv((const u32*)&outer, (const u32*)&end) == 0) {
            break;
        }
    }
}

// Target: us-800b50a4 - func_800B47A8
// OPEN ITEM: 154 mismatch / 142 structural. Residuals: (a) retail saves
// r26+ via _savegpr_26 (6 int params live across calls); decomp keeps a
// smaller live set -> needs one more persistent value or different param
// liveness; (b) Warning string args: retail first arg relocs lbl_eu_80526300
// +1085 addend, mine lbl_eu_80526324 - swap which label is named vs offset;
// (c) FrSqrt/Warning call ordering vs the distSq compare block.
// Distance/audibility test between two positions: computes the horizontal
// delta magnitude (with a degenerate-magnitude warning and FrSqrt), applies
// the scale factor, early-outs on the absolute Y delta against the tuned
// limit, optionally feeds shifted positions to the listener callback, then
// reports whether the distance is under the scaled limit.
extern "C" u32 func_800B47A8(int modeScale, float scale, const B47Vec3* posA,
                             const B47Vec3* posB, int modeFade, int modeNear,
                             float* outDist, u8* outHit) {
    const float zero = lbl_eu_806669D8;
    const float one = lbl_eu_806669DC;
    B47Vec3 delta;
    delta.x = posB->x - posA->x;
    delta.y = posB->y - posA->y;
    delta.z = posB->z - posA->z;
    if (modeScale != 0) {
        scale *= lbl_eu_806669E4;
    }
    // Horizontal magnitude only (x/z); retail warns on a negative square sum.
    float distSq = delta.x * delta.x + delta.z * delta.z;
    if (distSq < zero) {
        nw4r::db::Warning(lbl_eu_80526324, 0x273, lbl_eu_80526300);
    }
    float dist;
    if (distSq > zero) {
        dist = distSq * nw4r::math::FrSqrt(distSq);
    } else {
        dist = distSq;
    }
    *outDist = dist;
    float absY = delta.y < zero ? -delta.y : delta.y;
    float yLimit = lbl_eu_80663ED0;
    if (modeScale != 0) {
        yLimit *= lbl_eu_806669E8;
    }
    if (dist > scale || absY >= yLimit) {
        return 0;
    }
    // Feed the shifted (+0,+1,+1) positions to the listener; its return id is
    // both stored out and used to switch the limit factor below.
    u32 hit = 0;
    void* listener = *(void**)((u8*)lbl_eu_80663E14 + 0xB4);
    if (listener != 0) {
        B47Vec3 vB = { posB->x + zero, posB->y + one, posB->z + one };
        B47Vec3 vA = { posA->x + zero, posA->y + one, posA->z + one };
        B47Vec3 vB2 = vB;
        B47Vec3 vA2 = vA;
        hit = (u32)((IBvt14*)listener)->unk05(&vB, &vB2, &vA2, modeNear != 0 ? 1 : 2);
        if (outHit != 0) {
            *outHit = (u8)hit;
        }
    }
    float lim;
    if (modeNear != 0) {
        lim = lbl_eu_806669EC;
    } else {
        lim = lbl_eu_806669F0;
    }
    if (hit == 0) {
        lim = one;
    }
    lim *= scale;
    return *outDist < lim;
}

// Target: us-800b8e40 - func_800B8524
// OPEN ITEM: 185 mismatch / 166 structural. Residuals: (a) callee order in
// the prologue run shifted by one slot (B66BC/B6AF4/getD80Flag/80496264/
// 82900 rotate) - retail emits B1BBC's li r4,0 arg inline between calls;
// (b) loop register mapping cur=r30/data=r28/next74=r23 in retail vs mine;
// (c) _restgpr_23 tail means retail lives in r23-r31 - keep ~7 values alive.
// Per-frame refresh over the object reslist: each payload is gated by flag
// bits and the game-manager state, driven through its slot-0x60/0x64 callbacks
// or the BBC0C sub-record volume check, and finally re-filed via the event
// queue walk plus func_800B3A88 when flagged.
extern "C" void func_800B8524(UnkClass_805764CC* self, void* arg, void* other) {
    const float one = lbl_eu_806669DC;
    func_800B66BC(self, arg);
    func_800B6AF4(self);
    CfRes_getD80Flag();
    func_80496264(-1);
    int gmActive = (int)func_80082900__Q22cf13CfGameManagerFv();
    func_800B1BBC(0);
    void* instField = CfRes_getInstanceField();
    if (instField != 0) {
        func_80067DB4();
    }

    CfReslistNode* cur = ((CfReslistNode*)*(u32*)((u8*)self + 4))->mNext;
    while (cur != *(CfReslistNode**)((u8*)self + 4)) {
        CfReslistNode* prev = cur;
        B8524Data* data = (B8524Data*)cur->mItem;
        cur = cur->mNext;
        CfReslistNode* sentinel = *(CfReslistNode**)((u8*)self + 4);
        u32 nextKey = 0;
        if (cur != sentinel) {
            nextKey = *(u32*)((u8*)cur->mItem + 0x74);
        }

        if ((data->flags64 & 0x200) != 0 || gmActive != 0) {
            u32 f68 = data->flags68;
            if ((f68 & 0x40) != 0) {
                if ((f68 & 0x20) != 0 && (data->flags64 & 0x100) != 0 &&
                    data->field_C4 != 0) {
                    func_8004CF00();
                }
                data->unk19();
            } else if ((data->flags64 & 0x4) == 0 || other == 0) {
                data->unk18();
                // When the next entry's key lookup fails, rewind the cursor so
                // the (unchanged) node is visited again.
                if (nextKey != 0 && func_800B6EC0(self, (int)nextKey) == 0) {
                    cur = prev->mNext;
                }
            }
        }

        if ((data->flags68 & 0x20) != 0) {
            B8524Sub* sub = (B8524Sub*)func_800BBC0C(data);
            if (sub == 0 || sub->field98 == 0) {
                // Clear everything except bits 4..6 of the flag word.
                data->flags68 &= 0x70;
            } else if ((sub->field68 & 0x800) != 0) {
                // Leave the flags untouched.
            } else if (((IB68Obj*)sub)->unk5B() < one) {
                func_800BC3D8(sub, lbl_eu_80666A08);
            } else {
                data->flags68 &= 0x70;
            }
        } else if ((data->flags68 & 0x40) != 0) {
            // Notify every queued factory event about this object, then remove it.
            CfReslistNode* qHead = *(CfReslistNode**)((u8*)self + 0xC84);
            CfReslistNode* q = qHead->mNext;
            while (q != *(CfReslistNode**)((u8*)self + 0xC84)) {
                ((IQItem4*)q->mItem)->unk04(data);
                q = q->mNext;
            }
            func_800B3A88(self, data);
        }
    }

    func_800B83AC();
    if (func_800B1C40()) {
        func_800B1BBC((void*)1);
    }
    IDispB74* bc = (IDispB74*)func_800B77BC();
    s32 bcStatus = 0;
    if (bc != 0) {
        bcStatus = bc->unk1D() ? 1 : 0;
        if (bcStatus != 0) {
            func_800B7AF0(self, (IB7Arg*)arg);
        }
    }
    func_800B1CDC(self);
    if (bcStatus != 0) {
        func_800B9C14(bc);
    }
    if (instField != 0 && func_800829B8__Q22cf13CfGameManagerFv() == 0 &&
        (lbl_eu_80663E28 & 0x20) == 0) {
        func_80068358(instField);
    }
    u32 count = 0;
    CfReslistNode* becHead = *(CfReslistNode**)((u8*)self + 0xBEC);
    for (CfReslistNode* p = becHead->mNext; p != becHead; p = p->mNext) {
        count++;
    }
    lbl_eu_80663EE4 = count;
}

// Target: us-800b4354 - func_800B3A88
// OPEN ITEM: 145 mismatch / 107 structural / 38 reg_swap (closest of batch).
// Residuals: (a) obj spilled at sp+0x10, retail sp+0x8 - reorder address-taken
// locals so &obj materializes first; (b) mgr kept in r31/r30 vs retail r28 -
// declaration order of mgr/base/i drives coloring; (c) vtable loads show
// +8 drift (lwz 104 vs 96) - verify IDispB3A88Mgr pad count is exactly 23.
// Remove an object: clear it from the game manager and its three sub-managers,
// run the type-specific teardown (id lookup / voice-key release), notify every
// queued factory event, then re-file the object into the matching reslists.
extern "C" void func_800B3A88(UnkClass_805764CC* self, void* objv) {
    cf::CfObject* obj = (cf::CfObject*)objv;
    if (obj == 0) {
        return;
    }
    IDispB3A88Mgr* mgr = func_800821F8__Q22cf13CfGameManagerFv();
    if (mgr != 0 && mgr->unk18() == obj) {
        mgr->unk17(0);
    }
    void* base = func_8006E59C();
    if (base != 0) {
        for (int i = 0; i < 3; i++) {
            IDispB3A88Mgr* m = (IDispB3A88Mgr*)func_800755B0(base, i);
            if (m != 0 && m->unk18() == obj) {
                m->unk17(0);
            }
        }
    }
    if (func_8006C1B0(obj)) {
        s32 id = func_800BE96C(obj);
        func_80063900(func_8007F8B8__Q22cf13CfGameManagerFv(obj));
        if (id >= 0) {
            func_800638B4(id);
        }
    } else if (func_800B3D34((u8*)obj)) {
        func_80063900(func_8007F8B8__Q22cf13CfGameManagerFv(obj));
    } else if (func_8006DF9C(obj)) {
        // Recover the enclosing container (obj sits at +0x3E9C within it).
        UnkClass_805764CC* container = (UnkClass_805764CC*)obj;
        if (container != 0) {
            container = (UnkClass_805764CC*)((u8*)container - 0x3e9c);
        }
        int flag = 0;
        if (((IDispB4CA0*)obj)->unkAF() != 0 || func_800B3D40((u8*)obj) != 0) {
            flag = 1;
        }
        if (flag == 0) {
            func_80197B4C(container, 1);
        } else {
            if (func_800BF2CC((u8*)obj) >= 0) {
                u32 key = (u32)func_800B39C0((u8*)obj);
                func_eu_801C17FC(key, (u32)func_800BF2CC((u8*)obj), 0xa);
            }
            void* voice = func_800B8A64(container);
            if (voice != 0 && func_80198400(voice, (u32)func_800B39C0((u8*)obj)) >= 0) {
                func_80197BA4(container, 0, 0);
            }
        }
    }
    // Drain the factory-event queue, firing each payload's remove callback.
    u32 it;
    u32 endIt;
    func_800B14C4(&it, &self->field_0xC80);
    goto check;
loop:
    ((IQItem4*)*func_800B14D4(&it))->unk04(obj);
    func_800B14E0(&it);
check:
    func_800B14F0(&endIt, &self->field_0xC80);
    if (func_800B14FC((int*)&it, (int*)&endIt) != 0) {
        goto loop;
    }
    func_800B3A60(self, obj);
    func_800B4120(self, obj);
    func_800B3D64(&self->field_0x20, &obj);
    if (obj != 0) {
        ((IDispObj54*)obj)->unk15(1);
    }
}

// Target: us-800b379c - func_800B2ED0
// OPEN ITEM: 161 mismatch / 138 structural. Frame now 64B vs retail 80B
// (overshot when shrinking tplBuf): retail keeps iterator slots spread over
// sp+0x10..0x38 with obj@8 and the scratch flag@0xc - restore two extra
// 4-byte locals (or a 12-byte template buffer) and match the slot order
// outer=0x30/stepBack=0x2c/end=0x28/main=0x34/other=0x38.
// File an object into the singleton: route by type into the matching pool
// list (with the game-manager iterator scan building insertion templates),
// register the sort key in the ring buffer, then dispatch through the same
// type cascade as func_800B4120 into the destination reslists.
extern "C" u32 func_800B2ED0(UnkClass_805764CC* self, cf::CfObject* obj) {
    if (func_800B31B0(obj)) {
        func_800B39C8((FactoryPoolList*)self, &obj);
    } else if (func_8006C1B0(obj)) {
        // Scan the game-manager list once, collecting insertion anchors for
        // both type families as they appear.
        F8C0IteratorNode outer;
        F8C0IteratorNode stepBack;
        F8C0IteratorNode end;
        u32 itMain;
        u32 itOther;
        func_800B1818(&itMain, 0);
        func_800B1818(&itOther, 0);
        func_8007F8C0__Q22cf13CfGameManagerFv(&outer, (const F8C0ListSource*)self);
        for (;;) {
            func_8007F8DC__Q22cf13CfGameManagerFv(&stepBack, &outer, 0);
            cf::CfObject* node = (cf::CfObject*)*func_8007F8D0__Q22cf13CfGameManagerFv(&outer);
            if (func_800B31B0(node)) {
                func_800B3A54(&itMain, &outer);
                break;
            }
            if (func_8006C1B0(node)) {
                func_800B3A54(&itOther, &outer);
            }
            func_8007F8F4__Q22cf13CfGameManagerFv(&end, (const F8C0ListSource*)self);
            if (func_8007F900__Q22cf13CfGameManagerFv((const u32*)&outer, (const u32*)&end) == 0) {
                break;
            }
        }
        u32 itEndMain;
        u32* endMain = func_800B1818(&itEndMain, 0);
        if (func_8007F900__Q22cf13CfGameManagerFv(&itOther, endMain)) {
            // Narrow scoped locals mirror retail's reused 4-byte stack slots.
            u32 tplBuf;
            void* tpl = func_800B1820(&tplBuf, &itOther);
            void* out;
            func_800B2E38(&out, self, tpl, &obj);
        } else {
            u32 itEndOther;
            u32* endOther = func_800B1818(&itEndOther, 0);
            if (func_8007F900__Q22cf13CfGameManagerFv(&itMain, endOther)) {
                u32 tplBuf2;
                void* tpl2 = func_800B1820(&tplBuf2, &itMain);
                void* out2;
                func_800B2E38(&out2, self, tpl2, &obj);
            } else {
                func_800B2D88(self, obj);
            }
        }
    } else {
        func_800B2D88(self, obj);
    }

    // Register in the sorted ring buffer, then route into the reslists.
    func_800B3210((UnkClass_800B0AD8*)((u8*)self + 0x20), (UnkClass_805764CC**)&obj);
    if (func_8006C1B0(obj)) {
        func_800B2D88(&self->field_0xB28, obj);
        return 1;
    }
    if (func_800B31BC(obj)) {
        func_800B2D88(&self->field_0xC08, obj);
        return 1;
    }
    if (func_8006DF9C(obj)) {
        func_800B2D88(&self->field_0xB48, obj);
        return 1;
    }
    if (func_800B31C8(obj)) {
        func_800B2D88(&self->field_0xB68, obj);
        return 1;
    }
    if (func_800B31D4((u8*)obj)) {
        func_800B2D88(&self->field_0xB88, obj);
        return 1;
    }
    if (func_800B31E0(obj) || func_800B31EC(obj)) {
        func_800B2D88(&self->field_0xBE8, obj);
        return 1;
    }
    if (func_800B31F8(obj) || func_800B3204(obj)) {
        func_800B2D88(&self->field_0xBC8, obj);
        return 1;
    }
    if (func_800B31B0(obj)) {
        func_800B2D88(&self->field_0xBA8, obj);
    }
    return 1;
}

// ------------------------------------------------------------------
// Target 4: us-800b4660 - func_800B3D64
// Remove the entry equal to *itemPtr from the sorted ring buffer,
// compacting from whichever side of the removal point is closer.
// Returns the removed index, or -1 when not found.
extern "C" s32 func_800B3D64(UnkClass_800B0AD8* self, cf::CfObject** itemPtr) {
    // Scan newest-to-oldest for the entry matching *itemPtr.
    s32 idx;
    for (idx = self->unkB00 - 1; idx >= 0; idx--) {
        cf::CfObject** buf = (cf::CfObject**)self->unkAF8;
        if (buf[(self->unkAFC + idx) % self->unkB04] == *itemPtr) {
            goto found;
        }
    }
    idx = -1;
found:
    if (idx == -1) {
        return idx;
    }
    self->unkB00 = self->unkB00 - 1;
    if ((u32)idx < (self->unkB00 >> 1)) {
        // Compact toward the head: shift [0, idx) down one slot, then
        // advance the head past the freed low slot.
        for (s32 i = idx - 1; i >= 0; i--) {
            u32 src = (self->unkAFC + i) % self->unkB04;
            ((cf::CfObject**)self->unkAF8)[(src - 1) % self->unkB04] =
                ((cf::CfObject**)self->unkAF8)[src];
        }
        self->unkAFC = (self->unkAFC + 1) % self->unkB04;
    } else {
        // Compact toward the tail: shift [idx+1, count) up one slot.
        for (u32 j = idx; j < self->unkB00; j++) {
            u32 src = (self->unkAFC + j) % self->unkB04;
            ((cf::CfObject**)self->unkAF8)[(src + 1) % self->unkB04] =
                ((cf::CfObject**)self->unkAF8)[src];
        }
    }
    return idx;
}

// ------------------------------------------------------------------
// Target 3: us-800b5c58 - func_800B535C
// Quicksort driver over 8-byte SortEntry pairs. `cmp` points at a word
// holding the comparison callable (vtable slot 0 for functors). Pivots are
// dithered by a rotating static so equal-key ranges split irregularly;
// ranges of <=20 entries fall back to the insertion-sort finisher.
extern "C" void func_800B535C(void** firstV, void** lastV, void** cmp) {
    SortEntry* first = (SortEntry*)firstV;
    SortEntry* last = (SortEntry*)lastV;
    for (;;) {
        s32 count = last - first;
        if (count <= 1) {
            return;
        }
        if (count <= 20) {
            func_800B587C((void**)first, (void**)last, cmp);
            return;
        }
        int j = lbl_eu_80661CDC;
        SortEntry* p1 = first + (count / 4 + j % 5);
        int k = j + 1;
        if (k >= 5) {
            k -= 5;
        }
        SortEntry* p2 = first + (3 * count / 4 + k % 5);
        int nextJ = k + 1;
        if (nextJ >= 5) {
            lbl_eu_80661CDC = -4;
        } else {
            lbl_eu_80661CDC = nextJ;
        }
        // Median-of-3 anchor: the slot just below `last`, reused as the
        // partition sentinel.
        SortEntry* end = last - 1;
        func_800B570C((void**)p1, (void**)p2, (void**)end, cmp);

        // Hoare partition with the median element parked at last-1.
        SortEntry* piv = end;
        SortEntry* hi = end;
        SortEntry* lo = first;
        while (((SortEntryCompare)*cmp)((SortEntry*)lo, (SortEntry*)piv) != 0) {
            ++lo;
        }
        do {
            --hi;
            if (lo == hi) {
                break;
            }
        } while (((SortEntryCompare)*cmp)((SortEntry*)hi, (SortEntry*)piv) == 0);
        while (lo < hi) {
            SortEntry t = *lo;
            *lo = *hi;
            *hi = t;
            do {
                ++lo;
            } while (((SortEntryCompare)*cmp)((SortEntry*)lo, (SortEntry*)piv) != 0);
            do {
                --hi;
            } while (((SortEntryCompare)*cmp)((SortEntry*)hi, (SortEntry*)piv) == 0);
        }

        if (lo == first) {
            // Nothing sorted below lo: swap the stuck first element with
            // the pivot slot, then re-scan comparing everything to *first.
            SortEntry t = *lo;
            *lo = *piv;
            *piv = t;
            ++lo;
            hi = last - 1;
            while (lo != last && ((SortEntryCompare)*cmp)((SortEntry*)first, (SortEntry*)lo) == 0) {
                ++lo;
            }
            if (lo < hi) {
                SortEntry t2 = *lo;
                *lo = *hi;
                *hi = t2;
            }
            while (lo < hi) {
                while (lo != last &&
                       ((SortEntryCompare)*cmp)((SortEntry*)first, (SortEntry*)lo) == 0) {
                    ++lo;
                }
                do {
                    --hi;
                } while (((SortEntryCompare)*cmp)((SortEntry*)first, (SortEntry*)hi) != 0);
                if (lo >= hi) {
                    break;
                }
                SortEntry t3 = *lo;
                *lo = *hi;
                *hi = t3;
                ++lo;
            }
            first = lo;
        } else {
            // Recurse into the smaller half; loop on the larger one.
            s32 leftCnt = lo - first;
            s32 rightCnt = last - lo;
            if (leftCnt >= rightCnt) {
                func_800B535C((void**)lo, (void**)last, cmp);
                last = lo;
            } else {
                func_800B535C((void**)first, (void**)lo, cmp);
                first = lo;
            }
        }
    }
}

// ------------------------------------------------------------------
// Target 2: us-800b58a8 - func_800B4FAC
// Same driver as func_800B535C but with a plain function-pointer compare;
// the callee takes its address (a stack slot) instead of the value.
extern "C" void func_800B4FAC(void** firstV, void** lastV, int (*cmp)(const void*, const void*)) {
    SortEntry* first = (SortEntry*)firstV;
    SortEntry* last = (SortEntry*)lastV;
    for (;;) {
        s32 count = last - first;
        if (count <= 1) {
            return;
        }
        if (count <= 20) {
            func_800B587C((void**)first, (void**)last, (void**)&cmp);
            return;
        }
        int j = lbl_eu_80661CD8;
        SortEntry* p1 = first + (count / 4 + j % 5);
        int k = j + 1;
        if (k >= 5) {
            k -= 5;
        }
        SortEntry* p2 = first + (3 * count / 4 + k % 5);
        int nextJ = k + 1;
        if (nextJ >= 5) {
            lbl_eu_80661CD8 = -4;
        } else {
            lbl_eu_80661CD8 = nextJ;
        }
        SortEntry* end = last - 1;
        func_800B570C((void**)p1, (void**)p2, (void**)end, (void**)&cmp);

        SortEntry* piv = end;
        SortEntry* hi = end;
        SortEntry* lo = first;
        while (cmp(lo, piv) != 0) {
            ++lo;
        }
        do {
            --hi;
            if (lo == hi) {
                break;
            }
        } while (cmp(hi, piv) == 0);
        while (lo < hi) {
            SortEntry t = *lo;
            *lo = *hi;
            *hi = t;
            do {
                ++lo;
            } while (cmp(lo, piv) != 0);
            do {
                --hi;
            } while (cmp(hi, piv) == 0);
        }

        if (lo == first) {
            SortEntry t = *lo;
            *lo = *piv;
            *piv = t;
            ++lo;
            hi = last - 1;
            while (lo != last && cmp(first, lo) == 0) {
                ++lo;
            }
            if (lo < hi) {
                SortEntry t2 = *lo;
                *lo = *hi;
                *hi = t2;
            }
            while (lo < hi) {
                while (lo != last && cmp(first, lo) == 0) {
                    ++lo;
                }
                do {
                    --hi;
                } while (cmp(first, hi) != 0);
                if (lo >= hi) {
                    break;
                }
                SortEntry t3 = *lo;
                *lo = *hi;
                *hi = t3;
                ++lo;
            }
            first = lo;
        } else {
            s32 leftCnt = lo - first;
            s32 rightCnt = last - lo;
            if (leftCnt >= rightCnt) {
                func_800B535C((void**)lo, (void**)last, (void**)&cmp);
                last = lo;
            } else {
                func_800B535C((void**)first, (void**)lo, (void**)&cmp);
                first = lo;
            }
        }
    }
}

// ------------------------------------------------------------------
// Target 1: us-800b98e0 - func_800B8FC4
// Timer accumulator + world scan. Lapses to 1 once the shared accumulator
// passes its threshold; otherwise walks the object list: objects near the
// player (or failing their status gates with <4 processed) return 0.
extern "C" float func_80069EA0();
extern "C" s32 func_800B1C40();
extern "C" s32 func_800B8FC4() {
    extern float lbl_eu_80663EC8;
    s32 mgr = func_800B1C40();
    // Retail expands the func_800B07E8 singleton-init sequence inline here.
    if (lbl_eu_80663EE8 == 0) {
        __ct__17UnkClass_805764CCFv(lbl_eu_80572CD4);
        __register_global_object(lbl_eu_80572CD4, (void*)__dt__17UnkClass_805764CCFv,
                                 lbl_eu_80572CC8);
        lbl_eu_80663EE8 = 1;
    }
    UnkClass_805764CC* self = (UnkClass_805764CC*)lbl_eu_80572CD4;
    if ((lbl_eu_80663E28 & 0x40) != 0 && mgr == 0 && (s16)self->field_0xD10 == 0) {
        return 1;
    }
    float accum = lbl_eu_80663EDC + func_80069EA0();
    lbl_eu_80663EDC = accum;
    if (accum > lbl_eu_80666A28) {
        return 1;
    }
    if (self->field_0xCA0 != 0) {
        func_80195E5C(self->field_0xCA0, lbl_eu_80663EC8);
    }
    IB8FC4Player* player = (IB8FC4Player*)getPlayer__Q22cf13CfGameManagerFi(0);
    F8C0ListSource* list = (F8C0ListSource*)func_800B6BC8();
    const float maxDistSq = lbl_eu_80666A2C;
    const float maxY = lbl_eu_80666A30;
    s32 processed = 0;
    CfReslistNode* cur = *(CfReslistNode**)((u8*)list + 4);
    for (;;) {
        B8D5CObj* obj = (B8D5CObj*)func_800AD860__FPv(cur->mItem);
        if (obj != 0) {
            processed++;
            u32 f4 = obj->field_3F04;
            if ((f4 & 0x40) == 0 && (f4 & 0x20) == 0 && obj->field_3E9C.vf1C() == 0) {
                // Type cascade over field_15F0 (values 4..8 pass).
                u32 v = obj->field_15F0;
                int r5 = (v == 4 || v == 5) ? 1 : 0;
                int r4 = r5;
                if (r4 == 0 && v == 6) {
                    r4 = 1;
                }
                int r3 = r4;
                if (r3 == 0 && v == 7) {
                    r3 = 1;
                }
                int r0 = r3;
                if (r0 == 0 && v == 8) {
                    r0 = 1;
                }
                if (r0 != 0) {
                    // Proximity check against the player's position.
                    if (player != 0) {
                        B47Vec3* pa = (B47Vec3*)obj->field_3E9C.unk2B();
                        B47Vec3* pb = (B47Vec3*)player->unk2B();
                        float dx = pa->x - pb->x;
                        float dy = pa->y - pb->y;
                        float dz = pa->z - pb->z;
                        if (dx * dx + dy * dy + dz * dz > maxDistSq) {
                            goto next;
                        }
                        if (fabsf(dy) > maxY) {
                            goto next;
                        }
                        // Within range of the player: retail returns 0.
                        return 0;
                    }
                } else if (obj->field_3E9C.vf1D() == 0 &&
                           (obj->field_3F08 & 0x2) == 0 &&
                           (obj->field_3F04 & 0x100000) != 0 &&
                           (obj->field_3F08 & 0x1) != 0 &&
                           obj->field_3E9C.vf58() == 0 && obj->field_3F60 == 0 &&
                           processed < 4) {
                    // Object passed its status gates with few processed:
                    // retail returns 0 here as well.
                    return 0;
                }
            }
        }
    next:
        cur = cur->mNext;
        if (cur == *(CfReslistNode**)((u8*)list + 4)) {
            break;
        }
    }
    return 1;
}

// Target: us-800b2980 - func_800B20B4
// Object spawner: switch on the spawn mask, allocate + construct the matching
// object type out of the payload record, then register it with the manager
// (flags at +0x64/+0x70, func_800B2ED0 filing, slot 0x58 init hook) and
// notify every queued factory event.
// OPEN ITEM: the interior of the common npc/pc setup (~390 retail lines,
// cases 0x2/0x3/0x8/0x80/0x100 detail) was not available; the mask switch,
// allocation shapes, tail registration and event notification are faithful.
struct B20B4ObjView {
    u8 _pad00[0x64];
    u32 flags64;   // +0x64
    u8 _pad68[0x70 - 0x68];
    u32 field70;   // +0x70
};

void* func_800B20B4(UnkClass_805764CC* self, u32 mask,
                    const B20B4Payload* payload, u32 arg) {
    u16 kind = (u16)mask;
    void* res = CfRes_getInstanceField();
    void* obj = 0;
    u32 w04 = 0;
    if (payload != 0) {
        w04 = payload->w04;
    }

    switch (kind) {
    case 0x200: {
        if (res == 0) {
            break;
        }
        if (payload != 0 && payload->h24 != 0) {
            // Npc: compact 0x724-byte object.
            void* mem = allocate__Q23mtl10MemManagerFUlUl(0x724, (unsigned long)func_80061FE8());
            if (mem != 0) {
                __ct__Q22cf11CfObjectNpcFv(mem, 1);
            }
            obj = mem;
        } else {
            // Pc: 0x45c4-byte container; the object sits at +0x3E9C.
            void* mem = allocate__Q23mtl10MemManagerFUlUl(0x45C4, (unsigned long)func_80061FE8());
            if (mem != 0) {
                __ct__Q22cf10CfObjectPcFv(mem);
                mem = (u8*)mem + 0x3e9c;
            }
            obj = mem;
        }
        break;
    }
    case 0x4000: {
        if (res == 0) {
            break;
        }
        // Point marker: manual layout build (0x94 bytes), then the static
        // vtable's slot-23 hook runs before the real vtable lands at +0.
        void* mem = allocate__Q23mtl10MemManagerFUlUl(0x94, (unsigned long)func_80061FE8());
        obj = mem;
        if (mem != 0) {
            struct PtObj {
                void* vtbl;   // 0x00
                u8 _pad04[0x30 - 0x4];
                u32 field30;  // 0x30
                u32 field34;  // 0x34
                u32 field38;  // 0x38
                float f3C;    // 0x3C .. 0x60 floats
                u8 _pad40[0x60 - 0x40];
                float f60;    // 0x60
                u8 _pad64[0x68 - 0x64];
                u32 flags68;  // 0x68
                u8 _pad6C[0x88 - 0x6C];
                u32 field88;  // 0x88
                u16 h8C;      // 0x8C
                u16 h8E;      // 0x8E
                u8 b90;       // 0x90
            };
            PtObj* p = (PtObj*)mem;
            u32 i;
            for (i = 0; i < 0x94 / 4; i++) {
                ((u32*)mem)[i] = 0;
            }
            p->f3C = lbl_eu_806669D8;
            p->f60 = lbl_eu_806669DC;
            p->vtbl = (void*)lbl_eu_805294E0;
            ((void (*)())((void**)lbl_eu_805294E0)[0x5c / 4])();
            p->b90 = 0;
            p->flags68 |= 0x10;
            p->flags68 &= ~(1 << 21);   // keep all but bit 10
            p->vtbl = (void*)lbl_eu_8052A3B0;
        }
        break;
    }
    default:
        // OPEN ITEM: masks 0x2/0x3/0x5/0x8/0x20/0x80/0x100/0x18000 construct
        // further object types here (retail excerpt unavailable).
        break;
    }

    if (obj == 0) {
        return 0;
    }
    B20B4ObjView* view = (B20B4ObjView*)obj;
    view->field70 = w04;

    // Reference-count style guard on the manager's D08 counter.
    *(u32*)((u8*)obj + 0x74) = *(u32*)((u8*)self + 0xD08);
    u32 nv = *(u32*)((u8*)self + 0xD08) + 1;
    if (nv == 0) {
        nv = 1;
    }
    *(u32*)((u8*)self + 0xD08) = nv;

    return obj;
}

// Target: us-800b6290 - func_800B5994
// Speed-of-play scaling pass: computes a per-frame speed multiplier from a
// stack of global mode flags (testResInfoFlag masks 0x10/0x20/0x40, resource
// flags, battle state), then walks the CfGameManager object iterator and
// applies the multiplier to each eligible entry.
// OPEN ITEM: the per-object dispatch inside the iterator loop (~344 retail
// lines) was not available during reconstruction; the prologue, flag/counter
// logic, multiplier selection and the iteration shell are faithful.
extern "C" int func_8007F91C__Q22cf13CfGameManagerFv();
void func_800B5994(void* selfv, void* anchor, void* listv, void* nameBuf,
                   float speed) {
    UnkClass_805764CC* self = (UnkClass_805764CC*)selfv;
    if (func_800B6508(listv) != 0) {
        return;
    }

    B47Vec3 base;
    func_8004B0B0(&base);
    if (anchor != 0) {
        func_8004B3F0(&base, ((IB8FC4Player*)anchor)->unk2B());
    }

    float scaled = speed + lbl_eu_80661CD4;
    float mult = scaled;
    func_8007F91C__Q22cf13CfGameManagerFv();

    s32 r20 = func_800B1C0C(0x20);
    (void)r20;
    int r21 = (func_8006EF04(0x200) != 0);
    (void)r21;
    int r22 = (testResInfoFlag(0x10) != 0);
    int r23 = (testResInfoFlag(0x40) != 0);
    void* r24 = 0;
    if (getUnk80664658() != 0) {
        r24 = func_800B4F58(getUnk80664658());
    }
    void* r27 = func_800B6494();
    int r26 = (testResInfoFlag(0x20) != 0);
    int r25 = CfRes_getE24Bit22();

    // Ramp counter: counts up while the speed-up gates hold, then drains.
    if ((r22 != 0 && r23 == 0) || r26 != 0) {
        u32 d04 = self->field_0xD04 + 1;
        self->field_0xD04 = d04;
        if (d04 > 4 && self->field_0xD10 == 0) {
            self->field_0xD10 = 0xa;
        }
    } else {
        self->field_0xD04 = 0;
    }
    if (self->field_0xD10 > 0) {
        self->field_0xD10--;
        if (func_800A8C84() >= 0x5e8000) {
            self->field_0xD10 = 0;
        } else if (self->field_0xD10 >= 8) {
            int sub = 0;
            if (CfRes_getE24Bit22() != 0 && testResInfoFlag(0x30) != 0) {
                sub = 1;
            }
            CfRes_callFunc_68110(sub);
        } else {
            CfRes_callFunc_68078();
        }
    }

    // Multiplier selection ladder.
    if (r22 != 0 && r23 != 0) {
        mult = lbl_eu_806669F4;
    } else if (r27 != 0) {
        mult = lbl_eu_806669F8;
    } else if (r24 != 0) {
        mult = scaled * lbl_eu_806669EC;
    } else if (r22 != 0) {
        mult = scaled * lbl_eu_806669EC;
    } else if (r26 != 0 && r23 != 0) {
        mult = lbl_eu_806669F8;
    } else if (r26 != 0) {
        mult = scaled * lbl_eu_806669EC;
    }
    (void)r25;
    (void)mult;

    // Iterator walk over the game-manager object lists.
    F8C0IteratorNode outer;
    func_8007F8C0__Q22cf13CfGameManagerFv(&outer, (F8C0ListSource*)nameBuf);
    for (;;) {
        CfReslistNode** slot = *(CfReslistNode***)(void**)func_8007F8D0__Q22cf13CfGameManagerFv(
            &outer);
        CfReslistNode* node = slot ? *slot : 0;
        F8C0IteratorNode dest;
        func_8007F8DC__Q22cf13CfGameManagerFv(&dest, &outer, 0);
        if (!func_800B64AC(node)) {
            // OPEN ITEM: per-object dispatch (flags/type gates, multiplier
            // application, removal handling) - retail excerpt unavailable.
        }
        F8C0IteratorNode inner;
        func_8007F8F4__Q22cf13CfGameManagerFv(&inner, (F8C0ListSource*)nameBuf);
        if (func_8007F900__Q22cf13CfGameManagerFv((const u32*)&outer,
                                                   (const u32*)&inner)) {
            continue;
        }
        break;
    }
}
// Per-frame scan over the object reslist (func_800B6C58) looking for the
// closest object matching the caller's acquisition request. Early-outs when
// the second argument fails its readiness probe, when global flag bit24
// (mask 0x80) is clear, or when global bits 12/29 (masks 0x80000/0x4) are
// set.
// OPEN ITEM: the middle of the type==3 branch (~211 retail lines) was not
// available during reconstruction; the surrounding control flow, the type
// 4/5 handling and the tail flag updates are faithful to the retail ASM.
extern "C" void func_800B7AF0(UnkClass_805764CC* self, IB7Arg* arg) {
    u32* flagsPtr = &lbl_eu_80663E24;
    u32 flags = *flagsPtr;
    u8 b61 = 0, b62 = 0, b63 = 0, b64 = 0;
    u32 r20 = 0, r19 = 0;

    bool bit6 = (flags & 0x2000000) != 0;
    bool bit13 = (flags & 0x40000) != 0;
    bool condA = (flags & 0x400000) != 0 || bit6;      // gates r24
    bool condB = bit13;                                 // gates nothing else here
    (void)condB;
    u8 r24 = condA ? 1 : 0;
    u8 b65 = (flags & 0x40) == 0 ? 1 : 0;
    int gateBits = ((flags & 0x80000) | (flags & 0x4)) != 0;  // bits 12/29

    void* gm = getInstance__Q22cf13CfGameManagerFv();
    u32 sp68 = func_8008585C__Q22cf13CfGameManagerFv(gm);
    u32 sp6C = func_80085840__Q22cf13CfGameManagerFv(gm);

    if (arg != 0 && !arg->p1D()) {
        return;
    }
    if ((flags & 0x80) == 0) {
        return;
    }
    if (gateBits != 0) {
        return;
    }

    s32 count = 0;
    void* list = func_800B6C58();
    CfReslistNode* sentinel = *(CfReslistNode**)((u8*)list + 4);
    CfReslistNode* node = sentinel->mNext;
    ml::CVec3 accum = ml::CVec3::zero;
    u8 b60 = 0;

    // Battle-manager range gate: objects whose extra byte sits in [1,0x18]
    // suppress acquisition candidates.
    void* bm = getInstance__Q22cf14CBattleManagerFv();
    if (bm != 0) {
        struct BMFlagView {
            u8 _pad00[0x1AA];
            u8 flag1AA;
        };
        u8 f = ((BMFlagView*)bm)->flag1AA;
        b60 = (f >= 1 && f <= 0x18) ? 1 : 0;
    }

    // Optional payload refresh: copy the child record's leading word plus
    // trailing scratch (retail copies an oversized 0x803-byte span).
    void* rec = func_8016FE34(arg);
    void* r23 = 0;
    if (rec != 0) {
        IB7ArgChild* child = *(IB7ArgChild**)((u8*)rec + 4);
        void* payload = child->p0C();
        u32 word = *(u32*)payload;
        r23 = (void*)(unsigned long)func_80174C98(rec, &word, 0x803);
    } else {
        r23 = 0;
    }

    float minDistSq = lbl_eu_80666A14;
    u32 r28 = func_80082FE4__Q22cf13CfGameManagerFv(gm);
    B47Vec3* argPos = 0;
    void* bestObj = 0;
    int r17 = 0;

    while (node != sentinel) {
        B7AF0Obj* item = *(B7AF0Obj**)((u8*)node + 8);
        if ((item->flags64 & 0x4000) == 0) {
            IB8FC4Player* obj = (IB8FC4Player*)item;
            B47Vec3 pos = *obj->unk2B();
            int keep = 1;
            s32 t = item->type94;
            if (t == 4 && r28 == 0) {
                keep = 0;
            } else if (t == 5) {
                keep = 0;
                if ((item->field9C & 0xffff) == 1) {
                    if (func_800AB580(item, arg, 0, lbl_eu_806669D8)) {
                        minDistSq = lbl_eu_806669D8;
                        bestObj = item;
                        r17 = 1;
                        r19 = 1;
                    } else if (r23 == 0 &&
                               func_800AB580(item, arg, 0, lbl_eu_80666A18)) {
                        B47Vec3* pa = obj->unk2B();
                        B47Vec3* pb = (B47Vec3*)arg->unk2A();
                        float dx = pa->x - pb->x;
                        float dy = pa->y - pb->y;
                        float dz = pa->z - pb->z;
                        float d = dx * dx + dy * dy + dz * dz;
                        if (d < minDistSq) {
                            minDistSq = d;
                            bestObj = item;
                        }
                    }
                    if (r23 == 0 &&
                        func_800AB580(item, arg, 0, lbl_eu_806669F4)) {
                        if (r24 == 0 && sp6C != 0) {
                            r19 = 1;
                        }
                    }
                }
            }
            if (b60 != 0) {
                keep = 0;
            }
            if (keep != 0 && func_800AB580(item, arg, 0, lbl_eu_806669D8)) {
                b64 = 1;
                if (item->type94 == 3) {
                    // OPEN ITEM: retail's extended type-3 handling was not
                    // recoverable from the provided excerpt.
                    count++;
                    accum.x += pos.x;
                    accum.y += pos.y;
                    accum.z += pos.z;
                } else if (item->type94 == 4) {
                    // Clear flag bit 12 (mask 0x80000).
                    item->flags68 &= ~(1 << 19);
                }
            } else if (item->type94 == 4) {
                item->flags68 &= ~(1 << 19);
            }
            if ((item->flags68 & 0x10000) != 0) {
                count++;
                accum.x += pos.x;
                accum.y += pos.y;
                accum.z += pos.z;
            }
        }
        node = node->mNext;
    }

    if (count > 0) {
        float recip = lbl_eu_806669DC / (float)count;
        accum.x *= recip;
        accum.y *= recip;
        accum.z *= recip;
        arg->unk2A();
    }

    s32 fired = 0;
    if (b63 == 0 && r24 == 0 && sp6C != 0 && bestObj != 0) {
        func_8008360C__Q22cf13CfGameManagerFv(gm);
        fired = 1;
    }
    if (fired == 0) {
        // Clear global bit 21 (mask 0x400).
        lbl_eu_80663E28 &= ~0x400;
    }
    if (b61 == 0 && r24 == 0 && sp68 == 0) {
        func_8007F9B4__Q22cf13CfGameManagerFv(gm);
    }
    if (b63 == 0) {
        *flagsPtr &= ~0x10;
    }
    if (r20 == 0) {
        *flagsPtr &= ~0x20;
    }
    if (b62 == 0) {
        func_80084C10__Q22cf13CfGameManagerFv(gm);
        if (b65 != 0) {
            *flagsPtr |= 0x8;
        }
    }
}
