/*
 * Data-only TU for monolibdata1 (retail split object build/us/obj/monolibdata1.o).
 * Replaces the generated monolibdata1.s dump.
 *
 * Sections (retail): .rodata 0x57A | .data 0x458 | .bss 0x2CD0 | .sdata 0x100
 *                    .sbss 0x2D | .sdata2 0x1E8 | .sbss2 0x4  (all align 8)
 *
 * Typing notes
 * ------------
 * - .rodata: class-name/format strings + a few u32 lookup tables. String
 *   objects are declared with their exact retail sizes (MWCC 4-aligns .rodata
 *   char arrays by default; three objects need `#pragma align 8` to
 *   reproduce retail gaps). MWCC counts a string literal's characters plus an
 *   implicit terminator, so literals omit the retail's final NUL byte.
 *   Two 2-byte strings ("." and " ") are folded into neighbouring objects
 *   because a standalone const char[2] would land in .sdata2, not .rodata.
 * - .data: vtables (40-slot IWorkEvent-derived layout), 3-slot type-info
 *   records (RTTI ptr, 0, dtor), RTTI base-chain records and two switch jump
 *   tables. Function slots reference the retail-mangled extern "C" names
 *   below so reloc names match retail exactly; jump-table entries use
 *   (char*)&fn + off to emit addend relocs.
 * - .sdata: MemManager handle/flag globals + a run of 8-byte MWCC RTTI
 *   type-info objects {name-string ptr, 0-or-vtable ptr}.
 * - .sdata2: const float/double constants and small const strings (MWCC
 *   small-data placement).
 * - .bss/.sbss/.sbss2: NOBITS; sizes/alignments match retail (bss layout
 *   verified per-symbol against the retail object).
 *
 * Known residuals (cannot be expressed in legal C++/MWCC, see report):
 * - 5 template-destructor relocs in .data (names contain '<' '>'):
 *   __dt__12CMsgParam<8>Fv, __dt__23reslist<P11CWorkThread>Fv,
 *   __dt__29_reslist_base<P11CWorkThread>Fv, __dt__11reslist<Ul>Fv,
 *   __dt__17_reslist_base<Ul>Fv -> placeholder 0 words (bytes already 0).
 * - .bss @LOCAL@getInstance__Q22ml6MTRandFv@instance_806561E0 (contains '@',
 *   illegal identifier; MWCC has no asm-label support) -> placeholder-named
 *   0x9D0 blob at the same size/alignment; coordinator may redefine-sym.
 */

#pragma force_active on

