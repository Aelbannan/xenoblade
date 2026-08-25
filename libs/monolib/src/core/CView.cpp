#include <types.h>
// Retail names are unmangled (extern "C"): the CView dtor shell and the
// CFontLayer-base deleting-dtor shell referenced by the vtable.
extern "C" void __dt__5CViewFv(void*);
extern "C" void func_8043FBC4(u8* self) {
    __dt__5CViewFv((char*)self - 0x1C4);
}

// us-8043ef34: deleting-dtor shell (class id 0x8043C59C -- vtbl/typeinfo anchor
// in this TU): if (self && flags > 0) operator delete(self); return self;
extern "C" void* __dt__8043C59C(void* self, int flags) {
    if (self != NULL && flags > 0) {
        operator delete(self);
    }
    return self;
}

// us-8043ef74: CViewFrame::~CViewFrame -- same deleting-dtor shell.
extern "C" void* __dt__10CViewFrameFv(void* self, int flags) {
    if (self != NULL && flags > 0) {
        operator delete(self);
    }
    return self;
}

#include "monolib/core.hpp"
#include "monolib/core/CViewRectData.hpp"
#include "monolib/device.hpp"
#include "monolib/work/CWorkThread.hpp"

//Not yet decompiled (monolib/src/core/CSplitFrame.cpp); only its virtual destructor is
//referenced here (CView::wkUpdate deletes it once it goes inactive), so a minimal stub with a
//single virtual function is enough to reproduce the retail vtable-slot call.
class CSplitFrame {
public:
    virtual ~CSplitFrame();
};

extern "C" CViewRoot* getInstance__9CViewRootFv();
CView* getCurrent__9CViewRootFv();
void invalidCurrent__9CViewRootFP5CView(CView* view);
extern "C" GXRenderModeObj* getRenderModeObj__9CDeviceVIFv();
extern "C" CView* getView1__11CSplitFrameFv(void* splitFrame);
extern "C" CView* getView2__11CSplitFrameFv(void* splitFrame);
s16 getSplitLine__11CSplitFrameFv(void* splitFrame);
void setSplitLine__11CSplitFrameFs(void* splitFrame, s16 line);
extern "C" bool isActive__11CSplitFrameFv(void* splitFrame);
void apply__11CSplitFrameFv(void* splitFrame);
void setCurrent__9CViewRootFP5CView(CView* view);
CWorkThread* getWorkThread__9CWorkUtilFUl(u32 workId);
extern "C" void func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(CViewRectDataCore* data, const ml::CPnt16& size);
extern "C" void func_8043FD10__10CViewFrameFR7CRect16PC10CViewFrame(ml::CRect16* rect, const CViewFrame* frame);
extern "C" void func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(CViewRectDataCore* data, const ml::CPnt16& maxSize);
extern "C" void __ct__CViewFrame(CViewFrame* frame);
// CFontLayer base ctor (un-mangled retail symbol; defined in the shipped
// CFontLayer.o, which is NonMatching so no C++ declaration exists anywhere).
extern "C" void __ct__CFontLayer(CFontLayer* layer);
// Third arg is forwarded from CView::func_8043CCCC's own r4 (retail keeps
// the dead mr r5,r4 because the callee prototype has three parameters).
extern "C" void func_80441EF0__10CViewFrameFR7CRect16PC10CViewFrame(ml::CRect16* rect,
                                                          const CViewFrame* frame,
                                                          u32 arg);
void func_8043FC60__10CViewFrameFUl(CViewFrame* frame, u32 owner);
u32 func_8044BE2C__8CGXCacheFv();
void* allocate_array__Q23mtl10MemManagerFUlUl(u32 size, mtl::ALLOC_HANDLE handle);
void func_8043E010__5CViewFv(CView* view);
CView* getFullScreenView__9CViewRootFv();
CProc* pssGetRoot__5CProcFP5CProc(CProc* proc);
void func_8044A578__8CGXCacheFv(CGXCache* cache, ml::CCol4* color, int flag);
void getScissorRect1__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(ml::CRect16* out, void* split);
void getScissorRect2__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(ml::CRect16* out, void* split);
void func_80442B54__9CViewRootFPvPv(void* viewRect, void* scissorRect, void* insetRect);
void func_80442C68__9CViewRootFv();
void func_8044BE38__8CGXCacheFv(CGXCache* cache);
void func_80442DA8__9CViewRootFv();
void fontFlush__10CFontLayerFi(CFontLayer* layer, int flag);
void render__10CViewFrameFv(CViewFrame* frame);
// Incomplete arrays force lis/addi (not SDA lwz) -- same as CViewRoot::create.
extern char lbl_eu_8056B298[];
extern char lbl_eu_8056B280[];
// FloatUtils-owned .sdata2 constants used by the ctor (retail references these
// instead of pooling TU-local copies; CView.o has .sdata2 size 0).
extern float lbl_eu_8066A2D0;  // 1.0f
extern float lbl_eu_8066A2D4;  // 0.6f
// reslist<IWorkEvent*> / _reslist_base<IWorkEvent*> / CMsgParam<10> vtable
// addresses used by the ctor -- the objects are emitted by the explicit
// instantiations below and renamed by UNIT_RULES["CView.o"].
extern char lbl_eu_8056B6F0[];
extern char lbl_eu_8056B6D8[];
extern char lbl_eu_8056B6CC[];
// reslist<IWorkEvent*> / _reslist_base<IWorkEvent*> / CMsgParam<10> vtables are
// emitted by MWCC itself from the FORCEKEEP construction below (their dtor
// slots reference template-mangled symbols that cannot be spelled in C++);
// UNIT_RULES["CView.o"] renames them to lbl_eu_8056B6D8 / lbl_eu_8056B6F0 /
// lbl_eu_8056B6CC and the __RTTI__ typeinfos to lbl_eu_80663588 / 80663590 /
// 80663580.
//
// NOTE: data_vtables.hpp is NOT included here. Under this TU's flags
// (-ipa file -RTTI on, set in configure.py) the header's global-scope
// `extern "C" void* __RTTI__10IWorkEvent` / `__RTTI__11CWorkThread` collide
// with MWCC's RTTI name table once CDeviceBase.hpp's inline ctor is in scope
// (MWCC error 10322 "illegal name overloading"); CProc/CProcRoot/CTaskManager
// avoid it with -RTTI off, which this TU cannot use. The same C-linkage
// symbols are declared inside a namespace instead (MWCC does not mangle
// extern "C" names, so the emitted reloc name is still exactly
// `__RTTI__10IWorkEvent` / `__RTTI__11CWorkThread`), and the remaining
// function externs are declared locally.
namespace RTTIExterns {
extern "C" void* __RTTI__10IWorkEvent;
extern "C" void* __RTTI__11CWorkThread;
}

// Retail <10> deleting-dtor shape: no vtable store; the entry clears are
// guarded by a null-test of the entry-array address (r3+4, not foldable).
// NOTE: our build (-RTTI on) additionally emits a vptr store here (+0xC) that
// retail lacks — sibling reslist dtors carry the store in BOTH builds (their
// classes have bases); standalone CMsgParam<10> gets it elided in retail's
// compiler run. Experiments: extern "C" free-function forms stay frameless
// (bclr early-return, no r31/prologue) and cannot reproduce the compiler-
// generated dtor's register convention. See attempts.jsonl us-8043f120.
template <>
CMsgParam<10>::~CMsgParam(){
    if (mEntries != NULL) {
        clear();
    }
}

extern "C" int WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
extern "C" int OnFileEvent__10IWorkEventFP10CEventFile(void*);
extern "C" int WorkEvent3__10IWorkEventFPv(void*);
extern "C" int WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb(int);
extern "C" int WorkEvent6__10IWorkEventFv();
extern "C" int WorkEvent7__10IWorkEventFv();
extern "C" int WorkEvent8__10IWorkEventFv();
extern "C" int WorkEvent9__10IWorkEventFv();
extern "C" int WorkEvent10__10IWorkEventFv();
extern "C" int WorkEvent11__10IWorkEventFv();
extern "C" int WorkEvent12__10IWorkEventFv();
extern "C" int WorkEvent13__10IWorkEventFv();
extern "C" int WorkEvent14__10IWorkEventFv();
extern "C" int WorkEvent15__10IWorkEventFv();
extern "C" int WorkEvent16__10IWorkEventFv();
extern "C" int WorkEvent17__10IWorkEventFv();
extern "C" int WorkEvent18__10IWorkEventFv();
extern "C" int WorkEvent19__10IWorkEventFv();
extern "C" int WorkEvent20__10IWorkEventFv();
extern "C" int WorkEvent21__10IWorkEventFv();
extern "C" int WorkEvent22__10IWorkEventFv();
extern "C" int WorkEvent23__10IWorkEventFv();
extern "C" int WorkEvent24__10IWorkEventFv();
extern "C" int WorkEvent25__10IWorkEventFv();
extern "C" int WorkEvent26__10IWorkEventFv();
extern "C" int WorkEvent27__10IWorkEventFv();
extern "C" int WorkEvent28__10IWorkEventFv();
extern "C" int WorkEvent29__10IWorkEventFv();
extern "C" int WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);
extern "C" bool wkStandbyLogin__11CWorkThreadFv(CView* self);
extern "C" bool wkStandbyLogout__11CWorkThreadFv(CWorkThread* self);
extern "C" bool wkStandbyLogin__5CViewFv(CView* self);
extern "C" bool wkStandbyLogout__5CViewFv(CView* self);
extern "C" void wkUpdate__5CViewFv();

#include "decomp.h"

// === Blob monolibdata1/1d dissolve: CView.cpp owns .rodata 0x805225E0-
// 0x80522650, .sdata 0x80663578-0x806635A0, .data 0x8056B5C0-0x8056B700,
// .bss 0x80656EC8-0x80656EE8, .sbss 0x806655C8-0x806655D0. ===

// RTTI locators (.sdata, 8 bytes each): { name, class-info }.
extern const char lbl_eu_8066A2D8[];  // "CView" (.sdata2, another TU's range)
extern u32 lbl_eu_8056B6B0[7];  // CView class-info, defined below

// RTTI CView locator. Declared BEFORE the FORCEKEEP instantiation below so it
// lands first in .sdata (retail order: CView, CMsgParam<10>,
// reslist<IWorkEvent*>, _reslist_base<IWorkEvent*>, CFontLayer).
extern "C" u32 lbl_eu_80663578[2] = { (u32)&lbl_eu_8066A2D8, (u32)&lbl_eu_8056B6B0 };

// reslist<IWorkEvent*> / _reslist_base<IWorkEvent*> / CMsgParam<10> template
// vtables + RTTI (weak, compiler-named; UNIT_RULES["CView.o"] renames them to
// lbl_eu_8056B6D8 / lbl_eu_8056B6F0 / lbl_eu_8056B6CC and the __RTTI__
// typeinfos to lbl_eu_80663588 / 80663590 / 80663580). This TU needs -ipa off
// (the data_vtables.hpp __RTTI__ externs collide with MWCC's RTTI name table
// under -ipa file once any virtual class is in scope), and -ipa off drops
// unreferenced weak data, so the construction below keeps the vtables alive
// (the inline ctors emit the vtable references; the explicit dtor calls keep
// the template dtors, which are retail CView.cpp text functions).
//
// Placed at the TOP of the data block so MWCC's RTTI name-string pool entries
// ("CMsgParam<10>" / "reslist<IWorkEvent *>" / "_reslist_base<IWorkEvent *>")
// and the .sdata RTTI locators emit BEFORE the manual CFontLayer
// string/locator, reproducing the retail .rodata order (template names at
// 0x805225E0/805225F0/80522608, CFontLayer at 0x80522624, check-timeout
// message at 0x80522630) and .sdata order (CView @0x80663578, CMsgParam<10>
// @0x80663580, reslist<IWorkEvent*> @0x80663588, _reslist_base<IWorkEvent*>
// @0x80663590, CFontLayer @0x80663598).
extern "C" void FORCEKEEP_CView_templates() {
    reslist<IWorkEvent*> keepResList;
    CMsgParam<10> keepMsg(0);
    (void)keepResList;
    (void)keepMsg;
}

// RTTI name strings (.rodata). "CView" (lbl_eu_8066A2D8) lives in .sdata2
// (another TU's range) and is referenced as an extern. CFontLayer's is defined
// here (its typeinfo sits in this TU's .sdata range). The template name
// strings are emitted by MWCC as pool entries from the FORCEKEEP construction
// above; retail labels them lbl_eu_805225E0 / lbl_eu_805225F0 /
// lbl_eu_80522608 (UNIT_RULES renames the @N pool labels).
// Template RTTI name strings, defined in retail order (UNIT_RULES retargets
// the auto-emitted pool relocs onto these and drops the pooled copies).
const char lbl_eu_805225E0[14] = {0x43,0x4D,0x73,0x67,0x50,0x61,0x72,0x61,0x6D,0x3C,0x31,0x30,0x3E,0x00};  /* "CMsgParam<10>" (13+NUL) */
const char lbl_eu_805225F0[22] = {0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x49,0x57,0x6F,0x72,0x6B,0x45,0x76,0x65,0x6E,0x74,0x20,0x2A,0x3E,0x00};  /* "reslist<IWorkEvent *>" (21+NUL) */
const char lbl_eu_80522608[28] = {0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x49,0x57,0x6F,0x72,0x6B,0x45,0x76,0x65,0x6E,0x74,0x20,0x2A,0x3E,0x00};  /* "_reslist_base<IWorkEvent *>" (27+NUL) */
const char lbl_eu_80522624[11] = {0x43,0x46,0x6F,0x6E,0x74,0x4C,0x61,0x79,0x65,0x72,0x00};  /* "CFontLayer" (10+NUL; retail 0xB) */
// "\u30e1\u30e2\u30ea\u304c\u2026\u306a\u304f\u3057\u307e\u3057\u305f" check-timeout
// message (retail .rodata 0x80522630;
// 0x20 includes the splitter-absorbed 5-byte alignment pad). The retail
// wkStandbyLogout timeout path that references it is not yet decompiled, so
// DECOMP_FORCEACTIVE below keeps the symbol alive.
const char lbl_eu_80522630[0x20] =
    "\x83\x8d\x83\x4f\x83\x41\x83\x45\x83\x67\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd";

// RTTI CFontLayer locator -- after FORCEKEEP so it lands LAST in .sdata.
extern "C" u32 lbl_eu_80663598[2] = { (u32)&lbl_eu_80522624, 0 };

