/* Data-only TU for monolibdata1d (retail: build/us/obj/monolibdata1d.o).
 *
 * Sections: .data 0x89C (2204 B), .bss 0x370 (880 B), .sbss 0xB8 (184 B);
 * 468 relocs in .data (vtable / RTTI / typeinfo / jumptable references).
 *
 * This is a pure data TU compiled by MWCC with `-lang=c++` only (no include
 * paths), so it is self-contained: no <types.h>, no monolib headers.  The
 * owning types are the monolib library classes (CView / CWorkThread /
 * IWorkEvent / CProcess / CDeviceBase / ...), whose vtable shapes are
 * reproduced here with typed structs; function slots reference the retail
 * mangled names via `extern "C"` declarations (MWCC never mangles those, so
 * the emitted reloc name is exactly the identifier).
 *
 * Encoding notes (verified against the retail split object):
 *  - .data symbols in forward declaration order; every word is either a
 *    reloc site (placeholder 0 bytes, exactly like retail) or a literal.
 *  - vtable header is [typeinfo label, offset-to-top] where the typeinfo
 *    label (lbl_eu_8066xxxx / lbl_eu_806619xx) lives in another unit.
 *  - .bss forward, .sbss REVERSE declaration order (MWCC emits .sbss in
 *    reverse of the source list); both NOBITS so only size+align matter.
 *
 * Residuals (reported to the coordinator):
 *  - 10 function-slot relocs cannot be expressed in C++ (the names contain
 *    `@`/`<`/`>` or start with the reserved `__dt`): 5 anonymous-namespace
 *    symbols and 5 template-instantiation dtors.  Slots are literal zeros;
 *    bytes match retail (retail also stores placeholder zeros there), but the
 *    .data reloc list drifts at those 10 offsets.
 *  - retail split .data has sh_addralign=4 (dtk derives it from the DOL base
 *    0x8056B55C); MWCC always emits sh_addralign=8 for .data.  Byte content
 *    is identical; only the section alignment metadata differs.
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

/* ===================== external data labels (other units) ===================
 * Typeinfo / RTTI-chain objects referenced by the vtable headers below.
 * MWCC never mangles global data names, so the reloc name is the identifier.
 */
extern char lbl_eu_80661948; /* typeinfo chain: CProcess (SI list)            */
extern char lbl_eu_80661950; /* typeinfo chain: CChildListNode                */
extern char lbl_eu_80661958; /* typeinfo chain: CDoubleListNode               */
extern char lbl_eu_80663570; /* typeinfo chain: CTTask<CRootProc>             */
extern char lbl_eu_80663578; /* typeinfo chain: CView                         */
extern char lbl_eu_80663580; /* typeinfo chain: CMsgParam<10>                 */
extern char lbl_eu_80663588; /* typeinfo chain: reslist<IWorkEvent*>          */
extern char lbl_eu_80663590; /* typeinfo chain: _reslist_base<IWorkEvent*>    */
extern char lbl_eu_80663598; /* typeinfo chain: (CView SI descriptor)         */
extern char lbl_eu_806635A0; /* typeinfo chain: CViewFrame                    */
extern char lbl_eu_806635A8; /* typeinfo chain: CViewRoot                     */
extern char lbl_eu_806635B0; /* typeinfo chain: CWorkControl                  */
extern char lbl_eu_806635B8; /* typeinfo chain: CWorkFlowSetup                */
extern char lbl_eu_806635C0; /* typeinfo chain: CWorkRootThread               */
extern char lbl_eu_806635C8; /* typeinfo chain: CWorkSystem                   */
extern char lbl_eu_806635D0; /* typeinfo chain: CWorkSystemMem                */
extern char lbl_eu_806635D8; /* typeinfo chain: TChildListHeader<CProcess>    */
extern char lbl_eu_806635E0; /* typeinfo chain: TChildListHeader<CChildList..>*/
extern char lbl_eu_806635E8; /* typeinfo chain: CDeviceRemotePad              */
extern char lbl_eu_806635F0; /* typeinfo chain: CDeviceBase                   */
extern char lbl_eu_806635F8; /* typeinfo chain: CDeviceSC                     */
extern char __RTTI__10IWorkEvent;   /* IWorkEvent typeinfo object             */
extern char __RTTI__11CWorkThread;  /* CWorkThread typeinfo object            */

