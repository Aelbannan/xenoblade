#pragma once

/* Retail-named data symbol declarations for the monolibdata1d data blob
 * dissolve. The blob's .data objects (class vtables, RTTI base lists, and the
 * small reslist/CMsgParam vtables) are defined in their owning TU's source as
 * explicit structs of (u32)&target words so the compiled object reproduces the
 * retail bytes and reloc names exactly.
 *
 * MWCC never mangles global-scope data names, and `extern "C"` function
 * declarations keep the parameter-encoded suffixes from being appended, so
 * every (u32)&target below emits the exact retail reloc name.
 */

#include <types.h>

// RTTI/typeinfo objects and name strings referenced by the vtables. These are
// the retail .sdata/.rodata objects (owned by the same TUs; their split ranges
// move over with the coordinator's monolibdata1.s dissolve). Each typeinfo is
// an 8-byte { class-name, cast-table } pair; declared as arrays so the owning
// TUs can define them (`u32 x[2] = {...}`) without a scalar/array conflict.
extern "C" u32 lbl_eu_80663570[2];   // RTTI CTTask<CRootProc>
extern "C" u32 lbl_eu_80663578[2];   // RTTI CView
extern "C" u32 lbl_eu_80663580[2];   // RTTI CMsgParam<10>
extern "C" u32 lbl_eu_80663588[2];   // RTTI reslist<IWorkEvent*>
extern "C" u32 lbl_eu_80663590[2];   // RTTI _reslist_base<IWorkEvent*>
extern "C" u32 lbl_eu_80663598[2];   // RTTI CFontLayer
extern "C" u32 lbl_eu_806635A0[2];   // RTTI CViewFrame
extern "C" u32 lbl_eu_806635A8[2];   // RTTI CViewRoot
extern "C" u32 lbl_eu_806635B0[2];   // RTTI CWorkControl
extern "C" u32 lbl_eu_806635B8[2];   // RTTI CWorkFlowSetup
extern "C" u32 lbl_eu_806635C0[2];   // RTTI CWorkRootThread
extern "C" u32 lbl_eu_806635C8[2];   // RTTI CWorkSystem
extern "C" u32 lbl_eu_806635D0[2];   // RTTI CWorkSystemMem
extern "C" u32 lbl_eu_806635D8[2];   // RTTI TChildListHeader<CProcess>
extern "C" u32 lbl_eu_806635E0[2];   // RTTI TChildListHeader<CChildListNode>
extern "C" u32 lbl_eu_806635E8[2];   // RTTI CDeviceRemotePad
extern "C" u32 lbl_eu_806635F0[2];   // RTTI CDeviceBase
extern "C" u32 lbl_eu_806635F8[2];   // RTTI CDeviceSC
extern "C" u32 lbl_eu_80661948[2];   // RTTI CProcess
extern "C" u32 lbl_eu_80661950[2];   // RTTI CChildListNode
extern "C" u32 lbl_eu_80661958[2];   // RTTI CDoubleListNode
extern "C" void* __RTTI__10IWorkEvent;
extern "C" void* __RTTI__11CWorkThread;

// IWorkEvent virtual handlers (weak defaults live in kyoshin/CGame.cpp).
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
// CWorkThread work-method vtable slots.
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned int);

// CWorkThread-slot overrides referenced by the per-class vtables.
extern "C" void wkStandbyLogin__5CViewFv();
extern "C" void wkStandbyLogout__5CViewFv();
extern "C" void wkUpdate__5CViewFv();
extern "C" void __dt__5CViewFv(void*);
extern "C" void wkStandbyLogin__9CViewRootFv();
extern "C" void wkStandbyLogout__9CViewRootFv();
extern "C" void __dt__9CViewRootFv();
extern "C" void wkStandbyLogin__12CWorkControlFv();
extern "C" void wkStandbyLogout__12CWorkControlFv();
extern "C" void __dt__12CWorkControlFv();
extern "C" void wkStandbyLogin__14CWorkFlowSetupFv();
extern "C" void wkStandbyLogout__14CWorkFlowSetupFv();
extern "C" void __dt__14CWorkFlowSetupFv();
extern "C" void wkUpdate__11CWorkSystemFv();
extern "C" void wkStandbyLogin__11CWorkSystemFv();
extern "C" void wkStandbyLogout__11CWorkSystemFv();
extern "C" void __dt__11CWorkSystemFv();
extern "C" void wkStandbyLogin__14CWorkSystemMemFv();
extern "C" void wkStandbyLogout__14CWorkSystemMemFv();
extern "C" void __dt__14CWorkSystemMemFv();
extern "C" void wkStandbyLogin__16CDeviceRemotePadFv();
extern "C" void wkStandbyLogout__16CDeviceRemotePadFv();
extern "C" void wkUpdate__16CDeviceRemotePadFv();
extern "C" void __dt__16CDeviceRemotePadFv();
extern "C" void wkStandbyLogin__9CDeviceSCFv();
extern "C" void wkStandbyLogout__9CDeviceSCFv();
extern "C" void __dt__9CDeviceSCFv();
extern "C" void wkStandbyLogin__11CDeviceBaseFv();
extern "C" void wkStandbyLogout__11CDeviceBaseFv();
extern "C" void __dt__11CDeviceBaseFv();
// (void*, int) matches the canonical declaration in lib/CLibVM.hpp; the
// 0-arg form conflicts under C linkage when a TU includes both headers.
extern "C" void __dt__11CWorkThreadFv(void*, int);

// CProcess-family vtables.
extern "C" void __dt__8CProcessFv();
extern "C" void Reset__14CChildListNodeFv();
extern "C" void Tail__8CProcessFv();
extern "C" void __dt__15CDoubleListNodeFv();
extern "C" void Reset__15CDoubleListNodeFv();
extern "C" void __dt__14CChildListNodeFv();
extern "C" void* __dt__10CViewFrameFv(void*, int);

// IWorkEvent-derived class vtable (0xA0): [rtti, 0, dtor, 32 handlers,
// wkUpdate, wkRender, wkRenderAfter, wkStandbyLogin, wkStandbyLogout,
// wkStandbyExceptionRetry].
struct IWorkEventVtbl {
    u32 rtti;
    u32 pad0;
    u32 dtor;
    u32 we1;
    u32 ofe;
    u32 we3;
    u32 we4;
    u32 opt;
    u32 we6, we7, we8, we9, we10, we11, we12, we13, we14, we15, we16, we17;
    u32 we18, we19, we20, we21, we22, we23, we24, we25, we26, we27, we28, we29, we30;
    u32 we31;
    u32 wkupd;
    u32 wkr;
    u32 wkra;
    u32 wksl;
    u32 wkso;
    u32 wkser;
};

// RTTI base-list tables: [RTTI(base1), offset1, RTTI(base2), offset2, ..., 0].
// Single-inheritance chains list every ancestor (direct + indirect).
struct RttiBaseList2 {
    u32 r1, o1, r2, o2, t0, t1;   // [r,0, r,0, 0,0] - 0x18
};
struct RttiBaseList1 {
    u32 r1, o1, r2, o2, t0;       // [r,0, r,0, 0]   - 0x14
};
struct RttiBaseListCView {
    u32 r1, o1, r2, o2, r3, o3, t0;  // [r,0x1C4, r,0, r,0, 0] - 0x1C
};