/* ------------------------------------------------------------------ */
/* Retail-mangled extern references (reloc names must match exactly). */
/* ------------------------------------------------------------------ */
extern "C" {
    /* CWorkThread (monolib/work/CWorkThread.hpp) */
    void  wkStandby__11CWorkThreadFv(void);
    void  wkUpdate__11CWorkThreadFv(void);
    void  wkRender__11CWorkThreadFv(void);
    void  wkRenderAfter__11CWorkThreadFv(void);
    bool  wkStandbyLogin__11CWorkThreadFv(void);
    bool  wkStandbyLogout__11CWorkThreadFv(void);
    bool  wkStandbyExceptionRetry__11CWorkThreadFUl(unsigned long);
    void  __dt__11CWorkThreadFv(void);
    unsigned int __RTTI__11CWorkThread;

    /* IWorkEvent (monolib/work/IWorkEvent.hpp) */
    bool  WorkEvent1__10IWorkEventFPvPCc(void*, const char*);
    bool  OnFileEvent__10IWorkEventFP10CEventFile(void*);
    bool  WorkEvent3__10IWorkEventFPv(void*);
    bool  WorkEvent4__10IWorkEventFv(void);
    void  OnPauseTrigger__10IWorkEventFb(bool);
    bool  WorkEvent6__10IWorkEventFv(void);
    bool  WorkEvent7__10IWorkEventFv(void);
    bool  WorkEvent8__10IWorkEventFv(void);
    bool  WorkEvent9__10IWorkEventFv(void);
    bool  WorkEvent10__10IWorkEventFv(void);
    bool  WorkEvent11__10IWorkEventFv(void);
    bool  WorkEvent12__10IWorkEventFv(void);
    bool  WorkEvent13__10IWorkEventFv(void);
    bool  WorkEvent14__10IWorkEventFv(void);
    bool  WorkEvent15__10IWorkEventFv(void);
    bool  WorkEvent16__10IWorkEventFv(void);
    bool  WorkEvent17__10IWorkEventFv(void);
    bool  WorkEvent18__10IWorkEventFv(void);
    bool  WorkEvent19__10IWorkEventFv(void);
    bool  WorkEvent20__10IWorkEventFv(void);
    bool  WorkEvent21__10IWorkEventFv(void);
    bool  WorkEvent22__10IWorkEventFv(void);
    bool  WorkEvent23__10IWorkEventFv(void);
    bool  WorkEvent24__10IWorkEventFv(void);
    bool  WorkEvent25__10IWorkEventFv(void);
    bool  WorkEvent26__10IWorkEventFv(void);
    bool  WorkEvent27__10IWorkEventFv(void);
    bool  WorkEvent28__10IWorkEventFv(void);
    bool  WorkEvent29__10IWorkEventFv(void);
    bool  WorkEvent30__10IWorkEventFv(void);
    void  WorkEvent31__10IWorkEventFv(void);
    unsigned int __RTTI__10IWorkEvent;

    /* CProc (monolib/core/CProc.hpp) */
    void  __dt__5CProcFv(void);
    bool  wkStandbyLogin__5CProcFv(void);
    bool  wkStandbyLogout__5CProcFv(void);
    unsigned int __RTTI__5CProc;

    /* CProcRoot (monolib/core/CProcRoot.hpp) */
    void  __dt__9CProcRootFv(void);
    bool  wkStandbyLogin__9CProcRootFv(void);
    bool  wkStandbyLogout__9CProcRootFv(void);

    /* CRsrcData (monolib/core/CRsrcData.hpp) */
    void  __dt__9CRsrcDataFv(void);
    void  wkUpdate__9CRsrcDataFv(void);
    bool  wkStandbyLogin__9CRsrcDataFv(void);
    bool  wkStandbyLogout__9CRsrcDataFv(void);

    /* CScriptCode (monolib/core/CScriptCode.hpp) */
    void  __dt__11CScriptCodeFv(void);
    bool  wkStandbyLogout__11CScriptCodeFv(void);

    /* CToken (monolib/core/CToken.hpp) + recovered symbol */
    void  __dt__6CTokenFv(void);
    void  func_8043ABD4__6CTokenFPCcPCc(const char*, const char*);

    /* Data owned by other units (vtable/type-info objects of other classes) */
    extern unsigned char lbl_eu_8056B55C[];
    extern unsigned char lbl_eu_8056B5A4[];
    extern unsigned char lbl_eu_8056B6B0[];
    extern unsigned char lbl_eu_8056B6E4[];
    extern unsigned char lbl_eu_8056B7B0[];
    extern unsigned char lbl_eu_8056B868[];
    extern unsigned char lbl_eu_8056B920[];
    extern unsigned char lbl_eu_8056B9D8[];
    extern unsigned char lbl_eu_8056BA90[];
    extern unsigned char lbl_eu_8056BB48[];
    extern unsigned char lbl_eu_8056BC60[];
    extern unsigned char lbl_eu_8056BD20[];
    extern unsigned char lbl_eu_8056BDD8[];
}

typedef unsigned int u32;

/* ------------------------------------------------------------------ */
/* .rodata                                                             */
/* ------------------------------------------------------------------ */
/* ProgArea debug banner strings; "." folded in (retail: separate 2-byte
 * .rodata object at +0x48 that would land in .sdata2 if standalone). */
const char lbl_eu_80522410[0x4A] =
    "ProgArea (%x / %x)\n\000ProgSizeOver/*\203v\203\215\203O\203\211\203\200"
    "\227\314\210\346\202\252\214\300\212E\202\360\222\264\202\246\202\334\202"
    "\265\202\275*/\n\000\000\000.";

/* Align 8: retail gap 0x4A..0x50 (6 bytes) before this string. */
#pragma align 8
const char lbl_eu_80522460[0x11] = "WorkThreadSystem";
#pragma align 4

const char lbl_eu_80522474[0xD] = "CMsgParam<8>";
const char lbl_eu_80522484[0x17] = "reslist<CWorkThread *>";
/* Trailing gap + " " + trailing gap folded in (retail " " is a standalone
 * 2-byte .rodata object at +0xB0 that would land in .sdata2 if standalone). */
const char lbl_eu_8052249C[0x2C] =
    "_reslist_base<CWorkThread *>\000\000\000\000\000\000\000\000 "
    "\000\000\000\000\000\000";
const char lbl_eu_805224C8[0x17] = "reslist<unsigned long>";
const char lbl_eu_805224E0[0x1D] = "_reslist_base<unsigned long>";
const char lbl_eu_80522500[0x7] = "(View)";
const char lbl_eu_80522508[0xA] = "CProcRoot";
const char lbl_eu_80522514[0x14] = "CDesktop\000CProcRoot\000";
const char lbl_eu_80522528[0xA] = "CRsrcData";
const char lbl_eu_80522534[0xA] = "CRsrcData";
const char lbl_eu_80522540[0xC] = "CScriptCode";
const char lbl_eu_8052254C[0xC] = "CScriptCode";
/* Event-id -> wii-menu flag lookup table (CTaskManager). */
const u32 lbl_eu_80522558[0xC] = {
    0x00000004u, 0x00000002u, 0x00000002u, 0x00000002u,
    0x00000003u, 0x00000003u, 0x00000003u, 0x00000003u,
    0x00000003u, 0x00000005u, 0x00000002u, 0x00000000u,
};
const char lbl_eu_80522588[0x26] = "@unnamed@CTaskManager_cpp@::CRootProc";
const char lbl_eu_805225B0[0x2E] =
    "CTTask<@unnamed@CTaskManager_cpp@::CRootProc>";