/* =============== external function declarations (retail names) ==============
 * `extern "C"` keeps the emitted symbol exactly equal to the identifier, so
 * the vtable slots reference the retail mangled names 1:1.  Declared as
 * void() -- only the NAME matters for reloc identity.
 */
extern "C" {
void __dt__5CViewFv();
void WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
void OnFileEvent__10IWorkEventFP10CEventFile(void*);
void WorkEvent3__10IWorkEventFPv(void*);
void WorkEvent4__10IWorkEventFv();
void OnPauseTrigger__10IWorkEventFb(bool);
void WorkEvent6__10IWorkEventFv();
void WorkEvent7__10IWorkEventFv();
void WorkEvent8__10IWorkEventFv();
void WorkEvent9__10IWorkEventFv();
void WorkEvent10__10IWorkEventFv();
void WorkEvent11__10IWorkEventFv();
void WorkEvent12__10IWorkEventFv();
void WorkEvent13__10IWorkEventFv();
void WorkEvent14__10IWorkEventFv();
void WorkEvent15__10IWorkEventFv();
void WorkEvent16__10IWorkEventFv();
void WorkEvent17__10IWorkEventFv();
void WorkEvent18__10IWorkEventFv();
void WorkEvent19__10IWorkEventFv();
void WorkEvent20__10IWorkEventFv();
void WorkEvent21__10IWorkEventFv();
void WorkEvent22__10IWorkEventFv();
void WorkEvent23__10IWorkEventFv();
void WorkEvent24__10IWorkEventFv();
void WorkEvent25__10IWorkEventFv();
void WorkEvent26__10IWorkEventFv();
void WorkEvent27__10IWorkEventFv();
void WorkEvent28__10IWorkEventFv();
void WorkEvent29__10IWorkEventFv();
void WorkEvent30__10IWorkEventFv();
void WorkEvent31__10IWorkEventFv();
void wkUpdate__5CViewFv();
void wkRender__11CWorkThreadFv();
void wkRenderAfter__11CWorkThreadFv();
void wkStandbyLogin__5CViewFv();
void wkStandbyLogout__5CViewFv();
void wkStandbyExceptionRetry__11CWorkThreadFUl(u32);
void updateMsg__5CViewFv();
void func_8043FBC4();
void CView_UnkVirtualFunc1__5CViewFv();
void detachRenderWork__5CViewFP11CWorkThread();
void CView_UnkVirtualFunc3__5CViewFv();
void CView_UnkVirtualFunc4__5CViewFv();
void CView_UnkVirtualFunc5__5CViewFv();
void CView_UnkVirtualFunc6__5CViewFv();
void CView_UnkVirtualFunc7__5CViewFv();
void CView_UnkVirtualFunc8__5CViewFv();
void CView_UnkVirtualFunc9__5CViewFv();
void __dt__10CViewFrameFv();
void __dt__9CViewRootFv();
void wkStandbyLogin__9CViewRootFv();
void wkStandbyLogout__9CViewRootFv();
void __dt__12CWorkControlFv();
void wkStandbyLogin__12CWorkControlFv();
void wkStandbyLogout__12CWorkControlFv();
void __dt__14CWorkFlowSetupFv();
void wkStandbyLogin__14CWorkFlowSetupFv();
void wkStandbyLogout__14CWorkFlowSetupFv();
void wkUpdate__11CWorkThreadFv();
void wkStandbyLogin__11CWorkThreadFv();
void wkStandbyLogout__11CWorkThreadFv();
void __dt__11CWorkSystemFv();
void wkUpdate__11CWorkSystemFv();
void wkStandbyLogin__11CWorkSystemFv();
void wkStandbyLogout__11CWorkSystemFv();
void __dt__14CWorkSystemMemFv();
void wkStandbyLogin__14CWorkSystemMemFv();
void wkStandbyLogout__14CWorkSystemMemFv();
void __dt__8CProcessFv();
void __dt__14CChildListNodeFv();
void Reset__14CChildListNodeFv();
void Tail__8CProcessFv();
void __dt__15CDoubleListNodeFv();
void Reset__15CDoubleListNodeFv();
void __dt__16CDeviceRemotePadFv();
void wkUpdate__16CDeviceRemotePadFv();
void wkStandbyLogin__16CDeviceRemotePadFv();
void wkStandbyLogout__16CDeviceRemotePadFv();
void __dt__11CDeviceBaseFv();
void __dt__9CDeviceSCFv();
void wkStandbyLogin__9CDeviceSCFv();
void wkStandbyLogout__9CDeviceSCFv();
}

