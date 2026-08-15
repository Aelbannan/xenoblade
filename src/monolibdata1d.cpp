/* Data-only TU for monolibdata1d (retail: build/us/obj/monolibdata1d.o).
 *
 * Sections: .data 0x89C (2204 B), .bss 0x370 (880 B), .sbss 0xB8 (184 B);
 * 468 relocs in .data (vtable / RTTI / typeinfo / jumptable references).
 *
 * The owning types are the monolib library classes (CView / CWorkThread /
 * IWorkEvent / CProcess / CDeviceBase / ...); this TU reproduces their
 * vtable / RTTI blobs as typed structs.  Function-pointer slots reference the
 * real methods through real declarations: each class is declared with its
 * retail method names and signatures, and the slot takes `&Class::method`.
 *
 * Declaration mechanism notes (verified against MWCC Wii/1.1):
 *  - MWCC rejects converting an instance-member pointer (pmf) to a plain
 *    function pointer, and rejects taking a destructor's address outright
 *    ("illegal pointer to constructor/destructor").  Instance methods are
 *    therefore declared as STATIC members here: a static member's address IS
 *    a plain function pointer, and MWCC mangles static members identically
 *    to instance members (verified: `static void CView::wkUpdate()` emits
 *    `wkUpdate__5CViewFv`).  The vtable slot content (a plain 4-byte
 *    address) is identical either way; only the port-side caller type
 *    changes, and the static declaration keeps the real parameter signature.
 *  - Parameter types MUST match the retail encoding (Ul vs Ui, P10CEventFile
 *    vs Pv): these change the mangled name.
 *  - Destructor slots (`__dt__5CViewFv`, ...) have no legal source-level
 *    address form in MWCC (see above); template-instantiation dtors
 *    (`__dt__13CMsgParam<10>Fv`, ...) additionally contain `<`/`>` in the
 *    symbol, which no identifier can express.  Those slots are literal 0
 *    with the retail name in a comment (bytes stay placeholder zeros, like
 *    retail); they are listed in the report as residuals.
 *
 * Residuals (reported to the coordinator; reloc drift in .data only):
 *  - 13 destructor slots (no MWCC address form exists).
 *  - 8 template-instantiation dtor slots (5 nameable templates + 3
 *    CTTask<@unnamed@ CRootProc>; names contain `<`/`>`/`@`).
 *  - 5 @unnamed@ anonymous-namespace slots (3 CTTask are double-counted
 *    above: `Move/Draw/dt__48CTTask<Q226@unnamed@...>`; plus 2
 *    CWorkRootThread) -- the mangle embeds the owning file basename.
 *  - .data sh_addralign: retail split object has 4 (dtk derives it from the
 *    DOL base 0x8056B55C); MWCC always emits 8 for .data.  Byte content is
 *    identical; only the section alignment metadata differs.
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long ulong;   /* MWCC mangles `Ul` (retail uses Ul) */
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

/* ================== real method declarations (owning classes) ==============
 * Minimal class declarations carrying the retail method names and signatures.
 * Methods are declared static so `&Class::method` is a plain 4-byte function
 * pointer (MWCC emits instance-method addresses as pointer-to-member pairs,
 * which cannot fill a vtable slot).  The mangled name is identical to the
 * retail instance-method symbol (verified per method below).
 */
class CWorkThread; /* used as a signature type (P11CWorkThread) */
class CEventFile;  /* used as a signature type (P10CEventFile)  */

