// Auto-scaffolded catalog TU for kyoshin/cf/CfObjectEnumList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfObjectEnumList.hpp"
#include "monolib/math.hpp"      // ml::CVec3
#include "monolib/util/MemManager.hpp"  // mtl::MemManager (allocate_array)
#include <nw4r/math.h>            // VEC3Sub (paired-single) + PSVECMag

// Forward decl (was pulled in via CfCam.hpp, which now conflicts with this
// TU's extern "C" declarations of func_8016FE34/func_800BBC0C).
namespace cf { class CfCamFollow; }

namespace cf {
    // --- CfObjectSelectorObj's +4 member list (mirrors CfObjEnumList's
    // embedded reslist base; see CfObjectEnumList.hpp) ---
    // Retail ~CfObjectSelectorObj inlines the whole destruction chain:
    // the CfObjEnumList dtor (implicit MWCC guard + explicit if(this)) then
    // the reslist-base dtor (guard + vtable store + sentinel walk + array
    // free).  Those three inlined dtor layers are the retail's three
    // `beq` null-guards on this+4.

    struct CfSelectorNode {         // reslist node (12 bytes)
        CfSelectorNode* mNext;      // 0x00
        CfSelectorNode* mPrev;      // 0x04
        void* mItem;                // 0x08
    };

    class CfSelectorListBase {     // reslist<T> base mirror (0x20 bytes)
    public:
        ~CfSelectorListBase() {
            // Explicit if(this): with MWCC's implicit guard this is the
            // retail's 2nd+3rd beq.  The vtable store mirrors the base
            // virtual-dtor's store of the retail label; the walk/free are
            // the reslist-base dtor body.
            if (this) {
                mVtable = (void*)&lbl_eu_8052585C;
                // clearList: null every node's mNext until back at the
                // sentinel (mStartNode member re-read each iteration).
                CfSelectorNode* sentinel = mStartNode;
                CfSelectorNode* node = sentinel->mNext;
                while (node != mStartNode) {
                    CfSelectorNode* next = node->mNext;
                    node->mNext = NULL;
                    node = next;
                }
                mStartNode->mNext = mStartNode;
                mStartNode->mPrev = mStartNode;
                if (!mOwnsList && mList != NULL) {
                    delete[] mList;   // retail __dla__FPv
                    mList = NULL;
                }
            }
        }
        void* mVtable;              // 0x00
        CfSelectorNode* mStartNode; // 0x04
        CfSelectorNode mStartNode_; // 0x08
        CfSelectorNode* mList;      // 0x14
        int mCapacity;              // 0x18
        bool mOwnsList;             // 0x1C
    };

    class CfSelectorObjEnumList {  // CfObjEnumList mirror (2nd dtor layer)
    public:
        // Empty body: MWCC's implicit guard is the retail's 1st beq, and
        // mBase is auto-destroyed (inlined base dtor below, 2 more beqs).
        ~CfSelectorObjEnumList() {}
        CfSelectorListBase mBase;   // 0x00
    };

    class CfObjectSelectorObj {
    public:
        // NON-virtual: the retail dtor stores no class vtable at this+0.
        ~CfObjectSelectorObj();

    private:
        void* m_pad00;              // 0x00
        CfSelectorObjEnumList mList; // 0x04
    };

    void* CfObjEnumList::func_800F6E98(int index) {
        return reinterpret_cast<void*>(mPtrArray[index]->objectId);
    }

    void* CfObjEnumList::func_800F6EAC(unsigned long idx) {
        return mPtrArray[idx]->object;
    }

    void* CfObjEnumList::func_800F6EC0(int index) {
        return mPtrArray[index];
    }
}

// __ct__cf_CfObjEnumList: CfObjEnumList constructor (retail symbol is the
// unmangled global name, so this is a global function, not a member).  The
// reslist<T> base is initialized inline first (base vtable store, backing
// fields, sentinel linkage), then the derived fields: the class vtable store,
// the array/count, the sort-descriptor vtables, the per-entry flag bytes
// (element 0 by hand, the rest through a pointer loop MWCC unrolls by 8), and
// finally the backing-array allocation (702 x 12-byte nodes; only each
// entry's first word is cleared, capacity 702).
void __ct__cf_CfObjEnumList(cf::CfObjEnumList* self) {
    // reslist<T> base ctor (inlined): the base vtable store is dead (the
    // derived store below overwrites it), so it must go through a volatile
    // write to survive dead-store elimination (flattened-reslist ctor
    // schedule, MWCC_CASES "kyoshin code_800B06A4").
    *(volatile u32*)self = (u32)&lbl_eu_8052585C[0];
    self->mList = NULL;
    self->mCapacity = 0;
    self->mOwnsList = false;
    self->mStartNode = &self->mStartNode_;
    self->mStartNode->mNext = &self->mStartNode_;
    self->mStartNode->mPrev = &self->mStartNode_;
    // derived fields
    self->mVtable = (void*)&lbl_eu_8052BD80[0];
    self->mPtrCount = 0;
    self->mSortVtableB = (void*)&lbl_eu_8052BDA0[0];
    self->mSortVtableA = (void*)&lbl_eu_8052BDA0[8];
    // mObjInfo flag-byte init: element 0 handled separately (retail peels it
    // into the straight-line store block), the rest is an unrolled pointer
    // walk by MWCC.
    self->mObjInfo[0].field_18 = 0;
    for (cf::CfObjEnumList::sObjInfo* p = &self->mObjInfo[1];
         p < &self->mObjInfo[384]; ++p) {
        p->field_18 = 0;
    }
    self->mObjInfoCount = 0;
    self->mList = mtl::MemManager::allocate_array(
        0x20E8, mtl::MemManager::getHandleMEM1());
    // Backing array: 702 x 12-byte nodes, only the first word cleared.  The
    // stores alias self->mList, so MWCC reloads the member each iteration
    // (retail: 8x lwz 0x14(r30) per unrolled block) - no cached local.
    for (u32 i = 0; i < 0x2BE; i++) {
        reinterpret_cast<cf::CfObjEnumList::sResNode*>(self->mList)[i].mNext =
            NULL;
    }
    self->mCapacity = 0x2BE;
    self->field_303C = 0;
    self->field_3034 = 0;
    self->field_3038 = 0;
}

// func_800F49F8: append every source-list entry (copy of sObjInfo) to this
// list.  Walks the source pointer array with a running pointer; mObjInfoCount
// is bumped before the copy, so the mPtrArray entry points at the
// just-appended slot via mObjInfoCount - 1.
void func_800F49F8(cf::CfObjEnumList* self, cf::CfObjEnumList* src) {
    for (u32 i = 0; i < src->mPtrCount; i++) {
        u32 idx = self->mObjInfoCount;
        cf::CfObjEnumList::sObjInfo* srcInfo = src->mPtrArray[i];
        self->mObjInfoCount = idx + 1;
        cf::CfObjEnumList::sObjInfo* dst = &self->mObjInfo[idx];
        dst->objectId = srcInfo->objectId;
        dst->object = srcInfo->object;
        // Deferred-store locals: retail loads field_0C before field_08 but
        // stores them in ascending order.
        u32 fieldC = srcInfo->field_0C;
        u32 field8 = srcInfo->field_08;
        dst->field_08 = field8;
        dst->field_0C = fieldC;
        dst->field_10 = srcInfo->field_10;
        dst->field_14 = srcInfo->field_14;
        dst->field_18 = srcInfo->field_18;
        self->mPtrArray[self->mPtrCount] =
            &self->mObjInfo[self->mObjInfoCount - 1];
        self->mPtrCount = self->mPtrCount + 1;
    }
}

// Forward decl: defined later in this TU.  Retail symbol is C-linkage
// (unmangled), so the definition below is extern "C" to match reloc names.
extern "C" void func_800F4B5C(cf::CfObjEnumList* self, int flags, u32 options);

// func_800F4A98: reset the list counts, then normalize the flags/options pair
// (param - 0x80000000 selects one of five presets) and hand off to the main
// enumerator func_800F4B5C as a tail call.  An unset param (0) just clears the
// lists.  The default case passes the raw (param, param2) pair through.
void func_800F4A98(cf::CfObjEnumList* self, u32 param, u32 param2) {
    self->mPtrCount = 0;
    self->mObjInfoCount = 0;
    if (param == 0) {
        return;
    }
    switch (param) {
    case 0x80000000:
        self->field_3034 = 0x100;
        self->field_3038 = 0x2802;
        break;
    case 0x80000001:
        self->field_3034 = 0x200;
        self->field_3038 = 0x2802;
        break;
    case 0x80000002:
        self->field_3034 = 0x100;
        self->field_3038 = 0x2802;
        break;
    case 0x80000003:
        self->field_3034 = 0x100;
        self->field_3038 = 0x802;
        break;
    case 0x80000004:
        self->field_3034 = 0x1b00;
        self->field_3038 = 0x8802;
        break;
    default:
        self->field_3034 = param;
        self->field_3038 = param2;
        break;
    }
    func_800F4B5C(self, self->field_3034, self->field_3038);
}

#define ENUM_DUMMY_VIRTUAL(n_) virtual void v##n_() = 0

class CfEnumObjectBase {
public:
    ENUM_DUMMY_VIRTUAL(0);  ENUM_DUMMY_VIRTUAL(1);  ENUM_DUMMY_VIRTUAL(2);
    ENUM_DUMMY_VIRTUAL(3);  ENUM_DUMMY_VIRTUAL(4);  ENUM_DUMMY_VIRTUAL(5);
    ENUM_DUMMY_VIRTUAL(6);  ENUM_DUMMY_VIRTUAL(7);  ENUM_DUMMY_VIRTUAL(8);
    ENUM_DUMMY_VIRTUAL(9);  ENUM_DUMMY_VIRTUAL(10); ENUM_DUMMY_VIRTUAL(11);
    ENUM_DUMMY_VIRTUAL(12); ENUM_DUMMY_VIRTUAL(13); ENUM_DUMMY_VIRTUAL(14);
    ENUM_DUMMY_VIRTUAL(15); ENUM_DUMMY_VIRTUAL(16); ENUM_DUMMY_VIRTUAL(17);
    ENUM_DUMMY_VIRTUAL(18); ENUM_DUMMY_VIRTUAL(19); ENUM_DUMMY_VIRTUAL(20);
    ENUM_DUMMY_VIRTUAL(21); ENUM_DUMMY_VIRTUAL(22); ENUM_DUMMY_VIRTUAL(23);
    ENUM_DUMMY_VIRTUAL(24); ENUM_DUMMY_VIRTUAL(25); ENUM_DUMMY_VIRTUAL(26);
    virtual bool isEnabled() = 0; // vtable +0x74
    ENUM_DUMMY_VIRTUAL(28); ENUM_DUMMY_VIRTUAL(29); ENUM_DUMMY_VIRTUAL(30);
    ENUM_DUMMY_VIRTUAL(31); ENUM_DUMMY_VIRTUAL(32); ENUM_DUMMY_VIRTUAL(33);
    ENUM_DUMMY_VIRTUAL(34); ENUM_DUMMY_VIRTUAL(35); ENUM_DUMMY_VIRTUAL(36);
    ENUM_DUMMY_VIRTUAL(37); ENUM_DUMMY_VIRTUAL(38); ENUM_DUMMY_VIRTUAL(39);
    ENUM_DUMMY_VIRTUAL(40); ENUM_DUMMY_VIRTUAL(41); ENUM_DUMMY_VIRTUAL(42);
    ENUM_DUMMY_VIRTUAL(43); ENUM_DUMMY_VIRTUAL(44); ENUM_DUMMY_VIRTUAL(45);
    ENUM_DUMMY_VIRTUAL(46); ENUM_DUMMY_VIRTUAL(47); ENUM_DUMMY_VIRTUAL(48);
    ENUM_DUMMY_VIRTUAL(49); ENUM_DUMMY_VIRTUAL(50); ENUM_DUMMY_VIRTUAL(51);
    ENUM_DUMMY_VIRTUAL(52); ENUM_DUMMY_VIRTUAL(53); ENUM_DUMMY_VIRTUAL(54);
    ENUM_DUMMY_VIRTUAL(55); ENUM_DUMMY_VIRTUAL(56); ENUM_DUMMY_VIRTUAL(57);
    ENUM_DUMMY_VIRTUAL(58); ENUM_DUMMY_VIRTUAL(59); ENUM_DUMMY_VIRTUAL(60);
    ENUM_DUMMY_VIRTUAL(61); ENUM_DUMMY_VIRTUAL(62); ENUM_DUMMY_VIRTUAL(63);
    ENUM_DUMMY_VIRTUAL(64); ENUM_DUMMY_VIRTUAL(65); ENUM_DUMMY_VIRTUAL(66);
    ENUM_DUMMY_VIRTUAL(67); ENUM_DUMMY_VIRTUAL(68); ENUM_DUMMY_VIRTUAL(69);
    ENUM_DUMMY_VIRTUAL(70); ENUM_DUMMY_VIRTUAL(71); ENUM_DUMMY_VIRTUAL(72);
    ENUM_DUMMY_VIRTUAL(73); ENUM_DUMMY_VIRTUAL(74); ENUM_DUMMY_VIRTUAL(75);
    ENUM_DUMMY_VIRTUAL(76); ENUM_DUMMY_VIRTUAL(77); ENUM_DUMMY_VIRTUAL(78);
    ENUM_DUMMY_VIRTUAL(79); ENUM_DUMMY_VIRTUAL(80); ENUM_DUMMY_VIRTUAL(81);
    ENUM_DUMMY_VIRTUAL(82); ENUM_DUMMY_VIRTUAL(83); ENUM_DUMMY_VIRTUAL(84);
    ENUM_DUMMY_VIRTUAL(85);
    virtual bool filterA() = 0;  // vtable +0x160
    virtual bool filterB() = 0;  // vtable +0x164
    ENUM_DUMMY_VIRTUAL(88);
    virtual float filterC() = 0; // vtable +0x16C
    ENUM_DUMMY_VIRTUAL(90);
    virtual float filterD() = 0; // vtable +0x174
};