/* ============================ vtable shapes ================================= */

/* Shared 40-word (0xA0) primary vtable of every CWorkThread-derived class:
 * [typeinfo, offset-to-top, ~dt, IWorkEvent::WorkEvent1..31, wk* overrides]. */
struct IWorkEventVtbl {
    void* typeinfo;                  /* SI chain label (lbl_eu_8066xxxx)  */
    u32   offsetToTop;               /* 0 for primary vtables             */
    void  (*dt)();                   /* dtor                              */
    void  (*workEvent1)(void*, const char*);
    void  (*onFileEvent)(void*);
    void  (*workEvent3)(void*);
    void  (*workEvent4)();
    void  (*onPauseTrigger)(bool);
    void  (*workEvent6)();
    void  (*workEvent7)();
    void  (*workEvent8)();
    void  (*workEvent9)();
    void  (*workEvent10)();
    void  (*workEvent11)();
    void  (*workEvent12)();
    void  (*workEvent13)();
    void  (*workEvent14)();
    void  (*workEvent15)();
    void  (*workEvent16)();
    void  (*workEvent17)();
    void  (*workEvent18)();
    void  (*workEvent19)();
    void  (*workEvent20)();
    void  (*workEvent21)();
    void  (*workEvent22)();
    void  (*workEvent23)();
    void  (*workEvent24)();
    void  (*workEvent25)();
    void  (*workEvent26)();
    void  (*workEvent27)();
    void  (*workEvent28)();
    void  (*workEvent29)();
    void  (*workEvent30)();
    void  (*workEvent31)();
    void  (*wkUpdate)();
    void  (*wkRender)();
    void  (*wkRenderAfter)();
    void  (*wkStandbyLogin)();
    void  (*wkStandbyLogout)();
    void  (*wkStandbyExceptionRetry)(u32);
};

/* IWorkEvent event slots 1..31, shared verbatim by every 40-word vtable. */
#define IWEVTBL_EVENTS                                                          \
    &WorkEvent1__10IWorkEventFPvPCc,                                            \
    &OnFileEvent__10IWorkEventFP10CEventFile,                                   \
    &WorkEvent3__10IWorkEventFPv,                                               \
    &WorkEvent4__10IWorkEventFv,                                                \
    &OnPauseTrigger__10IWorkEventFb,                                            \
    &WorkEvent6__10IWorkEventFv,                                                \
    &WorkEvent7__10IWorkEventFv,                                                \
    &WorkEvent8__10IWorkEventFv,                                                \
    &WorkEvent9__10IWorkEventFv,                                                \
    &WorkEvent10__10IWorkEventFv,                                               \
    &WorkEvent11__10IWorkEventFv,                                               \
    &WorkEvent12__10IWorkEventFv,                                               \
    &WorkEvent13__10IWorkEventFv,                                               \
    &WorkEvent14__10IWorkEventFv,                                               \
    &WorkEvent15__10IWorkEventFv,                                               \
    &WorkEvent16__10IWorkEventFv,                                               \
    &WorkEvent17__10IWorkEventFv,                                               \
    &WorkEvent18__10IWorkEventFv,                                               \
    &WorkEvent19__10IWorkEventFv,                                               \
    &WorkEvent20__10IWorkEventFv,                                               \
    &WorkEvent21__10IWorkEventFv,                                               \
    &WorkEvent22__10IWorkEventFv,                                               \
    &WorkEvent23__10IWorkEventFv,                                               \
    &WorkEvent24__10IWorkEventFv,                                               \
    &WorkEvent25__10IWorkEventFv,                                               \
    &WorkEvent26__10IWorkEventFv,                                               \
    &WorkEvent27__10IWorkEventFv,                                               \
    &WorkEvent28__10IWorkEventFv,                                               \
    &WorkEvent29__10IWorkEventFv,                                               \
    &WorkEvent30__10IWorkEventFv,                                               \
    &WorkEvent31__10IWorkEventFv