// CView vtable (.data, 0xD0): IWorkEventVtbl (0xA0) + the CFontLayer secondary
// vtable at +0x74 (this-adjust -0x1C4). Defined BEFORE the class-info so the
// .data emission order matches retail (vtable @0x8056B5E0, class-info
// @0x8056B6B0, template vtables @0x8056B6CC).
extern "C" void func_8043FBC4(u8* self);
extern "C" void CView_UnkVirtualFunc1__5CViewFv(CView* self);
extern "C" void detachRenderWork__5CViewFP11CWorkThread(void* self, void* thread);
extern "C" int CView_UnkVirtualFunc3__5CViewFv(CView* self, IWorkEvent* event);
extern "C" int CView_UnkVirtualFunc4__5CViewFv(CView* self, IWorkEvent* event);
extern "C" int CView_UnkVirtualFunc5__5CViewFv();
extern "C" int CView_UnkVirtualFunc6__5CViewFv();
extern "C" void CView_UnkVirtualFunc7__5CViewFv();
extern "C" void CView_UnkVirtualFunc8__5CViewFv(CView* self);
extern "C" void CView_UnkVirtualFunc9__5CViewFv(CView* self);

u32 lbl_eu_8056B5E0[0xD0 / 4] = {
    // IWorkEventVtbl
    (u32)&lbl_eu_80663578, 0, (u32)&__dt__5CViewFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__5CViewFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__5CViewFv,
    (u32)&wkStandbyLogout__5CViewFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
    // CFontLayer secondary vtable (0x30)
    (u32)&lbl_eu_80663578, 0xFFFFFE3C, (u32)&func_8043FBC4,
    (u32)&CView_UnkVirtualFunc1__5CViewFv, (u32)&detachRenderWork__5CViewFP11CWorkThread,
    (u32)&CView_UnkVirtualFunc3__5CViewFv, (u32)&CView_UnkVirtualFunc4__5CViewFv,
    (u32)&CView_UnkVirtualFunc5__5CViewFv, (u32)&CView_UnkVirtualFunc6__5CViewFv,
    (u32)&CView_UnkVirtualFunc7__5CViewFv, (u32)&CView_UnkVirtualFunc8__5CViewFv,
    (u32)&CView_UnkVirtualFunc9__5CViewFv,
};

// CView class-info (.data, 0x1C): [RTTI(CFontLayer), 0x1C4, RTTI(IWorkEvent),
// 0, RTTI(CWorkThread), 0, 0].
u32 lbl_eu_8056B6B0[7] = {
    (u32)&lbl_eu_80663598, 0x1C4, (u32)&RTTIExterns::__RTTI__10IWorkEvent, 0,
    (u32)&RTTIExterns::__RTTI__11CWorkThread, 0, 0,
};

// Fake interface for the +0xC4 vt-dispatch in CView_UnkVirtualFunc7: real
// virtuals emit the retail r12 dispatch (lwz r12,0(r3); lwz r12,0xC4(r12))
// instead of the manual cast's scratch r4. 41 fillers + 8 hidden = slot 49.
// Fake interface for the +0xC4 vt-dispatch in CView_UnkVirtualFunc7: real
// virtuals emit the retail r12 dispatch (lwz r12,0(r3); lwz r12,0xC4(r12))
// instead of the manual cast's scratch r4. 41 fillers + 8 hidden = slot 49.
// Same trick, tuned for the +0xB8 slot used by CView_UnkVirtualFunc4: the
// dispatched method takes the forwarded event in r4.
struct CViewVtB8If {
    virtual void f00();
    virtual void f01();
    virtual void f02();
    virtual void f03();
    virtual void f04();
    virtual void f05();
    virtual void f06();
    virtual void f07();
    virtual void f08();
    virtual void f09();
    virtual void f0A();
    virtual void f0B();
    virtual void f0C();
    virtual void f0D();
    virtual void f0E();
    virtual void f0F();
    virtual void f10();
    virtual void f11();
    virtual void f12();
    virtual void f13();
    virtual void f14();
    virtual void f15();
    virtual void f16();
    virtual void f17();
    virtual void f18();
    virtual void f19();
    virtual void f1A();
    virtual void f1B();
    virtual void f1C();
    virtual void f1D();
    virtual void f1E();
    virtual void f1F();
    virtual void f20();
    virtual void f21();
    virtual void f22();
    virtual void f23();
    virtual void f24();
    virtual void f25();
    virtual void f26();
    virtual void f27();
    virtual void f28();
    virtual void f29();
    virtual void f2A();
    virtual void f2B();
    virtual void fnB8(IWorkEvent* event);
};

// Same trick, one slot below CViewVtB8If: the +0xB4 slot dispatched by
// CView_UnkVirtualFunc3 to each split-frame pane (takes the forwarded event).
struct CViewVtB4If {
    virtual void f00();
    virtual void f01();
    virtual void f02();
    virtual void f03();
    virtual void f04();
    virtual void f05();
    virtual void f06();
    virtual void f07();
    virtual void f08();
    virtual void f09();
    virtual void f0A();
    virtual void f0B();
    virtual void f0C();
    virtual void f0D();
    virtual void f0E();
    virtual void f0F();
    virtual void f10();
    virtual void f11();
    virtual void f12();
    virtual void f13();
    virtual void f14();
    virtual void f15();
    virtual void f16();
    virtual void f17();
    virtual void f18();
    virtual void f19();
    virtual void f1A();
    virtual void f1B();
    virtual void f1C();
    virtual void f1D();
    virtual void f1E();
    virtual void f1F();
    virtual void f20();
    virtual void f21();
    virtual void f22();
    virtual void f23();
    virtual void f24();
    virtual void f25();
    virtual void f26();
    virtual void f27();
    virtual void f28();
    virtual void f29();
    virtual void f2A();
    virtual void fnB4(IWorkEvent* event);
};

struct CViewVtC4If {
    virtual void f00();
    virtual void f01();
    virtual void f02();
    virtual void f03();
    virtual void f04();
    virtual void f05();
    virtual void f06();
    virtual void f07();
    virtual void f08();
    virtual void f09();
    virtual void f0A();
    virtual void f0B();
    virtual void f0C();
    virtual void f0D();
    virtual void f0E();
    virtual void f0F();
    virtual void f10();
    virtual void f11();
    virtual void f12();
    virtual void f13();
    virtual void f14();
    virtual void f15();
    virtual void f16();
    virtual void f17();
    virtual void f18();
    virtual void f19();
    virtual void f1A();
    virtual void f1B();
    virtual void f1C();
    virtual void f1D();
    virtual void f1E();
    virtual void f1F();
    virtual void f20();
    virtual void f21();
    virtual void f22();
    virtual void f23();
    virtual void f24();
    virtual void f25();
    virtual void f26();
    virtual void f27();
    virtual void f28();
    virtual void f29();
    virtual void f2A();
    virtual void f2B();
    virtual void f2C();
    virtual void f2D();
    virtual void f2E();
    virtual void fnC4();
};


// sbss singleton (retail 8 bytes; only offset 0 is referenced by code, the
// second word is dead). The 8-byte struct reproduces the retail symbol size.
struct CViewSbssSingleton {
    CView* ptr;
    u32 pad;
} lbl_eu_806655C8;

// Keep the not-yet-decompiled wkStandbyLogout check-timeout message alive.
DECOMP_FORCEACTIVE(CView_cpp, lbl_eu_80522630);
DECOMP_FORCEACTIVE(CView_cpp, lbl_eu_805225E0);
DECOMP_FORCEACTIVE(CView_cpp, lbl_eu_805225F0);
DECOMP_FORCEACTIVE(CView_cpp, lbl_eu_80522608);

ml::CCol4 lbl_8065A0C8;

ml::CCol4 CView::sFrameColor;

void CView::setDefaultFrameColor(const ml::CCol4& color) {
    sFrameColor = color;
}

CView* CView::getCurrentView() {
    return lbl_eu_806655C8.ptr;
}

// Enqueue tag 6 onto the context ring (CMsgParam<10>). High-level enqueue
// restores signed ring index + stwux; retail -0x40 spill interleave is closed
// Chaitin near-miss - keep iterating in high-level C. behaviour:view-set-current-ring.
void CView::setCurrent() {
    CMsgParam<10>& messages =
        *reinterpret_cast<CMsgParam<10>*>(&mContextMsgVtable);
    messages.enqueue(6);
}

void CView::setRect(const ml::CRect16& rect) {
    // All CPnt16 so MWCC packs 4-byte homes (CRect16 aligns to 8 and leaves a
    // hole at sp+0xC). Contiguous pos+size pairs cast to CRect16* for the helper.
    // Decl order (first = higher addr): normalSize@0x18 ... modeSize@0x8.
    // modeSize as u16 pair so fbWidth/efbHeight loads stay lhz (not lha via s16).
    ml::CPnt16 normalSize;
    ml::CPnt16 normalPos;
    ml::CPnt16 splitSize;
    ml::CPnt16 splitPos;
    struct {
        u16 x;
        u16 y;
    } modeSize;
    CWorkThread* parentSnap;
    CWorkThread* sourceParent;
    GXRenderModeObj* renderMode;

    if ((unk278 & 1) != 0) {
        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
            (ml::CRect16*)&splitPos, &mFrame);
        splitSize.x = -splitPos.x;
        parentSnap = mParent;
        splitSize.y = -splitPos.y;
        *(u32*)&mFrame.mContentX = *(u32*)&splitSize;

        if (getInstance__9CViewRootFv() == parentSnap) {
            sourceParent = nullptr;
        } else {
            sourceParent = mParent;
            if (sourceParent != nullptr) {
                goto setRect_parent_typed;
            }
            sourceParent = nullptr;
            goto setRect_parent_done;
        setRect_parent_typed:
            {
                int parentType = sourceParent->mType;
                if (THREAD_CVIEW > parentType || parentType >= THREAD_CVIEW_MAX) {
                    sourceParent = nullptr;
                }
            }
        setRect_parent_done:;
        }

        if (sourceParent != nullptr) {
            func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(
                &mRectData, static_cast<CView*>(sourceParent)->mRectData.mBoundsSize);
        } else {
            {
                u16 modeHeight;
                u16 modeWidth;

                renderMode = getRenderModeObj__9CDeviceVIFv();
                modeHeight = renderMode->efbHeight;
                renderMode = getRenderModeObj__9CDeviceVIFv();
                modeWidth = renderMode->fbWidth;
                modeSize.x = modeWidth;
                modeSize.y = modeHeight;
                func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(
                    &mRectData, *(const ml::CPnt16*)&modeSize);
            }
        }
        goto setRect_tail;
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
        (ml::CRect16*)&normalPos, &mFrame);
    normalSize.x = rect.mPos.x - normalPos.x;
    normalSize.y = rect.mPos.y - normalPos.y;
    *(u32*)&mFrame.mContentX = *(u32*)&normalSize;
    func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(&mRectData, rect.mSize);

setRect_tail:
    if ((unk278 & 0x10) == 0) {
        func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(&mRectData, rect.mSize);
    }
}

bool CView::attachRenderWork(CWorkThread* pThread) {
    // Dual context-ring enqueue: tag0+WorkID then tag1+thread*.
    // Raw ring manipulation (not CMsgParam<10>) to match retail's
    // stmw r21 / -0x80 frame / stwux slot / retroactive flag+wid.
    volatile struct {
        u32 w0, w1, w2, w3, w4, w5, w6;
        s16 half;
        u8 byte;
        u8 pad;
    } snapFan0;
    volatile struct {
        u32 w0, w1, w2, w3, w4, w5, w6;
        s16 half;
        u8 byte;
        u8 pad;
    } snapTag1;
    u32 sum;
    u32 slot;
    u8* slotPtr;
    u32 prevIdx;
    u32 writeIdx;

    // Enqueue tag 0: attach work ID to unk238
    sum = unk3F0 + mContextRingWriteIndex;
    slot = sum % mContextRingCapacity;
    slotPtr = (u8*)mContextRingBase + slot * 0x24;
    *(u32*)slotPtr = 0; // tag 0
    *(u32*)(slotPtr + 0x4) = snapFan0.w0;
    *(u32*)(slotPtr + 0x8) = snapFan0.w1;
    *(u32*)(slotPtr + 0xC) = snapFan0.w2;
    *(u32*)(slotPtr + 0x10) = snapFan0.w3;
    *(u32*)(slotPtr + 0x14) = snapFan0.w4;
    *(u32*)(slotPtr + 0x18) = snapFan0.w5;
    *(u32*)(slotPtr + 0x1C) = snapFan0.w6;
    *(s16*)(slotPtr + 0x20) = snapFan0.half;
    slotPtr[0x22] = snapFan0.byte;
    slotPtr[0x23] = 0;

    prevIdx = mContextRingWriteIndex;
    writeIdx = mContextRingWriteIndex + 1;
    mContextRingWriteIndex = writeIdx;
    unk3FC = prevIdx;

    sum = unk3F0 + prevIdx;
    slot = sum % mContextRingCapacity;
    slotPtr = (u8*)mContextRingBase + slot * 0x24;
    slotPtr[0x23] = 3;

    sum = unk3F0 + unk3FC;
    slot = sum % mContextRingCapacity;
    slotPtr = (u8*)mContextRingBase + slot * 0x24;
    *(u32*)(slotPtr + 0x4) = pThread->mWorkID;


    // Enqueue tag 1: attach IWorkEvent* to unk258
    sum = unk3F0 + mContextRingWriteIndex;
    slot = sum % mContextRingCapacity;
    slotPtr = (u8*)mContextRingBase + slot * 0x24;
    *(u32*)slotPtr = 1; // tag 1
    *(u32*)(slotPtr + 0x4) = snapTag1.w0;
    *(u32*)(slotPtr + 0x8) = snapTag1.w1;
    *(u32*)(slotPtr + 0xC) = snapTag1.w2;
    *(u32*)(slotPtr + 0x10) = snapTag1.w3;
    *(u32*)(slotPtr + 0x14) = snapTag1.w4;
    *(u32*)(slotPtr + 0x18) = snapTag1.w5;
    *(u32*)(slotPtr + 0x1C) = snapTag1.w6;
    *(s16*)(slotPtr + 0x20) = snapTag1.half;
    slotPtr[0x22] = snapTag1.byte;
    slotPtr[0x23] = 0;

    prevIdx = mContextRingWriteIndex;
    writeIdx = mContextRingWriteIndex + 1;
    mContextRingWriteIndex = writeIdx;
    unk3FC = prevIdx;

    sum = unk3F0 + prevIdx;
    slot = sum % mContextRingCapacity;
    slotPtr = (u8*)mContextRingBase + slot * 0x24;
    slotPtr[0x23] = 3;

    sum = unk3F0 + unk3FC;
    slot = sum % mContextRingCapacity;
    slotPtr = (u8*)mContextRingBase + slot * 0x24;
    *(u32*)(slotPtr + 0x4) = (u32)(WORK_ID)pThread;

    return true;
}

void CView::detachRenderWork(CWorkThread* pThread) {
    mFrame.detachRenderWork(pThread);
}