class CfEnumActorAuxBase {
public:
    ENUM_DUMMY_VIRTUAL(0);  ENUM_DUMMY_VIRTUAL(1);  ENUM_DUMMY_VIRTUAL(2);
    ENUM_DUMMY_VIRTUAL(3);  ENUM_DUMMY_VIRTUAL(4);  ENUM_DUMMY_VIRTUAL(5);
    ENUM_DUMMY_VIRTUAL(6);  ENUM_DUMMY_VIRTUAL(7);  ENUM_DUMMY_VIRTUAL(8);
    ENUM_DUMMY_VIRTUAL(9);  ENUM_DUMMY_VIRTUAL(10); ENUM_DUMMY_VIRTUAL(11);
    ENUM_DUMMY_VIRTUAL(12); ENUM_DUMMY_VIRTUAL(13); ENUM_DUMMY_VIRTUAL(14);
    ENUM_DUMMY_VIRTUAL(15); ENUM_DUMMY_VIRTUAL(16); ENUM_DUMMY_VIRTUAL(17);
    ENUM_DUMMY_VIRTUAL(18); ENUM_DUMMY_VIRTUAL(19); ENUM_DUMMY_VIRTUAL(20);
    ENUM_DUMMY_VIRTUAL(21); ENUM_DUMMY_VIRTUAL(22); ENUM_DUMMY_VIRTUAL(23);
    ENUM_DUMMY_VIRTUAL(24); ENUM_DUMMY_VIRTUAL(25); ENUM_DUMMY_VIRTUAL(26);
    ENUM_DUMMY_VIRTUAL(27); ENUM_DUMMY_VIRTUAL(28); ENUM_DUMMY_VIRTUAL(29);
};

class CfEnumActorAux : public CfEnumActorAuxBase {
public:
    virtual bool test(int) = 0; // vtable +0x80
};

class CfEnumObject : public CfEnumObjectBase {
public:
    u8 _pad_004[0x64 - 0x04];
    u32 flags64;
    u32 field68;            // 0x68 (mFlags68)
    u8 _pad_06C[0x74 - 0x6C];
    u32 objectId;
    u8 _pad_078[0x8C - 0x78];
    u16 type8C;
    u8 _pad_08E[0xC4 - 0x8E];
    CfEnumActorAux* actorAux;
};

class CfEnumActorBase {
public:
    ENUM_DUMMY_VIRTUAL(0);   ENUM_DUMMY_VIRTUAL(1);   ENUM_DUMMY_VIRTUAL(2);
    ENUM_DUMMY_VIRTUAL(3);   ENUM_DUMMY_VIRTUAL(4);   ENUM_DUMMY_VIRTUAL(5);
    ENUM_DUMMY_VIRTUAL(6);   ENUM_DUMMY_VIRTUAL(7);   ENUM_DUMMY_VIRTUAL(8);
    ENUM_DUMMY_VIRTUAL(9);   ENUM_DUMMY_VIRTUAL(10);  ENUM_DUMMY_VIRTUAL(11);
    ENUM_DUMMY_VIRTUAL(12);  ENUM_DUMMY_VIRTUAL(13);  ENUM_DUMMY_VIRTUAL(14);
    ENUM_DUMMY_VIRTUAL(15);  ENUM_DUMMY_VIRTUAL(16);  ENUM_DUMMY_VIRTUAL(17);
    ENUM_DUMMY_VIRTUAL(18);  ENUM_DUMMY_VIRTUAL(19);  ENUM_DUMMY_VIRTUAL(20);
    ENUM_DUMMY_VIRTUAL(21);  ENUM_DUMMY_VIRTUAL(22);  ENUM_DUMMY_VIRTUAL(23);
    ENUM_DUMMY_VIRTUAL(24);  ENUM_DUMMY_VIRTUAL(25);  ENUM_DUMMY_VIRTUAL(26);
    ENUM_DUMMY_VIRTUAL(27);  ENUM_DUMMY_VIRTUAL(28);  ENUM_DUMMY_VIRTUAL(29);
    ENUM_DUMMY_VIRTUAL(30);  ENUM_DUMMY_VIRTUAL(31);  ENUM_DUMMY_VIRTUAL(32);
    ENUM_DUMMY_VIRTUAL(33);  ENUM_DUMMY_VIRTUAL(34);  ENUM_DUMMY_VIRTUAL(35);
    ENUM_DUMMY_VIRTUAL(36);  ENUM_DUMMY_VIRTUAL(37);  ENUM_DUMMY_VIRTUAL(38);
    ENUM_DUMMY_VIRTUAL(39);  ENUM_DUMMY_VIRTUAL(40);  ENUM_DUMMY_VIRTUAL(41);
    ENUM_DUMMY_VIRTUAL(42);  ENUM_DUMMY_VIRTUAL(43);  ENUM_DUMMY_VIRTUAL(44);
    ENUM_DUMMY_VIRTUAL(45);  ENUM_DUMMY_VIRTUAL(46);  ENUM_DUMMY_VIRTUAL(47);
    ENUM_DUMMY_VIRTUAL(48);  ENUM_DUMMY_VIRTUAL(49);  ENUM_DUMMY_VIRTUAL(50);
    ENUM_DUMMY_VIRTUAL(51);  ENUM_DUMMY_VIRTUAL(52);  ENUM_DUMMY_VIRTUAL(53);
    ENUM_DUMMY_VIRTUAL(54);  ENUM_DUMMY_VIRTUAL(55);  ENUM_DUMMY_VIRTUAL(56);
    ENUM_DUMMY_VIRTUAL(57);  ENUM_DUMMY_VIRTUAL(58);  ENUM_DUMMY_VIRTUAL(59);
    ENUM_DUMMY_VIRTUAL(60);  ENUM_DUMMY_VIRTUAL(61);  ENUM_DUMMY_VIRTUAL(62);
    ENUM_DUMMY_VIRTUAL(63);  ENUM_DUMMY_VIRTUAL(64);  ENUM_DUMMY_VIRTUAL(65);
    ENUM_DUMMY_VIRTUAL(66);  ENUM_DUMMY_VIRTUAL(67);  ENUM_DUMMY_VIRTUAL(68);
    ENUM_DUMMY_VIRTUAL(69);  ENUM_DUMMY_VIRTUAL(70);  ENUM_DUMMY_VIRTUAL(71);
    ENUM_DUMMY_VIRTUAL(72);  ENUM_DUMMY_VIRTUAL(73);
    virtual float value() = 0;   // decl idx 74 -> retail vtable +0x130 (sort key)
    ENUM_DUMMY_VIRTUAL(75);  ENUM_DUMMY_VIRTUAL(76);  ENUM_DUMMY_VIRTUAL(77);
    ENUM_DUMMY_VIRTUAL(78);  ENUM_DUMMY_VIRTUAL(79);  ENUM_DUMMY_VIRTUAL(80);
    ENUM_DUMMY_VIRTUAL(81);  ENUM_DUMMY_VIRTUAL(82);  ENUM_DUMMY_VIRTUAL(83);
    ENUM_DUMMY_VIRTUAL(84);  ENUM_DUMMY_VIRTUAL(85);
    virtual float value2() = 0;  // decl idx 86 -> retail vtable +0x160 (sort key)
    ENUM_DUMMY_VIRTUAL(87);  ENUM_DUMMY_VIRTUAL(88);  ENUM_DUMMY_VIRTUAL(89);
    ENUM_DUMMY_VIRTUAL(90);  ENUM_DUMMY_VIRTUAL(91);  ENUM_DUMMY_VIRTUAL(92);
    ENUM_DUMMY_VIRTUAL(93);  ENUM_DUMMY_VIRTUAL(94);  ENUM_DUMMY_VIRTUAL(95);
    ENUM_DUMMY_VIRTUAL(96);  ENUM_DUMMY_VIRTUAL(97);  ENUM_DUMMY_VIRTUAL(98);
    ENUM_DUMMY_VIRTUAL(99);  ENUM_DUMMY_VIRTUAL(100); ENUM_DUMMY_VIRTUAL(101);
    ENUM_DUMMY_VIRTUAL(102); ENUM_DUMMY_VIRTUAL(103); ENUM_DUMMY_VIRTUAL(104);
    ENUM_DUMMY_VIRTUAL(105); ENUM_DUMMY_VIRTUAL(106); ENUM_DUMMY_VIRTUAL(107);
    ENUM_DUMMY_VIRTUAL(108); ENUM_DUMMY_VIRTUAL(109); ENUM_DUMMY_VIRTUAL(110);
    ENUM_DUMMY_VIRTUAL(111); ENUM_DUMMY_VIRTUAL(112); ENUM_DUMMY_VIRTUAL(113);
    ENUM_DUMMY_VIRTUAL(114); ENUM_DUMMY_VIRTUAL(115); ENUM_DUMMY_VIRTUAL(116);
    ENUM_DUMMY_VIRTUAL(117); ENUM_DUMMY_VIRTUAL(118); ENUM_DUMMY_VIRTUAL(119);
    ENUM_DUMMY_VIRTUAL(120); ENUM_DUMMY_VIRTUAL(121); ENUM_DUMMY_VIRTUAL(122);
    ENUM_DUMMY_VIRTUAL(123); ENUM_DUMMY_VIRTUAL(124); ENUM_DUMMY_VIRTUAL(125);
    ENUM_DUMMY_VIRTUAL(126); ENUM_DUMMY_VIRTUAL(127); ENUM_DUMMY_VIRTUAL(128);
    ENUM_DUMMY_VIRTUAL(129); ENUM_DUMMY_VIRTUAL(130); ENUM_DUMMY_VIRTUAL(131);
    ENUM_DUMMY_VIRTUAL(132); ENUM_DUMMY_VIRTUAL(133); ENUM_DUMMY_VIRTUAL(134);
    ENUM_DUMMY_VIRTUAL(135); ENUM_DUMMY_VIRTUAL(136); ENUM_DUMMY_VIRTUAL(137);
    ENUM_DUMMY_VIRTUAL(138); ENUM_DUMMY_VIRTUAL(139); ENUM_DUMMY_VIRTUAL(140);
    ENUM_DUMMY_VIRTUAL(141); ENUM_DUMMY_VIRTUAL(142); ENUM_DUMMY_VIRTUAL(143);
    ENUM_DUMMY_VIRTUAL(144); ENUM_DUMMY_VIRTUAL(145); ENUM_DUMMY_VIRTUAL(146);
    ENUM_DUMMY_VIRTUAL(147); ENUM_DUMMY_VIRTUAL(148); ENUM_DUMMY_VIRTUAL(149);
    ENUM_DUMMY_VIRTUAL(150); ENUM_DUMMY_VIRTUAL(151); ENUM_DUMMY_VIRTUAL(152);
    ENUM_DUMMY_VIRTUAL(153); ENUM_DUMMY_VIRTUAL(154); ENUM_DUMMY_VIRTUAL(155);
    ENUM_DUMMY_VIRTUAL(156); ENUM_DUMMY_VIRTUAL(157); ENUM_DUMMY_VIRTUAL(158);
    ENUM_DUMMY_VIRTUAL(159); ENUM_DUMMY_VIRTUAL(160); ENUM_DUMMY_VIRTUAL(161);
    ENUM_DUMMY_VIRTUAL(162); ENUM_DUMMY_VIRTUAL(163); ENUM_DUMMY_VIRTUAL(164);
    ENUM_DUMMY_VIRTUAL(165); ENUM_DUMMY_VIRTUAL(166); ENUM_DUMMY_VIRTUAL(167);
    ENUM_DUMMY_VIRTUAL(168); ENUM_DUMMY_VIRTUAL(169); ENUM_DUMMY_VIRTUAL(170);
    ENUM_DUMMY_VIRTUAL(171); ENUM_DUMMY_VIRTUAL(172);
    virtual bool filter() = 0; // primary vtable +0x2BC