/* CProcess-shaped vtable (9 words): [typeinfo, 0, dt, Reset, Init, Term, Move,
 * Draw, Tail] -- Init/Term/Move/Draw are pure virtuals (0) in retail. */
struct CProcessVtbl {
    void* typeinfo;
    u32   offsetToTop;
    void  (*dt)();
    void  (*reset)();
    void  (*init)();
    void  (*term)();
    void  (*move)();
    void  (*draw)();
    void  (*tail)();
};

/* CDoubleListNode / CChildListNode-shaped vtable (4 words). */
struct CNodeVtbl {
    void* typeinfo;
    u32   offsetToTop;
    void  (*dt)();
    void  (*reset)();
};

/* 3-word vtable (single virtual dtor, no other slots). */
struct SmallVtbl3 {
    void* typeinfo;
    u32   offsetToTop;
    void  (*dt)();
};

/* 4-word vtable (dtor + one trailing slot). */
struct SmallVtbl4 {
    void* typeinfo;
    u32   offsetToTop;
    void  (*dt)();
    void  (*extra)();
};

/* RTTI reference blob (6 words): [__RTTI__10IWorkEvent, 0, __RTTI__11CWorkThread,
 * 0, 0, 0] -- the class-hierarchy descriptor following each 0xA0 vtable. */
struct RttiRef6 {
    void* rttiIWorkEvent;
    u32   z0;
    void* rttiCWorkThread;
    u32   z1;
    u32   z2;
    u32   z3;
};

/* RTTI reference blob (8 words): same plus [typeinfo 806635F0, 0, 0, 0] -- used
 * by CDeviceBase / CDeviceSC (extra base typeinfo). */
struct RttiRef8 {
    void* rttiIWorkEvent;
    u32   z0;
    void* rttiCWorkThread;
    u32   z1;
    void* extraTypeinfo;
    u32   z2;
    u32   z3;
    u32   z4;
};

/* CView's second (CFontLayer-subobject) vtable (12 words). */
struct CViewSecondaryVtbl {
    void* typeinfo;                 /* same SI label as the primary vtable  */
    u32   offsetToTop;              /* 0xFFFFFE3C = -452 (base at +0x1C4)   */
    void  (*f0)();                  /* func_8043FBC4                        */
    void  (*f1)();                  /* CView_UnkVirtualFunc1                */
    void  (*f2)();                  /* detachRenderWork                     */
    void  (*f3)();                  /* CView_UnkVirtualFunc3                */
    void  (*f4)();                  /* CView_UnkVirtualFunc4                */
    void  (*f5)();                  /* CView_UnkVirtualFunc5                */
    void  (*f6)();                  /* CView_UnkVirtualFunc6                */
    void  (*f7)();                  /* CView_UnkVirtualFunc7                */
    void  (*f8)();                  /* CView_UnkVirtualFunc8                */
    void  (*f9)();                  /* CView_UnkVirtualFunc9                */
};

/* CView complete vtable region (0xD0): primary + secondary. */
struct CViewVtbls {
    IWorkEventVtbl      primary;
    CViewSecondaryVtbl  secondary;
};

/* ================================= .data =================================== */

/* 0x8056B55C | 0x24 | four (typeinfo, 0) pairs + trailing 0 -- SI-list-style
 * block referencing the CProcess family typeinfo chains. */