// us-804405b8: enqueue a tag-1 message carrying `param` onto this view's
// context ring, flag the just-written slot consumed (byte 0x23 = 3), then
// store `param` into the payload word of the previous-index slot -- the same
// three-step tail as CView::attachRenderWork. Note the first modulo is
// computed signed (retail divw); the two later modulos are unsigned (divwu).
bool func_8043DC20__5CViewFUl(CView* view, u32 param) {
    // Uninitialized stack entry: retail only reads its payload fields
    // (wid..unk22) once each from the frame home.
    volatile CMsgParamEntry msg;
    s32 sumSigned;
    s32 capSigned;
    s32 slotIndex;
    u32 byteOff;
    u8* slotPtr;
    u8* ringBase;
    u32 prevIdx;
    u32 writeIdx;
    u32 sum;
    u32 slot;

    capSigned = (s32)view->mContextRingCapacity;
    sumSigned = (s32)view->unk3F0 + (s32)view->mContextRingWriteIndex;
    slotIndex = sumSigned % capSigned;
    byteOff = (u32)slotIndex * 0x24u;
    // Update-form pointer: advancing the pointer itself makes MWCC emit the
    // retail stwux (base+=offset) for the first payload store.
    u8* entry = view->mContextRingBase;
    entry += byteOff;
    *(u32*)entry = 1; // tag 1
    *(u32*)(entry + 0x4) = msg.wid;
    *(u32*)(entry + 0x8) = msg.unk8;
    *(u32*)(entry + 0xC) = msg.unkC;
    *(u32*)(entry + 0x10) = msg.unk10;
    *(u32*)(entry + 0x14) = msg.unk14;
    *(u32*)(entry + 0x18) = msg.unk18;
    *(u32*)(entry + 0x1C) = msg.unk1C;
    *(u16*)(entry + 0x20) = (u16)msg.unk20;
    entry[0x22] = msg.unk22;
    entry[0x23] = 0;

    writeIdx = view->mContextRingWriteIndex + 1;
    prevIdx = writeIdx - 1;
    view->mContextRingWriteIndex = writeIdx;
    view->unk3FC = prevIdx;

    sum = view->unk3F0 + prevIdx;
    slot = sum % view->mContextRingCapacity;
    slotPtr = view->mContextRingBase + slot * 0x24;
    slotPtr[0x23] = 3;

    sum = view->unk3F0 + view->unk3FC;
    slot = sum % view->mContextRingCapacity;
    slotPtr = view->mContextRingBase + slot * 0x24;
    *(u32*)(slotPtr + 0x4) = param;
    return true;
}

// us-804409a8: three bubble passes over the child-view list, ordering views
// whose first attached work resolves to this view's proc root ahead of others,
// then by root priority (unk1E8), then by the view's own unk460.
void func_8043E010__5CViewFv(CView* view) {
    // Everything is ordered relative to the *current* view's proc root.
    CView* current;
    _reslist_node<WORK_ID>* endNode;
    _reslist_node<WORK_ID>* curNode;
    u32 count;

    current = CViewRoot::getCurrent();
    if (current == nullptr) {
        return;
    }

    // Count the work IDs attached to the current view; bail if empty.
    reslist<WORK_ID>* selfList = (reslist<WORK_ID>*)&current->unk238;
    if (selfList->size() == 0) {
        return;
    }

    // Resolve the current view's proc root from its first attached work ID.
    CProc* thisRoot;
    thisRoot = pssGetRoot__5CProcFP5CProc(CProc::convertToProc(
        getWorkThread__9CWorkUtilFUl(selfList->front())));

    bool swapped;
    _reslist_node<CWorkThread*>* cur;
    _reslist_node<CWorkThread*>* next;
    CView* itemCur;
    CView* itemNext;
    _reslist_node<WORK_ID>* s1;
    _reslist_node<WORK_ID>* n1;
    u32 c1;
    _reslist_node<WORK_ID>* s2;
    _reslist_node<WORK_ID>* n2;
    u32 c2;
    CProc* rootCur;
    CProc* rootNext;

    // Pass 1: move views sharing our proc root ahead of foreign-root views.
    do {
        swapped = false;
        cur = ((_reslist_node<CWorkThread*>*)view->mChildren.mStartNodePtr)->mNext;
        while (cur != (_reslist_node<CWorkThread*>*)view->mChildren.mStartNodePtr) {
            next = cur->mNext;
            if (next == (_reslist_node<CWorkThread*>*)view->mChildren.mStartNodePtr) {
                break;
            }
            itemCur = CView::convertToView(cur->mItem);
            itemNext = CView::convertToView(next->mItem);
            {
                c1 = 0;
                s1 = (_reslist_node<WORK_ID>*)itemCur->unk238.mStartNodePtr;
                n1 = s1->mNext;
                while (n1 != s1) {
                    c1++;
                    n1 = n1->mNext;
                }
                if (c1 != 0) {
                    c2 = 0;
                    s2 = (_reslist_node<WORK_ID>*)itemNext->unk238.mStartNodePtr;
                    n2 = s2->mNext;
                    while (n2 != s2) {
                        c2++;
                        n2 = n2->mNext;
                    }
                    if (c2 != 0) {
                        rootCur = pssGetRoot__5CProcFP5CProc(CProc::convertToProc(
                            getWorkThread__9CWorkUtilFUl(s1->mNext->mItem)));
                        rootNext = pssGetRoot__5CProcFP5CProc(CProc::convertToProc(
                            getWorkThread__9CWorkUtilFUl(s2->mNext->mItem)));
                        // Swap when only the earlier view shares our root.
                        if (rootCur != nullptr && rootNext != nullptr &&
                            rootCur == thisRoot && rootNext != thisRoot) {
                            next->mItem = itemCur;
                            swapped = true;
                            cur->mItem = itemNext;
                        }
                    }
                }
            }
            cur = cur->mNext;
        }
    } while (swapped);

    // Pass 2: within our proc root, order by root priority field unk1E8.
    do {
        swapped = false;
        cur = ((_reslist_node<CWorkThread*>*)view->mChildren.mStartNodePtr)->mNext;
        while (cur != (_reslist_node<CWorkThread*>*)view->mChildren.mStartNodePtr) {
            next = cur->mNext;
            if (next == (_reslist_node<CWorkThread*>*)view->mChildren.mStartNodePtr) {
                break;
            }
            itemCur = CView::convertToView(cur->mItem);
            itemNext = CView::convertToView(next->mItem);
            {
                c1 = 0;
                s1 = (_reslist_node<WORK_ID>*)itemCur->unk238.mStartNodePtr;
                n1 = s1->mNext;
                while (n1 != s1) {
                    c1++;
                    n1 = n1->mNext;
                }
                if (c1 != 0) {
                    c2 = 0;
                    s2 = (_reslist_node<WORK_ID>*)itemNext->unk238.mStartNodePtr;
                    n2 = s2->mNext;
                    while (n2 != s2) {
                        c2++;
                        n2 = n2->mNext;
                    }
                    if (c2 != 0) {
                        rootCur = pssGetRoot__5CProcFP5CProc(CProc::convertToProc(
                            getWorkThread__9CWorkUtilFUl(s1->mNext->mItem)));
                        rootNext = pssGetRoot__5CProcFP5CProc(CProc::convertToProc(
                            getWorkThread__9CWorkUtilFUl(s2->mNext->mItem)));
                        // Both under our root: order by root priority.
                        if (rootCur != nullptr && rootNext != nullptr &&
                            rootCur == thisRoot && rootNext == thisRoot &&
                            (s32)rootCur->unk1E8 < (s32)rootNext->unk1E8) {
                            next->mItem = itemCur;
                            swapped = true;
                            cur->mItem = itemNext;
                        }
                    }
                }
            }
            cur = cur->mNext;
        }
    } while (swapped);

    // Pass 3: final ordering by the view's own unk460 value.
    do {
        swapped = false;
        cur = ((_reslist_node<CWorkThread*>*)view->mChildren.mStartNodePtr)->mNext;
        while (cur != (_reslist_node<CWorkThread*>*)view->mChildren.mStartNodePtr) {
            next = cur->mNext;
            if (next == (_reslist_node<CWorkThread*>*)view->mChildren.mStartNodePtr) {
                break;
            }
            itemCur = CView::convertToView(cur->mItem);
            itemNext = CView::convertToView(next->mItem);
            if ((s32)itemCur->unk460 < (s32)itemNext->unk460) {
                next->mItem = itemCur;
                swapped = true;
                cur->mItem = itemNext;
            }
            cur = cur->mNext;
        }
    } while (swapped);
}

// Drain the context ring: classify each pending message and apply side effects.
#if 1
void CView::updateMsg() {
#if 1
    struct CtxSnap {
        u32 w0;
        u32 w1;
        u32 w2;
        u32 w3;
        u32 w4;
        u32 w5;
        u32 w6;
        s16 half;
        u8 byte;
        u8 pad;
    };
    // Three uninit 0x24 snaps (retail -0x150 homes @ 0x48 / 0x28 / 0x08).
    // Keep only these - dual fan-copy volatiles force -0x140 / stmw r17.
    volatile CtxSnap snapFan0;
    volatile CtxSnap snapFan1;
    volatile CtxSnap snapTag1;
#endif
    u32 tag0;
    u32 tag1;
    u32 flag;
    u32 readIdx;
    u32 cap;
    CMsgParamEntry* entry;
    u32 tag;
    _reslist_node<WORK_ID>* listSentinel;
    _reslist_node<WORK_ID>* listNode;
    CWorkThread* parentSnap;
    CWorkThread* parentView;
    CWorkThread* workThread;
    void** childSentinel;
    void** childNode;
    CWorkThread* childThread;
    CView* childView;
    u32 writeIdx;
    u32 prevIdx;
    u32 sumU;
    u32 slotU;
    s32 sumSigned;
    s32 capSigned;
    s32 slotIndex;
    u32 byteOff;
    u8* childRing;
    u8* childSlot;
    u32 workId;

    goto updateMsg_check;

updateMsg_loop:
    readIdx = unk3F0;
    cap = mContextRingCapacity;
    u32 ringIdx = readIdx % cap;
    CMsgParamEntry* contextRing = (CMsgParamEntry*)mContextRingBase;
    entry = &contextRing[ringIdx];
    tag = entry->command;

    // No explicit range guard: tags > 7 match no case and fall through to
    // updateMsg_advance, and MWCC's table-jump lowering emits the retail
    // single cmpli/bc guard on its own (an explicit if duplicates it).
    switch (tag) {
        case 0: {
            // Attach WORK_ID to unk238; fan-out dual ring msgs to child views.
            u32 msgItem = entry->wid;
            {
                _reslist_node<WORK_ID>* sentinel238 = reinterpret_cast<_reslist_node<WORK_ID>*>(unk238.mStartNodePtr);
                _reslist_node<WORK_ID>* node238 = sentinel238->mNext;
                while (node238 != sentinel238) {
                    if (node238->mItem == msgItem) {
                        break;
                    }
                    node238 = node238->mNext;
                }
                if (node238 == sentinel238) {
                reinterpret_cast<reslist<u32>*>(&unk238)->push_back(msgItem);
            }

            parentSnap = mParent;
            if (getInstance__9CViewRootFv() == parentSnap) {
                parentView = nullptr;
            } else {
                parentView = mParent;
                if (parentView == nullptr) {
                    parentView = nullptr;
                } else if ((s32)parentView->mType < 0x30) {
                    parentView = nullptr;
                } else if ((s32)parentView->mType >= 0x35) {
                    parentView = nullptr;
                }
            }

            if (parentView == nullptr) {
                goto updateMsg_advance;
            }

            workThread = getWorkThread__9CWorkUtilFUl(entry->wid);
            childSentinel = (void**)mChildren.mStartNodePtr;
            childNode = (void**)childSentinel[0];
            while (childNode != childSentinel) {
                childThread = (CWorkThread*)childNode[2];
                childView = CView::convertToView(childThread);
                if (childView != nullptr) {
#if 1
                    CMsgParam<10>& childMessages =
                        *reinterpret_cast<CMsgParam<10>*>(&childView->mContextMsgVtable);
                    childMessages.enqueue(0);
                    childMessages.last().unk23 = 3;
                    childMessages.last().wid = workThread->mWorkID;
                    childMessages.enqueue(1);
                    childMessages.last().unk23 = 3;
                    childMessages.last().wid = (WORK_ID)workThread;
#else
                    tag0 = 0;
                    flag = 3;
                    tag1 = 1;
                    workId = workThread->mWorkID;

                    sumSigned = (s32)childView->unk3F0 + (s32)childView->mContextRingWriteIndex;
                    capSigned = (s32)childView->mContextRingCapacity;
                    slotIndex = sumSigned / capSigned;
                    byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
                    childRing = (u8*)childView->mContextRingBase;
                    childSlot = childRing + byteOff;
                    *(u32*)childSlot = tag0;
                    *(u32*)(childSlot + 0x4) = snapFan0.w0;
                    *(u32*)(childSlot + 0x8) = snapFan0.w1;
                    *(u32*)(childSlot + 0xC) = snapFan0.w2;
                    *(u32*)(childSlot + 0x10) = snapFan0.w3;
                    *(u32*)(childSlot + 0x14) = snapFan0.w4;
                    *(u32*)(childSlot + 0x18) = snapFan0.w5;
                    *(u32*)(childSlot + 0x1C) = snapFan0.w6;
                    *(s16*)(childSlot + 0x20) = snapFan0.half;
                    childSlot[0x22] = snapFan0.byte;
                    childSlot[0x23] = (u8)tag0;

                    writeIdx = childView->mContextRingWriteIndex + 1;
                    prevIdx = writeIdx - 1;
                    childView->mContextRingWriteIndex = writeIdx;
                    childView->unk3FC = prevIdx;

                    sumU = childView->unk3F0 + prevIdx;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    childSlot[0x23] = (u8)flag;

                    sumU = childView->unk3F0 + childView->unk3FC;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    *(u32*)(childSlot + 0x4) = workId;

                    sumSigned = (s32)childView->unk3F0 + (s32)childView->mContextRingWriteIndex;
                    capSigned = (s32)childView->mContextRingCapacity;
                    slotIndex = sumSigned / capSigned;
                    byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
                    childRing = (u8*)childView->mContextRingBase;
                    childSlot = childRing + byteOff;
                    *(u32*)childSlot = tag1;
                    *(u32*)(childSlot + 0x4) = snapFan1.w0;
                    *(u32*)(childSlot + 0x8) = snapFan1.w1;
                    *(u32*)(childSlot + 0xC) = snapFan1.w2;
                    *(u32*)(childSlot + 0x10) = snapFan1.w3;
                    *(u32*)(childSlot + 0x14) = snapFan1.w4;
                    *(u32*)(childSlot + 0x18) = snapFan1.w5;
                    *(u32*)(childSlot + 0x1C) = snapFan1.w6;
                    *(s16*)(childSlot + 0x20) = snapFan1.half;
                    childSlot[0x22] = snapFan1.byte;
                    childSlot[0x23] = (u8)tag0;

                    writeIdx = childView->mContextRingWriteIndex + 1;
                    childView->mContextRingWriteIndex = writeIdx;
                    prevIdx = writeIdx - 1;
                    childView->unk3FC = prevIdx;

                    sumU = childView->unk3F0 + prevIdx;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    childSlot[0x23] = (u8)flag;

                    sumU = childView->unk3F0 + childView->unk3FC;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    *(u32*)(childSlot + 0x4) = (u32)workThread;
#endif
                }
                childNode = (void**)childNode[0];
            }
            break;
        }
        }
        case 1: {
            // Attach IWorkEvent* to unk258; fan-out tag-1 msgs to child views.
            u32 msgItem = entry->wid;
            {
                _reslist_node<void*>* sentinel258 = reinterpret_cast<_reslist_node<void*>*>(unk258.mStartNodePtr);
                _reslist_node<void*>* node258 = sentinel258->mNext;
                while (node258 != sentinel258) {
                    if ((u32)(uintptr_t)node258->mItem == msgItem) {
                        break;
                    }
                    node258 = node258->mNext;
                }
                if (node258 == sentinel258) {
                    reinterpret_cast<reslist<void*>*>(&unk258)->push_back(
                        *reinterpret_cast<void**>(&msgItem));
                }
            }

            parentSnap = mParent;
            if (getInstance__9CViewRootFv() == parentSnap) {
                parentView = nullptr;
            } else {
                parentView = mParent;
                if (parentView == nullptr) {
                    parentView = nullptr;
                } else if ((s32)parentView->mType < 0x30) {
                    parentView = nullptr;
                } else if ((s32)parentView->mType >= 0x35) {
                    parentView = nullptr;
                }
            }

            if (parentView == nullptr) {
                goto updateMsg_advance;
            }

            msgItem = entry->wid;
            childSentinel = (void**)mChildren.mStartNodePtr;
            childNode = (void**)childSentinel[0];
            while (childNode != childSentinel) {
                childThread = (CWorkThread*)childNode[2];
                childView = CView::convertToView(childThread);
                if (childView != nullptr) {
#if 1
                    CMsgParam<10>& childMessages =
                        *reinterpret_cast<CMsgParam<10>*>(&childView->mContextMsgVtable);
                    childMessages.enqueue(1);
                    childMessages.last().unk23 = 3;
                    childMessages.last().wid = msgItem;
#else
                    tag1 = 1;
                    flag = 3;
                    tag0 = 0;

                    sumSigned = (s32)childView->unk3F0 + (s32)childView->mContextRingWriteIndex;
                    capSigned = (s32)childView->mContextRingCapacity;
                    slotIndex = sumSigned / capSigned;
                    byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
                    childRing = (u8*)childView->mContextRingBase;
                    childSlot = childRing + byteOff;
                    *(u32*)childSlot = tag1;
                    *(u32*)(childSlot + 0x4) = snapTag1.w0;
                    *(u32*)(childSlot + 0x8) = snapTag1.w1;
                    *(u32*)(childSlot + 0xC) = snapTag1.w2;
                    *(u32*)(childSlot + 0x10) = snapTag1.w3;
                    *(u32*)(childSlot + 0x14) = snapTag1.w4;
                    *(u32*)(childSlot + 0x18) = snapTag1.w5;
                    *(u32*)(childSlot + 0x1C) = snapTag1.w6;
                    *(s16*)(childSlot + 0x20) = snapTag1.half;
                    childSlot[0x22] = snapTag1.byte;
                    childSlot[0x23] = (u8)tag0;

                    writeIdx = childView->mContextRingWriteIndex + 1;
                    prevIdx = writeIdx - 1;
                    childView->mContextRingWriteIndex = writeIdx;
                    childView->unk3FC = prevIdx;

                    sumU = childView->unk3F0 + prevIdx;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    childSlot[0x23] = (u8)flag;

                    sumU = childView->unk3F0 + childView->unk3FC;
                    slotU = sumU / childView->mContextRingCapacity;
                    childSlot = (u8*)childView->mContextRingBase +
                        (sumU - slotU * childView->mContextRingCapacity) * 0x24u;
                    *(u32*)(childSlot + 0x4) = msgItem;
#endif
                }
                childNode = (void**)childNode[0];
            }
            break;
        }
        case 2: {
            // Remove WORK_ID from unk238.
            u32 msgItem = entry->wid;
            {
                _reslist_node<WORK_ID>* sentinel238 =
                    reinterpret_cast<_reslist_node<WORK_ID>*>(unk238.mStartNodePtr);
                _reslist_node<WORK_ID>* node238 = sentinel238->mNext;
                while (node238 != sentinel238) {
                    _reslist_node<WORK_ID>* next238 = node238->mNext;
                    if (node238->mItem == msgItem) {
                        node238->mPrev->mNext = next238;
                        next238->mPrev = node238->mPrev;
                        node238->mNext = nullptr;
                    }
                    node238 = next238;
                }
            }
            break;
        }
        case 3:
            unk278 |= 0x3;
            break;
        case 4:
            unk278 |= 0x4;
            break;
        case 5:
            unk278 &= ~0x4;
            break;
        case 6:
            CViewRoot::setCurrent(this);
            break;
        case 7:
            if (entry->wid == 0) {
                unk278 |= 0x20;
            } else {
                unk278 &= ~0x20;
            }
            break;
        }

    updateMsg_advance:
        readIdx = unk3F0;
        cap = mContextRingCapacity;
        mContextRingWriteIndex -= 1;
        unk3F0 = (readIdx + 1) - ((readIdx + 1) / cap) * cap;

    updateMsg_check:
        if (mContextRingWriteIndex != 0) {
            goto updateMsg_loop;
        }
}

