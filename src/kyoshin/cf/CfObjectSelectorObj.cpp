// Auto-scaffolded catalog TU for kyoshin/cf/CfObjectSelectorObj
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"
#include "monolib/util/reslist.hpp"
#include <nw4r/math.h>

namespace cf {
class CfObject;
}

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
extern const f32 lbl_eu_80666EFC;  // radians -> FIdx angle scale
extern const f32 lbl_eu_80666F00;  // half-angle scale
extern const f32 lbl_eu_8066A1FC;  // two*pi (angle wrap)

// Retail keeps these in split1.s (.sbss 0x80663F10/0x80663F14); declare
// extern so this TU emits no local storage.
extern f32 lbl_eu_80663F10;
extern const f32 lbl_eu_8066A1F8;
extern const f32 lbl_eu_80666F0C;
extern const f32 lbl_eu_80666F08;
extern const f32 lbl_eu_80666F04;

// Opaque item tag for the top-level lists; only the pointer type matters.
class CfObjEnumItem;

// Enum-list rebuild / probe helpers shared with CfObjectEnumList.cpp are
// declared by the harness include chain (CTaskGame.hpp and friends); the
// list-rebuild ctors and scan helpers are declared here.
extern "C" {
void __ct__800FB044(void* list, void* spot, u32 options, f32 threshold);
void __ct__800FD250(void* list);
void __ct__800FA9B4(void* list, void* obj, u32 options);
void func_800F89DC(void* list);
void* func_800F6EC0(void* list, s32 index);
void* func_800F6E98(void* list, s32 index);
void* func_800FD2FC(void* list, s32 index);
void* func_800FD378(void* list, s32 index);
// func_80174C98 / func_800B708C come from the kyoshin headers (CChainTimer.hpp
// / CTalkWindow.hpp) already on the include chain.
void func_800FB270(void* self, void* pos, u32 flags, f32 a, f32 b, f32 c);
void func_800F49F8(void* dst, void* src);
// func_800F6ED0 comes from CfObjectImplMove.hpp: (CfMoveEnumList*, u32).
}

// Selector request worker (defined below); bare C-linkage name matches
// retail (PLAN.md 17.6 approved rename fix).
namespace cf { class CfSelectorUnit; }
extern "C" void func_800FD774(cf::CfSelectorUnit* list);

// Layout view over an inner enum-list element slot.
struct SelEnumSlotView {
    u8 _pad_00[0x4];
    void* object;     // 0x04
    u8 _pad_08[0x14 - 0x8];
    f32 field_14;     // 0x14 (cached distance)
    u8 field_18;      // 0x18 (filter mark byte)
};

// Object state word queried by the enumeration filter.
struct SelObjFlagsView {
    u8 _pad_00[0x64];
    u32 flags64;
};
// Player aux block whose radius widens the distance threshold.
struct SelActorAuxView {
    u8 _pad_00[0x63C];
    f32 radius;
};