struct TypeinfoQuad {
    void* t0; u32 z0;
    void* t1; u32 z1;
    void* t2; u32 z2;
    void* t3; u32 z3;
    u32   tail;
};
TypeinfoQuad lbl_eu_8056B55C = {
    &lbl_eu_80661958, 0,
    &lbl_eu_80661950, 0,
    &lbl_eu_80661948, 0,
    &lbl_eu_80663570, 0,
    0,
};

/* 0x8056B580 | 0x24 | CTTask<CRootProc> vtable (CProcess shape).  dtor/Move/
 * Draw are anonymous-namespace symbols (CTaskManager_cpp) -- not nameable. */
CProcessVtbl lbl_eu_8056B580 = {
    &lbl_eu_80663570, 0,
    0,                     /* __dt__48CTTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>Fv */
    &Reset__14CChildListNodeFv,
    0,                     /* pure virtual Init  */
    0,                     /* pure virtual Term  */
    0,                     /* Move__48CTTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>Fv */
    0,                     /* Draw__48CTTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>Fv */
    &Tail__8CProcessFv,
};

/* 0x8056B5A4 | 0x1C | three (typeinfo, 0) pairs + trailing 0. */
struct TypeinfoTriple {
    void* t0; u32 z0;
    void* t1; u32 z1;
    void* t2; u32 z2;
    u32   tail;
};
TypeinfoTriple lbl_eu_8056B5A4 = {
    &lbl_eu_80661958, 0,
    &lbl_eu_80661950, 0,
    &lbl_eu_80661948, 0,
    0,
};

/* 0x8056B5C0 | 0x20 | CView::updateMsg switch jump table (relocs w/ addends). */
u32 jumptable_eu_8056B5C0[8] = {
    (u32)((char*)&updateMsg__5CViewFv + 0x138),
    (u32)((char*)&updateMsg__5CViewFv + 0x460),
    (u32)((char*)&updateMsg__5CViewFv + 0x6A0),
    (u32)((char*)&updateMsg__5CViewFv + 0x6E4),
    (u32)((char*)&updateMsg__5CViewFv + 0x6F4),
    (u32)((char*)&updateMsg__5CViewFv + 0x704),
    (u32)((char*)&updateMsg__5CViewFv + 0x714),
    (u32)((char*)&updateMsg__5CViewFv + 0x720),
};

/* 0x8056B5E0 | 0xD0 | CView complete vtable region (primary 0xA0 + secondary
 * 0x30 for the CFontLayer subobject at +0x1C4). */
CViewVtbls lbl_eu_8056B5E0 = {
    { &lbl_eu_80663578, 0,
      &__dt__5CViewFv,
      IWEVTBL_EVENTS,
      &wkUpdate__5CViewFv,
      &wkRender__11CWorkThreadFv,
      &wkRenderAfter__11CWorkThreadFv,
      &wkStandbyLogin__5CViewFv,
      &wkStandbyLogout__5CViewFv,
      &wkStandbyExceptionRetry__11CWorkThreadFUl },
    { &lbl_eu_80663578, 0xFFFFFE3C,
      &func_8043FBC4,
      &CView_UnkVirtualFunc1__5CViewFv,
      &detachRenderWork__5CViewFP11CWorkThread,
      &CView_UnkVirtualFunc3__5CViewFv,
      &CView_UnkVirtualFunc4__5CViewFv,
      &CView_UnkVirtualFunc5__5CViewFv,
      &CView_UnkVirtualFunc6__5CViewFv,
      &CView_UnkVirtualFunc7__5CViewFv,
      &CView_UnkVirtualFunc8__5CViewFv,
      &CView_UnkVirtualFunc9__5CViewFv },
};

/* 0x8056B6B0 | 0x1C | CView SI descriptor (7 words). */
struct RttiRef7 {
    void* t0; u32 z0;
    void* rttiIWorkEvent; u32 z1;
    void* rttiCWorkThread; u32 z2;
    u32   z3;
};
RttiRef7 lbl_eu_8056B6B0 = {
    &lbl_eu_80663598, 0,
    &__RTTI__10IWorkEvent, 0,
    &__RTTI__11CWorkThread, 0,
    0,
};