    u32 field_004;
    u8 tagState;
    u8 _pad_009[0x3374 - 0x009];
    u32 flags3374;
    u8 _pad_3378[0x3E9C - 0x3378];
};

class CfEnumActor : public CfEnumActorBase, public CfEnumObject {
public:
    CfEnumObject* getMoveObject() {
        return static_cast<CfEnumObject*>(this);
    }
};

#undef ENUM_DUMMY_VIRTUAL

struct CfEnumKind {
    u8 _pad_00[0x91];
    s8 kind91;
};

extern "C" {
void* func_800B07E8__Fv();
CfEnumObject* func_800B7034();
CfEnumObject* func_800B7058(CfEnumObject*);
CfEnumObject* func_800B76A4();
CfEnumObject* func_800B77E4(CfEnumObject*);
CfEnumObject* func_800B77BC();
CfEnumObject* func_800B7934(CfEnumObject*);
CfEnumObject* func_800B76CC();
CfEnumObject* func_800B781C(CfEnumObject*);
CfEnumObject* func_800B76F4();
CfEnumObject* func_800B7854(CfEnumObject*);
CfEnumObject* func_800B771C();
CfEnumObject* func_800B788C(CfEnumObject*);
CfEnumObject* func_800B7744();
CfEnumObject* func_800B78C4(CfEnumObject*);
CfEnumObject* func_800B776C();
CfEnumObject* func_800B78FC(CfEnumObject*);
CfEnumObject* func_800B7794();
CfEnumObject* func_800B796C(CfEnumObject*);
CfEnumActor* func_8016FE34(CfEnumObject*);
CfEnumActor* func_800BBC0C(CfEnumObject*);
int func_80148778(void*, int);
int func_80051B38(CfEnumActorAux*);
int func_8009CF8C(int);
CfEnumKind* func_800AB3D0(CfEnumObject*);
void* getInstance__Q22cf14CBattleManagerFv();
int func_800DA06C(void*, CfEnumActor*);
CfEnumActor* func_800EA384(void*);
CfEnumActor* func_800EA3AC(void*, CfEnumObject*);
CfEnumObject* func_800FE68C();
CfEnumObject* func_800B708C__Fi(int);
CfEnumObject* getPlayer__Q22cf13CfGameManagerFi(int);
extern const float lbl_eu_80666EB8;
extern const float lbl_eu_80666EBC;
// In-TU definition later (list-rebuild partner check); C-linkage so the
// retail unmangled reloc name matches (same convention as func_800F4B5C).
extern "C" int func_800FD3FC(cf::CfObjEnumList* list, CfEnumActor* aux,
                             ml::CVec3* spot);
void* func_80496264(void* scene, int index);   // scene pose/xform block
void* func_8049627C(void* scene, int flag);    // scene view frame
int func_8049B59C(void* out, void* pose, void* pos); // view-space transform
void getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
    void* rect, const void* frame);
int func_804B1BDC(void* self, void* a, void* b); // aux cone probe (two vecs)
}

// Two register-allocation variants of the append helper.  The retail's
// deferred stores to the info local keep (object, objectId) order in most
// cases, but the object-list loop cases (case1/case100/case4000/case8000/
// case10000/case800) need objectId assigned before object to reproduce the
// retail register allocation in the append block.
#define ENUM_ADD_OBJECT(info_, object_)                                      \
    do {                                                                     \
        (info_).field_18 = false;                                            \
        (info_).objectId = (object_)->objectId;                              \
        (info_).object = (object_);                                          \
        self->mObjInfo[self->mObjInfoCount++] = (info_);                     \
        self->mPtrArray[self->mPtrCount++] =                                 \
            &self->mObjInfo[self->mObjInfoCount - 1];                        \
    } while (false)

// Variant with (object, objectId) assignment order (retail store order).
#define ENUM_ADD_OBJECT_ALT(info_, object_)                                  \
    do {                                                                     \
        (info_).field_18 = false;                                            \
        (info_).object = (object_);                                          \
        (info_).objectId = (object_)->objectId;                              \
        self->mObjInfo[self->mObjInfoCount++] = (info_);                     \
        self->mPtrArray[self->mPtrCount++] =                                 \
            &self->mObjInfo[self->mObjInfoCount - 1];                        \
    } while (false)

#define ENUM_BASIC_FILTERS(rejected_, object_, options_, compare_)           \
    do {                                                                     \
        if (!(rejected_) && !((options_) & 1) &&                             \
            !(object_)->isEnabled()) {                        \
            (rejected_) = true;                                              \
        }                                                                    \
        if (!(rejected_) && ((options_) & 2) &&                              \
            (!(object_)->filterA() ||                         \
             (compare_) != (object_)->filterC())) {           \
            (rejected_) = true;                                              \
        }                                                                    \
    } while (false)

#define ENUM_ACTOR_CORE_FILTERS(rejected_, object_, actor_, options_, zero_, one_) \
    do {                                                                           \
        if (!(rejected_) && !((options_) & 1) && !(object_)->isEnabled()) {        \
            (rejected_) = true;                                                    \
        }                                                                          \
        if (!(rejected_) && ((options_) & 2) &&                                    \
            (!(actor_)->filterA() || (zero_) != (actor_)->filterC() ||             \
             (one_) != (actor_)->filterD() || !(actor_)->filterB())) {             \
            (rejected_) = true;                                                    \
        }                                                                          \
    } while (false)

#define ENUM_ACTOR_FILTERS(rejected_, object_, actor_, options_, zero_, one_) \
    do {                                                                      \
        ENUM_ACTOR_CORE_FILTERS(                                              \
            rejected_, object_, actor_, options_, zero_, one_);              \
        if (!(rejected_) && ((options_) & 0x100) &&                           \
            func_80148778(&(actor_)->tagState, 0xE5)) {                       \
            (rejected_) = true;                                               \
        }                                                                     \
        if (!(rejected_) && ((options_) & 0x200)) {                           \
            if (func_80148778(&(actor_)->tagState, 0xE6)) {                   \
                (rejected_) = true;                                           \
            } else if ((actor_)->actorAux == 0 ||                            \
                       !func_80051B38((actor_)->actorAux)) {                  \
                (rejected_) = true;                                           \
            }                                                                 \
        }                                                                     \
        if (!(rejected_) && ((options_) & 0x800)) {                           \
            if ((actor_)->filter()) {                           \
                (rejected_) = true;                                           \
            }                                                                 \
            if (func_80148778(&(actor_)->tagState, 0xF8)) {                   \
                (rejected_) = true;                                           \
            }                                                                 \
        }                                                                     \
        if (!(rejected_) && ((options_) & 0x1000) &&                          \
            (actor_)->filter() &&                               \
            (actor_)->actorAux->test(0)) {                                    \
            (rejected_) = true;                                               \
        }                                                                     \
        if (!(rejected_) && ((options_) & 0x2000) &&                          \
            !func_800DA06C(getInstance__Q22cf14CBattleManagerFv(), actor_)) { \
            (rejected_) = true;                                               \
        }                                                                     \
    } while (false)