const char lbl_eu_805225E0[0xE] = "CMsgParam<10>";
const char lbl_eu_805225F0[0x16] = "reslist<IWorkEvent *>";
const char lbl_eu_80522608[0x1C] = "_reslist_base<IWorkEvent *>";
const char lbl_eu_80522624[0xB] = "CFontLayer";
/* Shift-JIS error string ("ログアウトに失敗しました") + pad. */
const char lbl_eu_80522630[0x20] =
    "\203\215\203O\203A\203E\203g\202\311\216\270\224s\202\265\202\334\202\265"
    "\202\275\000\000\000\000\000\000\000";
const char lbl_eu_80522650[0xB] = "CViewFrame";
const char lbl_eu_80522660[0xA] = "CViewRoot";
const char lbl_eu_8052266C[0xA] = "CViewRoot";
const char lbl_eu_80522678[0xD] = "CWorkControl";
/* Work-flow class-name table (CWorkControl task flow). */
const char lbl_eu_80522688[0x80] =
    "CWorkFlowTvMode\000CWorkFlowSetup\000CWorkFlowShutdownAll\000CWorkFlowWiiMenu"
    "\000CWorkFlowWiiReset\000CWorkFlowWiiPowerOff\000CWorkControl"
    "\000\000\000\000\000\000\000";
const char lbl_eu_80522708[0xF] = "CWorkFlowSetup";
const char lbl_eu_80522718[0x29] = "@unnamed@CWorkRoot_cpp@::CWorkRootThread";
/* CWorkRoot exit-dialog strings. */
const char lbl_eu_80522744[0x4C] =
    "CWorkRoot\000exit wii menu\n\000exit wii reset\n\000exit wii power off\n\000"
    "exit prog end\n";
const char lbl_eu_80522790[0xC] = "CWorkSystem";
/* CWorkSystem exit/message strings (incl. Shift-JIS log-out error). */
const char lbl_eu_8052279C[0x7C] =
    "exit wii power off\n\000exit wii reset\n\000CWorkSystemMem\000"
    "CWorkSystemCache\000CWorkSystemPack\000"
    "\203\215\203O\203A\203E\203g\202\311\216\270\224s\202\265\202\334\202\265"
    "\202\275\000CWorkSystem\000\000\000";
const char lbl_eu_80522818[0xF] = "CWorkSystemMem";
const char lbl_eu_80522828[0x1B] = "TChildListHeader<CProcess>";

/* Align 8: retail gap 0x433..0x438 (5 bytes) before this string. */
#pragma align 8
const char lbl_eu_80522848[0x21] = "TChildListHeader<CChildListNode>";
#pragma align 4

/* Device event-bit lookup tables. */
#pragma align 8
const u32 lbl_eu_80522870[0x1C] = {
    0x00000001u, 0x00000001u, 0x00000002u, 0x00000002u, 0x00000008u, 0x00000004u,
    0x00000004u, 0x00000008u, 0x00000800u, 0x00000010u, 0x00000400u, 0x00000020u,
    0x00000200u, 0x00000040u, 0x00000100u, 0x00000080u, 0x00008000u, 0x00000100u,
    0x00000010u, 0x00000200u, 0x00001000u, 0x00000400u, 0x00002000u, 0x00001000u,
    0x00004000u, 0x00000800u, 0x00000000u, 0x00000000u,
};
#pragma align 4
const u32 lbl_eu_805228E0[0x20] = {
    0x00000002u, 0x00000001u, 0x00008000u, 0x00000002u, 0x00000001u, 0x00000004u,
    0x00004000u, 0x00000008u, 0x00000800u, 0x00000100u, 0x00000400u, 0x00000200u,
    0x00001000u, 0x00000400u, 0x00000010u, 0x00200000u, 0x00000040u, 0x00400000u,
    0x00000008u, 0x00800000u, 0x00000020u, 0x01000000u, 0x00002000u, 0x02000000u,
    0x00000200u, 0x04000000u, 0x00000080u, 0x08000000u, 0x00000004u, 0x10000000u,
    0x00000000u, 0x00000000u,
};
const char lbl_eu_80522960[0x11] = "CDeviceRemotePad";
const char lbl_eu_80522974[0xC] = "CDeviceBase";
const char lbl_eu_80522980[0xA] = "CDeviceSC";