// Interface views for the retail virtual dispatches. Never instantiated, so
// no vtables are emitted; declared virtuals land at (index+2)*4 with RTTI on.
struct SelPosDispatch {
    virtual void p000();  virtual void p001();  virtual void p002();  virtual void p003();
    virtual void p004();  virtual void p005();  virtual void p006();  virtual void p007();
    virtual void p008();  virtual void p009();  virtual void p010();  virtual void p011();
    virtual void p012();  virtual void p013();  virtual void p014();  virtual void p015();
    virtual void p016();  virtual void p017();  virtual void p018();  virtual void p019();
    virtual void p020();  virtual void p021();  virtual void p022();  virtual void p023();
    virtual void p024();  virtual void p025();  virtual void p026();  virtual void p027();
    virtual void p028();  virtual void p029();  virtual void p030();  virtual void p031();
    virtual void p032();  virtual void p033();  virtual void p034();  virtual void p035();
    virtual void p036();  virtual void p037();  virtual void p038();  virtual void p039();
    virtual void p040();
    virtual void* vAC(); // index 41 -> vtable 0xAC (position vector)
};
struct SelFacingDispatch {
    virtual void q000();  virtual void q001();  virtual void q002();  virtual void q003();
    virtual void q004();  virtual void q005();  virtual void q006();  virtual void q007();
    virtual void q008();  virtual void q009();  virtual void q010();  virtual void q011();
    virtual void q012();  virtual void q013();  virtual void q014();  virtual void q015();
    virtual void q016();  virtual void q017();  virtual void q018();  virtual void q019();
    virtual void q020();  virtual void q021();  virtual void q022();  virtual void q023();
    virtual void q024();  virtual void q025();  virtual void q026();  virtual void q027();
    virtual void q028();  virtual void q029();  virtual void q030();  virtual void q031();
    virtual void q032();  virtual void q033();  virtual void q034();  virtual void q035();
    virtual void q036();  virtual void q037();  virtual void q038();  virtual void q039();
    virtual void q040();  virtual void q041();  virtual void q042();  virtual void q043();
    virtual void q044();  virtual void q045();  virtual void q046();  virtual void q047();
    virtual void q048();  virtual void q049();  virtual void q050();  virtual void q051();
    virtual void q052();  virtual void q053();  virtual void q054();  virtual void q055();
    virtual void q056();  virtual void q057();  virtual void q058();  virtual void q059();
    virtual void q060();  virtual void q061();  virtual void q062();  virtual void q063();
    virtual void q064();  virtual void q065();  virtual void q066();  virtual void q067();
    virtual void q068();  virtual void q069();  virtual void q070();  virtual void q071();
    virtual void q072();  virtual void q073();  virtual void q074();  virtual void q075();
    virtual void q076();  virtual void q077();  virtual void q078();  virtual void q079();
    virtual void q080();  virtual void q081();  virtual void q082();  virtual void q083();
    virtual void q084();  virtual void q085();  virtual void q086();  virtual void q087();
    virtual void q088();  virtual void q089();  virtual void q090();  virtual void q091();
    virtual void q092();  virtual void q093();  virtual void q094();  virtual void q095();
    virtual void q096();  virtual void q097();  virtual void q098();  virtual void q099();
    virtual void q100();  virtual void q101();  virtual void q102();  virtual void q103();
    virtual void q104();  virtual void q105();  virtual void q106();  virtual void q107();
    virtual void q108();  virtual void q109();  virtual void q110();  virtual void q111();
    virtual void q112();  virtual void q113();  virtual void q114();  virtual void q115();
    virtual void q116();  virtual void q117();  virtual void q118();  virtual void q119();
    virtual void q120();  virtual void q121();  virtual void q122();  virtual void q123();
    virtual void q124();  virtual void q125();  virtual void q126();  virtual void q127();
    virtual void q128();  virtual void q129();  virtual void q130();  virtual void q131();
    virtual void q132();  virtual void q133();  virtual void q134();  virtual void q135();
    virtual void q136();  virtual void q137();  virtual void q138();  virtual void q139();
    virtual void q140();  virtual void q141();  virtual void q142();  virtual void q143();
    virtual void q144();  virtual void q145();  virtual void q146();  virtual void q147();
    virtual void q148();  virtual void q149();  virtual void q150();  virtual void q151();
    virtual void q152();  virtual void q153();  virtual void q154();  virtual void q155();
    virtual void q156();  virtual void q157();  virtual void q158();  virtual void q159();
    virtual void q160();  virtual void q161();  virtual void q162();  virtual void q163();
    virtual void q164();  virtual void q165();  virtual void q166();  virtual void q167();
    virtual void q168();  virtual void q169();  virtual void q170();  virtual void q171();
    virtual void q172();  virtual void q173();  virtual void q174();  virtual void q175();
    virtual void q176();  virtual void q177();  virtual void q178();  virtual void q179();
    virtual void q180();  virtual void q181();  virtual void q182();  virtual void q183();
    virtual void q184();  virtual void q185();  virtual void q186();  virtual void q187();
    virtual void q188();  virtual void q189();  virtual void q190();  virtual void q191();
    virtual void q192();  virtual void q193();  virtual void q194();  virtual void q195();
    virtual void q196();  virtual void q197();  virtual void q198();  virtual void q199();
    virtual void q200();  virtual void q201();  virtual void q202();  virtual void q203();
    virtual void q204();  virtual void q205();  virtual void q206();  virtual void q207();
    virtual void q208();  virtual void q209();  virtual void q210();  virtual void q211();
    virtual void q212();  virtual void q213();  virtual void q214();  virtual void q215();
    virtual void q216();  virtual void q217();  virtual void q218();  virtual void q219();
    virtual void q220();  virtual void q221();  virtual void q222();  virtual void q223();
    virtual void q224();  virtual void q225();  virtual void q226();  virtual void q227();
    virtual void q228();  virtual void q229();  virtual void q230();  virtual void q231();
    virtual void q232();  virtual void q233();  virtual void q234();  virtual void q235();
    virtual void q236();  virtual void q237();  virtual void q238();  virtual void q239();
    virtual void q240();  virtual void q241();  virtual void q242();  virtual void q243();
    virtual void q244();  virtual void q245();  virtual void q246();  virtual void q247();
    virtual void q248();  virtual void q249();  virtual void q250();  virtual void q251();
    virtual void q252();  virtual void q253();  virtual void q254();  virtual void q255();
    virtual void q256();  virtual void q257();  virtual void q258();  virtual void q259();
    virtual void q260();  virtual void q261();  virtual void q262();  virtual void q263();
    virtual void q264();  virtual void q265();  virtual void q266();  virtual void q267();
    virtual void q268();  virtual void q269();  virtual void q270();  virtual void q271();
    virtual void q272();  virtual void q273();  virtual void q274();  virtual void q275();
    virtual void q276();  virtual void q277();  virtual void q278();  virtual void q279();
    virtual void q280();  virtual void q281();  virtual void q282();  virtual void q283();
    virtual void q284();  virtual void q285();  virtual void q286();  virtual void q287();
    virtual void q288();  virtual void q289();  virtual void q290();  virtual void q291();
    virtual void q292();  virtual void q293();  virtual void q294();  virtual void q295();
    virtual void q296();  virtual void q297();  virtual void q298();  virtual void q299();
    virtual void q300();  virtual void q301();  virtual void q302();  virtual void q303();
    virtual void q304();  virtual void q305();  virtual void q306();  virtual void q307();
    virtual void q308();  virtual void q309();  virtual void q310();  virtual void q311();
    virtual void q312();  virtual void q313();  virtual void q314();  virtual void q315();
    virtual void q316();  virtual void q317();  virtual void q318();  virtual void q319();
    virtual void q320();  virtual void q321();  virtual void q322();  virtual void q323();
    virtual void q324();  virtual void q325();  virtual void q326();  virtual void q327();
    virtual void q328();  virtual void q329();  virtual void q330();  virtual void q331();
    virtual void q332();  virtual void q333();  virtual void q334();  virtual void q335();
    virtual void q336();  virtual void q337();  virtual void q338();  virtual void q339();
    virtual void q340();  virtual void q341();  virtual void q342();  virtual void q343();
    virtual void q344();  virtual void q345();  virtual void q346();  virtual void q347();
    virtual void q348();  virtual void q349();  virtual void q350();  virtual void q351();
    virtual void q352();  virtual void q353();  virtual void q354();  virtual void q355();
    virtual void q356();  virtual void q357();  virtual void q358();  virtual void q359();
    virtual void q360();  virtual void q361();  virtual void q362();
    virtual f32 getFacing(); // index 363 -> vtable 0x5B4
};