class IWorkEvent {
public:
    static bool WorkEvent1(void* r4, const char* r5); /* WorkEvent1__10IWorkEventFPvPCc */
    static bool OnFileEvent(CEventFile* pEventFile);  /* OnFileEvent__10IWorkEventFP10CEventFile */
    static bool WorkEvent3(void* r4);                 /* WorkEvent3__10IWorkEventFPv */
    static bool WorkEvent4();                         /* WorkEvent4__10IWorkEventFv */
    static void OnPauseTrigger(bool paused);          /* OnPauseTrigger__10IWorkEventFb */
    static bool WorkEvent6();
    static bool WorkEvent7();
    static bool WorkEvent8();
    static bool WorkEvent9();
    static bool WorkEvent10();
    static bool WorkEvent11();
    static bool WorkEvent12();
    static bool WorkEvent13();
    static bool WorkEvent14();
    static bool WorkEvent15();
    static bool WorkEvent16();
    static bool WorkEvent17();
    static bool WorkEvent18();
    static bool WorkEvent19();
    static bool WorkEvent20();
    static bool WorkEvent21();
    static bool WorkEvent22();
    static bool WorkEvent23();
    static bool WorkEvent24();
    static bool WorkEvent25();
    static bool WorkEvent26();
    static bool WorkEvent27();
    static bool WorkEvent28();
    static bool WorkEvent29();
    static bool WorkEvent30();
    static void WorkEvent31();
};

class CWorkThread {
public:
    static void wkUpdate();                          /* wkUpdate__11CWorkThreadFv */
    static void wkRender();                          /* wkRender__11CWorkThreadFv */
    static void wkRenderAfter();                     /* wkRenderAfter__11CWorkThreadFv */
    static bool wkStandbyLogin();                    /* wkStandbyLogin__11CWorkThreadFv */
    static bool wkStandbyLogout();                   /* wkStandbyLogout__11CWorkThreadFv */
    static bool wkStandbyExceptionRetry(ulong wid);  /* wkStandbyExceptionRetry__11CWorkThreadFUl */
};

class CView {
public:
    static void wkUpdate();                          /* wkUpdate__5CViewFv */
    static bool wkStandbyLogin();                    /* wkStandbyLogin__5CViewFv */
    static bool wkStandbyLogout();                   /* wkStandbyLogout__5CViewFv */
    static void CView_UnkVirtualFunc1();             /* CView_UnkVirtualFunc1__5CViewFv */
    static void detachRenderWork(CWorkThread* pThread); /* detachRenderWork__5CViewFP11CWorkThread */
    static void CView_UnkVirtualFunc3();
    static void CView_UnkVirtualFunc4();
    static void CView_UnkVirtualFunc5();
    static void CView_UnkVirtualFunc6();
    static void CView_UnkVirtualFunc7();
    static void CView_UnkVirtualFunc8();
    static void CView_UnkVirtualFunc9();
    static void updateMsg();                         /* updateMsg__5CViewFv */
};

class CViewRoot {
public:
    static bool wkStandbyLogin();                    /* wkStandbyLogin__9CViewRootFv */
    static bool wkStandbyLogout();                   /* wkStandbyLogout__9CViewRootFv */
};

class CWorkControl {
public:
    static bool wkStandbyLogin();                    /* wkStandbyLogin__12CWorkControlFv */
    static bool wkStandbyLogout();                   /* wkStandbyLogout__12CWorkControlFv */
};

class CWorkFlowSetup {
public:
    static bool wkStandbyLogin();                    /* wkStandbyLogin__14CWorkFlowSetupFv */
    static bool wkStandbyLogout();                   /* wkStandbyLogout__14CWorkFlowSetupFv */
};

class CWorkSystem {
public:
    static void wkUpdate();                          /* wkUpdate__11CWorkSystemFv */
    static bool wkStandbyLogin();                    /* wkStandbyLogin__11CWorkSystemFv */
    static bool wkStandbyLogout();                   /* wkStandbyLogout__11CWorkSystemFv */
};

class CWorkSystemMem {
public:
    static bool wkStandbyLogin();                    /* wkStandbyLogin__14CWorkSystemMemFv */
    static bool wkStandbyLogout();                   /* wkStandbyLogout__14CWorkSystemMemFv */
};

class CDeviceRemotePad {
public:
    static void wkUpdate();                          /* wkUpdate__16CDeviceRemotePadFv */
    static bool wkStandbyLogin();                    /* wkStandbyLogin__16CDeviceRemotePadFv */
    static bool wkStandbyLogout();                   /* wkStandbyLogout__16CDeviceRemotePadFv */
};