/* ------------------------------------------------------------------ */
/* Forward declarations for .data references                            */
/* ------------------------------------------------------------------ */
/* 8-byte MWCC RTTI type-info object: { class-name string, 0/vtable }. */
struct MonolibRttiInfo {
    const char* name;
    const void* extra;
};

extern MonolibRttiInfo lbl_eu_80663520;
extern MonolibRttiInfo lbl_eu_80663528;
extern MonolibRttiInfo lbl_eu_80663530;
extern MonolibRttiInfo lbl_eu_80663538;
extern MonolibRttiInfo lbl_eu_80663540;
extern MonolibRttiInfo lbl_eu_80663548;
extern MonolibRttiInfo lbl_eu_80663550;
extern MonolibRttiInfo lbl_eu_80663558;
extern MonolibRttiInfo lbl_eu_80663560;

extern const char lbl_eu_8066A288[];
extern const char lbl_eu_8066A290[];
extern const char lbl_eu_8066A294[];
extern const char lbl_eu_8066A298[];
extern const char lbl_eu_8066A29C[];
extern const char lbl_eu_8066A2A0[];
extern const char lbl_eu_8066A2A4[];
extern const char lbl_eu_8066A2A8[];
extern const char lbl_eu_8066A2AC[];
extern const char lbl_eu_8066A2B0[];
extern const char lbl_eu_8066A2B8[];

/* ------------------------------------------------------------------ */
/* .data                                                               */
/* ------------------------------------------------------------------ */
#define IWE_SLOTS \
    (u32)&WorkEvent1__10IWorkEventFPvPCc, \
    (u32)&OnFileEvent__10IWorkEventFP10CEventFile, \
    (u32)&WorkEvent3__10IWorkEventFPv, \
    (u32)&WorkEvent4__10IWorkEventFv, \
    (u32)&OnPauseTrigger__10IWorkEventFb, \
    (u32)&WorkEvent6__10IWorkEventFv, \
    (u32)&WorkEvent7__10IWorkEventFv, \
    (u32)&WorkEvent8__10IWorkEventFv, \
    (u32)&WorkEvent9__10IWorkEventFv, \
    (u32)&WorkEvent10__10IWorkEventFv, \
    (u32)&WorkEvent11__10IWorkEventFv, \
    (u32)&WorkEvent12__10IWorkEventFv, \
    (u32)&WorkEvent13__10IWorkEventFv, \
    (u32)&WorkEvent14__10IWorkEventFv, \
    (u32)&WorkEvent15__10IWorkEventFv, \
    (u32)&WorkEvent16__10IWorkEventFv, \
    (u32)&WorkEvent17__10IWorkEventFv, \
    (u32)&WorkEvent18__10IWorkEventFv, \
    (u32)&WorkEvent19__10IWorkEventFv, \
    (u32)&WorkEvent20__10IWorkEventFv, \
    (u32)&WorkEvent21__10IWorkEventFv, \
    (u32)&WorkEvent22__10IWorkEventFv, \
    (u32)&WorkEvent23__10IWorkEventFv, \
    (u32)&WorkEvent24__10IWorkEventFv, \
    (u32)&WorkEvent25__10IWorkEventFv, \
    (u32)&WorkEvent26__10IWorkEventFv, \
    (u32)&WorkEvent27__10IWorkEventFv, \
    (u32)&WorkEvent28__10IWorkEventFv, \
    (u32)&WorkEvent29__10IWorkEventFv, \
    (u32)&WorkEvent30__10IWorkEventFv, \
    (u32)&WorkEvent31__10IWorkEventFv

/* CWorkThread::wkStandby switch jump table. */
u32 jumptable_eu_8056B0E0[0xC] = {
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0x194),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0x64),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0x74),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0x90),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0xA0),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0xB0),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0xD8),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0x100),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0x11C),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0x188),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0x138),
    (u32)((char *)&wkStandby__11CWorkThreadFv + 0x160),
};