/* 0x8056B6CC | 0xC | CMsgParam<10> vtable; dtor is a template name. */
SmallVtbl3 lbl_eu_8056B6CC = {
    &lbl_eu_80663580, 0,
    0,                     /* __dt__13CMsgParam<10>Fv -- template name, unnameable */
};

/* 0x8056B6D8 | 0xC | reslist<IWorkEvent*> vtable; dtor is a template name. */
SmallVtbl3 lbl_eu_8056B6D8 = {
    &lbl_eu_80663588, 0,
    0,                     /* __dt__22reslist<P10IWorkEvent>Fv -- template name */
};

/* 0x8056B6E4 | 0xC | 3-word blob, dtor slot unused. */
SmallVtbl3 lbl_eu_8056B6E4 = {
    &lbl_eu_80663590, 0,
    0,
};

/* 0x8056B6F0 | 0x10 | _reslist_base<IWorkEvent*> vtable; dtor is a template
 * name, second slot unused. */
SmallVtbl4 lbl_eu_8056B6F0 = {
    &lbl_eu_80663590, 0,
    0,                     /* __dt__28_reslist_base<P10IWorkEvent>Fv -- template name */
    0,
};

/* 0x8056B700 | 0x10 | CViewFrame vtable. */
SmallVtbl4 lbl_eu_8056B700 = {
    &lbl_eu_806635A0, 0,
    &__dt__10CViewFrameFv,
    0,
};