class CDeviceSC {
public:
    static bool wkStandbyLogin();                    /* wkStandbyLogin__9CDeviceSCFv */
    static bool wkStandbyLogout();                   /* wkStandbyLogout__9CDeviceSCFv */
};

class CProcess {
public:
    static void Tail();                              /* Tail__8CProcessFv */
};

class CChildListNode {
public:
    static void Reset();                             /* Reset__14CChildListNodeFv */
};

class CDoubleListNode {
public:
    static void Reset();                             /* Reset__15CDoubleListNodeFv */
};

/* Real C-linkage function (unmangled C symbol in the retail image). */
extern "C" void func_8043FBC4();

/* ============================ vtable shapes ================================= */

/* Shared 40-word (0xA0) primary vtable of every CWorkThread-derived class:
 * [typeinfo, offset-to-top, ~dt, IWorkEvent::WorkEvent1..31, wk* overrides]. */
struct IWorkEventVtbl {
    void* typeinfo;                  /* SI chain label (lbl_eu_8066xxxx)  */
    u32   offsetToTop;               /* 0 for primary vtables             */
    void  (*dt)();                   /* dtor (untypeable in MWCC -> 0)    */
    bool  (*workEvent1)(void*, const char*);
    bool  (*onFileEvent)(CEventFile*);
    bool  (*workEvent3)(void*);
    bool  (*workEvent4)();
    void  (*onPauseTrigger)(bool);
    bool  (*workEvent6)();
    bool  (*workEvent7)();
    bool  (*workEvent8)();
    bool  (*workEvent9)();
    bool  (*workEvent10)();
    bool  (*workEvent11)();
    bool  (*workEvent12)();
    bool  (*workEvent13)();
    bool  (*workEvent14)();
    bool  (*workEvent15)();
    bool  (*workEvent16)();
    bool  (*workEvent17)();
    bool  (*workEvent18)();
    bool  (*workEvent19)();
    bool  (*workEvent20)();
    bool  (*workEvent21)();
    bool  (*workEvent22)();
    bool  (*workEvent23)();
    bool  (*workEvent24)();
    bool  (*workEvent25)();
    bool  (*workEvent26)();
    bool  (*workEvent27)();
    bool  (*workEvent28)();
    bool  (*workEvent29)();
    bool  (*workEvent30)();
    void  (*workEvent31)();
    void  (*wkUpdate)();
    void  (*wkRender)();
    void  (*wkRenderAfter)();
    bool  (*wkStandbyLogin)();
    bool  (*wkStandbyLogout)();
    bool  (*wkStandbyExceptionRetry)(ulong);
};

/* IWorkEvent event slots 1..31, shared verbatim by every 40-word vtable. */
#define IWEVTBL_EVENTS                                                          \
    &IWorkEvent::WorkEvent1,                                                    \
    &IWorkEvent::OnFileEvent,                                                   \
    &IWorkEvent::WorkEvent3,                                                    \
    &IWorkEvent::WorkEvent4,                                                    \
    &IWorkEvent::OnPauseTrigger,                                                \
    &IWorkEvent::WorkEvent6,                                                    \
    &IWorkEvent::WorkEvent7,                                                    \
    &IWorkEvent::WorkEvent8,                                                    \
    &IWorkEvent::WorkEvent9,                                                    \
    &IWorkEvent::WorkEvent10,                                                   \
    &IWorkEvent::WorkEvent11,                                                   \
    &IWorkEvent::WorkEvent12,                                                   \
    &IWorkEvent::WorkEvent13,                                                   \
    &IWorkEvent::WorkEvent14,                                                   \
    &IWorkEvent::WorkEvent15,                                                   \
    &IWorkEvent::WorkEvent16,                                                   \
    &IWorkEvent::WorkEvent17,                                                   \
    &IWorkEvent::WorkEvent18,                                                   \
    &IWorkEvent::WorkEvent19,                                                   \
    &IWorkEvent::WorkEvent20,                                                   \
    &IWorkEvent::WorkEvent21,                                                   \
    &IWorkEvent::WorkEvent22,                                                   \
    &IWorkEvent::WorkEvent23,                                                   \
    &IWorkEvent::WorkEvent24,                                                   \
    &IWorkEvent::WorkEvent25,                                                   \
    &IWorkEvent::WorkEvent26,                                                   \
    &IWorkEvent::WorkEvent27,                                                   \
    &IWorkEvent::WorkEvent28,                                                   \
    &IWorkEvent::WorkEvent29,                                                   \
    &IWorkEvent::WorkEvent30,                                                   \
    &IWorkEvent::WorkEvent31

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
    void  (*f2)(CWorkThread*);      /* detachRenderWork                     */
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
 * Draw are @unnamed@ symbols from CTaskManager_cpp.cpp -- not reproducible
 * in this TU. */