// Inner-selector flavour: old declared before cur.
template <>
_reslist_base<cf::CfObject*>::~_reslist_base() {
    *(void**)this = (void*)lbl_eu_8052585C;
    _reslist_node<cf::CfObject*>* old;
    _reslist_node<cf::CfObject*>* cur = mStartNodePtr->mNext;
    while (cur != mStartNodePtr) {
        old = cur;
        cur = cur->mNext;
        old->mNext = nullptr;
    }
    mStartNodePtr->mNext = mStartNodePtr;
    mStartNodePtr->mPrev = mStartNodePtr;
    if (!unk1C && mList != nullptr) {
        __dla__FPv(mList);
        mList = nullptr;
    }
}

// Top-level flavour: old declared before cur.
template <>
_reslist_base<CfObjEnumItem*>::~_reslist_base() {
    *(void**)this = (void*)lbl_eu_8052585C;
    _reslist_node<CfObjEnumItem*>* old;
    _reslist_node<CfObjEnumItem*>* cur = mStartNodePtr->mNext;
    while (cur != mStartNodePtr) {
        old = cur;
        cur = cur->mNext;
        old->mNext = nullptr;
    }
    mStartNodePtr->mNext = mStartNodePtr;
    mStartNodePtr->mPrev = mStartNodePtr;
    if (!unk1C && mList != nullptr) {
        __dla__FPv(mList);
        mList = nullptr;
    }
}

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

// Top-level list wrapper: keeps the custom clear/destroy variants used by
// __dt__800FDEF8.
class CfObjEnumList : public reslist<CfObjEnumItem*> {
public:
    // Retail reslist::destroyList: clear + free + reset capacity.
    void destroyList() {
        _reslist_node<CfObjEnumItem*>* old;
        _reslist_node<CfObjEnumItem*>* cur = this->mStartNodePtr->mNext;
        while (cur != this->mStartNodePtr) {
            old = cur;
            cur = cur->mNext;
            old->mNext = NULL;
        }
        this->mStartNodePtr->mNext = this->mStartNodePtr;
        this->mStartNodePtr->mPrev = this->mStartNodePtr;
        if (this->unk1C == false && this->mList != NULL) {
            __dla__FPv(this->mList);
            this->mList = NULL;
        }
        this->mCapacity = 0;
    }

    // Variant used by the top-level list teardowns; declaring cur before old
    // flips MWCC's register assignment at those inline sites.
    void teardownTop();

    void clearList() {
        _reslist_node<CfObjEnumItem*>* cur = this->mStartNodePtr->mNext;
        _reslist_node<CfObjEnumItem*>* old;
        while (cur != this->mStartNodePtr) {
            old = cur;
            cur = cur->mNext;
            old->mNext = NULL;
        }
        this->mStartNodePtr->mNext = this->mStartNodePtr;
        this->mStartNodePtr->mPrev = this->mStartNodePtr;
        if (this->unk1C == false && this->mList != NULL) {
            __dla__FPv(this->mList);
            this->mList = NULL;
        }
        this->mCapacity = 0;
    }
};

// Inner selector list wrapper.
class CfSelUnitList : public reslist<cf::CfObject*> {
public:
    void teardownInner();
    // Retail reslist::destroyList: clear + free + reset capacity.
    void destroyList() {
        _reslist_node<cf::CfObject*>* old;
        _reslist_node<cf::CfObject*>* cur = this->mStartNodePtr->mNext;
        while (cur != this->mStartNodePtr) {
            old = cur;
            cur = cur->mNext;
            old->mNext = NULL;
        }
        this->mStartNodePtr->mNext = this->mStartNodePtr;
        this->mStartNodePtr->mPrev = this->mStartNodePtr;
        if (this->unk1C == false && this->mList != NULL) {
            __dla__FPv(this->mList);
            this->mList = NULL;
        }
        this->mCapacity = 0;
    }
};

// Selector unit: vtable + embedded list + per-request scalar state block.
// Retail places two instances at obj+0x60A0 and obj+0x910C, so the state
// words land at absolute 0x90E4../0xC150.. respectively.
class CfSelectorUnit {
public:
    u32 vtable;                   // +0x0000 (lbl_eu_8052BDD8)
    CfSelUnitList mList;          // +0x0004 (0x20 bytes)
    u8 _pad24[0x624 - 0x24];      // +0x0024-0x0623
    u32 mCount;                   // +0x0624 (inner list ptr-array count)
    u8 _pad628[0x3034 - 0x628];   // +0x0628-0x3033
    u32 w3034;               // +0x3034 (abs 0x90D4 / 0xC0D4)
    u8 _pad3038[0x3044 - 0x3038]; // +0x3038-0x3043
    u32 w3044;               // +0x3044
    u32 w3048;               // +0x3048
    u32 w304C;               // +0x304C
    u32 w3050;               // +0x3050
    u32 w3054;               // +0x3054
    f32 f3058;               // +0x3058
    f32 f305C;               // +0x305C
    f32 f3060;               // +0x3060
    u32 w3064;               // +0x3064
    u32 w3068;               // +0x3068
};