extern "C" void func_800F4B5C(cf::CfObjEnumList* self, int flags, u32 options) {
    cf::CfObjEnumList::sObjInfo info0;
    cf::CfObjEnumList::sObjInfo info1;
    cf::CfObjEnumList::sObjInfo info2;
    cf::CfObjEnumList::sObjInfo info3;
    cf::CfObjEnumList::sObjInfo info4;
    cf::CfObjEnumList::sObjInfo info5;
    cf::CfObjEnumList::sObjInfo info6;
    cf::CfObjEnumList::sObjInfo info7;
    cf::CfObjEnumList::sObjInfo info8;
    cf::CfObjEnumList::sObjInfo info9;
    cf::CfObjEnumList::sObjInfo info10;
    cf::CfObjEnumList::sObjInfo info11;
    cf::CfObjEnumList::sObjInfo info12;
    cf::CfObjEnumList::sObjInfo info13;
    cf::CfObjEnumList::sObjInfo info14;
    cf::CfObjEnumList::sObjInfo info15;
    cf::CfObjEnumList::sObjInfo info16;
    cf::CfObjEnumList::sObjInfo info17;
    cf::CfObjEnumList::sObjInfo info18;
    bool rejected;
    CfEnumObject* object;
    CfEnumActor* actor;

    if (flags == 0) {
        return;
    }

    if (flags & 1) {
        func_800B07E8__Fv();
        object = func_800B7034();
        float zero = lbl_eu_80666EB8;
        float one = lbl_eu_80666EBC;
        while (object != 0) {
            rejected = false;
            if (!(options & 1) && !object->isEnabled()) {
                rejected = true;
            }
            if (!rejected && (options & 2) &&
                (!object->filterA() ||
                 zero != object->filterC())) {
                rejected = true;
            }
            if (!rejected && flags == 0x200 &&
                (object->filterD() < one ||
                 (object->field68 & 0x4800))) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B7058(object);
                continue;
            }
            ENUM_ADD_OBJECT(info0, object);
            func_800B07E8__Fv();
            object = func_800B7058(object);
        }
        return;
    } else if (flags & 4) {
        CfEnumObject* selector = func_800FE68C();
        CfEnumObject* object = *(CfEnumObject**)((u8*)selector + 0x90E4);
        if (object != 0) {
            object = func_800B708C__Fi((int)object);
            ENUM_ADD_OBJECT_ALT(info1, object);
        }
        flags &= ~4u;
    } else if (flags & 0x10) {
        CfEnumObject* object = getPlayer__Q22cf13CfGameManagerFi(0);
        if (object != 0) {
            ENUM_ADD_OBJECT_ALT(info2, object);
        }
        flags &= ~0x10u;
    } else if (flags & 0x40000) {
        func_800F4B5C(self, 4, options);
        if (self->mPtrCount == 0) {
            func_800F4B5C(self, 0x10, options);
        }
        flags &= ~0x40000u;
    } else if (flags & 0x20) {
        func_800B07E8__Fv();
        object = func_800B76A4();
        float one = lbl_eu_80666EBC;
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            rejected = false;
            actor = func_8016FE34(object);
            if (!(object->flags64 & 2)) {
                rejected = true;
            }
            ENUM_ACTOR_CORE_FILTERS(rejected, object, actor, options, zero, one);
            if (!rejected && (options & 0x100) &&
                func_80148778(&actor->tagState, 0xE5)) {
                rejected = true;
            }
            if (!rejected && (options & 0x200)) {
                if (func_80148778(&actor->tagState, 0xE6)) {
                    rejected = true;
                } else if (actor->actorAux == 0 ||
                           !func_80051B38(actor->actorAux)) {
                    rejected = true;
                }
            }
            if (!rejected && (options & 0x800)) {
                if (actor->filter()) {
                    rejected = true;
                }
                if (func_80148778(&actor->tagState, 0xF8)) {
                    rejected = true;
                }
            }
            if (!rejected && (options & 0x1000) &&
                actor->filter()) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B77E4(object);
                continue;
            }
            ENUM_ADD_OBJECT_ALT(info3, object);
            func_800B07E8__Fv();
            object = func_800B77E4(object);
        }
        flags &= ~0x20u;
    } else if (flags & 0x100000) {
        CfEnumObject* object = getPlayer__Q22cf13CfGameManagerFi(1);
        if (object != 0) {
            ENUM_ADD_OBJECT_ALT(info4, object);
        }
        flags &= ~0x100000u;
    } else if (flags & 0x200000) {
        CfEnumObject* object = getPlayer__Q22cf13CfGameManagerFi(2);
        if (object != 0) {
            ENUM_ADD_OBJECT_ALT(info5, object);
        }
        flags &= ~0x200000u;
    } else if (flags & 8) {
        func_800B07E8__Fv();
        object = func_800B77BC();
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            rejected = false;
            if (!(object->flags64 & 0x200)) {
                rejected = true;
            }
            ENUM_BASIC_FILTERS(rejected, object, options, zero);
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B7934(object);
                continue;
            }
            ENUM_ADD_OBJECT_ALT(info6, object);
            break;
        }
        flags &= ~8u;
    } else if (flags & 0x100) {
        func_800B07E8__Fv();
        object = func_800B76CC();
        float one = lbl_eu_80666EBC;
        while (object != 0) {
            rejected = false;
            actor = func_8016FE34(object);
            if (!(object->flags64 & 4)) {
                rejected = true;
            }
            if (!rejected && (actor->flags3374 & 0x2000)) {
                rejected = true;
            }
            if (!rejected && !(options & 1) &&
                (!object->isEnabled() ||
                 (actor->flags3374 & 0x08000000))) {
                rejected = true;
            }
            if (!rejected && (options & 2) &&
                (!actor->filterA() ||
                 one != actor->filterD() ||
                 !actor->filterB())) {
                rejected = true;
            }
            if (!rejected && (options & 0x100) &&
                func_80148778(&actor->tagState, 0xE5)) {
                rejected = true;
            }
            if (!rejected && (options & 0x200)) {
                if (func_80148778(&actor->tagState, 0xE6)) {
                    rejected = true;
                } else if (actor->actorAux == 0 ||
                           !func_80051B38(actor->actorAux)) {
                    rejected = true;
                }
            }
            if (!rejected && (options & 0x800)) {
                if (actor->filter()) {
                    rejected = true;
                }
                if (func_80148778(&actor->tagState, 0xF8)) {
                    rejected = true;
                }
            }
            if (!rejected && (options & 0x1000) &&
                actor->filter() && actor->actorAux->test(0)) {
                rejected = true;
            }
            if (!rejected && (options & 0x2000) &&
                !func_800DA06C(getInstance__Q22cf14CBattleManagerFv(), actor)) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B781C(object);
                continue;
            }
            ENUM_ADD_OBJECT(info7, object);
            func_800B07E8__Fv();
            object = func_800B781C(object);
        }
        flags &= ~0x100u;
    } else if (flags & 0x400000) {
        func_800B07E8__Fv();
        object = func_800B76CC();
        float one = lbl_eu_80666EBC;
        while (object != 0) {
            rejected = false;
            actor = func_8016FE34(object);
            if (!(object->flags64 & 4)) {
                rejected = true;
            }
            if (!rejected && !(actor->flags3374 & 0x2000)) {
                rejected = true;
            }
            if (!rejected && !(options & 1) &&
                (!object->isEnabled() ||
                 (actor->flags3374 & 0x08000000))) {
                rejected = true;
            }
            if (!rejected && (options & 2) &&
                (!actor->filterA() ||
                 one != actor->filterD() ||
                 !actor->filterB())) {
                rejected = true;
            }
            if (!rejected && (options & 0x2000) &&
                !func_800DA06C(getInstance__Q22cf14CBattleManagerFv(), actor)) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B781C(object);
                continue;
            }
            ENUM_ADD_OBJECT_ALT(info8, object);
            func_800B07E8__Fv();
            object = func_800B781C(object);
        }
        flags &= ~0x400000u;
    } else if (flags & 0x200) {
        func_800B07E8__Fv();
        object = func_800B76F4();
        float one = lbl_eu_80666EBC;
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            rejected = false;
            actor = func_800BBC0C(object);
            if (!(object->flags64 & 8)) {
                rejected = true;
            }
            if (!rejected && !(options & 1) && !object->isEnabled()) {
                rejected = true;
            }
            CfEnumObject* actorObject = reinterpret_cast<CfEnumObject*>(actor);
            if (!rejected && (options & 2) &&
                (!actorObject->filterA() || zero != actorObject->filterC() ||
                 one != actorObject->filterD() || !actorObject->filterB())) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B7854(object);
                continue;
            }
            ENUM_ADD_OBJECT_ALT(info9, object);
            func_800B07E8__Fv();
            object = func_800B7854(object);
        }
        flags &= ~0x200u;
    } else if (flags & 0x400) {
        func_800B07E8__Fv();
        object = func_800B771C();
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            rejected = false;
            if (!(object->flags64 & 0x80)) {
                rejected = true;
            }
            ENUM_BASIC_FILTERS(rejected, object, options, zero);
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B788C(object);
                continue;
            }
            ENUM_ADD_OBJECT_ALT(info10, object);
            func_800B07E8__Fv();
            object = func_800B788C(object);
        }

        func_800B07E8__Fv();
        object = func_800B76CC();
        float one = lbl_eu_80666EBC;
        float secondZero = lbl_eu_80666EB8;
        while (object != 0) {
            rejected = false;
            actor = func_8016FE34(object);
            if (!(object->flags64 & 4)) {
                rejected = true;
            }
            if (!rejected && !(actor->flags3374 & 0x2000)) {
                rejected = true;
            }
            if (!rejected && !(options & 1) && !object->isEnabled()) {
                rejected = true;
            }
            if (!rejected && (options & 2) &&
                (!actor->filterA() || secondZero != actor->filterC() ||
                 one != actor->filterD() || !actor->filterB())) {
                rejected = true;
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B781C(object);
                continue;
            }
            ENUM_ADD_OBJECT_ALT(info11, object);
            func_800B07E8__Fv();
            object = func_800B781C(object);
        }
        flags &= ~0x400u;
    } else if (flags & 0x80000) {
        func_800B07E8__Fv();
        object = func_800B7744();
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            rejected = false;
            if (!(object->flags64 & 0x20)) {
                rejected = true;
            }
            ENUM_BASIC_FILTERS(rejected, object, options, zero);
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B78C4(object);
                continue;
            }
            ENUM_ADD_OBJECT_ALT(info12, object);
            func_800B07E8__Fv();
            object = func_800B78C4(object);
        }
        flags &= ~0x80000u;
    } else if (flags & 0x800) {
        func_800B07E8__Fv();
        object = func_800B776C();
        float zero = lbl_eu_80666EB8;
        bool rejected = false;
        while (object != 0) {
            rejected = false;
            if (!(object->flags64 & 0x4000) &&
                !(object->flags64 & 0x8000)) {
                rejected = true;
            }
            ENUM_BASIC_FILTERS(rejected, object, options, zero);
            if (!rejected && (options & 0x8000) &&
                (u16)(object->type8C + 0xFED1) <= 1) {
                rejected = true;
            }
            if (!rejected) {
                bool unavailable = func_8009CF8C(0x3354) == 0;
                if (unavailable) {
                    CfEnumKind* kind = func_800AB3D0(object);
                    if (kind != 0 && kind->kind91 == 6) {
                        rejected = true;
                    }
                }
            }
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B78FC(object);
                continue;
            }
            ENUM_ADD_OBJECT(info13, object);
            func_800B07E8__Fv();
            object = func_800B78FC(object);
        }
        flags &= ~0x800u;
    } else if (flags & 0x1000) {
        func_800B07E8__Fv();
        object = func_800B7794();
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            rejected = false;
            if (!(object->flags64 & 0x100)) {
                rejected = true;
            }
            ENUM_BASIC_FILTERS(rejected, object, options, zero);
            if (rejected) {
                func_800B07E8__Fv();
                object = func_800B796C(object);
                continue;
            }
            ENUM_ADD_OBJECT_ALT(info14, object);
            func_800B07E8__Fv();
            object = func_800B796C(object);
        }
        flags &= ~0x1000u;
    } else if (flags & 0x4000) {
        void* manager = getInstance__Q22cf14CBattleManagerFv();
        CfEnumActor* source = func_800EA384(manager);
        object = source;
        float one = lbl_eu_80666EBC;
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            rejected = false;
            actor = func_8016FE34(object);
            ENUM_ACTOR_FILTERS(rejected, object, actor, options, zero, one);
            if (rejected) {
                manager = getInstance__Q22cf14CBattleManagerFv();
                source = func_800EA3AC(manager, object);
                object = source;
                continue;
            }
            ENUM_ADD_OBJECT(info15, object);
            manager = getInstance__Q22cf14CBattleManagerFv();
            source = func_800EA3AC(manager, object);
            object = source;
        }
        flags &= ~0x4000u;
    } else if (flags & 0x8000) {
        void* manager = getInstance__Q22cf14CBattleManagerFv();
        CfEnumActor* source = func_800EA384(manager);
        object = source;
        float one = lbl_eu_80666EBC;
        float zero = lbl_eu_80666EB8;
        while (object != 0) {
            rejected = false;
            actor = func_8016FE34(object);
            if (!(object->flags64 & 2)) {
                rejected = true;
            }
            ENUM_ACTOR_FILTERS(rejected, object, actor, options, zero, one);
            if (rejected) {
                manager = getInstance__Q22cf14CBattleManagerFv();
                source = func_800EA3AC(manager, object);
                object = source;
                continue;
            }
            ENUM_ADD_OBJECT(info16, object);
            manager = getInstance__Q22cf14CBattleManagerFv();
            source = func_800EA3AC(manager, object);
            object = source;
        }
        flags &= ~0x8000u;
    } else if (flags & 0x10000) {
        void* manager = getInstance__Q22cf14CBattleManagerFv();
        CfEnumActor* source = func_800EA384(manager);
        object = source;
        float zero = lbl_eu_80666EB8;
        float one = lbl_eu_80666EBC;
        while (object != 0) {
            rejected = false;
            actor = func_8016FE34(object);
            if (object->flags64 & 4) {
                rejected = true;
            }
            ENUM_ACTOR_FILTERS(rejected, object, actor, options, zero, one);
            if (rejected) {
                manager = getInstance__Q22cf14CBattleManagerFv();
                source = func_800EA3AC(manager, object);
                object = source;
                continue;
            }
            ENUM_ADD_OBJECT(info17, object);
            manager = getInstance__Q22cf14CBattleManagerFv();
            source = func_800EA3AC(manager, object);
            object = source;
        }
        flags &= ~0x10000u;
    } else if (flags & 0x20000) {
        void* manager = getInstance__Q22cf14CBattleManagerFv();
        CfEnumActor* source = func_800EA384(manager);
        object = source;
        while (object != 0) {
            rejected = false;
            func_800BBC0C(object);
            if (object->flags64 & 8) {
                rejected = true;
            }
            if (!rejected && !(options & 1) && !object->isEnabled()) {
                rejected = true;
            }
            if (rejected) {
                manager = getInstance__Q22cf14CBattleManagerFv();
                source = func_800EA3AC(manager, object);
                object = source;
                continue;
            }
            ENUM_ADD_OBJECT_ALT(info18, object);
            manager = getInstance__Q22cf14CBattleManagerFv();
            source = func_800EA3AC(manager, object);
            object = source;
        }
        flags &= ~0x20000u;
    } else {
        return;
    }

    func_800F4B5C(self, flags, options);
}

#undef ENUM_ACTOR_FILTERS
#undef ENUM_ACTOR_CORE_FILTERS
#undef ENUM_BASIC_FILTERS
#undef ENUM_ADD_OBJECT

// func_800F6D50: resolve an object by id and append it to this list.  The
// local sObjInfo is only partially initialized (field_18/objectId/object);
// the remaining fields are copied as-is from the local's stack slot, matching
// the retail garbage-field copy.
void func_800F6D50(cf::CfObjEnumList* self, int param) {
    CfEnumObject* obj = func_800B708C__Fi(param);
    if (obj == 0) {
        return;
    }
    cf::CfObjEnumList::sObjInfo info;
    info.object = obj;
    info.field_18 = false;
    info.objectId = obj->objectId;
    self->mObjInfo[self->mObjInfoCount++] = info;
    self->mPtrArray[self->mPtrCount++] =
        &self->mObjInfo[self->mObjInfoCount - 1];
}

// func_800F6E08: find the first entry (cycling from index 0 through
// count-1) whose field_18 has none of the 0x70 flag bits set; return its
// objectId, or 0 if every entry is flagged.
void* func_800F6E08(cf::CfObjEnumList* self) {
    volatile u32* countPtr = &self->mPtrCount;
    u32 count = *countPtr;
    if (count == 0) {
        return 0;
    }
    int last = count - 1;
    int i = (last + 1) % count;
    while (last != i) {
        cf::CfObjEnumList::sObjInfo* obj = self->mPtrArray[i];
        if ((obj->field_18 & 0x70) == 0) {
            return (void*)obj->objectId;
        }
        i = (i + 1) % *countPtr;
    }
    cf::CfObjEnumList::sObjInfo* obj = self->mPtrArray[last];
    if ((obj->field_18 & 0x70) != 0) {
        return 0;
    }
    return (void*)obj->objectId;
}