CProcessVtbl lbl_eu_8056B580 = {
    &lbl_eu_80663570, 0,
    0,   /* UNRESOLVED: __dt__48CTTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>Fv (owning file: CTaskManager_cpp.cpp) */
    &CChildListNode::Reset,
    0,   /* pure virtual Init */
    0,   /* pure virtual Term */
    0,   /* UNRESOLVED: Move__48CTTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>Fv (owning file: CTaskManager_cpp.cpp) */
    0,   /* UNRESOLVED: Draw__48CTTask<Q226@unnamed@CTaskManager_cpp@9CRootProc>Fv (owning file: CTaskManager_cpp.cpp) */
    &CProcess::Tail,
};

/* 0x8056B5A4 | 0x3C | three (typeinfo, 0) pairs + trailing 0, immediately
 * followed by CView::updateMsg's switch jump table (retail symbols
 * lbl_eu_8056B5A4 + jumptable_eu_8056B5C0).  Declared as one object because
 * MWCC 8-aligns any top-level array whose first member is >= 16 bytes; the
 * merged object's first member is a pointer, so the whole block stays
 * 4-aligned exactly like retail (verified: {ptr,...} first-member structs
 * keep 4-alignment even with a trailing u32[8]). */
struct TypeinfoTripleAndJumptable {
    void* t0; u32 z0;
    void* t1; u32 z1;
    void* t2; u32 z2;
    u32   tail;
    u32   jt[8];
};
TypeinfoTripleAndJumptable lbl_eu_8056B5A4 = {
    &lbl_eu_80661958, 0,
    &lbl_eu_80661950, 0,
    &lbl_eu_80661948, 0,
    0,
    {   /* jumptable_eu_8056B5C0: updateMsg relocs w/ addends */
        (u32)((char*)&CView::updateMsg + 0x138),
        (u32)((char*)&CView::updateMsg + 0x460),
        (u32)((char*)&CView::updateMsg + 0x6A0),
        (u32)((char*)&CView::updateMsg + 0x6E4),
        (u32)((char*)&CView::updateMsg + 0x6F4),
        (u32)((char*)&CView::updateMsg + 0x704),
        (u32)((char*)&CView::updateMsg + 0x714),
        (u32)((char*)&CView::updateMsg + 0x720),
    },
};

/* 0x8056B5E0 | 0xD0 | CView complete vtable region (primary 0xA0 + secondary
 * 0x30 for the CFontLayer subobject at +0x1C4). */