// Full selector data block (retail size 0xC188).
class CfObjectSelectorData {
public:
    CfObjEnumList mList1;       // 0x0000 (0x20)
    u8 _pad20[0x620 - 0x20];    // 0x0020-0x061F
    u32 field620;               // 0x0620 candidate count
    u8 _pad624[0x3040 - 0x624]; // 0x0624-0x303F
    CfObjEnumList mList2;       // 0x3040
    u8 _pad3060[0x3660 - 0x3060]; // 0x3060-0x365F
    u32 field3660;              // 0x3660 previously selected id
    u8 _pad3664[0x6080 - 0x3664]; // 0x3664-0x607F
    u32 field6080;               // 0x6080
    u8 _pad6084[0x608C - 0x6084]; // 0x6084-0x608B
    u32 field608C;               // 0x608C
    u8 _pad6090[0x6094 - 0x6090]; // 0x6090-0x6093
    u32 field6094;               // 0x6094
    u32 field6098;               // 0x6098
    u8 _pad609C[0x60A0 - 0x609C]; // 0x609C-0x609F
    CfSelectorUnit mInner1;      // 0x60A0-0x910B
    CfSelectorUnit mInner2;      // 0x910C-0xC177
    u8* fieldC178;               // 0xC178
    u32 fieldC17C;               // 0xC17C
    u32 fieldC180;               // 0xC180
    u32 fieldC184;               // 0xC184
};

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

extern cf::CfObjectSelectorData* lbl_eu_80663F14;

cf::CfObjectSelectorObj* func_800FE68C() {
    return (cf::CfObjectSelectorObj*)lbl_eu_80663F14;
}

// Common selector-request sequence: point 0xC178 at the primary inner list,
// mark the request active (0xC180 low byte = 2), carry over the adjusted
// handle, flag 0x9108 as pending, run the list walk, report result arrival.
bool func_800FE6A4(cf::CfObjectSelectorObj* obj, unsigned int a, unsigned int b, unsigned int c) {
    cf::CfObjectSelectorData* self = reinterpret_cast<cf::CfObjectSelectorData*>(obj);
    u32 sel = self->fieldC180 & 0xFFFFFF00;
    u32 handle = self->fieldC17C;
    self->fieldC178 = (u8*)&self->mInner1;
    self->fieldC180 = sel | 2;
    // 0xC17C holds a rebased handle; adjust it back into the real address space.
    if (handle != 0) {
        handle = handle + 0x3E9C;
    }
    self->mInner1.w3054 = handle;
    // Keep low bits with bit0 set (retail folds this into a single rlwinm).
    self->mInner1.w3068 = (self->mInner1.w3068 | 1) & 0xFFFFFFF3;
    self->mInner1.w3044 = c;
    self->mInner1.w3048 = a;
    self->mInner1.w304C = b;
    func_800FD774(&self->mInner1);
    return self->mInner1.w3044 != 0;
}

// Re-issues the current filter values (0x608C/0x6094/0x6098) as a request.
bool func_800FE738(cf::CfObjectSelectorObj* obj, unsigned int) {
    cf::CfObjectSelectorData* self = reinterpret_cast<cf::CfObjectSelectorData*>(obj);
    u32 sel = self->fieldC180 & 0xFFFFFF00;
    u32 val6098 = self->field6098;
    u32 val6094 = self->field6094;
    self->fieldC178 = (u8*)&self->mInner1;
    self->fieldC180 = sel | 2;
    u32 val608C = self->field608C;
    // Keep low bits with bit0 set (retail folds this into a single rlwinm).
    // 0xC17C holds a rebased handle; adjust it back into the real address space.
    u32 handle = self->fieldC17C;
    if (handle != 0) {
        handle = handle + 0x3E9C;
    }
    self->mInner1.w3054 = handle;
    self->mInner1.w3068 = (self->mInner1.w3068 | 1) & 0xFFFFFFF3;
    self->mInner1.w3044 = val6098;
    self->mInner1.w3048 = val608C;
    self->mInner1.w304C = val6094;
    func_800FD774(&self->mInner1);
    return self->mInner1.w3044 != 0;
}