// func_800F8794: quicksort pivot pick.  Scan (lo, hi] for the first index
// whose sort value differs from value(lo); return that index, or lo when
// value(lo) >= value(p) (descending tie-break), or -1 when the whole range
// is equal to value(lo).  Retail symbols are unmangled globals.
extern "C" int func_800F8794(CfSortableList* self, int lo, int hi);

// func_800F8890: Hoare partition (defined later in this TU).
extern "C" int func_800F8890(CfSortableList* self, int lo, int hi, int pivot);

extern "C" int func_800F8794(CfSortableList* self, int lo, int hi) {
    int p = lo + 1;
    while (p <= hi && self->value(lo) == self->value(p)) {
        p++;
    }
    if (p > hi) {
        return -1;
    }
    if (self->value(lo) >= self->value(p)) {
        return lo;
    }
    return p;
}

// func_800F7DEC: quicksort recursion over the sortable list.  With the unit's
// -inline auto, MWCC expands func_800F8794/func_800F8890 at every call site,
// producing the retail multi-stage inline blob.
extern "C" void func_800F7DEC(CfSortableList* self, int lo, int hi) {
    if (lo == hi) {
        return;
    }
    int p = func_800F8794(self, lo, hi);
    if (p == -1) {
        return;
    }
    int m = func_800F8890(self, lo, hi, p);
    if (lo != m - 1) {
        func_800F7DEC(self, lo, m - 1);
    }
    if (m != hi) {
        func_800F7DEC(self, m, hi);
    }
}

// func_800F8890: Hoare partition of the sortable list.  The pivot value is
// read via value(pivot), then the list is scanned from both ends (value(i) <
// pivot on the left, value(j) >= pivot on the right) and out-of-place
// elements are swapped through the get() accessor.  Returns the split index.
extern "C" int func_800F8890(CfSortableList* self, int lo, int hi, int pivot) {
    int i = lo;
    int j = hi;
    float pivotValue = self->value(pivot);
    while (i <= j) {
        while (i <= hi && self->value(i) < pivotValue) {
            i++;
        }
        while (j >= lo && self->value(j) >= pivotValue) {
            j--;
        }
        if (i > j) {
            break;
        }
        void* tmp = *reinterpret_cast<void**>(self->get(i));
        *reinterpret_cast<void**>(self->get(i)) =
            *reinterpret_cast<void**>(self->get(j));
        *reinterpret_cast<void**>(self->get(j)) = tmp;
        i++;
        j--;
    }
    return i;
}

void func_800F89DC(){}

// Shared sort tail of the two list-refresh entry points (func_800F9AEC /
// func_800F800F6ED0 family): after the per-entry value refresh, run the
// quicksort over the embedded sortable list at +0x624.  Retail expands the
// first partition stages inline; the structure here mirrors the observed
// nesting (pivot find -> partition -> nested pivot/partition on each half,
// deeper levels recurse into func_800F7DEC).
static void enumRefreshSort(cf::CfObjEnumList* self, u32 sortFlag) {
    self->field_062C = sortFlag;
    int hi = self->mPtrCount - 1;
    if (hi == 0) {
        return;
    }
    CfSortableList* sl = reinterpret_cast<CfSortableList*>(&self->mSortVtableA);
    int pivot = func_800F8794(sl, 0, hi);
    if (pivot == -1) {
        return;
    }
    int m = func_800F8890(sl, 0, hi, pivot);
    if (0 != m - 1) {
        int p2 = func_800F8794(sl, 0, m - 1);
        if (p2 != -1) {
            int m2 = func_800F8890(sl, 0, m - 1, p2);
            if (0 != m2 - 1) {
                func_800F7DEC(sl, 0, m2 - 1);
            }
            if (m2 != m - 1) {
                func_800F7DEC(sl, m2, m - 1);
            }
        }
    }
    if (m != hi) {
        int p3 = func_800F8794(sl, m, hi);
        if (p3 != -1) {
            int m3 = func_800F8890(sl, m, hi, p3);
            if (m != m3 - 1) {
                func_800F7DEC(sl, m, m3 - 1);
            }
            if (m3 != hi) {
                func_800F7DEC(sl, m3, hi);
            }
        }
    }
}

// func_800F9AEC: refresh each entry's sort value via the actor's vtable
// 0x128 float accessor (reached through func_8016FE34), then sort the list.
// Needs at least two entries to do anything.
void func_800F9AEC(cf::CfObjEnumList* self) {
    if (self->mPtrCount < 2) {
        return;
    }
    for (u32 i = 0; i < self->mPtrCount; i++) {
        cf::CfObjEnumList::sObjInfo* p = self->mPtrArray[i];
        CfEnumActor* actor = func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor != NULL) {
            p->field_14 = reinterpret_cast<CfEnumActorValueView*>(actor)->value128();
        }
    }
    enumRefreshSort(self, 1);
}

// func_800F6ED0: recompute each entry's distance from spot (vtable 0xAC
// position vector minus spot, PSVECMag), clear field_062C, then sort.
void func_800F6ED0(cf::CfObjEnumList* self, ml::CVec3* spot) {
    if (self->mPtrCount < 2) {
        return;
    }
    for (u32 i = 0; i < self->mPtrCount; i++) {
        cf::CfObjEnumList::sObjInfo* p = self->mPtrArray[i];
        nw4r::math::VEC3* pos = reinterpret_cast<nw4r::math::VEC3*>(
            reinterpret_cast<CfEnumObjPosView*>(p->object)->vAC());
        nw4r::math::VEC3 delta;
        nw4r::math::VEC3Sub(&delta, pos,
                            reinterpret_cast<const nw4r::math::VEC3*>(spot));
        nw4r::math::VEC3 d;
        d.x = delta.x;
        d.y = delta.y;
        d.z = delta.z;
        p->field_14 = PSVECMag((const Vec*)&d);
    }
    enumRefreshSort(self, 0);
}

// __ct__800FA9B4: rebuild the list by projecting every object position through
// the scene's view transform: func_80496264/func_8049627C fetch the pose block
// and view frame, func_8049B59C writes the view-space position into info+8 for
// two probe slots, then a screen-rect depth gate (getFrame2ViewOffset +
// func_804BE348) and the func_804B5088 probe decide acceptance.  Accepted
// entries get the 0x10 mark bit; every entry is re-appended.
void __ct__800FA9B4(cf::CfObjEnumList* list, void* scene, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    void* pose = func_80496264(scene, 0);
    u8* frame = static_cast<u8*>(func_8049627C(scene, 0));
    struct RebuildCtx {
        u32 ownsCopy;                          // +0x000 (<- list+0x1C)
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x004
        u32 count;                             // +0x604
        u8* b;                                 // +0x608
        u8* a;                                 // +0x60C
        u32 field;                             // +0x610
    };
    RebuildCtx ctx;
    ctx.ownsCopy = list->mOwnsList;
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    list->mPtrCount = 0;
    const float c02 = lbl_eu_80666EC8; // f28: 0.2 depth scale
    const float c01 = lbl_eu_80666ECC; // f31
    const double magic = lbl_eu_80666EC0; // s32->double conversion magic
    (void)c01;
    (void)magic;
    // Frame-origin shorts + probe-table record layout.
    struct ViewFrameBlock {
        u8 _pad[0x1C8];
        s16 originX;             // 0x1C8
        s16 originY;             // 0x1CA
        u8 _pad2[0x230 - 0x1CC];
        s16 sizeX;               // 0x230
    };
    ViewFrameBlock* vf = reinterpret_cast<ViewFrameBlock*>(frame);
    for (u32 i = 0; i < ctx.count; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumObjPosView* obj = reinterpret_cast<CfEnumObjPosView*>(p->object);
        // Three stacked position temps; v128 fills all three, the vAC fallback
        // only the latter two (retail store pattern).
        ml::CVec3 tmp[3];
        CfEnumPosBlock* v = obj->v128();
        if (v != 0) {
            float tz = v->z;
            float ty = v->y;
            float tx = v->x;
            tmp[0].x = tx;
            tmp[0].y = ty;
            tmp[0].z = tz;
            tmp[1] = tmp[0];
            tmp[2] = tmp[0];
        } else {
            ml::CVec3* src =
                reinterpret_cast<ml::CVec3*>(obj->vAC());
            tmp[1].x = src->x;
            tmp[1].y = src->y;
            tmp[1].z = src->z;
            tmp[2] = tmp[1];
        }
        bool confirmed = false;
        for (int k = 0; k < 2; k++) {
            // View-space transform lands in info.field_08/0C/10.
            int ret = func_8049B59C(&p->field_08, pose, &tmp[1 + k]);
            p->field_18 |= 1;
            if (ret != 0) {
                // Screen-rect depth gate: offset the frame rect by the frame's
                // stored origin shorts and scale by 0.2 (retail converts the
                // combined s16 via the 2^52 double idiom).
                struct SRect16 {
                    s16 x, y, w, h;
                } rc;
                getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
                    &rc, frame + 0x1DC);
                s16 fx = vf->originX;
                s16 fy = vf->originY;
                s16 fw = vf->sizeX;
                float vx = (float)(fw + rc.x + fx + (-20)) * c02;
                float vy = (float)(fy + rc.y) * c02;
                ml::CVec3 probe;
                probe.x = vx;
                probe.y = vy;
                probe.z = p->field_10;
                if (func_804BE348(&probe, &probe, 0x44A45, 0, 0) != 0) {
                    confirmed = false;
                    break;
                }
            }
            // Global probe table walk (12-byte records indexed by slot k).
            confirmed = func_804B5088(
                            reinterpret_cast<cf::CfCamFollow*>((&lbl_eu_80665958)[k]),
                            (u8*)pose + 0x10C, &tmp[1 + k], 1, 0) == 0;
            if (confirmed) {
                break;
            }
        }
        bool accept = !confirmed;
        if (options & 2) {
            accept = confirmed;
        }
        if (accept) {
            p->field_18 |= 0x10;
        }
        list->mPtrArray[list->mPtrCount++] = p;
    }
}

