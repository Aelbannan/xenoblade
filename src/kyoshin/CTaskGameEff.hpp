#pragma once

#include <types.h>
#include "monolib/work/CProcess.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// Effect-singleton utilities with retail-unmangled C linkage (referenced from
// cbRenderBefore/func_80045284; operate on the effect singleton lbl_eu_8065FC18).
extern "C" void func_804CBB60(void*);
extern "C" void func_804CBB84(void*, void*);
extern "C" void func_804CBE48(void*);
extern "C" void func_804CC104(void*);
extern "C" void func_804CBEE8(void*);
// Per-frame effect update helpers (func_80045044) and singleton teardown
// (Term) - retail unmangled C linkage like the others.
extern "C" void func_804CBB14(void*, f32);
extern "C" void func_804CBC90(void*);
extern "C" void func_804CBD14(void*);
extern "C" void func_804CBDB4(void*);
extern "C" void func_804CC154(void*);

// Default effect time constant used by func_80044FBC when the caller does not
// supply an explicit time (retail sdata2 float).
extern f32 lbl_eu_80665D94;

// Default effect time constant stored into field_0x6C by the ctor (sdata2).
extern f32 lbl_eu_80665D90;

// 3-word view of a pointer-to-member-function slot (mMoveFunc/mDrawFunc),
// used by the raw-init ctor to null both callback hooks.
struct CTaskGameEffPtmf {
    u32 w[3];
};

// Self-allocating factory for the after-task (defined in cf/CTaskGameEffAfter.cpp).
void* __ct__CTaskGameEffAfter(CProcess* parent);

// Effect-singleton setup helpers (retail unmangled; operate on lbl_eu_8065FC18).
// func_804CB9F4 queries the effect resource size; func_804CBA00/804CBAA8 bind
// the effect buffer and attach the scene to the singleton.
extern "C" void* func_804CB9F4(void);
extern "C" void func_804CBA00(void*, void*, void*);
extern "C" void func_804CBAA8(void*, void*, int);

// Raw CProcess constructor + global null pointer-to-member-function constant
// (used by the raw-init style ctor, mirroring CTaskGameEffAfter).
extern "C" void __ct__8CProcessFv(CProcess* self);

// Retail vtables / thunk pointers in .data. lbl_eu_80525BFC is the final
// CTaskGameEff vtable; lbl_eu_80525CAC the interim; the 0x54/0x58/0x70/0x74
// fields hold CScn render-callback thunks.
extern const u8 lbl_eu_80525BFC[];
extern const u8 lbl_eu_80525CAC[];
extern const u8 lbl_eu_80525C90[];
extern const u8 lbl_eu_80525C84[];
extern const u8 lbl_eu_80525C6C[];

#include "kyoshin/cfsys/CfObjectImplMove.hpp"