void CView::wkUpdate() {
    u32 hasView1;
    u32 hasView2;

    if (unk45C != nullptr && !isActive__11CSplitFrameFv(unk45C)) {
        if (unk45C != nullptr) {
            delete static_cast<CSplitFrame*>(unk45C);
            unk45C = nullptr;
        }
    }

    updateMsg();

    {
        u32 hasView1a;
        u32 hasView2a;

        hasView2a = 0;
        hasView1a = 0;
        if (unk45C == nullptr) {
            goto wkUpdate_no_apply;
        }
        if (getView1__11CSplitFrameFv(unk45C) == nullptr) {
            goto wkUpdate_no_apply;
        }
        hasView1a = 1;

    wkUpdate_no_apply:
        if (hasView1a == 0) {
            goto wkUpdate_apply_check;
        }
        if (getView2__11CSplitFrameFv(unk45C) == nullptr) {
            goto wkUpdate_apply_check;
        }
        hasView2a = 1;

    wkUpdate_apply_check:
        if (hasView2a != 0) {
            apply__11CSplitFrameFv(unk45C);
        }
    }

    if (mChildren.empty() && unk238.empty()) {
        hasView2 = 0;
        hasView1 = 0;
        if (unk45C == nullptr) {
            goto wkUpdate_no_setevent_check;
        }
        if (getView1__11CSplitFrameFv(unk45C) == nullptr) {
            goto wkUpdate_no_setevent_check;
        }
        hasView1 = 1;

    wkUpdate_no_setevent_check:
        if (hasView1 == 0) {
            goto wkUpdate_setevent;
        }
        if (getView2__11CSplitFrameFv(unk45C) == nullptr) {
            goto wkUpdate_setevent;
        }
        hasView2 = 1;

    wkUpdate_setevent:
        if (hasView2 == 0) {
            wkSetEvent(EVT_NONE);
        }
    }
}

// TU-local clone of reslist::size() with retail's temp order (end/cur node
// temps before length) so the inlined expansion allocates count into the
// higher register like the original build.
static u32 ResListWorkSize(const reslist<WORK_ID>* list) {
    _reslist_node<WORK_ID>* endNode = list->mStartNodePtr;
    u32 length = 0;
    _reslist_node<WORK_ID>* curNode = endNode->mNext;
    while (curNode != endNode) {
        curNode = curNode->mNext;
        length++;
    }
    return length;
}

// us-8044237c: standby logout gate. Logout only proceeds once every child
// thread is gone, no attached work remains, and any split frame has already
// been torn down. A stalled logout is reported through the watchdog.
// (Defined as a retail-named free function: CWorkThread's virtual slot is
// shared and the member cannot be declared in the read-only shared header.)
bool wkStandbyLogout__5CViewFv(CView* self) {
    // Logout gate: every child thread gone, no attached work IDs left, and
    // any split frame lacking both panes. Stalled logouts hit the watchdog.
    // 30000 frames (~30s) until the check-timeout message fires.
    self->CWorkThread::wkCheckTimeout(
        30000,
        self->mChildren.empty() &&
            ResListWorkSize(reinterpret_cast<const reslist<WORK_ID>*>(&self->unk238)) == 0 &&
            !(self->unk45C != nullptr &&
              getView1__11CSplitFrameFv(self->unk45C) != nullptr &&
              getView2__11CSplitFrameFv(self->unk45C) != nullptr),
        lbl_eu_80522630);

    if (self->unk45C != nullptr && !isActive__11CSplitFrameFv(self->unk45C)) {
        if (self->unk45C != nullptr) {
            delete static_cast<CSplitFrame*>(self->unk45C);
            self->unk45C = nullptr;
        }
    }

    if (self->mChildren.empty() && self->unk45C == nullptr &&
        ResListWorkSize(reinterpret_cast<const reslist<WORK_ID>*>(&self->unk238)) == 0) {
        CViewRoot::invalidCurrent(self);
        return wkStandbyLogout__11CWorkThreadFv(self);
    }
    return false;
}

void CView::renderView() {
    // Retail r28: 0, then fs pssGetRoot, then 0/1.
    u32 crossRootFlag;
    CView* fullScreenView;
    CProc* thisRoot;
    s16 savedSizeX;
    s16 savedSizeY;
    _reslist_node<WORK_ID>* listNode;
    u32 listLen;
    _reslist_node<WORK_ID>* listStart;
    CView* parentView;
    CWorkThread* parentThread;
    u32 invalidRect;
    u32 hasView1;
    u32 hasView2;
    u32 colorUpdateOff;
    u32 flags278;
    // Decl order first = higher addr. Share scratch homes so the frame stays
    // -0x180 while preserving the retail rectangle lifetimes.
    volatile ml::CRect16 viewRect;
    ml::CRect16 accumRect;
    ml::CRect16 scissorOut;
    ml::CRect16 insetRect;
    ml::CRect16 clearRect;
    ml::CRect16 scissorSrc;
    volatile ml::CRect16 parentAccumRect;
    ml::CRect16 home34;
    ml::CRect16 home28;
    ml::CRect16 home30; // early / y-loop / clear
    ml::CRect16 home18; // also home20
    ml::CRect16 home14; // also home1C
    ml::CRect16 home08;
    s16 yAccum;
    void** attachNode;
    CWorkThread* attachWork;
    u32 msgQualified;
    u32 stateReady;
    u32 shouldRender;
    _reslist_node<CWorkThread*>* childNode;
    CView* childView;
    CView* frameParent;
    s32 accumX;
    s32 accumY;
    s32 parentAccumX;
    s32 parentAccumY;

    if ((unk278 & 0x40) != 0) {
        return;
    }

    func_8043E010__5CViewFv(this);

    crossRootFlag = 0;
    fullScreenView = getFullScreenView__9CViewRootFv();

    if ((fullScreenView->unk278 & 0x2) != 0) {
        goto renderView_after_cross;
    }
    if (fullScreenView == this) {
        goto renderView_after_cross;
    }

    listStart = reinterpret_cast<_reslist_node<WORK_ID>*>(unk238.mStartNodePtr);
    listLen = 0;
    listNode = listStart->mNext;
    goto renderView_count_this_check;
renderView_count_this_body:
    listNode = listNode->mNext;
    listLen = listLen + 1;
renderView_count_this_check:
    if (listNode != listStart) {
        goto renderView_count_this_body;
    }
    if (listLen == 0) {
        goto renderView_after_cross;
    }

    listStart = reinterpret_cast<_reslist_node<WORK_ID>*>(fullScreenView->unk238.mStartNodePtr);
    listLen = 0;
    listNode = listStart->mNext;
    goto renderView_count_fs_check;
renderView_count_fs_body:
    listNode = listNode->mNext;
    listLen = listLen + 1;
renderView_count_fs_check:
    if (listNode != listStart) {
        goto renderView_count_fs_body;
    }
    if (listLen == 0) {
        goto renderView_after_cross;
    }

    crossRootFlag = (u32)pssGetRoot__5CProcFP5CProc(CProc::convertToProc(
        getWorkThread__9CWorkUtilFUl(listStart->mNext->mItem)));

    listStart = reinterpret_cast<_reslist_node<WORK_ID>*>(unk238.mStartNodePtr);
    thisRoot = pssGetRoot__5CProcFP5CProc(CProc::convertToProc(
        getWorkThread__9CWorkUtilFUl(listStart->mNext->mItem)));

    {
        CProc* fullScreenRoot = (CProc*)crossRootFlag;
        crossRootFlag = 0;
        if (fullScreenRoot == thisRoot) {
            goto renderView_after_cross;
        }
        if (thisRoot == nullptr) {
            goto renderView_after_cross;
        }
        if ((s32)unk460 < 7) {
            goto renderView_after_cross;
        }
        crossRootFlag = 1;
    }

renderView_after_cross:
    if (fullScreenView != this) {
        goto renderView_after_cache_color;
    }

    {
        ml::CCol4 cacheColor;
        // Retail: lfs scale->f4, z, y, x; fmuls b; lfs w; fmuls g/r; stfs a,r,g,b
        float scale = mAlpha;
        float b = unk444.z * scale;
        float a = unk444.w;
        float g = unk444.y * scale;
        float r = unk444.x * scale;
        cacheColor.a = a;
        cacheColor.r = r;
        cacheColor.g = g;
        cacheColor.b = b;
        func_8044A578__8CGXCacheFv(CDeviceGX::getCacheInstance(), &cacheColor, 0);
    }

renderView_after_cache_color:
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home30,
                                                              &mFrame);
    savedSizeX = mRectData.mViewSize.x;
    savedSizeY = mRectData.mViewSize.y;
    yAccum = mFrame.mContentY;
    parentThread = mParent;

    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_y_accum_check;
    }
    parentView = CView::convertToView(mParent);
    goto renderView_y_accum_check;