u32 func_800FE7D8(cf::CfObjectSelectorObj* obj) {
    cf::CfSelectorLayout* self = reinterpret_cast<cf::CfSelectorLayout*>(obj);
    u32 zero = 0;
    if (self->field_0x90E4 == 0) return zero;

    u32 sel = self->field_0xC180;
    // Keep only the low byte of 0xC180, set bit0, then clear bits 8-11.
    self->field_0xC180 = ((sel & 0xFFFFFF00) | 1) & ~0xF00;
    self->field_0xC178 = reinterpret_cast<cf::CfSelectorLayout::CfSelectorTarget*>(zero);
    if ((self->field_0xC174 & 1) != 0) {
        self->field_0xC174 = zero;
        self->field_0xC150 = zero;
        self->field_0x9730 = zero;
        self->field_0xC140 = zero;
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
    u32 val608C;
    u32 val6094 = self->field6094;
    u32 handle;
    self->field6098 = c;
    self->mInner1.w3048 = val608C = self->field608C;
    self->mInner1.w304C = val6094;
    // 0xC17C holds a rebased handle; adjust it back into the real address space.
    handle = self->fieldC17C;
    self->fieldC180 = (sel & 0xFFFFFF00) | 2;
    self->fieldC178 = (u8*)&self->mInner1;
    if (handle != 0) {
        handle = handle + 0x3E9C;
    }
    self->mInner1.w3054 = handle;
    self->mInner1.w3068 = (self->mInner1.w3068 | 1) & 0xFFFFFFF3;
    self->mInner1.w3044 = c;
    // Stores are re-issued here because mInner1's address escapes below.
    self->mInner1.w3048 = val608C;
    self->mInner1.w304C = val6094;
    func_800FD774(&self->mInner1);
    return self->mInner1.w3044 != 0;
}

void sinit_800FEA14() {
    // lbl_eu_80663F10 = lbl_eu_80666F08 * (lbl_eu_8066A1F8 / lbl_eu_80666F0C);
    lbl_eu_80663F10 = lbl_eu_80666F08 * (lbl_eu_8066A1F8 / lbl_eu_80666F0C);
}

// ---------------------------------------------------------------------
// func_800FD774 (selector request worker).
// Rebuilds the inner enum list for the pending request, then optionally
// applies the distance/cone filter pass (w304C bit 0x4000), and finally
// resolves the requested object id via the projected rebuild + search
// helpers.  Result lands in w3044; status bits land in w3068.
// ---------------------------------------------------------------------
extern "C" void func_800FD774(cf::CfSelectorUnit* self) {
    if (!(self->w3068 & 1)) {
        return;
    }
    func_800F4A98(reinterpret_cast<CfMoveEnumList*>(&self->mList), self->w3048,
                  self->w304C);
    if (lbl_eu_80666EF8 != self->f3058) {
        void* spot = ((SelPosDispatch*)self->w3054)->vAC();
        __ct__800FB044(&self->mList, spot, 0, self->f3058);
    }

    if (self->w304C & 0x4000) {
        // Distance / facing-cone filter pass over the enumerated entries.
        nw4r::math::VEC3 delta;
        nw4r::math::VEC3 d;
        const f32 twoPi = lbl_eu_8066A1FC;
        nw4r::math::VEC3* dp = &d;
        const f32 pi = lbl_eu_8066A1F8;
        nw4r::math::VEC3* deltap = &delta;
        const f32 angScale = lbl_eu_80666EFC;
        const f32 halfScale = lbl_eu_80666F00;
        for (u32 i = 0; i < self->mCount; i++) {
            SelEnumSlotView* info =
                reinterpret_cast<SelEnumSlotView*>(
                    func_800F6EC0(&self->mList, i));
            if (reinterpret_cast<SelObjFlagsView*>(info->object)->flags64 & 4) {
                continue;
            }
            // Stale-distance entries are re-marked before the cone test.
            if (info->field_14 > self->f305C) {
                info->field_18 |= 0x70;
            }
            if (reinterpret_cast<SelObjFlagsView*>(info->object)->flags64 &
                0x100) {
                continue;
            }
            f32 range = self->f305C; // distance threshold (+ aux radius)
            SelActorAuxView* aux =
                reinterpret_cast<SelActorAuxView*>(func_800BBC0C(info->object));
            if (aux != NULL) {
                range += aux->radius;
            }
            // Player position (embedded +0x3E9C sub-object, slot 0xAC) and
            // facing angle (owner slot 0x5B4).
            u8* ownerSub = reinterpret_cast<u8*>(self->w3064) + 0x3E9C;
            const nw4r::math::VEC3* playerPos =
                reinterpret_cast<const nw4r::math::VEC3*>(
                    ((SelPosDispatch*)ownerSub)->vAC());
            f32 facing = ((SelFacingDispatch*)self->w3064)->getFacing();
            f32 cone = self->f3060; // half-angle

            nw4r::math::VEC3* pos = reinterpret_cast<nw4r::math::VEC3*>(
                ((SelPosDispatch*)info->object)->vAC());
            nw4r::math::VEC3Sub(deltap, pos, playerPos);
            d.x = deltap->x;
            d.y = deltap->y;
            d.z = deltap->z;
            f32 dist = PSVECMag((const Vec*)dp);
            f32 ang = angScale * nw4r::math::Atan2FIdx(d.x, d.z) - facing;
            while (pi <= ang) {
                ang -= twoPi;
            }
            while (ang < -pi) {
                ang += twoPi;
            }
            bool hit = range >= dist && -(cone * halfScale) <= ang &&
                       ang <= cone * halfScale;
            if (!hit) {
                info->field_18 |= 0x70;
            }
        }
        __ct__800FD250(&self->mList);
    }

    __ct__800FA9B4(&self->mList, (void*)self->w3050, 1);
    if (self->w3044 == 0) {
        void* spot = ((SelPosDispatch*)self->w3054)->vAC();
        func_800F6ED0(reinterpret_cast<CfMoveEnumList*>(&self->mList),
                      spot);
        u32 got =
            (u32)func_800F6E08(reinterpret_cast<CfMoveEnumList*>(&self->mList));
        self->w3044 = got;
        self->w3068 &= ~0x30000000u;
        if (got == 0) {
            self->w3068 |= 0x10;
        }
    } else {
        func_800F89DC(&self->mList);
        // Find the current result entry's index in the rebuilt list.
        u32 idx = 0;
        do {
            if (func_800F6E98(&self->mList, idx) == (void*)self->w3044) {
                break;
            }
            idx++;
        } while (idx < self->mCount);
        if (idx == self->mCount) {
            self->w3044 = 0;
            self->w3068 |= 0x10;
            return;
        }
        // Navigation flags pick the neighbour-scan direction.
        u32 flags = self->w3068;
        if (!(flags & 0x20) && !(flags & 0x40)) {
            if (flags & 4) {
                self->w3044 = (u32)func_800FD2FC(&self->mList, idx);
            } else if (flags & 8) {
                self->w3044 = (u32)func_800FD378(&self->mList, idx);
            }
        }
        self->w3068 &= ~0x30000000u;
    }
}

// ---------------------------------------------------------------------
// func_800FDE4C (retail 0x800FE934, size 0xAC).
// Re-initialises the selector state when the singleton exists.
// ---------------------------------------------------------------------
void func_800FDE4C(cf::CfObjectSelectorData* obj, u32 a4, u32 a5) {
    if (lbl_eu_80663F14 == NULL) return;

    u32 zero = 0;
    u32 v608C = 0x80000004;
    u32 v6094 = 0x4002;
    obj->mInner1.w3044 = zero;
    obj->mInner1.w3048 = zero;
    obj->mInner1.w304C = zero;
    obj->mInner1.w3050 = a5;
    obj->mInner1.f3058 = lbl_eu_80666EF8;
    obj->mInner1.w3068 = zero;
    obj->mInner2.w3044 = zero;
    obj->mInner2.w3048 = zero;
    obj->mInner2.w304C = zero;
    obj->mInner2.w3050 = a5;
    obj->mInner2.f3058 = lbl_eu_80666EF8;
    obj->mInner2.w3068 = zero;
    obj->mInner1.f305C = lbl_eu_80661DB0;
    obj->mInner2.f305C = lbl_eu_80661DB0;
    obj->mInner1.f3060 = lbl_eu_80663F10;
    obj->mInner2.f3060 = lbl_eu_80663F10;
    obj->mInner1.w3064 = a4;
    obj->mInner2.w3064 = a4;
    obj->fieldC184 = a5;
    obj->fieldC178 = NULL;
    obj->fieldC17C = a4;
    obj->field6080 = zero;
    obj->fieldC180 = obj->field6080 | 1;
    obj->field608C = v608C;
    obj->field6094 = v6094;
    obj->field6098 = zero;
    obj->mInner1.w3048 = v608C;
    obj->mInner1.w304C = v6094;
}

// ---------------------------------------------------------------------
// __ct__800FDB4C (retail 0x800FE634, size 0xD0).
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
            obj->mInner1.vtable = (u32)lbl_eu_8052BDD8;
            __ct__cf_CfObjEnumList((u8*)&obj->mInner1.mList);
            u32 zero = 0;
            obj->mInner1.w3044 = zero;
            f32 fv = lbl_eu_80666EF8;
            obj->mInner1.w3048 = zero;
            obj->mInner1.w3050 = zero;
            obj->mInner1.w3054 = zero;
            obj->mInner1.f3058 = fv;
            obj->mInner1.w3068 = zero;
            obj->mInner2.vtable = (u32)lbl_eu_8052BDD8;
            __ct__cf_CfObjEnumList((u8*)&obj->mInner2.mList);
            obj->mInner2.w3044 = zero;
            fv = lbl_eu_80666EF8;
            obj->mInner2.w3048 = zero;
            obj->mInner2.w3050 = zero;
            obj->mInner2.w3054 = zero;
            obj->mInner2.f3058 = fv;
            obj->mInner2.w3068 = zero;
            obj->fieldC180 = zero;
        }
        lbl_eu_80663F14 = obj;
    }
}