// Virtual container embedded at battle-object +0x3E9C (func_8004513C resolves
// effects through its vtable slot 0x220, which takes the full id word).
// Never instantiated here, so no vtable is emitted.
class EffHostContainer {
public:
    virtual ~EffHostContainer();
    virtual void* f002(); virtual void* f003(); virtual void* f004(); virtual void* f005();
    virtual void* f006(); virtual void* f007(); virtual void* f008(); virtual void* f009();
    virtual void* f010(); virtual void* f011(); virtual void* f012(); virtual void* f013();
    virtual void* f014(); virtual void* f015(); virtual void* f016(); virtual void* f017();
    virtual void* f018(); virtual void* f019(); virtual void* f020(); virtual void* f021();
    virtual void* f022(); virtual void* f023(); virtual void* f024(); virtual void* f025();
    virtual void* f026(); virtual void* f027(); virtual void* f028(); virtual void* f029();
    virtual void* f030(); virtual void* f031(); virtual void* f032(); virtual void* f033();
    virtual void* f034(); virtual void* f035(); virtual void* f036(); virtual void* f037();
    virtual void* f038(); virtual void* f039(); virtual void* f040(); virtual void* f041();
    virtual void* f042(); virtual void* f043(); virtual void* f044(); virtual void* f045();
    virtual void* f046(); virtual void* f047(); virtual void* f048(); virtual void* f049();
    virtual void* f050(); virtual void* f051(); virtual void* f052(); virtual void* f053();
    virtual void* f054(); virtual void* f055(); virtual void* f056(); virtual void* f057();
    virtual void* f058(); virtual void* f059(); virtual void* f060(); virtual void* f061();
    virtual void* f062(); virtual void* f063(); virtual void* f064(); virtual void* f065();
    virtual void* f066(); virtual void* f067(); virtual void* f068(); virtual void* f069();
    virtual void* f070(); virtual void* f071(); virtual void* f072(); virtual void* f073();
    virtual void* f074(); virtual void* f075(); virtual void* f076(); virtual void* f077();
    virtual void* f078(); virtual void* f079(); virtual void* f080(); virtual void* f081();
    virtual void* f082(); virtual void* f083(); virtual void* f084(); virtual void* f085();
    virtual void* f086(); virtual void* f087(); virtual void* f088(); virtual void* f089();
    virtual void* f090(); virtual void* f091(); virtual void* f092(); virtual void* f093();
    virtual void* f094(); virtual void* f095(); virtual void* f096(); virtual void* f097();
    virtual void* f098(); virtual void* f099(); virtual void* f100(); virtual void* f101();
    virtual void* f102(); virtual void* f103(); virtual void* f104(); virtual void* f105();
    virtual void* f106(); virtual void* f107(); virtual void* f108(); virtual void* f109();
    virtual void* f110(); virtual void* f111(); virtual void* f112(); virtual void* f113();
    virtual void* f114(); virtual void* f115(); virtual void* f116(); virtual void* f117();
    virtual void* f118(); virtual void* f119(); virtual void* f120(); virtual void* f121();
    virtual void* f122(); virtual void* f123(); virtual void* f124(); virtual void* f125();
    virtual void* f126(); virtual void* f127(); virtual void* f128(); virtual void* f129();
    virtual void* f130(); virtual void* f131(); virtual void* f132(); virtual void* f133();
    virtual void* f134();
    virtual void* vfn220(u32 id);   // vtable slot 0x220
};

// Battle/effect host object handed to func_8004513C: embeds a virtual
// container at +0x3E9C whose vtable slot 0x220 resolves an effect by id.
struct EffHostObj {
    u8 field_0x0[0x3E9C];
    EffHostContainer field_0x3E9C;
};

// C-linkage retail effect helpers (CfObjectMove/CScn management).
extern "C" void func_800ACF78(void* obj, void* target, u32 child);
extern "C" void func_800ACEF8(void* obj, const void* src);
extern "C" void* func_800817BC__Q22cf13CfGameManagerFv(u32 value, u32 unused);
extern "C" void* func_8008187C__Q22cf13CfGameManagerFv(u32 index);
extern "C" void* __dynamic_cast(void* obj, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);

// RTTI typeinfos for __dynamic_cast of effect objects.
extern "C" const void* lbl_eu_80661970;
extern "C" const void* lbl_eu_806618F0;

// Minimal CScn declaration local to this TU set: only the render-callback
// members used by Init/Term/func_800452EC/func_800453EC. Declared with the
// real class tag so member calls emit the retail mangled symbols
// addRenderCB__4CScnFP10IScnRenderUlUl / removeRenderCB__4CScnFP10IScnRender.
// Guarded so the kyoshin task headers can be included together in one TU
// (same minimal declaration appears in CTaskGameEvt.hpp / CTaskGamePic.hpp).
#ifndef KYOSHIN_MINIMAL_CSCN_DECLARED
#define KYOSHIN_MINIMAL_CSCN_DECLARED
class CScn {
public:
    void addRenderCB(IScnRender* cb, u32 prio, u32 flag);
    void removeRenderCB(IScnRender* cb);
};
#endif