CViewVtbls lbl_eu_8056B5E0 = {
    { &lbl_eu_80663578, 0,
      0,   /* UNRESOLVED dtor: __dt__5CViewFv (MWCC rejects dtor addresses) */
      IWEVTBL_EVENTS,
      &CView::wkUpdate,
      &CWorkThread::wkRender,
      &CWorkThread::wkRenderAfter,
      &CView::wkStandbyLogin,
      &CView::wkStandbyLogout,
      &CWorkThread::wkStandbyExceptionRetry },
    { &lbl_eu_80663578, 0xFFFFFE3C,
      &func_8043FBC4,
      &CView::CView_UnkVirtualFunc1,
      &CView::detachRenderWork,
      &CView::CView_UnkVirtualFunc3,
      &CView::CView_UnkVirtualFunc4,
      &CView::CView_UnkVirtualFunc5,
      &CView::CView_UnkVirtualFunc6,
      &CView::CView_UnkVirtualFunc7,
      &CView::CView_UnkVirtualFunc8,
      &CView::CView_UnkVirtualFunc9 },
};

/* 0x8056B6B0 | 0x1C | CView SI descriptor (7 words). */
struct RttiRef7 {
    void* t0; u32 z0;
    void* rttiIWorkEvent; u32 z1;
    void* rttiCWorkThread; u32 z2;
    u32   z3;
};
RttiRef7 lbl_eu_8056B6B0 = {
    &lbl_eu_80663598, 0x000001C4,   /* 0x1C4: CFontLayer subobject offset in CView */
    &__RTTI__10IWorkEvent, 0,
    &__RTTI__11CWorkThread, 0,
    0,
};

/* 0x8056B6CC | 0xC | CMsgParam<10> vtable; dtor is a template instantiation
 * (symbol contains `<`/`>` -- not expressible). */
SmallVtbl3 lbl_eu_8056B6CC = {
    &lbl_eu_80663580, 0,
    0,   /* UNRESOLVED: __dt__13CMsgParam<10>Fv (template name) */
};

/* 0x8056B6D8 | 0xC | reslist<IWorkEvent*> vtable; dtor is a template
 * instantiation. */
SmallVtbl3 lbl_eu_8056B6D8 = {
    &lbl_eu_80663588, 0,
    0,   /* UNRESOLVED: __dt__22reslist<P10IWorkEvent>Fv (template name) */
};

/* 0x8056B6E4 | 0xC | 3-word blob, dtor slot unused. */
SmallVtbl3 lbl_eu_8056B6E4 = {
    &lbl_eu_80663590, 0,
    0,
};

/* 0x8056B6F0 | 0x10 | _reslist_base<IWorkEvent*> vtable; dtor is a template
 * instantiation, second slot unused. */
SmallVtbl4 lbl_eu_8056B6F0 = {
    &lbl_eu_80663590, 0,
    0,   /* UNRESOLVED: __dt__28_reslist_base<P10IWorkEvent>Fv (template name) */
    0,
};

/* 0x8056B700 | 0x10 | CViewFrame vtable. */
SmallVtbl4 lbl_eu_8056B700 = {
    &lbl_eu_806635A0, 0,
    0,   /* UNRESOLVED dtor: __dt__10CViewFrameFv (MWCC rejects dtor addresses) */
    0,
};

/* 0x8056B710 | 0xA0 | CViewRoot vtable (wkUpdate NOT overridden). */
IWorkEventVtbl lbl_eu_8056B710 = {
    &lbl_eu_806635A8, 0,
    0,   /* UNRESOLVED dtor: __dt__9CViewRootFv (MWCC rejects dtor addresses) */
    IWEVTBL_EVENTS,
    &CWorkThread::wkUpdate,
    &CWorkThread::wkRender,
    &CWorkThread::wkRenderAfter,
    &CViewRoot::wkStandbyLogin,
    &CViewRoot::wkStandbyLogout,
    &CWorkThread::wkStandbyExceptionRetry,
};

/* 0x8056B7B0 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056B7B0 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056B7C8 | 0xA0 | CWorkControl vtable. */
IWorkEventVtbl lbl_eu_8056B7C8 = {
    &lbl_eu_806635B0, 0,
    0,   /* UNRESOLVED dtor: __dt__12CWorkControlFv (MWCC rejects dtor addresses) */
    IWEVTBL_EVENTS,
    &CWorkThread::wkUpdate,
    &CWorkThread::wkRender,
    &CWorkThread::wkRenderAfter,
    &CWorkControl::wkStandbyLogin,
    &CWorkControl::wkStandbyLogout,
    &CWorkThread::wkStandbyExceptionRetry,
};