renderView_y_accum_body:
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home30,
                                                              &parentView->mFrame);
    yAccum = (s16)(yAccum + (s16)(parentView->mFrame.mContentY + home30.mPos.y));
    parentThread = parentView->mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_y_accum_check;
    }
    parentView = CView::convertToView(parentView->mParent);

renderView_y_accum_check:
    if (parentView != nullptr) {
        goto renderView_y_accum_body;
    }
    *reinterpret_cast<volatile s16*>(&home08.mPos.y) = yAccum;

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home34,
                                                              &mFrame);
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home28,
                                                              &mFrame);

    invalidRect = 0;
    viewRect.mPos.y = (s16)(mFrame.mContentY + home28.mPos.y);
    viewRect.mPos.x = (s16)(mFrame.mContentX + home28.mPos.x);
    viewRect.mSize.x = mRectData.mViewSize.x;
    viewRect.mSize.y = mRectData.mViewSize.y;
    if (mRectData.mViewSize.x <= 0) {
        goto renderView_mark_invalid;
    }
    if (mRectData.mViewSize.y > 0) {
        goto renderView_after_size_gate1;
    }
renderView_mark_invalid:
    invalidRect = 1;
renderView_after_size_gate1:
    if (invalidRect != 0) {
        goto renderView_tail;
    }

    invalidRect = 0;
    if (savedSizeX <= 0) {
        goto renderView_mark_invalid2;
    }
    if (savedSizeY > 0) {
        goto renderView_after_size_gate2;
    }
renderView_mark_invalid2:
    invalidRect = 1;
renderView_after_size_gate2:
    if (invalidRect != 0) {
        goto renderView_tail;
    }

    {
        u32 maxSizeBad;

        maxSizeBad = 0;
        if (mRectData.mBoundsSize.x > 0) {
            goto renderView_after_max_gate;
        }
        if (mRectData.mBoundsSize.y > 0) {
            goto renderView_after_max_gate;
        }
        maxSizeBad = 1;
    renderView_after_max_gate:
        if (maxSizeBad != 0) {
            goto renderView_tail;
        }
    }

    if (crossRootFlag != 0) {
        goto renderView_scissor_setup;
    }

    {
        u32 hasView2a;
        u32 hasView1a;

        hasView2a = 0;
        hasView1a = 0;
        if (unk45C == nullptr) {
            goto renderView_clear_no_view1;
        }
        if (getView1__11CSplitFrameFv(unk45C) == nullptr) {
            goto renderView_clear_no_view1;
        }
        hasView1a = 1;
    renderView_clear_no_view1:
        if (hasView1a == 0) {
            goto renderView_clear_split_check;
        }
        if (getView2__11CSplitFrameFv(unk45C) == nullptr) {
            goto renderView_clear_split_check;
        }
        hasView2a = 1;
    renderView_clear_split_check:
        if (hasView2a != 0) {
            goto renderView_scissor_setup;
        }

        flags278 = unk278;
        if ((flags278 & 1) != 0) {
            goto renderView_scissor_setup;
        }

        colorUpdateOff = (flags278 >> 2) & 1;
        {
            CDrawGX draw;
            draw.func_80456570(0);
            draw.func_8045657C(1);
            if (colorUpdateOff != 0) {
                GXSetColorUpdate(GX_FALSE);
                goto renderView_clear_begin;
            }
            {
                ml::CCol4 clearColor;
                clearColor = *reinterpret_cast<ml::CCol4*>(&unk444);
                clearColor.a *= mAlpha;
                draw.setCol(clearColor);
            }
        renderView_clear_begin:
            draw.setGXCacheId(mGXCacheId);
            draw.begin(PRIM_QUADS, 1);
            getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home30,
                                                                      &mFrame);
            clearRect.mPos.y = (s16)(mFrame.mContentY + home30.mPos.y);
            clearRect.mPos.x = (s16)(mFrame.mContentX + home30.mPos.x);
            clearRect.mSize.x = mRectData.mViewSize.x;
            clearRect.mSize.y = mRectData.mViewSize.y;
            draw.add(clearRect);
            draw.end();
            if (colorUpdateOff != 0) {
                GXSetColorUpdate(GX_TRUE);
            }
        }
    }

renderView_scissor_setup:
    if (crossRootFlag != 0) {
        goto renderView_works_or_children;
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home18, &mFrame);
    accumRect.mPos.y = (s16)(mFrame.mContentY + home18.mPos.y);
    accumRect.mPos.x = (s16)(mFrame.mContentX + home18.mPos.x);
    accumRect.mSize.x = mRectData.mViewSize.x;
    accumRect.mSize.y = mRectData.mViewSize.y;

    accumX = mFrame.mContentX;
    accumY = mFrame.mContentY;

    parentThread = mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_xy_accum_check;
    }
    parentView = CView::convertToView(mParent);
    goto renderView_xy_accum_check;

renderView_xy_accum_body:
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home14,
                                                              &parentView->mFrame);
    accumX += (s16)(parentView->mFrame.mContentX + home14.mPos.x);
    accumY += (s16)(parentView->mFrame.mContentY + home14.mPos.y);
    parentThread = parentView->mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_xy_accum_check;
    }
    parentView = CView::convertToView(parentView->mParent);

renderView_xy_accum_check:
    if (parentView != nullptr) {
        goto renderView_xy_accum_body;
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home18, &mFrame);
    accumRect.mPos.x = (s16)(accumX + home18.mPos.x);
    accumRect.mPos.y = (s16)(accumY + home18.mPos.y);

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home08, &mFrame);
    parentAccumX = mFrame.mContentX;
    parentAccumY = mFrame.mContentY;
    parentAccumRect.mPos.x = (s16)(mFrame.mContentX + home08.mPos.x);
    parentAccumRect.mPos.y = (s16)(mFrame.mContentY + home08.mPos.y);
    parentAccumRect.mSize.x = mRectData.mViewSize.x;
    parentAccumRect.mSize.y = mRectData.mViewSize.y;

    parentThread = mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_parent_accum_check;
    }
    parentView = CView::convertToView(mParent);
    goto renderView_parent_accum_check;

renderView_parent_accum_body:
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home14,
                                                              &parentView->mFrame);
    parentAccumX += (s16)(parentView->mFrame.mContentX + home14.mPos.x);
    parentAccumY += (s16)(parentView->mFrame.mContentY + home14.mPos.y);
    parentThread = parentView->mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        parentView = nullptr;
        goto renderView_parent_accum_check;
    }
    parentView = CView::convertToView(parentView->mParent);

renderView_parent_accum_check:
    if (parentView != nullptr) {
        goto renderView_parent_accum_body;
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home08, &mFrame);
    parentAccumRect.mPos.x = (s16)(parentAccumX + home08.mPos.x);
    parentAccumRect.mPos.y = (s16)(parentAccumY + home08.mPos.y);

    parentThread = mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        frameParent = nullptr;
        goto renderView_frame_parent_ready;
    }
    frameParent = CView::convertToView(mParent);

renderView_frame_parent_ready:
    if (frameParent == nullptr) {
        goto renderView_local_scissor;
    }

    {
        u32 hasView2b;
        u32 hasView1b;

        hasView2b = 0;
        hasView1b = 0;
        if (frameParent->unk45C == nullptr) {
            goto renderView_fp_no_view1;
        }
        if (getView1__11CSplitFrameFv(frameParent->unk45C) == nullptr) {
            goto renderView_fp_no_view1;
        }
        hasView1b = 1;
    renderView_fp_no_view1:
        if (hasView1b == 0) {
            goto renderView_fp_split_check;
        }
        if (getView2__11CSplitFrameFv(frameParent->unk45C) == nullptr) {
            goto renderView_fp_split_check;
        }
        hasView2b = 1;
    renderView_fp_split_check:
        if (hasView2b != 0) {
            goto renderView_parent_split_scissor;
        }
    }

renderView_local_scissor:
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&home14, &mFrame);
    scissorSrc.mPos.y = (s16)(mFrame.mContentY + home14.mPos.y);
    scissorSrc.mPos.x = (s16)(mFrame.mContentX + home14.mPos.x);
    scissorSrc.mSize.x = mRectData.mViewSize.x;
    scissorSrc.mSize.y = mRectData.mViewSize.y;
    goto renderView_scissor_ready;

renderView_parent_split_scissor:
    {
        u32 hasView2c;
        u32 hasView1c;
        CView* splitView1;

        hasView2c = 0;
        hasView1c = 0;
        if (frameParent->unk45C == nullptr) {
            goto renderView_fp2_no_view1;
        }
        if (getView1__11CSplitFrameFv(frameParent->unk45C) == nullptr) {
            goto renderView_fp2_no_view1;
        }
        hasView1c = 1;
    renderView_fp2_no_view1:
        if (hasView1c == 0) {
            goto renderView_fp2_split_check;
        }
        if (getView2__11CSplitFrameFv(frameParent->unk45C) == nullptr) {
            goto renderView_fp2_split_check;
        }
        hasView2c = 1;
    renderView_fp2_split_check:
        if (hasView2c == 0) {
            splitView1 = nullptr;
            goto renderView_split_which;
        }
        splitView1 = getView1__11CSplitFrameFv(frameParent->unk45C);
    renderView_split_which:
        if (splitView1 == this) {
            getScissorRect1__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(
                &scissorSrc, frameParent->unk45C);
            goto renderView_scissor_ready;
        }
        getScissorRect2__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(
            &scissorSrc, frameParent->unk45C);
        goto renderView_scissor_ready;
    }

renderView_scissor_ready:
    scissorOut.mPos.x = parentAccumRect.mPos.x;
    scissorOut.mPos.y = parentAccumRect.mPos.y;
    scissorOut.mSize.x = scissorSrc.mSize.x;
    scissorOut.mSize.y = scissorSrc.mSize.y;
    insetRect.mPos.x = mRectData.mScrollX;
    insetRect.mPos.y = mRectData.mScrollY;
    insetRect.mSize.x = mRectData.mBoundsSize.x;
    insetRect.mSize.y = mRectData.mBoundsSize.y;
    func_80442B54__9CViewRootFPvPv(&accumRect, &scissorOut, &insetRect);

renderView_works_or_children:
    if (crossRootFlag != 0) {
        goto renderView_children;
    }

    hasView2 = 0;
    hasView1 = 0;
    if (unk45C == nullptr) {
        goto renderView_wk_no_view1;
    }
    if (getView1__11CSplitFrameFv(unk45C) == nullptr) {
        goto renderView_wk_no_view1;
    }
    hasView1 = 1;
renderView_wk_no_view1:
    if (hasView1 == 0) {
        goto renderView_wk_split_check;
    }
    if (getView2__11CSplitFrameFv(unk45C) == nullptr) {
        goto renderView_wk_split_check;
    }
    hasView2 = 1;
renderView_wk_split_check:
    if (hasView2 != 0) {
        goto renderView_children;
    }

    lbl_eu_806655C8.ptr = this;

    if (isRunning()) {
        wkRender();
    }
#if 0
    if ((mFlags & THREAD_FLAG_EXCEPTION) != 0) {
        msgQualified = 1;
        goto renderView_self_qualified;
    }

    {
        u32 msgIndex;
        u32 msgFront;
        u32 msgModulus;
        u32 msgSum;
        u32 msgRemainder;
        u32 msgEntryOffset;
        CMsgParamEntry* msgArray;
        u32 msgCount;

        msgCount = *(u32*)((u8*)this + 0x1AC);

        for (msgIndex = 0; msgIndex < msgCount; msgIndex++) {
            msgFront = *(u32*)((u8*)this + 0x1A8);
            msgModulus = *(u32*)((u8*)this + 0x1B0);
            msgSum = msgFront + msgIndex;
            msgArray = *(CMsgParamEntry**)((u8*)this + 0x1A4);
            msgRemainder = msgSum / msgModulus;
            msgRemainder = msgRemainder * msgModulus;
            msgRemainder = msgSum - msgRemainder;
            msgEntryOffset = msgRemainder * 0x24;

            if (*(u32*)((u8*)msgArray + msgEntryOffset) != 2) {
                continue;
            }

            goto renderView_self_msg_eval;
        }

        msgIndex = -1;

    renderView_self_msg_eval:
        msgQualified = (msgIndex >> 31) ^ 1;
    }

renderView_self_qualified:
    shouldRender = 0;
    if (msgQualified != 0) {
        goto renderView_self_state_end;
    }
    stateReady = 1;
    if (mState == THREAD_STATE_LOGIN) {
        goto renderView_self_state_ready;
    }
    if (mState == THREAD_STATE_RUN) {
        goto renderView_self_state_ready;
    }
    stateReady = 0;
renderView_self_state_ready:
    if (stateReady == 0) {
        goto renderView_self_state_end;
    }
    shouldRender = 1;
renderView_self_state_end:
    if (shouldRender != 0) {
        wkRender();
    }
#endif

    attachNode = (void**)unk238.mStartNodePtr;
    attachNode = (void**)*attachNode;
    goto renderView_attach_wk_check;

renderView_attach_wk_body:
    attachWork = getWorkThread__9CWorkUtilFUl(reinterpret_cast<_reslist_node<WORK_ID>*>(attachNode)->mItem);
    if (attachWork == nullptr) {
        goto renderView_attach_wk_next;
    }
    lbl_eu_806655C8.ptr = this;
    if (attachWork == this) {
        goto renderView_attach_wk_next;
    }

    if (attachWork->isRunning()) {
        goto renderView_attach_do_render_inline;
    }
    if (!attachWork->isException()) {
        goto renderView_attach_wk_next;
    }
renderView_attach_do_render_inline:
    attachWork->wkRender();