// ---------------------------------------------------------------------
// __dt__800FDC1C (retail 0x800FE704, size 0x230).
// Singleton destroy: deleting destructor of the data block (four member
// destructors inlined) then clear the singleton pointer.
// ---------------------------------------------------------------------
void __dt__800FDC1C() {
    cf::CfObjectSelectorData* obj = lbl_eu_80663F14;
    if (obj != NULL) {
        // Fully implicit destructor chain: delete expands to the two leading
        // obj guards (skip-to-clear), the four generated member teardowns,
        // then __dl__FPv.
        delete obj;
        lbl_eu_80663F14 = NULL;
    }
}

// ---------------------------------------------------------------------
// __dt__800FDEF8 (retail 0x800FE9E0, size 0x20C).
// Non-deleting reset: re-initialise the scalar state and clear the four
// resource lists (walk + free + reset capacity) when the singleton exists.
// ---------------------------------------------------------------------
void __dt__800FDEF8(cf::CfObjectSelectorData* obj) {
    if (lbl_eu_80663F14 != NULL) {
        obj->mInner1.w3044 = 0;
        obj->mInner1.w3048 = 0;
        obj->mInner1.w304C = 0;
        obj->mInner1.w3050 = 0;
        obj->mInner1.w3054 = 0;
        obj->mInner1.f3058 = lbl_eu_80666EF8;
        obj->mInner1.w3068 = 0;
        obj->mInner1.mList.destroyList();
        obj->mInner2.w3044 = 0;
        obj->mInner2.w3048 = 0;
        obj->mInner2.w304C = 0;
        obj->mInner2.w3050 = 0;
        obj->mInner2.w3054 = 0;
        obj->mInner2.f3058 = lbl_eu_80666EF8;
        obj->mInner2.w3068 = 0;
        obj->mInner2.mList.destroyList();
        obj->mList1.clearList();
        obj->mList2.clearList();
        u32 zero = 0;
        obj->fieldC178 = (u8*)zero;
        obj->fieldC17C = zero;
        // Store-forwarding of fieldC17C is what yields retail's ori r0,r4,1.
        obj->fieldC180 = obj->fieldC17C | 1;
    }
}

// ---------------------------------------------------------------------
// func_800FE860 (retail 0x800FF348, size 0xB0).
// Toggles the 0x40 "selected" bits on both selector-target states and the
// 0x100 "active" bit, but only when the request direction matches the
// current activation state.
// ---------------------------------------------------------------------
void func_800FE860(cf::CfObjectSelectorData* obj, u32 arg4) {
    // Proceed only when the request direction differs from the active state.
    u32 flag = obj->fieldC180 & 0x100;
    // Redundant-looking nested guards are preserved because MWCC keeps the
    // inner checks as separate compares (no cross-block folding).
    if (flag || arg4 == 0) {
        if (flag == 0) return;
        if (arg4 != 0) return;
    }
    if (arg4 != 0) {
        obj->mInner1.w3068 |= 0x40;
    } else {
        obj->mInner1.w3068 &= ~0x40;
    }
    if (arg4 != 0) {
        obj->mInner2.w3068 |= 0x40;
    } else {
        obj->mInner2.w3068 &= ~0x40;
    }
    if (arg4 == 0) {
        obj->fieldC180 &= ~0x100;
    } else {
        obj->fieldC180 |= 0x100;
    }
}