/* 0x8056B868 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056B868 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056B880 | 0xA0 | CWorkFlowSetup vtable. */
IWorkEventVtbl lbl_eu_8056B880 = {
    &lbl_eu_806635B8, 0,
    0,   /* UNRESOLVED dtor: __dt__14CWorkFlowSetupFv (MWCC rejects dtor addresses) */
    IWEVTBL_EVENTS,
    &CWorkThread::wkUpdate,
    &CWorkThread::wkRender,
    &CWorkThread::wkRenderAfter,
    &CWorkFlowSetup::wkStandbyLogin,
    &CWorkFlowSetup::wkStandbyLogout,
    &CWorkThread::wkStandbyExceptionRetry,
};

/* 0x8056B920 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056B920 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056B938 | 0xA0 | CWorkRootThread vtable (anonymous namespace class in
 * CWorkRoot_cpp.cpp); dtor + wkStandbyLogout are @unnamed@ symbols. */
IWorkEventVtbl lbl_eu_8056B938 = {
    &lbl_eu_806635C0, 0,
    0,   /* UNRESOLVED @unnamed@: __dt__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThreadFv (owning file: CWorkRoot_cpp.cpp) */
    IWEVTBL_EVENTS,
    &CWorkThread::wkUpdate,
    &CWorkThread::wkRender,
    &CWorkThread::wkRenderAfter,
    &CWorkThread::wkStandbyLogin,
    0,   /* UNRESOLVED @unnamed@: wkStandbyLogout__Q223@unnamed@CWorkRoot_cpp@15CWorkRootThreadFv (owning file: CWorkRoot_cpp.cpp) */
    &CWorkThread::wkStandbyExceptionRetry,
};

/* 0x8056B9D8 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056B9D8 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056B9F0 | 0xA0 | CWorkSystem vtable (wkUpdate overridden). */
IWorkEventVtbl lbl_eu_8056B9F0 = {
    &lbl_eu_806635C8, 0,
    0,   /* UNRESOLVED dtor: __dt__11CWorkSystemFv (MWCC rejects dtor addresses) */
    IWEVTBL_EVENTS,
    &CWorkSystem::wkUpdate,
    &CWorkThread::wkRender,
    &CWorkThread::wkRenderAfter,
    &CWorkSystem::wkStandbyLogin,
    &CWorkSystem::wkStandbyLogout,
    &CWorkThread::wkStandbyExceptionRetry,
};

/* 0x8056BA90 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056BA90 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056BAA8 | 0xA0 | CWorkSystemMem vtable (wkUpdate NOT overridden). */
IWorkEventVtbl lbl_eu_8056BAA8 = {
    &lbl_eu_806635D0, 0,
    0,   /* UNRESOLVED dtor: __dt__14CWorkSystemMemFv (MWCC rejects dtor addresses) */
    IWEVTBL_EVENTS,
    &CWorkThread::wkUpdate,
    &CWorkThread::wkRender,
    &CWorkThread::wkRenderAfter,
    &CWorkSystemMem::wkStandbyLogin,
    &CWorkSystemMem::wkStandbyLogout,
    &CWorkThread::wkStandbyExceptionRetry,
};

/* 0x8056BB48 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056BB48 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056BB60 | 0x24 | CProcess vtable (Init/Term/Move/Draw pure virtuals). */
CProcessVtbl lbl_eu_8056BB60 = {
    &lbl_eu_80661948, 0,
    0,   /* UNRESOLVED dtor: __dt__8CProcessFv (MWCC rejects dtor addresses) */
    &CChildListNode::Reset,
    0,   /* pure virtual Init */
    0,   /* pure virtual Term */
    0,   /* pure virtual Move */
    0,   /* pure virtual Draw */
    &CProcess::Tail,
};