#if 0
    {
        u32 exceptFlag;

        exceptFlag = (attachWork->mFlags & THREAD_FLAG_EXCEPTION) != 0 ? 1 : 0;
        if (exceptFlag != 0) {
            msgQualified = 1;
            goto renderView_attach_qualified;
        }

        {
            u32 msgIndex;
            u32 msgFront;
            u32 msgModulus;
            u32 msgSum;
            u32 msgRemainder;
            u32 msgEntryOffset;
            CMsgParamEntry* msgArray;
            u32 msgCount;

            msgCount = *(u32*)((u8*)attachWork + 0x1AC);

            for (msgIndex = 0; msgIndex < msgCount; msgIndex++) {
                msgFront = *(u32*)((u8*)attachWork + 0x1A8);
                msgModulus = *(u32*)((u8*)attachWork + 0x1B0);
                msgSum = msgFront + msgIndex;
                msgArray = *(CMsgParamEntry**)((u8*)attachWork + 0x1A4);
                msgRemainder = msgSum / msgModulus;
                msgRemainder = msgRemainder * msgModulus;
                msgRemainder = msgSum - msgRemainder;
                msgEntryOffset = msgRemainder * 0x24;

                if (*(u32*)((u8*)msgArray + msgEntryOffset) != 2) {
                    continue;
                }

                goto renderView_attach_msg_eval;
            }

            msgIndex = -1;

        renderView_attach_msg_eval:
            msgQualified = (msgIndex >> 31) ^ 1;
        }

    renderView_attach_qualified:
        shouldRender = 0;
        if (msgQualified != 0) {
            goto renderView_attach_state_end;
        }
        stateReady = 1;
        if (attachWork->mState == THREAD_STATE_LOGIN) {
            goto renderView_attach_state_ready;
        }
        if (attachWork->mState == THREAD_STATE_RUN) {
            goto renderView_attach_state_ready;
        }
        stateReady = 0;
    renderView_attach_state_ready:
        if (stateReady == 0) {
            goto renderView_attach_state_end;
        }
        shouldRender = 1;
    renderView_attach_state_end:
        if (shouldRender != 0) {
            goto renderView_attach_do_render;
        }

        if (exceptFlag != 0) {
            msgQualified = 1;
            goto renderView_attach_ex_done;
        }

        {
            u32 msgIndex;
            u32 msgFront;
            u32 msgModulus;
            u32 msgSum;
            u32 msgRemainder;
            u32 msgEntryOffset;
            CMsgParamEntry* msgArray;
            u32 msgCount;

            msgCount = *(u32*)((u8*)attachWork + 0x1AC);

            for (msgIndex = 0; msgIndex < msgCount; msgIndex++) {
                msgFront = *(u32*)((u8*)attachWork + 0x1A8);
                msgModulus = *(u32*)((u8*)attachWork + 0x1B0);
                msgSum = msgFront + msgIndex;
                msgArray = *(CMsgParamEntry**)((u8*)attachWork + 0x1A4);
                msgRemainder = msgSum / msgModulus;
                msgRemainder = msgRemainder * msgModulus;
                msgRemainder = msgSum - msgRemainder;
                msgEntryOffset = msgRemainder * 0x24;

                if (*(u32*)((u8*)msgArray + msgEntryOffset) != 2) {
                    continue;
                }

                goto renderView_attach_ex_msg_eval;
            }

            msgIndex = -1;

        renderView_attach_ex_msg_eval:
            msgQualified = (msgIndex >> 31) ^ 1;
        }

    renderView_attach_ex_done:
        if (msgQualified == 0) {
            goto renderView_attach_wk_next;
        }

    renderView_attach_do_render:
        attachWork->wkRender();
    }
#endif

renderView_attach_wk_next:
    attachNode = (void**)*attachNode;

renderView_attach_wk_check:
    if (attachNode != (void**)unk238.mStartNodePtr) {
        goto renderView_attach_wk_body;
    }

    lbl_eu_806655C8.ptr = nullptr;
    func_8044BE38__8CGXCacheFv(CDeviceGX::getCacheInstance());
    func_80442DA8__9CViewRootFv();
    fontFlush__10CFontLayerFi(static_cast<CFontLayer*>(this), 1);

renderView_children:
    childNode = reinterpret_cast<_reslist_node<CWorkThread*>*>(mChildren.mStartNodePtr);
    childNode = reinterpret_cast<_reslist_node<CWorkThread*>*>(childNode->mNext);
    goto renderView_child_check;

renderView_child_body:
    childView = CView::convertToView(childNode->mItem);
    childView->renderView();
    childNode = childNode->mNext;

renderView_child_check:
    if (childNode != reinterpret_cast<_reslist_node<CWorkThread*>*>(mChildren.mStartNodePtr)) {
        goto renderView_child_body;
    }

    attachNode = (void**)unk238.mStartNodePtr;
    attachNode = (void**)*attachNode;
    goto renderView_attach_after_check;

renderView_attach_after_body:
    attachWork = getWorkThread__9CWorkUtilFUl(reinterpret_cast<_reslist_node<WORK_ID>*>(attachNode)->mItem);
    if (attachWork == nullptr) {
        goto renderView_attach_after_next;
    }
    lbl_eu_806655C8.ptr = this;
    if (attachWork == this) {
        goto renderView_attach_after_next;
    }

    if (attachWork->isRunning()) {
        attachWork->wkRenderAfter();
    }
#if 0
    if ((attachWork->mFlags & THREAD_FLAG_EXCEPTION) != 0) {
        msgQualified = 1;
        goto renderView_after_qualified;
    }

    {
        u32 msgIndex;
        u32 msgFront;
        u32 msgModulus;
        u32 msgSum;
        u32 msgRemainder;
        u32 msgEntryOffset;
        CMsgParamEntry* msgArray;
        u32 msgCount;

        msgCount = *(u32*)((u8*)attachWork + 0x1AC);

        for (msgIndex = 0; msgIndex < msgCount; msgIndex++) {
            msgFront = *(u32*)((u8*)attachWork + 0x1A8);
            msgModulus = *(u32*)((u8*)attachWork + 0x1B0);
            msgSum = msgFront + msgIndex;
            msgArray = *(CMsgParamEntry**)((u8*)attachWork + 0x1A4);
            msgRemainder = msgSum / msgModulus;
            msgRemainder = msgRemainder * msgModulus;
            msgRemainder = msgSum - msgRemainder;
            msgEntryOffset = msgRemainder * 0x24;

            if (*(u32*)((u8*)msgArray + msgEntryOffset) != 2) {
                continue;
            }

            goto renderView_after_msg_eval;
        }

        msgIndex = -1;

    renderView_after_msg_eval:
        msgQualified = (msgIndex >> 31) ^ 1;
    }

renderView_after_qualified:
    shouldRender = 0;
    if (msgQualified != 0) {
        goto renderView_after_state_end;
    }
    stateReady = 1;
    if (attachWork->mState == THREAD_STATE_LOGIN) {
        goto renderView_after_state_ready;
    }
    if (attachWork->mState == THREAD_STATE_RUN) {
        goto renderView_after_state_ready;
    }
    stateReady = 0;
renderView_after_state_ready:
    if (stateReady == 0) {
        goto renderView_after_state_end;
    }
    shouldRender = 1;
renderView_after_state_end:
    if (shouldRender != 0) {
        attachWork->wkRenderAfter();
    }
#endif

renderView_attach_after_next:
    attachNode = (void**)*attachNode;

renderView_attach_after_check:
    if (attachNode != (void**)unk238.mStartNodePtr) {
        goto renderView_attach_after_body;
    }

    lbl_eu_806655C8.ptr = nullptr;
    func_8044BE38__8CGXCacheFv(CDeviceGX::getCacheInstance());
    func_80442DA8__9CViewRootFv();
    fontFlush__10CFontLayerFi(static_cast<CFontLayer*>(this), 1);

    if (crossRootFlag == 0) {
        func_80442C68__9CViewRootFv();
    }

renderView_tail:
    if (crossRootFlag != 0) {
        return;
    }

    parentThread = mParent;
    if (getInstance__9CViewRootFv() == parentThread) {
        frameParent = nullptr;
        goto renderView_frame_call;
    }
    frameParent = CView::convertToView(mParent);

renderView_frame_call:
    // Keep frameParent live in r4 by passing as extra param to render
    ((void(*)(CViewFrame*, CView*))render__10CViewFrameFv)(&mFrame, frameParent);
}

#endif

void CView::setDisp(bool r4, bool r5) {
    if (!r4) {
        unk278 |= 0x40;
    } else {
        unk278 &= ~0x40;
    }

    if (r5 && !r4) {
        CView* current = getCurrent__9CViewRootFv();
        if (current == this) {
            invalidCurrent__9CViewRootFP5CView(this);
        }
    }
}

// True when this view is neither the root current nor under it (inactive chrome).
bool CView::hasCurrent() const {
    bool result = false;
    if (CViewRoot::isCurrent(this)) {
        goto done;
    }
    if (CViewRoot::isCurrentChild(this, CViewRoot::getCurrent())) {
        goto done;
    }
    result = true;
done:
    return result;
}

// PLAN.md 17.6: whole-function asm. MWCC C++ spills then lwz unk45C; retail
// interleaves lwz/cmpwi into the callee-save prologue (89.2% C++ cap).
asm s16 CView::getSplitLine() {
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    lwz r0, 0x45c(r3)
    stw r31, 0x1c(r1)
    li r31, 0
    cmpwi r0, 0
    stw r30, 0x18(r1)
    li r30, 0
    stw r29, 0x14(r1)
    mr r29, r3
    beq getSplitLine_no_view1
    mr r3, r0
    bl getView1__11CSplitFrameFv
    cmpwi r3, 0
    beq getSplitLine_no_view1
    li r30, 1
getSplitLine_no_view1:
    cmpwi r30, 0
    beq getSplitLine_no_view2
    lwz r3, 0x45c(r29)
    bl getView2__11CSplitFrameFv
    cmpwi r3, 0
    beq getSplitLine_no_view2
    li r31, 1
getSplitLine_no_view2:
    cmpwi r31, 0
    beq getSplitLine_zero
    lwz r3, 0x45c(r29)
    bl getSplitLine__11CSplitFrameFv
    b getSplitLine_epilogue
getSplitLine_zero:
    li r3, 0
getSplitLine_epilogue:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
}

void CView::setSplitLine(s16 line) {
    u32 hasView2;
    u32 hasView1;

    hasView2 = 0;
    hasView1 = 0;
    if (unk45C == nullptr) {
        goto setSplitLine_no_view1;
    }
    if (getView1__11CSplitFrameFv(unk45C) == nullptr) {
        goto setSplitLine_no_view1;
    }
    hasView1 = 1;

setSplitLine_no_view1:
    if (hasView1 == 0) {
        goto setSplitLine_no_view2;
    }
    if (getView2__11CSplitFrameFv(unk45C) == nullptr) {
        goto setSplitLine_no_view2;
    }
    hasView2 = 1;

setSplitLine_no_view2:
    if (hasView2 == 0) {
        return;
    }
    setSplitLine__11CSplitFrameFs(unk45C, line);
}

// Offset view of the CView context-ring control block at +0x280 (used by
// ~CView's guarded index reset).
struct CViewRingCtl {
    u8 pad[0x170];   // 0x00..0x16F (mContextMsgVtable + ring buffer)
    u32 readIndex;   // 0x170 -> CView::unk3F0
    u32 writeIndex;  // 0x174 -> CView::mContextRingWriteIndex
};

// us-8043f178 (~85%, size-equal 432B, 63 mismatch): retag both vptrs by hand
// (novtable; the leading null-this test is compiler-generated - do NOT add an
// explicit `if (this != nullptr)`, it duplicates the branch), drain both
// attached-work lists with empty() + iterator walks (the empty() guard is
// REQUIRED - without it MWCC DCEs the bare traversal), reset the context-ring
// indices behind two address-null guards via a local CViewRingCtl (chains the
// second addic off the first pointer like retail), then run the inlined
// _reslist_base dtors (vptr retag + unlink + free) for unk258 then unk238;
// base dtors (CFontLayer @+0x1C4, CWorkThread) run implicitly afterwards.
// OPEN ITEM: retail's two walk loops are UNROTATED [init][b check][body]
// [check;bne]; every guarded source form here emits the rotated
// [check;beq-exit] layout (+stray b), shifting the __dla__/base-dtor call
// sites (all reloc drift below is that layout shift). Tried: plain-pointer
// walks (DCE'd or swapped regs), goto bottom-check (re-folded), do-while+goto
// guard (worse), manual sentinel-empty guard (regs swapped). The unrotated
// layout looks like a fixed rotation-heuristic wall under unit -O4.
CView::~CView() {
    {
        *(void**)this = (void*)&lbl_eu_8056B5E0;
        *(void**)((char*)this + 0x1C4) = (char*)&lbl_eu_8056B5E0 + 0xA0;

        // Empty iterator walks over unk238 / unk258.
        // Empty iterator walks over unk238 / unk258.
        {
            reslist<WORK_ID>* workList = reinterpret_cast<reslist<WORK_ID>*>(&unk238);
            if (!workList->empty()) {
                reslist<WORK_ID>::iterator endIt = workList->end();
                reslist<WORK_ID>::iterator it = workList->begin();
                while (it != endIt) {
                    ++it;
                }
            }
        }
        {
            reslist<IWorkEvent*>* eventList =
                reinterpret_cast<reslist<IWorkEvent*>*>(&unk258);
            if (!eventList->empty()) {
                reslist<IWorkEvent*>::iterator endIt = eventList->end();
                reslist<IWorkEvent*>::iterator it = eventList->begin();
                while (it != endIt) {
                    ++it;
                }
            }
        }

        CViewRingCtl* ring = reinterpret_cast<CViewRingCtl*>(&mContextMsgVtable);
        if (ring != nullptr && &ring->pad[4] != nullptr) {
            ring->writeIndex = 0;
            ring->readIndex = 0;
        }

        // Inlined _reslist_base<IWorkEvent*> dtor for unk258 (the doubled
        // address-null test mirrors the retail guard pair).
        CViewResList* pEventList = &unk258;
        if (pEventList != nullptr && pEventList != nullptr) {
            _reslist_node<void*>* sentinel =
                static_cast<_reslist_node<void*>*>(pEventList->mStartNodePtr);
            *reinterpret_cast<void**>(pEventList) = lbl_eu_8056B6F0;
            _reslist_node<void*>* node = sentinel->mNext;
            while (node != sentinel) {
                _reslist_node<void*>* cur = node;
                node = node->mNext;
                cur->mNext = nullptr;
            }
            sentinel->mNext = sentinel;
            sentinel->mPrev = sentinel;
            if (pEventList->unk1C == false && pEventList->mList != nullptr) {
                delete[] static_cast<_reslist_node<IWorkEvent*>*>(pEventList->mList);
                pEventList->mList = nullptr;
            }
        }

        // Inlined _reslist_base<u32> dtor for unk238.
        CViewResList* pWorkList = &unk238;
        if (pWorkList != nullptr && pWorkList != nullptr) {
            _reslist_node<WORK_ID>* sentinel =
                static_cast<_reslist_node<WORK_ID>*>(pWorkList->mStartNodePtr);
            *reinterpret_cast<void**>(pWorkList) = lbl_eu_8056B298;
            _reslist_node<WORK_ID>* node = sentinel->mNext;
            while (node != sentinel) {
                _reslist_node<WORK_ID>* cur = node;
                node = node->mNext;
                cur->mNext = nullptr;
            }
            sentinel->mNext = sentinel;
            sentinel->mPrev = sentinel;
            if (pWorkList->unk1C == false && pWorkList->mList != nullptr) {
                delete[] static_cast<_reslist_node<WORK_ID>*>(pWorkList->mList);
                pWorkList->mList = nullptr;
            }
        }
    }
}