// ---------------------------------------------------------------------
// func_800FE104: per-frame selector update. Refreshes both inner request
// lists, honours pending clear flags on the 0xC174 / 0x9108 state words,
// then rebuilds the candidate list (mList2) and either confirms the
// current selection, scans for it, or resets the singleton's primary
// request when no valid candidate survives.
// ---------------------------------------------------------------------

// Manager-side interface (owner+0x3ED4): slots 0x30/0x34/0x40 used.
struct SelMgrDispatch {
    virtual void g000(); virtual void g001(); virtual void g002(); virtual void g003();
    virtual void g004(); virtual void g005(); virtual void g006(); virtual void g007();
    virtual void g008(); virtual void g009();
    virtual u32 g030(u32 a = 0, u32 b = 0); // slot 0x30
    virtual void g034(u32 a = 0);           // slot 0x34
    virtual void g012();
    virtual void g013();
    virtual u32 g040(u32 a = 0);            // slot 0x40
};

// Owner+0x04 sub-object: slot 0x30 returns a record whose first word is
// probed by func_80174C98.
struct SelTok {
    u32 v;
};
struct SelSub4Dispatch {
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003();
    virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008(); virtual void s009();
    virtual SelTok* s030(); // slot 0x30
};

// Result of owner+0x3E9C slot 0x110: carries a cached distance at +0x14.
struct SelAuxInfo {
    u8 _pad00[0x14];
    f32 dist; // 0x14
};

// Owner+0x3E9C interface, slot 0x110.
struct SelAuxDispatch {
    virtual void a000(); virtual void a001(); virtual void a002(); virtual void a003();
    virtual void a004(); virtual void a005(); virtual void a006(); virtual void a007();
    virtual void a008(); virtual void a009(); virtual void a010(); virtual void a011();
    virtual void a012(); virtual void a013(); virtual void a014(); virtual void a015();
    virtual void a016(); virtual void a017(); virtual void a018(); virtual void a019();
    virtual void a020(); virtual void a021(); virtual void a022(); virtual void a023();
    virtual void a024(); virtual void a025(); virtual void a026(); virtual void a027();
    virtual void a028(); virtual void a029(); virtual void a030(); virtual void a031();
    virtual void a032(); virtual void a033(); virtual void a034(); virtual void a035();
    virtual void a036(); virtual void a037(); virtual void a038(); virtual void a039();
    virtual void a040(); virtual void a041(); virtual void a042(); virtual void a043();
    virtual void a044(); virtual void a045(); virtual void a046(); virtual void a047();
    virtual void a048(); virtual void a049(); virtual void a050(); virtual void a051();
    virtual void a052(); virtual void a053(); virtual void a054(); virtual void a055();
    virtual void a056(); virtual void a057(); virtual void a058(); virtual void a059();
    virtual void a060(); virtual void a061(); virtual void a062(); virtual void a063();
    virtual void a064(); virtual void a065();
    virtual SelAuxInfo* a110(); // slot 0x110
};

// Minimal view over the owner object this selector is attached to.
struct SelOwnerView {
    u8 _pad00[0x4];
    SelSub4Dispatch* mpSub4;   // 0x04
    u8 _pad08[0x3ED4 - 0x8];
    SelMgrDispatch* mpMgr;     // 0x3ED4
};