// Layout mirror of the retail _reslist_base<CScn> instantiation (same shape
// as monolib's reslist.hpp template). The retail dtor symbols use old flat
// template mangling, so the destructor is a plain global function over this.
struct EffResListNode {
    EffResListNode* mNext;   // 0x00
    EffResListNode* mPrev;   // 0x04
    CScn* mItem;             // 0x08 - stored scene pointer

    // Mirror of _reslist_node<T*>::setItem - guarded store with rethrow.
    void setItem(CScn* value) {
        if (value != nullptr) {
            try {
                mItem = value;
            } catch (...) {
                throw;
            }
        }
    }
};
struct ResListCScn {
    void* mVtable;             // 0x00 - _reslist_base<CScn> vtable
    EffResListNode* mHead;     // 0x04 - &mStartNode
    EffResListNode mStartNode; // 0x08
    EffResListNode* mList;     // 0x14 - slot array
    int mCapacity;             // 0x18
    u8 unk1C;                  // 0x1C - when true, array is not owned

    int findFirstEmptySlotIndex() {
        int i = 0;
        while (i < mCapacity) {
            if (mList[i].mNext == nullptr) break;
            i++;
        }
        return i;
    }

    void push_back(CScn* item) {
        EffResListNode* startNode = mHead;
        int i = findFirstEmptySlotIndex();
        EffResListNode* temp = &mList[i];
        temp->setItem(item);
        temp->mNext = startNode;
        temp->mPrev = startNode->mPrev;
        startNode->mPrev->mNext = temp;
        startNode->mPrev = temp;
    }
};

// Doubly-linked node of the effect scene list (embedded in CTaskGameEff at
// 0x7C; the mSceneList field at 0x78 points at the self-linked header node).
struct EffListNode {
    EffListNode* next;   // 0x00
    EffListNode* prev;   // 0x04
    CScn* scene;         // 0x08
};

// CRTP task base - canonical monolib template (declared-only members so the
// owning unit cpp can emit the retail out-of-line Move/Draw/dtor symbols via
// explicit `template<>` specializations).
#include "monolib/work/CTTask.hpp"
#include "monolib/util/reslist.hpp"

class CTaskGameEff : public CTTask<CTaskGameEff> {
public:
    // High-priority render callback subobject at 0x70 (prio 1). Its dtor is
    // the retail __dt__Q212CTaskGameEff18CEffRenderHighPrioFv; declared-only
    // so the containing dtor emits an out-of-line member-dtor call.
    class CEffRenderHighPrio : public IScnRender {
    public:
        virtual ~CEffRenderHighPrio();
    };

    virtual ~CTaskGameEff();
    void Init();
    void Term();
    void cbRenderBefore();

    /* 0x54 */ IScnRender field_0x54;    // render callback (prio 0xe)
    /* 0x58 */ IScnRender field_0x58;    // render callback (prio 8)
    /* 0x5C */ CScn* mScene;
    /* 0x60 */ u32 mMemAlloc;
    /* 0x64 */ u8 mActive;
    /* 0x68 */ u32 field_0x68;
    /* 0x6C */ f32 field_0x6C;
    /* 0x70 */ CEffRenderHighPrio field_0x70;   // render callback (prio 1)
    /* 0x74 */ ResListCScn mSceneList;          // scene list (vtable lbl_eu_80525C6C)
}; // size 0x94

// Render-callback / scene helper imports (retail unmangled symbols - keep
// extern "C" so the call reloc is func_80495FC8/80495FDC, not mangled).
extern "C" void func_80495FC8(CScn* scn, IScnRender* cb, u32 prio);
extern "C" void func_80495FDC(CScn* scn, IScnRender* cb, u32 prio);

// Effect-task singleton (set by Init; retail lbl_eu_80663D40).
extern CTaskGameEff* lbl_eu_80663D40;