/* 0x8056B710 | 0xA0 | CViewRoot vtable (wkUpdate NOT overridden). */
IWorkEventVtbl lbl_eu_8056B710 = {
    &lbl_eu_806635A8, 0,
    &__dt__9CViewRootFv,
    IWEVTBL_EVENTS,
    &wkUpdate__11CWorkThreadFv,
    &wkRender__11CWorkThreadFv,
    &wkRenderAfter__11CWorkThreadFv,
    &wkStandbyLogin__9CViewRootFv,
    &wkStandbyLogout__9CViewRootFv,
    &wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* 0x8056B7B0 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056B7B0 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056B7C8 | 0xA0 | CWorkControl vtable. */
IWorkEventVtbl lbl_eu_8056B7C8 = {
    &lbl_eu_806635B0, 0,
    &__dt__12CWorkControlFv,
    IWEVTBL_EVENTS,
    &wkUpdate__11CWorkThreadFv,
    &wkRender__11CWorkThreadFv,
    &wkRenderAfter__11CWorkThreadFv,
    &wkStandbyLogin__12CWorkControlFv,
    &wkStandbyLogout__12CWorkControlFv,
    &wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* 0x8056B868 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056B868 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056B880 | 0xA0 | CWorkFlowSetup vtable. */
IWorkEventVtbl lbl_eu_8056B880 = {
    &lbl_eu_806635B8, 0,
    &__dt__14CWorkFlowSetupFv,
    IWEVTBL_EVENTS,
    &wkUpdate__11CWorkThreadFv,
    &wkRender__11CWorkThreadFv,
    &wkRenderAfter__11CWorkThreadFv,
    &wkStandbyLogin__14CWorkFlowSetupFv,
    &wkStandbyLogout__14CWorkFlowSetupFv,
    &wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* 0x8056B920 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056B920 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056B938 | 0xA0 | CWorkRootThread vtable (anonymous namespace class in
 * CWorkRoot_cpp.cpp); dtor + wkStandbyLogout are @unnamed@ symbols. */
IWorkEventVtbl lbl_eu_8056B938 = {
    &lbl_eu_806635C0, 0,
    0,                     /* __dt__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThreadFv */
    IWEVTBL_EVENTS,
    &wkUpdate__11CWorkThreadFv,
    &wkRender__11CWorkThreadFv,
    &wkRenderAfter__11CWorkThreadFv,
    &wkStandbyLogin__11CWorkThreadFv,
    0,                     /* wkStandbyLogout__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThreadFv */
    &wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* 0x8056B9D8 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056B9D8 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056B9F0 | 0xA0 | CWorkSystem vtable (wkUpdate overridden). */
IWorkEventVtbl lbl_eu_8056B9F0 = {
    &lbl_eu_806635C8, 0,
    &__dt__11CWorkSystemFv,
    IWEVTBL_EVENTS,
    &wkUpdate__11CWorkSystemFv,
    &wkRender__11CWorkThreadFv,
    &wkRenderAfter__11CWorkThreadFv,
    &wkStandbyLogin__11CWorkSystemFv,
    &wkStandbyLogout__11CWorkSystemFv,
    &wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* 0x8056BA90 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056BA90 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056BAA8 | 0xA0 | CWorkSystemMem vtable (wkUpdate NOT overridden). */
IWorkEventVtbl lbl_eu_8056BAA8 = {
    &lbl_eu_806635D0, 0,
    &__dt__14CWorkSystemMemFv,
    IWEVTBL_EVENTS,
    &wkUpdate__11CWorkThreadFv,
    &wkRender__11CWorkThreadFv,
    &wkRenderAfter__11CWorkThreadFv,
    &wkStandbyLogin__14CWorkSystemMemFv,
    &wkStandbyLogout__14CWorkSystemMemFv,
    &wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* 0x8056BB48 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056BB48 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056BB60 | 0x24 | CProcess vtable (Init/Term/Move/Draw pure virtuals). */
CProcessVtbl lbl_eu_8056BB60 = {
    &lbl_eu_80661948, 0,
    &__dt__8CProcessFv,
    &Reset__14CChildListNodeFv,
    0,                     /* pure virtual Init */
    0,                     /* pure virtual Term */
    0,                     /* pure virtual Move */
    0,                     /* pure virtual Draw */
    &Tail__8CProcessFv,
};

/* 0x8056BB84 | 0xC | TChildListHeader<CProcess> vtable; dtor is a template
 * name. */
SmallVtbl3 lbl_eu_8056BB84 = {
    &lbl_eu_806635D8, 0,
    0,                     /* __dt__27TChildListHeader<8CProcess>Fv -- template name */
};

/* 0x8056BB90 | 0x10 | CDoubleListNode vtable. */
CNodeVtbl lbl_eu_8056BB90 = {
    &lbl_eu_80661958, 0,
    &__dt__15CDoubleListNodeFv,
    &Reset__15CDoubleListNodeFv,
};

/* 0x8056BBA0 | 0x10 | CChildListNode vtable. */
CNodeVtbl lbl_eu_8056BBA0 = {
    &lbl_eu_80661950, 0,
    &__dt__14CChildListNodeFv,
    &Reset__14CChildListNodeFv,
};

/* 0x8056BBB0 | 0x10 | TChildListHeader<CChildListNode> vtable; dtor is a
 * template name, second slot unused. */
SmallVtbl4 lbl_eu_8056BBB0 = {
    &lbl_eu_806635E0, 0,
    0,                     /* __dt__34TChildListHeader<14CChildListNode>Fv -- template name */
    0,
};

/* 0x8056BBC0 | 0xA0 | CDeviceRemotePad vtable. */
IWorkEventVtbl lbl_eu_8056BBC0 = {
    &lbl_eu_806635E8, 0,
    &__dt__16CDeviceRemotePadFv,
    IWEVTBL_EVENTS,
    &wkUpdate__16CDeviceRemotePadFv,
    &wkRender__11CWorkThreadFv,
    &wkRenderAfter__11CWorkThreadFv,
    &wkStandbyLogin__16CDeviceRemotePadFv,
    &wkStandbyLogout__16CDeviceRemotePadFv,
    &wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* 0x8056BC60 | 0x20 | RTTI descriptor (8 words, CDeviceBase family). */
RttiRef8 lbl_eu_8056BC60 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0,
                             &lbl_eu_806635F0, 0, 0, 0 };

/* 0x8056BC80 | 0xA0 | CDeviceBase vtable (retail symbol __vt__11CDeviceBase). */
IWorkEventVtbl __vt__11CDeviceBase = {
    &lbl_eu_806635F0, 0,
    &__dt__11CDeviceBaseFv,
    IWEVTBL_EVENTS,
    &wkUpdate__11CWorkThreadFv,
    &wkRender__11CWorkThreadFv,
    &wkRenderAfter__11CWorkThreadFv,
    &wkStandbyLogin__11CWorkThreadFv,
    &wkStandbyLogout__11CWorkThreadFv,
    &wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* 0x8056BD20 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056BD20 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056BD38 | 0xA0 | CDeviceSC vtable. */
IWorkEventVtbl lbl_eu_8056BD38 = {
    &lbl_eu_806635F8, 0,
    &__dt__9CDeviceSCFv,
    IWEVTBL_EVENTS,
    &wkUpdate__11CWorkThreadFv,
    &wkRender__11CWorkThreadFv,
    &wkRenderAfter__11CWorkThreadFv,
    &wkStandbyLogin__9CDeviceSCFv,
    &wkStandbyLogout__9CDeviceSCFv,
    &wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* 0x8056BDD8 | 0x20 | RTTI descriptor (8 words, CDeviceBase family). */
RttiRef8 lbl_eu_8056BDD8 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0,
                             &lbl_eu_806635F0, 0, 0, 0 };

/* ================================= .bss ====================================
 * Forward declaration order (MWCC emits .bss/.data forward).
 * 0x80656E88..0x806571F8 | 0x370 | align 8.
 */
class CRsrcData; /* fwd decl only: sRsrcPointerList is an array of pointers */

CRsrcData* sRsrcPointerList__5CRsrc[0x10]; /* 0x40 -- CRsrc::sRsrcPointerList */
float      lbl_8065A0C8[4];                /* 0x10 -- f32[4]                  */
float      sFrameColor__5CView[4];         /* 0x10 -- CView::sFrameColor (CCol4) */
u32        lbl_8065A0E8[4];                /* 0x10 -- unknown, u32[4]         */
u64        sErrorWii__9CWorkRoot[0x60];    /* 0x300 -- CWorkRoot::sErrorWii (CErrorWii) */

/* ================================= .sbss ===================================
 * REVERSE declaration order (MWCC emits .sbss in reverse of the source list;
 * see docs/MWCC_REFERENCE.md small-data section).  All symbols global.
 * 0x80665590..0x80665648 | 0xB8 | align 8.
 */
u64 lbl_eu_80665640;   /* 0x8 */
u64 lbl_eu_80665638;   /* 0x8 */
u64 lbl_eu_80665630;   /* 0x8 */
u64 lbl_eu_80665628;   /* 0x8 */
u64 lbl_eu_80665620;   /* 0x8 */
u32 lbl_eu_8066561C;   /* 0x4 */
u32 lbl_eu_80665618;   /* 0x4 */
u64 lbl_eu_80665610;   /* 0x8 */
u32 lbl_eu_8066560C;   /* 0x4 */
u32 spInstance_CWorkRootThread_unnamed; /* 0x4 -- retail name
    spInstance__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThread (not nameable) */
u64 lbl_eu_80665600;   /* 0x8 */
u64 lbl_eu_806655F8;   /* 0x8 */
u64 lbl_eu_806655F0;   /* 0x8 */
u64 lbl_eu_806655E8;   /* 0x8 */
u64 lbl_eu_806655E0;   /* 0x8 */
u64 lbl_eu_806655D8;   /* 0x8 */
u8  lbl_eu_806655D4;   /* 0x1 */
u32 lbl_eu_806655D0;   /* 0x4 */
u64 lbl_eu_806655C8;   /* 0x8 */
u32 lbl_eu_806655C4;   /* 0x4 */
u32 lbl_eu_806655C0;   /* 0x4 */
u32 lbl_eu_806655BC;   /* 0x4 */
u32 lbl_eu_806655B8;   /* 0x4 */
u64 lbl_eu_806655B0;   /* 0x8 */
u16 lbl_eu_806655A8;   /* 0x2 */
u32 spInstance__9CProcRoot; /* 0x4 -- CProcRoot::spInstance */
u32 lbl_eu_806655A0;   /* 0x4 */
u64 lbl_eu_80665598;   /* 0x8 */
u32 lbl_eu_80665594;   /* 0x4 */
u32 lbl_eu_80665590;   /* 0x4 */