void func_800FE104(cf::CfObjectSelectorData* self) {
    // Early guards: owner pointer only lives in a scratch register here.
    SelOwnerView* probe = reinterpret_cast<SelOwnerView*>(self->fieldC17C);
    if (probe == NULL || probe->mpMgr == NULL) return;

    func_800FD774(&self->mInner1);
    func_800FD774(&self->mInner2);

    // Pending-clear flag A: bit 29 of 0xC180 gates the 0xC174 clear word.
    if (self->fieldC180 & 0x4) {
        u32 v = self->mInner2.w3068; // abs 0xC174
        if ((v & 0x10) && (v & 1)) {
            self->mInner2.w3068 = 0;
            self->mInner2.w3044 = 0;
            self->mInner2.mCount = 0;
            self->mInner2.w3034 = 0;
        }
        self->fieldC180 = (self->fieldC180 & 0xFFFFFF00) | 2;
    }
    // Pending-clear flag B: bit 30 of 0xC180 gates the 0x9108 clear word.
    if (self->fieldC180 & 0x2) {
        u32 v = self->mInner1.w3068; // abs 0x9108
        if ((v & 0x10) && (v & 1)) {
            self->mInner1.w3068 = 0;
            self->mInner1.w3044 = 0;
            self->mInner1.mCount = 0;
            self->mInner1.w3034 = 0;
        }
        self->fieldC180 = ((self->fieldC180 & 0xFFFFFF00) | 1) & 0xFFFFF0FF;
    }

    {
        SelOwnerView* owner = reinterpret_cast<SelOwnerView*>(self->fieldC17C);
        u32 token = owner->mpSub4->s030()->v;
        if (func_80174C98(reinterpret_cast<void*>(owner), &token, 3) == 0) {
            goto l_ff100;
        }
    }

    {
        cf::CfObjectSelectorData* sel = lbl_eu_80663F14;
        SelObjFlagsView* obj = reinterpret_cast<SelObjFlagsView*>(
            func_800B708C((int)sel->mInner1.w3044));
        u32 f180 = self->fieldC180;
        if ((f180 & 0x400) || (f180 & 0x100) ||
            (obj != NULL && (obj->flags64 & 0x4))) {
            goto l_ff0dc;
        }

        func_800F4A98(reinterpret_cast<CfMoveEnumList*>(self), 0xB00, 0x8802);
        func_800F4A98(reinterpret_cast<CfMoveEnumList*>(&self->mList2), 0x1000, 2);
        void* pos = reinterpret_cast<SelPosDispatch*>(
                        reinterpret_cast<u8*>(reinterpret_cast<SelOwnerView*>(self->fieldC17C)) + 0x3E9C)->vAC();
        f32 facing = reinterpret_cast<SelFacingDispatch*>(
                         reinterpret_cast<SelOwnerView*>(self->fieldC17C))->getFacing();
        func_800FB270(self, pos, 0, lbl_eu_80661DB0, lbl_eu_80663F10, facing);
        pos = reinterpret_cast<SelPosDispatch*>(
                  reinterpret_cast<u8*>(reinterpret_cast<SelOwnerView*>(self->fieldC17C)) + 0x3E9C)->vAC();
        __ct__800FB044(&self->mList2, pos, 0, lbl_eu_80661DB0);
        func_800F49F8(self, &self->mList2);
        pos = reinterpret_cast<SelPosDispatch*>(
                  reinterpret_cast<u8*>(reinterpret_cast<SelOwnerView*>(self->fieldC17C)) + 0x3E9C)->vAC();
        func_800F6ED0(reinterpret_cast<CfMoveEnumList*>(self), pos);

        if (self->field620 == 0) goto l_ff090;

        if (reinterpret_cast<SelOwnerView*>(self->fieldC17C)->mpMgr->g040(2) == 0)
            goto l_fee90;
        SelAuxInfo* aux = reinterpret_cast<SelAuxDispatch*>(
                              reinterpret_cast<u8*>(reinterpret_cast<SelOwnerView*>(self->fieldC17C)) + 0x3E9C)->a110();
        if (aux->dist > lbl_eu_80666F04) goto l_fee90;
        if (self->field6080 == self->field3660) goto l_ffeff4;
        goto l_fee90;
    }

l_fee90:
    // Current selection lost or invalid.
    {
        u32 fl = reinterpret_cast<SelObjFlagsView*>(
            func_800B708C((int)(u32)func_800F6E98(self, 0)))->flags64;
        if ((fl & (0x8 | 0x100 | 0x4000)) || (fl & 0x8000)) {
            // Tear down pending request state on the singleton.
            cf::CfObjectSelectorData* s = lbl_eu_80663F14;
            if (s->mInner1.w3044 != 0) {
                u32 c = s->fieldC180;
                s->fieldC180 = ((c & 0xFFFFFF00) | 1) & 0xFFFFF0FF;
                s->fieldC178 = NULL;
                u32 v = s->mInner2.w3068;
                if (v & 1) {
                    s->mInner2.w3068 = 0;
                    s->mInner2.w3044 = 0;
                    s->mInner2.mCount = 0;
                    s->mInner2.w3034 = 0;
                }
                if (s->mInner1.w3068 & 1) {
                    s->mInner1.w3068 = 0;
                    s->mInner1.w3044 = 0;
                    s->mInner1.mCount = 0;
                    s->mInner1.w3034 = 0;
                }
            }
            // Reissue a fresh request on the primary inner list.
            void* cur = func_800F6E98(self, 0);
            s->fieldC180 = (s->fieldC180 & 0xFFFFFF00) | 2;
            s->fieldC178 = (u8*)&s->mInner1;
            // 0xC17C holds a rebased handle; adjust back into real addresses.
            u32 h = s->fieldC17C;
            if (h != 0) h += 0x3E9C;
            s->mInner1.w3068 = (s->mInner1.w3068 | 1) & 0xFFFFFFF3;
            s->mInner1.w3054 = h;
            s->mInner1.w3044 = (u32)cur;
            s->mInner1.w3048 = 0x80000004;
            s->mInner1.w304C = 0x4002;
            func_800FD774(&s->mInner1);
            reinterpret_cast<SelOwnerView*>(self->fieldC17C)->mpMgr->g030(4, 1);
        }
        reinterpret_cast<SelOwnerView*>(self->fieldC17C)->mpMgr->g030(2, 1);
        goto l_ff0d0;
    }

l_ffeff4:
    // Scan the rebuilt candidates for the currently highlighted entry.
    {
        void* target = func_800B708C((int)lbl_eu_80663F14->mInner1.w3044);
        u32 found = 0;
        for (u32 i = 0; i < self->field620; i++) {
            SelObjFlagsView* o = reinterpret_cast<SelObjFlagsView*>(
                func_800B708C((int)(u32)func_800F6E98(self, i)));
            if ((void*)o == target) {
                u32 fl = o->flags64;
                if ((fl & (0x8 | 0x100 | 0x4000)) || (fl & 0x8000)) {
                    found = 1;
                    break;
                }
            }
        }
        reinterpret_cast<SelOwnerView*>(self->fieldC17C)->mpMgr->g030(4, found);
        goto l_ff0d0;
    }

l_ff090:
    // No candidates at all: notify both manager channels.
    {
        reinterpret_cast<SelOwnerView*>(self->fieldC17C)->mpMgr->g034(4);
        reinterpret_cast<SelOwnerView*>(self->fieldC17C)->mpMgr->g034(2);
        goto l_ff0d0;
    }

l_ff100:
    reinterpret_cast<SelOwnerView*>(self->fieldC17C)->mpMgr->g034(4);
    goto l_end;

l_ff0dc:
    reinterpret_cast<SelOwnerView*>(self->fieldC17C)->mpMgr->g034(4);
    goto l_end;

l_ff0d0:
    self->field6080 = self->field3660;
l_end:
    // Retire one-shot bits once the request cycle has drained.
    u32 c = self->fieldC180;
    if (c & 0x100) {
        self->mInner1.w3068 &= 0xFFFFFFBF;
        self->mInner2.w3068 &= 0xFFFFFFBF;
        self->fieldC180 = c & 0xFFFFFEFF;
    }
}