// __ct__800FAE3C: rebuild the object list keeping only entries whose position
// is NOT accepted by the collision probe: func_804BE348(arg1, pos, 0x44A45,
// 0, 0), falling back to func_804B5088(globalProbe, pos, arg1, 1, 0).  The
// position comes from vtable 0x12C(0x64)/0x128 (floats at +0xC/+0x1C/+0x2C)
// or, if those return null, vtable 0xAC (plain vec3).  Mark bit 0x10.
void __ct__800FAE3C(cf::CfObjEnumList* list, ml::CVec3* arg1, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        u8* b;                                 // +0x604
        u8* a;                                 // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumObjPosView* objView =
            reinterpret_cast<CfEnumObjPosView*>(p->object);
        CfEnumPosBlock* v = objView->v12C(0x64);
        if (v == 0) {
            v = objView->v128();
        }
        ml::CVec3 vec;   // declared first -> higher frame slot (retail sp+20)
        ml::CVec3 tmp;   // second         -> lower frame slot (retail sp+8)
        ml::CVec3* src;
        if (v != 0) {
            // 3-float copy: locals declared z,y,x -> FPRs f0,f1,f2 (retail
            // load-descending/store-ascending shape).
            float tz = v->z;
            float ty = v->y;
            float tx = v->x;
            tmp.x = tx;
            tmp.y = ty;
            tmp.z = tz;
            src = &tmp;
        } else {
            src = reinterpret_cast<ml::CVec3*>(objView->vAC());
        }
        vec.x = src->x;
        vec.y = src->y;
        vec.z = src->z;
        bool flag = false;
        if (func_804BE348(arg1, &vec, 0x44A45, 0, 0) != 0) {
            flag = true;
        }
        if (!flag) {
            flag = (flag |
                    func_804B5088(
                        reinterpret_cast<cf::CfCamFollow*>(lbl_eu_80665958),
                        &vec, arg1, 1, 0)) != 0;
        }
        if (options & 2) {
            flag = !flag;
        }
        if (flag) {
            p->field_18 |= 0x10;
            if (options & 1) {
                list->mPtrArray[list->mPtrCount++] = p;
            }
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// __ct__800FB044: rebuild the object list by distance from spot.  dist =
// PSVECMag(obj->vAC() - spot), minus aux->field_63C when aux exists.  The
// entry is kept (mark 0x20) when dist < threshold and either a collision is
// found (func_804B1AD8 on aux+0x60C) or func_800FD3FC accepts it (options
// bit 3).
void __ct__800FB044(cf::CfObjEnumList* list, ml::CVec3* spot, u32 options,
                    float threshold) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        u8* b;                                 // +0x604
        u8* a;                                 // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    for (u32 i = 0; i < ctx.count; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumObject* obj = reinterpret_cast<CfEnumObject*>(p->object);
        CfEnumActor* aux = func_800BBC0C(obj);
        nw4r::math::VEC3 delta;
        nw4r::math::VEC3Sub(
            &delta,
            reinterpret_cast<nw4r::math::VEC3*>(
                reinterpret_cast<CfEnumObjPosView*>(obj)->vAC()),
            reinterpret_cast<const nw4r::math::VEC3*>(spot));
        nw4r::math::VEC3 d1;
        nw4r::math::VEC3 d2;
        d1.x = delta.x;
        d1.y = delta.y;
        d1.z = delta.z;
        d2.x = delta.x;
        d2.y = delta.y;
        d2.z = delta.z;
        p->field_14 = PSVECMag((const Vec*)&d2);
        if (aux != 0) {
            p->field_14 -=
                reinterpret_cast<CfEnumActorView*>(aux)->field_63C;
        }
        bool flag = p->field_14 < threshold;
        if ((obj->field68 & 4) && flag && aux != 0) {
            flag = func_804B1AD8(
                       &reinterpret_cast<CfEnumActorView*>(aux)->field_60C,
                       spot, threshold) != 0;
        }
        if (!flag && (options & 8)) {
            flag = func_800FD3FC(list, aux, spot) != 0;
        }
        if (options & 2) {
            flag = !flag;
        }
        if (flag) {
            p->field_18 |= 0x20;
            if (options & 1) {
                list->mPtrArray[list->mPtrCount++] = p;
            }
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// us-800fbd58 | func_800FB270: rebuild the list by distance + facing cone from
// spot.  dist = PSVECMag(obj->vAC() - spot); the heading is wrapped into
// [-pi, pi] and must fall inside +-cone*halfScale of angleOffset (arg3) while
// targetDist (range + aux radius) reaches dist.  Options bit 2 inverts the
// keep decision; kept entries get the 0x40 mark and are only re-appended when
// flags bit 0 is set (otherwise appended unconditionally).
void func_800FB270(cf::CfObjEnumList* self, ml::CVec3* spot, u32 flags,
                   float range, float cone, float angleOffset) {
    if (self->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        void* b;                               // +0x604
        void* a;                               // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(self->mPtrArray);
    ctx.count = self->mPtrCount;
    ctx.a = &lbl_eu_8052BDA0[0];
    ctx.b = &lbl_eu_8052BDA0[8];
    ctx.field = self->field_062C;
    self->mPtrCount = 0;
    const float pi = lbl_eu_8066A1F8;        // f26
    const float twoPi = lbl_eu_8066A1FC;     // f25
    const float angScale = lbl_eu_80666ED0;  // f24
    const float halfScale = lbl_eu_80666ED4; // f28
    u32 invert = flags & 2;
    for (u32 i = 0; i < ctx.count; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumObject* obj = reinterpret_cast<CfEnumObject*>(p->object);
        CfEnumActor* aux = func_800BBC0C(obj);
        float targetDist = range;
        if (aux != 0) {
            targetDist = range +
                reinterpret_cast<CfEnumActorView*>(aux)->field_63C;
        }
        nw4r::math::VEC3* pos = reinterpret_cast<nw4r::math::VEC3*>(
            reinterpret_cast<CfEnumObjPosView*>(obj)->vAC());
        nw4r::math::VEC3 delta;
        nw4r::math::VEC3Sub(&delta, pos,
                            reinterpret_cast<const nw4r::math::VEC3*>(spot));
        nw4r::math::VEC3 d;
        d.x = delta.x;
        d.y = delta.y;
        d.z = delta.z;
        float dist = PSVECMag((const Vec*)&d);
        float ang = angScale * nw4r::math::Atan2FIdx(d.x, d.z) - angleOffset;
        while (pi <= ang) {
            ang -= twoPi;
        }
        while (ang < -pi) {
            ang += twoPi;
        }
        bool hit = targetDist >= dist && -cone * halfScale <= ang &&
                   ang <= cone * halfScale;
        if ((obj->field68 & 4) && hit && aux != 0) {
            hit = func_804B1C9C(
                      &reinterpret_cast<CfEnumActorView*>(aux)->field_60C,
                      spot, range, cone, angleOffset) != 0;
        }
        if (!hit && (flags & 8)) {
            hit = func_800FD3FC(self, aux, spot) != 0;
        }
        if (invert) {
            hit = !hit;
        }
        if (hit) {
            p->field_18 |= 0x40;
            if (flags & 1) {
                self->mPtrArray[self->mPtrCount++] = p;
            }
        } else {
            self->mPtrArray[self->mPtrCount++] = p;
        }
    }
}

// us-800fc094 | func_800FB5AC: like func_800FB270 but with a second probe
// pass: when the first cone test hits, the position/heading are recomputed
// and tested against a second offset (angleMax) with half-angle cone (cone2).
// The aux probe needs BOTH cone pairs to fail before hit is cleared.
void func_800FB5AC(cf::CfObjEnumList* self, ml::CVec3* spot, u32 flags,
                   float range, float cone, float angleOffset,
                   float cone2, float angleMax) {
    if (self->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        void* b;                               // +0x604
        void* a;                               // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(self->mPtrArray);
    ctx.count = self->mPtrCount;
    ctx.a = &lbl_eu_8052BDA0[0];
    ctx.b = &lbl_eu_8052BDA0[8];
    ctx.field = self->field_062C;
    self->mPtrCount = 0;
    const float pi = lbl_eu_8066A1F8;         // f22
    const float twoPi = lbl_eu_8066A1FC;      // f21
    const float angScale = lbl_eu_80666ED0;   // f29
    const float halfScale = lbl_eu_80666ED4;  // f31
    u32 invert = flags & 2;
    for (u32 i = 0; i < ctx.count; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumObject* obj = reinterpret_cast<CfEnumObject*>(p->object);
        CfEnumActor* aux = func_800BBC0C(obj);
        float targetDist = range;
        if (aux != 0) {
            targetDist = range +
                reinterpret_cast<CfEnumActorView*>(aux)->field_63C;
        }
        nw4r::math::VEC3* pos = reinterpret_cast<nw4r::math::VEC3*>(
            reinterpret_cast<CfEnumObjPosView*>(obj)->vAC());
        nw4r::math::VEC3 delta;
        nw4r::math::VEC3Sub(&delta, pos,
                            reinterpret_cast<const nw4r::math::VEC3*>(spot));
        nw4r::math::VEC3 d;
        d.x = delta.x;
        d.y = delta.y;
        d.z = delta.z;
        float dist = PSVECMag((const Vec*)&d);
        float ang = angScale * nw4r::math::Atan2FIdx(d.x, d.z) - angleOffset;
        while (pi <= ang) {
            ang -= twoPi;
        }
        while (ang < -pi) {
            ang += twoPi;
        }
        bool hit = targetDist >= dist && -cone * halfScale <= ang &&
                   ang <= cone * halfScale;
        if (hit) {
            // Second probe pass: recompute against the angleMax offset and
            // the cone2 half-angle.
            pos = reinterpret_cast<nw4r::math::VEC3*>(
                reinterpret_cast<CfEnumObjPosView*>(obj)->vAC());
            nw4r::math::VEC3Sub(&delta, pos,
                                reinterpret_cast<const nw4r::math::VEC3*>(spot));
            nw4r::math::VEC3 d2;
            d2.x = delta.x;
            d2.y = delta.y;
            d2.z = delta.z;
            dist = PSVECMag((const Vec*)&d2);
            ang = angScale * nw4r::math::Atan2FIdx(d2.x, d2.z) - angleMax;
            while (pi <= ang) {
                ang -= twoPi;
            }
            while (ang < -pi) {
                ang += twoPi;
            }
            hit = targetDist >= dist && -cone2 * halfScale <= ang &&
                  ang <= cone2 * halfScale;
        }
        if ((obj->field68 & 4) && hit && aux != 0) {
            // Both cone pairs must fail to clear the hit.
            hit = func_804B1C9C(
                      &reinterpret_cast<CfEnumActorView*>(aux)->field_60C,
                      spot, range, cone, angleOffset) == 0 &&
                  func_804B1C9C(
                      &reinterpret_cast<CfEnumActorView*>(aux)->field_60C,
                      spot, range, cone2, angleMax) == 0;
        }
        if (!hit && (flags & 8)) {
            hit = func_800FD3FC(self, aux, spot) != 0;
        }
        if (invert) {
            hit = !hit;
        }
        if (hit) {
            p->field_18 |= 0x40;
            if (flags & 1) {
                self->mPtrArray[self->mPtrCount++] = p;
            }
        } else {
            self->mPtrArray[self->mPtrCount++] = p;
        }
    }
}

// __ct__800FBA18: rebuild the list against an oriented range/cone test built
// from spot + three floats (range, cone seed, angle).  The prelude builds two
// rotated direction bases via SinFIdx/CosFIdx (angle scaled by the FIdx
// conversion constant); each entry is gated by the battle-state block, the
// aux cone probe func_804B1BDC on aux+0x60C, and the func_800FD3FC fallback
// (options bit 3).  Accepted entries get the 0x70 mark; all are re-appended.
void __ct__800FBA18(cf::CfObjEnumList* list, ml::CVec3* spot, float a,
                    float b, float angle, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        u32 ownsCopy;                          // +0x000 (<- list+0x1C)
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x004
        u32 count;                             // +0x604
        u8* b_;                                // +0x608
        u8* a_;                                // +0x60C
        u32 field;                             // +0x610
    };
    RebuildCtx ctx;
    ctx.ownsCopy = list->mOwnsList;
    ctx.count = list->mPtrCount;
    ctx.a_ = lbl_eu_8052BDA0;
    ctx.b_ = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    list->mPtrCount = 0;
    const float fidxScale = lbl_eu_80666ED8; // radians -> FIdx for SinFIdx
    const float zero = lbl_eu_80666EB8;
    const float one = lbl_eu_80666EBC;
    // First basis: rotate the seed direction (0, b, 0) by 'angle' around Y and
    // tilt by 'a'; second basis repeats with the roles swapped.  Retail keeps
    // the full 3x3 products in stack temps.
    float s1 = nw4r::math::SinFIdx(fidxScale * angle);
    float c1 = nw4r::math::CosFIdx(fidxScale * angle);
    ml::CVec3 dirA;
    dirA.x = b * s1;
    dirA.y = b * c1 * one - b * zero;
    dirA.z = b * c1 * one + b * s1 - b * s1;
    float s2 = nw4r::math::SinFIdx(fidxScale * a);
    float c2 = nw4r::math::CosFIdx(fidxScale * a);
    ml::CVec3 dirB;
    dirB.x = s2;
    dirB.y = c2;
    dirB.z = -s2 * one + zero;
    ml::CVec3 probe1;   // retail sp+0x68
    ml::CVec3 probe2;   // retail sp+0x74
    probe1.x = spot->x + dirA.x;
    probe1.y = spot->y + dirA.y;
    probe1.z = spot->z + dirA.z;
    probe2.x = spot->x + dirB.x;
    probe2.y = spot->y + dirB.y;
    probe2.z = spot->z + dirB.z;
    u32 invert = options & 2;
    u32 useFallback = options & 8;
    for (u32 i = 0; i < ctx.count; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActorView* aux = reinterpret_cast<CfEnumActorView*>(
            func_800BBC0C(reinterpret_cast<CfEnumObject*>(p->object)));
        bool ok = false;
        // Battle-state gate: state words 2/3/6 accept directly.
        CfEnumVf298* v298 = aux->vf298();
        if (v298 != NULL && v298->field_0x50 != NULL) {
            u16 state = v298->field_0x50->field_0x3C;
            if (state == 2 || state == 3 || state == 6) {
                ok = true;
            }
        }
        if (!ok && aux != NULL) {
            ok = func_804B1BDC(&aux->field_60C, &probe2, &probe1) != 0;
        }
        if (!ok && useFallback) {
            ok = func_800FD3FC(list, reinterpret_cast<CfEnumActor*>(aux),
                               spot) != 0;
        }
        if (invert) {
            ok = !ok;
        }
        if (ok) {
            p->field_18 |= 0x70;
        }
        list->mPtrArray[list->mPtrCount++] = p;
    }
}

// __ct__800FBF08: rebuild the object list filtering entries through the
// arts-state gate: func_8016FE34(object) then func_80148778(actor+8, tag).
// flags bit 1 inverts the accept decision; accepted entries get the 0x70
// mark and are only re-appended when flags bit 0 is set.
void __ct__800FBF08(cf::CfObjEnumList* list, u32 tag, u32 flags) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        void* a;                               // +0x604 (lbl_eu_8052BDA0 + 8)
        void* b;                               // +0x608 (lbl_eu_8052BDA0)
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.b = lbl_eu_8052BDA0;
    ctx.a = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    u32 invert = flags & 2;       // accept-hit instead of reject-hit
    u32 appendMarked = flags & 1; // re-append 0x70-marked entries
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor =
            func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        int hit = func_80148778(reinterpret_cast<u8*>(actor) + 8, tag);
        bool ok = hit == 0;
        if (invert) {
            ok = !ok;
        }
        if (ok) {
            p->field_18 |= 0x70;
            if (!appendMarked) {
                continue;
            }
            list->mPtrArray[list->mPtrCount++] = p;
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// __ct__800FC040: rebuild the object list keeping only entries whose actor
// fails BOTH tag probes (func_80148778 with tag1 and tag2).  options bit 1
// inverts the keep decision; rejected entries get the 0x70 mark and are only
// re-appended when options bit 0 is set.
void __ct__800FC040(cf::CfObjEnumList* list, u32 tag1, u32 tag2, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    // Rebuild context: snapshot of the array plus a sort descriptor
    // {count, b, a, field_062C}.  The descriptor stores are never read in
    // retail; taking the array member's address (in the copy below) makes
    // MWCC treat the struct as address-taken, so it keeps the stores.
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        u8* b;                                 // +0x604
        u8* a;                                 // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor =
            func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        bool rejected = false;
        if (func_80148778(&actor->tagState, tag1) == 0 &&
            func_80148778(&actor->tagState, tag2) == 0) {
            rejected = true;
        }
        if (options & 2) {
            rejected = !rejected;
        }
        if (rejected) {
            p->field_18 |= 0x70;
            if (options & 1) {
                list->mPtrArray[list->mPtrCount++] = p;
            }
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// __ct__800FC19C: rebuild the object list keeping only entries whose actor's
// arts-state gate (func_80174C98 on the slot-0x30 word, queried with arg1)
// reports failure; when arg1 == 0x1C and the gate passed, additionally
// require the embedded move sub-object's vtable-0x84 result to be non-zero.
void __ct__800FC19C(cf::CfObjEnumList* list, u32 arg1, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        u8* b;                                 // +0x604
        u8* a;                                 // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor =
            func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        CfEnumActorView* actorView = reinterpret_cast<CfEnumActorView*>(actor);
        u32 val = actorView->field_04->b30()->field_0;
        bool rejected = func_80174C98(actor, &val, (int)arg1) == 0;
        if (arg1 == 0x1c && !rejected && actorView->sub.f31() == 0) {
            rejected = !rejected;   // rejected is provably 0 here -> becomes 1
        }
        if (options & 2) {
            rejected = !rejected;
        }
        if (rejected) {
            p->field_18 |= 0x70;
            if (options & 1) {
                list->mPtrArray[list->mPtrCount++] = p;
            }
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// __ct__800FC32C: rebuild the object list keeping only entries whose actor
// fails BOTH arts-state gates (func_80174C98 with arg1 and arg2); when
// arg1 == 0x1C and the gates passed, additionally require the embedded move
// sub-object's vtable-0x84 word to be non-zero.
void __ct__800FC32C(cf::CfObjEnumList* list, u32 arg1, u32 arg2, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        u8* b;                                 // +0x604
        u8* a;                                 // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor =
            func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        CfEnumActorView* actorView = reinterpret_cast<CfEnumActorView*>(actor);
        bool rejected = false;
        u32 val1 = actorView->field_04->b30()->field_0;
        if (func_80174C98(actor, &val1, (int)arg1) == 0) {
            u32 val2 = actorView->field_04->b30()->field_0;
            if (func_80174C98(actor, &val2, (int)arg2) == 0) {
                rejected = true;
            }
        }
        if (arg1 == 0x1c && !rejected && actorView->sub.f31() == 0) {
            rejected = !rejected;
        }
        if (options & 2) {
            rejected = !rejected;
        }
        if (rejected) {
            p->field_18 |= 0x70;
            if (options & 1) {
                list->mPtrArray[list->mPtrCount++] = p;
            }
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// __ct__800FC4FC: like __ct__800FC648 but keeps entries whose value() is
// greater-or-equal to the threshold (inverse comparison).
void __ct__800FC4FC(cf::CfObjEnumList* self, u32 flags, float threshold) {
    if (self->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        void* b;                               // +0x604
        void* a;                               // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(self->mPtrArray);
    ctx.count = self->mPtrCount;
    ctx.a = &lbl_eu_8052BDA0;
    ctx.b = &lbl_eu_8052BDA0[8];
    ctx.field = self->field_062C;
    self->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor = func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        float value = actor->value();
        bool keep = value >= threshold;
        if (flags & 2) {
            keep = !keep;
        }
        if (keep) {
            p->field_18 |= 0x70;
            if (flags & 1) {
                self->mPtrArray[self->mPtrCount++] = p;
            }
        } else {
            self->mPtrArray[self->mPtrCount++] = p;
        }
    }
}

// __ct__800FC648: rebuild the object list, keeping only entries whose actor's
// value() compares favourably against a float threshold.  flags bit 1 inverts
// the keep condition; entries that fail get field_18 |= 0x70, and are only
// re-appended when flags bit 0 is set.  The current mPtrArray is snapshotted
// to a stack copy first (the count is reloaded from the saved descriptor
// inside the search loop).
void __ct__800FC648(cf::CfObjEnumList* self, u32 flags, float threshold) {
    if (self->mPtrCount == 0) {
        return;
    }
    // Rebuild context: snapshot of the array plus a sort descriptor
    // {count, &lbl+8, &lbl, field_062C}.  The descriptor stores are never
    // read in retail; taking the array member's address (in the copy below)
    // makes MWCC treat the struct as address-taken, so it keeps the stores
    // and reloads count instead of dead-store-eliminating them.
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        void* b;                               // +0x604
        void* a;                               // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(self->mPtrArray);
    ctx.count = self->mPtrCount;
    ctx.a = &lbl_eu_8052BDA0;
    ctx.b = &lbl_eu_8052BDA0[8];
    ctx.field = self->field_062C;
    self->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor = func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        float value = actor->value();
        bool keep = value < threshold;
        if (flags & 2) {
            keep = !keep;
        }
        if (keep) {
            p->field_18 |= 0x70;
            if (flags & 1) {
                self->mPtrArray[self->mPtrCount++] = p;
            }
        } else {
            self->mPtrArray[self->mPtrCount++] = p;
        }
    }
}

// __ct__800FC790: like __ct__800FC8DC but keeps entries whose value2() is
// greater-or-equal to the threshold (inverse comparison).
void __ct__800FC790(cf::CfObjEnumList* self, u32 flags, float threshold) {
    if (self->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        void* b;                               // +0x604
        void* a;                               // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(self->mPtrArray);
    ctx.count = self->mPtrCount;
    ctx.a = &lbl_eu_8052BDA0;
    ctx.b = &lbl_eu_8052BDA0[8];
    ctx.field = self->field_062C;
    self->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor = func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        float value = actor->value2();
        bool keep = value >= threshold;
        if (flags & 2) {
            keep = !keep;
        }
        if (keep) {
            p->field_18 |= 0x70;
            if (flags & 1) {
                self->mPtrArray[self->mPtrCount++] = p;
            }
        } else {
            self->mPtrArray[self->mPtrCount++] = p;
        }
    }
}

// __ct__800FC8DC: like __ct__800FC648 but compares value2() (vtable +0x160)
// against the threshold.
void __ct__800FC8DC(cf::CfObjEnumList* self, u32 flags, float threshold) {
    if (self->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        void* b;                               // +0x604
        void* a;                               // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(self->mPtrArray);
    ctx.count = self->mPtrCount;
    ctx.a = &lbl_eu_8052BDA0;
    ctx.b = &lbl_eu_8052BDA0[8];
    ctx.field = self->field_062C;
    self->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor = func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        float value = actor->value2();
        bool keep = value < threshold;
        if (flags & 2) {
            keep = !keep;
        }
        if (keep) {
            p->field_18 |= 0x70;
            if (flags & 1) {
                self->mPtrArray[self->mPtrCount++] = p;
            }
        } else {
            self->mPtrArray[self->mPtrCount++] = p;
        }
    }
}

// __ct__800FCA24: rebuild the object list keeping only entries whose actor's
// embedded move sub-object (vtable 0x4C) does NOT reference the given spot
// AND that the battle manager rejects (func_800DA06C == 0).
void __ct__800FCA24(cf::CfObjEnumList* list, u32 spotId, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        u8* b;                                 // +0x604
        u8* a;                                 // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor =
            func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        CfEnumActorView* actorView = reinterpret_cast<CfEnumActorView*>(actor);
        bool rejected = false;
        u32 subId = actorView->sub.vfn13();
        if (subId != spotId ||
            func_800DA06C(getInstance__Q22cf14CBattleManagerFv(), actor) == 0) {
            rejected = true;
        }
        if (options & 2) {
            rejected = !rejected;
        }
        if (rejected) {
            p->field_18 |= 0x70;
            if (options & 1) {
                list->mPtrArray[list->mPtrCount++] = p;
            }
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// __ct__800FCB80: rebuild the object list keeping only entries whose actor is
// neither flagged by the player-state word (flags64 bits 1/2 of
// func_800B708C(spotId) select the field_3F00 bit test) nor references the
// given spot via the embedded move sub-object (vtable 0x4C).
void __ct__800FCB80(cf::CfObjEnumList* list, u32 spotId, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        u8* b;                                 // +0x604
        u8* a;                                 // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    CfEnumObject* obj = func_800B708C__Fi(spotId);
    u32 flags = obj->flags64;
    u32 n = ctx.count;
    bool flagBit1 = (flags >> 1) & 1;   // extrwi rD,rS,1,30
    bool flagBit2 = (flags >> 2) & 1;   // extrwi rD,rS,1,29
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor =
            func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        CfEnumActorView* actorView = reinterpret_cast<CfEnumActorView*>(actor);
        bool rejected = false;
        if (flagBit1 && (actorView->field_3F00 & 2)) {
            rejected = true;
        } else if (flagBit2 && (actorView->field_3F00 & 4)) {
            rejected = true;
        }
        u32 subId = actorView->sub.vfn13();
        if (subId == spotId) {
            rejected = true;
        }
        if (options & 2) {
            rejected = !rejected;
        }
        if (rejected) {
            p->field_18 |= 0x70;
            if (options & 1) {
                list->mPtrArray[list->mPtrCount++] = p;
            }
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// __ct__800FCD10: rebuild the object list keeping only entries whose actor
// has a partner in the battle-manager actor list: a candidate whose
// field_3F10 differs from arg1, whose embedded sub-object (vtable 0x4C) does
// not reference the actor's field_3F10, and who is accepted by
// func_800DA06C.  The list walk starts at func_800EA384(manager) (+0x3E9C)
// and advances via func_800EA3AC.
void __ct__800FCD10(cf::CfObjEnumList* list, u32 arg1, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        u8* b;                                 // +0x604
        u8* a;                                 // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor =
            func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        // Born before found/cur/cand so MWCC assigns the same saved registers
        // as retail (actorId=r22, found=r21, cur=r20, cand=r19).  The load is
        // only issued inside the partner check below.
        u32 actorId;
        bool found = false;
        // Adjust the call result in place (stays in r3 through the null
        // check), then copy to cur once: retail cmpwi/beq/addi/mr order.
        CfEnumActor* t = func_800EA384(getInstance__Q22cf14CBattleManagerFv());
        if (t != 0) {
            t = reinterpret_cast<CfEnumActor*>(reinterpret_cast<u8*>(t) + 0x3E9C);
        }
        CfEnumObject* cur = reinterpret_cast<CfEnumObject*>(t);
        while (cur != 0) {
            CfEnumActor* cand = func_8016FE34(cur);
            if (cand == 0 || cand == actor) {
                CfEnumActor* t2 = func_800EA3AC(
                    getInstance__Q22cf14CBattleManagerFv(), cur);
                if (t2 != 0) {
                    t2 = reinterpret_cast<CfEnumActor*>(
                        reinterpret_cast<u8*>(t2) + 0x3E9C);
                }
                cur = reinterpret_cast<CfEnumObject*>(t2);
            } else if (reinterpret_cast<CfEnumActorView*>(cand)->field_3F10 == arg1) {
                CfEnumActor* t2 = func_800EA3AC(
                    getInstance__Q22cf14CBattleManagerFv(), cur);
                if (t2 != 0) {
                    t2 = reinterpret_cast<CfEnumActor*>(
                        reinterpret_cast<u8*>(t2) + 0x3E9C);
                }
                cur = reinterpret_cast<CfEnumObject*>(t2);
            } else {
                actorId = reinterpret_cast<CfEnumActorView*>(actor)->field_3F10;
                if (actorId ==
                        reinterpret_cast<CfEnumActorView*>(cand)->sub.vfn13() &&
                    func_800DA06C(getInstance__Q22cf14CBattleManagerFv(), cand) != 0) {
                    found = true;
                    break;
                }
                CfEnumActor* t2 = func_800EA3AC(
                    getInstance__Q22cf14CBattleManagerFv(), cur);
                if (t2 != 0) {
                    t2 = reinterpret_cast<CfEnumActor*>(
                        reinterpret_cast<u8*>(t2) + 0x3E9C);
                }
                cur = reinterpret_cast<CfEnumObject*>(t2);
            }
        }
        if (options & 2) {
            found = !found;
        }
        if (found) {
            p->field_18 |= 0x70;
            if (options & 1) {
                list->mPtrArray[list->mPtrCount++] = p;
            }
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// __ct__800FCF1C: rebuild the object list keeping only entries whose actor's
// arts-state gate (func_80174C98, mode 0x806) fails, or whose vf298() battle
// block byte at +0x42 differs from arg1 (or is missing).
void __ct__800FCF1C(cf::CfObjEnumList* list, int arg1, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        u8* b;                                 // +0x604
        u8* a;                                 // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor =
            func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        CfEnumActorView* actorView = reinterpret_cast<CfEnumActorView*>(actor);
        u32 val = actorView->field_04->b30()->field_0;
        bool rejected = func_80174C98(actor, &val, 0x806) == 0;
        if (!rejected) {
            CfEnumBattleBlock* blk =
                actorView->vf298()->field_0x50;
            if (blk != 0) {
                if (arg1 != blk->field_0x42) {
                    rejected = true;
                }
            } else {
                rejected = true;
            }
        }
        if (options & 2) {
            rejected = !rejected;
        }
        if (rejected) {
            p->field_18 |= 0x70;
            if (options & 1) {
                list->mPtrArray[list->mPtrCount++] = p;
            }
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// __ct__800FD0B4: rebuild the object list keeping only entries whose actor
// fails the arts-state gate func_80174C98(mode 0xA), or whose vf298() battle
// block's +0x3C u16 is not one of {2, 3, 6}.
void __ct__800FD0B4(cf::CfObjEnumList* list, u32 options) {
    if (list->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        u8* b;                                 // +0x604
        u8* a;                                 // +0x608
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(list->mPtrArray);
    ctx.count = list->mPtrCount;
    ctx.a = lbl_eu_8052BDA0;
    ctx.b = lbl_eu_8052BDA0 + 8;
    ctx.field = list->field_062C;
    list->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        CfEnumActor* actor =
            func_8016FE34(reinterpret_cast<CfEnumObject*>(p->object));
        if (actor == 0) {
            continue;
        }
        CfEnumActorView* actorView = reinterpret_cast<CfEnumActorView*>(actor);
        bool rejected = true;
        u32 val = actorView->field_04->b30()->field_0;
        if (func_80174C98(actor, &val, 0xa) != 0) {
            CfEnumBattleBlock* blk = actorView->vf298()->field_0x50;
            if (blk != 0) {
                int t = blk->field_0x3C;
                if (t == 2 || t == 3 || t == 6) {
                    rejected = false;
                }
            }
        }
        if (options & 2) {
            rejected = !rejected;
        }
        if (rejected) {
            p->field_18 |= 0x70;
            if (options & 1) {
                list->mPtrArray[list->mPtrCount++] = p;
            }
        } else {
            list->mPtrArray[list->mPtrCount++] = p;
        }
    }
}

// __ct__800FD250: rebuild the object list, keeping only entries whose
// field_18 has none of the 0x70 flag bits set.  The current mPtrArray is
// snapshotted to a stack context (with the dead sort-descriptor stores the
// retail keeps), then the array is recompacted from index 0.
void __ct__800FD250(cf::CfObjEnumList* self) {
    if (self->mPtrCount == 0) {
        return;
    }
    struct RebuildCtx {
        cf::CfObjEnumList::sObjInfo* arr[384]; // +0x000
        u32 count;                             // +0x600
        void* a;                               // +0x604 (lbl_eu_8052BDA0 + 8)
        void* b;                               // +0x608 (lbl_eu_8052BDA0)
        u32 field;                             // +0x60C
    };
    struct CopyArr {
        cf::CfObjEnumList::sObjInfo* arr[384];
    };
    RebuildCtx ctx;
    *reinterpret_cast<CopyArr*>(ctx.arr) =
        *reinterpret_cast<CopyArr*>(self->mPtrArray);
    ctx.count = self->mPtrCount;
    ctx.b = lbl_eu_8052BDA0;
    ctx.a = lbl_eu_8052BDA0 + 8;
    ctx.field = self->field_062C;
    self->mPtrCount = 0;
    u32 n = ctx.count;
    for (u32 i = 0; i < n; i++) {
        cf::CfObjEnumList::sObjInfo* p = ctx.arr[i];
        if ((p->field_18 & 0x70) == 0) {
            self->mPtrArray[self->mPtrCount++] = p;
        }
    }
}

// func_800FD2FC: search forward from (index + 1) mod count for the first
// entry whose field_18 has none of the 0x70 flag bits set; return its
// objectId, or 0 if the whole cycle is flagged.
void* func_800FD2FC(cf::CfObjEnumList* self, int index) {
    volatile u32* countPtr = &self->mPtrCount;
    int i = (index + 1) % *countPtr;
    while (index != i) {
        cf::CfObjEnumList::sObjInfo* obj = self->mPtrArray[i];
        if ((obj->field_18 & 0x70) == 0) {
            return (void*)obj->objectId;
        }
        i = (i + 1) % *countPtr;
    }
    cf::CfObjEnumList::sObjInfo* obj = self->mPtrArray[index];
    if ((obj->field_18 & 0x70) != 0) {
        return 0;
    }
    return (void*)obj->objectId;
}

// func_800FD378: search backward from (index + count - 1) mod count for the
// first entry whose field_18 has none of the 0x70 flag bits set; return its
// objectId, or 0 if the whole cycle is flagged.
void* func_800FD378(cf::CfObjEnumList* self, int index) {
    volatile u32* countPtr = &self->mPtrCount;
    u32 count = *countPtr;
    int i = (index + count - 1) % count;
    while (index != i) {
        cf::CfObjEnumList::sObjInfo* obj = self->mPtrArray[i];
        if ((obj->field_18 & 0x70) == 0) {
            return (void*)obj->objectId;
        }
        count = *countPtr;
        i = (i + count - 1) % count;
    }
    cf::CfObjEnumList::sObjInfo* obj = self->mPtrArray[index];
    if ((obj->field_18 & 0x70) != 0) {
        return 0;
    }
    return (void*)obj->objectId;
}

// us-800fdee4 | func_800FD3FC: list-rebuild partner check.  The `list` param
// is unused (retail never reads r3); aux's position comes from vtable 0x128
// (CfEnumPosBlock, floats at +0xC/+0x1C/+0x2C) or, if null, vtable 0xAC (plain
// vec3).  The two Ys are nudged by lbl_eu_80666ECC and ordered so the first
// arg to func_804BE348 has the larger Y.  A lazy-initialized 12-float probe
// table (4 diagonal offsets) is added to both corners; any probe hit returns 1.
extern "C" int func_800FD3FC(cf::CfObjEnumList* list, CfEnumActor* aux,
                             ml::CVec3* spot) {
    if (aux == 0) {
        return 1;
    }
    // Declaration order drives the stack slots (first declared -> highest):
    // objPos 0x68, spotCopy 0x5C, zero 0x50, tmp 0x44, v1 0x38, v2 0x2C.
    ml::CVec3 objPos;
    ml::CVec3 spotCopy;
    ml::CVec3 zero;    // dead in retail (never read)
    ml::CVec3 tmp;     // v128-path float -> word round trip
    ml::CVec3 v1;      // loop probe corner (0x38)
    ml::CVec3 v2;      // loop probe corner (0x2C)
    // Locals force MWCC to load all three before storing.
    float sz = spot->z;
    float sy = spot->y;
    float sx = spot->x;
    spotCopy.x = sx;
    spotCopy.y = sy;
    spotCopy.z = sz;
    CfEnumObjPosView* view = reinterpret_cast<CfEnumObjPosView*>(aux);
    CfEnumPosBlock* blk = view->v128();
    if (blk != 0) {
        float tz = blk->z;
        float ty = blk->y;
        float tx = blk->x;
        tmp.x = tx;
        tmp.y = ty;
        tmp.z = tz;
        objPos = tmp;   // word copy (retail lwz/stw)
    } else {
        objPos = *reinterpret_cast<ml::CVec3*>(view->vAC());
    }
    spotCopy.y += lbl_eu_80666ECC;
    objPos.y += lbl_eu_80666ECC;
    ml::CVec3* low = &spotCopy;   // r23: smaller Y
    ml::CVec3* high = &objPos;    // r24: larger Y
    if (spotCopy.y > objPos.y) {
        low = &objPos;
        high = &spotCopy;
    }
    if (func_804BE348(high, low, 0x44A45, 0, 0) == 0) {
        return 1;
    }
    // ps_sub zeroing: ml::CVec3::sub does { temp; VEC3Sub(temp, lhs, rhs);
    // out.set(temp); } - the ps result lands in a helper temp (0x20), then a
    // scalar copy into zero (0x50), exactly the retail's dead zero block.
    ml::CVec3::sub(zero, *high, *high);
    if (lbl_eu_80663F08 == 0) {
        // Probe-offset table: four diagonal direction vectors.
        lbl_eu_80573A30[0] = lbl_eu_80666EBC;
        lbl_eu_80573A30[1] = lbl_eu_80666EB8;
        lbl_eu_80573A30[2] = lbl_eu_80666EBC;
        lbl_eu_80573A30[3] = lbl_eu_80666EF0;
        lbl_eu_80573A30[4] = lbl_eu_80666EB8;
        lbl_eu_80573A30[5] = lbl_eu_80666EBC;
        lbl_eu_80573A30[6] = lbl_eu_80666EF0;
        lbl_eu_80573A30[7] = lbl_eu_80666EB8;
        lbl_eu_80573A30[8] = lbl_eu_80666EF0;
        lbl_eu_80573A30[9] = lbl_eu_80666EBC;
        lbl_eu_80573A30[10] = lbl_eu_80666EB8;
        lbl_eu_80573A30[11] = lbl_eu_80666EF0;
        lbl_eu_80663F08 = 1;
    }
    for (u32 i = 0; i < 4; i++) {
        const ml::CVec3& tbl =
            reinterpret_cast<const ml::CVec3*>(lbl_eu_80573A30)[i];
        // add() = { temp; VEC3Add(temp, lhs, rhs); out.set(temp); } - the
        // ps result lands in the helper temp (0x14/0x08), then a scalar copy
        // into v1/v2 (0x38/0x2C), exactly the retail's loop body.
        ml::CVec3::add(v1, *high, tbl);
        ml::CVec3::add(v2, *low, tbl);
        if (func_804BE348(&v1, &v2, 0x44A45, 0, 0) == 0) {
            return 1;
        }
    }
    return 0;
}

extern "C" void* func_800FD68C(void* self, int index) { return (char*)self + index * 4; }

void func_800FD698(void* self) { ((void(*)(void*))func_800FD68C)((char*)self - 0x604); }

extern "C" void func_800FD6A0(u8* self) { ((void(*)(void*))func_800F4798)((char*)self - 0x604); }

// Retail dtor: implicit this-guard, then the inlined member-list destruction
// chain (3 null-guards on this+4 + base body), then the delete-this epilogue
// driven by the r4 flags param.  All three parts are compiler-generated from
// the empty body + member layout above.
cf::CfObjectSelectorObj::~CfObjectSelectorObj() {}