/* 0x8056BB84 | 0xC | TChildListHeader<CProcess> vtable; dtor is a template
 * instantiation. */
SmallVtbl3 lbl_eu_8056BB84 = {
    &lbl_eu_806635D8, 0,
    0,   /* UNRESOLVED: __dt__27TChildListHeader<8CProcess>Fv (template name) */
};

/* 0x8056BB90 | 0x10 | CDoubleListNode vtable. */
CNodeVtbl lbl_eu_8056BB90 = {
    &lbl_eu_80661958, 0,
    0,   /* UNRESOLVED dtor: __dt__15CDoubleListNodeFv (MWCC rejects dtor addresses) */
    &CDoubleListNode::Reset,
};

/* 0x8056BBA0 | 0x10 | CChildListNode vtable. */
CNodeVtbl lbl_eu_8056BBA0 = {
    &lbl_eu_80661950, 0,
    0,   /* UNRESOLVED dtor: __dt__14CChildListNodeFv (MWCC rejects dtor addresses) */
    &CChildListNode::Reset,
};

/* 0x8056BBB0 | 0x10 | TChildListHeader<CChildListNode> vtable; dtor is a
 * template instantiation, second slot unused. */
SmallVtbl4 lbl_eu_8056BBB0 = {
    &lbl_eu_806635E0, 0,
    0,   /* UNRESOLVED: __dt__34TChildListHeader<14CChildListNode>Fv (template name) */
    0,
};

/* 0x8056BBC0 | 0xA0 | CDeviceRemotePad vtable. */
IWorkEventVtbl lbl_eu_8056BBC0 = {
    &lbl_eu_806635E8, 0,
    0,   /* UNRESOLVED dtor: __dt__16CDeviceRemotePadFv (MWCC rejects dtor addresses) */
    IWEVTBL_EVENTS,
    &CDeviceRemotePad::wkUpdate,
    &CWorkThread::wkRender,
    &CWorkThread::wkRenderAfter,
    &CDeviceRemotePad::wkStandbyLogin,
    &CDeviceRemotePad::wkStandbyLogout,
    &CWorkThread::wkStandbyExceptionRetry,
};

/* 0x8056BC60 | 0x20 | RTTI descriptor (8 words, CDeviceBase family). */
RttiRef8 lbl_eu_8056BC60 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0,
                             &lbl_eu_806635F0, 0, 0, 0 };

/* 0x8056BC80 | 0xA0 | CDeviceBase vtable (retail symbol __vt__11CDeviceBase). */
IWorkEventVtbl __vt__11CDeviceBase = {
    &lbl_eu_806635F0, 0,
    0,   /* UNRESOLVED dtor: __dt__11CDeviceBaseFv (MWCC rejects dtor addresses) */
    IWEVTBL_EVENTS,
    &CWorkThread::wkUpdate,
    &CWorkThread::wkRender,
    &CWorkThread::wkRenderAfter,
    &CWorkThread::wkStandbyLogin,
    &CWorkThread::wkStandbyLogout,
    &CWorkThread::wkStandbyExceptionRetry,
};

/* 0x8056BD20 | 0x18 | RTTI descriptor. */
RttiRef6 lbl_eu_8056BD20 = { &__RTTI__10IWorkEvent, 0, &__RTTI__11CWorkThread, 0, 0, 0 };

/* 0x8056BD38 | 0xA0 | CDeviceSC vtable. */
IWorkEventVtbl lbl_eu_8056BD38 = {
    &lbl_eu_806635F8, 0,
    0,   /* UNRESOLVED dtor: __dt__9CDeviceSCFv (MWCC rejects dtor addresses) */
    IWEVTBL_EVENTS,
    &CWorkThread::wkUpdate,
    &CWorkThread::wkRender,
    &CWorkThread::wkRenderAfter,
    &CDeviceSC::wkStandbyLogin,
    &CDeviceSC::wkStandbyLogout,
    &CWorkThread::wkStandbyExceptionRetry,
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
