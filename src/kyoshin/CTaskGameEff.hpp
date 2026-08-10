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

// C-linkage retail effect helpers (CfObjectMove/CScn management).
extern "C" void func_800ACF78(void* obj, void* target, u32 child);
extern "C" void func_800ACEF8(void* obj, const void* src);
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
    /* 0x74 */ reslist<CScn> mSceneList;        // scene list (vtable lbl_eu_80525C6C)
}; // size 0x94

// Render-callback / scene helper imports (retail unmangled symbols - keep
// extern "C" so the call reloc is func_80495FC8/80495FDC, not mangled).
extern "C" void func_80495FC8(CScn* scn, IScnRender* cb, u32 prio);
extern "C" void func_80495FDC(CScn* scn, IScnRender* cb, u32 prio);

// Effect-task singleton (set by Init; retail lbl_eu_80663D40).
extern CTaskGameEff* lbl_eu_80663D40;