CView::CView(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 2) {
    // novtable: construct the CFontLayer base explicitly, then write both
    // retail vptrs by hand -- main vtbl at +0 and the CFontLayer secondary
    // vtbl at +0x1C4 (= main vtbl + 0xA0).
    // Cast through char* so MWCC does not emit its guarded derived-to-base
    // this-adjustment sequence -- retail performs the plain addi+bl.
    __ct__CFontLayer(reinterpret_cast<CFontLayer*>(reinterpret_cast<char*>(this) + 0x1c4));
    *(void**)this = (void*)&lbl_eu_8056B5E0;
    *(void**)(reinterpret_cast<char*>(this) + 0x1c4) =
        (void*)(&lbl_eu_8056B5E0 + 0xa0);
    // The typed reserve calls below recover retail's shared 0x10 capacity
    // lifetime; guarded postprocessing closes the final register permutation.
    u32 zero;
    void* sentinel1;
    void* sentinel0;

    zero = 0;
    mRectData.func_80459270();
    __ct__CViewFrame(&mFrame);

    *(void**)&unk238 = lbl_eu_8056B298;
    sentinel0 = &unk238.mSentinelNext;
    unk238.mList = (void*)zero;
    unk238.mCapacity = (int)zero;
    unk238.unk1C = (u8)zero;
    unk238.mStartNodePtr = sentinel0;
    unk238.mSentinelNext = sentinel0;
    unk238.mSentinelPrev = sentinel0;
    *(void**)&unk238 = lbl_eu_8056B280;

    *(void**)&unk258 = lbl_eu_8056B6F0;
    sentinel1 = &unk258.mSentinelNext;
    unk258.mList = (void*)zero;
    unk258.mCapacity = (int)zero;
    unk258.unk1C = (u8)zero;
    unk258.mStartNodePtr = sentinel1;
    unk258.mSentinelNext = sentinel1;
    unk258.mSentinelPrev = sentinel1;
    *(void**)&unk258 = lbl_eu_8056B6D8;

    unk278 = zero;
    unk27C = zero;
    mContextMsgVtable = (u32)lbl_eu_8056B6CC;
    mContextRingCapacity = 10;
    mContextRingBase = mContextRing;
    mContextRingWriteIndex = zero;
    unk3F0 = zero;
    unk3FC = zero;
    mName.clear();

    mGXCacheId = func_8044BE2C__8CGXCacheFv();
    mAlpha = lbl_eu_8066A2D0;  // 1.0f -- retail references FloatUtils' .sdata2 constant (no TU-local pool)
    unk45C = (void*)zero;
    unk460 = 0xb;
    mType = THREAD_CVIEW;
    func_8043FC60__10CViewFrameFUl(&mFrame, (u32)this);

    reinterpret_cast<reslist<u32>*>(&unk238)->reserve(mAllocHandle, 0x10);
    reinterpret_cast<reslist<void*>*>(&unk258)->reserve(mAllocHandle, 0x10);

    unk444.x = lbl_eu_8066A2D4;  // 0.6f -- FloatUtils-owned
    unk444.y = lbl_eu_8066A2D4;
    unk444.z = lbl_eu_8066A2D4;
    unk444.w = lbl_eu_8066A2D0;  // 1.0f
    unk464 = (s16)zero;
    unk466 = (s16)zero;
    unk468 = (s16)zero;
    unk46A = (s16)zero;

    mFrame.mFrameColor = sFrameColor;
    mFrame.mColor28 = lbl_8065A0C8;
}

// us-8044bfc0: walks the work-event list at +0x60 (head node stored at
// +0x60, nodes are {next; +8 = item}; circular - terminates when the walk
// returns to the head); for each node calls vtable+0xC4 on the item when its
// +0x50 code is in [48,53), otherwise on NULL (retail keeps the call for the
// NULL/invalid case - genuine low-memory read on Wii).
// OPEN ITEM: size PASS (136B) but mismatch 8. Retail's range check emits two
// raw cmpi's (48/53) with a tail-merged li reached by branch+fallthrough;
// every source form probed either range-folds into subi/cmpli (all signed
// same-CFG forms: ||, &&, else-if, nested ifs, gotos) or refuses to merge the
// identical null blocks when folding is blocked (u32 carrier + s32 casts,
// switch/case-ranges, dead-statement barriers). MWCC only combines the two
// tests into one shared block when it can also fold them - the unfolded
// shared-li shape is not reachable from high-level C under unit -O4.
// ~40 shapes probed across two sessions; see docs/MWCC_CASES.md range-fold.
void CView::CView_UnkVirtualFunc7() {
    u8* head = *reinterpret_cast<u8**>((u8*)this + 0x60);
    u8* node = *reinterpret_cast<u8**>(head);
    while (node != *reinterpret_cast<u8**>((u8*)this + 0x60)) {
        void* obj = *reinterpret_cast<void**>(node + 8);
        // Shared null-set block (reproduces retail's merged li + exact size).
        // Residual: retail's two raw cmpi 48/53 vs our -O4 range-folded
        // subi/cmpli - fixed-codegen wall (see note above).
        if (obj != NULL) {
            s32 code = *reinterpret_cast<s32*>((u8*)obj + 0x50);
            if (code < 48) {
                goto set_null;
            }
            if (code >= 53) {
                goto set_null;
            }
            goto call_item;
        }
        obj = NULL;
        goto call_item;
    set_null:
        obj = NULL;
    call_item:
        reinterpret_cast<CViewVtC4If*>((u8*)obj)->fnC4();
        node = *reinterpret_cast<u8**>(node);
    }
}
// us-80440380: same skeleton as UnkVirtualFunc4, but it strips the event's
// WORK_ID (CWorkThread::mWorkID @0x4C) from the view's work-ID reslist
// (unk238), then re-dispatches the event to each split-frame pane through
// virtual slot 0xB4 and finally to this view via slot 0xB8.
// Every textual getView1/getView2 occurrence is a fresh call in retail (opaque
// callee), hence the repeated short-circuit guard/ternary pairs.
extern "C" int CView_UnkVirtualFunc3__5CViewFv(CView* self, IWorkEvent* event) {
    // Unlink every node whose WORK_ID matches, marking removed nodes mNext=NULL
    // (explicitly expanded reslist<WORK_ID>::remove, statement-ordered to match
    // retail's register allocation).
    const WORK_ID removedId = reinterpret_cast<CWorkThread*>(event)->mWorkID;
    _reslist_node<WORK_ID>* head =
        reinterpret_cast<reslist<WORK_ID>*>(&self->unk238)->mStartNodePtr;
    _reslist_node<WORK_ID>* next = head->mNext;
    _reslist_node<WORK_ID>* curr = next;
    while (curr != head) {
        next = curr->mNext;
        if (removedId == curr->mItem) {
            _reslist_node<WORK_ID>* prev = curr->mPrev;
            prev->mNext = next;
            next->mPrev = prev;
            curr->mNext = nullptr;
        }
        curr = next;
    }

    // Pane 1: three-way guard into saved-reg flags, ternary select of pane 1,
    // test, then recompute the guard and the pane for the dispatch itself.
    CView* pane1 =
        (self->unk45C != nullptr &&
         getView1__11CSplitFrameFv(self->unk45C) != nullptr &&
         getView2__11CSplitFrameFv(self->unk45C) != nullptr)
            ? getView1__11CSplitFrameFv(self->unk45C)
            : nullptr;
    if (pane1 != nullptr) {
        CView* target1 =
            (self->unk45C != nullptr &&
             getView1__11CSplitFrameFv(self->unk45C) != nullptr &&
             getView2__11CSplitFrameFv(self->unk45C) != nullptr)
                ? getView1__11CSplitFrameFv(self->unk45C)
                : nullptr;
        reinterpret_cast<CViewVtB4If*>(target1)->fnB4(event);
    }

    // Pane 2: same guard/ternary pair with getView2 as the dispatched pane.
    CView* pane2 =
        (self->unk45C != nullptr &&
         getView1__11CSplitFrameFv(self->unk45C) != nullptr &&
         getView2__11CSplitFrameFv(self->unk45C) != nullptr)
            ? getView2__11CSplitFrameFv(self->unk45C)
            : nullptr;
    if (pane2 != nullptr) {
        CView* target2 =
            (self->unk45C != nullptr &&
             getView1__11CSplitFrameFv(self->unk45C) != nullptr &&
             getView2__11CSplitFrameFv(self->unk45C) != nullptr)
                ? getView2__11CSplitFrameFv(self->unk45C)
                : nullptr;
        reinterpret_cast<CViewVtB4If*>(target2)->fnB4(event);
    }

    // Final hop: forward the event to this view's own virtual slot 0xB8.
    reinterpret_cast<CViewVtB8If*>(self)->fnB8(event);

    return 1;
}

// us-804406b8: remove the given event from this view's attached-event reslist
// (unk258), then -- if any split frame has both panes -- re-dispatch the event
// to pane 1's and pane 2's view through virtual slot 0xB8. Each textual
// occurrence of getView1/getView2 is a fresh call in retail (opaque callee, no
// caching), hence the repeated short-circuit conditions.
extern "C" int CView_UnkVirtualFunc4__5CViewFv(CView* self, IWorkEvent* event) {
    // Unlink every node whose item matches, marking removed nodes mNext=NULL
    // (inlined reslist<IWorkEvent*>::remove).
    reinterpret_cast<reslist<IWorkEvent*>*>(&self->unk258)->remove(event);

    // Pane 1: retail computes the three-way guard into saved-reg flags,
    // selects pane 1 through a ternary, tests it, then recomputes the guard
    // and the pane for the dispatch itself.
    CView* pane1 =
        (self->unk45C != nullptr &&
         getView1__11CSplitFrameFv(self->unk45C) != nullptr &&
         getView2__11CSplitFrameFv(self->unk45C) != nullptr)
            ? getView1__11CSplitFrameFv(self->unk45C)
            : nullptr;
    if (pane1 != nullptr) {
        CView* target1 =
            (self->unk45C != nullptr &&
             getView1__11CSplitFrameFv(self->unk45C) != nullptr &&
             getView2__11CSplitFrameFv(self->unk45C) != nullptr)
                ? getView1__11CSplitFrameFv(self->unk45C)
                : nullptr;
        reinterpret_cast<CViewVtB8If*>(target1)->fnB8(event);
    }

    // Pane 2: same guard/ternary pair with getView2 as the dispatched pane.
    CView* pane2 =
        (self->unk45C != nullptr &&
         getView1__11CSplitFrameFv(self->unk45C) != nullptr &&
         getView2__11CSplitFrameFv(self->unk45C) != nullptr)
            ? getView2__11CSplitFrameFv(self->unk45C)
            : nullptr;
    if (pane2 != nullptr) {
        CView* target2 =
            (self->unk45C != nullptr &&
             getView1__11CSplitFrameFv(self->unk45C) != nullptr &&
             getView2__11CSplitFrameFv(self->unk45C) != nullptr)
                ? getView2__11CSplitFrameFv(self->unk45C)
                : nullptr;
        reinterpret_cast<CViewVtB8If*>(target2)->fnB8(event);
    }

    return 1;
}

// Retail stubs (0x8 bytes): addi r3,r3,476 (cast to the embedded
// CViewFrame base) followed by a sibcall 'b' to the base implementation.
extern "C" void CView_UnkVirtualFunc1__5CViewFv(CView* self) {
    self->mFrame.CView_UnkVirtualFunc1();
}
extern "C" void CView_UnkVirtualFunc8__5CViewFv(CView* self) {
    self->mFrame.CView_UnkVirtualFunc8();
}
extern "C" void CView_UnkVirtualFunc9__5CViewFv(CView* self) {
    self->mFrame.CView_UnkVirtualFunc9();
}

// us-8043CE90: byte flag at +0x8 of the split-frame object, gated on the
// split-frame pointer being set.
extern "C" u8 func_8043CE90__5CViewFv(CView* self) {
    void* split = self->unk45C;
    if (split != nullptr) {
        return *reinterpret_cast<u8*>(static_cast<u8*>(split) + 8);
    }
    return 0;
}

// us-8043CEAC: split-frame view checks with explicit flags -- each condition
// crosses a subsequent call, so the results live in saved regs in retail.
extern "C" s32 func_8043CEAC__5CViewFv(CView* self) {
    s32 flag2 = 0;
    s32 flag1 = 0;
    if (self->unk45C != nullptr) {
        if (getView1__11CSplitFrameFv(self->unk45C) != nullptr) {
            flag1 = 1;
        }
    }
    if (flag1 != 0) {
        if (getView2__11CSplitFrameFv(self->unk45C) != nullptr) {
            flag2 = 1;
        }
    }
    if (flag2 != 0) {
        return (s32)(intptr_t)getView2__11CSplitFrameFv(self->unk45C);
    }
    return 0;
}

// us-8043CAFC: like CEAC -- returns whether BOTH split-frame views exist.
extern "C" s32 func_8043CAFC__5CViewFv(CView* self) {
    s32 flag2 = 0;
    s32 flag1 = 0;
    if (self->unk45C != nullptr) {
        if (getView1__11CSplitFrameFv(self->unk45C) != nullptr) {
            flag1 = 1;
        }
    }
    if (flag1 != 0) {
        if (getView2__11CSplitFrameFv(self->unk45C) != nullptr) {
            flag2 = 1;
        }
    }
    return flag2;
}

// us-8043DF3C: single-step parent walk (CVIEW_WALK_NEXT shape, no loop).
static CView* CView_toView(CWorkThread* thread);

// us-8043E46C: walker that accumulates each ancestor's content origin + view
// offset straight into rect (no tail-offset application, no size stores).
// Retail computes dy before dx each iteration and re-reads rect.mPos from
// memory (no cached start locals), so keep those orderings here.
extern "C" void func_8043E46C__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect,
                                                          CView* other) {
    CView* cur;
    CWorkThread* parentSnap;
    ml::CRect16 off;
    rect.mPos.x = other->mFrame.mContentX;
    rect.mPos.y = other->mFrame.mContentY;
    parentSnap = other->mParent;

    // Walk init: root parent terminates the chain; otherwise cast via
    // convertToView (null + thread-type range check).
    if (getInstance__9CViewRootFv() == parentSnap) {
        cur = nullptr;
    } else {
        cur = CView_toView(other->mParent);
    }
    while (cur != nullptr) {
        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&off, &cur->mFrame);
        s16 dy = cur->mFrame.mContentY + off.mPos.y;
        s16 dx = cur->mFrame.mContentX + off.mPos.x;
        parentSnap = cur->mParent;
        rect.mPos.x += dx;
        rect.mPos.y += dy;

        if (getInstance__9CViewRootFv() == parentSnap) {
            cur = nullptr;
        } else {
            cur = CView_toView(parentSnap);
        }
    }
}

// CLib/CViewRoot init guards (static members; literal retail symbol names).
extern "C" bool isInitialized__4CLibFv();
extern "C" bool isInitialized__9CViewRootFv();