/* CWorkThread vtable. */
u32 lbl_eu_8056B110[0x28] = {
    (u32)&__RTTI__11CWorkThread, 0,
    (u32)&__dt__11CWorkThreadFv,
    IWE_SLOTS,
    (u32)&wkUpdate__11CWorkThreadFv,
    (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv,
    (u32)&wkStandbyLogin__11CWorkThreadFv,
    (u32)&wkStandbyLogout__11CWorkThreadFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* CMsgParam<8> vtable: { RTTI, 0, dtor }. */
u32 lbl_eu_8056B1B0[0x3] = {
    (u32)&lbl_eu_80663520, 0,
    0, /* RESIDUAL: __dt__12CMsgParam<8>Fv (template name not expressible) */
};

/* reslist<CWorkThread*> vtable. */
u32 lbl_eu_8056B1BC[0x3] = {
    (u32)&lbl_eu_80663528, 0,
    0, /* RESIDUAL: __dt__23reslist<P11CWorkThread>Fv */
};

/* reslist<CWorkThread*> (base?) vtable; no dtor slot. */
u32 lbl_eu_8056B1C8[0x3] = {
    (u32)&lbl_eu_80663530, 0, 0,
};

/* _reslist_base<CWorkThread*> vtable. */
u32 lbl_eu_8056B1D4[0x3] = {
    (u32)&lbl_eu_80663530, 0,
    0, /* RESIDUAL: __dt__29_reslist_base<P11CWorkThread>Fv */
};

/* CProc vtable. */
u32 lbl_eu_8056B1E0[0x28] = {
    (u32)&__RTTI__5CProc, 0,
    (u32)&__dt__5CProcFv,
    IWE_SLOTS,
    (u32)&wkUpdate__11CWorkThreadFv,
    (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv,
    (u32)&wkStandbyLogin__5CProcFv,
    (u32)&wkStandbyLogout__5CProcFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* reslist<unsigned long> vtable. */
u32 lbl_eu_8056B280[0x3] = {
    (u32)&lbl_eu_80663538, 0,
    0, /* RESIDUAL: __dt__11reslist<Ul>Fv */
};

/* reslist<unsigned long> (base?) vtable; no dtor slot. */
u32 lbl_eu_8056B28C[0x3] = {
    (u32)&lbl_eu_80663540, 0, 0,
};

/* _reslist_base<unsigned long> vtable. */
u32 lbl_eu_8056B298[0x4] = {
    (u32)&lbl_eu_80663540, 0,
    0, /* RESIDUAL: __dt__17_reslist_base<Ul>Fv */
    0,
};

/* CProcRoot vtable. */
u32 lbl_eu_8056B2A8[0x28] = {
    (u32)&lbl_eu_80663548, 0,
    (u32)&__dt__9CProcRootFv,
    IWE_SLOTS,
    (u32)&wkUpdate__11CWorkThreadFv,
    (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv,
    (u32)&wkStandbyLogin__9CProcRootFv,
    (u32)&wkStandbyLogout__9CProcRootFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* CProcRoot RTTI base chain: {IWorkEvent RTTI, 0, CWorkThread RTTI, 0, 0, 0}. */
u32 lbl_eu_8056B348[0x6] = {
    (u32)&__RTTI__10IWorkEvent, 0,
    (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

/* CRsrcData vtable. */
u32 lbl_eu_8056B360[0x28] = {
    (u32)&lbl_eu_80663550, 0,
    (u32)&__dt__9CRsrcDataFv,
    IWE_SLOTS,
    (u32)&wkUpdate__9CRsrcDataFv,
    (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv,
    (u32)&wkStandbyLogin__9CRsrcDataFv,
    (u32)&wkStandbyLogout__9CRsrcDataFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* CRsrcData RTTI base chain. */
u32 lbl_eu_8056B400[0x6] = {
    (u32)&__RTTI__10IWorkEvent, 0,
    (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

/* CScriptCode vtable. */
u32 lbl_eu_8056B418[0x28] = {
    (u32)&lbl_eu_80663558, 0,
    (u32)&__dt__11CScriptCodeFv,
    IWE_SLOTS,
    (u32)&wkUpdate__11CWorkThreadFv,
    (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv,
    (u32)&wkStandbyLogin__11CWorkThreadFv,
    (u32)&wkStandbyLogout__11CScriptCodeFv,
    (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};

/* CScriptCode RTTI base chain. */
u32 lbl_eu_8056B4B8[0x6] = {
    (u32)&__RTTI__10IWorkEvent, 0,
    (u32)&__RTTI__11CWorkThread, 0, 0, 0,
};

/* CToken print-format-string table. */
u32 lbl_eu_8056B4D0[0xC] = {
    (u32)&lbl_eu_8066A288, (u32)&lbl_eu_8066A290, (u32)&lbl_eu_8066A294,
    (u32)&lbl_eu_8066A298, (u32)&lbl_eu_8066A29C, (u32)&lbl_eu_8066A2A0,
    (u32)&lbl_eu_8066A2A4, (u32)&lbl_eu_8066A2A8, (u32)&lbl_eu_8066A2AC,
    (u32)&lbl_eu_8066A2B0, (u32)&lbl_eu_8066A2B8, 0,
};

/* CToken::func_8043ABD4 switch jump table. */
u32 jumptable_eu_8056B500[0xB] = {
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x960),
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x164),
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x1E0),
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x258),
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x2D0),
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x960),
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x3CC),
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x508),
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x628),
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x794),
    (u32)((char *)&func_8043ABD4__6CTokenFPCcPCc + 0x8C0),
};

/* CToken vtable. */
u32 lbl_eu_8056B52C[0x3] = {
    (u32)&lbl_eu_80663560, 0,
    (u32)&__dt__6CTokenFv,
};

/* ------------------------------------------------------------------ */
/* .sdata                                                              */
/* ------------------------------------------------------------------ */
u32 lbl_eu_80663500 = (u32)&lbl_eu_8066A1A8;              /* &"Mem1"   (MEM1 region name) */
u32 lbl_eu_80663504 = (u32)&lbl_eu_8066A1B0;              /* &"Mem2"   (MEM2 region name) */
u32 lbl_eu_80663508 = 0xFFFFFFFFu;                        /* MemManager::sHandleMEM1 */
u32 lbl_eu_8066350C = 0xFFFFFFFFu;                        /* MemManager::sHandleMEM2 */
unsigned char lbl_eu_80663510 = 1;                        /* MemManager::lbl_80663510 */
unsigned char lbl_eu_80663511 = 1;                        /* MemManager::lbl_80663511 */
u32 lbl_eu_80663514 = 0x00600000u;                        /* MEM2 region size (6MB) */
u32 lbl_eu_80663518 = (u32)&lbl_eu_80522460;              /* &"WorkThreadSystem" */
u32 lbl_eu_8066351C = 0xFFFFFFFFu;                        /* -1 */

MonolibRttiInfo lbl_eu_80663520 = { lbl_eu_80522474, 0 };                       /* CMsgParam<8> */
MonolibRttiInfo lbl_eu_80663528 = { lbl_eu_80522484, lbl_eu_8056B1C8 };         /* reslist<CWorkThread*> */
MonolibRttiInfo lbl_eu_80663530 = { lbl_eu_8052249C, 0 };                       /* _reslist_base<CWorkThread*> */
MonolibRttiInfo lbl_eu_80663538 = { lbl_eu_805224C8, lbl_eu_8056B28C };         /* reslist<unsigned long> */
MonolibRttiInfo lbl_eu_80663540 = { lbl_eu_805224E0, 0 };                       /* _reslist_base<unsigned long> */
MonolibRttiInfo lbl_eu_80663548 = { lbl_eu_80522508, lbl_eu_8056B348 };         /* CProcRoot */
MonolibRttiInfo lbl_eu_80663550 = { lbl_eu_80522528, lbl_eu_8056B400 };         /* CRsrcData */
MonolibRttiInfo lbl_eu_80663558 = { lbl_eu_80522540, lbl_eu_8056B4B8 };         /* CScriptCode */
MonolibRttiInfo lbl_eu_80663560 = { lbl_eu_8066A2C8, 0 };                       /* CToken */
MonolibRttiInfo lbl_eu_80663568 = { lbl_eu_80522588, lbl_eu_8056B55C };         /* @unnamed@CTaskManager_cpp@::CRootProc */
MonolibRttiInfo lbl_eu_80663570 = { lbl_eu_805225B0, lbl_eu_8056B5A4 };         /* CTTask<CRootProc> */
MonolibRttiInfo lbl_eu_80663578 = { lbl_eu_8066A2D8, lbl_eu_8056B6B0 };         /* CView */
MonolibRttiInfo lbl_eu_80663580 = { lbl_eu_805225E0, 0 };                       /* CMsgParam<10> */
MonolibRttiInfo lbl_eu_80663588 = { lbl_eu_805225F0, lbl_eu_8056B6E4 };         /* reslist<IWorkEvent*> */
MonolibRttiInfo lbl_eu_80663590 = { lbl_eu_80522608, 0 };                       /* _reslist_base<IWorkEvent*> */
MonolibRttiInfo lbl_eu_80663598 = { lbl_eu_80522624, 0 };                       /* CFontLayer */
MonolibRttiInfo lbl_eu_806635A0 = { lbl_eu_80522650, 0 };                       /* CViewFrame */
MonolibRttiInfo lbl_eu_806635A8 = { lbl_eu_80522660, lbl_eu_8056B7B0 };         /* CViewRoot */
MonolibRttiInfo lbl_eu_806635B0 = { lbl_eu_80522678, lbl_eu_8056B868 };         /* CWorkControl */
MonolibRttiInfo lbl_eu_806635B8 = { lbl_eu_80522708, lbl_eu_8056B920 };         /* CWorkFlowSetup */
MonolibRttiInfo lbl_eu_806635C0 = { lbl_eu_80522718, lbl_eu_8056B9D8 };         /* @unnamed@CWorkRoot_cpp@::CWorkRootThread */
MonolibRttiInfo lbl_eu_806635C8 = { lbl_eu_80522790, lbl_eu_8056BA90 };         /* CWorkSystem */
MonolibRttiInfo lbl_eu_806635D0 = { lbl_eu_80522818, lbl_eu_8056BB48 };         /* CWorkSystemMem */
MonolibRttiInfo lbl_eu_806635D8 = { lbl_eu_80522828, 0 };                       /* TChildListHeader<CProcess> */
MonolibRttiInfo lbl_eu_806635E0 = { lbl_eu_80522848, 0 };                       /* TChildListHeader<CChildListNode> */
MonolibRttiInfo lbl_eu_806635E8 = { lbl_eu_80522960, lbl_eu_8056BC60 };         /* CDeviceRemotePad */
MonolibRttiInfo lbl_eu_806635F0 = { lbl_eu_80522974, lbl_eu_8056BD20 };         /* CDeviceBase */
MonolibRttiInfo lbl_eu_806635F8 = { lbl_eu_80522980, lbl_eu_8056BDD8 };         /* CDeviceSC */

/* ------------------------------------------------------------------ */
/* .sbss (forward declaration order; retail symbol offsets verified)    */
/* ------------------------------------------------------------------ */
unsigned long long lbl_eu_80665550;    /* 8 bytes @0x00 */
unsigned char lbl_eu_80665558;         /* 1 byte  @0x08 */
unsigned char gap_10_80665559_sbss[3]; /* retail 3-byte gap @0x09 */
unsigned char lbl_eu_8066555C;         /* 1 byte  @0x0C */
unsigned char lbl_eu_8066555D;         /* 1 byte  @0x0D */
unsigned char lbl_eu_8066555E;         /* 1 byte  @0x0E */
unsigned char lbl_eu_8066555F;         /* 1 byte  @0x0F */
u32 lbl_eu_80665560;                   /* 4 bytes @0x10 */
u32 lbl_eu_80665564;                   /* 4 bytes @0x14 */
unsigned char lbl_eu_80665568;         /* 1 byte  @0x18 */
unsigned char lbl_eu_80665569[7];      /* 7 bytes @0x19 */
u32 lbl_eu_80665570;                   /* 4 bytes @0x20 */
unsigned char lbl_eu_80665574;         /* 1 byte  @0x24 */
unsigned char gap_10_80665575_sbss[3]; /* retail 3-byte gap @0x25 */
u32 lbl_eu_80665578;                   /* 4 bytes @0x28 */
unsigned char lbl_eu_8066557C;         /* 1 byte  @0x2C (MemManager flag) */

/* ------------------------------------------------------------------ */
/* .sdata2 (const small data)                                          */
/* ------------------------------------------------------------------ */
const float lbl_eu_8066A178 = 0.5f;
const float lbl_eu_8066A17C = 40.7436637878418f;
const float lbl_eu_8066A180 = 1.0f;
const float lbl_eu_8066A184 = -1.0f;
const float lbl_eu_8066A188 = 0.02454369328916073f;
const float lbl_eu_8066A18C = 0.0f;
const float lbl_eu_8066A190 = 40.7436637878418f;
const float lbl_eu_8066A194 = -1.0f;
const float lbl_eu_8066A198 = 1.0f;
const float lbl_eu_8066A19C = 0.02454369328916073f;
const float lbl_eu_8066A1A0[2] = { 0.0f, 0.0f };
const char lbl_eu_8066A1A8[0x5] = "Mem1";
const char lbl_eu_8066A1B0[0x5] = "Mem2";
const double lbl_eu_8066A1B8 = 1.0;
const float lbl_eu_8066A1C0 = 0.0f;
const float lbl_eu_8066A1C4 = 100.0f;
const double lbl_eu_8066A1C8 = 4503599627370496.0;
const float lbl_eu_8066A1D0 = 2.3283064365386963e-10f;
const double lbl_eu_8066A1D8 = 4503599627370496.0;
const float lbl_eu_8066A1E0[2] = { 0.5f, 0.0f };
const float lbl_eu_8066A1E8 = 0.0f;
const float lbl_eu_8066A1EC = 1.0f;
const float lbl_eu_8066A1F0 = 0.0f;
const float lbl_eu_8066A1F4 = 1.0f;
const float lbl_eu_8066A1F8 = 3.1415927410125732f;
const float lbl_eu_8066A1FC = 6.2831854820251465f;
const float lbl_eu_8066A200 = 1.5707963705062866f;
const float lbl_eu_8066A204 = 0.7853981852531433f;
const float lbl_eu_8066A208 = 9.999999974752427e-07f;
const float lbl_eu_8066A20C = 57.2957763671875f;
const float lbl_eu_8066A210 = 0.01745329238474369f;
const float lbl_eu_8066A214 = 100000000376832.0f;
const float lbl_eu_8066A218[2] = { -100000000376832.0f, 0.0f };
const float lbl_eu_8066A220 = 1.0f;
const float lbl_eu_8066A224 = 0.5f;
const float lbl_eu_8066A228[2] = { 0.0f, 0.0f };
const float lbl_eu_8066A230 = 1.0f;
const float lbl_eu_8066A234 = 0.5f;
const float lbl_eu_8066A238[2] = { 0.0f, 0.0f };
const float lbl_eu_8066A240 = 0.0f;
const float lbl_eu_8066A244 = 0.5f;
const float lbl_eu_8066A248 = 1.0f;
const float lbl_eu_8066A24C = -1.0f;
const float lbl_eu_8066A250[2] = { 0.02454369328916073f, 0.0f };
const float lbl_eu_8066A258 = 0.0f;
const float lbl_eu_8066A25C = 1.0f;
const float lbl_eu_8066A260[2] = { 0.0f, 0.0f };
const float lbl_eu_8066A268 = 40.7436637878418f;
const float lbl_eu_8066A26C = 0.0f;
const float lbl_eu_8066A270[2] = { 1.0f, 0.0f };
const float lbl_eu_8066A278 = 0.6000000238418579f;
const double lbl_eu_8066A280 = 4503601774854144.0;
const char lbl_eu_8066A288[0x5] = "????";
const char lbl_eu_8066A290[0x3] = "%f";
const char lbl_eu_8066A294[0x3] = "%d";
const char lbl_eu_8066A298[0x3] = "%x";
const char lbl_eu_8066A29C[0x4] = "%v3";
const char lbl_eu_8066A2A0[0x4] = "%va";
const char lbl_eu_8066A2A4[0x4] = "%v4";
const char lbl_eu_8066A2A8[0x4] = "%i3";
const char lbl_eu_8066A2AC[0x4] = "%i4";
const char lbl_eu_8066A2B0[0x6] = "%rect";
const char lbl_eu_8066A2B8[0x3] = "%s";
const double lbl_eu_8066A2C0 = 4503601774854144.0;
const char lbl_eu_8066A2C8[0x7] = "CToken";
const float lbl_eu_8066A2D0 = 1.0f;
const float lbl_eu_8066A2D4 = 0.6000000238418579f;
const char lbl_eu_8066A2D8[0x6] = "CView";
const float lbl_eu_8066A2E0 = 0.4000000059604645f;
const float lbl_eu_8066A2E4 = 0.5f;
const float lbl_eu_8066A2E8 = 0.0f;
const float lbl_eu_8066A2EC = 0.5f;
const float lbl_eu_8066A2F0 = 1.0f;
const float lbl_eu_8066A2F4 = 0.800000011920929f;
const float lbl_eu_8066A2F8 = 0.4000000059604645f;
const float lbl_eu_8066A2FC = 0.6000000238418579f;
const double lbl_eu_8066A300 = 4503601774854144.0;
const float lbl_eu_8066A308 = 0.1144770011305809f;
const float lbl_eu_8066A30C = 0.298911988735199f;
const float lbl_eu_8066A310 = 0.5866109728813171f;
const float lbl_eu_8066A314 = 0.699999988079071f;
const float lbl_eu_8066A318[2] = { 1.2000000476837158f, 0.0f };
const float lbl_eu_8066A320 = 0.15000000596046448f;
const float lbl_eu_8066A324 = 0.0f;
const float lbl_eu_8066A328 = 56.0f;
const float lbl_eu_8066A32C = 150.0f;
const float lbl_eu_8066A330 = 320.0f;
const float lbl_eu_8066A334 = 210.0f;
const double lbl_eu_8066A338 = 4503599627370496.0;
const double lbl_eu_8066A340 = 0.03333333333333333;
const float lbl_eu_8066A348 = 16666.666015625f;
const double lbl_eu_8066A350 = 4503599627370496.0;
const float lbl_eu_8066A358[2] = { 0.0f, 0.0f };

/* ------------------------------------------------------------------ */
/* .sbss2                                                              */
/* ------------------------------------------------------------------ */
#pragma align 8
const u32 lbl_8066DCF8 = 0;
#pragma align 4

/* ------------------------------------------------------------------ */
/* .bss                                                               */
/* ------------------------------------------------------------------ */
/* mtl::MemManager::sRegionBuffer (retail: mtl::RawArray<MemRegion, 0x230>
 * region array, 0x2300 bytes). */
unsigned long long sRegionBuffer__Q23mtl10MemManager[0x2300 / 8];

/* RESIDUAL: retail symbol is the function-local static
 * "@LOCAL@getInstance__Q22ml6MTRandFv@instance_806561E0" (ml::MTRand
 * singleton instance, 0x9D0 bytes). '@' is not a legal C++ identifier char
 * and MWCC has no asm-label support, so the exact name cannot be produced
 * from source; this placeholder keeps size/alignment identical. The
 * coordinator can add a link-time redefine-sym mapping. */
unsigned long long mtrand_instance_806561E0[0x9D0 / 8];