extern "C" CView* func_8043DF3C__5CViewFv(CView* self) {
    CWorkThread* parentSnap;
    CView* cur;
    parentSnap = self->mParent;
    if (getInstance__9CViewRootFv() == parentSnap) {
        cur = nullptr;
    } else {
        cur = CView_toView(self->mParent);
    }
    return cur;
}

// us-80442278: standby login. Guards on CLib/CViewRoot init, enqueues a raw
// tag-6 context-ring message when the split-mode bit (unk278 bit 3) is CLEAR,
// then forwards to the CWorkThread base login. Bit set skips straight to the
// base call.
extern "C" bool wkStandbyLogin__5CViewFv(CView* self) {
    if (!isInitialized__4CLibFv()) {
        goto login_fail;
    }
    if (!isInitialized__9CViewRootFv()) {
        goto login_fail;
    }
    if ((self->unk278 & 8) == 0) {
        // Tag-6 context-ring enqueue (inlined CMsgParam<10>::enqueue shape;
        // signed index, update-form stwux tag store). Payload fields are
        // uninitialized frame homes -- only read once each.
        volatile CMsgParamEntry msg;
        s32 capSigned = (s32)self->mContextRingCapacity;
        s32 sumSigned = (s32)self->unk3F0 + (s32)self->mContextRingWriteIndex;
        // Volatile-qualified view pins the base-field load above the payload
        // home reads (retail issues it right after the index-sum).
        u8* volatile& baseRef = self->mContextRingBase;
        u8* entry = baseRef + (u32)(sumSigned % capSigned) * 0x24u;
        *(u32*)entry = 6; // tag 6
        *(u32*)(entry + 0x4) = msg.wid;
        *(u32*)(entry + 0x8) = msg.unk8;
        *(u32*)(entry + 0xC) = msg.unkC;
        *(u32*)(entry + 0x10) = msg.unk10;
        *(u32*)(entry + 0x14) = msg.unk14;
        *(u32*)(entry + 0x18) = msg.unk18;
        *(u32*)(entry + 0x1C) = msg.unk1C;
        *(u16*)(entry + 0x20) = (u16)msg.unk20;
        entry[0x22] = msg.unk22;
        entry[0x23] = 0;

        u32 writeIdx = self->mContextRingWriteIndex + 1;
        u32 prevIdx = writeIdx - 1;
        self->mContextRingWriteIndex = writeIdx;
        self->unk3FC = prevIdx;
    }
    return wkStandbyLogin__11CWorkThreadFv(self);

login_fail:
    return false;
}


extern "C" int CView_UnkVirtualFunc6__5CViewFv() { return 0; }
extern "C" int CView_UnkVirtualFunc5__5CViewFv() { return 0; }

// Static init: copy CCol4::white into sFrameColor (u32 struct copy) and
// set lbl_8065A0C8 from four float constants. Statement order matters:
// retail materializes the lbl_8065A0C8 base (lis) before sFrameColor's, so
// the float-field stores must precede the struct copy; direct member
// assignment (not set()) reproduces the retail FPR coloring f3/f2/f1/f0.
extern float lbl_eu_8066A2E0;
extern float lbl_eu_8066A2E4;
extern float lbl_eu_8066A2D4;  // 0.6f (FloatUtils .sdata2)
extern float lbl_eu_8066A2D0;  // 1.0f (FloatUtils .sdata2)
extern "C" void sinit_8043FB70() {
    lbl_8065A0C8.r = lbl_eu_8066A2E0;
    lbl_8065A0C8.g = lbl_eu_8066A2E4;
    lbl_8065A0C8.b = lbl_eu_8066A2D4;
    lbl_8065A0C8.a = lbl_eu_8066A2D0;
    CView::sFrameColor = ml::CCol4::white;
}

// us-80441420: rect with zero origin and size = other->mRectData view size
// minus its insets (left/right, top/bottom). Retail has no `this` use -- the
// member-scoped name is emitted as a free function (rect=r3, other=r4).
extern "C" void func_8043EA88__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect,
                                                          CView* other) {
    // Best-known shape: both size chains compound, then the zero origin via
    // chained assignment. Byte diff vs retail is exactly one scheduler
    // transposition (retail issues 'sth r0,0(r3)' between the two final
    // subfs); every statement layout tried keeps either this swap, extsh
    // insertion, or reg drift.
    s16 w = other->mRectData.mViewSize.x - other->mRectData.mInsetLeft -
            other->mRectData.mInsetRight;
    s16 h = other->mRectData.mViewSize.y - other->mRectData.mInsetTop -
            other->mRectData.mInsetBottom;
    rect.mPos.y = rect.mPos.x = 0;
    rect.mSize.x = w;
    rect.mSize.y = h;
}

// ------------------------------------------------------------------
// Parent-chain position walkers. All four share the same upward walk:
// follow mParent while the parent passes the view-type filter and is not
// the CViewRoot singleton, accumulating each frame's view offset plus its
// content origin into (x, y). Retail emits the first step unrolled and
// computes the next node at the END of the loop body (bottom-tested), so
// each function below duplicates that structure.
// ------------------------------------------------------------------

// View-type filter used by the walkers -- retail uses the inline
// CView::convertToView predicate (rejects type < THREAD_CVIEW or >= MAX).
static CView* CView_toView(CWorkThread* thread) {
    return CView::convertToView(thread);
}

#define CVIEW_WALK_NEXT(cur, parentSnap)                                       \
    if (getInstance__9CViewRootFv() == (parentSnap)) {                         \
        (cur) = nullptr;                                                       \
    } else {                                                                   \
        (cur) = CView_toView((cur)->mParent);                                  \
    }

// 4-byte-aligned CRect16-shaped local: retail packs the walker stack
// rects at 4-byte strides, which 8-aligned ml::CRect16 cannot do.
struct CViewStackRect {
    ml::CPnt16 mPos;
    ml::CPnt16 mSize;
};

// us-80440f24: accumulated content origin + final frame's view-offset size.
extern "C" void func_8043E58C__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect,
                                                          CView* other) {
    CWorkThread* parentSnap;
    CView* cur;
    s16 x = other->mFrame.mContentX;
    s16 y = other->mFrame.mContentY;
    ml::CRect16 tailOff;
    ml::CRect16 off;
    parentSnap = other->mParent;

    if (getInstance__9CViewRootFv() == parentSnap) {
        cur = nullptr;
    } else {
        cur = CView_toView(other->mParent);
    }
    while (cur != nullptr) {
        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(&off, &cur->mFrame);
        s16 dx = cur->mFrame.mContentX + off.mPos.x;
        x += dx;
        parentSnap = cur->mParent;
        s16 dy = cur->mFrame.mContentY + off.mPos.y;
        y += dy;
        CVIEW_WALK_NEXT(cur, parentSnap);
    }

    func_8043FD10__10CViewFrameFR7CRect16PC10CViewFrame(&tailOff, &other->mFrame);
    rect.mPos.x = x;
    rect.mPos.y = y;
    rect.mSize.x = tailOff.mSize.x;
    rect.mSize.y = tailOff.mSize.y;
}

// us-80441044: accumulated content origin offset by this frame's own view
// offset (position only, no size).
extern "C" void func_8043E6AC__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect,
                                                          CView* other) {
    CWorkThread* parentSnap;
    CView* cur;
    ml::CPnt16 walkBuf[3];
    s16 x = other->mFrame.mContentX;
    s16 y = other->mFrame.mContentY;
    ml::CRect16 tailOff;
    ml::CRect16 off;
    parentSnap = other->mParent;

    if (getInstance__9CViewRootFv() == parentSnap) {
        cur = nullptr;
    } else {
        cur = CView_toView(other->mParent);
    }
    while (cur != nullptr) {
        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
            (ml::CRect16*)&walkBuf[0], &cur->mFrame);
        s16 dx = cur->mFrame.mContentX + walkBuf[0].x;
        x += dx;
        parentSnap = cur->mParent;
        s16 dy = cur->mFrame.mContentY + walkBuf[0].y;
        y += dy;
        CVIEW_WALK_NEXT(cur, parentSnap);
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
        (ml::CRect16*)&walkBuf[1], &other->mFrame);
    s16 posY = y + walkBuf[1].y;
    s16 posX = x + walkBuf[1].x;
    rect.mPos.x = posX;
    rect.mPos.y = posY;
}

// us-80441164: provisional rect from this frame's offsets and view size,
// then the walk recomputes the absolute origin.
extern "C" void func_8043E7CC__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect,
                                                          CView* other) {
    CWorkThread* parentSnap;
    CView* cur;
    ml::CPnt16 walkBuf[3];
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
        (ml::CRect16*)&walkBuf[2], &other->mFrame);
    s16 py = other->mFrame.mContentY + walkBuf[2].y;
    s16 px = other->mFrame.mContentX + walkBuf[2].x;
    rect.mPos.x = px;
    rect.mPos.y = py;
    rect.mSize.x = other->mRectData.mViewSize.x;
    rect.mSize.y = other->mRectData.mViewSize.y;
    // Retail hoists these loads after the provisional stores.
    s16 x = other->mFrame.mContentX;
    s16 y = other->mFrame.mContentY;

    parentSnap = other->mParent;

    if (getInstance__9CViewRootFv() == parentSnap) {
        cur = nullptr;
    } else {
        cur = CView_toView(other->mParent);
    }
    while (cur != nullptr) {
        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
            (ml::CRect16*)&walkBuf[1], &cur->mFrame);
        s16 dx = cur->mFrame.mContentX + walkBuf[1].x;
        x += dx;
        parentSnap = cur->mParent;
        s16 dy = cur->mFrame.mContentY + walkBuf[1].y;
        y += dy;
        CVIEW_WALK_NEXT(cur, parentSnap);
    }

    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
        (ml::CRect16*)&walkBuf[0], &other->mFrame);
    s16 tailY = y + walkBuf[0].y;
    s16 tailX = x + walkBuf[0].x;
    rect.mPos.x = tailX;
    rect.mPos.y = tailY;
}

// us-804412c0: like 8043E7CC but the size is shrunk by the view insets and
// the origin shifted by the left/top insets.
extern "C" void func_8043E928__5CViewFRQ22ml5CRectP5CView(ml::CRect& rect,
                                                          CView* other) {
    CWorkThread* parentSnap;
    CView* cur;
    s16 x = other->mFrame.mContentX;
    s16 y = other->mFrame.mContentY;
    ml::CPnt16 walkBuf[2];
    parentSnap = other->mParent;

    if (getInstance__9CViewRootFv() == parentSnap) {
        cur = nullptr;
    } else {
        cur = CView_toView(other->mParent);
    }
    while (cur != nullptr) {
        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
            (ml::CRect16*)&walkBuf[0], &cur->mFrame);
        s16 dx = cur->mFrame.mContentX + walkBuf[0].x;
        x += dx;
        parentSnap = cur->mParent;
        s16 dy = cur->mFrame.mContentY + walkBuf[0].y;
        y += dy;
        CVIEW_WALK_NEXT(cur, parentSnap);
    }

    // Tail statement order mirrors retail scheduling: origin-Y first, then
    // the y-basis loads, then origin-X / x-basis, sizes last.
    getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
        (ml::CRect16*)&walkBuf[1], &other->mFrame);
    s16 py = y + walkBuf[1].y;
    s16 px = x + walkBuf[1].x;
    py += other->mRectData.mInsetTop;
    px += other->mRectData.mInsetLeft;
    s16 sizeX = other->mRectData.mViewSize.x - other->mRectData.mInsetLeft -
                other->mRectData.mInsetRight;
    s16 sizeY = other->mRectData.mViewSize.y - other->mRectData.mInsetTop -
                other->mRectData.mInsetBottom;
    rect.mPos.x = px;
    rect.mPos.y = py;
    rect.mSize.x = sizeX;
    rect.mSize.y = sizeY;
}

// us-8043f664: recompute this view's rect data from its frame. Bit0 of
// unk278 selects split-mode (negated view offset as content origin, size
// from parent or render mode); otherwise size = frame rect extent. Bit4
// (0x10) skips the func_80459384 max-size clamp.
extern "C" void func_8043CCCC__5CViewFv(CView* self, u32 arg) {
    // Retail slot map (ascending): modeSize@0x8, ifPos@0xC, ifOff@0x10,
    // elsePos@0x14, elseOff@0x18, hole@0x1C, tmp@0x20. The per-branch rects
    // are split into pos/off CPnt16 halves; the callee's size half lands in
    // the off slot and is overwritten by the negated/diff values.
    CViewStackRect tmp;
    CWorkThread* parentSnap;
    ml::CPnt16 elseOff;
    ml::CPnt16 elsePos;
    ml::CPnt16 ifOff;
    ml::CPnt16 ifPos;
    struct {
        u16 x;
        u16 y;
    } modeSize;
    // Retail forwards its own r4 into the callee's third parameter.
    func_80441EF0__10CViewFrameFR7CRect16PC10CViewFrame(
        (ml::CRect16*)&tmp, &self->mFrame, arg);

    if ((self->unk278 & 1) != 0) {
        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
            (ml::CRect16*)&ifPos, &self->mFrame);
        ifOff.x = -ifPos.x;
        // Load anchored here so the scheduler places the mParent fetch
        // between the second neg and its store, matching retail.
        parentSnap = self->mParent;
        ifOff.y = -ifPos.y;
        *(u32*)&self->mFrame.mContentX = *(u32*)&ifOff;

        CView* parentView;
        if (getInstance__9CViewRootFv() == parentSnap) {
            parentView = nullptr;
        } else {
            parentView = CView::convertToView(self->mParent);
        }

        if (parentView != nullptr) {
            func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(
                &self->mRectData,
                static_cast<CView*>(parentView)->mRectData.mBoundsSize);
        } else {
            GXRenderModeObj* renderMode = getRenderModeObj__9CDeviceVIFv();
            u16 modeHeight = renderMode->efbHeight;
            renderMode = getRenderModeObj__9CDeviceVIFv();
            u16 modeW = renderMode->fbWidth;
            modeSize.x = modeW;
            modeSize.y = modeHeight;
            func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(
                &self->mRectData, *(const ml::CPnt16*)&modeSize);
        }
    } else {
        getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(
            (ml::CRect16*)&elsePos, &self->mFrame);
        elseOff.x = tmp.mPos.x - elsePos.x;
        elseOff.y = tmp.mPos.y - elsePos.y;
        *(u32*)&self->mFrame.mContentX = *(u32*)&elseOff;
        // NOTE: passes tmp.mSize (first-call rect), not the computed extent.
        func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(&self->mRectData, tmp.mSize);
    }

    if ((self->unk278 & 0x10) == 0) {
        func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(&self->mRectData, tmp.mSize);
    }
}
